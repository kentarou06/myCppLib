#include <vector>
#include "io.h"
using namespace std;

namespace speech{
  /* calculate signal coefficient when changing SNR to targetSNRdB */
  double getCoeForSignal( const vector<wav_type> signal, const vector<wav_type> noise,
			  const double targetSNRdB );
  /* calculate noise coefficient when changing SNR to targetSNRdB */
  double getCoeForNoise( const vector<wav_type> signal, const vector<wav_type> noise,
			 const double targetSNRdB );

  double getSNR(const vector<wav_type> signal, const vector<wav_type> noise);
  double getSNRbydB(const vector<wav_type> signal, const vector<wav_type> noise);

  double getEnergy(const vector<wav_type> &signal);
  double getEnergyBydB(const vector<wav_type> &signal);

  bool convEnergy( vector<wav_type> &signal,
		     const double cnvEnergy,
		     const double orgEnergy = -1.0 );
  bool convEnergyBydB( vector<wav_type> &signal,
		     const double cnvEnergyBydB,
		     const double orgEnergyBydB = -1.0 );

  /* http://www-section.cocolog-nifty.com/blog/2008/09/box-muller-2275.html*/
  void setSeed();
  double genRand();
  double gaussian(double myu, double sgm);

};
