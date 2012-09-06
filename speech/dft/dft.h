#include <vector>
#include <complex>
#include "../io/io.h"
using namespace std;

#ifndef READ_DFT
#define READ_DFT
namespace speech{

  class dft{
  private:
    static int *ip;
    static wav_type *w;
    static int ipN;
    static int wN;

    static void cdft(int n, int isgn, wav_type *a, int *ip, wav_type *w);
    static void makewt(int nw, int *ip, wav_type *w);
    static void bitrv2(int n, int *ip, wav_type *a);
    static void bitrv2conj(int n, int *ip, wav_type *a);
    static void cftfsub(int n, wav_type *a, wav_type *w);
    static void cftbsub(int n, wav_type *a, wav_type *w);
    static void cft1st(int n, wav_type *a, wav_type *w);
    static void cftmdl(int n, int l, wav_type *a, wav_type *w);

  public:
    /*
      if isIDFT
        inverse DFT, its coefficients are devided by x.size()
     */
    static vector< complex<wav_type> > cdft( const vector< complex<wav_type> > x,
					     const bool isIDFT=false);
    static double convert_index_to_freq( const int index,
					 const int window_size,
					 const int sampling_freq );
  };
};
#endif
