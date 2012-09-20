#include <iostream>
#include <vector>
#include "speech/frame/frame.h"
#include "speech/io/io.h"

using namespace std;
using namespace speech;

namespace speech{
  frame::frame(){  }
  frame::frame(const char* filename,
	       const int sampling_freq,
	       const int shift_msec,
	       const int window_msec,
	       const int n_Bytes,
	       const bool isStereo ){
    setFileName(filename, n_Bytes, isStereo );
    setSamplingFrequency(sampling_freq);
    setShiftMSec(shift_msec);
    setWindowMSec(window_msec);
  }

  /* run main loop */
  void frame::run(){

    vector<wav_type> left, right;
    if( isStereo ){
      if( !io::read( filename, n_Bytes, left, right ) ){
	cout << "failed to reading file : " << filename << endl;
	return;
      }
    }else{ // mono
      if( !io::read( filename, n_Bytes, left ) ){
	cout << "failed to reading file : " << filename << endl;
	return;
      }
    }

    const int shiftSize  = getNShiftSamples();
    const int windowSize = getNWindowSamples();
    vector<wav_type> lSeg(windowSize), rSeg;
    if( isStereo ) rSeg.resize( windowSize );

    for( int i=0,frame=0; i+windowSize<(int)left.size();i+=shiftSize,frame++){
      for( int j=0;j<windowSize;j++ ){
	lSeg[j] = left[i+j];
	if( isStereo )
	  rSeg[j] = right[i+j];
      }
      this->oneOfFrame( frame, lSeg, rSeg );
    }
  }

  /* this method should be inherited */
  void frame::oneOfFrame(int &frame,
		  vector<wav_type> &left,
		  vector<wav_type> &right ){
    // do nothing ( virtual method )
  }

  /*  getter */
  const char* frame::getFileName(){
    if( filename == 0 )
      return "";
    return filename;
  }
  int frame::getSamplingFrequency(){ return sampling_freq; }
  int frame::getShiftMSec(){ return shift_msec; }
  int frame::getWindowMSec(){ return window_msec; }
  int frame::getNShiftSamples(){ return sampling_freq * shift_msec / 1000; }
  int frame::getNWindowSamples(){ return sampling_freq * window_msec / 1000; }

  int frame::getMSec(const int frame){
    return frame * getNShiftSamples() * 1000 / sampling_freq;
  }

  /* setter */
  void frame::setFileName(const char* filename,
			  const int n_Bytes,
			  const bool isStereo ){
    this->n_Bytes = n_Bytes;
    this->isStereo = isStereo;

    if( fileNameSize= 0 ){
      delete[] this->filename;
    }

    fileNameSize = 0;
    while( filename[fileNameSize]!='\0' ) fileNameSize++;
    this->filename = new char[ fileNameSize+1 ];
    for( int i=0;i<fileNameSize;i++ )
      this->filename[i] = filename[i];
    this->filename[fileNameSize] = '\0';
  }
  void frame::setSamplingFrequency(const int sampling_freq){
    this->sampling_freq = sampling_freq;
  }
  void frame::setShiftMSec(const int shift_msec){
    this->shift_msec = shift_msec;
  }
  void frame::setWindowMSec(const int window_msec){
    this->window_msec = window_msec;
  }
}
