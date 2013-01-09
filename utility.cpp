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
#include <cstdlib>
int stoi(string s){
  return atoi(s.c_str());
}
#include <sstream>
string itos(int i){
  stringstream ss;
  ss << i;
  return ss.str();
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



#include <map>
setting::setting(){};

/* if setting_filename exist, read setting   */
setting::setting(string setting_filename){
  this->setting_filename = setting_filename;
  read();
}
setting::setting(char* setting_filename){
  this->setting_filename = atos(setting_filename);
  read();
}


bool setting::read(){
  ifstream ifs(setting_filename.c_str());
  if( !ifs )
    return false;

  string buf;
  while(getline(ifs,buf) && buf != "" ){
    int i=0;
    while( buf[i++]!='\t' );
    map_string[buf.substr(0,i-1)] = buf.substr(i);
  }
  while(getline(ifs,buf) && buf != "" ){
    int i=0;
    while( buf[i++]!='\t' );
    map_int[buf.substr(0,i-1)] = stoi(buf.substr(i));
  }
  ifs.close();



  return true;
}

bool setting::write(){
  ofstream ofs(setting_filename.c_str());
  if( !ofs )
    return false;

  for( map<string,string>::iterator it=map_string.begin();
       it != map_string.end();it++ ){
    pair<string, string> p = *it;
    ofs << p.first << "\t" << p.second << endl;
  }
  ofs << endl;
  for( map<string,int>::iterator it=map_int.begin();
       it != map_int.end();it++ ){
    pair<string, int> p = *it;
    ofs << p.first << "\t" << p.second << endl;
  }
  ofs << endl;
  ofs.close();

  return true;
}

void setting::set_string(const string key, const string value){
  map_string[key] = value;
}
string setting::get_string(const string key, const string default_value){
  if( map_string.find(key) == map_string.end() )
    return default_value;
  return map_string[key];
}

void setting::set_int(const string key, const int value){
  map_int[key] = value;
}
int setting::get_int(const string key, const int default_value){
  if( map_int.find(key) == map_int.end() )
    return default_value;
  return map_int[key];
}

