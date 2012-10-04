#include <vector>
#include "speech/io.h"
using namespace std;

namespace speech{

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
