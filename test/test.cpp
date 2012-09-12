#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <cstdio>
#include "speech/dft/dft.h"
#include "speech/io/io.h"
#include "speech/lpc/lpc.h"
#include "utility/utility.h"
using namespace std;
using namespace speech;

const double PI=acos(-1);

void test_io_cpp();
void test_fft_cpp();
void test_lpc_cpp();

template <typename T>
T tmp_TEST(T a, T b){
  return a+b;
}

int main(){
  cout << "run test" << endl;
  //  test_io_cpp();
  //  test_fft_cpp();
  test_lpc_cpp();

  return 0;
}

void test_lpc_cpp(){

}

void test_fft_cpp(){
  cout << "test_fft_cpp()" << endl;

  vector<complex<wav_type> > signal, spectram, inv;
  const int SAMPLING_FREQUENCY = 44100;
  const int WINDOW_SIZE = 1024;
  const int PERIOD = SAMPLING_FREQUENCY;
  const double f = 1000;

  cout << "SIGNAL" << endl;
  for( int i=0;i<WINDOW_SIZE;i++ ){
    signal.push_back( 1.0 * cos( 2.0 * PI * f * i / PERIOD) );
    printf("%2d | % 10.4lf % 10.4lf\n", i, signal[i].real(), signal[i].imag() );
  }

  spectram = dft::cdft( signal );
  cout << endl << "SPECTROGRAM" << endl;
  for( int i=0, len=WINDOW_SIZE/2 + 1; i<len;i++ ){
    double freq = dft::convert_index_to_freq( i, WINDOW_SIZE, SAMPLING_FREQUENCY );
    double re = spectram[i].real(), im = spectram[i].imag();
    double amp = abs( spectram[i] );
    double phs = arg( spectram[i] );
    printf( "%7.1lf | % 10.4lf % 10.4lf\t|  % 10.4lf % 10.4lf\n", freq, re, im, amp, phs);
  }

  inv = dft::cdft( spectram, true );
  cout << endl << "INVERSE" << endl;
  for( int i=0;i<(int)inv.size();i++ )
    printf("%2d | % 10.4lf % 10.4lf\n", i, inv[i].real(), inv[i].imag() );

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
