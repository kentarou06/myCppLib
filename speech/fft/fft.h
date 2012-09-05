#include <vector>
#include <complex>
#include "../io/io.h"
using namespace std;

#ifndef READ_DFT
#define READ_DFT
namespace speech{

  class dft{
  private:
    static vector<int> ip;
    static vector<wav_type> w;

    static void makewt(int nw, int *ip, double *w);
    static void bitrv2(int n, int *ip, double *a);
    static void bitrv2conj(int n, int *ip, double *a);
    static void cftfsub(int n, double *a, double *w);
    static void cftbsub(int n, double *a, double *w);

  public:
    static bool cdft(vector< complex<wav_type> > x, const bool isIDFT=false);
    //    static bool cdft(vector< complex<T> > x, const bool isIDFT=false);
  };
};
#endif
