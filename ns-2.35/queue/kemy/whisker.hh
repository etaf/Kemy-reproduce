#ifndef WHISKER_HH
#define WHISKER_HH

#include <string>
#include <vector>

#include "memoryrange.hh"
#include "dna.pb.h"

class Whisker {
private:
  int _window_increment;
  double _window_multiple;

  MemoryRange _domain;

public:
  unsigned int window( const unsigned int previous_window ) const { return std::min( std::max( 0, int( previous_window * _window_multiple + _window_increment ) ), 1000000 ); }
  const MemoryRange & domain( void ) const { return _domain; }


  std::string str(void) const;


  Whisker( const KemyBuffers::Whisker & dna );

};

#endif
