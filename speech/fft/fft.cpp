#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include "../io/io.h"
#include "fft.h"
using namespace std;

namespace speech{

  vector<int> dft::ip;
  vector<wav_type> dft::w;

  //  template <typename T>
  //  bool dft<T>::cdft(vector< complex<T> > x, const bool isIDFT ){
  bool dft::cdft(vector< complex<wav_type> > x, const bool isIDFT ){
    const int n = x.size();
    const int ipN = 2+(1<<(int)(log(n+0.5)/log(2))/2);
    const int wN = n/2;

    if( ip.size() != ipN ){
      ip.clear(); ip.reserve( ipN ); // size==ipN なら下のコードはいらない
      //      for( int i=0;i<ipN;i++ ) ip.push_back(0);
      w.clear();  w.reserve( wN );
    }

    return false;
  }

  /*
  template <typename T>
  void dft<T>::makewt(int nw, int *ip, double *w){
  }
  template <typename T>
  void dft<T>::bitrv2(int n, int *ip, double *a){
  }
  template <typename T>
  void dft<T>::bitrv2conj(int n, int *ip, double *a){
  }
  template <typename T>
  void dft<T>::cftfsub(int n, double *a, double *w){
  }
  template <typename T>
  void dft<T>::cftbsub(int n, double *a, double *w){
  }
  */
};
