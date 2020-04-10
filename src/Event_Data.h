/* -*- C++ -*- */
/*
 * Copyright (c) 2007-2009 Jani J. Hakala <jahakala@iki.fi>
 * All rights reserved.
 */
#ifndef EVENT_DATA_H
#define EVENT_DATA_H

#include <list>
#include <memory>
#include <stdexcept>
#include <string>

#include "JYFLTRAP_Consumer.h"

class invtype_error : public std::runtime_error {
public:
    invtype_error(const std::string & info) : std::runtime_error(info) { }
};

class Event_Data {
public:
    typedef enum { BUNCH, BUNCH_PACK, START_MSG, END_MSG, STOP_MSG,
		   UNKNOWN_EVENT } event_type_t;

    Event_Data(event_type_t t, const std::string & measurement_id = "") 
	: type_(t), measurement_id_(measurement_id) { }
    virtual ~Event_Data() { }

    event_type_t get_type(void) const { return this->type_; } ;
    const std::string & get_measurement_id(void) const {
	return measurement_id_;
    }
private:
    event_type_t type_;
    std::string measurement_id_;
};

namespace Mass_Scanning {
    typedef std::list<std::string> Setting_Item_List;
    typedef std::list<Setting_Item_List> Setting_List;
}

class Measurement_Info : public Event_Data {
public:
    Measurement_Info(const JYFLTRAP::Measurement_Info & info);
    virtual ~Measurement_Info();

    std::string measurement_id;
    std::string start_time;

    uint16_t bin_width;
    uint16_t bins;
    Mass_Scanning::Setting_List settings;
private:
    Measurement_Info();
};

class Measurement_End : public Event_Data {
public:
    Measurement_End(const std::string & measurement_id) 
	: Event_Data(Event_Data::END_MSG, measurement_id) { }
    virtual ~Measurement_End(void) { }
};

class Measurement_Stop : public Event_Data {
public:
    Measurement_Stop(const std::string & measurement_id) 
	: Event_Data(Event_Data::STOP_MSG, measurement_id) { }
    virtual ~Measurement_Stop(void) { }
};

class Bin_Data {
public:
    Bin_Data(const JYFLTRAP::Bunch_Data_Item & data_item);

    uint16_t bin;
    uint16_t count;
};

class Bunch_Data {
public:
    Bunch_Data(const JYFLTRAP::Bunch_Data & data);
    
    uint32_t seqno;
    std::string frequency;
    std::string timestamp;

    std::list<Bin_Data> data;
    uint32_t data_len;
protected:
    Bunch_Data();
};

class Bunch : public Event_Data, public Bunch_Data {
public:
    Bunch(void);
    virtual ~Bunch();
};

class Bunch_Pack : public Event_Data {
public:
    Bunch_Pack(const std::string & measurement_id, 
	       const JYFLTRAP::Bunch_Pack & bpack);
    virtual ~Bunch_Pack();

    virtual const std::list<Bunch_Data> & get_data(void) const {
	return bunches;
    }

    std::string 	  measurement_id;
    std::list<Bunch_Data> bunches;

private:
    Bunch_Pack();
};

typedef std::auto_ptr<Measurement_Info> Measurement_Info_ptr;
typedef std::auto_ptr<Bunch_Pack> Bunch_Pack_ptr;

namespace Mass_Scanning {
    class Event {
    public:
	Event(Event_Data * event_data);
	virtual ~Event();
	
	Event_Data::event_type_t get_type(void) const {
	    return this->event_data->get_type();
	}
	
	const std::string & get_measurement_id(void) const {
	    return this->event_data->get_measurement_id();
	}
	
	const Measurement_Info & get_measurement_info(void) const 
	    throw(invtype_error);
	const Measurement_End & get_measurement_end(void) const 
	    throw(invtype_error);
	const Measurement_Stop & get_measurement_stop(void) const 
	    throw(invtype_error);
	const Bunch_Pack & get_bunch_pack(void) const 
	    throw(invtype_error);
    private:
	Event_Data *  event_data;
    };

    typedef std::auto_ptr<Event> Event_ptr;
}


#endif
