#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <iostream>

class Client {
  public:
    Client(int p) : port(p) {};
    void start();

  private:
    int port;



};

#endif
