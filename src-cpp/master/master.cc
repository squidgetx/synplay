#include "master.h"
#include "asio.hpp"

#include <string>

using namespace std;
using namespace asio;

Master::Master(string ip_addr, int port){
  udp::endpoint endpt = udp::endpoint(ip::address::from_string(ip_addr),port);
}

void connect(){
}
