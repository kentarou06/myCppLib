#include <cmath>
#include "mel.h"
using namespace std;

namespace speech{

  MEL::MEL(const int &DFTSize, const int &filterN, const double &freqResolusion,
	   const double &lowFreq, const double &highFreq):
    DFTSize(DFTSize),
    filterN(filterN),
    freqResolusion(freqResolusion),
    lowFreq(lowFreq), highFreq(highFreq),
    lowMel(MEL::convertMelFrequency(lowFreq)),
    highMel(MEL::convertMelFrequency(highFreq))
  {
    centerMelFrequencies = MEL::getCenterMelFrequencies(lowMel, highMel, filterN);
    bank.reserve( filterN );
    double lm, hm, cm, freq, mFreq;
    for( int i=0;i<filterN;i++ ){
      bank[i].resize( DFTSize );

      lm = i>0?centerMelFrequencies[i-1]:lowMel;
      cm = centerMelFrequencies[i];
      hm = i+1<filterN?centerMelFrequencies[i+1]:highMel;
      for( int j=0;j<DFTSize;j++ ){
	freq = j * freqResolusion;
	mFreq = MEL::convertMelFrequency( freq );
	if( mFreq < lm )
	  bank[i][j] = 0.0;
	else if( mFreq < cm )
	  bank[i][j] = ( mFreq - lm ) / ( cm - lm );
	else if( mFreq < hm )
	  bank[i][j] = ( -mFreq + hm ) / ( hm - cm );
	else
	  bank[i][j] = 0.0;
      }
    }
  }

  vector<double> MEL::getFilter(const int filterIndex){
    return bank[filterIndex];
  }

  vector<double> MEL::getCenterMelFrequencies(const double lowMel,
					 const double highMel,
					 const int filterN){
    double width = (highMel-lowMel)/(filterN+1);
    vector<double> melFrequencies(filterN);
    for( int i=0;i<filterN;i++ )
      melFrequencies[i] = width * (i+1) + lowMel;

    return melFrequencies;
  }

  double MEL::convertMelFrequency(const double &freq){
    return 2595.0*log(1.0+freq/700.0)/log(10.0);
  }
  double MEL::convertFrequency(const double &melFreq){
    return 700.0*(pow(10.0, melFreq/2595.0)-1);
  }
}
