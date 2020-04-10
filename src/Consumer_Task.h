// -*- C++ -*-
/*
 * Copyright (c) 2007 Jani J. Hakala <jahakala@iki.fi>
 * All rights reserved.
 */
#ifndef CONSUMER_TASK_H
#define CONSUMER_TASK_H

#include <ace/ACE.h>
#include <ace/Task.h>

#include <tao/ORB.h>

class JYFLTRAP_Consumer;

class Consumer_Task : public ACE_Task_Base {
public:
    Consumer_Task(JYFLTRAP_Consumer * consumer,
		  unsigned int interval=0);

    virtual ~Consumer_Task();
    virtual int put(ACE_Message_Block *, ACE_Time_Value * = 0);
    virtual int shutdown(void);

    virtual int svc(void);

    JYFLTRAP_Consumer * get_consumer() const { return this->consumer; }
protected:
    JYFLTRAP_Consumer * consumer;
    unsigned int interval;
    ACE_Message_Queue<ACE_MT_SYNCH>	queue;
};

#endif
