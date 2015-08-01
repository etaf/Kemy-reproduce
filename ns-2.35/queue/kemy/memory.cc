#include <vector>
#include <assert.h>

#include "memory.hh"

using namespace std;

static const double alpha = 0.5;

void Memory::packet_drop(const double& tickno,const unsigned int & qlen)
{
    _ewma_qlen = (1 - alpha) * _ewma_qlen + alpha * qlen;

    _last_rec_time = tickno;


}
void Memory::packet_receive(const double& tickno,const unsigned int & qlen)
{
    _ewma_qlen = (1-alpha) * _ewma_qlen + alpha * qlen;

    _last_rec_time = tickno;

}
string Memory::str( void ) const
{
  char tmp[ 256 ];
  snprintf( tmp, 256, "ewma_qlen=%f", _ewma_qlen);
  return tmp;
}

Memory::Memory( const KemyBuffers::Memory & dna )
  : _ewma_qlen(  dna.ewma_qlen() ),
    _last_rec_time( 0 )
{
}


