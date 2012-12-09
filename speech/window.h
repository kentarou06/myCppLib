#include <vector>
#include "io.h"

namespace speech{

  vector<wav_type> rectangularWindow(int length);

  vector<wav_type> hannWindow(int length);

  vector<wav_type> hammingWindow(int length);

  vector<wav_type> blackmanWindow(int length);
};
