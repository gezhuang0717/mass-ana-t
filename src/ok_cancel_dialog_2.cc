// generated 2006/11/6 17:01:12 EET by tomero@phys103.phys.ad.jyu.fi.(none)
// using glademm V2.6.0
//
// newer (non customized) versions of this file go to ok_cancel_dialog_2.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "guiutil.hh"

#include "ok_cancel_dialog_2.hh"
#include "ok_cancel_dialog_2_init.inc"

#include <iostream>

void
ok_cancel_dialog_2::create_widget(ok_cancel_dialog_2 *& win,
				  const Glib::RefPtr<Gtk::Builder> & builder)
{
    builder->get_widget_derived("ok_cancel_dialog_2", win);
}

Glib::RefPtr<ok_cancel_dialog_2>
ok_cancel_dialog_2::create_widget(void)
{
    ok_cancel_dialog_2 * w = NULL;
    gtk_builder->get_widget_derived("ok_cancel_dialog_2", w);

    Glib::RefPtr<ok_cancel_dialog_2> ref(w);
    return ref;
}

ok_cancel_dialog_2::ok_cancel_dialog_2(GtkDialog * obj, const Glib::RefPtr<Gtk::Builder> & builder) : Gtk::Dialog(obj)
{
    init();
    init_widgets(builder);
    init_signals();
}

//--------------------------------------------------------------------------------------------------
void ok_cancel_dialog_2::set_output_dir(const Glib::ustring &outdir) {
	directory->set_text(outdir);
}
//--------------------------------------------------------------------------------------------------
void ok_cancel_dialog_2::set_prefix(const Glib::ustring &prefix) {
	ok_cancel_dialog_2::prefix->set_text(prefix);
}
//--------------------------------------------------------------------------------------------------
Glib::ustring ok_cancel_dialog_2::get_tof_data_filename() {
	return (getOutputDir() + prefix->get_text() + tof_data);
}
//--------------------------------------------------------------------------------------------------
Glib::ustring ok_cancel_dialog_2::get_tof_fit_filename() {
	return (getOutputDir() + prefix->get_text() + tof_fit);
}
//--------------------------------------------------------------------------------------------------
Glib::ustring ok_cancel_dialog_2::get_cts_data_filename() {
	return (getOutputDir() + prefix->get_text() + cts_data);
}
//--------------------------------------------------------------------------------------------------
Glib::ustring ok_cancel_dialog_2::get_cts_fit_filename() {
	return (getOutputDir() + prefix->get_text() + cts_fit);
}
//--------------------------------------------------------------------------------------------------
Glib::ustring ok_cancel_dialog_2::get_freq_as_function_of_number_of_ions_per_bunch_filename() {
	return (getOutputDir() + prefix->get_text() + z_class);
}
//--------------------------------------------------------------------------------------------------
Glib::ustring ok_cancel_dialog_2::get_counts_as_function_of_tof_filename() {
	return (getOutputDir() + prefix->get_text() + counts_vs_tof);
}
//--------------------------------------------------------------------------------------------------
Glib::ustring ok_cancel_dialog_2::get_bunches_as_function_of_ions_per_bunch_filename() {
	return (getOutputDir() + prefix->get_text() + bunches_vs_ions_per_bunch);
}
//--------------------------------------------------------------------------------------------------
Glib::ustring ok_cancel_dialog_2::getOutputDir(void) {
	std::string temp;
	temp = directory->get_text();
	std::string::size_type len;
	len = temp.length();
	if (len > 0) {
		if (  (temp[len-1] == '/') || (temp[len-1] == '\\') ) {
			temp[len-1] = '/';
		}
		else
			temp +='/';

	}
	return (Glib::ustring )temp;
}
//--------------------------------------------------------------------------------------------------
bool ok_cancel_dialog_2::on_directory_key_press_event(GdkEventKey *ev) {
	std::cout << "dir key press ev\n";
	return true;
}
//--------------------------------------------------------------------------------------------------
bool ok_cancel_dialog_2::on_prefix_key_press_event(GdkEventKey *ev) {
	std::cout << "prefix key press ev\n";
	return true;
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
