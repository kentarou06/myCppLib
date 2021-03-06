#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include "io.h"
#include "dft.h"
using namespace std;

namespace speech{

  int *dft::ip = 0;
  wav_type *dft::w = 0;
  int dft::ipN = 0;
  int dft::wN  = 0;

  double dft::convert_index_to_freq( const int index,
				     const int window_size,
				     const int sampling_freq ){
    return (double)index * sampling_freq / window_size;
  }

  //  template <typename T>
  //  bool dft<T>::cdft(vector< complex<T> > x, const bool isIDFT ){
  vector< complex<wav_type> > dft::cdft(vector< complex<wav_type> > x, const bool isIDFT ){
    const int n = x.size();
    const int _ipN = 2+(1<<(int)(log(n+0.5)/log(2))/2);
    const int _wN = n/2;

    if( ipN != _ipN ){
      ipN = _ipN;
      wN = _wN;
      if( ip!=0 ) delete[] ip;
      if( w!=0 )  delete[] w;
      ip = new int[ipN];
      w  = new wav_type[wN];
      ip[0] = 0;
    }

    int isgn = isIDFT?-1:1;
    wav_type a[2*n];
    for( int i=0;i<n;i++ ){
      a[2*i  ] = x[i].real();
      a[2*i+1] = x[i].imag();
    }

    cdft( 2*n, isgn, a, ip, w );

    const double deno = isIDFT?(n):1.0;
    vector< complex<wav_type> > rtn;
    for( int i=0;i<n;i++ ){
      wav_type re = a[2*i  ], im = a[2*i+1];
      rtn.push_back( complex<wav_type>( re, im ) / deno );
    }

    return rtn;
  }

  void dft::cdft(int n, int isgn, wav_type *a, int *ip, wav_type *w){
    if (n > (ip[0] << 2)) {
      makewt(n >> 2, ip, w);
    }
    if (n > 4) {
      if (isgn >= 0) {
	bitrv2(n, ip + 2, a);
	cftfsub(n, a, w);
      } else {
	bitrv2conj(n, ip + 2, a);
	cftbsub(n, a, w);
      }
    } else if (n == 4) {
      cftfsub(n, a, w);
    }
  }

  void dft::makewt(int nw, int *ip, wav_type *w){
    int j, nwh;
    double delta, x, y;

    ip[0] = nw;
    ip[1] = 1;
    if (nw > 2) {
      nwh = nw >> 1;
      delta = atan(1.0) / nwh;
      w[0] = 1;
      w[1] = 0;
      w[nwh] = cos(delta * nwh);
      w[nwh + 1] = w[nwh];
      if (nwh > 2) {
	for (j = 2; j < nwh; j += 2) {
	  x = cos(delta * j);
	  y = sin(delta * j);
	  w[j] = x;
	  w[j + 1] = y;
	  w[nw - j] = y;
	  w[nw - j + 1] = x;
	}
	bitrv2(nw, ip + 2, w);
      }
    }
  }

  void dft::bitrv2(int n, int *ip, wav_type *a){
    int j, j1, k, k1, l, m, m2;
    double xr, xi, yr, yi;

    ip[0] = 0;
    l = n;
    m = 1;
    while ((m << 3) < l) {
      l >>= 1;
      for (j = 0; j < m; j++) {
	ip[m + j] = ip[j] + l;
      }
      m <<= 1;
    }
    m2 = 2 * m;
    if ((m << 3) == l) {
      for (k = 0; k < m; k++) {
	for (j = 0; j < k; j++) {
	  j1 = 2 * j + ip[k];
	  k1 = 2 * k + ip[j];
	  xr = a[j1];
	  xi = a[j1 + 1];
	  yr = a[k1];
	  yi = a[k1 + 1];
	  a[j1] = yr;
	  a[j1 + 1] = yi;
	  a[k1] = xr;
	  a[k1 + 1] = xi;
	  j1 += m2;
	  k1 += 2 * m2;
	  xr = a[j1];
	  xi = a[j1 + 1];
	  yr = a[k1];
	  yi = a[k1 + 1];
	  a[j1] = yr;
	  a[j1 + 1] = yi;
	  a[k1] = xr;
	  a[k1 + 1] = xi;
	  j1 += m2;
	  k1 -= m2;
	  xr = a[j1];
	  xi = a[j1 + 1];
	  yr = a[k1];
	  yi = a[k1 + 1];
	  a[j1] = yr;
	  a[j1 + 1] = yi;
	  a[k1] = xr;
	  a[k1 + 1] = xi;
	  j1 += m2;
	  k1 += 2 * m2;
	  xr = a[j1];
	  xi = a[j1 + 1];
	  yr = a[k1];
	  yi = a[k1 + 1];
	  a[j1] = yr;
	  a[j1 + 1] = yi;
	  a[k1] = xr;
	  a[k1 + 1] = xi;
	}
	j1 = 2 * k + m2 + ip[k];
	k1 = j1 + m2;
	xr = a[j1];
	xi = a[j1 + 1];
	yr = a[k1];
	yi = a[k1 + 1];
	a[j1] = yr;
	a[j1 + 1] = yi;
	a[k1] = xr;
	a[k1 + 1] = xi;
      }
    } else {
      for (k = 1; k < m; k++) {
	for (j = 0; j < k; j++) {
	  j1 = 2 * j + ip[k];
	  k1 = 2 * k + ip[j];
	  xr = a[j1];
	  xi = a[j1 + 1];
	  yr = a[k1];
	  yi = a[k1 + 1];
	  a[j1] = yr;
	  a[j1 + 1] = yi;
	  a[k1] = xr;
	  a[k1 + 1] = xi;
	  j1 += m2;
	  k1 += m2;
	  xr = a[j1];
	  xi = a[j1 + 1];
	  yr = a[k1];
	  yi = a[k1 + 1];
	  a[j1] = yr;
	  a[j1 + 1] = yi;
	  a[k1] = xr;
	  a[k1 + 1] = xi;
	}
      }
    }
  }

  void dft::bitrv2conj(int n, int *ip, wav_type *a){
    int j, j1, k, k1, l, m, m2;
    double xr, xi, yr, yi;

    ip[0] = 0;
    l = n;
    m = 1;
    while ((m << 3) < l) {
      l >>= 1;
      for (j = 0; j < m; j++) {
	ip[m + j] = ip[j] + l;
      }
      m <<= 1;
    }
    m2 = 2 * m;
    if ((m << 3) == l) {
      for (k = 0; k < m; k++) {
	for (j = 0; j < k; j++) {
	  j1 = 2 * j + ip[k];
	  k1 = 2 * k + ip[j];
	  xr = a[j1];
	  xi = -a[j1 + 1];
	  yr = a[k1];
	  yi = -a[k1 + 1];
	  a[j1] = yr;
	  a[j1 + 1] = yi;
	  a[k1] = xr;
	  a[k1 + 1] = xi;
	  j1 += m2;
	  k1 += 2 * m2;
	  xr = a[j1];
	  xi = -a[j1 + 1];
	  yr = a[k1];
	  yi = -a[k1 + 1];
	  a[j1] = yr;
	  a[j1 + 1] = yi;
	  a[k1] = xr;
	  a[k1 + 1] = xi;
	  j1 += m2;
	  k1 -= m2;
	  xr = a[j1];
	  xi = -a[j1 + 1];
	  yr = a[k1];
	  yi = -a[k1 + 1];
	  a[j1] = yr;
	  a[j1 + 1] = yi;
	  a[k1] = xr;
	  a[k1 + 1] = xi;
	  j1 += m2;
	  k1 += 2 * m2;
	  xr = a[j1];
	  xi = -a[j1 + 1];
	  yr = a[k1];
	  yi = -a[k1 + 1];
	  a[j1] = yr;
	  a[j1 + 1] = yi;
	  a[k1] = xr;
	  a[k1 + 1] = xi;
	}
	k1 = 2 * k + ip[k];
	a[k1 + 1] = -a[k1 + 1];
	j1 = k1 + m2;
	k1 = j1 + m2;
	xr = a[j1];
	xi = -a[j1 + 1];
	yr = a[k1];
	yi = -a[k1 + 1];
	a[j1] = yr;
	a[j1 + 1] = yi;
	a[k1] = xr;
	a[k1 + 1] = xi;
	k1 += m2;
	a[k1 + 1] = -a[k1 + 1];
      }
    } else {
      a[1] = -a[1];
      a[m2 + 1] = -a[m2 + 1];
      for (k = 1; k < m; k++) {
	for (j = 0; j < k; j++) {
	  j1 = 2 * j + ip[k];
	  k1 = 2 * k + ip[j];
	  xr = a[j1];
	  xi = -a[j1 + 1];
	  yr = a[k1];
	  yi = -a[k1 + 1];
	  a[j1] = yr;
	  a[j1 + 1] = yi;
	  a[k1] = xr;
	  a[k1 + 1] = xi;
	  j1 += m2;
	  k1 += m2;
	  xr = a[j1];
	  xi = -a[j1 + 1];
	  yr = a[k1];
	  yi = -a[k1 + 1];
	  a[j1] = yr;
	  a[j1 + 1] = yi;
	  a[k1] = xr;
	  a[k1 + 1] = xi;
            }
	k1 = 2 * k + ip[k];
	a[k1 + 1] = -a[k1 + 1];
	a[k1 + m2 + 1] = -a[k1 + m2 + 1];
      }
    }
  }

  void dft::cftfsub(int n, wav_type *a, wav_type *w){
    int j, j1, j2, j3, l;
    double x0r, x0i, x1r, x1i, x2r, x2i, x3r, x3i;

    l = 2;
    if (n > 8) {
      cft1st(n, a, w);
      l = 8;
      while ((l << 2) < n) {
	cftmdl(n, l, a, w);
	l <<= 2;
      }
    }
    if ((l << 2) == n) {
      for (j = 0; j < l; j += 2) {
	j1 = j + l;
	j2 = j1 + l;
	j3 = j2 + l;
	x0r = a[j] + a[j1];
	x0i = a[j + 1] + a[j1 + 1];
	x1r = a[j] - a[j1];
	x1i = a[j + 1] - a[j1 + 1];
	x2r = a[j2] + a[j3];
	x2i = a[j2 + 1] + a[j3 + 1];
	x3r = a[j2] - a[j3];
	x3i = a[j2 + 1] - a[j3 + 1];
	a[j] = x0r + x2r;
	a[j + 1] = x0i + x2i;
	a[j2] = x0r - x2r;
	a[j2 + 1] = x0i - x2i;
	a[j1] = x1r - x3i;
	a[j1 + 1] = x1i + x3r;
	a[j3] = x1r + x3i;
	a[j3 + 1] = x1i - x3r;
      }
    } else {
      for (j = 0; j < l; j += 2) {
	j1 = j + l;
	x0r = a[j] - a[j1];
	x0i = a[j + 1] - a[j1 + 1];
	a[j] += a[j1];
	a[j + 1] += a[j1 + 1];
	a[j1] = x0r;
	a[j1 + 1] = x0i;
      }
    }
  }

  void dft::cftbsub(int n, wav_type *a, wav_type *w){
    int j, j1, j2, j3, l;
    double x0r, x0i, x1r, x1i, x2r, x2i, x3r, x3i;

    l = 2;
    if (n > 8) {
      cft1st(n, a, w);
      l = 8;
      while ((l << 2) < n) {
	cftmdl(n, l, a, w);
	l <<= 2;
      }
    }
    if ((l << 2) == n) {
      for (j = 0; j < l; j += 2) {
	j1 = j + l;
	j2 = j1 + l;
	j3 = j2 + l;
	x0r = a[j] + a[j1];
	x0i = -a[j + 1] - a[j1 + 1];
	x1r = a[j] - a[j1];
	x1i = -a[j + 1] + a[j1 + 1];
	x2r = a[j2] + a[j3];
	x2i = a[j2 + 1] + a[j3 + 1];
	x3r = a[j2] - a[j3];
	x3i = a[j2 + 1] - a[j3 + 1];
	a[j] = x0r + x2r;
	a[j + 1] = x0i - x2i;
	a[j2] = x0r - x2r;
	a[j2 + 1] = x0i + x2i;
	a[j1] = x1r - x3i;
	a[j1 + 1] = x1i - x3r;
	a[j3] = x1r + x3i;
	a[j3 + 1] = x1i + x3r;
      }
    } else {
      for (j = 0; j < l; j += 2) {
	j1 = j + l;
	x0r = a[j] - a[j1];
	x0i = -a[j + 1] + a[j1 + 1];
	a[j] += a[j1];
	a[j + 1] = -a[j + 1] - a[j1 + 1];
	a[j1] = x0r;
	a[j1 + 1] = x0i;
      }
    }
  }

  void dft::cft1st(int n, wav_type *a, wav_type *w){
    int j, k1, k2;
    double wk1r, wk1i, wk2r, wk2i, wk3r, wk3i;
    double x0r, x0i, x1r, x1i, x2r, x2i, x3r, x3i;

    x0r = a[0] + a[2];
    x0i = a[1] + a[3];
    x1r = a[0] - a[2];
    x1i = a[1] - a[3];
    x2r = a[4] + a[6];
    x2i = a[5] + a[7];
    x3r = a[4] - a[6];
    x3i = a[5] - a[7];
    a[0] = x0r + x2r;
    a[1] = x0i + x2i;
    a[4] = x0r - x2r;
    a[5] = x0i - x2i;
    a[2] = x1r - x3i;
    a[3] = x1i + x3r;
    a[6] = x1r + x3i;
    a[7] = x1i - x3r;
    wk1r = w[2];
    x0r = a[8] + a[10];
    x0i = a[9] + a[11];
    x1r = a[8] - a[10];
    x1i = a[9] - a[11];
    x2r = a[12] + a[14];
    x2i = a[13] + a[15];
    x3r = a[12] - a[14];
    x3i = a[13] - a[15];
    a[8] = x0r + x2r;
    a[9] = x0i + x2i;
    a[12] = x2i - x0i;
    a[13] = x0r - x2r;
    x0r = x1r - x3i;
    x0i = x1i + x3r;
    a[10] = wk1r * (x0r - x0i);
    a[11] = wk1r * (x0r + x0i);
    x0r = x3i + x1r;
    x0i = x3r - x1i;
    a[14] = wk1r * (x0i - x0r);
    a[15] = wk1r * (x0i + x0r);
    k1 = 0;
    for (j = 16; j < n; j += 16) {
      k1 += 2;
      k2 = 2 * k1;
      wk2r = w[k1];
      wk2i = w[k1 + 1];
      wk1r = w[k2];
      wk1i = w[k2 + 1];
      wk3r = wk1r - 2 * wk2i * wk1i;
      wk3i = 2 * wk2i * wk1r - wk1i;
      x0r = a[j] + a[j + 2];
      x0i = a[j + 1] + a[j + 3];
      x1r = a[j] - a[j + 2];
      x1i = a[j + 1] - a[j + 3];
      x2r = a[j + 4] + a[j + 6];
      x2i = a[j + 5] + a[j + 7];
      x3r = a[j + 4] - a[j + 6];
      x3i = a[j + 5] - a[j + 7];
      a[j] = x0r + x2r;
      a[j + 1] = x0i + x2i;
      x0r -= x2r;
      x0i -= x2i;
      a[j + 4] = wk2r * x0r - wk2i * x0i;
      a[j + 5] = wk2r * x0i + wk2i * x0r;
      x0r = x1r - x3i;
      x0i = x1i + x3r;
      a[j + 2] = wk1r * x0r - wk1i * x0i;
      a[j + 3] = wk1r * x0i + wk1i * x0r;
      x0r = x1r + x3i;
      x0i = x1i - x3r;
      a[j + 6] = wk3r * x0r - wk3i * x0i;
      a[j + 7] = wk3r * x0i + wk3i * x0r;
      wk1r = w[k2 + 2];
      wk1i = w[k2 + 3];
      wk3r = wk1r - 2 * wk2r * wk1i;
      wk3i = 2 * wk2r * wk1r - wk1i;
      x0r = a[j + 8] + a[j + 10];
      x0i = a[j + 9] + a[j + 11];
      x1r = a[j + 8] - a[j + 10];
      x1i = a[j + 9] - a[j + 11];
      x2r = a[j + 12] + a[j + 14];
      x2i = a[j + 13] + a[j + 15];
      x3r = a[j + 12] - a[j + 14];
      x3i = a[j + 13] - a[j + 15];
      a[j + 8] = x0r + x2r;
      a[j + 9] = x0i + x2i;
      x0r -= x2r;
      x0i -= x2i;
      a[j + 12] = -wk2i * x0r - wk2r * x0i;
      a[j + 13] = -wk2i * x0i + wk2r * x0r;
      x0r = x1r - x3i;
      x0i = x1i + x3r;
      a[j + 10] = wk1r * x0r - wk1i * x0i;
      a[j + 11] = wk1r * x0i + wk1i * x0r;
      x0r = x1r + x3i;
      x0i = x1i - x3r;
      a[j + 14] = wk3r * x0r - wk3i * x0i;
      a[j + 15] = wk3r * x0i + wk3i * x0r;
    }
  }

  void dft::cftmdl(int n, int l, wav_type *a, wav_type *w){
    int j, j1, j2, j3, k, k1, k2, m, m2;
    double wk1r, wk1i, wk2r, wk2i, wk3r, wk3i;
    double x0r, x0i, x1r, x1i, x2r, x2i, x3r, x3i;

    m = l << 2;
    for (j = 0; j < l; j += 2) {
      j1 = j + l;
      j2 = j1 + l;
      j3 = j2 + l;
      x0r = a[j] + a[j1];
      x0i = a[j + 1] + a[j1 + 1];
      x1r = a[j] - a[j1];
      x1i = a[j + 1] - a[j1 + 1];
      x2r = a[j2] + a[j3];
      x2i = a[j2 + 1] + a[j3 + 1];
      x3r = a[j2] - a[j3];
      x3i = a[j2 + 1] - a[j3 + 1];
      a[j] = x0r + x2r;
      a[j + 1] = x0i + x2i;
      a[j2] = x0r - x2r;
      a[j2 + 1] = x0i - x2i;
      a[j1] = x1r - x3i;
      a[j1 + 1] = x1i + x3r;
      a[j3] = x1r + x3i;
      a[j3 + 1] = x1i - x3r;
    }
    wk1r = w[2];
    for (j = m; j < l + m; j += 2) {
      j1 = j + l;
      j2 = j1 + l;
      j3 = j2 + l;
      x0r = a[j] + a[j1];
      x0i = a[j + 1] + a[j1 + 1];
      x1r = a[j] - a[j1];
      x1i = a[j + 1] - a[j1 + 1];
      x2r = a[j2] + a[j3];
      x2i = a[j2 + 1] + a[j3 + 1];
      x3r = a[j2] - a[j3];
      x3i = a[j2 + 1] - a[j3 + 1];
      a[j] = x0r + x2r;
      a[j + 1] = x0i + x2i;
      a[j2] = x2i - x0i;
      a[j2 + 1] = x0r - x2r;
      x0r = x1r - x3i;
      x0i = x1i + x3r;
      a[j1] = wk1r * (x0r - x0i);
      a[j1 + 1] = wk1r * (x0r + x0i);
      x0r = x3i + x1r;
      x0i = x3r - x1i;
      a[j3] = wk1r * (x0i - x0r);
      a[j3 + 1] = wk1r * (x0i + x0r);
    }
    k1 = 0;
    m2 = 2 * m;
    for (k = m2; k < n; k += m2) {
      k1 += 2;
      k2 = 2 * k1;
      wk2r = w[k1];
      wk2i = w[k1 + 1];
      wk1r = w[k2];
      wk1i = w[k2 + 1];
      wk3r = wk1r - 2 * wk2i * wk1i;
      wk3i = 2 * wk2i * wk1r - wk1i;
      for (j = k; j < l + k; j += 2) {
	j1 = j + l;
	j2 = j1 + l;
	j3 = j2 + l;
	x0r = a[j] + a[j1];
	x0i = a[j + 1] + a[j1 + 1];
	x1r = a[j] - a[j1];
	x1i = a[j + 1] - a[j1 + 1];
	x2r = a[j2] + a[j3];
	x2i = a[j2 + 1] + a[j3 + 1];
	x3r = a[j2] - a[j3];
	x3i = a[j2 + 1] - a[j3 + 1];
	a[j] = x0r + x2r;
	a[j + 1] = x0i + x2i;
	x0r -= x2r;
	x0i -= x2i;
	a[j2] = wk2r * x0r - wk2i * x0i;
	a[j2 + 1] = wk2r * x0i + wk2i * x0r;
	x0r = x1r - x3i;
	x0i = x1i + x3r;
	a[j1] = wk1r * x0r - wk1i * x0i;
	a[j1 + 1] = wk1r * x0i + wk1i * x0r;
	x0r = x1r + x3i;
	x0i = x1i - x3r;
	a[j3] = wk3r * x0r - wk3i * x0i;
	a[j3 + 1] = wk3r * x0i + wk3i * x0r;
      }
      wk1r = w[k2 + 2];
      wk1i = w[k2 + 3];
      wk3r = wk1r - 2 * wk2r * wk1i;
      wk3i = 2 * wk2r * wk1r - wk1i;
      for (j = k + m; j < l + (k + m); j += 2) {
	j1 = j + l;
	j2 = j1 + l;
	j3 = j2 + l;
	x0r = a[j] + a[j1];
	x0i = a[j + 1] + a[j1 + 1];
	x1r = a[j] - a[j1];
	x1i = a[j + 1] - a[j1 + 1];
	x2r = a[j2] + a[j3];
	x2i = a[j2 + 1] + a[j3 + 1];
	x3r = a[j2] - a[j3];
	x3i = a[j2 + 1] - a[j3 + 1];
	a[j] = x0r + x2r;
	a[j + 1] = x0i + x2i;
	x0r -= x2r;
	x0i -= x2i;
	a[j2] = -wk2i * x0r - wk2r * x0i;
	a[j2 + 1] = -wk2i * x0i + wk2r * x0r;
	x0r = x1r - x3i;
	x0i = x1i + x3r;
	a[j1] = wk1r * x0r - wk1i * x0i;
	a[j1 + 1] = wk1r * x0i + wk1i * x0r;
	x0r = x1r + x3i;
	x0i = x1i - x3r;
	a[j3] = wk3r * x0r - wk3i * x0i;
	a[j3 + 1] = wk3r * x0i + wk3i * x0r;
      }
    }
  }

};
