// -*- C++ -*-
/*
 * Copyright (c) 2007-2009 Jani J. Hakala <jahakala@iki.fi>
 * All rights reserved.
 */
#define ACE_NTRACE   1
#define ACE_NDEBUG   1
#define ACE_NLOGGING 1

#include <ace/Synch.h>

#include "ORB_Task.h"

#include "Mass_Scanning_Consumer.h"

Mass_Scanning::Consumer::
Consumer(
    int & argc, char *argv[], 
    alarm_callback * cb) : JYFLTRAP_Consumer("JYFLTRAP", "Mass Scanning")
{
    this->orb = CORBA::ORB_init(argc, argv, "");

    this->orb_task = new ORB_Task(orb);
    this->alarm_cb = cb;
}

Mass_Scanning::Consumer::
~Consumer()
{
    this->disconnect();
    
    orb_task->shutdown();
    orb_task->wait();
    delete orb_task;
}

void
Mass_Scanning::Consumer::
disconnect()
{
    ACE_TRACE(ACE_TEXT("disconnect"));
}

void
Mass_Scanning::Consumer::
connect()
{
    ACE_TRACE(ACE_TEXT("connect"));

    CORBA::Object_ptr poa  = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var root_poa = PortableServer::POA::_narrow(poa);
    PortableServer::POAManager_var poa_manager = root_poa->the_POAManager();
    poa_manager->activate();
    
    CORBA::Object_var naming_context_object =
	orb->resolve_initial_references("NameService");
    
    CosNaming::NamingContext_var naming_context =
	CosNaming::NamingContext::_narrow(naming_context_object);
    
    CosNotifyChannelAdmin::EventChannel_var        event_channel;

    CosNaming::Name name(1);
    name.length(1);
    name[0].id   = CORBA::string_dup("jyfltrap_event_channel"); 
    name[0].kind = CORBA::string_dup("object"); 
    
    CORBA::Object_var obj = naming_context->resolve(name);
    event_channel = CosNotifyChannelAdmin::EventChannel::_narrow(obj);
  
    JYFLTRAP_Consumer::connect(event_channel);

    this->orb_task->activate();
}

void
Mass_Scanning::Consumer::
offer_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed)
{
    ACE_TRACE(ACE_TEXT("offer_change"));

    CORBA::ULong len, i;

    len = added.length();
    for (i = 0; i != len; ++i) {
	const char * dname = added[i].domain_name;
	const char * tname = added[i].type_name;

	ACE_DEBUG ((LM_DEBUG, 
		    "New EventType domain = %s, type =  %s\n", 
		    dname, tname));
    }
    
    len = removed.length();
    for (i = 0; i != len; ++i) {
	const char * dname = removed[i].domain_name;
	const char * tname = removed[i].type_name;
	ACE_DEBUG ((LM_DEBUG, 
		    "Removed EventType domain = %s, type =  %s\n", 
		    dname, tname));
    }
}

void
Mass_Scanning::Consumer::
push_structured_event (
    const CosNotification::StructuredEvent & notification)
{
    ACE_TRACE(ACE_TEXT("push_structured_event"));

    /*
     * Small optimisation, "Bunch Pack" first in arrays.
     * "Bunch" is never sent 
     */
    static char * event_names[] = 
	{ "Bunch Pack", "Start Measurement", "End Measurement",  
	  "Stop Measurement" };
    static Event_Data::event_type_t event_types[] = 
	{ Event_Data::BUNCH_PACK, Event_Data::START_MSG, Event_Data::END_MSG, 
	  Event_Data::STOP_MSG };
    
    const char * domain_name =
	notification.header.fixed_header.event_type.domain_name;
    const char * type_name =
	notification.header.fixed_header.event_type.type_name;

    if ( !(strcmp(domain_name, "JYFLTRAP") == 0 
	   && strcmp(type_name, "Mass Scanning") == 0) ) {
	ACE_TRACE(ACE_TEXT("ook"));
	return;
    }

    const char * event_name =
	notification.header.fixed_header.event_name;

    Event_Data::event_type_t event_type = Event_Data::UNKNOWN_EVENT;
    unsigned int i = 0;
    while ( i < 4 ) {
	if ( strcmp(event_names[i], event_name) == 0 ) {
	    event_type = event_types[i];
	    break;
	}
	i++;
    }
    if ( i == 4 ) {
	ACE_TRACE(ACE_TEXT("eek"));
	return;
    }

    Event_Data * event_data = NULL;
    switch(event_type) {
    case Event_Data::START_MSG: {
	JYFLTRAP::Measurement_Info * info;
	if (!(notification.remainder_of_body >>= info)) {
	    return; 
	}
	event_data = new Measurement_Info(*info);
    }
	break;
    case Event_Data::END_MSG: {
	const char * m_id = 0;
	if (!(notification.remainder_of_body >>= m_id)) {
	    return; 
	}
	event_data = new Measurement_End(m_id);
    }
	break;
    case Event_Data::STOP_MSG: {
	const char * m_id = 0;
	if (!(notification.remainder_of_body >>= m_id)) {
	    return; 
	}
	event_data = new Measurement_Stop(m_id);
    }
	break;
    case Event_Data::BUNCH_PACK: {
	JYFLTRAP::Bunch_Pack * bpack;
	if (!(notification.remainder_of_body >>= bpack)) { 
	    ACE_TRACE(ACE_TEXT("1"));
	    return; 
	}
	event_data = new Bunch_Pack(std::string(bpack->measurement_id), *bpack);
    }
	break;
    default:
	return;
    };
    this->post_event(new Mass_Scanning::Event(event_data));

    ACE_DEBUG ((LM_DEBUG, 
		"Consumer received event, "
		"domain = %s, "
		"type =  %s, "
		"name = %s\n", domain_name, type_name, event_name));
}

void
Mass_Scanning::Consumer::
disconnect_structured_push_consumer(void)
{	
    ACE_TRACE(ACE_TEXT("disconnect_structured_push_consumer"));
}

Mass_Scanning::Event_ptr 
Mass_Scanning::Consumer::
get_event() throw(noevents_error)
{
    ACE_Guard<ACE_Thread_Mutex> guard(queue_mutex);
    Mass_Scanning::Event * item;
    if ( events.dequeue_head(item) == -1 ) {
	throw noevents_error("The event queue is empty");
    }
    return Mass_Scanning::Event_ptr(item);
}

int
Mass_Scanning::Consumer::
post_event(Mass_Scanning::Event * event)
{
    ACE_TRACE(ACE_TEXT("post_event"));

    ACE_Guard<ACE_Thread_Mutex> guard(queue_mutex);
    int res = events.enqueue_tail(event);
    if ( res == 0 ) {
	this->alarm_cb->alarm();
    }
    return res;
}

void 
Mass_Scanning::Consumer::
_wait(void)
{
    this->orb_task->thr_mgr()->wait();
}
