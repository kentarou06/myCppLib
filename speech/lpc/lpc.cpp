#include <iostream>
#include <vector>
#include "lpc.h"
#include "speech/io/io.h"
using namespace std;

namespace speech{
  /*
  vector<wav_type> LPC::signal;
  vector<wav_type> LPC::parcor;
  vector<wav_type> LPC::lpc;
  vector<wav_type> LPC::r;
  int LPC::p;
  */
  LPC::LPC(){
    init();
  };
  LPC::LPC(const vector<wav_type> &signal):signal(signal){
    init();
  }

  void LPC::init(){
    p = -1;
  }

  void LPC::setSignal(const vector<wav_type> &signal){
  }

  bool LPC::analysis(const int p){
    if( p<=0 )
      return false;

    return false;
  }
  /* if not call anaysis(), below functions return false */
  bool LPC::getPARCOR(vector<wav_type> &parcor){
    if( p<=0 )
      return false;

    return false;
  }
  bool LPC::getLPC(vector<wav_type> &lpc){
    if( p<=0 )
      return false;

    return false;
  }
  bool LPC::getAutocorrelation(vector<wav_type> &r){
    if( p<=0 )
      return false;

    return false;
  }

  int LPC::getOrder(){
    return p;
  }
};
