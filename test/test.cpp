#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include "speech/io/io.h"
#include "speech/fft/fft.h"
#include "utility/utility.h"
using namespace std;
using namespace speech;

const double PI=acos(-1);

void test_io_cpp();
void test_fft_cpp();

int main(){
  test_io_cpp();
  test_fft_cpp();

  cout << utility::get_class_name(PI) << endl;
  return 0;
}

void test_fft_cpp(){
  vector<complex<wav_type> > v;
  dft::cdft( v, true);
}

void test_io_cpp(){
  char filename[256] = "test00.raw"; //"vaiueo2d.raw";
  int n_Bytes = 1;
  vector<wav_type> left, right;

  // test for write()
  int T = 10000;
  for( int i=0;i<2*T;i++ ){
    double rad = 2.0*PI*100.0*i /T;
    left.push_back( 32767 * cos( rad ) );
    right.push_back( 20000 * sin( rad ) );
  }
  for( n_Bytes=1;n_Bytes<=2;n_Bytes++ ){
    filename[4] = n_Bytes + '0';
    // mono
    filename[5] = '1';
    io::write( filename, n_Bytes, left );
    // stereo
    filename[5] = '2';
    io::write( filename, n_Bytes, left, right );
  }

  // test for read()
  for( n_Bytes=1;n_Bytes<=2;n_Bytes++ ){
    filename[4] = n_Bytes + '0';
    // mono
    filename[5] = '1';
    io::read( filename, n_Bytes, left );
    cout << endl;
    for( int i=0;i<(int)left.size() && i<1000;i++ )
      cout << i << " " << left[i] << endl;
    // stereo
    filename[5] = '2';
    io::read( filename, n_Bytes, left, right );
    cout << endl;
    for( int i=0;i<(int)left.size() && i<1000;i++ )
      cout << i << " " << left[i] << " "
	   << right[i] << endl;
  }
}
