// generated 2005/9/27 11:12:51 EEST by tomero@phys103.phys.ad.jyu.fi.(none)
// using glademm V2.6.0
//
// newer (non customized) versions of this file go to lakritsi.cc_new

// This file is for your program, I won't touch it again!

#include <config.h>
#include <glibmm/arrayhandle.h>
#include <glibmm/listhandle.h>
#include <glibmm/value.h>
#include <glibmm/optioncontext.h>
#include <glibmm/exceptionhandler.h>
#include <gtkmm/main.h>
#include <iostream>

//#include <plotmm/plot.h>

#include "window1.hh"

#ifdef HAVE_ONLINE
//2007-04-24 CORBA stuff
#include <ace/ACE.h>
#include <ace/Synch.h>
#include <glibmm.h>
#include "Mass_Scanning_Consumer.h"
#include "alarm_cb.hh"
#endif

#include <boost/exception/all.hpp>

#include <gtkmm/builder.h>
Glib::RefPtr<Gtk::Builder> gtk_builder;

using namespace std;

void my_exception_handler()
{
    std::cerr << "Call to my_exception_handler" << std::endl;
    try {
	throw;
    } catch(std::exception &err) {
	std::cerr << "Exception catched. What: " << err.what() << std::endl;
    } catch(boost::exception &err) {
	std::cerr << "Exception catched. What: " << boost::diagnostic_information(err) << std::endl;
    } catch(Gtk::BuilderError &err) {
	std::cerr << "Exception catched. What: " << err.what() << std::endl;
    } catch(Glib::Exception &err) {
	std::cerr << "Exception catched. What: " << err.what() << std::endl;
    } catch(const std::string & err) {
	std::cerr << "Exception catched. What: " << err << std::endl;
    } catch(const char * err) {
	std::cerr << "Exception catched. What: " << err << std::endl;
    }
}


int main(int argc, char **argv)
{
	if (not init_rc_dir()) {
	    cout << "Failed to initialised User configuration dir" << endl;
	}
	#ifdef HAVE_ONLINE
	Glib::thread_init();
	#endif
	//cout << "argc " << argc << "\n";
	//cout << "argv[0] " << argv[0] << std::endl;
	Gtk::Main m(&argc, &argv);
	std::setlocale(LC_NUMERIC, "C");

	// Glib::add_exception_handler(sigc::ptr_fun(&my_exception_handler));

	gtk_builder = Gtk::Builder::create_from_string(
	    Glib::ustring(lakritsi_xml_data));
//	gtk_builder->add_from_string(
//	    Glib::ustring(coma_xml_data));
	Window1 *window1;
	Window1::create_widget(window1, gtk_builder);

	#ifdef HAVE_ONLINE
	if (argc < 3) {
		cout << "Try with options -ORBDefaultInitRef \'iiop://corba-naming:4001\'\n";
	}
	alarm_cb * cb = new alarm_cb(window1);
	Mass_Scanning::Consumer_ptr msc(new Mass_Scanning::Consumer(argc, argv, cb));
	cb->set_consumer(msc);
	msc->connect();
	#endif

	m.run(*window1);
	#ifdef HAVE_ONLINE
	delete cb;
	#endif
	delete window1;
  return 0;
}
