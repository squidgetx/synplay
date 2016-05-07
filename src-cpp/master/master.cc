#include "asio.hpp"

#include "master/master.h"
#include "master/msocket.h"
#include "util/syntime.h"
#include "net/mpacket.h"
#include "net/time_packet.h"

#include <string>

#include <sndfile.hh>

using namespace std;
using namespace asio::ip;

Master::Master(string filename,vector<udp::endpoint> r_endpts) :
  remote_endpts(r_endpts)
{
  for (auto endpt : remote_endpts) {
    msockets.push_back(MSocket(io_service,endpt,filename));
  }
}

void Master::run(){
  for (auto sock : msockets) {
    sock.start();
  }
  io_service.run();
}
