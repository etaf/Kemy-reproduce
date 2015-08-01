#include<stdio.h>
#include "whisker.hh"

using namespace std;

string Whisker::str( void ) const
{
  char tmp[ 256 ];
  snprintf( tmp, 256, "{%s} => (win=%d + %f * win)",
	    _domain.str().c_str(),  _window_increment, _window_multiple);
  return tmp;
}

Whisker::Whisker( const KemyBuffers::Whisker & dna )
  : _window_increment( dna.window_increment() ),
    _window_multiple( dna.window_multiple() ),
    _domain( dna.domain() )
{
}


