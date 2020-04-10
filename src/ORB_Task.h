// -*- C++ -*-
/*
 * Copyright (c) 2007 Jani J. Hakala <jahakala@iki.fi>
 * All rights reserved.
 */
#ifndef ORB_TASK_H
#define ORB_TASK_H

#ifdef __G_WIN32_H__
#undef pipe
#endif

#include <ace/ACE.h>
#include <ace/Task.h>

#include <tao/ORB.h>

class ORB_Task : public ACE_Task_Base {
public:
    ORB_Task(CORBA::ORB_var orb);
    virtual ~ORB_Task();

    virtual void shutdown(void);
    virtual int svc(void);
private:
    CORBA::ORB_var orb;
};

#endif
