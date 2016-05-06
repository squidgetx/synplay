#include "client.h"

#define PORT 6000

int main(int argc, char **argv) {
  asio::io_service io_service;
  Client * c = new Client(io_service, PORT);
  c->start();
  io_service.run();

  return 0;
}
