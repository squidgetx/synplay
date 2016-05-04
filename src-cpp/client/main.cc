#include "client.h"

#define PORT 6000

int main(char **argv) {
  Client * c = new Client(PORT);
  c->start();

  return 0;
}
