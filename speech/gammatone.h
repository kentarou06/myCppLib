/*  Gammatone filters.
      g(t) = t^(n-1) * exp( -2*PI*b*t ) * cos( 2*PI*f*t );
        n : order( in this program, n = 4 fixed )
	b : band width
	f : center frequency
	t : time

    Reference :
      An Efficient Implementation of the Patterson-Holdsworth
      Auditory Filter Bank

*/
#include <vector>
#include "io.h"
using namespace std;

/* multiply coefficients if USE_COEFFICIENTS is defined */
#define USE_COEFFICIENTS

namespace speech{
  class gammatone{
  private:
    double centerFrequency, PIERBandwidth, ERBandwidth;
    double samplingFrequency;
    double a[4][3], b[4][3];
    void init();
    vector<wav_type> secondOrderFilter(const int filterN,
				       const vector<wav_type> input );
    vector<wav_type> secondOrderInvFilter(const int filterN,
				       const vector<wav_type> input );

  public:
    gammatone( const double centerFrequency,
	       const double equivalentRectangularBandWidth,
	       const double samplingFrequency );

    /* input is passed by gammatone filter */
    vector<wav_type> filter( const vector<wav_type> input );

    /* inverse filter */
    vector<wav_type> invFilter( const vector<wav_type> input );

    /* bandwidth [Hz] that correspond to centerFrequency.
        return bandwidth = 24.7*( 4.37/1000.0 * centerFrequency + 1 )
     */
    static double getBandWidth(const double centerFrequency);
    /* same method above */
    static double getEquivalentRectangularBandwidth(const double centerFrequency);


    /* uniformly divide by ERB between beginFreq and endFreq
       and return frequencies
     */
    static vector<double> getCenterFrequencies(double beginFreq,
					       double endFreq,
					       int divideN );

  };
};
