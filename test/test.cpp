#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <cstdio>
#include "speech/dft.h"
#include "speech/filter.h"
#include "speech/frame.h"
#include "speech/gammatone.h"
#include "speech/io.h"
#include "speech/lpc.h"
#include "speech/mel.h"
#include "utility.h"
using namespace std;
using namespace speech;

const double PI = acos(-1);
void test_frame_cpp();  void test_io_cpp();
void test_fft_cpp();    void test_lpc_cpp();
void test_gammatone_cpp(); void test_lpc_cpp2();
void test_mel_cpp(); void test_utility();
void test_filter();
template <typename T>
T tmp_TEST(T a, T b){
  return a+b;
}

int main(){
  //  cout << "run test" << endl;
  //    test_io_cpp();
  //  test_fft_cpp();
  //  test_lpc_cpp();
  //  test_frame_cpp();
  //  test_gammatone_cpp();
  //  test_lpc_cpp2();
  //  test_mel_cpp();
  test_utility();
    //  test_filter();
  return 0;
}

void test_filter(){
  int samplingFrequency = 1000;
  double low_freq = 10.0, high_freq = 400.0;
  double cutoff = 100.0;
  vector<double> input(10000),output;
  for( int i=0;i<(int)input.size();i++ )
    input[i] = cos( 2.0*M_PI*low_freq*i/samplingFrequency )
      + cos( 2.0*M_PI*high_freq*i/samplingFrequency );

  //  output = LPF( input, cutoff, samplingFrequency, 256, hann_window );
  //  output = LPF( input, cutoff, samplingFrequency, 256, hamming_window );
  output = LPF( input, cutoff, samplingFrequency, 256, blackman_window );

  /*
    output = LPF( input, cutoff, samplingFrequency, 256 );
    output = LPF( output, cutoff, samplingFrequency, 256 );
    output = LPF( output, cutoff, samplingFrequency, 256 );
    output = LPF( output, cutoff, samplingFrequency, 256 );
  */
  for( int i=0;i<(int)input.size();i++ )
    cout << i << "\t" << input[i] << "\t" << output[i] << endl;
}

void test_utility(){
  cout << "test for utility.h" << endl;

  if( bool test_atos=false ){
    char a[100] = "abcedfg hij あいうえお\nかきくけこ";
    string s = atos(a);

    cout << " char : " << a << endl;
    cout << " str  : " << s << endl;
  }
  if( bool test_mkdir=false ){
    cout << "pwd : " << command::pwd() << endl;
    command::mkdir( "test_dir" );
  }

  if( bool test_setting=true ){
    setting st = setting("setting_test.txt");

    if( bool write_settings=false ){
      for( int i=1;i<5;i++ )
	st.set_string("str_key"+itos(i),"str_value1"+itos(i));
      for( int i=1;i<7;i++ )
	st.set_int("int_key"+itos(i), i);
      st.write();
    }

    for( int i=1;i<10;i++ ){
      string key = "int_key"+itos(i);
      cout << "int : [" << st.get_int(key, -1)
	   << "] (" << key << ")" << endl;
    }
    for( int i=1;i<10;i++ ){
      string key = "str_key"+itos(i);
      cout << "str : [" << st.get_string(key, "nothing")
	   << "] (" << i << ")" << endl;
    }
  }
}

void test_mel_cpp(){
  bool convertTest = false, centerMelFreqTest = false;
  if( convertTest ){
    for(int i=0;i<100;i++ ){
      double ml  = MEL::convertMelFrequency( 100.0*i );
      double freq= MEL::convertFrequency( ml );
      cout << i << "\t" << ml << "\t" << freq << endl;
    }
  }

  if( centerMelFreqTest ){
    vector<double> freqs = MEL::getCenterMelFrequencies(0.0, 10.0, 4);
    for( int i=0;i<(int)freqs.size();i++ )
      cout << i << "\t" << freqs[i] << endl;
  }

  int DFTSize = 16*2, filterN = 4*3*2;
  double freqResolusion = 1.0;
  double lowFreq=0.0, highFreq=16*2;
  MEL melFilterBank(DFTSize, filterN, freqResolusion, lowFreq, highFreq);

  vector< vector<double> > filterBank;
  for( int i=0;i<filterN;i++ )
    filterBank.push_back( melFilterBank.getFilter(i) );

  for( int freqIndex=0;freqIndex<(int)filterBank[0].size();freqIndex++ ){
    printf("%3d", freqIndex);
    for( int bankIndex=0;bankIndex<filterN;bankIndex++ ){
      if( filterBank[bankIndex][freqIndex] > 0.00001 )
	printf(" %.2lf", filterBank[bankIndex][freqIndex] );
      else
	printf(" %4d", 0);
    }
    cout << endl;
  }
}

class analysisAndSynthesisFrame : public frame{
public:
  int shiftSample;
  vector<wav_type> conv;

  analysisAndSynthesisFrame():
    frame( "../vaiueo2d.raw",
	   22050, // samplingFrequency
	   10,    // shift_msec
	   20)    // window_msec
  {
    shiftSample = getNShiftSamples();
  }

  void oneOfFrame(const int &frame,
		  vector<wav_type> &left, // preemphasised
		  vector<wav_type> &right ){

    // should be window function !

    for( int i=0;i<(int)left.size();i++ ) // hanning window
      left[i] *= ( 0.5 - 0.5 * cos(2.0*acos(-1.0)*i/(left.size()-1)) );
    int order = 24;
    LPC lpc( left );
    lpc.analysis( order );
    vector<wav_type> residue;
    if( !lpc.getResidue( residue ) )
      cout << "rResidue is empty  ";
    /* pitch in [ 80Hz, 300Hz]     */
    int mSt = sampling_freq/300,
        mEn = min( sampling_freq/80 +1,
		   (int)residue.size() );
    double mx=0.0;
    int mIndex = mSt;
    for( int i=mSt;i<mEn;i++ ){
      if( fabs(residue[i]) > mx ){
	mx=fabs(residue[i]); mIndex=i;
      }
    }
    if( frame==0 )
      cout << "[" << mSt <<", " << mEn <<"]" << endl;

    double power = 0.0;
    for( int i=0;i<(int)residue.size();i++ )
      power += residue[i] * residue[i];
    power = sqrt( power );

    cout << "mIndex = "<< mIndex
	 << " \t" << 1000.0*mIndex/sampling_freq
	 << " msec \t"
	 << 1.0*sampling_freq/mIndex << " [Hz]"
	 << " " << power << endl;

    vector<wav_type> input(left.size() ), output;
    for( int i=0;i<(int)input.size();i++ )
      input[i] = i%mIndex?0.0:power;

    if( !lpc.filter( input, output ) )
      cout << "filter failed" << endl;

    //    for( int i=0;i<100;i++ )      conv.push_back( 0.0 );

    int totalIndex = frame * shiftSample;
    for( int i=0;i<(int)output.size();i++ ){
      unsigned int wIndex = totalIndex + i;
      if( wIndex < conv.size() )
	conv[wIndex] += output[i];
      else
	conv.push_back( output[i] );
    }
  }
};

void test_lpc_cpp2(){
  analysisAndSynthesisFrame ASFrame;
  ASFrame.run();

  cout << "WRITING" << endl;
  const char* output="output.raw";
  const int n_Bytes = 2;
  if( io::write( output, n_Bytes, ASFrame.conv ) )
    cout << "writing success." << endl;
  else
    cout << "writing failed." << endl;
}

void test_gammatone_cpp(){
  const int samplingFrequency = 10000;
  const double centerFrequency = 1000.0;
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

  if( bool test_wav_class = true ){

    if( bool read_test = false ){
      string filename = "wav_output_test.wav";
      //      string filename = "../vaiueo2d.wav";
      wav w( filename );

      if( w.read() )
	cout << "success to read wav" << endl;
      else
	cout << "failed to read wav" << endl;

      vector<double> v = w.get_left_data();
      //      for( int i=0;i<(int)v.size();i++ )	cout << i/22050.0 << "\t" << v[i] << endl;

      return;
    }

    if( bool write_test = true ){
      string filename = "wav_output_test.wav";
      wav w( filename );
      int sampling_freq = 44100;
      int len = 11 * sampling_freq;
      vector<double> left(len);
      for( int i=0;i<len;i++ )
	left[i] = 30000.0 * cos( 2.0*M_PI*1000.0*i/sampling_freq );

      w.set_channel(1);
      w.set_sampling_frequency( sampling_freq );
      w.set_n_bytes(2);
      w.set_left_data(left);

      if( w.write() )
	cout << "success to write wav" << endl;
      else
	cout << "failed to write wav" << endl;
    }

    return ;
  }

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
