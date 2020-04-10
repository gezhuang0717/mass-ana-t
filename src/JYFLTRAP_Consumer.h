// -*- C++ -*-
/*
 * Copyright (c) 2007 Jani J. Hakala <jahakala@iki.fi>
 * All rights reserved.
 */
#ifndef JYFLTRAP_CONSUMER_H
#define JYFLTRAP_CONSUMER_H

#ifdef __G_WIN32_H__
#undef pipe
#endif

#include <orbsvcs/CosNotifyChannelAdminS.h>
#include <orbsvcs/CosNotifyCommC.h>
#include <orbsvcs/CosNamingC.h>

#include <string>

class JYFLTRAP_Consumer;
#include "JYFLTRAPC.h"

class JYFLTRAP_Consumer : public POA_CosNotifyComm::StructuredPushConsumer, 
			  public PortableServer::RefCountServantBase {
 public:
    JYFLTRAP_Consumer(
	const char * domain_name,
	const char * type_name);

    virtual ~JYFLTRAP_Consumer(void);
    
    virtual void connect(const CosNotifyChannelAdmin::EventChannel_var & ec);
    virtual void disconnect();

protected:
    JYFLTRAP_Consumer(void);
    JYFLTRAP_Consumer(JYFLTRAP_Consumer &);

    void init_supplier_stuff();

    CosNotifyChannelAdmin::EventChannel_var 	ec;
    CosNotifyChannelAdmin::ConsumerAdmin_var 	consumer_admin;
    CosNotifyChannelAdmin::InterFilterGroupOperator 	ifgop;

    CosNotifyChannelAdmin::ProxyID 			supplier_proxy_id;
    CosNotifyChannelAdmin::StructuredProxyPushSupplier_var supplier_proxy;

    CosNotifyChannelAdmin::AdminID 			admin_id;
    CosNotifyChannelAdmin::ChannelID			channel_id;

    std::string domain_name;
    std::string type_name;
};

#endif
