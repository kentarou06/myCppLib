#include <iostream>
#include <vector>
#include <cmath>
#include "lpc.h"
#include "io.h"
#include "dft.h"
using namespace std;

namespace speech{

#ifndef EPS
#define EPS 1.0e-9
#endif

  /*    private method   */
  void LPC::init(){
    p = -1;
    en = 0.0;
    signal.clear();
    parcor.clear();
    lpc.clear();
    r.clear();
  }

  /*   constructor   */
  LPC::LPC(){
    init();
  };
  LPC::LPC(const vector<wav_type> &signal){
    init();
    setSignal( signal );
  }

  /*  setter   */
  void LPC::setSignal(const vector<wav_type> &signal){
    init();
    this->signal.reserve( signal.size() );
    for( int i=0;i<(int)signal.size();i++ )
      this->signal.push_back( signal[i] );
  }
  bool LPC::setLPC(const vector<wav_type> lpc){
    if( lpc.empty() )
      return false;

    init();
    p = lpc.size();

    this->lpc.reserve( p );
    for( int i=0;i<p;i++ )
      this->lpc.push_back( lpc[i] );

    return true;
  }


  /*            getter            */

  bool LPC::getSignal(vector<wav_type> &signal){
    if( this->signal.empty() )
      return false;

    signal.clear();
    signal.reserve( this->signal.size() );
    for( int i=0;i<(int)this->signal.size();i++ )
      signal.push_back( this->signal[i] );

    return true;
  }

  /* if not call anaysis(), below functions return false */
  bool LPC::getPARCOR(vector<wav_type> &parcor){
    if( this->parcor.empty() )
      return false;

    parcor.clear();
    parcor.reserve( this->parcor.size() );
    for( int i=0;i<(int)this->parcor.size();i++ )
      parcor.push_back( this->parcor[i] );

    return true;
  }

  bool LPC::getLPC(vector<wav_type> &lpc){
    if( this->lpc.empty() )
      return false;

    lpc.clear();
    lpc.reserve( this->lpc.size() );
    for( int i=0;i<(int)this->lpc.size();i++ )
      lpc.push_back( this->lpc[i] );

    return true;
  }

  bool LPC::getAutocorrelation(vector<wav_type> &r){
    if( this->r.empty() )
      return false;

    r.clear();
    r.reserve( this->r.size() );
    for( int i=0;i<(int)this->r.size();i++ )
      r.push_back( this->r[i] );

    return true;
  }
  bool LPC::getResidue(vector<wav_type> &residue){
    if( this->lpc.empty() || this->signal.empty() )
      return false;

    residue.clear();
    residue.resize( this->signal.size() );
    for( int i=0;i<(int)this->signal.size();i++ ){
      residue[i] = signal[i];
      for( int k=1;k<(int)lpc.size()&&i-k>=0;k++ )
	     residue[i] -= lpc[k] * signal[i-k];
    }
    return true;
  }
  int LPC::getOrder(){
    return p;
  }

  bool LPC::getSpectral(vector<wav_type> &spec, int size){
    if( this->lpc.empty() )
      return false;

    vector< complex<wav_type> > coe, tmp;
    coe.reserve( 2*size );
    for( int i=0;i<2*size;i++ ){
      if( i<lpc.size() )
	coe.push_back( complex<wav_type>( lpc[i], 0.0 ) );
      else
	coe.push_back( complex<wav_type>( 0.0, 0.0 ) );
    }

    tmp = dft::cdft( coe );
    spec.clear();
    spec.reserve( size );
    for( int i=0;i<size;i++ )
      spec[i] = 1.0 / abs( tmp[i] );

    return true;
  }

  bool LPC::analysis(const int p){
    if( p<=0 )
      return false;

    this->p = p;

    parcor.clear();
    parcor.reserve( p+1 );
    parcor.resize( p+1, 0.0 );
    lpc.clear();
    lpc.reserve( p+1 );
    lpc.resize( p+1, 0.0 );
    r.clear();
    r.reserve( signal.size() );
    r.resize( signal.size(), 0.0 );

    /* get autocorrelation coefficients */
    for( int k=0;k<p+1;k++ ){
      r[k] = 0.0;
      for( int np=0;np<signal.size()-k;np++ )
	r[k] += signal[np] * signal[np+k];
    }

    /* get LP coefficients and PARCOR coefficients */
    wav_type b[p+2];
    double w;
    en = r[0];
    parcor[0] = r[1] / r[0];
    lpc[0] = 1.0;
    lpc[1] = -parcor[0];
    en = en - parcor[0] * parcor[0] * en;
    for( int mm=2;mm<=p;mm++ ){
      for( int j=0;j<mm;j++ )
	b[j] = lpc[mm-j-1];
      w=0.0;
      for( int ip=0;ip<mm;ip++ )
	w += r[mm-ip] * lpc[ip];
      parcor[mm-1] = w / en;
      for( int ip=1;ip<mm;ip++ )
	lpc[ip] -= parcor[mm-1] * b[ip-1];

      lpc[mm] = -parcor[mm-1];
      en -= parcor[mm-1] * parcor[mm-1] * en;
      if( en<0 || fabs(en)<EPS )
	break;
    }
    return true;
  }


  /* LPC synthesis */
  bool LPC::filter( const vector<wav_type> &input,
	       vector<wav_type> &output ){
    if( lpc.empty() )
      return false;

    output.clear();
    output.resize( input.size() );
    for( int i=0;i<(int)input.size();i++ ){
      output[i] = input[i];
      for( int k=1;k<(int)lpc.size()&&i-k>=0;k++ )
        output[i] -= lpc[k] * output[i-k];
    }
    return true;
  }



};
