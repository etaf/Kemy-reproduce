#ifndef MEMORY_HH
#define MEMORY_HH

#include <vector>
#include <string>

#include "dna.pb.h"

class Memory {
public:
  typedef double DataType;

private:
  DataType _ewma_qlen;
  double _last_rec_time;

public:
  Memory()
    : _ewma_qlen( 0 ),
      _last_rec_time( 0 )
  {}

 void advance_to( const unsigned int tickno __attribute((unused)) ) {}

  void packet_drop( const double& tickno, const unsigned int & qlen);
  void packet_receive( const double& tickno, const unsigned int & qlen);

  std::string str( void ) const;

  bool operator>=( const Memory & other ) const { return (_ewma_qlen >= other._ewma_qlen); }
  bool operator<( const Memory & other ) const { return (_ewma_qlen <  other._ewma_qlen) ; }
  bool operator==( const Memory & other ) const { return (_ewma_qlen ==  other._ewma_qlen); }

  Memory( const KemyBuffers::Memory & dna );

};


#endif
