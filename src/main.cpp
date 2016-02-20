#include "HartreeFock.hh"
#include <iostream>

//USAGE: hfock twobody.dat spdata.dat out_file.dat
int main(int argc, char**argv){
  if (argc < 4){
    std::cout << "USAGE: hfock twobody.dat spdata.dat out_file.dat" << std::endl;
    return -1;
  }
  HartreeFock *hfock = new HartreeFock();
  hfock->Run(argv[1],argv[2],argv[3]);
  return 0;
}
