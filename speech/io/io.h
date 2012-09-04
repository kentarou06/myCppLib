/*  Read and Write .raw file

  signed sample
    1Byte : -128 ~ 127
    2Byte : -32768 ~ 32767
  #channels
    1 or 2 ( mono or stereo )
*/

#include <iostream>
#include <vector>
using namespace std;

namespace speech{
  /* variable type of samples */
  typedef double wav_type;

  /* when #channel equals 1, right is set null_vector. */
  extern vector<wav_type> null_vector;

  class io{
  private:
    union sample{ short s; char c[2]; };

  public:

    /* read *.raw
       WARNING
       before reading, left and right are done clear()
    */
    static bool read(const char* filename, const int n_Bytes,
	      vector<wav_type> &left,
	  vector<wav_type> &right=null_vector );

    /* write *.raw */
    static bool write(const char* filename, const int n_Bytes,
	       const vector<wav_type> &left,
	       const vector<wav_type> &right=null_vector );
  };
};
