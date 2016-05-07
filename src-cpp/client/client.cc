#include "client.h"
#include <sndfile.hh>
#include "net/time_packet.h"
#include "util/syntime.h"

volatile mtime_offset_t offset;

static mtime_t get_master_clock_millis() {
  return get_millisecond_time() - offset;
}

static int received = 0;
volatile PaTime start_t = 0;
volatile int state = 0;

static int pacallback(const void *inputBuffer, void* outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData) {
  int16_t *audio_out = (int16_t*) outputBuffer;
  if (state == 0) {
    PaTime play_time = timeInfo->outputBufferDacTime;
  //  std::cerr << "Current time: " << timeInfo->currentTime << " Start t: " << start_t << std::endl;
    if (start_t == 0 || start_t >= play_time) {
      for(unsigned long i = 0; i < framesPerBuffer; i++) {
          *(audio_out++) = 0;
          *(audio_out++) = 0;
      }
      return paContinue;
    }
    state = 1;
  }

  std::deque<int16_t> * play_buffer = (std::deque<int16_t> *) userData;
  // copy as much from the byte buffer to the
  // audio outputBuffer
  for(unsigned long i = 0; i < framesPerBuffer; i++) {
    if (play_buffer->size() < 2) {
      *(audio_out++) = 0;
      *(audio_out++) = 0;
      continue;
    }
    *(audio_out++) = play_buffer->front();
   // printf("%i ", play_buffer->front());
    play_buffer->pop_front();
    *(audio_out++) = play_buffer->front();
   // printf("%i ", play_buffer->front());
    play_buffer->pop_front();
  }
 // printf("\n");

  return paContinue;
}

void Client::receiveFromFile() {
  sf_count_t num_read = file.read(file_buf, BUFFER_LENGTH);
  play_buffer.insert(play_buffer.end(), file_buf, file_buf + num_read);
  if (num_read == 0)
    return;

  receiveFromFile();
}

void Client::receive() {
  socket.async_receive_from(
    asio::buffer(data, LEN), sender_endpoint,
    [this](std::error_code ec, std::size_t bytes_recvd)
    {

      // immediately grab the receipt time
      mtime_t to_recvd = get_millisecond_time();

      received++;
      if (!ec && bytes_recvd > 0)
      {
        Packet *packet = Packet::unpack(data,bytes_recvd);
        switch (packet->get_type()){
          case TIME:
            receive_timesync(static_cast<TPacket *> (packet),to_recvd);
            break;
          case DATA:
            receive_data(static_cast<MPacket *> (packet));
            break;
        }
        receive();
      }
    }
  );
}

void Client::receive_data(MPacket *mpacket) {
        //mpacket->print();
        play_buffer.insert(play_buffer.end(), mpacket->get_payload(), mpacket->get_payload() + mpacket->get_payload_size());
        if (start_t == 0) {
          // convert this to stream time
          start_t  = (PaTime) (mpacket->get_timestamp() + offset + pa_offset) / 1000;
          std::cerr << "Start_t set to " << start_t << std::endl;
        }
}

void Client::receive_timesync(TPacket *tpacket, mtime_t to_recvd) {
        tpacket->to_recvd = to_recvd;
        tpacket->to_sent = get_millisecond_time();

        if (tpacket->tp_type == COMPLETE) {
          offset = tpacket->offset;
          std::cerr << "setting master/client offset: " << offset << std::endl;
          // Get offset between stream time and current time
          PaTime pa_start_time = Pa_GetStreamTime(stream);
          std::cerr << "Stream time: " << pa_start_time << std::endl;
          mtime_t system_start_time = get_millisecond_time();
          pa_offset = ((mtime_t) pa_start_time * 1000) - system_start_time;
          std::cerr << "pa_offset: " << pa_offset << std::endl;
        }

        // and send the reply
        socket.async_send_to(
            asio::buffer(tpacket->pack()), sender_endpoint,
            [this](std::error_code, std::size_t) {
              // reply sent
            }
        );
}

Client::Client(asio::io_service& io_service, uint16_t p) : port(p), packet_buffer(100),
  socket(io_service, udp::endpoint(udp::v4(), p)) {
  file = SndfileHandle("../yellow.wav");
  std::cout << file.samplerate() << " " << file.channels() << std::endl;
  std::cout << "Listening on " << port << std::endl;
}

void Client::start() {
  PaHostApiIndex host_idx;
  PaDeviceIndex device_idx;
  PaTime latency;

  /* Set up Port Audio */
  PaError err = Pa_Initialize();
  if (err != paNoError) goto error;

  // Determine Device Index
  host_idx = Pa_HostApiTypeIdToHostApiIndex(paJACK);
  device_idx = Pa_GetDefaultOutputDevice();
  if (host_idx != paHostApiNotFound) {
    device_idx = Pa_HostApiDeviceIndexToDeviceIndex(host_idx, 0);
    if (device_idx == paInvalidDevice) {
      device_idx = Pa_GetDefaultOutputDevice();
      std::cerr << "Unable to use JACK." << std::endl;
    }
  }

  latency = Pa_GetDeviceInfo(device_idx)->defaultLowOutputLatency;

  PaStreamParameters output_parameters;
  output_parameters.device = device_idx;
  output_parameters.channelCount = 2;
  output_parameters.sampleFormat = paInt16;
  output_parameters.suggestedLatency = latency;
  output_parameters.hostApiSpecificStreamInfo = NULL;

  /* No input, 2 stereo out */
  err = Pa_OpenStream(&stream, NULL, &output_parameters,
      SAMPLE_RATE, 64, paNoFlag, pacallback, &play_buffer);
  if (err != paNoError) goto error;

  err = Pa_StartStream(stream);
  if (err != paNoError) goto error;

  Pa_Sleep(1000);

  receive();//FromFile();


   // when we receive a timestamp, add pa_offset then convert to PaTime


  //for (;;) {}
  //Pa_StopStream(stream);
  //Pa_CloseStream(stream);
  //Pa_Terminate();
  return;
error:
  Pa_Terminate();
  fprintf( stderr, "An error occured while using the portaudio stream\n" );
  fprintf( stderr, "Error number: %d\n", err );
  fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
  return;

}


