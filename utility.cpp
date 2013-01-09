#include <string>
#include <typeinfo>
#include <cxxabi.h>
#include "utility.h"
using namespace std;

  /*
  template <typename T>
  char* get_class_name(const T &_class){
    int status;
    const type_info & id_p = typeid(_class);
    return abi::__cxa_demangle(id_p.name(), 0, 0, &status);
  }
  */

string atos(char* c){
  string r;

  while( *c!='\0' )
    r += *(c++);

  return r;
}

#include <iostream>
#include <cstdlib>
#include <fstream>
string read_tmp(int number_of_line=-1){
  ifstream ifs(TMP_FILE);
  string rtn, buf;
  if( !ifs ){
    cerr << "error" << endl;
    return "";
  }

  while( !ifs.eof() ){
    getline( ifs, buf );
    if( rtn.length() > 0 )
      rtn += "\n";
    rtn += buf;
    number_of_line--;
    if( !number_of_line )
      break;
  }
  return rtn;
}

string command::pwd(){
  system("pwd > " TMP_FILE );
  return read_tmp(1);
}

void command::mkdir(string newdir){
  string cmd = "mkdir " + newdir;
  system(cmd.c_str());
}

void command::mkdir(char* newdir){
  command::mkdir(atos(newdir));
}
