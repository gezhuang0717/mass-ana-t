// -*- C++ -*-
/*
 * Copyright (c) 2007 Jani J. Hakala <jahakala@iki.fi>
 * All rights reserved.
 */
#define ACE_NTRACE   1
#define ACE_NDEBUG   1
#define ACE_NLOGGING 1

#include <ace/ACE.h>
#include <ace/Log_Msg.h>
#include <iostream>

#include "JYFLTRAP_Consumer.h"

JYFLTRAP_Consumer::
JYFLTRAP_Consumer(
    const char * domain_name,
    const char * type_name)
    : domain_name(domain_name), type_name(type_name)
{
    ACE_TRACE(ACE_TEXT("JYFLTRAP_Consumer"));
}

JYFLTRAP_Consumer::
~JYFLTRAP_Consumer(void)
{
}

void
JYFLTRAP_Consumer::
disconnect(void)
{
    this->supplier_proxy->disconnect_structured_push_supplier();    
}

void
JYFLTRAP_Consumer::
connect(
    const CosNotifyChannelAdmin::EventChannel_var & ec)
{
    this->ec = ec;

    this->ifgop = CosNotifyChannelAdmin::OR_OP;
    consumer_admin = ec->new_for_consumers(this->ifgop, admin_id);
    ACE_ASSERT (!CORBA::is_nil(consumer_admin.in()));
    
    CosNotification::QoSProperties qos_props(1);
    qos_props[0].name  = CosNotification::OrderPolicy;
    qos_props[0].value <<= CosNotification::FifoOrder;
    consumer_admin->set_qos(qos_props);

    CosNotification::EventTypeSeq added(1);
    CosNotification::EventTypeSeq removed(1);
    added.length(1);
    removed.length(1);

    added[0].domain_name = CORBA::string_dup(domain_name.c_str());
    added[0].type_name = CORBA::string_dup(type_name.c_str());
    removed[0].domain_name = CORBA::string_dup("*");
    removed[0].type_name   = CORBA::string_dup("*");

    this->consumer_admin->subscription_change(added, removed);
    init_supplier_stuff();
}

void
JYFLTRAP_Consumer::
init_supplier_stuff()
{
    ACE_TRACE(ACE_TEXT("init_supplier_stuff"));

    CosNotifyComm::StructuredPushConsumer_var objref = this->_this();
    
    CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
	consumer_admin->obtain_notification_push_supplier(
	    CosNotifyChannelAdmin::STRUCTURED_EVENT, supplier_proxy_id);

    ACE_ASSERT(!CORBA::is_nil(proxysupplier.in()));
    
    this->supplier_proxy =
	CosNotifyChannelAdmin::StructuredProxyPushSupplier::
	_narrow(proxysupplier);
    ACE_ASSERT(!CORBA::is_nil(supplier_proxy.in()));
    
    this->supplier_proxy->connect_structured_push_consumer(objref);
}
