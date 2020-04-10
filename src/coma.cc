// Stand-alone coma.

#include <config.h>
#include <glibmm/arrayhandle.h>
#include <glibmm/listhandle.h>
#include <glibmm/value.h>
#include <glibmm/optioncontext.h>
#include <gtkmm/main.h>
#include <iostream>

#include "util.hh"
#include "postProcessor.hh"
#include "ame_stuff.hh"

#include <gtkmm/builder.h>
Glib::RefPtr<Gtk::Builder> gtk_builder;

using namespace std;

int main(int argc, char **argv)
{
	if (not init_rc_dir()) {
	    cout << "Failed to initialised User configuration dir" << endl;
	}
	//cout << "argc " << argc << "\n";
	//cout << "argv[0] " << argv[0] << std::endl;
	Gtk::Main m(&argc, &argv);
	setlocale(LC_NUMERIC, "C");

	gtk_builder = Gtk::Builder::create();
	std::vector<Glib::ustring> ids;

	ids.push_back(Glib::ustring("accelgroup1"));

	for (int i = 1; i <= 13; i++) {
	    ids.push_back(Glib::ustring("adjustment" + asString(i)));
	}
	ids.push_back(Glib::ustring("postProcessor"));
	gtk_builder->add_from_string(lakritsi_xml_data, ids);

	Post_Processor *coma;
	Post_Processor::create_widget(coma, gtk_builder);
	
	//post_process_stuffs coma_data;
	//coma->init(&coma_data);
	//coma->init();
	
/*	update_gtk();

	mass_table ame_just_for_coma;
	coma->init_mass_table(&ame_just_for_coma);


	post_process_stuffs default_post_process_stuff;
	default_post_process_stuff.element = "Zr97";
	default_post_process_stuff.time_of_scan = "2006-09-13 12:12:12";

	default_post_process_stuff.output_directory =  "X/home/tomero/data_out/2014_05_16_bfield/";
	default_post_process_stuff.post_process_filename = "post proc file";
	default_post_process_stuff.meas_out_file = "cu63.txt";
	default_post_process_stuff.ref_out_file = "cu63.txt";

	coma->init(&default_post_process_stuff);*/
	m.run(*coma);

	delete coma;
  return 0;
}
