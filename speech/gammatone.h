/*  Gammatone filters.
      g(t) = t^(n-1) * exp( -2*PI*b*t ) * cos( 2*PI*f*t );
        n : order( in this program, n = 4 fixed )
	b : band width
	f : center frequency
	t : time

    Reference :
      An Efficient Implementation of the Patterson-Holdsworth
      Auditory Filter Bank

    TODO : Amplitude adjustment. ( with g(t) ? )
*/
#include <vector>
#include "io.h"
using namespace std;

namespace speech{
  class gammatone{
  private:
    double centerFrequency, bandWidth;
    double samplingFrequency;
    double a[4][3], b[4][3];
    void init();
    vector<wav_type> secondOrderFilter(const int filterN,
				       const vector<wav_type> input );
    vector<wav_type> secondOrderInvFilter(const int filterN,
				       const vector<wav_type> input );

  public:
    gammatone( const double centerFrequency,
	       const double bandWidth,
	       const double samplingFrequency );

    /* input is passed by gammatone filter */
    vector<wav_type> filter( const vector<wav_type> input );

    /* inverse filter */
    vector<wav_type> invFilter( const vector<wav_type> input );

    /* bandwidth [Hz] that correspond to centerFrequency.
        bandwidth = 24.7*( 4.37/1000.0 * centerFrequency + 1 )
     */
    static double getBandWidth(const double centerFrequency);

    static vector<double> getCenterFrequencies(double beginFreq,
							  double endFreq,
							  int divideN );

  };
};
