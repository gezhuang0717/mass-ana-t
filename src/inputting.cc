// generated 2006/8/5 14:49:49 EEST by tomero@marakatti.(none)
// using glademm V2.6.0
//
// newer (non customized) versions of this file go to inputting.cc_new

// This file is for your program, I won't touch it again!
//

#include "config.h"
#include "compat.h"

#include "fit_parameters.hh"
#include "inputting.hh"
#include "window1.hh"

#include <gtkmm/filechooserdialog.h>
#include <gtkmm/stock.h>
#include <gtkmm/dialog.h>
#include <gtkmm/main.h>
#include <gtkmm/checkmenuitem.h>
#include <gtkmm/radiomenuitem.h>

#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem/path.hpp>
#undef BOOST_NO_CXX11_SCOPED_ENUMS

#include "inputting_init.inc"

using namespace std;
using namespace boost::filesystem;

void
Inputting::create_widget(Inputting *& win,
			 const Glib::RefPtr<Gtk::Builder> & builder)
{
    builder->get_widget_derived("inputting", win);
}

Inputting::Inputting(GtkVBox * obj, const Glib::RefPtr<Gtk::Builder> & builder) : Gtk::VBox(obj)
{
    init_widgets(builder);
    init_signals();

    inputdir_dialog = new Gtk::FileChooserDialog("Please choose input directory", Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
    inputdir_dialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    inputdir_dialog->add_button("Select", Gtk::RESPONSE_OK);
    inputdir_dialog->set_default_response(Gtk::RESPONSE_OK);
    online_progressbar->set_pulse_step(0.2);
}

//------------------------------------------------------------------------------
void Inputting::on_open_inputdir_button_clicked()
{
	/* In case eva format is used, we'll popup a message that we will load
	 * and process all files in the chosen directory. Idea is to make then
	 * "symbolic" links to files according to element and charge state */

	if (mainwin->get_data_file_format() == DATAFORMAT_EVA)
	{
		Gtk::MessageDialog infoa("Note!",Gtk::MESSAGE_INFO);
		infoa.set_message("Please choose (in the forthcoming dialog) directory\ncontaining your eva (.dat) files. Then I'll process\nthem which might take a while.");
		infoa.run();
		infoa.hide();
	}

	inputdir_dialog->set_current_folder(inputdir_entry ->get_text() );
	int result = inputdir_dialog->run();
	inputdir_dialog->hide();

	/// In case action cancelled
	if (result != Gtk::RESPONSE_OK)
		return;

	inputdir_entry -> set_text(inputdir_dialog->get_filename() );

	if (mainwin->get_data_file_format() == DATAFORMAT_EVA)
	{
		mainwin->process_eva_file_directory();
	}
	else
	{
	  dissect_full_inputdir_path(); // Split "/foo/bar/cu63_2" -> "/foo/bar" "cu63_" "2" and put to relevant boxes

	}

}
//------------------------------------------------------------------------------
void Inputting::on_2d_use_button_toggled()
{
	if (twod_use_button->get_active() ) {
		twod_filename_entry->set_sensitive();
		twod_suffix_entry->set_sensitive();
	}
	else {
		twod_filename_entry->set_sensitive(false);
		twod_suffix_entry->set_sensitive(false);
	}
}
//------------------------------------------------------------------------------
void Inputting::on_filenumber_spinbutton_changed()
{
	//filenumber_spinbutton->set_sensitive(false);
	mainwin->getNumberOfRoundsInFile(true);
	update_gtk();
//	putIntegerToEntry(*rounds_show_entry,data->getNumberOfRoundsInFile(getInputDataDir(),"/bunches.dat"));

	if (mainwin->instant_load->get_active() )
	{
		mainwin->LoadDataFile();
		mainwin->make_class_division();
	}
	if (mainwin->fit_immediately->get_active() ) {

		mainwin->StartFitting();
		mainwin->linear_fit_button_clicked();
	}
	on_round_spinbutton_value_changed();
	//filenumber_spinbutton->set_sensitive(true);
}
//------------------------------------------------------------------------------
void Inputting::on_round_spinbutton_value_changed()
{
	double absolute_max = mainwin->getNumberOfRoundsInFile();
	mainwin->set_number_of_rounds_in_file(absolute_max);
//	absolute_max += 10;
	double max2 = endround_spinbutton->get_value();
	double min2 = startround_spinbutton->get_value();

	if (min2 > absolute_max)
		min2 = absolute_max;
	if (max2 < min2)
		max2 = min2;
	if (max2 > absolute_max)
		max2 = absolute_max;

	startround_spinbutton->set_value(min2);
	endround_spinbutton->set_value(max2);
	startround_spinbutton->set_range(0,absolute_max);
	endround_spinbutton->set_range(startround_spinbutton->get_value(),absolute_max);
}
//------------------------------------------------------------------------------
void Inputting::on_load_datafile_clicked()
{
	mainwin->LoadDataFile();
}
//------------------------------------------------------------------------------
void Inputting::on_append_datafile_clicked()
{
	mainwin->LoadDataFile(true);
}
//------------------------------------------------------------------------------
void Inputting::on_delete_data_clicked()
{
	mainwin->DeleteData();
}
//------------------------------------------------------------------------------
void Inputting::on_batch_spinbutton_value_changed()
{
	mainwin->validateSpinButtons();
}
//------------------------------------------------------------------------------
void Inputting::on_batch_process_button_clicked()
{
	mainwin->batchProcess();
}
//------------------------------------------------------------------------------
void Inputting::on_round_batch_button_clicked()
{
	mainwin->SplitAndFitFile();
}
//------------------------------------------------------------------------------
void Inputting::on_show_data_info_clicked()
{
	mainwin->showDataInfo();
}
//------------------------------------------------------------------------------
void Inputting::on_refmeas_radiobutton_toggled() {
	mainwin->set_active_file_colors();

	mainwin->update_sn_menues_in_tof_vs_class_window();
	mainwin->set_startfreq_menuitems_for_tof_vs_class_window();
	on_guess_mass_cb_toggled();
	on_guess_charge_cb_toggled();

	mainwin->update_cts_menues_in_cts_vs_tof_window();
	mainwin->update_events_menues_in_cts_vs_bunches_window();

	set_start_freq_to_box();
	if (mainwin->instant_load->get_active() ) {
		mainwin->LoadDataFile();
		if (mainwin->fit_immediately->get_active() ) {
			mainwin->StartFitting();
		}
		return;
	}
	mainwin->bin_or_cts_win_value_changed();

}
//------------------------------------------------------------------------------
void Inputting::set_start_freq_to_box() {
	if (meas_radiobutton->get_active() ) {
		if (meas_freq_start->get_value() > 0 ) {
			putDoubleToEntry(*mainwin->fit_parameters->f_entry,meas_freq_start->get_value());
		}
	}
	if (ref_radiobutton->get_active() ) {
		if (ref_freq_start->get_value() > 0 ) {
			putDoubleToEntry(*mainwin->fit_parameters->f_entry,ref_freq_start->get_value());
		}
	}
}
//------------------------------------------------------------------------------
void Inputting::on_first_bunch_button_toggled() {
}
//------------------------------------------------------------------------------
void Inputting::on_guess_mass_cb_toggled() {
	if (guess_mass_cb->get_active() ) {
		set_mass_from_file_name();
		mass_spinbutton->set_sensitive(0);
	}
	else
		mass_spinbutton->set_sensitive(1);
}
//------------------------------------------------------------------------------
void Inputting::on_guess_charge_cb_toggled() {
	if (guess_charge_cb->get_active() ) {
		set_charge_from_file_name();
		charge_spinbutton->set_sensitive(0);
	}
	else
		charge_spinbutton->set_sensitive(1);
}
//------------------------------------------------------------------------------
void Inputting::set_mass_from_file_name(void) {
	string getting_the_mass;
	if (ref_radiobutton->get_active()) {
		getting_the_mass = ref_element_entry->get_text();
	}
	else
		getting_the_mass = meas_element_entry->get_text();
	string original = getting_the_mass;

	//Check whether it is carbon cluster
	try {
		if (getting_the_mass.size() > 2) {
			if (getting_the_mass[0] == 'c' && getting_the_mass[1] == 'c') {
				size_t numbers_end_at = getting_the_mass.substr(2).find_first_not_of("0123456789");
				string nummero = getting_the_mass.substr(2).substr(0,numbers_end_at);
				//cout << "nummero " << nummero << endl;
				int mass_numberoo = as_int(nummero);
				mass_spinbutton->set_value(mass_numberoo*12);
				return;
			}
		}
	}
	catch (...) {

	}
	///And if it is something else..


	try {
		size_t startti = getting_the_mass.find_first_of("123456789");
		getting_the_mass = getting_the_mass.substr(startti);
		size_t enddi  = getting_the_mass.find_first_not_of("1234567890");
		getting_the_mass = getting_the_mass.substr(0,enddi);
	}
	catch (...) {
		//cout << "ERG!\n";
		mainwin->message_dialog.set_title("ERG!");
		mainwin->message_dialog.set_message("\n Could not extract\nthe mass number!\n   (Tried to get it from \""+original+"\")   \n");
		mainwin->message_dialog.run();
		mainwin->message_dialog.hide();
		getting_the_mass = "666";
	}
	//cout << "\"" << getting_the_mass << "\"" << "\n";
	int mass_numberoo = as_int(getting_the_mass);
	if (mass_numberoo < 5) {
		mainwin->message_dialog.set_title("Warning!");
		mainwin->message_dialog.set_message("\nLooks like your mass number is < 5.\nIs this intentional?\n");
		mainwin->message_dialog.run();
		mainwin->message_dialog.hide();
	}
	mass_spinbutton->set_value(mass_numberoo);

}
//------------------------------------------------------------------------------
void Inputting::set_charge_from_file_name(void) {
	string getting_the_charge;
	if (ref_radiobutton->get_active()) {
		getting_the_charge = ref_element_entry->get_text();
	}
	else
		getting_the_charge = meas_element_entry->get_text();
	string original = getting_the_charge;
	//Check if there is only 1 "_"; if YES, then its singly charged
	if (getting_the_charge.find_first_of("_") == getting_the_charge.find_last_of("_")) {
		charge_spinbutton->set_value(1);
		return;
	}
	//otherwise try to find the last ar75_2_
	if ( getting_the_charge.find_first_of("_") < getting_the_charge.find_last_of("_")) {
	 	int chargge = as_double(getting_the_charge.substr(getting_the_charge.find_first_of("_")+1));
		charge_spinbutton->set_value(chargge);
	}



}
//------------------------------------------------------------------------------
void Inputting::swap_ref_meas_radiobuttons() {
	if (ref_radiobutton->get_active() )
		meas_radiobutton->set_active();
	else
		ref_radiobutton->set_active();
}
//------------------------------------------------------------------------------
void Inputting::on_apply_round_skips_button_clicked()
{
	mainwin->practically_update_all();
}
//------------------------------------------------------------------------------
void Inputting::on_save_round_skips_to_file_button_clicked() {
	mainwin->save_rounds_to_skip();
}
//------------------------------------------------------------------------------
void Inputting::on_split_many_files_clicked() {
	mainwin->split_many_files();
}
//------------------------------------------------------------------------------
void Inputting::on_startingfreq_changed() {
	set_start_freq_to_box();
}
//------------------------------------------------------------------------------
#ifdef HAVE_ONLINE
std::string Inputting::get_desired_id_of_measurement() {
	if (meas_id_foo_rb->get_active()) {
		return "foo";
	}
	if (meas_id_bar_rb->get_active()) {
		return "bar";
	}
	if (meas_id_all_rb->get_active()) {
		return "EVERYTHING";
	}
	if (meas_id_other_rb->get_active()) {
		return meas_id_other_entry->get_text();
	}
	return "wtf?";
}
#endif
//------------------------------------------------------------------------------
void Inputting::on_meas_id_rb_toggled() {
#ifdef HAVE_ONLINE
	mainwin->set_window_labels_with_meas_id();
#endif
}
//------------------------------------------------------------------------------
void Inputting::dissect_full_inputdir_path()
{
    boost::filesystem::path full_path(inputdir_entry->get_text().c_str());
    std::cout << full_path.filename().string() << "\n";
    std::cout << full_path.parent_path().string() << "\n";

    // Split "cu63_3" to "cu63_" and "3"
    string::size_type pos = full_path.filename().string().find_last_of("_");

    // Proceed only if there was "_"
    if (pos != std::string::npos )
    {
      std::string elementy = full_path.filename().string().substr(0,pos+1);
      std::string number = full_path.filename().string().substr(pos+1);

      std::cout << "ele:" << elementy << "\n";
      std::cout << "num:" << number << "\n";

      if (ref_radiobutton->get_active())
	ref_element_entry->set_text(elementy);
      else
	meas_element_entry->set_text(elementy);

      // This needs to be before setting the number since filenumber_spinbutton change will
      // trigger data load & fit (if autoload and/or autofit is checked)
      inputdir_entry->set_text(full_path.parent_path().string());
      filenumber_spinbutton->set_value(as_int(number));

    }


    // First, replace all backslashes
}

//------------------------------------------------------------------------------
/*
Glib::ustring Inputting::get_full_inputdata_dir(void){
	std::string temp = inputdir_entry->get_text();
	std::string::size_type len = temp.length();
//	std::cout << len << "\n";
	if (len > 0) {
		if (  (temp[len-1] == '/') || (temp[len-1] == '\\') ) {
			temp[len-1] = '/';
		}
		else
			temp +='/';

	}
	inputdir_entry->set_text(temp);
	if (twod_use_button->get_active() ) {
		if (ref_radiobutton->get_active() )
			return (temp + ref_element_entry->get_text() );
		else
			return (temp + meas_element_entry->get_text() );
	}
	else
		if (ref_radiobutton->get_active() )
			return (temp + ref_element_entry->get_text() + filenumber_spinbutton->get_text());
		else
			return (temp + meas_element_entry->get_text() + filenumber_spinbutton->get_text());
}
*/
//------------------------------------------------------------------------------
boost::filesystem::path Inputting::get_full_inputdata_dir(void)
{
	boost::filesystem::path dirpath(inputdir_entry->get_text().c_str());
	
	Glib::ustring subpath;
	
	if (ref_radiobutton->get_active() )
		subpath += ref_element_entry->get_text();
	else
		subpath += meas_element_entry->get_text();
	
	subpath += filenumber_spinbutton->get_text();
	
	dirpath /= boost::filesystem::path(subpath.c_str());
	return dirpath;
}
//------------------------------------------------------------------------------
boost::filesystem::path Inputting::get_full_bunchfile_path()
{
    // First check if /aa/bb/bunches.dat exists
    boost::filesystem::path file = get_full_inputdata_dir() / boost::filesystem::path("bunches.dat");
    boost::system::error_code ec;

    if (exists(file,ec))
	    return file;
    
    // In case bunches.dat is not there then try bunches-xxx.dat
    file = get_full_inputdata_dir() / boost::filesystem::path("bunches-xxx.dat");
    if (exists(file,ec))
	    return file;
    

    return file.parent_path();

}
