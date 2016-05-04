#include "client.h"

static int pacallback(const void *in, void* out, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData) {

}

void Client::receive() {
  socket.async_receive_from(
    asio::buffer(data, 1024), sender_endpoint,
    [this](std::error_code ec, std::size_t bytes_recvd)
    {
      if (!ec && bytes_recvd > 0)
      {
        MPacket * mpacket = MPacket::unpack(data, bytes_recvd);
        packet_buffer.put(mpacket);
      }
      receive();
    });
}

Client::Client(asio::io_service& io_service, int p) : port(p), packet_buffer(100),  
  socket(io_service, udp::endpoint(udp::v4(), p)) {
  std::cout << "Listening on " << port << std::endl;
}

void Client::start() {

  /* Set up Port Audio */
  PaError err = Pa_Initialize();
  PaStream *stream;

  // dummy data for now
  int udata;
  /* No input, 2 stereo out */
  err = Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, SAMPLE_RATE, 4, pacallback, &udata);

  receive();

  // listen for a packet
  // print its contents!

}


