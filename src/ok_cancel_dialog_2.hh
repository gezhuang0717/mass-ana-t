// generated 2006/11/6 17:01:12 EET by tomero@phys103.phys.ad.jyu.fi.(none)
// using glademm V2.6.0
//
// newer (non customized) versions of this file go to ok_cancel_dialog_2.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _OK_CANCEL_DIALOG_2_HH
// #  include "ok_cancel_dialog_2_glade.hh"
#  define _OK_CANCEL_DIALOG_2_HH

#include "ok_cancel_dialog_2_headers.inc"

class ok_cancel_dialog_2 : public Gtk::Dialog
{
#include "ok_cancel_dialog_2_members.inc"
  private:
	Glib::ustring tof_data,tof_fit,cts_data,cts_fit,z_class,counts_vs_tof,bunches_vs_ions_per_bunch;
	Glib::ustring getOutputDir(void);
  public:
   	static Glib::RefPtr<ok_cancel_dialog_2> create_widget(void);

	void set_output_dir(const Glib::ustring &outdir);
	void set_prefix(const Glib::ustring &prefix);
	Glib::ustring get_tof_data_filename();
	Glib::ustring get_tof_fit_filename();
	Glib::ustring get_cts_data_filename();
	Glib::ustring get_cts_fit_filename();
	Glib::ustring get_freq_as_function_of_number_of_ions_per_bunch_filename();
	Glib::ustring get_counts_as_function_of_tof_filename();
	Glib::ustring get_bunches_as_function_of_ions_per_bunch_filename();

	//Events next:
	bool on_directory_key_press_event(GdkEventKey *ev);
	bool on_prefix_key_press_event(GdkEventKey *ev);

        void init(void) {
	    tof_data = "tof_dat.txt";
	    cts_data = "cts_dat.txt";
	    z_class = "z_class.txt";
	    tof_fit  = "tof_fit.txt";
	    cts_fit =  "cts_fit.txt";
	    counts_vs_tof = "tof_distribution.txt";
	    bunches_vs_ions_per_bunch = "bunch_size_distribution.txt";
	}

// static Glib::RefPtr<ok_cancel_dialog_2> create_widget(void);
};
#endif
