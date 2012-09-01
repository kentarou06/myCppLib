#include <iostream>
#include "speech/io/io.h"
using namespace std;

void test_io_cpp();

int main(){
  test_io_cpp();

  return 0;
}


void test_io_cpp(){
  char filename[] = "vaiueo2d.raw";
  int n_Bytes = 2;
  vector<wav_type> left, right;

  /*
  cout << "MAIN" << endl;
  cout << "null_vector : " << &null_vector
       << " " << (long long)&null_vector
       << "\tR: " << &right
       << " " << (long long)&right
       << endl << endl;;
  */

  read( filename, n_Bytes, left, right );
  read( filename, n_Bytes, left );

  write( filename, n_Bytes, left, right );
  write( filename, n_Bytes, left );
}
