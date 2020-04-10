// generated 2007/2/11 17:43:27 EET by tomero@marakatti.(none)
// using glademm V2.6.0
//

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _TICKERER_HH
#  include "tickerer_headers.inc"
#  define _TICKERER_HH

#include "config.h"
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>
#include <vector>

//class window1;

class Calibration_tree_view : public Gtk::TreeView
{
  private:
	Glib::RefPtr<Gtk::ListStore> calib_tree;

	Gtk::TreeModel::ColumnRecord columnss;

	Gtk::TreeModelColumn<Glib::ustring> col_element;
	Gtk::TreeModelColumn<unsigned int> col_A;
	Gtk::TreeModelColumn<unsigned int> col_q;
	Gtk::TreeModelColumn<double> col_mass_excess;
	Gtk::TreeModelColumn<std::string> col_calib_type; //mass from AME/ref_masses/user_masses or custom..
	Gtk::TreeModelColumn<double> col_frequency;
	Gtk::TreeModelColumn<Glib::ustring> col_start_datum;
	Gtk::TreeModelColumn<Glib::ustring> col_end_datum;

  public:
	Calibration_tree_view();
	~Calibration_tree_view(){}
	bool add_point_from_element(const Glib::ustring &element, double frequency, const Glib::ustring &start_datum, const Glib::ustring &end_datum);
	bool load_calibration_file(const Glib::ustring &filename);
	bool save_calibration_file(const Glib::ustring &filename);
	double get_frequency(const Glib::ustring &element, unsigned long scan_time);

};


class Tickerer : public Gtk::VBox
{
#  include "tickerer_members.inc"
private:
  Calibration_tree_view trap1_calibration;
	//bool plotting_is_active;

/*	Gtk::TreeView calib_tree_view;
	Glib::RefPtr<Gtk::ListStore> calib_tree;

	Gtk::TreeModel::ColumnRecord columnss;

	Gtk::TreeModelColumn<Glib::ustring> col_element;
	Gtk::TreeModelColumn<unsigned int> col_A;
	Gtk::TreeModelColumn<unsigned int> col_q;
	Gtk::TreeModelColumn<double> col_mass_excess;
	Gtk::TreeModelColumn<string> col_calib_type; //mass from AME/ref_masses/user_masses or custom..
	Gtk::TreeModelColumn<double> col_trap1_frequency;
	Gtk::TreeModelColumn<Glib::ustring> col_start_datum;
	Gtk::TreeModelColumn<Glib::ustring> col_end_datum;
*/

public:
	~Tickerer() {
	}
	//Signals

};

class ticks_tree_view : public Gtk::TreeView
{

};

#endif
