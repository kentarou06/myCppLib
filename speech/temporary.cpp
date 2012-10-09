#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include "temporary.h"

namespace speech{
  const double base = pow(2.0, 15);
  double getEnergy(const vector<wav_type> &signal){
    double energy = 0.0, tmp;

    for( int i=0;i<(int)signal.size();i++ ){
      tmp = signal[i] / base;
      energy += tmp * tmp;
    }
    energy = energy / signal.size();

    return energy;
  }

  double getEnergyBydB(const vector<wav_type> &signal){
    double energy = getEnergy( signal );
    return 10.0 * log( energy )/ log( 10.0 );
  }

  bool convEnergy( vector<wav_type> &signal,
		     const double cnvEnergy,
		     const double _orgEnergy ){
    double orgEnergy = _orgEnergy;
    if( orgEnergy < 0.0 )
      orgEnergy = getEnergy( signal );

    double k = sqrt( cnvEnergy / orgEnergy );
    for( int i=0;i<(int)signal.size();i++ )
      signal[i] = k * signal[i];

    return true;
  }
  bool convEnergyBydB( vector<wav_type> &signal,
		     const double cnvEnergyBydB,
		     const double _orgEnergyBydB ){
    double orgEnergyBydB = _orgEnergyBydB;
    if( orgEnergyBydB < 0.0 )
      orgEnergyBydB = getEnergyBydB( signal );

    double cnvEnergy = 10.0 * pow(10.0, 0.1 * cnvEnergyBydB );
    double orgEnergy = 10.0 * pow(10.0, 0.1 * orgEnergyBydB );

    return convEnergy( signal, cnvEnergy, orgEnergy );
  }


  /* http://www-section.cocolog-nifty.com/blog/2008/09/box-muller-2275.html*/  void setSeed() {
    srand((unsigned)time(NULL));
  }
  double genRand() {
    return rand()/(1.0+RAND_MAX);
  }
  double gaussian(double myu, double sgm){
    static int    frag = 0;
    static double save = 0.0;
  
    if(frag == 0){
      double u_0 = genRand();
      double u_1 = genRand();
      double v_0 = myu + sgm*sqrt(-2*log(1-u_0))*cos(2*M_PI*u_1);
      double v_1 = myu + sgm*sqrt(-2*log(1-u_0))*sin(2*M_PI*u_1);
      save       = v_1;
      frag       = 1;
      return v_0;
    }
    else{
      frag = 0;
      return save;
    }
  }


};

