#include <iostream>
#include <string>

#include "master.h"
#include "asio.hpp"

using namespace std;
using namespace asio::ip;

#define USAGE "./main FILENAME [ADDR PORT]+\n"

int main(int argc, char *argv[])
{
  
  if (argc < 4 || argc % 2 != 0){
    cerr << USAGE << endl;
    return 1;
  }
 
  string filename = argv[1];
  vector<udp::endpoint> remote_endpts;

  for (i = 2; i < argc; i += 2) {
    string ip_addr = argv[i];
    uint16_t port = atoi(argv[i+1]);
    remote_endpts.push_back(udp::endpoint(address::from_string(ip_addr),port));
  }

  Master *master = new Master(filename,remote_endpts);
  master->run();

  return 0;
}
