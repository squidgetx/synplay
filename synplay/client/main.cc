#include "client.h"

#define USAGE "./main PORT"

int main(int argc, char **argv) {

  if (argc < 2){
    std::cerr << USAGE << std::endl;
  }

  int port = atoi(argv[1]);

  asio::io_service io_service;
  Client * c = new Client(io_service, port);
  c->start();
  io_service.run();

  return 0;
}
