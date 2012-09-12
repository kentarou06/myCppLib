#include <vector>
#include "speech/io/io.h"
using namespace std;

namespace speech{
  class LPC{
  private:
    vector<wav_type> signal;
    vector<wav_type> parcor;
    vector<wav_type> lpc;
    vector<wav_type> r;
    int p;
    void init();
  public:
    LPC();
    LPC(const vector<wav_type> &signal);
    void setSignal(const vector<wav_type> &signal);

    bool analysis(const int p);
    /* if not call anaysis(), below functions return false */
    bool getPARCOR(vector<wav_type> &parcor);
    bool getLPC(vector<wav_type> &lpc);
    bool getAutocorrelation(vector<wav_type> &r);
    int getOrder();
  };
};
