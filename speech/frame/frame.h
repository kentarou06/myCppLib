#include <vector>
#include "speech/io/io.h"

namespace speech{

  /* 音声ファイルに対して、フレームの切り出しを行う．   */
  class frame{
  private:
    int fileNameSize;
    char* filename;
    int sampling_freq, shift_msec, window_msec, n_Bytes;
    bool isStereo;

  public:
    frame();
    frame(const char* filename,
	  const int sampling_freq,
	  const int shift_msec,
	  const int window_msec,
	  const int n_Bytes = 2,
	  const bool isStereo = false );

    /* run main loop */
    void run();
    /* this method should be inherited */
    virtual void oneOfFrame(int &frame,
		    vector<wav_type> &left,
		    vector<wav_type> &right = speech::null_vector );

    const char* getFileName();
    int getSamplingFrequency();
    int getShiftMSec();
    int getWindowMSec();
    int getNShiftSamples();
    int getNWindowSamples();

    int getMSec(const int frame);

    void setFileName(const char* filename,
		     const int n_Bytes = 2,
		     const bool isStereo = false );
    void setSamplingFrequency(const int sampling_freq);
    void setShiftMSec(const int shift_msec);
    void setWindowMSec(const int window_msec);
  };
}
