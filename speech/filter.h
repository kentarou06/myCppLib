#include <vector>
#include "io.h"
#include "window.h"
using namespace std;

namespace speech{
  vector<wav_type> LPF(const vector<wav_type> &signal,
		       const double &cutoff_frequency,
		       const int &sampling_frequency,
		       const int &fft_length = 1024,
		       const int &window_kind = rectangular_window);

};
