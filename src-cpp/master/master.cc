#include "asio.hpp"

#include "master/master.h"
#include "master/msocket.h"
#include "net/mpacket.h"
#include "net/time_packet.h"

#include <string>

#include <sndfile.hh>

using namespace std;
using namespace asio::ip;

Master::Master(string& filename,vector<udp::endpoint>& r_endpts) :
  remote_endpts(r_endpts)
{
  msockets.reserve(r_endpts.size());
  for (auto endpt : remote_endpts) {
    msockets.push_back(new MSocket(io_service,endpt,filename));
  }
}

Master::~Master(){
  for (MSocket *sock : msockets) {
    delete sock;
  }
}

void Master::run(){
  for (MSocket *sock : msockets) {
    sock->start();
  }
  io_service.run();
}
