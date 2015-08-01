#ifndef KEMY_QUEUE_HH
#define KEMY_QUEUE_HH
#include "flags.h"
#include "random.h"
#include "template.h"
#include "queue.h"
#include "kemy/whiskertree.hh"
#include "address.h"
class KemyQueue:public Queue
{
    public:
        KemyQueue();
        ~KemyQueue();

    protected:
        WhiskerTree*  _whiskers;
        Memory _memory;
        unsigned int _the_window;
	    int summarystats;

	    int qib_;       	/* bool: queue measured in bytes? */
        PacketQueue *q_;
        void  enque(Packet* );
        Packet*  deque();
};
#endif
