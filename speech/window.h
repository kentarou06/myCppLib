#include <vector>
#include "io.h"

namespace speech{
  extern const int rectangular_window;
  extern const int hann_window;
  extern const int hamming_window;
  extern const int blackman_window;

  vector<wav_type> getWindow(const int &length,
			     const int window_kind);

  /* TODO: define below code
  vector<wav_type> getWindow(const int &length,
			     const int window_kind=rectangular_window);
  */

  vector<wav_type> rectangularWindow(const int &length);

  vector<wav_type> hannWindow(const int &length);

  vector<wav_type> hammingWindow(const int &length);

  vector<wav_type> blackmanWindow(const int &length);
};
