#include <vector>
#include <cmath>
#include "io.h"
#include "window.h"
using namespace std;

namespace speech{

  const int rectangular_window = 1;
  const int hann_window        = 2;
  const int hamming_window     = 3;
  const int blackman_window    = 4;

  vector<wav_type> getWindow(const int &length,
			     const int window_kind){
    switch( window_kind ){
    case rectangular_window: return rectangularWindow(length);
    case hann_window:        return hannWindow(length);
    case hamming_window:     return hammingWindow(length);
    case blackman_window:    return blackmanWindow(length);
    }
    return null_vector;
  }

  vector<wav_type> rectangularWindow(const int &length){
    vector<wav_type> window(length);
    const double coe = 2.0 * M_PI / ( length - 1 );
    for( int i=0;i<length;i++ )
      window[i] = 1.0;
    return window;
  }

  vector<wav_type> hannWindow(const int &length){
    vector<wav_type> window(length);
    const double coe = 2.0 * M_PI / ( length - 1 );
    for( int i=0;i<length;i++ )
      window[i] = 0.5 + 0.5 * cos( coe * i );
    return window;
  }

  vector<wav_type> hammingWindow(const int &length){
    vector<wav_type> window(length);
    const double coe = 2.0 * M_PI / ( length - 1 );
    for( int i=0;i<length;i++ )
      window[i] = 0.54 + 0.46 * cos( coe * i );
    return window;
  }

  vector<wav_type> blackmanWindow(const int &length){
    vector<wav_type> window(length);
    const double coe1 = 2.0 * M_PI / ( length - 1 );
    const double coe2 = 2.0 * coe1;
    for( int i=0;i<length;i++ )
      window[i] = 0.42 + 0.5 * cos( coe1 * i ) + 0.08 * cos( coe2 * i );
    return window;
  }
};
