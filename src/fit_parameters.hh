// generated 2006/2/16 22:11:32 EET by jahakala@pingviini.(none)
// using glademm V2.6.0
//
// newer (non customized) versions of this file go to fit_parameters.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _FIT_PARAMETERS_HH
#define _FIT_PARAMETERS_HH

#include "config.h"

#include <ostream>

// #include "postProcessor.hh"
#include "util.hh"
#include "guiutil.hh"
#include "fit_parameters_headers.inc"

class Fit_Parameters: public Gtk::Table {
#include "fit_parameters_members.inc"
public:
// 	void get_values_for_post_process(post_process_stuffs & stuffs);
// 	void set_values_from_post_process(const post_process_stuffs & stuffs);

	void pretty_print(std::ostream & os, const char DL = '\t');
	void copy_results_to_initial_values();
	void on_auto_t_tot_button_toggled();
	void on_fit_or_not_changed();
	void set_crap_boxes_sensitivity(bool sensitivity);
  void on_auto_conv_button_toggled();
  void on_autofreq_button_toggled();
};
#endif
