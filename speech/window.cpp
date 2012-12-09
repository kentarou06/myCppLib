#include <vector>
#include <cmath>
#include "io.h"
#include "window.h"

namespace speech{
  vector<wav_type> rectangularWindow(int length){
    vector<wav_type> window(length);
    const double coe = 2.0 * M_PI / ( length - 1 );
    for( int i=0;i<length;i++ )
      window[i] = 1.0;
    return window;
  }

  vector<wav_type> hannWindow(int length){
    vector<wav_type> window(length);
    const double coe = 2.0 * M_PI / ( length - 1 );
    for( int i=0;i<length;i++ )
      window[i] = 0.5 + 0.5 * cos( coe * i );
    return window;
  }

  vector<wav_type> hammingWindow(int length){
    vector<wav_type> window(length);
    const double coe = 2.0 * M_PI / ( length - 1 );
    for( int i=0;i<length;i++ )
      window[i] = 0.54 + 0.46 * cos( coe * i );
    return window;
  }

  vector<wav_type> blackmanWindow(int length){
    vector<wav_type> window(length);
    const double coe1 = 2.0 * M_PI / ( length - 1 );
    const double coe2 = 2.0 * coe1;
    for( int i=0;i<length;i++ )
      window[i] = 0.42 + 0.5 * cos( coe1 * i ) + 0.08 * cos( coe2 * i );
    return window;
  }
};
