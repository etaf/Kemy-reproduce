/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
/*
 * Copyright (c) 1994 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the Computer Systems
 *	Engineering Group at Lawrence Berkeley Laboratory.
 * 4. Neither the name of the University nor of the Laboratory may be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*Ported to ns2.1b8 by Sunil Thulasidasan , LANL ; 10/19/2001 */
/*Last modified on 09/11/02*/

#include <math.h>
#include <stdlib.h>
#include <sys/types.h>
#include "blue.h"
#include "random.h"
#include "delay.h"
#include "flags.h"

static class BlueClass : public TclClass {

 public:
	BlueClass() : TclClass("Queue/Blue") {}
	TclObject* create(int, const char*const*) {
		return (new Blue);
	}
} class_blue;


Blue::Blue() { 
	q_ = new PacketQueue();
	pq_ = q_;
	bind_bool("drop_front_", &drop_front_);
	bind_bool("bytes", &bytes_ );
	bind_bool("setbit", &setbit_);
	bind("decrement", &decrement_);
	bind("increment", &increment_);
	bind_time("dhold-time", &dholdtime_);
	bind_time("ihold-time", &iholdtime_);
	bind("dalgorithm", &dalgorithm_);
	bind("ialgorithm", &ialgorithm_);
	bind("pmark", &pmark_);
	bind("pktsize", &mean_pktsize_);
}


Blue::~Blue() {
	{
		delete q_;
	}
	
}

int 
Blue::command(int argc, const char*const* argv) {
		
	Tcl& tcl = Tcl::instance();
	if (argc == 3) {
		if (!strcmp(argv[1], "packetqueue-attach")) {
			delete q_;
			if (!(q_ = (PacketQueue*) TclObject::lookup(argv[2])))
				return (TCL_ERROR);
			else {
				pq_ = q_;
				return (TCL_OK);
			}
		}
		
		if (strcmp(argv[1], "link") == 0) {
			LinkDelay* link_  = (LinkDelay*)TclObject::lookup(argv[2]);
			if (link_ == 0) {
				tcl.resultf("Blue : No link delay Object %s\n",
					    argv[2]);
				return(TCL_ERROR);
			}
			bandwidth_ = link_->bandwidth();
			return(TCL_OK);
		}
	}
	
	return Queue::command(argc, argv);
}


void
Blue::reset()
{
	Queue::reset();
	ptc_ = (bandwidth_ / (8 * mean_pktsize_));
	idle_ = 1;
	idletime_ = Scheduler::instance().clock();
	pmark_ = 0;
	ifreezetime_ = 0;
	dfreezetime_ = 0;
}


void 
Blue::decrement_pmark(int how)
{
	double now = Scheduler::instance().clock();
	if (now - dfreezetime_ > dholdtime_) {
		dfreezetime_ = now;
    	switch (dalgorithm_) {
    	case 0:
	case 2:
		switch (how) {
			case 0:
        			pmark_ -= decrement_;
				break;
			case 1:
			default:
				break;
		}
		break;
    	default:
    	case 1:
		switch (how) {
			case 0:
        		pmark_ -= decrement_;
				break;
			case 1:
			default:
        			pmark_ -= decrement_/10;
				break;
		}
    	}
    	if (pmark_ < 0)
		pmark_ = 0.0;
    }
}


void 
Blue::increment_pmark(int how)
{
    double now = Scheduler::instance().clock();

    if (now - ifreezetime_ > iholdtime_) {
    	ifreezetime_ = now;
    	switch (ialgorithm_) {
    	case 0:
		switch (how) {
			case 0:
        			pmark_ += increment_;
				break;
			case 1:
			default:
				break;
		}
		break;
	case 2:
		switch (how) {
			case 0:
        			pmark_ = 2*pmark_ + increment_;
				break;
			case 1:
			default:
				break;
		}
    	default:
    	case 1:
		switch (how) {
			case 0:
        			pmark_ += increment_;
				break;
			case 1:
			default:
        			pmark_ += increment_/10;
				break;
		}
    	}
    	if (pmark_ > 1.0) 
		pmark_ = 1.00;
    }
}



 
void 
Blue::enque(Packet* p)
{

	double now = Scheduler::instance().clock();
	int m;

	if (idle_) {
		idle_ = 0;
		m = int(ptc_ * (now - idletime_));
	}
	else 
		m = 0;
	
	if (drop_early(p)) {
		drop(p);
		//return;
	}

	else {
		q_->enque(p);
		if (q_->length() >= qlim_) {
			
			increment_pmark(0);
			q_->remove(p);
			drop(p);
			
		}
	}
}


Packet* 
Blue::deque()
{

	
	Packet* p = q_->deque();


	if (p != 0) {
		idle_ = 0;
	}
	else {
		decrement_pmark(0);
		idle_ = 1;
		idletime_ = Scheduler::instance().clock();
	}
	return (p);
}

int
Blue::drop_early(Packet *p)
{

	hdr_flags* hf = hdr_flags::access(p);
	double u = Random::uniform();

	if (u <= pmark_) {
			if (setbit_ && hf->ect()) {
				hf->ce() = 1; /*set ECN bit here*/
			}
			else return (1);
	}
	return(0);
}
