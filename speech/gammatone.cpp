#include <vector>
#include <cmath>
#include "gammatone.h"
#include "io.h"
using namespace std;

namespace speech{
  const double PI = cos(-1.0);

  gammatone::gammatone( const double centerFrequency,
			const double bandWidth,
			const double samplingFrequency ){
    this->centerFrequency = centerFrequency;
    this->bandWidth = 2.0 * PI * 1.019
      * bandWidth; // further multiplied by 1.019
    this->samplingFrequency = samplingFrequency;

    init();
  }

  void gammatone::init(){
    const double dt = 1.0 / samplingFrequency;
    const double rad = 2.0 * PI * centerFrequency * dt;
    const double cs = cos( rad ), sn = sin( rad );
    const double ex = exp( dt * bandWidth );
    const double sq1 = sqrt( 3.0 + pow(2.0, 3.0/2.0) ),
      sq2 = sqrt( 3.0 - pow(2.0, 3.0/2.0) );

    for( int i=0;i<4;i++ ){
      a[i][0] = dt;
      a[i][2] = 0.0;
      b[i][0] = 1.0;
      b[i][1] = -2.0 * cs / ex;
      b[i][2] = exp( -2.0 * bandWidth * dt );
    }
    a[0][1] = -( 2.0*dt*cs/ex + 2.0*sq1*dt*sn/ex ) / 2.0;
    a[1][1] = -( 2.0*dt*cs/ex - 2.0*sq1*dt*sn/ex ) / 2.0;
    a[2][1] = -( 2.0*dt*cs/ex + 2.0*sq2*dt*sn/ex ) / 2.0;
    a[3][1] = -( 2.0*dt*cs/ex - 2.0*sq2*dt*sn/ex ) / 2.0;
  }

  vector<wav_type> gammatone::filter( const vector<wav_type> input ){
    vector<wav_type> output = input;

    for( int filterN=0; filterN<4; filterN++ )
      output = secondOrderFilter( filterN, output );

    return output;
  }

  vector<wav_type> gammatone::secondOrderFilter( const int filterN,
						 const vector<wav_type> input ){
    vector<wav_type> output( input.size() );
    double z1 = 0.0, z2 = 0.0;

    for( int i=0;i<(int)input.size();i++ ){
      output[i] = a[filterN][0] * input[i] + z1;
      z1 = a[filterN][1] * input[i] - b[filterN][1] * output[i] + z2;
      z2 = a[filterN][2] * input[i] - b[filterN][2] * output[i];
    }

    return output;
  }

  double gammatone::getBandWidth(const double centerFrequency){
    return 24.7 * ( 4.37 * centerFrequency / 1000.0 + 1.0 );
  }
};
