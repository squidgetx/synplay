#include "client.h"

static int pacallback(const void *inputBuffer, void* outputBuffer,
    unsigned long framesPerBuffer, 
    const PaStreamCallbackTimeInfo* timeInfo, 
    PaStreamCallbackFlags statusFlags, 
    void *userData) {

  char *audio_out = (char*) outputBuffer;
  RingBuffer<char> * play_buffer = (RingBuffer<char> *) userData;
  // copy as much from the byte buffer to the 
  // audio out
  bool endl = false;
  for(int i = 0; i < framesPerBuffer; i++) {
    *audio_out = play_buffer->get();
    if (*audio_out != 0) {
      std::cout << *audio_out;
      endl = true;
    }
    audio_out++;
  }

  if (endl) {
    std::cout << std::endl;
  }

  return paContinue;

}

void Client::receive() {
  socket.async_receive_from(
    asio::buffer(data, 1024), sender_endpoint,
    [this](std::error_code ec, std::size_t bytes_recvd)
    {
      if (!ec && bytes_recvd > 0)
      {
        std::cout << "got " << bytes_recvd << "bytes" << std::endl;
        MPacket * mpacket = MPacket::unpack(data, bytes_recvd);
        mpacket->print();
        // For now just put everything in the play buffer
       // packet_buffer.put(mpacket);
        play_buffer.putn(mpacket->get_payload(), mpacket->get_payload_size());
      }
      receive();
    });
}

Client::Client(asio::io_service& io_service, int p) : port(p), packet_buffer(100), play_buffer(4096),
  socket(io_service, udp::endpoint(udp::v4(), p)) {
  std::cout << "Listening on " << port << std::endl;
}

void Client::start() {

  /* Set up Port Audio */
  PaError err = Pa_Initialize();
  if (err != paNoError) goto error;
  PaStream *stream;

  /* No input, 2 stereo out */
  err = Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, SAMPLE_RATE, 4, pacallback, &play_buffer);
  if (err != paNoError) goto error;

  receive();
 
  err = Pa_StartStream(stream);
  if (err != paNoError) goto error;


  //Pa_Sleep(100000);

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


