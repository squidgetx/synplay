#include <iostream>
#include <string>

#include "master.h"

using namespace std;

#define USAGE "./main ADDR PORT\n"

int main(int argc, char *argv[])
{
  
  if (argc < 3){
    cerr << USAGE << endl;
    return 1;
  }
 
  string ip_addr = argv[1];
  uint16_t port = atoi(argv[2]);
  string filename = argv[3];

  Master *master = new Master(ip_addr,port,filename);
  master->run();

  return 0;
}
