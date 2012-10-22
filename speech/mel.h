#include <vector>
#include "io.h"

namespace speech{
  /* MEL filter bank
     DFT index such as 0, 1, ..., DFTSize-1 represents
     one side of spectrum (positive frequency).
   */
  class MEL{
  protected:
    MEL();
    int DFTSize, filterN, freqResolusion;
    double lowFreq, highFreq, lowMel, highMel;
    vector<double> centerMelFrequencies;
    vector< vector<double> > bank;

  public:
    MEL(const int &DFTSize, const int &filterN, const double &freqResolusion,
	const double &lowFreq, const double &highFreq);

    vector<double> getFilter(const int filterIndex);

    static vector<double> getCenterMelFrequencies(const double lowMel,
						  const double highMel,
						  const int fileterN);

    static double convertMelFrequency(const double &freq);
    static double convertFrequency(const double &melFreq);
  };
}
