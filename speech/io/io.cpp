#include <vector>
#include "io.h"
using namespace std;

vector<wav_type> null_vector;

/* read *.raw */
bool read(char* filename, int n_Bytes,
	  vector<wav_type> &left, vector<wav_type> &right){

  if( &null_vector==&right )
    cout << " channel = 1" << endl;
  else
    cout << " channel = 2" << endl;

  return false;
}

/* write *.raw */
bool write(char* filename, int n_Bytes,
	   vector<wav_type> &left, vector<wav_type> &right){

  if( &null_vector==&right )
    cout << " channel = 1" << endl;
  else
    cout << " channel = 2" << endl;

  return false;
}

