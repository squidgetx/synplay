
#ifndef MASTER_H
#define MASTER_H

class Master
{
  public:
    Master::Master(string ip_addr, int port);
    ~Master ();

  private:
    int port;
};

#endif
