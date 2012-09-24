#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <cstdio>
#include "speech/dft.h"
#include "speech/frame.h"
#include "speech/gammatone.h"
#include "speech/io.h"
#include "speech/lpc.h"
#include "utility/utility.h"
using namespace std;
using namespace speech;

const double PI=acos(-1);
void test_frame_cpp();  void test_io_cpp();
void test_fft_cpp();    void test_lpc_cpp();
void test_gammatone_cpp();

template <typename T>
T tmp_TEST(T a, T b){
  return a+b;
}

int main(){
  cout << "run test" << endl;
  //  test_io_cpp();
  //  test_fft_cpp();
  //  test_lpc_cpp();
  //  test_frame_cpp();
  test_gammatone_cpp();
  return 0;
}

void test_gammatone_cpp(){

  const int samplingFrequency = 10000;
  const double centerFrequency = 100.0;
  const double bandwidth = gammatone::getBandWidth( centerFrequency );

  const int length = samplingFrequency;
  vector<double> impulseByDef(length);
  for( int i=0;i<length;i++ ){
    double t = (double) i / samplingFrequency;
    impulseByDef[i] = pow(t,4-1) * exp( -2.0*PI*bandwidth*t )
      * cos( 2.0*PI*centerFrequency*t );
  }

  gammatone gf( centerFrequency, bandwidth, samplingFrequency );
  vector<double> impulseByClass(length);
  impulseByClass[0] = 1.0;
  for( int i=1;i<length;i++ )
    impulseByClass[i] = 0.0;
  impulseByClass = gf.filter( impulseByClass );

  double mx1=0.0, mx2=0.0;
  for( int i=0;i<length;i++ ){
    mx1 = max(mx1, fabs(impulseByDef[i]) );
    mx2 = max(mx2, fabs(impulseByClass[i]) );
  }
  for( int i=0;i<length;i++ ){
    impulseByDef[i] = 1.0 * impulseByDef[i] / mx1;
    impulseByClass[i] = 1.0 * impulseByClass[i] / mx2;
  }

  cout << "index\ttime\tdefinition\tIIR" << endl;
  for( int i=0;i<length;i++ )
    cout << i << "\t" << (double)i/samplingFrequency
	 << "\t" << impulseByDef[i]
	 << "\t" << impulseByClass[i] << endl;

}



class inheritFrame : public frame{
public:
  inheritFrame(const char* filename,
	       const int sampling_freq,
	       const int shift_msec,
	       const int window_msec,
	       const int n_Bytes = 2,
	       const bool isStereo = false );
  void oneOfFrame(const int &frame,
		  vector<wav_type> &left,
		  vector<wav_type> &right = speech::null_vector );
};

inheritFrame::inheritFrame(const char* filename,
			   const int sampling_freq,
			   const int shift_msec,
			   const int window_msec,
			   const int n_Bytes,
			   const bool isStereo )
  : frame( filename, sampling_freq, shift_msec, window_msec,
	   n_Bytes, isStereo ){
}
void inheritFrame::oneOfFrame(const int &frame,
			      vector<wav_type> &left,
			      vector<wav_type> &right ){
  cout << " inheritFrame::oneOfFrame #" << frame
       << "\t" << getMSec(frame) << " [ms]" << endl;

}

void test_frame_cpp(){
  cout << "test_frame_cpp" <<  endl;
  char filename[256] = "../vaiueo2d.raw";
  const int sampling_freq = 22050,
    shift_msec = 20,
    window_msec = 40,
    n_Bytes = 2,
    isStereo = false;
  inheritFrame fr( filename, sampling_freq, shift_msec, window_msec,
		   n_Bytes, isStereo );

  fr.run();
};





void test_lpc_cpp(){
  char filename[256] = "../vaiueo2d.raw";
  vector<wav_type> left;
  int n_Bytes = 2;
  LPC lpc;

  if( !io::read( filename, n_Bytes, left ) ){
    cout << "failed to reading file : " << filename << endl;
    return ;
  }

  int samplingRate = 22050;
  int windowSize = samplingRate * 40 / 1000;
  int shiftSize  = windowSize / 2;  // half shift
  int p = 24;
  /*
  cout << "sampleSize : " << left.size() << endl
       << "windowSize : " << windowSize << endl
       << "shiftSize  : " << shiftSize << endl;
  */
  vector<wav_type> parcor, lp;
  for( int i=0,frame=0;i+windowSize < (int)left.size();i+=shiftSize,frame++ ){
    vector<wav_type> sig(windowSize);
    for( int j=0;j<windowSize;j++ )
      sig.push_back( left[i+j] );
    lpc.setSignal( sig );
    lpc.analysis( p );

    /*
    lpc.getLPC( lp );
    lpc.getPARCOR( parcor );

    cout << endl << "FRAME " << frame << endl;
    cout << "index\tlpc\tparcor" << endl;
    for( int i=0;i<10;i++ )
      cout << i << "\t" << lp[i] << "\t" << parcor[i] << endl;
    */

    vector<wav_type> spec;
    int fftSize = 512;
    lpc.getSpectral( spec, fftSize );

    cout << "FRAME : " << frame << ", \t"
	 << ( (double)i / samplingRate) << " [ms]" << endl;
    for( int i=0;i<fftSize/2;i++ ){
      double freq = dft::convert_index_to_freq( i,
					   2*fftSize,
					   samplingRate );
      cout << freq << "\t" << spec[i] << endl;
    }
    cout << endl;

    cout << "input integer : ";
    cin >> fftSize;
  }

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
