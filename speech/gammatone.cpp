#include <vector>
#include <cmath>
#include "gammatone.h"
#include "io.h"
using namespace std;

namespace speech{
  const double PI = acos(-1.0);

  gammatone::gammatone( const double centerFrequency,
			const double bandWidth,
			const double samplingFrequency ){
    this->centerFrequency = centerFrequency;
    this->bandWidth = 2.0 * PI * bandWidth;
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

#ifdef USE_COEFFICIENTS
    double coe = pow( bandWidth, 4.0 );
    for( int i=0;i<(int)output.size();i++ )
      output[i] = coe * output[i];
#endif

    return output;
  }

  vector<wav_type> gammatone::invFilter( const vector<wav_type> input ){
    vector<wav_type> output = input;

    for( int filterN=0; filterN<4; filterN++ )
      output = secondOrderInvFilter( filterN, output );

#ifdef USE_COEFFICIENTS
    double coe = pow( bandWidth, -4.0 );
    for( int i=0;i<(int)output.size();i++ )
      output[i] = coe * output[i];
#endif

    return output;
  }

  vector<wav_type> gammatone::secondOrderInvFilter(const int filterN,
					const vector<wav_type> input ){
    vector<wav_type> output( input.size() );
    double z1 = 0.0, z2 = 0.0, z3 = 0.0, z4 = 0.0;

    for( int i=0;i<(int)input.size();i++ ){
      output[i] = input[i] + b[filterN][1] * z1 + b[filterN][2] * z2
	- a[filterN][1] * z3 - a[filterN][2] * z4;
      output[i] = output[i] / a[filterN][0];
      z2 = z1;
      z1 = input[i];
      z4 = z3;
      z3 = output[i];
    }

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

  double getERB(double f){
    return 21.4 * log( 4.37*f/1000.0 + 1 ) / log( 10.0 );
  }
  double getFreq(double erb){
    return ( pow(10.0,erb/21.4)-1 ) * 1000/4.37;
  }
  double getERBBand(double f){
    return 24.7*( 4.37/1000.0 * f + 1 );
  }
  vector<double> gammatone::getCenterFrequencies(double beginFreq,
						 double endFreq,
						 int divideN ){
    double beginERB = getERB( beginFreq );
    double endERB   = getERB( endFreq );
    double space = (endERB - beginERB) / (divideN-1);
    vector<double> frequencies(divideN);
    for( int i=0;i<divideN;i++ ){
      double erb  = beginERB + i * space;
      frequencies[i] = getFreq( erb );
      /*
      double freq = getFreq( erb );
      double band = getERBBand( freq );
      */
    }
    return frequencies;
  }
};
