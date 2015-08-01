#include "kemy-queue.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

static class KemyQueueClass : public TclClass{
    public:
        KemyQueueClass() : TclClass("Queue/KEMY"){}
        TclObject* create(int, const char*const*){
            return (new KemyQueue);
        }
}class_kemy;
KemyQueue::KemyQueue() :
        _whiskers(NULL),
        _memory(),
        _the_window(0)
        {
		bind_bool("summarystats_", &summarystats);
		bind_bool("queue_in_bytes_", &qib_);  // boolean: q in bytes?
            const char * filename = getenv("WHISKERS");
        	if ( !filename ) {
        		fprintf( stderr, "KemyQueue: Missing WHISKERS environment variable.\n" );
        		throw 1;
        	}

        	/* open file */
        	int fd = open( filename, O_RDONLY );
        	if ( fd < 0 ) {
        		perror( "open" );
        		throw 1;
        	}

        	/* parse whisker definition */
        	KemyBuffers::WhiskerTree tree;
        	if ( !tree.ParseFromFileDescriptor( fd ) ) {
        		fprintf( stderr, "KemyQueue: Could not parse whiskers in \"%s\".\n", filename );
        		throw 1;
        	}

        	/* close file */
        	if ( ::close( fd ) < 0 ) {
        		perror( "close" );
        		throw 1;
        	}

        	/* store whiskers */
        	_whiskers = new WhiskerTree( tree );

	        q_ = new PacketQueue();			    // inner queue
            pq_ = q_;

        }

KemyQueue::~KemyQueue(){
    delete q_;
}

void KemyQueue::enque(Packet* p)
{
	if (summarystats) {
                Queue::updateStats(qib_?q_->byteLength():q_->length());
	}

    hdr_ip* iph = hdr_ip::access(p);
    if (q_->length()+1 > qlim_){
        drop(p);
        return;
    }
    double tickno = Scheduler::instance().clock();
    const Whisker & current_whisker(_whiskers->use_whisker(_memory));
     _the_window = current_whisker.window(_the_window);
    if( q_->length() >= _the_window)
    {
        _memory.packet_drop(tickno, q_->length());
        drop(p);
    }
    else
    {
        _memory.packet_receive(tickno, q_->length());
        q_->enque(p);
    }
}
Packet* KemyQueue::deque()
{
	if (summarystats) {
                Queue::updateStats(qib_?q_->byteLength():q_->length());
	}

    return q_->deque();
}
