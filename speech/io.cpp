#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include "io.h"
#include "../utility.h"
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


  /* wav */
  wav::wav(){}
  wav::wav(const string &filename){
    set_filename(filename);
  }

  void wav::set_filename(const string &filename){
    this->filename = filename;
  }
  string wav::get_filename(){
    return filename;
  }

  void wav::set_channel(const int &channel){
    this->channel = channel;
  }
  int wav::get_channel(){
    return channel;
  }

  void wav::set_sampling_frequency(const int &sampling_frequency){
    this->sampling_frequency = sampling_frequency;
  }
  int wav::get_sampling_frequency(){
    return sampling_frequency;
  }

  void wav::set_n_bytes(const int &n_bytes){
    this->n_bytes = n_bytes;
  }
  int wav::get_n_bytes(){
    return n_bytes;
  }

  void wav::set_left_data(const vector<wav_type> &left){
    this->left = left;
  }
  vector<wav_type> wav::get_left_data(){
    return this->left;
  }

  void wav::set_right_data(const vector<wav_type> &right){
    this->right = right;
  }
  vector<wav_type> wav::get_right_data(){
    return this->right;
  }

  // read 4 Byte or bytes Byte
  int read_int(ifstream &ifs, int bytes=4 ){
    char c;
    int r=0;
    for( int i=0;i<bytes;i++ ){
      ifs.read( &c, 1);
      r = r | ( (0xff & c) << i*8 );
    }
    return r;
  }

  //  return chunk_size
  int read_chunk(ifstream &ifs, string &chunk_name){
    char c[5];

    ifs.read( c, 4 ); // chanc name
    c[4] = '\0';
    chunk_name = atos(c);

    return read_int(ifs);
  }

  // read 'fmt ' chunk
  void read_fmt(ifstream &ifs, wav *w){
    read_int( ifs, 2 ); // format ID 1(0x0100)

    w->set_channel( read_int( ifs, 2 ) );         // channel
    w->set_sampling_frequency( read_int( ifs ) ); // sampling frequency

    read_int( ifs );    // data speed [Byte / sec]
    read_int( ifs, 2 ); // block size [n_bytes * channel])
    w->set_n_bytes( read_int( ifs,2 ) / 8 );  // bit/sample [8*n_bytes]
  }

  // read 'data' chunk
  void read_data(ifstream &ifs, wav *w, int size){
    vector<wav_type> left, right;

    const int n_Bytes = w->get_n_bytes();
    const bool isMono = w->get_channel()==1;
    union io::sample smp;
    bool isLeft = true;
    while( size>0 ){
      ifs.read( &smp.c[0], sizeof(smp.c[0]) );
      size -= sizeof(smp.c[0]);

      if( isMono ){
	if( n_Bytes==1 ){ // 1byte 1channel
	  left.push_back( (double)smp.c[0] );
	}else{            // 2byte 1channel
	  ifs.read( &smp.c[1], sizeof(smp.c[1]) );
	  size -= sizeof(smp.c[1]);
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
	  size -= sizeof(smp.c[1]);
	  if( isLeft )
	    left.push_back( (double)smp.s );
	  else
	    right.push_back( (double)smp.s );
	}
	isLeft = !isLeft;
      }
    }
    w->set_left_data(left);
    w->set_right_data(right);
  }

  bool wav::read(){
    ifstream ifs( filename.c_str(), ios::in | ios::binary );
    if( !ifs ){
      cerr << "cannot open file : " << filename << endl;
      return false;
    }

    char c[5];
    int filesize;

    c[4] = '\0';
    ifs.read( c, 4 ); // 'RIFF'
    filesize = read_int(ifs); // filesize - 8
    ifs.read( c, 4 ); // 'WAVE'

    string chunk_name;
    int chunk_size;
    while( filesize > 0 ){
      chunk_size = read_chunk(ifs, chunk_name);
      filesize -= 8;
      filesize -= chunk_size;

      if( filesize < 0 )
	break;

      if( chunk_name == "fmt " ){
	read_fmt(ifs, this);
      }else if( chunk_name == "data" ){
	read_data(ifs, this, chunk_size);
      }else{
	cerr << "unknown chunk name [" << chunk_name << "]" <<  endl;
	ifs.seekg( chunk_size, ios::cur );
      }
    }
    ifs.close();

    return true;
  }

  // write 4 Byte of bytes Byte
  void write_int(int write_data, ofstream &ofs, int bytes=4){
    char c;
    for( int i=0;i<bytes;i++ ){
      c = (char)( 0xff & (write_data >> i*8) );
      ofs.write( &c, 1 );
    }
  }

  bool write_fmt(ofstream &ofs, wav *w){
    ofs.write( "fmt ", 4 );
    write_int( 16, ofs ); // chunk size

    write_int( 1, ofs, 2 ); // format ID
    write_int( w->get_channel(), ofs, 2 );
    write_int( w->get_sampling_frequency(), ofs );

    write_int( w->get_channel() * w->get_n_bytes()
	       * w->get_sampling_frequency(), ofs );
    write_int( w->get_n_bytes() * w->get_channel(), ofs, 2 );
    write_int( 8 * w->get_n_bytes(), ofs, 2 );

    return true;
  }

  bool write_data(ofstream &ofs, wav *w){
    ofs.write( "data", 4 );
    write_int( w->get_channel() * w->get_n_bytes()
	       * w->get_left_data().size(), ofs );
    vector<wav_type> left  = w->get_left_data();
    vector<wav_type> right = w->get_right_data();

    const bool isMono = w->get_channel()==1;
    const int n_Bytes = w->get_n_bytes();
    if( !isMono && left.size()!=right.size() ){
      cerr << "different #sample between left and right" << endl;
      return false;
    }


    const int maxSample = n_Bytes==1?127:32767;
    double max = 0.0, tmp, exceedValue = 0.0;
    bool isExceed = false;
    union io::sample smp;
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
    if( isExceed ){
      cerr << "exceed value : "
	   << exceedValue << " limit : "
	   << maxSample << endl;

      return false;
    }

    return true;
  }


  bool wav::write(){
    ofstream ofs( filename.c_str(), ios::out | ios::binary );
    if( !ofs ){
      cerr << "cannot open file : " << filename << endl;
      return false;
    }

    int filesize = 4 + 16+8 + left.size()*channel*n_bytes+8; // WAVE,fmt,data
    ofs.write( "RIFF", 4);
    write_int( filesize, ofs );
    ofs.write( "WAVE", 4);

    if( !write_fmt(ofs, this) ){
      cerr << "fmt writing err" << endl;
      return false;
    }
    if( !write_data(ofs, this) ){
      cerr << "data writing err" << endl;
      return false;
    }

    ofs.close();

    return true;
  }

};

