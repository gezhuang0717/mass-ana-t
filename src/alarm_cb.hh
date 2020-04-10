/***************************************************************************
 *            alarm_cb.hh
 *
 *  Wed May  2 10:53:35 2007
 *  Copyright  2007  User
 *  Email
 ****************************************************************************/

#ifndef _ALARM_CB_HH
#define _ALARM_CB_HH

#include "config.h"

#ifdef HAVE_ONLINE

#include <glibmm/dispatcher.h>

#include "Mass_Scanning_Consumer.h"

class Online_Listener {
public:
   virtual std::string get_desired_online_id() = 0;
    virtual void set_online_measurement_id(const std::string & id) = 0;

    virtual void online_stopped() = 0;
    virtual void online_ended() = 0;
    virtual bool online_info(const Measurement_Info &info) = 0;
    virtual bool online_bunchpack(const std::list<Bunch_Data> &data) = 0;
};

class alarm_cb : public alarm_callback {
public:
    alarm_cb(Online_Listener * ol) : ol(ol) {
      signal_thingee.connect(sigc::mem_fun(*this, &alarm_cb::event));
    }
    void set_consumer(Mass_Scanning::Consumer_ptr consumeri);
    void event(void);
    virtual void alarm(void);

    virtual ~alarm_cb();
private:
    Glib::Dispatcher    signal_thingee;
    Online_Listener * ol;
    Mass_Scanning::Consumer_ptr consumer;
};
#endif

#endif
