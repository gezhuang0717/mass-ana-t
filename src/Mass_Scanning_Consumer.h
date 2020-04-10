// -*- C++ -*-
/*
 * Copyright (c) 2007 Jani J. Hakala <jahakala@iki.fi>
 * All rights reserved.
 */
#ifndef MASS_SCANNING_CONSUMER_H
#define MASS_SCANNING_CONSUMER_H

#include <string>
#include <stdexcept>

#ifdef __G_WIN32_H__
#undef pipe
#endif

#include <ace/Synch.h>
#include <ace/Refcounted_Auto_Ptr.h>
#include <ace/Unbounded_Queue.h>

#include "JYFLTRAP_Consumer.h"
#include "Event_Data.h"

class ORB_Task;

class alarm_callback {
public:
    virtual ~alarm_callback() { }
    virtual void alarm(void) = 0;
};

class noevents_error : public std::runtime_error {
public:
    noevents_error(const std::string & info) : std::runtime_error(info) { }
};

class snafu_error : public std::runtime_error {
public:
    snafu_error(const std::string & info) : std::runtime_error(info) { }
};

namespace Mass_Scanning {
    class Consumer : protected JYFLTRAP_Consumer {
public:
	Consumer(int & argc, char *argv[], 
		 alarm_callback * cb);
	virtual ~Consumer();
	
	virtual Event_ptr get_event() throw(noevents_error);
	virtual int post_event(Event *);
	
	virtual void connect(void);
	virtual void disconnect(void);
	
    /* For testing purposes */
	virtual void _wait(void);
    protected:
	virtual void offer_change (
	    const CosNotification::EventTypeSeq & added,
	    const CosNotification::EventTypeSeq & removed);
	
	virtual void push_structured_event (
	    const CosNotification::StructuredEvent & notification);
	
	virtual void disconnect_structured_push_consumer(void);
	
	CORBA::ORB_var orb;
	ACE_Thread_Mutex	queue_mutex;
	ACE_Unbounded_Queue<Event *> events;
	ORB_Task * 		orb_task;
	alarm_callback * 	alarm_cb;
    };

    typedef ACE_Refcounted_Auto_Ptr<Consumer, ACE_Null_Mutex> Consumer_ptr;
}

#endif
