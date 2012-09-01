#include <iostream>
#include <vector>
using namespace std;

typedef double wav_type;

extern vector<wav_type> null_vector;

/* read *.raw  */
bool read(char* filename, int n_Bytes,
	  vector<wav_type> &left,
	  vector<wav_type> &right=null_vector );

/* write *.raw */
bool write(char* filename, int n_Bytes,
	   vector<wav_type> &left,
	   vector<wav_type> &right=null_vector );
