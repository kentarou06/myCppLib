#include <vector>
#include "dft.h"
#include "io.h"
#include "filter.h"
#include "window.h"
using namespace std;
using namespace speech;

namespace speech{
  vector<wav_type> LPF(const vector<wav_type> &signal,
		       const double &cutoff_frequency,
		       const int &sampling_frequency,
		       const int &fft_length,
		       const int &window_kind ){

    for( int n=fft_length;n>1;n/=2 )
      if( n%2 ){
	cerr << "fft length != 2^n" << endl;
	return null_vector;
      }

    vector< complex<wav_type> > Hd(fft_length);
    for( int i=0,j=fft_length,len=fft_length/2;i<=len;i++,j-- ){
      double freq = dft::convert_index_to_freq( i, fft_length, sampling_frequency );
      Hd[i].real() = Hd[j%fft_length].real() = freq<=cutoff_frequency?1.0:0.0;
      Hd[i].imag() = Hd[j%fft_length].imag() = 0.0;
    }

    vector< complex<wav_type> > hd = dft::cdft( Hd, true ); // IFFT
    vector<double> window = getWindow( fft_length, window_kind );

    vector<double> output(signal.size());
    for( int i=0;i<(int)signal.size();i++ ){
      output[i] = 0.0;
      for( int j=0;j<(int)hd.size();j++ )
	if( i-j>=0 && i-j < (int)signal.size() )
	  output[i] += hd[j].real() * signal[i-j];
    }

    return output;
  }

};
