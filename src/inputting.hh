// generated 2006/8/5 14:54:06 EEST by tomero@marakatti.(none)
// using glademm V2.6.0
//
// newer (non customized) versions of this file go to inputting.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _INPUTTING_HH
#define _INPUTTING_HH

#include "config.h"
#include "compat.h"

#include <gtkmm/filechooserdialog.h>
#include <gtkmm/stock.h>
#include <boost/filesystem/path.hpp>


class Window1;
#include "inputting_headers.inc"

class Inputting : public Gtk::VBox
{
#  include "inputting_members.inc"
public:
	~Inputting() {
		delete inputdir_dialog;
	}
	void set_parent(Window1 * window) {
		this->mainwin = window;
	}
	void on_meas_id_other_entry_changed(){on_meas_id_rb_toggled();}
	void on_meas_id_rb_toggled();
	void on_apply_round_skips_button_clicked();
	void on_round_spinbutton_value_changed();
	void on_guess_mass_cb_toggled();
	void on_guess_charge_cb_toggled();
	void on_refmeas_radiobutton_toggled();
	void set_mass_from_file_name();
	void set_charge_from_file_name();
	void swap_ref_meas_radiobuttons();
	void on_save_round_skips_to_file_button_clicked();
	void on_split_many_files_clicked();
	void set_start_freq_to_box();
	void on_startingfreq_changed();
	void on_filenumber_spinbutton_changed();
	//Glib::ustring get_full_inputdata_dir();
	boost::filesystem::path get_full_inputdata_dir();
	boost::filesystem::path get_full_bunchfile_path();

	#ifdef HAVE_ONLINE
	std::string get_desired_id_of_measurement();
	#endif
private:
	Gtk::FileChooserDialog *inputdir_dialog;

	void dissect_full_inputdir_path();
	void on_first_bunch_button_toggled();
	void on_open_inputdir_button_clicked();
	void on_2d_use_button_toggled();
	void on_load_datafile_clicked();
	void on_append_datafile_clicked();
	void on_delete_data_clicked();
	void on_batch_spinbutton_value_changed();
	void on_batch_process_button_clicked();
	void on_round_batch_button_clicked();
	void on_show_data_info_clicked();
	Window1 *mainwin;
};
#endif
