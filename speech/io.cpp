#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include "io.h"
using namespace std;

namespace speech{

  vector<wav_type> null_vector;

  /* read *.raw */
  bool io::read(const char* filename, const int n_Bytes,
	    vector<wav_type> &left, vector<wav_type> &right){

    if( n_Bytes!=1 && n_Bytes!=2 ){
      cerr << "n_Bytes must be 1 or 2" << endl;
      return false;
    }

    ifstream ifs( filename, ios::in | ios::binary );
    if( !ifs ){
      cerr << "cannot open file : " << filename << endl;
      return false;
    }

    left.clear();  right.clear();
    bool isMono = &null_vector==&right;
    bool isLeft = true;
    union sample smp;
    while( !ifs.eof() ){
      ifs.read( &smp.c[0], sizeof(smp.c[0]) );

      if( isMono ){
	if( n_Bytes==1 ){ // 1byte 1channel
	  left.push_back( (double)smp.c[0] );
	}else{            // 2byte 1channel
	  ifs.read( &smp.c[1], sizeof(smp.c[1]) );
	  left.push_back( (double)smp.s );
	}
      }else{
	if( n_Bytes==1 ){ // 1byte 2channel
	  if( isLeft )
	    left.push_back( (double)smp.c[0] );
	  else
	    right.push_back( (double)smp.c[0] );
	}else{            // 2byte 2channel
	  ifs.read( &smp.c[1], sizeof(smp.c[1]) );
	  if( isLeft )
	    left.push_back( (double)smp.s );
	  else
	    right.push_back( (double)smp.s );
	}
	isLeft = !isLeft;
      }
    }
    ifs.close();
    return true;
  }

  /* write *.raw */
  bool io::write(const char* filename, const int n_Bytes,
		 const vector<wav_type> &left,
		 const vector<wav_type> &right){

    if( n_Bytes!=1 && n_Bytes!=2 ){
      cerr << "n_Bytes must be 1 or 2" << endl;
      return false;
    }

    ofstream ofs( filename, ios::out | ios::binary );
    if( !ofs ){
      cerr << "cannot open file : " << filename << endl;
      return false;
    }

    bool isMono = &null_vector==&right;
    if( !isMono && left.size()!=right.size() ){
      cerr << "different #sample between left and right" << endl;
      return false;
    }


    const int maxSample = n_Bytes==1?127:32767;
    double max = 0.0, tmp, exceedValue = 0.0;
    bool isExceed = false;
    union sample smp;
    for( int i=0;i<(int)left.size();i++ ){
      tmp = left[i];
      if( !isExceed && fabs(tmp) > maxSample ){
	isExceed = true;
	exceedValue = fabs( tmp );
      }

      if( n_Bytes==1 ){ // 1byte 1channel
	smp.c[0] = (signed char)tmp;
	ofs.write( &smp.c[0], sizeof(char) );
      }else{            // 2byte 1channel
	smp.s = (short)tmp;
	ofs.write( &smp.c[0], sizeof(char) );
	ofs.write( &smp.c[1], sizeof(char) );
      }

      if( !isMono ){
	tmp = right[i];
	if( !isExceed && fabs(tmp) > maxSample ){
	  isExceed = true;
	  exceedValue = fabs( tmp );
	}
	if( n_Bytes==1 ){ // 1byte 2channel
	  smp.c[0] = (signed char)tmp;
	  ofs.write( &smp.c[0], sizeof(char) );
	}else{            // 2byte 2channel
	  smp.s = (short)tmp;
	  ofs.write( &smp.c[0], sizeof(char) );
	  ofs.write( &smp.c[1], sizeof(char) );
	}
      }
    }
    if( isExceed )
      cerr << "exceed value : "
	   << exceedValue << " limit : "
	   << maxSample << endl;
    ofs.close();
    return true;;
  }
};