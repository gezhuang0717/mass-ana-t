// -*- C++ -*- 
/*
 * Copyright (c) 2007 Jani J. Hakala <jahakala@iki.fi>
 * All rights reserved.
 */
#define ACE_NTRACE   1
// #define ACE_NDEBUG   1
// #define ACE_NLOGGING 1

#include "JYFLTRAPC.h"
#include "JYFLTRAP_Consumer.h"
#include "Consumer_Task.h"

Consumer_Task::
Consumer_Task(
    JYFLTRAP_Consumer * consumer,
    unsigned int interval) : consumer(consumer),
			     interval(interval) 
{ 
    ACE_TRACE(ACE_TEXT("Consumer_Task"));
}

Consumer_Task::
~Consumer_Task() 
{
    ACE_TRACE(ACE_TEXT("~Consumer_Task"));
}

int 
Consumer_Task::
put(ACE_Message_Block * mb, ACE_Time_Value * tv)
{
    return this->queue.enqueue_tail(mb, tv);
}

int 
Consumer_Task::
shutdown(void)
{
    ACE_Message_Block * mb;
    mb = new ACE_Message_Block(0, ACE_Message_Block::MB_STOP);
    return queue.enqueue_head(mb);
}

int 
Consumer_Task::
svc(void) 
{ 
    ACE_Message_Block * mblk = NULL;
    while ( 1 ) {
	if (queue.dequeue_head(mblk) == -1) {
	    if (errno != EWOULDBLOCK) {
		break;
	    }
	}
	if (mblk->msg_type() == ACE_Message_Block::MB_STOP) {
	    ACE_DEBUG((LM_DEBUG, "svc MB_STOP\n"));
	    mblk->release();

	    return 0;
	} 
    }
    return 0; 
}
