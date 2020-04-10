// -*- C++ -*-
/*
 * Copyright (c) 2007-2009 Jani J. Hakala <jahakala@iki.fi>
 * All rights reserved.
 */
#define ACE_NTRACE   1
#define ACE_NDEBUG   1
#define ACE_NLOGGING 1

#include "Event_Data.h"

Measurement_Info::
Measurement_Info(
    const JYFLTRAP::Measurement_Info & info) 
    : Event_Data(Event_Data::START_MSG, std::string(info.measurement_id))
{
    this->start_time = std::string(info.start_time);
    this->bins = info.bins;
    this->bin_width = info.bin_width;
    // this->settings = std::string(info.settings);
}

Measurement_Info::~Measurement_Info() 
{
}

Bin_Data::
Bin_Data(
    const JYFLTRAP::Bunch_Data_Item & item) 
{
    this->bin = item.bin;
    this->count = item.count;
}

Bunch_Data::
Bunch_Data(void) 
{
}

Bunch_Data::
Bunch_Data(
    const JYFLTRAP::Bunch_Data & data) 
{
    this->seqno = data.seqno;
    this->frequency = std::string(data.frequency);
    this->timestamp = std::string(data.timestamp);
    this->data_len = 0;

    uint32_t len = data.counts.length();
    for (uint32_t i = 0; i < len; i++) {
        this->data.push_back(Bin_Data(data.counts[i]));
        this->data_len++;
    }
}

Bunch::
Bunch(void) 
    : Event_Data(Event_Data::BUNCH) 
{
}

Bunch::
~Bunch() 
{
}

Bunch_Pack::
Bunch_Pack(
    const std::string & measurement_id, 
    const JYFLTRAP::Bunch_Pack & bpack)
    : Event_Data(Event_Data::BUNCH_PACK, measurement_id)
{
    uint32_t len = bpack.bunches.length();
    for (uint32_t i = 0; i < len; i++) {
        bunches.push_back(Bunch_Data(bpack.bunches[i]));
    }
}

Bunch_Pack::
~Bunch_Pack() 
{
}

Mass_Scanning::Event::
Event(
    Event_Data * event_data) 
{
    this->event_data = event_data;
}

Mass_Scanning::Event::
~Event() 
{
    delete event_data;
}

const Measurement_Info &
Mass_Scanning::Event::
get_measurement_info(void) const throw(invtype_error) 
{
    if (event_data->get_type() != Event_Data::START_MSG) {
        throw invtype_error("Expected START_MSG");
    }
    Measurement_Info *info =
        static_cast < Measurement_Info * >(event_data);
    return *info;
}

const Measurement_End &
Mass_Scanning::Event::
get_measurement_end(void) const throw(invtype_error) 
{
    if (event_data->get_type() != Event_Data::END_MSG) {
        throw invtype_error("Expected END_MSG");
    }
    Measurement_End *ret = static_cast < Measurement_End * >(event_data);
    return *ret;
}

const Measurement_Stop &
Mass_Scanning::Event::
get_measurement_stop(void) const throw(invtype_error) 
{
    if (event_data->get_type() != Event_Data::STOP_MSG) {
        throw invtype_error("Expected STOP_MSG");
    }
    Measurement_Stop *ret = static_cast < Measurement_Stop * >(event_data);
    return *ret;
}

const Bunch_Pack &
Mass_Scanning::Event::
get_bunch_pack(void) const throw(invtype_error) 
{
    if (event_data->get_type() != Event_Data::BUNCH_PACK) {
        throw invtype_error("Expected BUNCH_PACK");
    }
    Bunch_Pack *ret = static_cast < Bunch_Pack * >(event_data);
    return *ret;
}
