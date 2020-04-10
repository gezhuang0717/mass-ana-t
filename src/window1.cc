// generated 2005/9/13 18:07:12 EEST by tomero@a62-248-137-142.elisa-laajakaista.fi.(none)
// using glademm V2.0.0
//
//

// This needs to be first, otherwise windows build may break.
#include "curl_stuff.hh"

#include "window1.hh"
#include "fit_parameters.hh"
#include "inputting.hh"

#define LOCK 	bool locked_here;	if (locked)	locked_here = false;	else { locked_here = true; locked = true;}
#define RELEASE redraw_windows_if_necessary(locked_here);

#include "window1_init.inc"

using namespace std;

void
Window1::create_widget(Window1 *& win,
		       const Glib::RefPtr<Gtk::Builder> & builder)
{
    builder->get_widget_derived("window1", win);
}

//CONSTRUCTOR
Window1::Window1(GtkWindow * obj, const Glib::RefPtr<Gtk::Builder> & builder) :
		Gtk::Window(obj),
//		ame_mass_table(local_or_rc_file("nubase.mas12")),
		message_dialog("Info",false,Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK),
		ok_apply_cancel_dialog("ok-apply-cancel-dialog",false,
							   Gtk::MESSAGE_QUESTION,Gtk::BUTTONS_NONE)
{
	zclass_plot_container.set_data(&zclass_plot_data);
	zclass_plot_container.graph_type = YERR;
	
	zclass_extrapolation_plot_container.set_data(&zclass_extrapolation_plot_data);
	zclass_extrapolation_plot_container.graph_type = YERR;
	zclass_extrapolation_plot_container.color = Gdk::Color("blue");
	

//	coma = NULL;
	histogram_compression_factor = 5;

	init_widgets(builder);
	init_signals();
	initialize_child_windows();
	fitted_class_results->add_data(&zclass_plot_container);
	fitted_class_results->add_data(&zclass_extrapolation_plot_container);


	very_first_loading = true;
	ok_apply_cancel_dialog.add_button("OK",Gtk::RESPONSE_OK);
	ok_apply_cancel_dialog.add_button("Apply",Gtk::RESPONSE_APPLY);
	ok_apply_cancel_dialog.add_button("cancel",Gtk::RESPONSE_CANCEL);

	is_in_constructor = true;
	locked=false;
		//iconify();
	initialize_fit_function_combobox();
	suspend_online_updating = false;
	background_color_for_windows = Gdk::Color("White");
	notebookN->prev_page();
	notebookN->set_show_tabs(0);
	current_version= "2012-09-12" ; revision = 299         ; //KEEEEP THE SPACINGS!!!!!!!!!!!!!!!
	std::string build_string = std::string(GIT_VERSION) + ", compiled " + std::string(COMPILE_DATE);
	set_title("Lakritsi " + build_string);
	#ifdef HAVE_ONLINE
	background_color_for_windows = Gdk::Color("Orange");
	// window_title_without_meas_id = ("ONLINE Lakritsi "+ current_version + " (Build #" + asString(revision) +")");
	set_title("ONLINE Lakritsi " + build_string );
	#endif
	inputting->set_parent(this);

	set_colors(); 	background_alpha = 0.15;
	bin_interval_in_2d_contour = 20;

	set_icon( Gdk::Pixbuf::create_from_inline(8640,lakritsi_inline,true)); //_from_file("lakritsi.png");
	data = new cFrequencies(20,&message_dialog);
	data->setLoadProgressBar(inputting->load1_progressbar);
	data->setFitProgressBar(fit_progressbar);

	old_data_file_format_in_use=false;
	MS_windows=false;
	individual_statistics->set_active(false);
	sum_statistics->set_active(false); sum_statistics_2->set_active(false);


	datafmt_med->set_active(0);		datafmt_new->set_active(0); 		datafmt_old->set_active(0);


	primary_quantity = Q_VALUE; q_value1->set_active();
	show_datastuff_cb->set_active(0);
	show_fitting_things_cb->set_active(0);

	loadIniFile();

	ShowDebuggingStuff(false);
	show();
	validateSpinButtons();

	fit_parameters->on_auto_t_tot_button_toggled();
  fit_parameters->on_auto_conv_button_toggled();

	inputting->on_refmeas_radiobutton_toggled();
	stop_now = false;
	initialize_dialogs();

	if (check_for_new_version_at_startup->get_active() ) {
		Gtk::MessageDialog check_version_dialog("Checking if new version is available.. (from research.jyu.fi)");
		check_version_dialog.show();
		check_new_version();
		check_version_dialog.hide();
	}
	else {
		cout << "Did not check for new version. (checking of it is disabled)\n";
	}
	on_show_partly_toggled();
	move(0,20);
	testia();
	ok_apply_cancel_dialog.hide();
	#ifdef HAVE_ONLINE
	inputting->round_labeli->set_use_markup(true);
	inputting->loading_options_notebook->hide();
	inputting->filename_box->hide();
	inputting->input_directory_box->hide();
	inputting->twod_use_button->hide();
	instant_load->set_active(false); instant_load->set_sensitive(0);
	data->set_online_progressbar(inputting->online_progressbar);

	online_histogrammer.initialize_ions_per_x_bunches(&online_bunches, ions_per_x_bunches_window);
// 	class_histogrammer.initialize_bunches_per_class(&online_bunches, bunches_per_class_window, &class_borders);
	set_window_labels_with_meas_id();
	#endif
	#ifndef HAVE_ONLINE
	inputting->online_rounds_box->hide();
	inputting->online_status_box->hide();
	inputting->online_progressbar->hide();
	#endif
	resize(1,1);
	doing_fitting = false;
	set_background_colors();


	this->signal_window_state_event().connect(sigc::mem_fun(*this, &Window1::on_window1_window_state_event), false);
	present();
	move_windows_to_initial_positions();
	is_in_constructor = false;

	// process_eva_file_directory();

	set_number_of_rounds_in_file(getNumberOfRoundsInFile(true) );

	//ame_mass_table_p = & ame_mass_table;
	fit_function_combo_box_p = & fit_function_combo_box;

	cts_vs_class->set_ymin_with_force(0);

	if (instant_load->get_active() ) {
		LoadDataFile();
		if (fit_immediately->get_active() ) {
			StartFitting();
		}
		return;
	}
	

#ifdef HAVE_ONLINE
  ref_radiobutton->set_active();
  ref_radiobutton->set_sensitive(false);
  meas_radiobutton->set_sensitive(false);
  
  input_directory_box->hide();
  hbox33->hide();
#endif
}


Window1::~Window1()
{
  delete data;
  delete five_plotareas_window;
  delete yksi_plotarea_window;

  delete outputdir_dialog;

#ifdef HAVE_ONLINE
// 	delete toinen_yksi_plotarea_window;
#endif
#ifndef HAVE_ONLINE
#endif
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool Window1::on_window1_focus_in_event(GdkEventFocus* event){
	if (!is_in_constructor) {

	}
	//present();
	return false; //Jos lahettaa true, ei tule kursoria widgetteihin!
}
//------------------------------------------------------------------------------
bool Window1::on_window1_window_state_event(GdkEventWindowState* event) {
	//cout << "GdkEventWindowChange.\n";

	return false;
}
//------------------------------------------------------------------------------
void Window1::on_show_settings_button_clicked() {

}
//------------------------------------------------------------------------------
void Window1::freq_window_changed() {
	LOCK
/*	data->set_all_bunches_active();

	//Applying frequency window
	int freqs_outside =	data->set_frequency_range(get_freq_min(),	get_freq_max() );
	//Applying round filter
	data->set_bunches_that_will_be_skipped(get_rounds_to_skip());
	char out_str[256];
	char all_str[256];
    sprintf(out_str,"%d",freqs_outside);
	sprintf(all_str,"%d",data->getFrequencyNumber());
    string message = out_str; message.append("/");
	message.append(all_str); message.append(" out");
	freqs_out_label->set_text(message); */
	data->setAllDataNotFitted();
	tof_vs_class_need_update = true;
	cts_vs_class_need_update = true;
	data_histogram_need_update = true;
	fitted_class_results_need_update = true;
	event_histogram_need_update = true;
	#ifndef HAVE_ONLINE
	offline_histogram_need_update = true;
	#endif
	inputting->on_round_spinbutton_value_changed();
	bin_or_cts_win_value_changed();
	#ifndef HAVE_ONLINE
	show_offline_histogram();
	#endif
	make_class_division();

	ShowDataHistogram();
	validateWindowingSpinbuttons();
	validateCountFitRanges();
	fitted_class_results->nullify();
	RELEASE
}
//------------------------------------------------------------------------------
void Window1::make_event_histogram_bars(void) {
	LOCK
	event_histogram_need_update=true;
	for (int i=1;i<50;i++) {
		event_histogram->xbars.delete_bar(asString(i));
	}
	//Draw upper ions/bunch boundaries of classes
	vector<int> boundaries = data->getClassUpperBoundary();
	if (!boundaries.empty() ) {
		BAR valibaari;
		valibaari.color = Gdk::Color("#cc3399"); valibaari.width=1;
		int j(1);
		for (vector<int>::iterator i=boundaries.begin(); i != boundaries.end();++i) {
			valibaari.id = asString(j);
			valibaari.value = *i +0.5;
			j++;
			event_histogram->xbars.add_bar(valibaari);
		}
	}
	BAR toAdd;
	toAdd.width = 2;
	toAdd.id="events_left";
	toAdd.color = Gdk::Color("Purple");
	event_histogram->xbars.delete_bar("events_left");
	event_histogram->xbars.delete_bar("events_right");
	toAdd.value = get_cts_min()-0.5;
	event_histogram->xbars.add_bar(toAdd);
	toAdd.value = get_cts_max()+0.5;
	toAdd.id="events_right";
	event_histogram->xbars.add_bar(toAdd);
	event_histogram->show_contents();
	event_histogram->show_xbars(true);
	RELEASE
}
//------------------------------------------------------------------------------
void Window1::on_auto_divide_to_classes_button_clicked(void) {
	make_class_division(true);
	redraw_all_small_windows();
}
//------------------------------------------------------------------------------
void Window1::on_fit_or_not_changed() {
	fit_parameters->on_fit_or_not_changed();
}
//------------------------------------------------------------------------------
bool Window1::make_class_division(bool force_automatic){
	LOCK
	if (force_automatic){
		autoDivideToClasses();
		get_class_borders_from_data();
	}
	else {
		if (class_borders_fixed_button->get_active() ) {
			data->divideManuallyToClasses(class_borders);
		}
		else {
			autoDivideToClasses();
			get_class_borders_from_data();
		}
	}
	class_division_result_entry->set_text( data->getClassDivResult() );
	set_was_class_div_ok_text();
	validateSpinButtons();
	fitted_class_results->nullify();
	showFittedClassResults();
	extrapolate_result_entry->set_text("");
	extrapolate_result_err_entry->set_text("");
	data->setAllDataNotFitted();
	showFitResults();
	make_event_histogram_bars();
	show_single_class_histogram();
	RELEASE
	return true;
}
//------------------------------------------------------------------------------
void Window1::get_class_borders_from_data() {
	class_borders = data->get_class_borders_from_data();
}
//------------------------------------------------------------------------------
void Window1::set_countrate_effect_not_fitted() {
	LOCK
	slope_entry->set_text("--");
	dslope_entry->set_text("--");
	dconst_entry->set_text("--");
	const_entry->set_text("--");
	extrapolate_result_entry->set_text("X");
	extrapolate_result_err_entry->set_text("X");
	fitted_class_results->clear_data2();
	fitted_class_results_need_update = true;
	RELEASE
}
//------------------------------------------------------------------------------
void Window1::copy_results_to_initial_values(void) {
    if (this->is_class_fitted()) {
	fit_parameters->copy_results_to_initial_values();
	putDoubleToEntry(*f2_entry, getDoubleFromEntry(*fitted_f2_entry));
	putDoubleToEntry(*fraction_entry,
			 getDoubleFromEntry(*fitted_fraction_entry));
    }
}
//------------------------------------------------------------------------------
void Window1::on_class_borders_fixed_button_toggled(){
	//If user decides to fix the borders, those are read to GUI for later use
	if ( class_borders_fixed_button->get_active() ) {
		get_class_borders_from_data();
	}

}
//------------------------------------------------------------------------------
void Window1::set_2nd_peak_fit(bool active) {
	bool state = active;
	f2_entry->set_sensitive(state);
	fitted_f2err_entry->set_sensitive(state);
	fitted_f2_entry->set_sensitive(state);
	ff2_button->set_sensitive(state);


	fraction_entry->set_sensitive(state);
	fitted_fractionerr_entry->set_sensitive(state);
	fitted_fraction_entry->set_sensitive(state);
	ffraction_button->set_sensitive(state);
	fit_parameters->autofreq_button->set_sensitive(!state);
	fit_parameters->autofreq_button->set_active(!state);

	set_startfreq_menuitems_for_tof_vs_class_window();
}
//------------------------------------------------------------------------------
void Window1::set_3rd_peak_fit(bool active) {
	bool state = active;
//	set_2nd_peak_fit(state);
	f3_entry->set_sensitive(state);
	fitted_f3err_entry->set_sensitive(state);
	fitted_f3_entry->set_sensitive(state);
	ff3_button->set_sensitive(state);


	f3_weight_entry->set_sensitive(state);
	fitted_weight3_err_entry->set_sensitive(state);
	fitted_weight3_entry->set_sensitive(state);
	fweight3_button->set_sensitive(state);

}
//------------------------------------------------------------------------------
bool Window1::check_new_version(void) {
	FileFromWeb versioncheck;
	string latest_version;
	versioncheck.getFileAsString("http://research.jyu.fi/igisol/trap/lakritsi_version", latest_version);
//	cout << "juu: \"" << latest_version << "\"\n";
//	cout << "pituus " << latest_version.length() << "\n";
	if (latest_version.length() != 10 ) {
		cout << "couldn't check latest version! Network down?\n";
		return 0;
	}
	else {
		if ( latest_version.find_first_not_of("01234567890-") != latest_version.npos )
			cout << "Could not check for latest version!\n";
		else {
			if ( loc_tm(current_version+" 0:0:0") < loc_tm(latest_version+ " 0:0:0") ) {
//				cout << "vanha!\n";
				message_dialog.set_title("New version available!");
				message_dialog.set_message(
					"There appears to be a new version available!\n Current version: " + current_version +
					"\n New version: " + latest_version);
				message_dialog.run();
				message_dialog.hide();
			}
		}

	}


    return 0;
}
//------------------------------------------------------------------------------
/*void Window1::get_values_for_post_process(post_process_stuffs &stuffs)
{
    fit_parameters->get_values_for_post_process(stuffs);
	stuffs.freq2.v = getDoubleFromEntry(*f2_entry);
	stuffs.fraction.v = getDoubleFromEntry(*fraction_entry);

    stuffs.output_directory = getOutputDir();
    stuffs.ref_out_file = ref_outfile_entry->get_text();
    stuffs.meas_out_file = meas_outfile_entry->get_text();
    stuffs.post_process_filename = post_process_filename_entry->get_text();
}*/
//------------------------------------------------------------------------------
void Window1::on_post_process_button_clicked()
{
  /*
    Post_Processor coma;
    get_values_for_post_process(stuffi);
    if (&coma == NULL) {
	Post_Processor::create_widget(coma, gtk_builder);
	coma.what_version_is_COMA(revision);
	coma.signal_close().connect(sigc::mem_fun(*this,
						   &Window1::when_post_processing_endeth));
	set_sensitive(0);
    }
//    coma->init_mass_table(ame_mass_table_p); // Needs to be before init !!
    coma.init(&stuffi);
    coma.present();

    //fit_parameters->set_values_from_post_process(stuffi);*/
}
//------------------------------------------------------------------------------
void Window1::when_post_processing_endeth(void) {
//	cout << "Post rosessi kuoli.\n";
/*	fit_parameters->set_values_from_post_process(stuffi);
	coma->hide();
	set_sensitive();
*/
}
//------------------------------------------------------------------------------
void Window1::on_help_activated() {
    message_dialog.set_title("No Help");
	message_dialog.set_message("No manual available yet!\n");
	message_dialog.run();
	message_dialog.hide();
}
//------------------------------------------------------------------------------
int Window1::getNumberOfRoundsInFile(bool force) {
	if (is_in_constructor) return -1;

	#ifdef HAVE_ONLINE
	return -1;
	#endif
	if (datafmt_eva->get_active())
	{
		if (inputting->ref_radiobutton->get_active() )
		{
			return data->get_number_of_rounds_from_eva_data(inputting->filenumber_spinbutton->get_value_as_int(),ref_eva_files);
		}
		else
		{
			return data->get_number_of_rounds_from_eva_data(inputting->filenumber_spinbutton->get_value_as_int(),meas_eva_files);

		}

	}



//	if ( ! (inputting->twod_use_button->get_active() ) ) {
	return data->getNumberOfRoundsInFile(inputting->get_full_bunchfile_path(), force);
//	}
//	else {
//		return data->getNumberOfRoundsInFile(inputting->get_full_inputdata_dir(),
//						"/"+get_2d_filename(inputting->twod_filename_entry->get_text(),inputting->twod_suffix_entry->get_text(),inputting->filenumber_spinbutton->get_value_as_int() ),force);
//	}

}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Window1::on_dataformat_changed() {
	if (datafmt_eva->get_active() )
	{
		data->setDataFileFormat(DATAFORMAT_EVA);
		ref_element_entry->set_sensitive(false);
		meas_element_entry->set_sensitive(false);
		return;
	}
	else
	{
		ref_element_entry->set_sensitive(true);
		meas_element_entry->set_sensitive(true);
	}
	if (datafmt_med->get_active() ) {
		//cout << "MEd, not supported (reverted to new)\n";
		datafmt_new->set_active();
		//data->setDataFileFormat(0);
		data->setDataFileFormat(DATAFORMAT_NEW);
		return;
	}
	if (datafmt_new->get_active() ) {
		//cout << "new\n";
		data->setDataFileFormat(DATAFORMAT_NEW);
		return;
	}
	if (datafmt_old->get_active() ) {
		//cout << "old\n";
		data->setDataFileFormat(DATAFORMAT_OLD);
		return;
	}
	if (datafmt_newer->get_active() ) {
		//cout << "datafmt: since feb2009\n";
		data->setDataFileFormat(DATAFORMAT_NEWER);
		return;
	}

}
//------------------------------------------------------------------------------
void Window1::on_quit_activate() {
	hide();
}
//------------------------------------------------------------------------------
void Window1::on_save_to_ini_file_activate() {
	#ifdef HAVE_ONLINE
        store_settings("","lakritsi_online.ini",false);
	std::cout <<  "Saved to online_lakritsi.ini\n";
	#endif
	#ifndef HAVE_ONLINE
	store_settings("","lakritsi.ini",false);
	std::cout <<  "Saved to lakritsi.ini\n";
	#endif
}
//------------------------------------------------------------------------------
void Window1::on_save_to_default_ini_file_activate() {
	#ifdef HAVE_ONLINE
    	store_settings("","lakritsi_online.ini",false,  true);
	std::cout <<  "Saved to online_lakritsi.ini\n";
	#endif
	#ifndef HAVE_ONLINE
	store_settings("","lakritsi.ini",false, true);
	std::cout <<  "Saved to lakritsi.ini\n";
	#endif
}
//------------------------------------------------------------------------------
void Window1::on_windows_on_top_activate() {
	#ifdef HAVE_ONLINE
// 	toinen_yksi_plotarea_window->present();
	#endif
	five_plotareas_window->present();
	yksi_plotarea_window->present();
	#ifndef HAVE_ONLINE

	#endif
}
//------------------------------------------------------------------------------
void Window1::on_hide_small_windows_activate() {
	five_plotareas_window->iconify();
	yksi_plotarea_window->iconify();

	#ifdef HAVE_ONLINE
// 	toinen_yksi_plotarea_window->iconify();
	#endif
	#ifndef HAVE_ONLINE

	#endif
	present();
}
//------------------------------------------------------------------------------
void Window1::hide_windows() {
	/*tof_vs_class->hide();
	cts_vs_class->hide();
	fitted_class_results->hide();
	data_histogram->hide();
	event_histogram->hide();
	iconify();*/
}
//------------------------------------------------------------------------------
void Window1::reshow_windows() {

}
//------------------------------------------------------------------------------
void Window1::on_about_activate() {   //todo

	message_dialog.set_title("About");
	message_dialog.set_message("Lakritsi the Analyser\n\n"+get_title()+ "\n\nSend comments, bugs, etc. to\ntommi.o.eronen@jyu.fi" );
	message_dialog.run();
	message_dialog.hide();
}
//------------------------------------------------------------------------------
void Window1::on_load_settings_activate(){
	Gtk::FileChooserDialog dialog("Load setting file", Gtk::FILE_CHOOSER_ACTION_OPEN);

	Glib::RefPtr<Gtk::FileFilter> filter_any =
	    get_file_filter("*.settings", "*.settings");
	Glib::RefPtr<Gtk::FileFilter> filter_settings =
	    get_file_filter("*.*", "*.*");

	file_chooser_add_filter(dialog, filter_settings);
	file_chooser_add_filter(dialog, filter_any);

	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
	dialog.set_current_folder(outputdir_entry ->get_text() );
	dialog.set_default_response(Gtk::RESPONSE_OK);
	dialog.set_select_multiple(false);
	int result = dialog.run();
	if (result == Gtk::RESPONSE_OK) {
		loadIniFile(dialog.get_filename() );
	}
	return;
}
//------------------------------------------------------------------------------
void Window1::on_save_settings_activate()
{
	Glib::ustring outfilename;
	Gtk::FileChooserDialog dialog("Save setting file", Gtk::FILE_CHOOSER_ACTION_SAVE);

	Glib::RefPtr<Gtk::FileFilter> filter_any =
	    get_file_filter("*.settings", "*.settings");
	Glib::RefPtr<Gtk::FileFilter> filter_settings =
	    get_file_filter("*.*", "*.*");

	file_chooser_add_filter(dialog, filter_settings);
	file_chooser_add_filter(dialog, filter_any);

	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);
	dialog.set_current_folder( outputdir_entry->get_text() );
	dialog.set_default_response(Gtk::RESPONSE_OK);
	int result = dialog.run();
	if (result == Gtk::RESPONSE_OK) {
		outfilename = dialog.get_filename();
	}
	else
		return;
	store_settings("",outfilename,false);

}
//------------------------------------------------------------------------------
void Window1::on_choose_outdir_button_clicked()
{
	outputdir_dialog->set_current_folder(outputdir_entry ->get_text() );

	int result = outputdir_dialog->run();
	outputdir_dialog->hide();
	if (result == Gtk::RESPONSE_OK) {
		outputdir_entry -> set_text(outputdir_dialog->get_filename() );
	}
}
//------------------------------------------------------------------------------
void Window1::on_outputdir_entry_changed()
{
		//outputdir = outputdir_entry->get_text();
	outputdir = getOutputDir();
}
//------------------------------------------------------------------------------
void Window1::on_get_total_ions_button_clicked()
{
	validateSpinButtons();

}
//------------------------------------------------------------------------------
void Window1::on_general_boundaries_changed() //Changing bin or count window
{
	validateClassEditionButtons();
	return;
}
//------------------------------------------------------------------------------
void Window1::on_class_boundaries_changed()
{
	validateClassEditionButtons();
}
//------------------------------------------------------------------------------
void Window1::on_save_class_definition_button_clicked()
{
	LOCK
	data->setLowerEventBoundary(edit_class_spinbutton->get_value_as_int(),min_z_spinbutton->get_value_as_int());
	data->setUpperEventBoundary(edit_class_spinbutton->get_value_as_int(),max_z_spinbutton->get_value_as_int());
	class_division_result_entry->set_text( data->getClassDivResult() );
	set_was_class_div_ok_text();
	data->setClassNotFitted(edit_class_spinbutton->get_value_as_int() );
	get_class_borders_from_data();
	showFitResults();
	make_event_histogram_bars();
	RELEASE
}
//------------------------------------------------------------------------------
void Window1::on_edit_class_spinbutton_value_changed()
{
	validateSpinButtons();
	make_event_histogram_bars();
}
//------------------------------------------------------------------------------
void Window1::on_how_many_classes_spinbutton_value_changed()
{
	LOCK
	make_class_division(true);
	validateSpinButtons();
	make_event_histogram_bars();
	get_class_borders_from_data();
	RELEASE
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Window1::export_data_clicked()
{
	//cout << "export data clicked.\n";
	if (data->getFrequencyNumber() <= 0) {
		message_dialog.set_title("Strange!");
		message_dialog.set_message("Nothing to export!\nFile not existing?");
		message_dialog.run();
		message_dialog.hide();
		return;
	}
	if (data->isClassFitted(showclass_spinbutton->get_value_as_int()) != true) {
		message_dialog.set_title("Strange!");
		message_dialog.set_message("This class appears to be not fitted!\nNot outputting anything!");
		message_dialog.run();
		message_dialog.hide();
		return;
	}
//	int result = radiobutton1->get_active();
	Glib::ustring outdir,element,gnuplotfile,dataoutfilename;
	int scan_number;
	scan_number = inputting->filenumber_spinbutton->get_value_as_int();
	outdir=getOutputDir();
	if (inputting->ref_radiobutton->get_active() ) {
		gnuplotfile = ref_gnuplot_outfile_entry->get_text();
		dataoutfilename = ref_outfile_entry->get_text();
		element = inputting->ref_element_entry->get_text();
	}
	if (inputting->meas_radiobutton->get_active() ) {
		gnuplotfile = meas_gnuplot_outfile_entry->get_text();
		dataoutfilename = meas_outfile_entry->get_text();
		element = inputting->meas_element_entry->get_text();
	}
//	std::cout << "Datafile: " << dataoutfilename << "\n";
//	std::cout << "Gnuplotfile: " << gnuplotfile << "\n";
//	element=element_entry->get_text();
	Glib::ustring warningtext; warningtext = Window1::outputfile_warning(outdir,dataoutfilename,element,scan_number);

	int result(Gtk::RESPONSE_APPLY);
	if (warningtext != ""  ) {
		Gtk::MessageDialog overwrite_or_not_dialog(
				warningtext + "\n\nWhat do you want to do?",
				false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_NONE);
		overwrite_or_not_dialog.add_button("Do not output",Gtk::RESPONSE_CANCEL);
		overwrite_or_not_dialog.add_button("Output anyway",Gtk::RESPONSE_APPLY);
		overwrite_or_not_dialog.add_button("Overwrite old file",Gtk::RESPONSE_OK);
		overwrite_or_not_dialog.set_default_response(Gtk::RESPONSE_CANCEL);
		overwrite_or_not_dialog.set_title("Warning!");
		result = overwrite_or_not_dialog.run();
		overwrite_or_not_dialog.hide();
	}
	if (result == Gtk::RESPONSE_APPLY) {
		std::cout << data->outputFitsAndDats(outdir,element,scan_number);
		store_settings(outdir,dataoutfilename+".settings");
		data->outputGnuplotFile(outdir,gnuplotfile,element,scan_number,single_gnuplot_outfile_button->get_active(),".ps");
		data->outputAllFittedThings(outdir, dataoutfilename, element, scan_number, false, inputting->round_skips_entry->get_text(), getDoubleFromEntry(*sn_entry), revision);
	}
	if (result == Gtk::RESPONSE_OK) {
		std::cout << data->outputFitsAndDats(outdir,element,scan_number);
		store_settings(outdir,dataoutfilename+".settings");
		data->outputGnuplotFile(outdir,gnuplotfile,element,scan_number,single_gnuplot_outfile_button->get_active(),".ps");
		data->outputAllFittedThings(outdir, dataoutfilename, element, scan_number, true, inputting->round_skips_entry->get_text(), getDoubleFromEntry(*sn_entry), revision);
	}
}
//------------------------------------------------------------------------------
void Window1::StartFitting() //FITS all classes
{
	if (doing_fitting)
		return;
	if (is_in_constructor)
		return;
	doing_fitting = true;
	LOCK
	if (inputting->round_skips_entry->get_text() != "")
		warn_about_frequencies_having_different_amount_of_bunches();

	setInitialFitValues(std::cout);
	if (data->is_some_parameter_to_fit_initially_zero()) {
	  Gtk::MessageDialog erroridialogi("Some fitparameter has zero initial value!", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
	  erroridialogi.set_title("Error starting the fitting!!");
	  erroridialogi.set_keep_above(true);
	  erroridialogi.run();
	  doing_fitting = false;
	  RELEASE
	  return;
	}
	for (int i=0; i < data->getNumberOfClasses(); i++){
		#ifdef HAVE_ONLINE
		if (data->getCountsInClass(i,cout) < inputting->needed_online_ions_sp->get_value_as_int() )
			continue;
		#endif
		data->Fit(i, std::cout, fit_parameters->autofreq_button->get_active());
		showclass_spinbutton->set_value(i);	on_showclass_spinbutton_value_changed();
		redraw_windows_if_necessary(true);

	}
	showFittedClassResults();
	showFitResults();
	linear_fit_button_clicked();
	RELEASE
	doing_fitting = false;
}
//------------------------------------------------------------------------------
void Window1::autoDivideToClasses()
{
	LOCK
	if ( data->getFrequencyNumber() == 0){
		class_div_ok_entry->set_text("No data!");
		class_division_result_entry->set_text("");
		RELEASE
		return;
	}

	int cts_upper,cts_lower,bin_lower,bin_upper, mass,class_no;
	Glib::ustring temp;
	mass = inputting->mass_spinbutton->get_value_as_int();
	cts_lower = get_cts_min();
	cts_upper = get_cts_max();
	bin_lower = get_bin_min();
	bin_upper = get_bin_max();

	class_no = how_many_classes_spinbutton->get_value_as_int();
	//std::cout <<
	data->divideAutomaticallyToClasses(bin_lower,bin_upper,cts_lower,cts_upper,class_no,std::cout);
	std::cout << "Class division went fine. \n";
	RELEASE
}
//------------------------------------------------------------------------------
void Window1::LoadDataFile(bool append)
{
	if (is_in_constructor)
		return;
	LOCK
	if (!append)
	  DeleteData();
	data->do_not_load_first_bunch_after_freq_change =inputting->first_bunch_button->get_active();
	inputting->on_guess_mass_cb_toggled();
	inputting->on_guess_charge_cb_toggled();

	/// In case we're loading EVA format file. Note that evafiles in the directory are already loaded
	/// to memory. This sets the one in memory to actual data.
	if (datafmt_eva->get_active())
	{
		if (inputting->ref_radiobutton->get_active() )
		{
			int rounds = data->show_eva_data(inputting->filenumber_spinbutton->get_value_as_int(),ref_eva_files);
			set_number_of_rounds_in_file(rounds);
		}
		if (inputting->meas_radiobutton->get_active() )
		{
			int rounds = data->show_eva_data(inputting->filenumber_spinbutton->get_value_as_int(),meas_eva_files);
			set_number_of_rounds_in_file(rounds);
		}

	}
	else
	{
		set_number_of_rounds_in_file(getNumberOfRoundsInFile() );
		if (load_and_use_round_selection_automatically->get_active())
			load_rounds_to_skip(inputting->get_full_inputdata_dir().string()+"/skips");
		
		std::cout << data->readDirectory(inputting->get_full_bunchfile_path(),
						 inputting->startround_spinbutton->get_value_as_int(),
						 inputting->endround_spinbutton->get_value_as_int(),
						 std::cout);
		mass_scan_settings.load_settingfile(inputting->get_full_inputdata_dir().string()+"/mass scan settings.dat");
	}
	setInitialFitValues(std::cout);
	data->set_bunches_that_will_be_skipped(get_rounds_to_skip());
	practically_update_all();

	if (very_first_loading && data->getFrequencyNumber() > 0)
	{
		std::cout << "classes assigned?\n";
		on_auto_divide_to_classes_button_clicked();
		very_first_loading = false;
	}

	time_in_use->set_text(data->getAverageTime());
	scan_time_label->set_text(data->getAverageTime());
	RELEASE


}
//------------------------------------------------------------------------------
void Window1::DeleteData()
{
//	delete data;
//	data = new cFrequencies;

	data->deleteData();
	mass_scan_settings.delete_old();
	//Dont show any graphs
	tof_vs_class->nullify();
	cts_vs_class->nullify();
	data_histogram->nullify();
	event_histogram->nullify();
	fitted_class_results->nullify();
	//Set blanks to any result entries
	extrapolate_result_entry->set_text("");
	extrapolate_result_err_entry->set_text("");
	class_division_result_entry->set_text("");
	class_div_ok_entry->set_text("");
	inputting->rounds_show_entry->set_text("");
	time_in_use->set_text("");
	set_countrate_effect_not_fitted();
	sn_entry->set_text("");

	#ifdef HAVE_ONLINE
	online_bunches.clear();
	#endif

}
//------------------------------------------------------------------------------
void Window1::validateCountFitRanges(void) {
	right_fit_range_spinbutton->set_range(data->getMinimumFrequency(),data->getMaximumFrequency() );
	left_fit_range_spinbutton->set_range(data->getMinimumFrequency(),data->getMaximumFrequency() );
	right_fit_range_spinbutton->set_value(data->getMaximumFrequency() );
	left_fit_range_spinbutton->set_value(data->getMinimumFrequency() );
}
//------------------------------------------------------------------------------

void Window1::ShowDataHistogram() //DRAWS histogram out of all data - counts as a function of TOF
{
	LOCK
	tof_histogram_data = data->get_histogram();
	// std::cout << "size: " << tof_histogram_data->size() << "\n";
	tof_histogram_plot_container.set_data(tof_histogram_data);
	tof_histogram_plot_container.graph_type = FANCY_HISTOGRAM;
	tof_histogram_plot_container.color = Gdk::Color("Blue");
	tof_histogram_plot_container.color_alpha = 0.3;
	tof_histogram_plot_container.set_data_compression_factor(histogram_compression_factor);
	tof_histogram_plot_container.data_name = "TOF projection";
	data_histogram->add_data(&tof_histogram_plot_container);
	
	
	data_histogram->show_data();
	data_histogram->autosetPlottingBox();
	data_histogram_need_update = true;

	
	// And then event histogram
	event_histogram_data = data->get_event_histogram();
	event_histogram_plot_container.set_data(event_histogram_data);
	event_histogram_plot_container.graph_type = FANCY_HISTOGRAM;
	event_histogram_plot_container.color = Gdk::Color("Blue");
	event_histogram_plot_container.color_alpha = 0.3;
	event_histogram_plot_container.data_name = "Events";
	event_histogram->add_data(&event_histogram_plot_container);

	event_histogram->show_data();
	event_histogram->autosetPlottingBox();
	event_histogram_need_update = true;
	
/*	data->getHistogram(counts_in_bin);
	//int data_histogram_compression_factor=1;
	DATASET baardata;
	//cout << "siize " << baardata.data.size() << "\n";
	convert_histogram_and_compress(counts_in_bin,baardata,
								   histogram_compression_factor);
	baardata.graph_type = FANCY_HISTOGRAM;
	baardata.color = Gdk::Color("Blue");
	baardata.color_alpha=0.2;
	baardata.data_name = "All ions";
	baardata.legend_position = TOP_LEFT;

	data_histogram->input_data(baardata);
	data_histogram->show_data();
	data_histogram->autosetPlottingBox();
	data_histogram_need_update = true;


	data->getEventsAsTable(events); events.push_back(0);
	event_histogram->set_dataname("# Bunches", TOP_LEFT);
	event_histogram->input_bar_data(events);
	event_histogram->set_graph_style(FANCY_HISTOGRAM);
	//event_histogram->dat.set_cairo_rgba(1.0,0.0,0.0,0.3);
	event_histogram->dat.set_color_alpha(0.3);
	event_histogram->dat.color= Gdk::Color("Red");

	count_events.clear();
	int num(0);
	for (vector<int>::iterator i=events.begin(); i != events.end(); ++i) {
		count_events.push_back( (*i) * num);
		num++;
	}
	event_histogram->set_dataname2("# Ions", TOP_LEFT);
	event_histogram->input_bar_data2(count_events);
	event_histogram->set_graph_style2(FANCY_HISTOGRAM);
	//event_histogram->dat2.set_cairo_rgba(0.0,0.0,1.0,0.2);
	event_histogram->dat2.set_color_alpha(0.2);
	event_histogram->dat2.color= Gdk::Color("Blue");

	event_histogram->show_data();
	event_histogram->autosetPlottingBox();
	event_histogram->expand_xtics(1); event_histogram->set_mxtics(5);
	event_histogram_need_update=true;

	data_histogram_need_update = true;
*/
	RELEASE
}
//------------------------------------------------------------------------------
void Window1::showDataInfo()
{
	message_dialog.set_message(data->getMCSdata());
	message_dialog.set_title("Info on data");
	message_dialog.run();
	if (using_debugging) {
		data->printContentOfFrequency(debug_level->get_value_as_int());
		data->printFitFields();
	}
	message_dialog.hide();
}
//------------------------------------------------------------------------------
void Window1::startSingleClassFitting()
{
	if (doing_fitting)
		return;
	doing_fitting = true;
	if (inputting->round_skips_entry->get_text() != "")
		warn_about_frequencies_having_different_amount_of_bunches();

	LOCK
	setInitialFitValues(std::cout);
	data->Fit(showclass_spinbutton->get_value_as_int(), std::cout,
		  fit_parameters->autofreq_button->get_active());
	showFittedClassResults();
	showFitResults();
	RELEASE
	doing_fitting = false;
}
//------------------------------------------------------------------------------
void Window1::linear_fit_button_clicked()
{
	LOCK
	double slope=0.00001, constant=1.7e6;
	double slope_err, const_err,covariance;
	DATASET fit,extrapolated_value;
	extrapolated_value.graph_type = YERR;
	data->InitializeLinearFit(
				getDoubleFromEntry(*inislope_entry)/1000.,
				fitslope_checkbutton->get_active(),
				autoslope_checkbutton->get_active(),
				getDoubleFromEntry(*iniconst_entry),
				fitconst_checkbutton->get_active(),
				autoconst_checkbutton->get_active());


	if (data->FitLinear(slope,constant,0,std::cout)){
		data->getLinearFitErrors(slope_err,const_err,covariance);
		data->getLinearFit(fit,0,data->getAverageCountsInClass(how_many_classes_spinbutton->get_value_as_int()-1)+1.0,30);
		putDoubleToEntry(*slope_entry,slope*1000.);
		putDoubleToEntry(*dslope_entry,slope_err*1000.);
		putDoubleToEntry(*const_entry,constant);
		putDoubleToEntry(*dconst_entry,const_err);
		putDoubleToEntry(*lin_cov_entry,covariance,1);

//		std::cout << "joo\n";
//		fitted_class_results->input_linear_fit(slope, constant, 50);
//		fitted_class_results->input_linear_fit_error(slope_err,const_err,data->getAverageCountsInClasses(std::cout),true);
		fit.color=Gdk::Color("Blue");
		fitted_class_results->input_fit(fit,true);
		fitted_class_results->show_fit(true);
		showFittedClassResults();
		double xpolation_to = getDoubleFromEntry(*countrate_extrapolate_entry);
		data->setInterpolatedCountrateEffect_x(xpolation_to);
		double xpol_value = data->getInterpolatedCountrateEffectValue(xpolation_to,std::cout);
		double xpol_value_err = data->getInterpolatedCountrateEffectValueError(xpolation_to,std::cout);

		try {
//		fitted_class_results
//				->setPlottingBox(0,fit.data.begin()->y_value-fit.data.begin()->y_value_err,data->getAverageCountsInClass(how_many_classes_spinbutton->get_value_as_int()-1)+1.0,fit.data[fit.data.size()-1].y_value+fit.data[fit.data.size()-1].y_value_err);
		}
		catch (...) {
				message_dialog.set_title("Bug!");
				message_dialog.set_message("Bug bug while st_plotting_box\n");
				message_dialog.run();
				message_dialog.hide();
		}
		Datapoint extr; extr.x_value = xpolation_to; extr.y_value = xpol_value; extr.y_value_err = xpol_value_err;
		//extrapolated_value.data.push_back(extr);
		zclass_extrapolation_plot_data.push_back(extr);
		//fitted_class_results->input_data2(extrapolated_value);
		fitted_class_results_need_update = true;
		putDoubleToEntry(*extrapolate_result_entry,xpol_value);
		putDoubleToEntry(*extrapolate_result_err_entry,xpol_value_err);
	}
	else{
		set_countrate_effect_not_fitted();
	}
	RELEASE
}
//------------------------------------------------------------------------------
void Window1::on_showclass_spinbutton_value_changed()
{
	LOCK
	showFitResults();
	show_single_class_histogram();
	RELEASE
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//After this: OWN FUNCTIONS

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Glib::ustring Window1::getOutputDir(void) {
	std::string temp;
	temp = outputdir_entry->get_text();
	std::string::size_type len;
	len = temp.length();
	if (len > 0) {
		if (  (temp[len-1] == '/') || (temp[len-1] == '\\') ) {
			temp[len-1] = '/';
		}
		else
			temp +='/';

	}
	return temp;
}
//------------------------------------------------------------------------------
Glib::ustring Window1::get_2d_filename(Glib::ustring before, Glib::ustring after, int nummber) {
	Glib::ustring nimi;
	char charnum[50];
	nimi.append(before);
	sprintf(charnum,"%05d",nummber);
	nimi.append(charnum);
	nimi.append(after);
//	std::cout << "\n---" << nimi << "---\n";

	return nimi;
}
//------------------------------------------------------------------------------
void Window1::validateSpinButtons(void){
	//Check, that no illegal class number is in box
	int was_showclass_number = showclass_spinbutton->get_value_as_int();
	showclass_spinbutton->set_range(0,(data->getNumberOfClasses()-1));
	showclass_spinbutton->set_value(was_showclass_number);

	//Check that batch process numbers are correct
	int max;
	string value= inputting->endfile_spinbutton->get_text();
	inputting->endfile_spinbutton->set_range(inputting->startfile_spinbutton->get_value_as_int(),10000);
	inputting->endfile_spinbutton->set_text(value);
	inputting->on_round_spinbutton_value_changed();

	//Check that edit_class_spinbutton is not too much
	max = edit_class_spinbutton->get_value_as_int();
	edit_class_spinbutton->set_range(0,(data->getNumberOfClasses()-1));
	edit_class_spinbutton->set_value(max);

	//Put start_cts and end_cts to editing boxes
	min_z_spinbutton->set_value(data->getLowerEventBoundary(edit_class_spinbutton->get_value_as_int()));
	max_z_spinbutton->set_value(data->getUpperEventBoundary(edit_class_spinbutton->get_value_as_int()));
	validateClassEditionButtons();
	validateWindowingSpinbuttons();

	putIntegerToEntry(*total_ions_show_entry,data->getTotalCounts(
						get_bin_min(),
						get_bin_max(),
						get_cts_min(),
						get_cts_max(),
						std::cout));
	return;
}
//------------------------------------------------------------------------------
void Window1::validateClassEditionButtons(void){
	min_z_spinbutton->set_range(get_cts_min(),get_cts_max());
	int max = max_z_spinbutton->get_value_as_int();
	max_z_spinbutton->set_range(min_z_spinbutton->get_value_as_int(),get_cts_max());
	max_z_spinbutton->set_value(max);
	putIntegerToEntry(*ions_in_class_show_entry,data->getTotalCounts(
						get_bin_min(),
						get_bin_max(),
						min_z_spinbutton->get_value_as_int(),
						max_z_spinbutton->get_value_as_int(),
						std::cout));

	return;
}
//------------------------------------------------------------------------------
void Window1::validateWindowingSpinbuttons(){
	LOCK
	double maxxi = ref_bin_max_spinbutton->get_value();
	ref_bin_max_spinbutton->set_range(ref_bin_min_spinbutton->get_value(),10000);
	ref_bin_max_spinbutton->set_value(maxxi);

	maxxi = ref_cts_max_spinbutton->get_value();
	ref_cts_max_spinbutton->set_range(ref_cts_min_spinbutton->get_value(),10000);
	ref_cts_max_spinbutton->set_value(maxxi);

	maxxi = meas_bin_max_spinbutton->get_value();
	meas_bin_max_spinbutton->set_range(meas_bin_min_spinbutton->get_value(),10000);
	meas_bin_max_spinbutton->set_value(maxxi);

	maxxi = meas_cts_max_spinbutton->get_value();
	meas_cts_max_spinbutton->set_range(meas_cts_min_spinbutton->get_value(),10000);
	meas_cts_max_spinbutton->set_value(maxxi);

	data_histogram->show_xbars(true);
	//NYTH
	if (!data_histogram->xbars.replace_value_in_bar(get_bin_min(), "bin_min")) {
		data_histogram->xbars.add_bar(BAR(get_bin_min(),Gdk::Color("Blue"),"","bin_min"));
	}
	if (!data_histogram->xbars.replace_value_in_bar(get_bin_max(), "bin_max")) {
		data_histogram->xbars.add_bar(BAR(get_bin_max(),Gdk::Color("Blue"),"","bin_max"));
	}
	data_histogram_need_update = true;

	RELEASE
}
//------------------------------------------------------------------------------
std::string Window1::showFitResults(void){ //Shows fitted params and DRAWS fitted class
	LOCK
	int i;
	double fit_results[20];
	bool was_it_fitted[20];
	double chisq;
	int class_no = showclass_spinbutton->get_value_as_int();
	if (data->getNumberOfClasses() < 1){
		if (!is_in_constructor)
			//std::cout << "No classes yet! Make class division!\n";
		RELEASE
		return "";
	}

// Fit results
	data->getFitResults(class_no,fit_results,i);
	if (data->isClassFitted(class_no) ) {
		if ( data->get_fit_func_type() == SINGLE_PEAK_FIT ||
		     data->get_fit_func_type() == DOUBLE_PEAK_FIT ||
		     data->get_fit_func_type() == TRIPLE_PEAK_FIT
						|| data->get_fit_func_type() == SINGLE_PEAK_WITH_CRAP )
		{
			//NYTH
			//tof_vs_class->xbars.delete_bars(); //BAARS
			if (!tof_vs_class->xbars.replace_value_in_bar(fit_results[0],"resofreq1")) {
				tof_vs_class->xbars.add_bar(BAR(fit_results[0],Gdk::Color("Blue"),"f1","resofreq1"));
			}
			tof_vs_class->xbars.delete_bar("resofreq2");
  			tof_vs_class->xbars.delete_bar("resofreq3");
			//tof_vs_class->add_xbar(fit_results[0]);
			if (data->is_2nd_peak_in_the_game() )
				tof_vs_class->xbars.add_bar(BAR(fit_results[8],Gdk::Color("Blue"),"f2","resofreq2"));
			if (data->is_3rd_peak_in_the_game() )
			{
				tof_vs_class->xbars.add_bar(BAR(fit_results[10],Gdk::Color("Blue"),"f3","resofreq3"));
			}
			tof_vs_class->show_xbars(true);

			putDoubleToEntry(*fit_parameters->fitted_f_entry,fit_results[0]);
			putDoubleToEntry(*fit_parameters->fitted_p_entry,fit_results[1],1);
			putDoubleToEntry(*fit_parameters->fitted_ax_entry,fit_results[2]);
			putDoubleToEntry(*fit_parameters->fitted_magn_entry,fit_results[3]);
			putDoubleToEntry(*fit_parameters->fitted_cycl_entry,fit_results[4],1);
			putDoubleToEntry(*fit_parameters->fitted_fi_entry,fit_results[5]);
			putDoubleToEntry(*fit_parameters->fitted_t_rf_entry,fit_results[6]);
			putDoubleToEntry(*fit_parameters->fitted_a_rf_entry,fit_results[7]);
			if (data->is_2nd_peak_in_the_game() ) {
				putDoubleToEntry(*fitted_f2_entry,fit_results[8]);
				putDoubleToEntry(*fitted_fraction_entry,fit_results[9]);
			}
			if (data->is_3rd_peak_in_the_game() ) {
				putDoubleToEntry(*fitted_f3_entry,fit_results[10]);
				putDoubleToEntry(*fitted_weight3_entry,fit_results[11]);
			}
			if (data->get_fit_func_type() == SINGLE_PEAK_WITH_CRAP) {
				putDoubleToEntry(*fitted_crap_tof_entry,fit_results[8]);
				putDoubleToEntry(*fitted_crap_fraction_entry,fit_results[9]);
			}

			if (fit_results[1] < 0 ) {
				st_normal_bg_color(*fit_parameters->fitted_p_entry,
						   error);
			}
			else
				st_normal_bg_color(*fit_parameters->fitted_p_entry, default_widget_base_color);



		// Uncertainties
			data->getFitErrors(class_no,fit_results,was_it_fitted,i,chisq);
			if (was_it_fitted[0])
				putDoubleToEntry(*fit_parameters->fitted_ferr_entry,fit_results[0]);
			else
				fit_parameters->fitted_ferr_entry->set_text("FIXED");
			if (was_it_fitted[1])
				putDoubleToEntry(*fit_parameters->fitted_perr_entry,fit_results[1],1);
			else
				fit_parameters->fitted_perr_entry->set_text("FIXED");
			if (was_it_fitted[2])
				putDoubleToEntry(*fit_parameters->fitted_axerr_entry,fit_results[2]);
			else
				fit_parameters->fitted_axerr_entry->set_text("FIXED");
			if (was_it_fitted[3])
				putDoubleToEntry(*fit_parameters->fitted_magnerr_entry,fit_results[3]);
			else
				fit_parameters->fitted_magnerr_entry->set_text("FIXED");
			if (was_it_fitted[4])
				putDoubleToEntry(*fit_parameters->fitted_cyclerr_entry,fit_results[4],1);
			else
				fit_parameters->fitted_cyclerr_entry->set_text("FIXED");
			if (was_it_fitted[5])
				putDoubleToEntry(*fit_parameters->fitted_fierr_entry,fit_results[5]);
			else
				fit_parameters->fitted_fierr_entry->set_text("FIXED");
			if (was_it_fitted[6])
				putDoubleToEntry(*fit_parameters->fitted_t_rferr_entry,fit_results[6]);
			else
				fit_parameters->fitted_t_rferr_entry->set_text("FIXED");
			if (was_it_fitted[7])
				putDoubleToEntry(*fit_parameters->fitted_a_rferr_entry,fit_results[7]);
			else
				fit_parameters->fitted_a_rferr_entry->set_text("FIXED");
			if (data->is_2nd_peak_in_the_game()) { //2nd peak stuff
				if (was_it_fitted[8])
					putDoubleToEntry(*fitted_f2err_entry,fit_results[8]);
				else
					fitted_f2err_entry->set_text("FIXED");
				if (was_it_fitted[9])
					putDoubleToEntry(*fitted_fractionerr_entry,fit_results[9]);
				else
					fitted_fractionerr_entry->set_text("FIXED");
			}
			if (data->is_3rd_peak_in_the_game()) { //3rd peak stuff
				if (was_it_fitted[10])
					putDoubleToEntry(*fitted_f3err_entry,fit_results[10]);
				else
					fitted_f3err_entry->set_text("FIXED");
				if (was_it_fitted[11])
					putDoubleToEntry(*fitted_weight3_err_entry,fit_results[11]);
				else
					fitted_weight3_err_entry->set_text("FIXED");
			}
			if (data->get_fit_func_type() == SINGLE_PEAK_WITH_CRAP) {
				if (was_it_fitted[8])
					putDoubleToEntry(*fitted_crap_toferr_entry,fit_results[8]);
				else
					fitted_crap_toferr_entry->set_text("FIXED");
				if (was_it_fitted[9])
					putDoubleToEntry(*fitted_crap_fractionerr_entry,fit_results[9]);
				else
					fitted_crap_fractionerr_entry->set_text("FIXED");
			}

		//Chisq
			putDoubleToEntry(*chisq_entry, chisq);
			putDoubleToEntry(*chisq_2nd, chisq);
		}
		// Results, when RAMSEY was in use
		if (data->get_fit_func_type() == SINGLE_RAMSEY || data->get_fit_func_type() == SINGLE_RAMSEY_WITH_CRAP
				  || data->get_fit_func_type() == SINGLE_RAMSEY_WITH_DAMPING ) {
			//NYTH
			//tof_vs_class->xbars.delete_bars(); //BAARS
			if (!tof_vs_class->xbars.replace_value_in_bar(fit_results[0],"resofreq1")) {
				tof_vs_class->xbars.add_bar(BAR(fit_results[0],Gdk::Color("Blue"),"f1","resofreq1"));
			}
			//tof_vs_class->add_xbar(fit_results[0]);
			tof_vs_class->show_xbars(true);

			putDoubleToEntry(*fit_parameters->fitted_f_entry,fit_results[0]);
			putDoubleToEntry(*fit_parameters->fitted_ax_entry,fit_results[1]);
			putDoubleToEntry(*fit_parameters->fitted_magn_entry,fit_results[2]);
			putDoubleToEntry(*fit_parameters->fitted_t_rf_entry,fit_results[3]);
			putDoubleToEntry(*fit_parameters->fitted_t_wait_entry,fit_results[4]);
			putDoubleToEntry(*fit_parameters->fitted_a_rf_entry,fit_results[5]);
			if (data->get_fit_func_type() == SINGLE_RAMSEY_WITH_CRAP) {
				putDoubleToEntry(*fitted_crap_tof_entry,fit_results[6]);
				putDoubleToEntry(*fitted_crap_fraction_entry,fit_results[7]);
			}
			if (data->get_fit_func_type() == SINGLE_RAMSEY_WITH_DAMPING) {
				putDoubleToEntry(*fitted_p_entry,fit_results[6]);
			}

		// Uncertainties
			data->getFitErrors(class_no,fit_results,was_it_fitted,i,chisq);
			if (was_it_fitted[0])
				putDoubleToEntry(*fit_parameters->fitted_ferr_entry,fit_results[0]);
			else
				fit_parameters->fitted_ferr_entry->set_text("FIXED");
			if (was_it_fitted[1])
				putDoubleToEntry(*fit_parameters->fitted_axerr_entry,fit_results[1]);
			else
				fit_parameters->fitted_axerr_entry->set_text("FIXED");
			if (was_it_fitted[2])
				putDoubleToEntry(*fit_parameters->fitted_magnerr_entry,fit_results[2]);
			else
				fit_parameters->fitted_magnerr_entry->set_text("FIXED");
			if (was_it_fitted[3])
				putDoubleToEntry(*fit_parameters->fitted_t_rferr_entry,fit_results[3]);
			else
				fit_parameters->fitted_t_rferr_entry->set_text("FIXED");
			if (was_it_fitted[4])
				putDoubleToEntry(*fit_parameters->fitted_t_waiterr_entry,fit_results[4]);
			else
				fit_parameters->fitted_t_waiterr_entry->set_text("FIXED");
			if (was_it_fitted[5])
				putDoubleToEntry(*fit_parameters->fitted_a_rferr_entry,fit_results[5]);
			else
				fit_parameters->fitted_a_rferr_entry->set_text("FIXED");
			if (data->get_fit_func_type() == SINGLE_RAMSEY_WITH_CRAP) {
				if (was_it_fitted[6])
					putDoubleToEntry(*fitted_crap_toferr_entry,fit_results[6]);
				else
					fitted_crap_toferr_entry->set_text("FIXED");
				if (was_it_fitted[7])
					putDoubleToEntry(*fitted_crap_fractionerr_entry,fit_results[7]);
				else
					fitted_crap_fractionerr_entry->set_text("FIXED");
			}
			if (data->get_fit_func_type() == SINGLE_RAMSEY_WITH_DAMPING) {
				if (was_it_fitted[6])
					putDoubleToEntry(*fitted_perr_entry,fit_results[6]);
				else
					fitted_perr_entry->set_text("FIXED");
			}

		//Chisq
			putDoubleToEntry(*chisq_entry, chisq);
			putDoubleToEntry(*chisq_2nd,chisq);
		}
	}
	else {
		tof_vs_class->show_xbars(false);
		fit_parameters->fitted_f_entry->set_text("-");
		fit_parameters->fitted_p_entry->set_text("-");
		fit_parameters->fitted_ax_entry->set_text("-");
		fit_parameters->fitted_magn_entry->set_text("-");
		fit_parameters->fitted_cycl_entry->set_text("-");
		fit_parameters->fitted_fi_entry->set_text("-");
		fit_parameters->fitted_t_rf_entry->set_text("-");
		fit_parameters->fitted_t_wait_entry->set_text("-");
		fit_parameters->fitted_a_rf_entry->set_text("-");
		if (data->is_2nd_peak_in_the_game() ) {
			fitted_f2_entry->set_text("-");
			fitted_f2err_entry->set_text("-");
			fitted_fraction_entry->set_text("-");
			fitted_fractionerr_entry->set_text("-");
		}
		if (data->is_3rd_peak_in_the_game() ) {
			fitted_f3_entry->set_text("-");
			fitted_f3err_entry->set_text("-");
			fitted_weight3_entry->set_text("-");
			fitted_weight3_err_entry->set_text("-");
		}
		fit_parameters->fitted_ferr_entry->set_text("-");
		fit_parameters->fitted_perr_entry->set_text("-");
		fit_parameters->fitted_axerr_entry->set_text("-");
		fit_parameters->fitted_magnerr_entry->set_text("-");
		fit_parameters->fitted_cyclerr_entry->set_text("-");
		fit_parameters->fitted_fierr_entry->set_text("-");
		fit_parameters->fitted_t_rferr_entry->set_text("-");
		fit_parameters->fitted_t_waiterr_entry->set_text("-");
		fit_parameters->fitted_a_rferr_entry->set_text("-");



		chisq_entry->set_text("--");
		chisq_2nd->set_text("--");
	}
//Showing tof data points
	Glib::ustring header = "TOF vs CLASS ";
	char num[5];
	sprintf(num,"%d",class_no);
	
	header.append(num);
	tof_vs_scan_parameter_data = data->get_tof_vs_frequency(class_no);
	tof_vs_scan_parameter_plot_container.set_data(tof_vs_scan_parameter_data);
	tof_vs_scan_parameter_plot_container.graph_type = YERR;
	tof_vs_scan_parameter_plot_container.set_color_alpha(1.0);
	tof_vs_scan_parameter_plot_container.color = Gdk::Color("black");
	tof_vs_scan_parameter_plot_container.data_name = "TOF vs scan parameter data";
	
	tof_vs_class->add_data(&tof_vs_scan_parameter_plot_container);
	tof_vs_class->set_xmin_with_force(data->getMinimumFrequency());
	tof_vs_class->set_xmax_with_force(data->getMaximumFrequency());
	tof_vs_class->autosetPlottingBox();
	tof_vs_class->show_data(true);


	tof_vs_scan_parameter_fit = data->getFitAsPoints(class_no);
	tof_vs_scan_parameter_fit_plot_container.set_data(tof_vs_scan_parameter_fit);
	tof_vs_scan_parameter_fit_plot_container.graph_type = LINE;
	tof_vs_scan_parameter_fit_plot_container.set_color_alpha(1.0);
	tof_vs_scan_parameter_fit_plot_container.color = Gdk::Color("blue");
	tof_vs_scan_parameter_fit_plot_container.data_name = "TOF vs scan parameter fit";
	
	tof_vs_class->add_data(&tof_vs_scan_parameter_fit_plot_container);

	// Old style fit container
	/*
	if (data->getFitAsPoints(class_no,tof_vs_class->fit.data)) {
		tof_vs_class->fit.color = Gdk::Color("Blue");
		tof_vs_class->show_fit(true);
	}
	else
		tof_vs_class->show_fit(false);

	tof_vs_class->autosetPlottingBox();*/
//	tof_vs_class->show_contents();
	tof_vs_class_need_update = true;

//Showing counts data points
	header = "CTS vs CLASS "; header.append(num);
	#ifdef HAVE_ONLINE
	//header.append(" (online)");
	#endif
	cts_vs_class->set_plot_title(header);
	{
		counts_vs_scan_parameter_data = data->get_counts_vs_frequency(class_no);
		counts_vs_scan_parameter_plot_container.set_data(counts_vs_scan_parameter_data);
		counts_vs_scan_parameter_plot_container.graph_type = FANCY_HISTOGRAM;
		counts_vs_scan_parameter_plot_container.set_color_alpha(0.3);
		counts_vs_scan_parameter_plot_container.color = Gdk::Color("Purple");
		counts_vs_scan_parameter_plot_container.data_name = "Counts vs scan parameter";
		
		cts_vs_class->add_data(&counts_vs_scan_parameter_plot_container);
		cts_vs_class->set_xmin_with_force(data->getMinimumFrequency());
		cts_vs_class->set_xmax_with_force(data->getMaximumFrequency());
		cts_vs_class->autosetPlottingBox();
		cts_vs_class->show_data(true);
		
		/*
		DATASET counts_dataset;
		counts_dataset.graph_type = FANCY_HISTOGRAM;
		data->getCountsOfClass(counts_dataset,class_no);
		
		cts_vs_class->input_data(counts_dataset);
		cts_vs_class->autosetPlottingBox();
		cts_vs_class->show_data(true);
		*/
		cts_vs_class_need_update=true;
	}
//Showing 2d-contour stuff
	get_and_show_2d_contour_of_cts();
	showFittedClassResults();
	RELEASE
	return "";
}

//------------------------------------------------------------------------------
void Window1::setInitialFitValues(ostream &log){
//	cout << "Mass value " <<  mass_spinbutton->get_value_as_int() << "\n";
//	std::cout << data->initializeFitSimion(
//		mass_spinbutton->get_value_as_int(),
//		"trap10v_slow_flatpot.pa0",
//		"mag.pa"
//	);

	if (fit_parameters->auto_t_tot_button->get_active() )
		putDoubleToEntry(*fit_parameters->t_tot_entry,mass_scan_settings.get_duration("Trap2 cyclotron"));
	if (fit_parameters->auto_conv_button->get_active() )
		putDoubleToEntry(*fit_parameters->a_rf_entry,mass_scan_settings.get_amplitude("Trap2 cyclotron"));

	//cout << "here" << endl;
	//Inputting mass
	data->initializeFit_fast_tof(inputting->mass_spinbutton->get_value_as_int(),inputting->charge_spinbutton->get_value_as_int() );

	//if (function_konig->get_active() ) {
	if (are_we_fitting_konig() ) {
		std::cout << data->initializeSingleFit(
				getDoubleFromEntry(*fit_parameters->f_entry),
				fit_parameters->ff_button->get_active(),
				getDoubleFromEntry(*fit_parameters->p_entry),
				fit_parameters->fp_button->get_active(),
				getDoubleFromEntry(*fit_parameters->ax_entry),
				fit_parameters->fax_button->get_active(),
				getDoubleFromEntry(*fit_parameters->magn_entry),
				fit_parameters->fmagn_button->get_active(),
				getDoubleFromEntry(*fit_parameters->cycl_entry),
				fit_parameters->fcycl_button->get_active(),
				getDoubleFromEntry(*fit_parameters->fi_entry),
				fit_parameters->ffi_button->get_active(),
				getDoubleFromEntry(*fit_parameters->t_tot_entry),
				fit_parameters->ft_rf_button->get_active(),
				getDoubleFromEntry(*fit_parameters->a_rf_entry),
				fit_parameters->fa_rf_button->get_active()
		);
		// if fitting multiple peaks
		//cout << "boxtext: \"" << fit_function_combo_box_p->get_active_text() << "\"" << endl;
		if (fit_function_combo_box_p->get_active_text() == KONIG3 ||
		    fit_function_combo_box_p->get_active_text() == KONIG4 )
		{
			data->initializeSecondPeakForFitting(
				getDoubleFromEntry(*f2_entry),
				ff2_button->get_active(),
				getDoubleFromEntry(*fraction_entry),
				ffraction_button->get_active());
				//cout << "2nd or 3rd peak ini\n";
		}
		if (fit_function_combo_box_p->get_active_text() == KONIG4 )
		{
			data->initializeThirdPeakForFitting(
				getDoubleFromEntry(*f3_entry),
				ff3_button->get_active(),
				getDoubleFromEntry(*f3_weight_entry),
				fweight3_button->get_active());
				//cout << "3rd peak ini\n";
		}
	}
	//Initialize Ramsey fit
	//if (function_ramsey->get_active() || function_ramsey_damping->get_active() ) {
	if (are_we_fitting_ramsey() ) {
		std::cout << data->initializeSingleRamseyFit(
				getDoubleFromEntry(*fit_parameters->f_entry),		//Frequency
				fit_parameters->ff_button->get_active(),
				getDoubleFromEntry(*fit_parameters->ax_entry),	//Axial energy
				fit_parameters->fax_button->get_active(),
				getDoubleFromEntry(*fit_parameters->magn_entry),	//Magnetron0
				fit_parameters->fmagn_button->get_active(),
        getDoubleFromEntry(*fit_parameters->t_RF_ramsey_entry), //t_wait -> T_RF fringe for Ramsey
        fit_parameters->ft_wait_button->get_active(),
				getDoubleFromEntry(*fit_parameters->t_tot_entry),	//t_fringe --> t_tot
				fit_parameters->ft_rf_button->get_active(),
				getDoubleFromEntry(*fit_parameters->a_rf_entry),		//amplitude
				fit_parameters->fa_rf_button->get_active()
		);
		//Initialize Ramsey damping part
		//if (function_ramsey_damping->get_active() ) {
		if ( fit_function_combo_box_p->get_active_text() == RAMSEY3 ) {
			std::cout << data->initializeRamseyDamping(
					getDoubleFromEntry(*fit_parameters->p_entry),		//pressure
					fit_parameters->fp_button->get_active()
			);
		}
	}

	//if (fit_crap_chooser->get_active() && !two_peak_chooser->get_active() ) {
	if (fit_function_combo_box_p->get_active_text() == RAMSEY2 || fit_function_combo_box_p->get_active_text() == KONIG2) {
		data->initializeConstantCrapForSinglePeakFit(
				getDoubleFromEntry(*fit_parameters->crap_tof_entry),
				fit_parameters->fcrap_tof_button->get_active(),
				getDoubleFromEntry(*fit_parameters->crap_fraction_entry),
				fit_parameters->fcrap_fraction_button->get_active()
		);
	}

	return;
}
//------------------------------------------------------------------------------
void Window1::showFittedClassResults(void){ // DRAWS freq vs counts/bunch & interpolation result out
	LOCK
	double ave_counts_in_class[20];
	double fittedfreq[20];
	double fittedfreq_err[20];
	
	zclass_plot_data.clear();
	
	Datapoint to_add;
	
	//int counter(0);
	for (int i=0;i<data->getNumberOfClasses();i++)
	{
		if (data->isClassFitted(i))
		{
			//ave_counts_in_class[counter]=data->getAverageCountsInClass(i);
			//data->getFittedValueAndChisqAdjustedError(i,fittedfreq[counter],fittedfreq_err[counter]);
			//counter++;
			to_add.x_value = data->getAverageCountsInClass(i);
			data->getFittedValueAndChisqAdjustedError(i,to_add.y_value,to_add.y_value_err);
			zclass_plot_data.push_back(to_add);
		}
	}
	//fitted_class_results->input_data(ave_counts_in_class,fittedfreq,fittedfreq_err,counter,"",YERR);

	fitted_class_results->show_data(true);
	fitted_class_results->autosetPlottingBox();
	fitted_class_results->setPlottingBox(0,-1,-1,-1);
//	fitted_class_results->show_contents();
	fitted_class_results_need_update=true;
	RELEASE
	return;
}
//------------------------------------------------------------------------------
void Window1::SplitAndFitFile(){
	int start, stop, abs_maximum,end,step,storestart,storeend;
	step = inputting->round_split_spinbutton->get_value_as_int();
	start = inputting->startround_spinbutton->get_value_as_int();
	end = inputting->endround_spinbutton->get_value_as_int();
	storestart = start; storeend = end;
	if (start == 0)
		start = 1;
	stop = start + step - 1;
	if (end==0)
	{
		// Get it from the file. There might be wrong file so do not use the spinbutton upper range.
		abs_maximum = getNumberOfRoundsInFile(true);
	}
	else
		abs_maximum = end;
	
	// Let's warn the stupid user in case the last chunk will be less than the desired amount of rounds
	int last_chunk_size = abs_maximum - start + 1;
	if ( last_chunk_size % step != 0 )
	{
		string msg_text = "Last chunk will have only ";
		msg_text += to_string(last_chunk_size % step) + " rounds, ";
		msg_text += to_string(step) + " requested.\nContinue anyway?\n\n";
		msg_text += "You can change the range for rounds under \"single file load\" tab.";
		Gtk::MessageDialog ask_dialog(msg_text,false,Gtk::MESSAGE_QUESTION,Gtk::BUTTONS_YES_NO);
		ask_dialog.set_title("Achtung!");
		ask_dialog.set_default_response(Gtk::RESPONSE_NO);
		if (ask_dialog.run() == Gtk::RESPONSE_NO )
		{
			return;
		}
	}
	
	
	while (1) {
		if (stop > abs_maximum) {
			stop = abs_maximum;
		}
		inputting->startround_spinbutton->set_value(start);
		inputting->endround_spinbutton->set_value(stop);
		LoadDataFile();
		make_class_division();
		StartFitting();
		linear_fit_button_clicked();
		update_gtk();
		export_data_clicked();
		if (stop == abs_maximum)
			break;
		start+= step;
		stop+= step;
	}
	inputting->startround_spinbutton->set_value(storestart);
	inputting->endround_spinbutton->set_value(storeend);
	return;
}
//------------------------------------------------------------------------------
int Window1::batchProcess(void){
	if (inputting->batch_process_button->get_label() == "STOP") {
		stop_now=true;
		return -1;
	}
	else
		stop_now=false;
	//Checking, if output file exists:
	inputting->batch_progressbar->set_fraction(0);
	Glib::ustring outdir,element,gnuplotfile,dataoutfilename;
	int scan_number;
	scan_number = inputting->filenumber_spinbutton->get_value_as_int();
	outdir=getOutputDir();
	if (inputting->ref_radiobutton->get_active() ) {
		gnuplotfile = ref_gnuplot_outfile_entry->get_text();
		dataoutfilename = ref_outfile_entry->get_text();
		element = inputting->ref_element_entry->get_text();
	}
	if (inputting->meas_radiobutton->get_active() ) {
		gnuplotfile = meas_gnuplot_outfile_entry->get_text();
		dataoutfilename = meas_outfile_entry->get_text();
		element = inputting->meas_element_entry->get_text();
	}
	ifstream kill_file;
	kill_file.open( std::string(outdir+dataoutfilename).c_str() );
	while (1) {
		if (!kill_file) {
			kill_file.close();
			break;
		}
		else {
			int rows(0);string row;
			while (getline(kill_file,row)) {
				rows++;
			}
			kill_file.close();
			char rownumstr[100]; sprintf(rownumstr,"%d",rows);
			ok_apply_cancel_dialog.set_title("Warnink!");
			ok_apply_cancel_dialog.set_message( string(dataoutfilename + " already exists!\n (Rows in file " + rownumstr +")\n\n"
											+outputfile_warning(outdir,dataoutfilename,element,scan_number)	));
			int result = ok_apply_cancel_dialog.run();
			ok_apply_cancel_dialog.hide();
			if (result == Gtk::RESPONSE_OK) {
				cout << "deleted file\n";
				std::remove( std::string(outdir+dataoutfilename).c_str() );
				break;
			}
			if (result == Gtk::RESPONSE_APPLY) {
				break;
			}
			if (result == Gtk::RESPONSE_CANCEL) {
				return -1;
			}
		}
	}
	//End checking file existence
	double counter(0.0);
	//Saving autoload and autofit to temporary variables:
	bool autoLoad=instant_load->get_active();
	bool autoFit=fit_immediately->get_active();
	//Setting autoload+fit off for awhile
	instant_load->set_active(false);
	fit_immediately->set_active(false);

	//Setting STOP-button
	files_to_skip = get_files_to_skip();
	inputting->batch_process_button->set_label("STOP");
	double steps(0); // = (endfile_spinbutton->get_value() - startfile_spinbutton->get_value()+1.0);
	for (int i=inputting->startfile_spinbutton->get_value_as_int(); i <= inputting->endfile_spinbutton->get_value_as_int();
				i += inputting->batch_step_spinbutton->get_value_as_int() )
	{
		if ( !skip_this_file(i) ) {
			steps+=1.0;
		}
	}

	for (int i = inputting->startfile_spinbutton->get_value_as_int();i<= inputting->endfile_spinbutton->get_value_as_int();
				i += inputting->batch_step_spinbutton->get_value_as_int() )
	{
		if (stop_now) {
			inputting->batch_process_button->set_label("GO!");
			return -2;
		}
		if (skip_this_file(i) )
			continue;
		inputting->filenumber_spinbutton->set_value(i);
		inputting->batch_progressbar->set_text("Loading: " + asString(i)); update_gtk();
		LoadDataFile();
		make_class_division();
		inputting->batch_progressbar->set_text("Fitting: " + asString(i)); update_gtk();
		StartFitting();
		linear_fit_button_clicked();
		inputting->batch_progressbar->set_text("Exporting: " + asString(i)); update_gtk();

		instant_load->set_active(autoLoad);
		fit_immediately->set_active(autoFit);
		export_data_clicked();
		instant_load->set_active(false);
		fit_immediately->set_active(false);

//		batch_progressbar->set_se_step(1.0/(endfile_spinbutton->get_value() - startfile_spinbutton->get_value()+1.0) );
		counter+= 1.0;
		inputting->batch_progressbar->set_fraction(counter/steps);
		inputting->batch_progressbar->set_text("Done: " + asString(i)); update_gtk();
	}
	instant_load->set_active(autoLoad);
	fit_immediately->set_active(autoFit);
	inputting->batch_process_button->set_label("GO!");
	return 0;
}

//------------------------------------------------------------------------------
void Window1::ShowDebuggingStuff(bool yes_or_no){
	if (!yes_or_no){
		debug_label->hide();
		debug_level->hide();
		debugitus_button->hide();

	}
	using_debugging = yes_or_no;
}
//------------------------------------------------------------------------------
void Window1::move_windows_to_initial_positions(void){
	int lakritsi_x,lakritsi_y,small_x,small_y;
	int screen_width;
	Glib::RefPtr<Gdk::Screen> screeni = get_screen();
	screen_width = screeni->get_width();
	get_size(lakritsi_x,lakritsi_y);
	move(0,0);
	if (MS_windows == true) {
		small_x = (screen_width - lakritsi_x)/2-10;
		/*small_y = lakritsi_y/3-10;
		tof_vs_class			->resize(small_x,small_y);
		cts_vs_class			->resize(small_x,small_y);
		data_histogram			->resize(small_x,small_y);
		fitted_class_results	->resize(small_x,small_y);
		event_histogram			->resize(small_x,small_y);

		tof_vs_class        	->move(lakritsi_x+10,30);
		fitted_class_results	->move(lakritsi_x+10+small_x,30);
		data_histogram		->move(lakritsi_x+10,small_y+60);
		event_histogram		->move(lakritsi_x+10+small_x,small_y+60);
		cts_vs_class		->move(lakritsi_x+10,small_y*2+90);
		*/
		return;
	}

	small_x = (screen_width - lakritsi_x)/2-25;
	small_y = lakritsi_y/3-25;
	/*tof_vs_class			->resize(small_x,small_y);
	cts_vs_class			->resize(small_x,small_y);
	data_histogram			->resize(small_x,small_y);
	fitted_class_results	->resize(small_x,small_y);
	event_histogram			->resize(small_x,small_y);

	tof_vs_class        	->move(lakritsi_x-50,0);
	fitted_class_results	->move(lakritsi_x+small_x-40,0);
	data_histogram			->move(lakritsi_x-50,small_y+60);
	event_histogram			->move(lakritsi_x+small_x-40,small_y+60);
	cts_vs_class			->move(lakritsi_x-50,small_y*2+100);
	*/
	return;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Window1::bin_or_cts_win_value_changed() {
	LOCK
	set_tof_plot_range_from_bin_gate_if_allowed();
	validateWindowingSpinbuttons();
	data->setAllDataNotFitted();
	data->setBinAndCountWindowManually(
			get_bin_min(),
			get_bin_max(),
			get_cts_min(),
			get_cts_max() );
	if (how_many_classes_spinbutton->get_value_as_int() == 1) {
		data->setLowerEventBoundary(0,get_cts_min());
		data->setUpperEventBoundary(0,get_cts_max());

	}
	////////Round filter
	data->set_all_bunches_active();
	//Applying frequency window
	data->set_frequency_range(get_freq_min(), get_freq_max() );
	//Applying round filter
	data->set_bunches_that_will_be_skipped(get_rounds_to_skip());
	/////////
	class_division_result_entry->set_text( data->getClassDivResult() );
	class_div_ok_entry->set_text(data->wasClassDivOk());
	validateSpinButtons();
	fitted_class_results->nullify();
	showFittedClassResults();
	extrapolate_result_entry->set_text("");
	extrapolate_result_err_entry->set_text("");
	data->setAllDataNotFitted();
	showFitResults();

	#ifndef HAVE_ONLINE
	offline_histogram_need_update = true;
	#endif
	#ifdef HAVE_ONLINE
	if (!is_in_constructor)
		online_histogrammer.set_bin_gate(get_bin_min(),get_bin_max() );
	#endif
	make_event_histogram_bars();
	get_and_show_2d_contour_of_cts();
	show_single_class_histogram();
	redraw_all_small_windows();
	RELEASE

}
//------------------------------------------------------------------------------
void Window1::on_primary_changed(){

}
//------------------------------------------------------------------------------
void Window1::on_gaussian_fit_range_changed(const double & x1, const double& y1, const double& x2, const double& y2, const Glib::ustring&)
{
	
	// Left_fit_range has no limitations so it is safe to change that.
	left_fit_range_spinbutton->set_value(x1);
	right_fit_range_spinbutton->set_value(x2);
	
}

void Window1::on_gaussfit_range_changed()
{
	LOCK
	double x[2]; x[0]=left_fit_range_spinbutton->get_value(); x[1]=right_fit_range_spinbutton->get_value();
	right_fit_range_spinbutton->set_range(x[0],data->getMaximumFrequency());
	right_fit_range_spinbutton->set_value(x[1]); x[1]=right_fit_range_spinbutton->get_value();
	cts_vs_class->show_xbars(true);
	//cts_vs_class->add_xbars(x,2);
	//NYTH
	
	// Change the bars and create new if not existing
	if (!cts_vs_class->xbars.replace_value_in_bar(x[0],"lefti")) {
		cts_vs_class->xbars.add_bar(BAR(x[0],Gdk::Color("Blue"),"","lefti"));
	}
	if (!cts_vs_class->xbars.replace_value_in_bar(x[1],"righti")) {
		cts_vs_class->xbars.add_bar(BAR(x[0],Gdk::Color("Blue"),"","righti"));
	}

	//cts_vs_class->show_contents();
	cts_vs_class_need_update=true;
	RELEASE
}
//------------------------------------------------------------------------------
void Window1::on_fit_gaussian_button_clicked()
{
	LOCK
	DATASET daatta;
	double a,b,c,d,e,f,g,h,i;
	if (g_autofreq->get_active() )
		putDoubleToEntry(*gaussian_freq_entry,left_fit_range_spinbutton->get_value()/2.0 + right_fit_range_spinbutton->get_value() /2.0,9);
	data->FitGaussian(left_fit_range_spinbutton->get_value(), right_fit_range_spinbutton->get_value(),
				showclass_spinbutton->get_value_as_int(),
				getDoubleFromEntry(*gaussian_freq_entry), getDoubleFromEntry(*gaussian_fwhm_entry),
				getDoubleFromEntry(*gaussian_height_entry), getDoubleFromEntry(*gaussian_bgr_entry),
				cb0->get_active(),cb1->get_active(),cb2->get_active(),cb3->get_active(),
				a,b,c,d,
				e,f,g,h,
				i,daatta,100);
	putDoubleToEntry(*g_freq,a);putDoubleToEntry(*g_fwhm,b);putDoubleToEntry(*g_height,c);putDoubleToEntry(*g_bgr,d);
	putDoubleToEntry(*g_freq_err,e);putDoubleToEntry(*g_fwhm_err,f);putDoubleToEntry(*g_height_err,g);putDoubleToEntry(*g_bgr_err,h);
	putDoubleToEntry(*g_chisq,i);
	cts_vs_class->input_fit(daatta,false);
	cts_vs_class->show_fit(true);
//	cts_vs_class->show_contents();
	cts_vs_class_need_update=true;
	RELEASE
}
//------------------------------------------------------------------------------
void Window1::on_meas_time_change_entry_clicked()
{
	data->setAverageTime(entry67->get_text() );
	time_in_use->set_text(data->getAverageTime());
}
//------------------------------------------------------------------------------
int Window1::redraw_windows_if_necessary(bool herelock) {
	if (!herelock) {
//		cout << "not done!\n";
		return -1;
	}
	int updates(0);
	if (tof_vs_class_need_update) {
		tof_vs_class->show_contents();
		tof_vs_class_need_update=false;
//		cout << "tof_vs_class redrawn\n";
		updates++;
	}
	if (cts_vs_class_need_update) {
		cts_vs_class->show_contents();
		cts_vs_class_need_update=false;
//		cout << "cts_vs_class redrawn\n";
		updates++;
	}
	if (event_histogram_need_update) {
		event_histogram->show_contents();
		event_histogram_need_update=false;
//		cout << "event_histogram redrawn\n";
		updates++;
	}
	if (data_histogram_need_update) {
		data_histogram->show_contents();
		data_histogram_need_update=false;
//		cout << "data_histogram redrawn\n";
		updates++;
	}
	if (fitted_class_results_need_update) {
		fitted_class_results->show_contents();
		fitted_class_results_need_update=false;
//		cout << "fitted_class_results redrawn\n";
		updates++;
	}
	#ifndef HAVE_ONLINE
	if (offline_histogram_need_update) {
		fitted_class_results->show_contents();
		fitted_class_results_need_update=false;
//		cout << "fitted_class_results redrawn\n";
		updates++;
	}
	#endif
	locked=false;
	return updates;
}
//------------------------------------------------------------------------------
Glib::ustring Window1::outputfile_warning(const Glib::ustring &outdir, const Glib::ustring &dataoutfilename,
									const Glib::ustring &element, int scan_number)
{
	Glib::ustring errorstring,elementti=element.substr(0,element.find_first_of("_"));
	ifstream filex;
	filex.open( string(outdir+dataoutfilename).c_str());
	std::string row,piece;
	if (!filex) {
		filex.close();
		return "";
	}
	else {
		while (getline(filex,row)) {
			if (row[0] == '#')
				continue;
			if (row == "")
				continue;
			//Getting element name from data file
			piece = erota(row,'\t'); piece = piece.substr(0,piece.find_first_of("_"));
			//cout << "piece:" << piece << "; elem:"<< elementti << "\n";
			if (piece != elementti) {
				errorstring += 	"File contains \"" + piece +
								"\",\nbut you're outputting \"" + elementti + "\"!\n";
			}
			//Getting number of classes
			piece = erota(row,'\t'); //scan number, not wanted
			piece = erota(row,'\t'); //total number of classes
			if ( piece != how_many_classes_spinbutton->get_text() ) {
				errorstring += 	"File seem to have " + piece + " class(es),\n" +
								"but you have used " + how_many_classes_spinbutton->get_text() + " class(es)!\n";
			}
			break;
		}

	}

	//return Glib::ustring(piece +"\n"+element);
	filex.close();
	return errorstring;
}
//------------------------------------------------------------------------------
bool Window1::is_class_fitted()
{
    return (0 != data->isClassFitted(showclass_spinbutton->get_value_as_int()));
}
//------------------------------------------------------------------------------
void Window1::set_colors() {
	inactive_files.set("LightGrey");
	error.set("red");
	ookke.set("limegreen");
	default_widget_base_color = get_st_normal_bg_color(*inputting->inputdir_entry);
//	st_normal_bg_color(*class_division_result_entry, inactive_files);
//	st_normal_bg_color(*class_div_ok_entry, ookke);
}
//------------------------------------------------------------------------------
void Window1::set_was_class_div_ok_text(void) {
	class_div_ok_entry->set_text(data->wasClassDivOk());
	if (data->wasClassDivOk() == "Ok.")
		st_normal_bg_color(*class_div_ok_entry, ookke);
	else
		st_normal_bg_color(*class_div_ok_entry, error);

	return;
}
//------------------------------------------------------------------------------
void Window1::set_number_of_rounds_in_file(int rounds) {

	//putIntegerToEntry(*inputting->rounds_show_entry,rounds );
	bool singles = data->is_data_collected_in_single_bunch_mode();
	int number_of_bunches_in_one_spectrum = data->getNumberOfBunchesInOneSpectrum();
	string text = asString(number_of_bunches_in_one_spectrum) + "x" + asString(rounds);
	// std::cout << "rounds: " << rounds << "\n";
	if (singles)
		text.append("s");
	inputting->rounds_show_entry->set_text(text);
	if (rounds < 0) {
	    	st_normal_bg_color(*inputting->rounds_show_entry, error);
	}
	else
	    	st_normal_bg_color(*inputting->rounds_show_entry, ookke);
}
//------------------------------------------------------------------------------
void Window1::set_active_file_colors() {
    Gdk::RGBA meas_color, ref_color;

    if (inputting->ref_radiobutton->get_active()) {
	meas_color = inactive_files;
	ref_color = default_widget_base_color;
    } else {
	meas_color = default_widget_base_color;
	ref_color = inactive_files;
    }

    st_normal_bg_color(*meas_outfile_entry, meas_color);
    st_normal_bg_color(*meas_gnuplot_outfile_entry, meas_color);
    st_normal_bg_color(*meas_bin_min_spinbutton, meas_color);
    st_normal_bg_color(*meas_bin_max_spinbutton, meas_color);
    st_normal_bg_color(*meas_cts_min_spinbutton, meas_color);
    st_normal_bg_color(*meas_cts_max_spinbutton, meas_color);
    st_normal_bg_color(*meas_freq_min_spinbutton, meas_color);
    st_normal_bg_color(*meas_freq_max_spinbutton, meas_color);

    st_normal_bg_color(*inputting->ref_element_entry, ref_color);
    st_normal_bg_color(*ref_outfile_entry, ref_color);
    st_normal_bg_color(*ref_gnuplot_outfile_entry, ref_color);
    st_normal_bg_color(*ref_bin_min_spinbutton, ref_color);
    st_normal_bg_color(*ref_bin_max_spinbutton, ref_color);
    st_normal_bg_color(*ref_cts_min_spinbutton, ref_color);
    st_normal_bg_color(*ref_cts_max_spinbutton, ref_color);
    st_normal_bg_color(*ref_freq_min_spinbutton, ref_color);
    st_normal_bg_color(*ref_freq_max_spinbutton, ref_color);

    st_normal_bg_color(*ref_sn_base_from_sp, ref_color);
    st_normal_bg_color(*ref_sn_base_to_sp, ref_color);
    st_normal_bg_color(*ref_sn_reso_from_sp, ref_color);
    st_normal_bg_color(*ref_sn_reso_to_sp, ref_color);
    st_normal_bg_color(*meas_sn_base_from_sp, meas_color);
    st_normal_bg_color(*meas_sn_base_to_sp, meas_color);
    st_normal_bg_color(*meas_sn_reso_from_sp, meas_color);
    st_normal_bg_color(*meas_sn_reso_to_sp, meas_color);
}
//------------------------------------------------------------------------------
void Window1::initialize_child_windows(void) {
	tof_vs_class = &a;
	cts_vs_class = &b;
	data_histogram = &c;
	event_histogram = &d;
	fitted_class_results = &e;

	fitted_class_results->set_plot_title("FREQ vs CLASS");
	data_histogram->set_plot_title("COUNTS vs TIME-OF-FLIGHT");
	event_histogram->set_plot_title("# vs IONS/BUNCH");
	cts_vs_class->set_plot_title("CTS vs CLASS");
	tof_vs_class->set_plot_title("TOF vs CLASS");

	tof_vs_class->allow_user_to_toggle_log_scale(false);

	#ifdef HAVE_ONLINE
	five_plotareas_window = new ViisiPlottiaWindow(&a,&b,&c,&d,&e,"ONLINE Lakritsi plots");
	#endif
	#ifndef HAVE_ONLINE
	five_plotareas_window = new ViisiPlottiaWindow(&a,&b,&c,&d,&e,"Lakritsi plots");
	#endif
	five_plotareas_window->show();

	//NYTH
	update_events_menues_in_cts_vs_bunches_window();
	// cts_vs_class->add_range(left_fit_range_spinbutton,right_fit_range_spinbutton, "gaussian fit range");
	
	// Add left-click-drag selection for gaussian fit range
	cts_vs_class->add_range("gaussian fit range");
	cts_vs_class->signal_range_selected.connect(sigc::mem_fun(*this, &Window1::on_gaussian_fit_range_changed));

	// Range selection with a mouse in tof-vs-class window
	tof_vs_class->signal_range_selected.connect(sigc::mem_fun(*this, &Window1::on_tof_vs_class_range_changed));
	// Click selections with a left mouse button
	tof_vs_class->signal_point_selected.connect(sigc::mem_fun(*this, &Window1::on_tof_vs_class_singleclick));
	
	// Range selection with a mouse in event histogram
	event_histogram->signal_range_selected.connect(sigc::mem_fun(*this, &Window1::on_count_histogram_range_changed));
	event_histogram->show_xbars(true);
	
	// Range selection with a mouse in data histogram (tof bin)
	data_histogram->signal_range_selected.connect(sigc::mem_fun(*this, &Window1::on_tof_histogram_range_changed));

	cts_vs_class_need_update=0;
	tof_vs_class_need_update=0;
	data_histogram_need_update=0;
	event_histogram_need_update=0;
	fitted_class_results_need_update=0;


	event_histogram->set_ymin_with_force();
	event_histogram->set_xmin_with_force();
	data_histogram->set_ymin_with_force();
	data_histogram->set_xmin_with_force();
	#ifndef HAVE_ONLINE
	offline_histogram = &f;

	offline_histogram->set_plot_title("History / 5 bunches");
	offline_histogram->set_ymin_with_force(0.0);

	yksi_plotarea_window = new YksiPlottiWindow(&f,"History / 5 bunches");
	yksi_plotarea_window->show();

	offline_histogram_need_update=0;
	#endif
	#ifdef HAVE_ONLINE
	ions_per_x_bunches_window = &f;
// 	bunches_per_class_window = &g;

	ions_per_x_bunches_window->set_plot_title("ONLINE history / 5 bunches");
// 	bunches_per_class_window->set_plot_title("ONLINE Bunches per class (of 20 bunches) ");

	yksi_plotarea_window = new YksiPlottiWindow(&f,"ONLINE History / 5 bunches");
// 	toinen_yksi_plotarea_window = new YksiPlottiWindow(&g,"ONLINE Bunches per class (of 20 bunches)");
	set_window_labels_with_meas_id();

	yksi_plotarea_window->show();
// 	toinen_yksi_plotarea_window->show();


	class_histogrammer.set_merging(20);

	#endif

}
//------------------------------------------------------------------------------
void Window1::set_background_colors() {
	double alphaa = background_alpha;
	Gdk::Color vari = background_color_for_windows;

	tof_vs_class->set_background_color(vari,alphaa);
	cts_vs_class->set_background_color(vari,alphaa);
	fitted_class_results->set_background_color(vari,alphaa);
	event_histogram->set_background_color(vari,alphaa);
	data_histogram->set_background_color(vari,alphaa);

	#ifndef HAVE_ONLINE
	offline_histogram->set_background_color(vari,alphaa);
	#endif
	#ifdef HAVE_ONLINE
	//inputting->meas_id_other_entry->
	st_normal_bg_color(*inputting->meas_id_other_entry, vari);
	ions_per_x_bunches_window->set_background_color(vari,alphaa);
// 	bunches_per_class_window->set_background_color(vari,alphaa);
	#endif
}
//------------------------------------------------------------------------------
void Window1::on_debugitus_button_clicked() {

	//ShowDataHistogram();
	//#ifndef HAVE_ONLINE
	//show_offline_histogram();
	//#endif
	//data->set_bunches_that_will_be_skipped(get_rounds_to_skip());
	//tof_vs_class->dump_cairostuff_to_psfile("blaa.pdf",600,400);

	//inputting->swap_ref_meas_radiobuttons();
	//get_files_to_skip();
	//vector<CTS_HISTOGRAM_OF_FREQ> testi;
	//testi = data->get_2d_contour_of_counts(0,data->getMinimumFrequency()-100.,data->getMaximumFrequency()+100.,bin_min_spinbutton->get_value_as_int(),bin_max_spinbutton->get_value_as_int(),bin_interval_in_2d_contour);
/*	for (vector<CTS_HISTOGRAM_OF_FREQ>::iterator blaa = testi.begin(); blaa != testi.end(); ++blaa) {
		int sum(0);
		cout << "Center: " << blaa->center_freq << "\n";
		cout << "Half left, half right: " << blaa->half_distance_to_left_freq << ", " << blaa->half_distance_to_right_freq << "\n";
		for (vector<int>::iterator blaa2 = blaa->counts_in_interval.begin(); blaa2 != blaa->counts_in_interval.end(); ++blaa2) {
			cout << *blaa2 << "\n";
			sum += *blaa2;
		}
		cout << "Total: " << sum << "\n";
	}*/
	//tof_vs_class->input_cts_contour(testi);

	/*set_sensitive(false);
	update_gtk();
	set_sensitive(true);

	debugitus_button->set_sensitive();*/
	data->printAllOut(cout,debug_level->get_value_as_int());
}
//------------------------------------------------------------------------------
void Window1::initialize_dialogs() {

	outputdir_dialog = new Gtk::FileChooserDialog("Please choose output directory", Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
	outputdir_dialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	outputdir_dialog->add_button("Select", Gtk::RESPONSE_OK);
	outputdir_dialog->set_default_response(Gtk::RESPONSE_OK);

	save_windows_content_dialog = ok_cancel_dialog_2::create_widget();
	save_windows_content_dialog->hide();
	save_windows_content_dialog->set_response_sensitive(Gtk::RESPONSE_OK);
	save_windows_content_dialog->set_default_response(Gtk::RESPONSE_OK);
//	outputdir_dialog->set_transient_for(*this);
}
//------------------------------------------------------------------------------
void Window1::testia() {
//	Gtk::FileChooser *testib;
//	testib = new Gtk:FileChooser;
	fitted_class_results->nullify();
}
//------------------------------------------------------------------------------
void Window1::on_save_data_of_each_window1_activate() {
	save_windows_content_dialog->set_output_dir(getOutputDir());

	int result = save_windows_content_dialog->run();
	save_windows_content_dialog->hide();
	if (result == Gtk::RESPONSE_OK) {
		tof_vs_class->save_data(save_windows_content_dialog->get_tof_data_filename());
		tof_vs_class->save_fit(save_windows_content_dialog->get_tof_fit_filename());

		cts_vs_class->save_data(save_windows_content_dialog->get_cts_data_filename());
		cts_vs_class->save_fit(save_windows_content_dialog->get_cts_fit_filename());

		fitted_class_results->save_data(save_windows_content_dialog->get_freq_as_function_of_number_of_ions_per_bunch_filename());
		data_histogram->save_data(save_windows_content_dialog->get_counts_as_function_of_tof_filename());
		event_histogram->save_data(save_windows_content_dialog->get_bunches_as_function_of_ions_per_bunch_filename());
	}
}
//------------------------------------------------------------------------------
void Window1::on_check_for_new_version_at_startup_activate() {

}

vector<int> Window1::get_files_to_skip()
{
    return get_skips(*(inputting->skips_entry));
}
vector<int> Window1::get_rounds_to_skip()
{
    return get_skips(*(inputting->round_skips_entry));
}
//------------------------------------------------------------------------------
bool  Window1::skip_this_file(int this_one) {
	if ( !files_to_skip.empty() ) {
		for (vector<int>::iterator i=files_to_skip.begin(); i !=files_to_skip.end(); ++i) {
			if (this_one == *i)
				return true;
		}
	}
	return false;
}
//------------------------------------------------------------------------------
void Window1::on_trap1_helper_toggled() {
	main_page->hide();
	tickerer->hide();
	if (tickerer_chooser->get_active() ) {
		resize(2,2);
		tickerer->show();
	}
	else {
		main_page->show();
		tickerer->hide();

		on_windows_on_top_activate();
		resize(2,2);
	}
}
//------------------------------------------------------------------------------
void Window1::on_show_partly_toggled() {
	if (show_fitting_things_cb->get_active() )
		tweaks_part->show();
	else
		tweaks_part->hide();
	if (!show_datastuff_cb->get_active() ) {
		class_division_box->hide();
		gates_table->hide();
	}
	else {
		class_division_box->show();
		gates_table->show();
	}
	resize(1,1);

}
//------------------------------------------------------------------------------
void Window1::on_choose_background_color_activate() {
	Gtk::ColorSelectionDialog colorselektor("Please choose background color..");
	colorselektor.get_color_selection()->set_current_color(background_color_for_windows);
	colorselektor.get_color_selection()->set_current_alpha( static_cast<int>(background_alpha*65535));
	int result = colorselektor.run();
	if (result  == Gtk::RESPONSE_OK ) {
		background_color_for_windows = colorselektor.get_color_selection()->get_current_color();
		background_alpha = (colorselektor.get_color_selection()->get_current_alpha()*1.)/65535.;
		set_background_colors();
	}
}
//------------------------------------------------------------------------------
void Window1::on_choose_2d_plotcolor_activate() {
	Gtk::ColorSelectionDialog colorselektor("Please choose contour color..");
	colorselektor.get_color_selection()->set_current_color(tof_vs_class->get_contour_color());
	int result = colorselektor.run();
	if (result  == Gtk::RESPONSE_OK ) {
		tof_vs_class->set_contour_color(colorselektor.get_color_selection()->get_current_color());

	}
}
//------------------------------------------------------------------------------
void Window1::on_choose_bin_interval_in_2d_plot_activate() {
	LOCK

	Gtk::MessageDialog ask_dialog("Choose bin interval\nfor 2d-contour",false,
								  Gtk::MESSAGE_QUESTION,Gtk::BUTTONS_OK_CANCEL);
	ask_dialog.set_title("Hey!");
	// message_dialog.set_message("Choose bin interval\nfor 2d-contour");
	Gtk::SpinButton nummero;
	nummero.set_range(1,100);
	nummero.set_increments(1,10);
	nummero.set_value(bin_interval_in_2d_contour);
	nummero.show();
	//dialoki->get_vbox()->pack_start(nummero, Gtk::PACK_SHRINK);
	ask_dialog.get_message_area()->pack_start(nummero,false,false);
	if (ask_dialog.run() == Gtk::RESPONSE_OK ) {
		set_2d_contour_tof_interval(nummero.get_value_as_int());
		//cout << "nummero: " <<nummero.get_value_as_int() << "\n";
	}
	RELEASE
}
//------------------------------------------------------------------------------
void Window1::set_2d_contour_tof_interval(int binss) {
	bin_interval_in_2d_contour = binss;
	data->set2d_contour_tof_interval(binss);
	get_and_show_2d_contour_of_cts();
}
//------------------------------------------------------------------------------
void Window1::get_and_show_2d_contour_of_cts(void) {
	LOCK
	vector<CTS_HISTOGRAM_OF_FREQ> * testi;
	testi = data->get_2d_contour_of_counts(showclass_spinbutton->get_value_as_int(),data->getMinimumFrequency()-100.,data->getMaximumFrequency()+100.,get_bin_min(),get_bin_max(),bin_interval_in_2d_contour);
	tof_vs_class->input_cts_contour(*testi);
	tof_vs_class_need_update = true;
	RELEASE
}
//------------------------------------------------------------------------------
#ifdef HAVE_ONLINE
//When starting new scan
bool Window1::online_info(const Measurement_Info &info) {
	data->set_MCS_data(info.bins, info.bin_width);
	loc_tm time_now(0);
	inputting->online_starting_status->set_text("Started: " + time_now.get_just_time_as_string());
			//+ "  Bins: " + asString(info.bins) + ", Width: " + as_2f(data->getBin_width()));

	return true;
}
//------------------------------------------------------------------------------
void Window1::online_ended(void) {
	//When clear_all pressed in mass scan
	if (inputting->delete_old_online_checkbutton->get_active() ) {
		DeleteData();
	}
	loc_tm time_now(0);
	inputting->online_starting_status->set_text("Stopped: " + time_now.get_just_time_as_string());
	//online_scan_progressbar->set_fraction(0);
// 	inputting->swap_ref_meas_radiobuttons();
	cout << "STOPPED\n\n";
}
//------------------------------------------------------------------------------
void Window1::online_stopped(void) {
	loc_tm time_now(0);
	inputting->online_starting_status->set_text("Paused: " + time_now.get_just_time_as_string()+ " Want more?");
}
//------------------------------------------------------------------------------
bool Window1::online_bunchpack(const std::list<Bunch_Data> &data) {
	int bunches_inserted = 0;
	for (std::list<Bunch_Data>::const_iterator foo = data.begin(); foo != data.end(); ++foo) {
			waiting_bunches.push_back(*foo);
	}
	if (!suspend_online_updating && !(waiting_bunches.empty() ) ) {
		bunches_inserted = this->data->insert_online_bunchpack(waiting_bunches);
		online_bunches.splice(online_bunches.end(), waiting_bunches);
	}
	if (suspend_online_updating)
		return false;
	online_histogrammer.set_bin_gate(get_bin_min(),get_bin_max());
	online_histogrammer.update_ions_per_x_bunches();

// 	class_histogrammer.update_bunches_per_class();

	freq_window_changed();

	loc_tm time_now(0);

	inputting->online_status->set_text("At " + time_now.get_just_time_as_string()+":  "+ asString(bunches_inserted) + " bunches came");
	double round_number = this->data->getTotalBunchNumber()/(this->data->getFrequencyNumber()*1.0);
	//inputting->online_rounds_etc_status->set_text("Freqs: " + asString(this->data->getFrequencyNumber()) +
	//						", bunches: " + asString(this->data->getTotalBunchNumber()) +
	//						", bunches/freq: " + as_2f(round_number));
	inputting->online_rounds_etc_status->set_text("Bunches accepted: " + asString(this->data->getTotalBunchNumber()));
	int osa = floor(100.0*(round_number - floor(round_number)));
	char output[256]; sprintf(output,"%02d",osa);
	inputting->round_labeli->set_markup("<span font=\"30\">"+as_0f(floor(round_number))+"</span>"+"."+output);

	if (round_number < 1.000011) {
		inputting->online_progressbar->pulse();
	}
	else {
		inputting->online_progressbar->set_fraction(round_number-floor(round_number));
	}


	if (inputting->online_autofit->get_active() ) {
		//mainwin->make_class_division(); Already commandered in on_freq_window_changed()
		StartFitting();
		linear_fit_button_clicked();
	}
	//update_online_scan_progress();
	return true;
}

#endif
//------------------------------------------------------------------------------
void Window1::on_load_window_positions_activate() {
	loadWindowPositions();
	update_gtk();
	loadWindowPositions();
}
//------------------------------------------------------------------------------
void Window1::on_save_window_positions_activate() {
	saveWindowPositions();
}
//------------------------------------------------------------------------------
void Window1::show_single_class_histogram() {
	LOCK
	
	tof_histogram_of_class_data = data->get_histogram_of_class(showclass_spinbutton->get_value_as_int());
	//std::cout << "size of class histo: " << tof_histogram_data_of_class->size() << "\n";
	tof_histogram_of_class_container.set_data(tof_histogram_of_class_data);
	tof_histogram_of_class_container.graph_type = FANCY_HISTOGRAM;
	tof_histogram_of_class_container.color = Gdk::Color("Black");
	tof_histogram_of_class_container.color_alpha = 0.3;
	tof_histogram_of_class_container.data_name = "This class";
	// std::cout << "histogram_compression_factor " << histogram_compression_factor << "\n";
	tof_histogram_of_class_container.set_data_compression_factor(histogram_compression_factor);
	data_histogram->add_data(&tof_histogram_of_class_container);
	
	
	data_histogram->show_data();
	data_histogram->autosetPlottingBox();
	data_histogram_need_update = true;	
	
	/*
	//2007-08-05 data histogram of ions in one class
	DATASET single_class_histogrammi; //DATA pointti;
	vector<int> single_class_histogram;
	data->get_cts_vs_bin_histogram_of_class(single_class_histogram,showclass_spinbutton->get_value_as_int());

	single_class_histogrammi.graph_type = FANCY_HISTOGRAM;
	single_class_histogrammi.color = Gdk::Color("Black");
	single_class_histogrammi.color_alpha=0.01;
	single_class_histogrammi.data_name = "This class";
	single_class_histogrammi.legend_position = TOP_LEFT;

	convert_histogram_and_compress(single_class_histogram, single_class_histogrammi, histogram_compression_factor);

	//data_histogram->input_data2(single_class_histogrammi);
	data_histogram->input_data(single_class_histogrammi);
	data_histogram->autosetPlottingBox();
	data_histogram_need_update = true;*/
	RELEASE
}
//------------------------------------------------------------------------------
void Window1::on_statistics_toggled() {
	LOCK
	data->stat_mode = get_statistics_mode();
	bin_or_cts_win_value_changed();
	if (fit_immediately->get_active())
		StartFitting();
	RELEASE
}
//------------------------------------------------------------------------------
STATISTICS_MODE Window1::get_statistics_mode() {
	if (individual_statistics->get_active() )
		return INDIVIDUAL_STATISTICS;
	if (sum_statistics->get_active() )
		return SUM_STATISTICS;
	if (sum_statistics_2->get_active() )
		return SUM_STATISTICS2;
	return INDIVIDUAL_STATISTICS;
}
//------------------------------------------------------------------------------
vector<int> Window1::get_skips(Gtk::Entry &entri) {
	vector<int> returnee;
	std::string line = entri.get_text(); std::string line2;
	vector<std::string> parts;
	unsigned int comma;
	//Get the parts separated by comma
	while (1)	{
		comma = line.find_first_of(",");
		if (line != "")
			parts.push_back( line.substr(0,comma) );
		line2 = line.substr(comma+1);
//		cout << "part:" << parts.back() << ", line:" << line << "\n";
		if (line.find_first_of(",") == string::npos) {
//			cout << "Loppu\n";
			break;
		}
		line = line2;
	}
	//Check whether there is dashes in there
	if ( !parts.empty() ) {
		for (vector<string>::iterator i=parts.begin(); i !=parts.end(); ++i) {
			comma = i->find_first_of("-");
			//If range, like 14-17
			if (i->find_first_of("-") != string::npos) {
				int starter = as_int(i->substr(0,comma));
				int ender = as_int(i->substr(comma+1));
				cout << "Start, end:" << starter << " " << ender << "\n";
				if (ender >= starter) {
					for (int j = starter; j <= ender; j++) {
//						cout << "Pushattiin range\n";
						returnee.push_back(j);
					}
				}
			}
			//If just single number:
			else {
//				cout << "Pushattiin single" << *i << " \n";
				returnee.push_back(as_int(*i));
			}
		}
	}
/*	if ( !returnee.empty() ) {
		cout << "skipping these : ";
		for (vector<int>::iterator i=returnee.begin(); i !=returnee.end(); ++i) {
				cout << " " << *i;
		}
		cout << "\n";
	}*/

	return returnee;
}
//------------------------------------------------------------------------------
#ifndef HAVE_ONLINE
void Window1::show_offline_histogram(void)
{
	round_change_points.clear();
	history_histogram_with_gates_data = data->get_history_of_counts(
		get_bin_min(),get_bin_max(),get_cts_min(), get_cts_max(), 5,round_change_points);
	history_histogram_with_gates_plot_container.set_data(history_histogram_with_gates_data);
	history_histogram_with_gates_plot_container.graph_type = FANCY_HISTOGRAM;
	history_histogram_with_gates_plot_container.color = Gdk::Color("Blue");
	history_histogram_with_gates_plot_container.color_alpha = 0.2;
	history_histogram_with_gates_plot_container.data_name = "TOF projection";
	offline_histogram->add_data(&history_histogram_with_gates_plot_container);

	history_histogram_data = data->get_history_of_counts(0,0,0,0,5,round_change_points);
	history_histogram_plot_container.set_data(history_histogram_with_gates_data);
	history_histogram_plot_container.graph_type = FANCY_HISTOGRAM;
	history_histogram_plot_container.color = Gdk::Color("Red");
	history_histogram_plot_container.color_alpha = 0.3;
	history_histogram_plot_container.data_name = "TOF projection";
	offline_histogram->add_data(&history_histogram_plot_container);

	/*
	
	std::vector<int> gated_historia,all_historia,round_change_points;
	data->form_a_history_of_counts(get_bin_min(),get_bin_max(),get_cts_min(), get_cts_max(), gated_historia,5,round_change_points);
	data->form_a_history_of_counts(0,0,0,0,all_historia,5,round_change_points);
	offline_histogram->input_bar_data(all_historia);
	offline_histogram->input_bar_data2(gated_historia);
	offline_histogram->set_graph_style(FANCY_HISTOGRAM);
	offline_histogram->set_graph_style2(FANCY_HISTOGRAM);
	offline_histogram->dat.set_color_alpha(0.3); offline_histogram->dat.color=Gdk::Color("Red");
	offline_histogram->dat2.set_color_alpha(0.2); offline_histogram->dat2.color=Gdk::Color("Blue");
	//offline_histogram->xbars.delete_bars(); //BAARS
	offline_histogram->xbars.delete_bar("rounds");
	*/
	offline_histogram->xbars.delete_bar("rounds");
	int round_counter(1);
	for (vector<int>::iterator i = round_change_points.begin(); i != round_change_points.end(); ++i) {
		//NYTH

		offline_histogram->xbars.add_bar(BAR(*i,Gdk::Color("Black"),asString(round_counter), "rounds"));
		//offline_histogram->add_xbar(*i,Gdk::Color("Black"),asString(round_counter));
		round_counter++;
	}
	offline_histogram->autosetPlottingBox(); offline_histogram->show_xbars(true);
	offline_histogram->show_contents();
}
#endif
//------------------------------------------------------------------------------
void Window1::on_gating_changed(void) {
	freq_window_changed();
}
//------------------------------------------------------------------------------
void Window1::save_rounds_to_skip() {
	string fnaame;
//	if (inputting->twod_use_button->get_active())
//		fnaame = inputting->get_full_inputdata_dir()+get_2d_filename("/skips","",inputting->filenumber_spinbutton->get_value_as_int());
//	else
	fnaame = inputting->get_full_inputdata_dir().string()+"/skips";
	ofstream settingsfile;
	settingsfile.open( fnaame.c_str());
	if (!settingsfile) {
	  Gtk::MessageDialog warning_dialog("Rounds skip file\n"+fnaame +"\ncant be written!");
	  warning_dialog.set_title("Argh!");
	  warning_dialog.run();
	  warning_dialog.hide();
	  return;
	}
	char DL = '\t';
	loc_tm current_time(0);
	settingsfile.precision(10);
	settingsfile << "#saved at " << loc_date(current_time,loc_tm::d_oma) << " " << loc_time(current_time,loc_tm::t_International) << "\n";
	settingsfile << "ROUNDS_TO_SKIP"	<< DL << inputting->round_skips_entry->get_text() << "\n";
	settingsfile.close();
}
//------------------------------------------------------------------------------
bool Window1::load_rounds_to_skip(const string &skipfilename) {
	if (loadIniFile(skipfilename) == -1) {
		cout << "No individual rounds to skip file found.\n";
		inputting->round_skips_entry->set_text("");
		return 0;
	}
	return 1;
}
//------------------------------------------------------------------------------
void Window1::warn_about_frequencies_having_different_amount_of_bunches() {
	if (data->does_all_freqs_have_same_amount_of_bunches() )
		return;

	Gtk::MessageDialog warn_dialog("It appears that not all frequencies have same amount of bunches!\n\n- BEWARE: if round selection is used,\n results will be erroneus!");
	warn_dialog.set_title("Warnink!");
	warn_dialog.run();
	warn_dialog.hide();

}
//------------------------------------------------------------------------------
void Window1::split_many_files() {
	Gtk::MessageDialog blaa("Not implemented yet! Use via settingfile to process many!");
	blaa.run();
	blaa.hide();
}
//------------------------------------------------------------------------------
void Window1::redraw_all_small_windows() {
		tof_vs_class->show_contents();
		cts_vs_class->show_contents();
		event_histogram->show_contents();
		data_histogram->show_contents();
		fitted_class_results->show_contents();
	#ifndef HAVE_ONLINE
		offline_histogram->show_contents();
	#endif

}
//------------------------------------------------------------------------------
void Window1::on_cts_window_changed() {
	make_class_division(true);
}
//------------------------------------------------------------------------------
void Window1::set_startfreq_menuitems_for_tof_vs_class_window()
{
	tof_vs_class->remove_singleclickmenuitems();
	if ( are_we_fitting_konig() ) {
//		if (two_peak_chooser->get_active()) { //eli kaksi taajuutta
		if (fit_function_combo_box_p->get_active_text() == KONIG3 ) {
			//tof_vs_class->add_singleclickmenuitem(fit_parameters->f_entry,"frequency 1 start value");
			//tof_vs_class->add_singleclickmenuitem(f2_entry,"frequency 2 start value");
			tof_vs_class->add_singleclickmenuitem("fit start value for frequency 1");
			tof_vs_class->add_singleclickmenuitem("fit start value for frequency 2");
		}
		else if ( fit_function_combo_box_p->get_active_text() == KONIG4  )
		{
			tof_vs_class->add_singleclickmenuitem("fit start value for frequency 1");
			tof_vs_class->add_singleclickmenuitem("fit start value for frequency 2");
			tof_vs_class->add_singleclickmenuitem("fit start value for frequency 3");
		}
		else
		{
			//tof_vs_class->add_singleclickmenuitem(fit_parameters->f_entry," start frequency");
			tof_vs_class->add_singleclickmenuitem("fit start value for frequency");
		}
	}
	if ( are_we_fitting_ramsey() ) {
		if (inputting->ref_radiobutton->get_active()) {
			// tof_vs_class->add_singleclickmenuitem(inputting->ref_freq_start,"start frequency (ref)");
			tof_vs_class->add_singleclickmenuitem("fit start value for frequency (ref)");
		}
		else if (inputting->meas_radiobutton->get_active()) {
			// tof_vs_class->add_singleclickmenuitem(inputting->meas_freq_start,"start frequency (meas)");
			tof_vs_class->add_singleclickmenuitem("fit start value for frequency (meas)");
		}
	}
}

//------------------------------------------------------------------------------
void Window1::set_online_measurement_id(const std::string & id)
{
    inputting->last_bunchpack_id->set_text(id);
};

//------------------------------------------------------------------------------
#ifdef HAVE_ONLINE
std::string Window1::get_desired_online_id()
{
    return inputting->get_desired_id_of_measurement();
}
//------------------------------------------------------------------------------
void Window1::set_window_labels_with_meas_id() {
	std::string online_id = get_desired_online_id();
	set_title(online_id + " " + window_title_without_meas_id);
	five_plotareas_window->set_default_window_title(Glib::ustring(online_id + " plots"));
	yksi_plotarea_window->set_default_window_title(Glib::ustring(online_id + " History / 5 bunches"));
// 	toinen_yksi_plotarea_window->set_default_window_title(Glib::ustring(online_id + " Bunches per class (of 20 bunches)"));
}
#endif
//------------------------------------------------------------------------------
int Window1::get_bin_min() {
	if (ref_radiobutton->get_active() || windows_same_as_ref_cb->get_active()) {
		return ref_bin_min_spinbutton->get_value_as_int();
	}
	else
		return meas_bin_min_spinbutton->get_value_as_int();
}
//------------------------------------------------------------------------------
int Window1::get_bin_max() {
	if (ref_radiobutton->get_active() || windows_same_as_ref_cb->get_active()) {
		return ref_bin_max_spinbutton->get_value_as_int();
	}
	else
		return meas_bin_max_spinbutton->get_value_as_int();
}
//------------------------------------------------------------------------------
int Window1::get_cts_min() {
	if (ref_radiobutton->get_active() || windows_same_as_ref_cb->get_active()) {
		return ref_cts_min_spinbutton->get_value_as_int();
	}
	else
		return meas_cts_min_spinbutton->get_value_as_int();
}
//------------------------------------------------------------------------------
int Window1::get_cts_max() {
	if (ref_radiobutton->get_active() || windows_same_as_ref_cb->get_active()) {
		return ref_cts_max_spinbutton->get_value_as_int();
	}
	else
		return meas_cts_max_spinbutton->get_value_as_int();
}
//------------------------------------------------------------------------------
double Window1::get_freq_min() {
	if (ref_radiobutton->get_active() || windows_same_as_ref_cb->get_active()) {
		return ref_freq_min_spinbutton->get_value();
	}
	else
		return meas_freq_min_spinbutton->get_value();
}
//------------------------------------------------------------------------------
double Window1::get_freq_max() {
	if (ref_radiobutton->get_active() || windows_same_as_ref_cb->get_active()) {
		return ref_freq_max_spinbutton->get_value();
	}
	else
		return meas_freq_max_spinbutton->get_value();
}
//------------------------------------------------------------------------------
void Window1::on_windows_same_as_ref_cb_toggled() {
    // FIXFIX table1->set_sensitive(!windows_same_as_ref_cb->get_active() );
	if (meas_radiobutton->get_active()) {
		bin_or_cts_win_value_changed();
	}
}
//------------------------------------------------------------------------------
void Window1::update_cts_menues_in_cts_vs_tof_window() {
	tof_vs_class->remove_ranges();
	if (inputting->ref_radiobutton->get_active()) {
		tof_vs_class->add_range("frequency window (ref)");
		tof_vs_class->add_range("baseline for S/N calculation (ref)");
		tof_vs_class->add_range("resonance for S/N calculation (ref)");
	}
	else {
		tof_vs_class->add_range("frequency window (meas)");
		tof_vs_class->add_range("baseline for S/N calculation (meas)");
		tof_vs_class->add_range("resonance for S/N calculation (meas)");
	}
}
//------------------------------------------------------------------------------
void Window1::update_events_menues_in_cts_vs_bunches_window() {
	data_histogram->remove_ranges();
	event_histogram->remove_ranges();
	if (inputting->ref_radiobutton->get_active() ) // || windows_same_as_ref_cb->get_active())
	{
		data_histogram->add_range("bin window (ref)");
		event_histogram->add_range("count window (ref)");
	}
	else
	{
		data_histogram->add_range("bin window (meas)");
		event_histogram->add_range("count window (meas)");
	}
}
//------------------------------------------------------------------------------
void Window1::on_use_bin_gate_as_tof_plot_range_toggled() {
	set_tof_plot_range_from_bin_gate_if_allowed();
}
//------------------------------------------------------------------------------
void Window1::set_tof_plot_range_from_bin_gate_if_allowed() {
	LOCK
	tof_vs_class_need_update = true;
	if (use_bin_gate_as_tof_plot_range->get_active() ) {
		tof_vs_class->override_yrange( (get_bin_min()+ data->get_MCS_recording_offset_in_bins() )*data->getBin_width(),
									   (get_bin_max()+ data->get_MCS_recording_offset_in_bins() )*data->getBin_width());
	}
	RELEASE
}
//------------------------------------------------------------------------------
void Window1::on_fit_function_combobox_changed() {
		#ifdef HAVE_ONLINE
	suspend_online_updating = true;
	#endif
	//cout << fit_function_combo_box_p->get_active_text() << "\n";

	bool state(0),state2(0);
	//if (function_konig->get_active()) {
	if (are_we_fitting_konig() ) {
		state = 0;
	}
	//if (function_ramsey->get_active() ) {
	if (are_we_fitting_ramsey() ) {
		state = 1;
	}
	//if (function_ramsey_damping->get_active() ) {
	if (fit_function_combo_box_p->get_active_text() == RAMSEY3 ) {
		state = 1;
		state2 = 1;
	}

	//Set crap fit parameters box active
	if (fit_function_combo_box_p->get_active_text() == KONIG2 || fit_function_combo_box_p->get_active_text() == RAMSEY2  ) {
	  fit_parameters->set_crap_boxes_sensitivity(true);
	}
	else
	  fit_parameters->set_crap_boxes_sensitivity(false);

	if (fit_function_combo_box_p->get_active_text() == KONIG3 ) {
	  set_2nd_peak_fit(true);
	  set_3rd_peak_fit(false);
	  extra_peaks_box->show_all();
	}
	else if (fit_function_combo_box_p->get_active_text() == KONIG4)
	{
	  set_2nd_peak_fit(true);
	  set_3rd_peak_fit(true);
	  extra_peaks_box->show_all();
	}
	else {
	  extra_peaks_box->hide();
	  set_2nd_peak_fit(false);
	  set_3rd_peak_fit(false);
	}

	fit_parameters->t_RF_ramsey_label->set_sensitive(state);
	fit_parameters->t_RF_ramsey_entry->set_sensitive(state);
	fit_parameters->fitted_t_wait_entry->set_sensitive(state);
	fit_parameters->fitted_t_waiterr_entry->set_sensitive(state);
	fit_parameters->ft_wait_button->set_sensitive(state);
	fit_parameters->t_wait_unit_label->set_sensitive(state);

	if (!state) { //Ramsey tavarat piilooon
		fit_parameters->t_RF_ramsey_label->hide();
		fit_parameters->t_RF_ramsey_entry->hide();
		fit_parameters->fitted_t_wait_entry->hide();
		fit_parameters->fitted_t_waiterr_entry->hide();
		fit_parameters->ft_wait_button->hide();
		fit_parameters->t_wait_unit_label->hide();
		resize(1,1);
	}
	else {
		fit_parameters->t_RF_ramsey_label->show();
		fit_parameters->t_RF_ramsey_entry->show();
		fit_parameters->fitted_t_wait_entry->show();
		fit_parameters->fitted_t_waiterr_entry->show();
		fit_parameters->ft_wait_button->show();
		fit_parameters->t_wait_unit_label->show();
	}

	fit_parameters->p_entry->set_sensitive(!state || state2);
	fit_parameters->fp_button->set_sensitive(!state || state2);
	fit_parameters->fitted_p_entry->set_sensitive(!state || state2);
	fit_parameters->fitted_perr_entry->set_sensitive(!state || state2);
	fit_parameters->p_unit_label->set_sensitive(!state || state2);
	fit_parameters->p_label->set_sensitive(!state || state2);

	fit_parameters->cycl_entry->set_sensitive(!state);
	fit_parameters->fcycl_button->set_sensitive(!state);
	fit_parameters->fitted_cycl_entry->set_sensitive(!state);
	fit_parameters->fitted_cyclerr_entry->set_sensitive(!state);
	fit_parameters->cycl_unit_label->set_sensitive(!state);
	fit_parameters->cycl_label->set_sensitive(!state);

	fit_parameters->fi_entry->set_sensitive(!state);
	fit_parameters->ffi_button->set_sensitive(!state);
	fit_parameters->fitted_fi_entry->set_sensitive(!state);
	fit_parameters->fitted_fierr_entry->set_sensitive(!state);
	fit_parameters->fi_unit_label->set_sensitive(!state);
	fit_parameters->fi_label->set_sensitive(!state);

	if (state2 == true) {
		fit_parameters->p_label->set_text("damping");
		fit_parameters->p_unit_label->set_text("??");
	}
	else {
		fit_parameters->p_label->set_text("pressure");
		fit_parameters->p_unit_label->set_text("mbar");
	}
	set_startfreq_menuitems_for_tof_vs_class_window();
	fit_parameters->on_fit_or_not_changed();
	#ifdef HAVE_ONLINE
	suspend_online_updating = false;
	#endif

}
//------------------------------------------------------------------------------
void Window1::initialize_fit_function_combobox() {
  fit_function_combo_box_p = &fit_function_combo_box;
  fit_function_combo_box_p->append(KONIG1);
  fit_function_combo_box_p->append(KONIG2);
  fit_function_combo_box_p->append(KONIG3);
  fit_function_combo_box_p->append(KONIG4);
  fit_function_combo_box_p->append(RAMSEY1);
  fit_function_combo_box_p->append(RAMSEY2);
  fit_function_combo_box_p->append(RAMSEY3);


  fit_function_combo_box_p->signal_changed().connect(sigc::mem_fun(*this, &Window1::on_fit_function_combobox_changed), false);

  fit_function_combo_box_p->set_active_text(KONIG1);


  hbox_duh->add(*fit_function_combo_box_p);
  fit_function_combo_box_p->show();
}

//------------------------------------------------------------------------------
bool Window1::are_we_fitting_konig(void) {
  //cout << "Are we fitting konig?\n" << endl;
  if (fit_function_combo_box_p->get_active_text() == KONIG1 )
	return 1;
  if (fit_function_combo_box_p->get_active_text() == KONIG2 )
	return 1;
  if (fit_function_combo_box_p->get_active_text() == KONIG3 )
	return 1;
  if (fit_function_combo_box_p->get_active_text() == KONIG4 )
	return 1;
  if (fit_function_combo_box_p->get_active_text() == KONIG5 )
	return 1;
  
  //cout << "Not fitting konig\n";
  return 0;
}
//------------------------------------------------------------------------------
bool Window1::are_we_fitting_ramsey(void) {
  if (fit_function_combo_box_p->get_active_text() == RAMSEY1)
	return 1;
  if (fit_function_combo_box_p->get_active_text() == RAMSEY2)
	return 1;
  if (fit_function_combo_box_p->get_active_text() == RAMSEY3)
	return 1;
  return 0;
}
//------------------------------------------------------------------------------

void Window1::process_eva_file_directory()
{
	/// evafiledirectory will contains all contents of .dat files.
	evafiledirectory.evafiles.clear();
	std::cout << "AA\n";

	/* Get filenames from the directory having .dat ending */
	std::list<std::string> filenames;

	std::string path = inputdir_entry->get_text();

	//For ease of debugging
	//std::string path = "/home/tomero/Projects/lakua_gitilla/src/";

	//datafmt_eva->set_active();

	/// Get all .dat files in directory, stored in "path"
	get_filenames( path, ".dat", filenames);

	/// in case no files found:
	if (filenames.size() == 0 )
	{
		Gtk::MessageDialog msg("",false, Gtk::MESSAGE_WARNING);
		msg.set_title("WARNING! ACHTUNG! VAROITUS!");
		msg.set_message("No sweeps could be found! Tried to read directory\n\n" + path);
		msg.run();
		return;
	}
	filenames.sort();


	///Reading in all .dat files.
	for (std::list<std::string>::iterator i = filenames.begin(); i != filenames.end(); ++i)
	{
		evafiledirectory.add_evafile(*i);
		//std::cout << "added: " << *i << "\n";
	}

	/// Let's see what types of scans were there.
	std::list<cSimilarScans> types = evafiledirectory.get_ion_species_types();

	/// Create the choosing structure for the found types.
	Gtk::Dialog choosing("Choose data to analyze:");
	choosing.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
	choosing.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);

	/// choosing_grp and choosing2_grp are for reference and ion-of-interest selection.
	Gtk::RadioButtonGroup choosing_grp,choosing2_grp;
	Gtk::Label ref,meas;
	ref.set_text( "Please choose REFERENCE ion species:            ");
	meas.set_text("\nPlease choose MEASUREMENT ion species:");
	std::vector<choise> choises,choises2;

	/// Populate the list of scan types. Same for ref and meas
	for (std::list<cSimilarScans>::iterator i = types.begin(); i != types.end(); ++i)
	{
		//std::cout << "N:" << i->number_of_similar_scans << "; element: " << i->element << "; mass: " << i->mass_number << "; charge: " << i->charge_state << ";\n";

		choise to_add(&(*i),choosing_grp);
		//std::cout << "Push back: ele " << to_add.ss->element << "|\n";
		choises.push_back(to_add);
		choises2.push_back(to_add);
		choises.back().radiobutton->show();
		choises2.back().radiobutton->show();

	}

	/// Add elements of REFERENCE to the dialog:
	choosing.get_vbox()->pack_start(ref);

	for (int i = 0; i < choises.size(); i++)
	{
		choosing.get_vbox()->pack_start(*(choises[i].radiobutton));
		choises[i].radiobutton->set_group(choosing_grp);
	}
	choises[0].radiobutton->set_active();

	/// Add elements of MEASUREMENT to the dialog:
	choosing.get_vbox()->pack_start(meas);

	for (int i = 0; i < choises2.size(); i++)
	{
		choosing.get_vbox()->pack_start(*(choises2[i].radiobutton));
		choises2[i].radiobutton->set_group(choosing2_grp);
	}
	choises2[0].radiobutton->set_active();

	choosing.show_all_children();
	int result = choosing.run();

	if (result == Gtk::RESPONSE_OK)
	{
		/// This sets correct "gd154_" to the REF element box
		for (int i = 0; i < choises.size(); i++)
		{
			if (choises[i].radiobutton->get_active())
			{
				//std::cout << "akt:  " << i << "\n";
				std::string ref_text = choises[i].ss->element + asString(choises[i].ss->mass_number)+"_";
				if (choises[i].ss->charge_state > 1)
					ref_text += asString(choises[i].ss->charge_state) + "_";
				ref_element_entry->set_text(ref_text);

				ref_eva_files.clear();
				// std::swap(ref_eva_files,choises[i].ss->similar_evafiles);
				ref_eva_files.resize(choises[i].ss->similar_evafiles.size());
				std::copy(choises[i].ss->similar_evafiles.begin(), choises[i].ss->similar_evafiles.end(), ref_eva_files.begin());
			}
		}
		/// This sets correct "gd152_" to the MEAS element box
		for (int i = 0; i < choises2.size(); i++)
		{
			if (choises2[i].radiobutton->get_active())
			{
				//std::cout << "akt2: " << i << "\n";
				std::string meas_text = choises2[i].ss->element + asString(choises2[i].ss->mass_number)+"_";
				if (choises2[i].ss->charge_state > 1)
					meas_text += asString(choises2[i].ss->charge_state) + "_";
				meas_element_entry->set_text(meas_text);

				meas_eva_files.clear();
				// std::swap(meas_eva_files,choises2[i].ss->similar_evafiles);
				meas_eva_files.resize(choises2[i].ss->similar_evafiles.size());
				std::copy(choises2[i].ss->similar_evafiles.begin(), choises2[i].ss->similar_evafiles.end(), meas_eva_files.begin());

			}
		}

		std::cout << "ref, meas sizes: " << ref_eva_files.size() << " " << meas_eva_files.size() << "\n";
	}
	inputting->on_filenumber_spinbutton_changed();
}

void Window1::on_histogram_compression_activate()
{
	Gtk::Dialog ask_compression_factor_dialog("Set compression factor");
	
	ask_compression_factor_dialog.add_button("Apply", Gtk::RESPONSE_APPLY);
	ask_compression_factor_dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
	
	Gtk::SpinButton factor_sp;
	factor_sp.get_adjustment()->configure(histogram_compression_factor,
					      1, 20, 1, 10, 0);
	
	ask_compression_factor_dialog.get_vbox()->pack_start(factor_sp);
	
	ask_compression_factor_dialog.show_all_children();
	int result = ask_compression_factor_dialog.run();
	
	if (result == Gtk::RESPONSE_APPLY)
	{
		histogram_compression_factor = factor_sp.get_value_as_int();
		ShowDataHistogram();
	}
}

void Window1::on_tof_vs_class_range_changed(const double & x1,const double & y1,const double & x2,const double & y2,const Glib::ustring & range_name)
{
	if (range_name == "baseline for S/N calculation (ref)")
	{
		ref_sn_base_from_sp->set_value(x1);
		ref_sn_base_to_sp->set_value(x2);
	}
	else if (range_name == "resonance for S/N calculation (ref)")
	{
		ref_sn_reso_from_sp->set_value(x1);
		ref_sn_reso_to_sp->set_value(x2);
	}
	else if (range_name == "frequency window (ref)")
	{
		ref_freq_min_spinbutton->set_value(x1);
		ref_freq_max_spinbutton->set_value(x2);
	}
	else if (range_name == "baseline for S/N calculation (meas)")
	{
		meas_sn_base_from_sp->set_value(x1);
		meas_sn_base_to_sp->set_value(x2);
	}
	else if (range_name == "resonance for S/N calculation (meas)")
	{
		meas_sn_reso_from_sp->set_value(x1);
		meas_sn_reso_to_sp->set_value(x2);
	}
	else if (range_name == "frequency window (meas)")
	{
		meas_freq_min_spinbutton->set_value(x1);
		meas_freq_max_spinbutton->set_value(x2);
	}
}
void Window1::on_tof_vs_class_singleclick(const double & x1,const double & y1,const Glib::ustring & click_name)
{
	if ( are_we_fitting_konig() ) {
		if (fit_function_combo_box_p->get_active_text() == KONIG3 || 
		    fit_function_combo_box_p->get_active_text() == KONIG4)
		{
			if (click_name == "fit start value for frequency 1")
			{
				fit_parameters->f_entry->set_text(as_3f(x1));
			}
			else if (click_name == "fit start value for frequency 2")
			{
				f2_entry->set_text(as_3f(x1));
			}
			else if (click_name == "fit start value for frequency 3")
			{
				f3_entry->set_text(as_3f(x1));
			}
		}
		else
		{
			if (click_name == "fit start value for frequency") {
				fit_parameters->f_entry->set_text(as_3f(x1));
			}
		}
	}
	if ( are_we_fitting_ramsey() ) {
		if (inputting->ref_radiobutton->get_active()) {
			if (click_name == "fit start value for frequency (ref)")
			{
				inputting->ref_freq_start->set_value(x1);
			}
		}
		else if (inputting->meas_radiobutton->get_active()) {
			if (click_name == "fit start value for frequency (meas)")
			{
				inputting->meas_freq_start->set_value(x1);
			}
		}
	}
}
void Window1::on_count_histogram_range_changed(const double & x1,const double & y1,const double & x2,const double & y2,const Glib::ustring & range_name)
{
	/// event histogram is this!
	if ( range_name == "count window (ref)")
	{
		ref_cts_min_spinbutton->set_value(x1);
		ref_cts_max_spinbutton->set_value(x2);
	}
	else if ( range_name == "count window (meas)")
	{
		meas_cts_min_spinbutton->set_value(x1);
		meas_cts_max_spinbutton->set_value(x2);
		
		if ( windows_same_as_ref_cb->get_active() )
		{
			ref_cts_min_spinbutton->set_value(x1);
			ref_cts_max_spinbutton->set_value(x2);
		}
	}
}
void Window1::on_tof_histogram_range_changed(const double & x1,const double & y1,const double & x2,const double & y2,const Glib::ustring & range_name)
{
	/// event histogram is this!
	if ( range_name == "bin window (ref)")
	{
		ref_bin_min_spinbutton->set_value(x1);
		ref_bin_max_spinbutton->set_value(x2);
	}
	else if ( range_name == "bin window (meas)")
	{
		meas_bin_min_spinbutton->set_value(x1);
		meas_bin_max_spinbutton->set_value(x2);
		
		if ( windows_same_as_ref_cb->get_active() )
		{
			ref_bin_min_spinbutton->set_value(x1);
			ref_bin_max_spinbutton->set_value(x2);
		}
	}
}
void Window1::on_shift_scanrange_button_clicked()
{
	data->shift_scanrange(shift_scanparameter_spinbutton->get_value());
	practically_update_all();
}

void Window1::on_send_to_elog_button_clicked()
{
  // Author is really required:
  if ( elog_author_entry->get_text() == "")
  {
    Gtk::MessageDialog errordialogi("Author field cannot be empty!\n\n", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
    errordialogi.set_title("Error sending to ELOG!!");
    errordialogi.set_keep_above(true);
    errordialogi.run();
    return;
  }

  //
  boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
  std::string aika = format_time_YYMMDD_hhmmss(now);
  std::cout << aika << endl;
  boost::filesystem::path elog_work_path(elog_workdir_entry->get_text().c_str());
  boost::filesystem::path gnuplot_script_file = elog_work_path; gnuplot_script_file /= aika+".gp";
  boost::filesystem::path png_out_file = elog_work_path; png_out_file /= aika+".png";
  boost::filesystem::path blaa_blaa_out_file = elog_work_path; blaa_blaa_out_file /= aika+".txt";
    
//   cout << gnuplot_script_file.c_str() << endl;
  
  bool not_ok1 = create_summary_plot_gnuplot_script(gnuplot_script_file, png_out_file);
  bool not_ok2 = execute_gnuplot_script(gnuplot_script_file);
  bool not_ok3 = create_summary_blaa_blaa_text(blaa_blaa_out_file);
  bool not_ok4 = 1;
  

  if ( !not_ok1 || !not_ok2 || !not_ok3 ) // so is ok
  {
     string command = string("\"") + double_backslashify(elog_executable_entry->get_text()) + " -h qad.igisol -p 8020 -l IGISOL2017 ";
     command += " -a \"Author\"=\""+ elog_author_entry->get_text()+"\"";
     command += " -a What=\""+ elog_what_entry->get_text()+"\"";
     command += " -a \"Experiment number\"=\""+ elog_experiment_number_entry->get_text()+"\"";
     command += " -f \""+ double_backslashify(png_out_file.string()) + "\"";
     command += " -m \""+ double_backslashify(blaa_blaa_out_file.string()) + "\"\"";
     
	 cout << "Command to do: " << command << endl;
     int res = system(command.c_str());
     if ( res != 0 )
       not_ok4 = 1;
     else not_ok4 = 0;
  }
  if ( not_ok1 || not_ok2 || not_ok3 || not_ok4 )
  {
    string error_string;
    if ( not_ok1)
      error_string += "- Could not create gnuplot script\n";
    if ( not_ok2)
      error_string += "- Failed to plot (gnuplot had an error)\n";
    if ( not_ok3)
      error_string += "- Error creating text\n";
    if ( not_ok4)
      error_string += "- Failed to send to elog\n";
    Gtk::MessageDialog erroridialogi(error_string, false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
    erroridialogi.set_title("Error sending to ELOG!!");
    erroridialogi.set_keep_above(true);
    erroridialogi.run();
  }
  
//   elog -h qad.igisol -p 8020 -l IGISOL2017
//   -a Author=Tommi
//   -a What="JYFLTRAP Scan" 
//   -a "Experiment Number"=20170105_fission
//   -f most_recent.png
//   -m "textia.txt"
  
}

void Window1::on_elog_workdir_chooser_button_clicked()
{
  Gtk::FileChooserDialog dialog("Choose elog working (temporary) directory", Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);

  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
  dialog.set_current_folder(elog_workdir_entry->get_text());
  dialog.set_default_response(Gtk::RESPONSE_OK);
  dialog.set_select_multiple(false);
  int result = dialog.run();
  if (result == Gtk::RESPONSE_OK) {
    elog_workdir_entry->set_text( dialog.get_filename() );
  }
}

void Window1::on_gnuplot_executable_chooser_button_clicked() 
{ 
  Gtk::FileChooserDialog dialog("Choose gnuplot executable", Gtk::FILE_CHOOSER_ACTION_OPEN);

  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
//   dialog.set_current_folder(gnuplot_executable_entry->get_text());
  dialog.set_default_response(Gtk::RESPONSE_OK);
  dialog.set_select_multiple(false);
  int result = dialog.run();
  if (result == Gtk::RESPONSE_OK) {
    gnuplot_executable_entry->set_text( dialog.get_filename() );
  }
}

void Window1::on_elog_executable_chooser_button_clicked()
{
  Gtk::FileChooserDialog dialog("Choose elog executable", Gtk::FILE_CHOOSER_ACTION_OPEN);

  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
//   dialog.set_current_folder(elog_executable_entry->get_text());
  dialog.set_default_response(Gtk::RESPONSE_OK);
  dialog.set_select_multiple(false);
  int result = dialog.run();
  if (result == Gtk::RESPONSE_OK) {
    elog_executable_entry->set_text( dialog.get_filename() );
  }
}

void Window1::on_debug1_button_clicked()
{

}
void Window1::on_debug2_button_clicked()
{
}
void Window1::on_debug3_button_clicked()
{
}
