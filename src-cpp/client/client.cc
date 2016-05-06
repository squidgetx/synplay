#include "client.h"
#include <sndfile.hh>

static int pacallback(const void *inputBuffer, void* outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData) {

  int16_t *audio_out = (int16_t*) outputBuffer;
  std::deque<int16_t> * play_buffer = (std::deque<int16_t> *) userData;
  // copy as much from the byte buffer to the
  // audio out
  for(unsigned long i = 0; i < framesPerBuffer; i++) {
    if (play_buffer->size() < 2) {
      *(audio_out++) = 0;
      *(audio_out++) = 0;
    } else {
      *(audio_out++) = play_buffer->front();
      play_buffer->pop_front();
      *(audio_out++) = play_buffer->front();
      play_buffer->pop_front();
    }
  }


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
    asio::buffer(data, 1024), sender_endpoint,
    [this](std::error_code ec, std::size_t bytes_recvd)
    {
      if (!ec && bytes_recvd > 0)
      {
        std::cout << "got " << bytes_recvd << "bytes" << std::endl;
        for(std::size_t i = 0; i < bytes_recvd; i++) {
          printf("%02x ", data[i]);
        }
        std::cout << std::endl;
        MPacket * mpacket = MPacket::unpack(data, bytes_recvd);
        mpacket->print();
        // For now just put everything in the play buffer
       // packet_buffer.put(mpacket);
        play_buffer.insert(play_buffer.end(), mpacket->get_payload(), mpacket->get_payload() + mpacket->get_payload_size());
      }
      receive();
    });
}

Client::Client(asio::io_service& io_service, uint16_t p) : port(p), packet_buffer(100),
  socket(io_service, udp::endpoint(udp::v4(), p)) {
  file = SndfileHandle("../yellow.wav");
  std::cout << file.samplerate() << " " << file.channels() << std::endl;
  std::cout << "Listening on " << port << std::endl;
}

void Client::start() {

  /* Set up Port Audio */
  PaError err = Pa_Initialize();
  if (err != paNoError) goto error;
  PaStream *stream;

  /* No input, 2 stereo out */
  err = Pa_OpenDefaultStream(&stream, 0, 2, paInt16, SAMPLE_RATE, 64, pacallback, &play_buffer);
  if (err != paNoError) goto error;

  receive();//FromFile();

  err = Pa_StartStream(stream);
  if (err != paNoError) goto error;


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


