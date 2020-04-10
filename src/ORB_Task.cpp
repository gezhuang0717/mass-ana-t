// -*- C++ -*- 
/*
 * Copyright (c) 2007 Jani J. Hakala <jahakala@iki.fi>
 * All rights reserved.
 */
#define ACE_NTRACE   1
#define ACE_NDEBUG   1
#define ACE_NLOGGING 1

#include "ORB_Task.h"

ORB_Task::
ORB_Task(CORBA::ORB_var orb) : orb(orb) 
{ 
    ACE_TRACE(ACE_TEXT("ORB_Task"));
}

ORB_Task::
~ORB_Task() 
{
    ACE_TRACE(ACE_TEXT("~ORB_Task"));
};

void 
ORB_Task::
shutdown(void) 
{
    ACE_TRACE(ACE_TEXT("shutdown"));
    orb->shutdown(true);
    orb->destroy();
}

int
ORB_Task::
svc(void) 
{
    ACE_TRACE(ACE_TEXT("svc"));
    try {
	orb->run();
    } catch (const CORBA::Exception& ex) {
	ACE_DEBUG ((LM_DEBUG, "Exception: %s\n", ex._info().c_str()));
    }
    return 0;
}
