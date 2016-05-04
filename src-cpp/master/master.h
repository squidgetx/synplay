#include <string>

#ifndef MASTER_H
#define MASTER_H

class Master
{
  public:
    Master(std::string ip_addr, int port);
    ~Master ();

  private:
    int port;
};

#endif
