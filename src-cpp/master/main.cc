#include <iostream>
#include "master.h"

using namespace std;

#define USAGE "./main PORT\n"

int main(int argc, char *argv[])
{
  
  if (argc < 2){
    cerr << USAGE << endl;
  }
 
  int port = atoi(argv[1]);

  Master *master = new Master(port);

  return 0;
}
