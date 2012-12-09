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

#ifndef READ_IO
#define READ_IO
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

  class wav{
  private:
    string filename;
    int channel, sampling_frequency, n_bytes;
    vector<wav_type> left, right;

  public:
    wav();
    wav(const char* filename);

    void set_filename(const string &filename);
    string get_filename();

    void set_channel(const int &channel);
    int get_channel();

    void set_sampling_frequency(const int &sampling_frequency);
    int get_sampling_frequency();

    void set_n_bytes(const int &n_bytes);
    int get_n_bytes();

    void set_left_data(const vector<wav_type> &left);
    vector<wav_type> get_left_data();

    void set_right_data(const vector<wav_type> &right);
    vector<wav_type> get_right_data();

    bool read();
    bool write();
  };
};
#endif
