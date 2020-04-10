// Stand-alone coma.

#include <config.h>
#include <glibmm/arrayhandle.h>
#include <glibmm/listhandle.h>
#include <glibmm/value.h>
#include <glibmm/optioncontext.h>
#include <gtkmm/main.h>
#include <iostream>

#include "util.hh"
#include "ramseyer.hh"
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

	ids.push_back(Glib::ustring("ramseyer"));
	for (int i = 53; i <= 69; i++) {
	    ids.push_back(Glib::ustring("adjustment" + asString(i)));
	}
	gtk_builder->add_from_string(lakritsi_xml_data, ids);

	Ramseyer *ramseyer;
	Ramseyer::create_widget(ramseyer, gtk_builder);
	update_gtk();

	m.run(*ramseyer);

	delete ramseyer;
  return 0;
}
