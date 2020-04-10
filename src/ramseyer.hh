// generated 2007/2/11 17:43:27 EET by tomero@marakatti.(none)
// using glademm V2.6.0
//
// newer (non customized) versions of this file go to ramseyer.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _RAMSEYER_HH
#define _RAMSEYER_HH

#include "config.h"
#include "window2.hh"
#include "fitting1.hh"

#include <iostream>
#include <vector>

#include "ramseyer_headers.inc"

class Ramseyer : public Gtk::Window
{
#  include "ramseyer_members.inc"
private:
	Plottiarea ramsey_graph;
	Gtk::Label labeli;
//	YksiPlottiWindow * plotwindowi;
	//DATASET f1,f2,f3;
	FIT_FUNC *fit_func; //Perhaps easiest way of accessing ramsey-function...
	bool plotting_is_active;
	
	PlotDataContainer f1_plot_container, f2_plot_container, f3_plot_container;
	std::vector<Datapoint> f1_data, f2_data, f3_data;

public:
	~Ramseyer() {
		//delete ramseywindow;
		//delete plotwindowi;
		delete fit_func;
	}
	//Signals
	void on_ramsey_value_changed();
	void on_ramsey_make_range_clicked();
	void on_rams_freq_changed();
	void on_ramsey_plotrange_changed();
	void on_enabled_toggled();
	void on_debug_values_changed();
	int load_ini_file(const Glib::ustring &filename);
	bool save_ini_file(const Glib::ustring &filename);
	double get_transmission(double radius);
	void validate_spinbuttons();
	//void plot_stuff();
	void plot_stuff_v2();
	void set_button_sensitivity(bool activate);

};
#endif
