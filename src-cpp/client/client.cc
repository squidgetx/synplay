#include "client.h"
#include <sndfile.hh>
#include "net/time_packet.h"
#include "util/syntime.h"

static int received = 0;

static int pacallback(const void *inputBuffer, void* outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData) {

  int16_t *audio_out = (int16_t*) outputBuffer;

  streamState * s_state = (streamState *) userData;

  std::deque<MPacket *> * packet_buffer = s_state->packet_buffer;

  PaTime play_time = timeInfo->outputBufferDacTime;

  for(unsigned long i = 0; i < framesPerBuffer; i++) {

    // Don't play anything if there are no packets to play
    if (packet_buffer->empty()) {
      *(audio_out++) = 0;
      *(audio_out++) = 0;
      continue;
    }

    MPacket * mp = packet_buffer->front();

    // Make sure timestamps are monotonically increasing
    // (discard packets that have a timestamp less than
    // the most recent timestamp we have seen)
    if (mp->get_pa_timestamp() < s_state->last_timestamp) {
      packet_buffer->pop_front();
      i--;
    } else {
      s_state->last_timestamp = mp->get_pa_timestamp();
    }


    // Don't play if it's not time to play this packet yet
    if (mp->get_pa_timestamp() >= play_time) {
      *(audio_out++) = 0;
      *(audio_out++) = 0;
      continue;
    }

    // Get the next packet if this one is finished
    if (mp->remaining() < 2) {
      packet_buffer->pop_front();
      i--;
      continue;
    }

    *(audio_out++) = mp->get_int16_t();
    *(audio_out++) = mp->get_int16_t();

  }

  return paContinue;
}

PaTime Client::get_pa_time(mtime_t master_time) {
  // Convert a master clock millisecond timestamp
  // to the equivalent Port Audio stream timestamp
  return (PaTime) (master_time + offset + pa_offset) / 1000.0;
}

void Client::receive() {
  socket.async_receive_from(
    asio::buffer(data, LEN), sender_endpoint,
    [this](std::error_code ec, std::size_t bytes_recvd)
    {

      // immediately grab the receipt time
      mtime_t to_recvd = get_millisecond_time();

      if (!ec && bytes_recvd > 0)
      {
        Packet *packet = Packet::unpack(data,bytes_recvd);
        if (packet != nullptr) {
          switch (packet->get_type()){
            case PacketType::TIME:
              receive_timesync(static_cast<TPacket *> (packet),to_recvd);
              break;
            case PacketType::DATA:
              receive_data(static_cast<MPacket *> (packet));
              break;
          }
        }
        receive();
      }
    }
  );
}

void Client::receive_data(MPacket *mpacket) {
  received++;
  // printf("%d\r", received);
  mpacket->set_pa_timestamp(get_pa_time(mpacket->get_timestamp()));
  s_state->packet_buffer->push_back(mpacket);
  // mpacket->print();

}

void Client::receive_timesync(TPacket *tpacket, mtime_t to_recvd) {
  tpacket->to_recvd = to_recvd;
  tpacket->to_sent = get_millisecond_time();

  if (tpacket->tp_type == COMPLETE) {
    offset = tpacket->offset;
    avg_offset += offset;
    avg_rounds += 1;
  } else if (tpacket->tp_type == FINAL) {
    std::cerr << "setting master/client offset: " << offset << "after " <<
      avg_rounds << " rounds" << std::endl;
    // Get offset between stream time and current time
    PaTime pa_start_time = Pa_GetStreamTime(stream);
    std::cerr << "Stream time: " << pa_start_time << std::endl;
    mtime_t system_start_time = get_millisecond_time();
    std::cerr << "System time: " << system_start_time << std::endl;
    pa_offset = ((mtime_t) (pa_start_time * 1000)) - system_start_time;
    std::cerr << "pa_offset: " << pa_offset << std::endl;
    offset = avg_offset / avg_rounds;
    avg_offset = 0;
    avg_rounds = 0;
  }

  // and send the reply
  socket.async_send_to(
    asio::buffer(tpacket->pack()), sender_endpoint,
    [this](std::error_code, std::size_t) {
      // reply sent
    }
  );
}

Client::Client(asio::io_service& io_service, uint16_t p) : port(p),
  socket(io_service, udp::endpoint(udp::v4(), p)) {
  std::cout << "Listening on " << port << std::endl;
  s_state = new streamState(100);
}

void Client::start() {
  PaHostApiIndex host_idx;
  PaDeviceIndex device_idx;
  PaTime latency;

  /* Set up Port Audio */
  PaError err = Pa_Initialize();
  if (err != paNoError) {
    fprintf( stderr, "An error occured while initializing the portaudio stream\n" );
    Pa_Terminate();
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return;

  }

  // Determine Device Index
  host_idx = Pa_HostApiTypeIdToHostApiIndex(paJACK);
  device_idx = Pa_GetDefaultOutputDevice();
  std::cerr << "Default device idx : " << device_idx << std::endl;
  if (host_idx != paHostApiNotFound) {
    device_idx = Pa_HostApiDeviceIndexToDeviceIndex(host_idx, 0);
    std::cerr << "JACK device idx : " << device_idx << std::endl;
    if (device_idx == paInvalidDevice) {
      device_idx = Pa_GetDefaultOutputDevice();
      std::cerr << "Unable to use JACK." << std::endl;
    }
  } else {
    std::cerr << "JACK not found" << std::endl;
  }

  const PaDeviceInfo * deviceInfo = Pa_GetDeviceInfo(device_idx);
  latency = deviceInfo->defaultLowOutputLatency;
  std::cerr << "Device sample rate: " << deviceInfo->defaultSampleRate << std::endl;

  PaStreamParameters output_parameters;
  output_parameters.device = device_idx;
  output_parameters.channelCount = 2;
  output_parameters.sampleFormat = paInt16;
  output_parameters.suggestedLatency = latency;
  output_parameters.hostApiSpecificStreamInfo = NULL;

  /* No input, 2 stereo out */
  err = Pa_OpenStream(&stream, NULL, &output_parameters,
      SAMPLE_RATE, 64, paNoFlag, pacallback, s_state);
  if (err != paNoError) {
    fprintf( stderr, "An error occured while opening the portaudio stream\n" );
    goto error;
  }

  err = Pa_StartStream(stream);
  if (err != paNoError) {
    fprintf( stderr, "An error occured while starting the portaudio stream\n" );
    goto error;
  }

  Pa_Sleep(1000);

  receive();

  //for (;;) {}
  //Pa_StopStream(stream);
  //Pa_CloseStream(stream);
  //Pa_Terminate();
  return;
error:
  Pa_Terminate();
  fprintf( stderr, "Error number: %d\n", err );
  fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
  return;

}


