#ifndef MEMORYRANGE_HH
#define MEMORYRANGE_HH

#include <string>

#include "memory.hh"
#include "dna.pb.h"

class MemoryRange {
private:
  Memory _lower, _upper;

public:
  MemoryRange( const Memory & s_lower, const Memory & s_upper )
    : _lower( s_lower ), _upper( s_upper ){}


  bool contains( const Memory & query ) const;

  bool operator==( const MemoryRange & other ) const;

  std::string str( void ) const;

  MemoryRange( const KemyBuffers::MemoryRange & dna );

};

#endif
