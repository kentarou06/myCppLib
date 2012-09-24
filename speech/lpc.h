
/*  p : order
    lpc coefficient  lpc[0]=1.0, lpc[1], lpc[2],..., lpc[p]
*/


#include <vector>
#include "io.h"
using namespace std;

namespace speech{
  class LPC{
  private:
    vector<wav_type> signal;
    vector<wav_type> parcor;
    vector<wav_type> lpc;
    vector<wav_type> r;
    double en;
    int p;
    void init();
  public:
    LPC();
    LPC(const vector<wav_type> &signal);

    /* if not call anaysis(), below functions return false */
    void setSignal(const vector<wav_type> &signal);
    bool setLPC(const vector<wav_type> lpc);

    bool getSignal(vector<wav_type> &signal);
    bool getPARCOR(vector<wav_type> &parcor);
    /* lpc[0] is always 1.0 if return value is true */
    bool getLPC(vector<wav_type> &lpc);
    bool getAutocorrelation(vector<wav_type> &r);
    int getOrder();
    bool getSpectral(vector<wav_type> &spec, int fftSize);

    bool analysis(const int p);
  };
};
