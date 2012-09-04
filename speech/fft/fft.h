#include <vector>
#include <complex>
using namespace std;

namespace speech{
  typedef double wav_type; // define ../io/io.h

  template <typename T>
  class dft{
  private:
    static vector<T> *ip;
    //    static vector<wav_type> *ip;
    static double *w;

    static void makewt(int nw, int *ip, double *w);
    static void bitrv2(int n, int *ip, double *a);
    static void bitrv2conj(int n, int *ip, double *a);
    static void cftfsub(int n, double *a, double *w);
    static void cftbsub(int n, double *a, double *w);
  public:
    static bool cdft(vector< complex<T> > x, const bool isIDFT=false);
  };
};
