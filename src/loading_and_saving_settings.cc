#include <vector>
#include <boost/algorithm/string.hpp>
#include <string>

#include <json_spirit_value.h>
#include <json_spirit_reader_template.h>
#if 0
#include <json_spirit_writer.h>
#include <json_spirit_writer_template.h>
#endif

#include "window1.hh"

#include "fit_parameters.hh"
#include "inputting.hh"

#include <gtkmm/checkmenuitem.h>
#include <gtkmm/radiomenuitem.h>

using namespace std;

typedef std::vector<std::string> Pieces;


static void
add_value(json_spirit::mObject & obj,
	  const std::string & name,
	  const std::string & str)
{
    try {
	int x = boost::lexical_cast<int>(str);
	obj[name] = x;
	return;
    } catch( boost::bad_lexical_cast const &) { }

    try {
	double x = boost::lexical_cast<double>(str);
	obj[name] = x;
	return;
    } catch( boost::bad_lexical_cast const &) { }
    obj[name] = str;
}

static void
add_value(json_spirit::mArray & arr,
	  int idx,
	  const std::string & str)
{
    try {
	int x = boost::lexical_cast<int>(str);
	arr[idx] = x;
	return;
    } catch( boost::bad_lexical_cast const &) { }

    try {
	double x = boost::lexical_cast<double>(str);
	arr[idx] = x;
	return;
    } catch( boost::bad_lexical_cast const &) { }
    arr[idx] = str;
}

static void
load_ini_file(
    json_spirit::mObject & lakritsi_obj,
    boost::filesystem::ifstream & openedfile)
{
    std::string row;

    while (getline(openedfile, row)) {
	if (row[0] == '#') {
	    continue;
	}
	Pieces pieces;

	boost::split(pieces, row, boost::is_any_of("\t"));
	if (pieces.size() > 2) {
	    json_spirit::mArray arr(pieces.size() - 1);
	    int i = 0;
	    for (Pieces::const_iterator iter = pieces.begin() + 1;
		 iter != pieces.end();
		 ++iter) {
		add_value(arr, i, *iter);
		i++;
	    }
	    lakritsi_obj[pieces[0]] = arr;
	} else if (pieces.size() == 2) {
	    add_value(lakritsi_obj, pieces[0], pieces[1]);
	} else {
	    lakritsi_obj[pieces[0]] = "";
	}
    }
#if 0
    json_spirit::mObject main_obj;
    main_obj["lakritsi"] = lakritsi_obj;
    boost::filesystem::ofstream jsonfile;
    jsonfile.open("lakritsi.json");
    json_spirit::write(main_obj, jsonfile,
		       json_spirit::pretty_print
		       | json_spirit::remove_trailing_zeros
		       | json_spirit::single_line_arrays);
#endif
}

int Window1::loadIniFile(const Glib::ustring & filename){
	boost::filesystem::ifstream openedfile;
	int tempnumero;
	double tempdbl;
	int rownumber(0);
	string errorstring;

	open_local_or_rc_file(openedfile, filename);
  	if (!openedfile)
		return -1;
	std::string row;
	std::string piece;
	int invalids(0);

	json_spirit::mObject jobj;
	load_ini_file(jobj, openedfile);

	inputting->twod_use_button->set_active(
	    jobj["2D_INPUT_FILE"].get_int() == 1);

	typedef struct {
	    const char * name;
	    Gtk::Entry * entry;
	} Entry_Stuff;

	Entry_Stuff entries[] = {
	    { "DATASET_NAME", post_process_filename_entry },
	    { "FILES_TO_SKIP", inputting->skips_entry },
	    { "INPUT_DIR", inputting->inputdir_entry },
	    { "MEAS_ELEMENT", inputting->meas_element_entry },
	    { "MEAS_GNUPLOT_OUTFILE", meas_gnuplot_outfile_entry },
	    { "MEAS_OUTFILE", meas_outfile_entry },
	    { "OUTPUT_DIR", outputdir_entry },
	    { "REF_ELEMENT", inputting->ref_element_entry },
	    { "REF_OUTFILE", ref_outfile_entry },
	    { "REF_GNUPLOT_OUTFILE", ref_gnuplot_outfile_entry },
	    { "ROUNDS_TO_SKIP", inputting->round_skips_entry },
      { "ELOG_EXP_NUMBER", elog_experiment_number_entry },
      { "ELOG_AUTHOR", elog_author_entry },
      { "ELOG_WHAT", elog_what_entry },
      { "GNUPLOT_EXECUTABLE", gnuplot_executable_entry },
      { "ELOG_EXECUTABLE", elog_executable_entry },
      { "COPY_COMMAND", copy_command_entry},
      { "ELOG_WORKDIR", elog_workdir_entry}
	};

	for(int i=0; i < sizeof(entries)/sizeof(Entry_Stuff); i++) {
	    const Entry_Stuff * x = &entries[i];
	    x->entry->set_text(jobj[x->name].get_str());
	}
	on_outputdir_entry_changed();

	typedef struct {
	    const char * name;
	    Gtk::CheckMenuItem * cb;
	} CBI_Stuff;
	CBI_Stuff cbis[] = {
	    { "INSTANT_FIT", fit_immediately },
	    { "LOAD_AND_USE_ROUND_SELECTION",
	      load_and_use_round_selection_automatically },
	    { "LOAD_OR_NOT", load_window_positions_from_settingfile },
	    { "CHECK_NEW_VERSION_AT_STARTUP",
	      check_for_new_version_at_startup }
	};
	for(int i=0; i < sizeof(cbis)/sizeof(CBI_Stuff); i++) {
	    const CBI_Stuff * x = &cbis[i];
	    x->cb->set_active(jobj[x->name].get_int() == 1);
	}

	typedef struct {
	    const char * name;
	    Gtk::SpinButton * button;
	} SB_Stuff;
	SB_Stuff int_spinbuttons[] = {
	    { "BATCH_START", inputting->startfile_spinbutton },
	    { "BATCH_STOP", inputting->endfile_spinbutton },
	    { "BATCH_STEP", inputting->batch_step_spinbutton },
	    { "CHARGE", inputting->charge_spinbutton },
	    { "CLASSES", how_many_classes_spinbutton },
	    { "MASS", inputting->mass_spinbutton },
	    { "NUMBER", inputting->filenumber_spinbutton },
	    { "REF_BIN_START", ref_bin_min_spinbutton },
	    { "REF_BIN_STOP", ref_bin_max_spinbutton },
	    { "REF_CTS_START", ref_cts_min_spinbutton },
	    { "REF_CTS_STOP", ref_cts_max_spinbutton },
	    { "MEAS_BIN_START", meas_bin_min_spinbutton },
	    { "MEAS_BIN_STOP", meas_bin_max_spinbutton },
	    { "MEAS_CTS_START", meas_cts_min_spinbutton },
	    { "MEAS_CTS_STOP", meas_cts_max_spinbutton }
	};

	for(int i=0; i < sizeof(int_spinbuttons)/sizeof(SB_Stuff); i++) {
	    const SB_Stuff * x = &int_spinbuttons[i];
	    x->button->set_value(jobj[x->name].get_int());
	}

	SB_Stuff real_spinbuttons[] = {
	    { "MEAS_FREQ_WIN_START", meas_freq_min_spinbutton },
	    { "MEAS_FREQ_WIN_STOP", meas_freq_max_spinbutton },
	    { "MEAS_FREQ_START", inputting->meas_freq_start },
	    { "MEAS_SIGNAL_TO_NOISE_BASELINE_FREQ1", meas_sn_base_from_sp },
	    { "MEAS_SIGNAL_TO_NOISE_BASELINE_FREQ2", meas_sn_base_to_sp },
	    { "MEAS_SIGNAL_TO_NOISE_RESONANCE_FREQ1", meas_sn_reso_from_sp },
	    { "MEAS_SIGNAL_TO_NOISE_RESONANCE_FREQ2", meas_sn_reso_to_sp },
	    { "REF_FREQ_START", inputting->ref_freq_start },
	    { "REF_FREQ_WIN_START", ref_freq_min_spinbutton },
	    { "REF_FREQ_WIN_STOP", ref_freq_max_spinbutton },
	    { "REF_SIGNAL_TO_NOISE_BASELINE_FREQ1", ref_sn_base_from_sp },
	    { "REF_SIGNAL_TO_NOISE_BASELINE_FREQ2", ref_sn_base_to_sp },
	    { "REF_SIGNAL_TO_NOISE_RESONANCE_FREQ1", ref_sn_reso_from_sp },
	    { "REF_SIGNAL_TO_NOISE_RESONANCE_FREQ2", ref_sn_reso_to_sp }
	};

	for(int i=0; i < sizeof(real_spinbuttons)/sizeof(SB_Stuff); i++) {
	    const SB_Stuff * x = &real_spinbuttons[i];
	    x->button->set_value(jobj[x->name].get_real());
	}

	if (jobj["FILE_IN_USE"].get_str() == "MEAS") {
	    inputting->meas_radiobutton->set_active();
	} else {
	    inputting->ref_radiobutton->set_active();
	}
	{
	    std::string tmp = jobj["STATISTICS_MODE"].get_str();

	    if (tmp == "SUMMED") {
		sum_statistics->set_active();
	    } else if (tmp == "SUMMED2") {
		sum_statistics_2->set_active();
	    } else if (tmp == "INDIVIDUAL") {
		individual_statistics->set_active();
	    }
	}


	{
	    std::string tmp = jobj["DATA_FORMAT"].get_str();
	    old_data_file_format_in_use = false;

	    if (tmp == "OLD") {
		old_data_file_format_in_use = true;
		datafmt_old->set_active();
		data->setDataFileFormat(DATAFORMAT_OLD);
		std::cout << "\n\nUsing old data file format. Is this intentional?\n";
	    } else if (tmp == "MED") {
		datafmt_new->set_active();
		data->setDataFileFormat(DATAFORMAT_NEW); // reverts to "new" format
		std::cout << "Trying MED datafileformat! Not supported! (Reverting to new format)\n";
	    } else if (tmp == "FEB2009") {
		datafmt_newer->set_active();
		data->setDataFileFormat(DATAFORMAT_NEWER);
		std::cout << "Datafileformat since Feb2009\n";
	    } else if (tmp == "EVA") {
		datafmt_eva->set_active();
		data->setDataFileFormat(DATAFORMAT_EVA);
		//std::cout << "EVA f\n";
	    } else {
		datafmt_new->set_active();
		data->setDataFileFormat(DATAFORMAT_NEW);
	    }
	}
	if (jobj["DATA_FORMAT"].get_str() == "WIN") {
	    //			std::cout << "On winkku\n";
	    MS_windows = true;
	} else {
	    //		std::cout << "ei oo winkku\n";
	    MS_windows = false;
	}

	{
	    bool loadi = (jobj["INSTANT_LOAD"].get_int() == 1);
#ifdef HAVE_ONLINE
	    instant_load->set_active(false);
#endif
#ifndef HAVE_ONLINE
	    instant_load->set_active(loadi);
#endif
	}

#if 0

	if (piece == "AUTOMAGIC") {
	    piece = erota(row,'\t'); // 'auto t_rf'
	    if (piece == "1")
		fit_parameters->auto_t_rf_button->set_active();
	    if (piece == "0")
		fit_parameters->auto_t_rf_button->set_active(false);
	    piece = erota(row,'\t'); // 'auto a_rf'
	    if (piece == "1")
		fit_parameters->auto_a_rf_button->set_active();
	    if (piece == "0")
		fit_parameters->auto_a_rf_button->set_active(false);
	    continue;
	}

	if (piece == "FREQ"){
	    piece = erota(row,'\t');
	    fit_parameters->f_entry->set_text(piece);
	    piece = erota(row,'\t');
	    if (piece == "1")
		fit_parameters->ff_button->set_active();
	    else
		fit_parameters->ff_button->set_active(false);
	    piece = erota(row,'\t');
	    if (piece == "1")
		fit_parameters->autofreq_button->set_active();
	    else
		fit_parameters->autofreq_button->set_active(false);
	    continue;
	}
#endif
	typedef struct {
	    const char * name;
	    Gtk::Entry * entry;
	    Gtk::CheckButton * fit;
	    Gtk::CheckButton * auto_;
	} Asdf;

	Asdf foos[] = {
	    { "FREQ", fit_parameters->f_entry,
	      fit_parameters->ff_button, fit_parameters->autofreq_button },
	    { "PRESS", fit_parameters->p_entry,
	      fit_parameters->fp_button, NULL},
	    { "AXIAL", fit_parameters->ax_entry,
	      fit_parameters->fax_button, NULL},
	    { "MAGN", fit_parameters->magn_entry,
	      fit_parameters->fmagn_button, NULL},
	    { "CYCL", fit_parameters->cycl_entry,
	      fit_parameters->fcycl_button, NULL},
	    { "PHASE", fit_parameters->fi_entry,
	      fit_parameters->ffi_button, NULL},
	    { "TIME", fit_parameters->t_tot_entry,
	      fit_parameters->ft_rf_button, NULL},
	    { "TIME_WAIT", fit_parameters->t_RF_ramsey_entry,
	      fit_parameters->ft_wait_button, NULL},
	    { "AMPL", fit_parameters->a_rf_entry,
	      fit_parameters->fa_rf_button, NULL},
	    { "AMPL", fit_parameters->a_rf_entry,
	      fit_parameters->fa_rf_button, NULL}
	    //{ "FREQ2", f2_entry, ff2_button, NULL},
	    //{ "FRACTION", fraction_entry, ffraction_button, NULL}
	    //{ "FREQ3", f3_entry, ff3_button, NULL},
	    //{ "WEIGHT3", f3_weight_entry, fweight3_button, NULL}
	};

	for(int i=0; i < sizeof(foos)/sizeof(Asdf); i++) {
	    const Asdf * x = &foos[i];
	    const json_spirit::mArray & arr = jobj[x->name].get_array();
	    std::stringstream tmp;
	    tmp.precision(4);

	    tmp << arr[0].get_real();
	    x->entry->set_text(tmp.str());
	    if (x->fit) {
		x->fit->set_active(arr[1].get_int() == 1);
	    }
	    if (x->auto_) {
		x->auto_->set_active(arr[2].get_int() == 1);
	    }
	}


	fit_function_combo_box_p->set_active_text(
	    jobj["FITFUNCTION"].get_str());

	class_borders_fixed_button->set_active(
	    jobj["FIXED_CLASSES"].get_int() == 1);
	inputting->first_bunch_button->set_active(
	    jobj["DISREGARD_FIRST_BUNCH"].get_int() == 1);

	{
	    const json_spirit::mArray & arr = jobj["CONSTANT"].get_array();
	    if (arr.size() == 3) {
		stringstream tmp;
		cerr << "Before " << endl;
		tmp << arr[0].get_int();
		cerr << "After" << endl;
		iniconst_entry->set_text(tmp.str());
		fitconst_checkbutton->set_active(arr[1].get_int() == 1);
		autoconst_checkbutton->set_active(arr[2].get_int() == 1);
	    }
	}
	{
	    const json_spirit::mArray & arr = jobj["SLOPE"].get_array();
	    if (arr.size() == 3) {
		stringstream tmp;
		tmp << arr[0].get_int();
		inislope_entry->set_text(tmp.str());
		fitslope_checkbutton->set_active(arr[1].get_int() == 1);
		autoslope_checkbutton->set_active(arr[2].get_int() == 1);
	    }
	}
	//2007-03-12 Minimizing parts
	show_datastuff_cb->set_active(
	    jobj["SHOW_CLASS_DIVISION_PART"].get_int() == 1);
	show_fitting_things_cb->set_active(
	    jobj["SHOW_FITTING_PART"].get_int() == 1);

	//2007-03-15 2d-contour
	{
	    const json_spirit::mArray & arr = jobj["2D_CONTOUR_COLOR"].get_array();
	    if (arr.size() == 3) {
		Gdk::Color kolori;
		kolori.set_rgb(arr[0].get_int(), arr[1].get_int(),
			       arr[2].get_int());
		tof_vs_class->set_contour_color(kolori);
	    }
	}
	bin_interval_in_2d_contour = jobj["2D_BIN_INTERVAL"].get_int();

	//2007-06-12 background
	{
	    const json_spirit::mArray & arr = jobj["BGR_COLOR_OF_WINDOWS"].get_array();
	    if (arr.size() == 3) {
		background_color_for_windows.set_rgb(
		    arr[0].get_int(), arr[1].get_int(), arr[2].get_int());
	    }
	}

	background_alpha = jobj["BGR_ALPHA"].get_int();

	//2007-04-20 Guessing the mass number
	inputting->guess_mass_cb->set_active(
	    jobj["GUESS_MASS_NUMBER"].get_int() == 1);
	inputting->guess_charge_cb->set_active(
	    jobj["GUESS_CHARGE_STATE"].get_int() == 1);
	use_bin_gate_as_tof_plot_range->set_active(
	    jobj["SET_TOF_PLOT_RANGE_FROM_BIN_GATE"].get_int() == 1);
	windows_same_as_ref_cb->set_active(
	    jobj["SAME_WINDOWS_FOR_MEAS_AS_FOR_REF"].get_int() == 1);

#if 0
//2008-02-25 Commands via settingfile
	if (piece == "PRESS_SPLIT_SINGLE_FILE") {
	    SplitAndFitFile();
	}
#endif
	validateSpinButtons();
	return invalids;
}
//------------------------------------------------------------------------------
bool Window1::store_settings(Glib::ustring directory, Glib::ustring filename, bool append, bool rcfile) {
	boost::filesystem::ofstream settingsfile;

	ios_base::openmode mode = ios::out;
	if (append) {
	    mode = ios::app;
	}

	if (rcfile) {
	    open_local_or_rc_file(settingsfile,
				  (directory+filename), mode);
	} else {
	    settingsfile.open((directory+filename).c_str(), mode);
	}
	if (!settingsfile) {
		std::cout << "Could not open the settingsfile " << directory << filename << std::endl;
		return 0;
	}
	char DL = '\t';
	loc_tm current_time(0);
	settingsfile.precision(10);
	settingsfile << "#Settings saved at " << loc_date(current_time,loc_tm::d_oma) << " " << loc_time(current_time,loc_tm::t_International) << "\n";
	settingsfile << "DATASET_NAME"	<< DL << post_process_filename_entry->get_text() << "\n";
	settingsfile << "2D_INPUT_FILE"	<< DL << inputting->twod_use_button->get_active()		<<	"\n";
	settingsfile <<	"INPUT_DIR"		<< DL << inputting->inputdir_entry->get_text() 		<<	"\n";
	settingsfile <<	"REF_ELEMENT"	<< DL << inputting->ref_element_entry->get_text() 		<<	"\n";
	settingsfile <<	"MEAS_ELEMENT"	<< DL << inputting->meas_element_entry->get_text() 	<<	"\n";
	settingsfile <<	"NUMBER"		<< DL << inputting->filenumber_spinbutton->get_text()	<<	"\n";
	settingsfile << "#whether data gets read immediately after change of filenumber; 1=yes, 0=no\n";
	settingsfile << "INSTANT_LOAD"	<< DL << instant_load->get_active()			<<	"\n";
	settingsfile << "INSTANT_FIT"	<< DL << fit_immediately->get_active()		<<	"\n";
	settingsfile << "LOAD_AND_USE_ROUND_SELECTION"	<< DL << load_and_use_round_selection_automatically->get_active() << "\n";
	settingsfile << "DISREGARD_FIRST_BUNCH" << DL << inputting->first_bunch_button->get_active()	<< "\n";
	settingsfile << "STATISTICS_MODE" << DL;
	if (individual_statistics->get_active() )
		settingsfile << "INDIVIDUAL\n";
	if (sum_statistics->get_active() )
		settingsfile << "SUMMED\n";
	if (sum_statistics_2->get_active() ) {
		settingsfile << "SUMMED2\n";
	}

	settingsfile <<	"MASS"			<< DL << inputting->mass_spinbutton->get_text()		<<	"\n";
	settingsfile <<	"CHARGE"			<< DL << inputting->charge_spinbutton->get_text()		<<	"\n";
	settingsfile <<	"OUTPUT_DIR"	<< DL << outputdir_entry->get_text()		<<	"\n";
	settingsfile <<	"CLASSES"		<< DL << how_many_classes_spinbutton->get_text() <<	"\n";

	settingsfile <<	"REF_BIN_START"		<< DL << ref_bin_min_spinbutton->get_text()			<<	"\n";
	settingsfile <<	"REF_BIN_STOP"		<< DL << ref_bin_max_spinbutton->get_text()			<<	"\n";
	settingsfile <<	"REF_CTS_START"		<< DL << ref_cts_min_spinbutton->get_text()			<<	"\n";
	settingsfile <<	"REF_CTS_STOP"		<< DL << ref_cts_max_spinbutton->get_text()			<<	"\n";
	settingsfile << "REF_FREQ_WIN_START"	<< DL << ref_freq_min_spinbutton->get_text()		<<	"\n";
	settingsfile << "REF_FREQ_WIN_STOP"		<< DL << ref_freq_max_spinbutton->get_text()		<<	"\n";
	settingsfile <<	"MEAS_BIN_START"		<< DL << meas_bin_min_spinbutton->get_text()			<<	"\n";
	settingsfile <<	"MEAS_BIN_STOP"		<< DL << meas_bin_max_spinbutton->get_text()			<<	"\n";
	settingsfile <<	"MEAS_CTS_START"		<< DL << meas_cts_min_spinbutton->get_text()			<<	"\n";
	settingsfile <<	"MEAS_CTS_STOP"		<< DL << meas_cts_max_spinbutton->get_text()			<<	"\n";
	settingsfile << "MEAS_FREQ_WIN_START"	<< DL << meas_freq_min_spinbutton->get_text()		<<	"\n";
	settingsfile << "MEAS_FREQ_WIN_STOP"		<< DL << meas_freq_max_spinbutton->get_text()		<<	"\n";
	settingsfile << "SAME_WINDOWS_FOR_MEAS_AS_FOR_REF"		<< DL << windows_same_as_ref_cb->get_active()		<< "\n";
	settingsfile << "SET_TOF_PLOT_RANGE_FROM_BIN_GATE" << DL << use_bin_gate_as_tof_plot_range->get_active()		<< "\n";

	settingsfile <<	"BATCH_START"	<< DL << inputting->startfile_spinbutton->get_text()		<<	"\n";
	settingsfile <<	"BATCH_STOP"	<< DL << inputting->endfile_spinbutton->get_text()			<<	"\n";
	settingsfile <<	"FILES_TO_SKIP"	<< DL << inputting->skips_entry->get_text()			<<	"\n";
	settingsfile <<	"ROUNDS_TO_SKIP"	<< DL << inputting->round_skips_entry->get_text()			<<	"\n";
	settingsfile << "BATCH_STEP"	<< DL << inputting->batch_step_spinbutton->get_text()		<<	"\n";
	settingsfile << "REF_FREQ_START"	<< DL << inputting->ref_freq_start->get_value()		<<	"\n";
	settingsfile << "MEAS_FREQ_START"	<< DL << inputting->meas_freq_start->get_value()		<<	"\n";
	settingsfile <<	"REF_OUTFILE"	<< DL << ref_outfile_entry->get_text()			<<	"\n";
	settingsfile <<	"REF_GNUPLOT_OUTFILE"	<< DL << ref_gnuplot_outfile_entry->get_text()	<<	"\n";
	settingsfile <<	"MEAS_OUTFILE"			<< DL << meas_outfile_entry->get_text()			<<	"\n";
	settingsfile <<	"MEAS_GNUPLOT_OUTFILE"	<< DL << meas_gnuplot_outfile_entry->get_text()	<<	"\n";
  
  // 2017-01-17
  settingsfile << "ELOG_EXP_NUMBER" << DL << elog_experiment_number_entry->get_text() << "\n";
  settingsfile << "ELOG_AUTHOR" << DL << elog_author_entry->get_text() << "\n";
  settingsfile << "ELOG_WHAT" << DL << elog_what_entry->get_text() << "\n";
  settingsfile << "GNUPLOT_EXECUTABLE" << DL << gnuplot_executable_entry->get_text() << "\n";
  settingsfile << "ELOG_EXECUTABLE" << DL << elog_executable_entry->get_text() << "\n";
  settingsfile << "COPY_COMMAND" << DL << copy_command_entry->get_text() << "\n";
  settingsfile << "ELOG_WORKDIR" << DL << elog_workdir_entry->get_text() << "\n";
  
  
	if (inputting->meas_radiobutton->get_active() )
		settingsfile <<	"FILE_IN_USE" << DL << "MEAS\n";
	else
		settingsfile <<	"FILE_IN_USE" << DL << "REF\n";
	settingsfile <<	"#Data format: OLD, MED, NEW (individual bunches)\n";
	while (1) {
		if (datafmt_old->get_active() ) {
			settingsfile <<	"DATA_FORMAT" 		<< DL << "OLD" <<	"\n";
			break;
		}
		if (datafmt_new->get_active() ) {
			settingsfile <<	"DATA_FORMAT" 		<< DL << "NEW" <<	"\n";
			break;
		}
		if (datafmt_med->get_active() ) {
			settingsfile <<	"DATA_FORMAT" 		<< DL << "MED" <<	"\n";
			break;
		}
		if (datafmt_newer->get_active() ) {
			settingsfile <<	"DATA_FORMAT" 		<< DL << "FEB2009" <<	"\n";
			break;
		}
		if (datafmt_eva->get_active() ) {
			settingsfile <<	"DATA_FORMAT" 		<< DL << "EVA" <<	"\n";
			break;
		}
		break;
	}
	settingsfile <<	"#Preferred system WIN LINUX  (effects initial window positions)\n";
	if (MS_windows)
		settingsfile <<	"PLATFORM"			<< DL << "WIN"	<<	"\n";
	else
		settingsfile <<	"PLATFORM"			<< DL << "LINUX"<<	"\n";
	settingsfile <<	"#\n#\n";
	settingsfile <<	"#------------------------------------------------------------------------------\n";
	settingsfile <<	"#Starting fitting parameters\n";
	settingsfile <<	"#\n";
	settingsfile <<	"#Frequency needs 3 arguments. PARAM	VALUE	FITTED? AUTO?\n";
	fit_parameters->pretty_print(settingsfile);
//	settingsfile << "FREQ2" << DL << f2_entry->get_text() << DL
//				<< ff2_button->get_active() << "\n";
//	settingsfile << "FRACTION" << DL << fraction_entry->get_text() << DL
//				<< ffraction_button->get_active() << "\n";
//	settingsfile << "FREQ3" << DL << f3_entry->get_text() << DL
//				<< ff3_button->get_active() << "\n";
//	settingsfile << "WEIGHT3" << DL << f3_weight_entry->get_text() << DL
//				<< fweight3_button->get_active() << "\n";
				
	settingsfile << "FITFUNCTION" << DL << fit_function_combo_box_p->get_active_text() << "\n";
/*
	settingsfile << "FIT2PEAKS_ACTIVE" << DL << two_peak_chooser->get_active() << "\n";
	settingsfile << "FIT_CRAP_HAVING_CONSTANT_TOF" << DL << fit_crap_chooser->get_active() << "\n";
	if (function_konig->get_active() )
		settingsfile << "FIT_FUNCTION" << DL << "NORMAL" << "\n";
	if (function_ramsey->get_active() )
		settingsfile << "FIT_FUNCTION" << DL << "RAMSEY" << "\n";
	if (function_ramsey_damping->get_active() )
		settingsfile << "FIT_FUNCTION" << DL << "RAMSEY_DAMPING" << "\n";
*/
	settingsfile << "FIXED_CLASSES" << DL << class_borders_fixed_button->get_active() << "\n";
	//2007-01-19 Adding linear slope stuff
	settingsfile <<	"#------------------------------------------------------------------------------\n";
	settingsfile << "#Linear fitting stuff\n";
	settingsfile << "SLOPE" << DL << inislope_entry->get_text() << DL
											<< fitslope_checkbutton->get_active() << DL
											<< autoslope_checkbutton->get_active() << "\n";
	settingsfile << "CONSTANT" << DL << as_double(iniconst_entry->get_text()) << DL
											<< fitconst_checkbutton->get_active() << DL
											<< autoconst_checkbutton->get_active() << "\n";

	settingsfile <<	"#------------------------------------------------------------------------------\n";
	//2007-03-12 Partly shrink windows stuff
	settingsfile << "SHOW_CLASS_DIVISION_PART" << DL << show_datastuff_cb->get_active() << "\n";
	settingsfile << "SHOW_FITTING_PART" << DL << show_fitting_things_cb->get_active() << "\n";
	//2007-03-15 2d-plot stuff
	Gdk::Color cc; cc = tof_vs_class->get_contour_color();
	settingsfile << "2D_CONTOUR_COLOR" << DL << cc.get_red() << DL << cc.get_green() << DL << cc.get_blue() << "\n";
	settingsfile << "BGR_COLOR_OF_WINDOWS" << DL << background_color_for_windows.get_red() << DL << background_color_for_windows.get_green() << DL << background_color_for_windows.get_blue() << "\n";
	settingsfile << "BGR_ALPHA" << DL << background_alpha << "\n";
	settingsfile << "2D_BIN_INTERVAL" << DL << bin_interval_in_2d_contour << "\n";
	settingsfile << "#initial window positions\n";
	settingsfile << "LOAD_OR_NOT" << DL << load_window_positions_from_settingfile->get_active() << "\n";
	//2007-04-20 Guessing the mass number
	settingsfile << "GUESS_MASS_NUMBER" << DL << inputting->guess_mass_cb->get_active() << "\n";
	settingsfile << "GUESS_CHARGE_STATE" << DL << inputting->guess_charge_cb->get_active() << "\n";

	settingsfile << "CHECK_NEW_VERSION_AT_STARTUP" << DL << check_for_new_version_at_startup->get_active() << "\n";

	settingsfile << save_spinbutton_string(*ref_sn_base_from_sp,"REF_SIGNAL_TO_NOISE_BASELINE_FREQ1");
	settingsfile << save_spinbutton_string(*ref_sn_base_to_sp,  "REF_SIGNAL_TO_NOISE_BASELINE_FREQ2");
	settingsfile << save_spinbutton_string(*ref_sn_reso_from_sp,"REF_SIGNAL_TO_NOISE_RESONANCE_FREQ1");
	settingsfile << save_spinbutton_string(*ref_sn_reso_to_sp,  "REF_SIGNAL_TO_NOISE_RESONANCE_FREQ2");

	settingsfile << save_spinbutton_string(*meas_sn_base_from_sp,"MEAS_SIGNAL_TO_NOISE_BASELINE_FREQ1");
	settingsfile << save_spinbutton_string(*meas_sn_base_to_sp,  "MEAS_SIGNAL_TO_NOISE_BASELINE_FREQ2");
	settingsfile << save_spinbutton_string(*meas_sn_reso_from_sp,"MEAS_SIGNAL_TO_NOISE_RESONANCE_FREQ1");
	settingsfile << save_spinbutton_string(*meas_sn_reso_to_sp,  "MEAS_SIGNAL_TO_NOISE_RESONANCE_FREQ2");


	settingsfile.close();
	return 1;
}
//------------------------------------------------------------------------------
void Window1::saveWindowPositions(const Glib::ustring & filename) {
	ofstream settingsfile;
	settingsfile.open(filename.c_str());
	if (!settingsfile) {
	  Gtk::MessageDialog argh("Window file "+filename +" cant be written!");
	  argh.set_title("Argh!");
	  argh.run();
	  argh.hide();

	}
	char DL = '\t';
	int x,y,w,h; get_position(x,y); get_size(w,h);
	settingsfile << "LAKRITSI_WINDOW" << DL << x << DL << y << DL << w << DL << h <<"\n";
	five_plotareas_window->get_position(x,y); five_plotareas_window->get_size(w,h);
	settingsfile << "WINDOW_OF_FIVE" << DL << x << DL << y << DL << w << DL << h <<"\n";
	yksi_plotarea_window->get_position(x,y); yksi_plotarea_window->get_size(w,h);
	settingsfile << "IONS_PER_X_BUNCHES_WINDOW" << DL << x << DL << y << DL << w << DL << h <<"\n";
	#ifdef HAVE_ONLINE
// 	toinen_yksi_plotarea_window->get_position(x,y); toinen_yksi_plotarea_window->get_size(w,h);
// 	settingsfile << "BUNCHES_PER_CLASS_WINDOW" << DL << x << DL << y << DL << w << DL << h <<"\n";
	#endif
	#ifndef HAVE_ONLINE
	//offline_histogram->get_position(x,y); offline_histogram->get_size(w,h);
	//settingsfile << "OFFLINE_HISTOGRAM_WINDOW" << DL << x << DL << y << DL << w << DL << h <<"\n";
	#endif

	settingsfile.close();
}
//------------------------------------------------------------------------------
void Window1::loadWindowPositions(const Glib::ustring & filename) {
	boost::filesystem::ifstream openedfile;
	string piece,window_name,row;
	openedfile.open(filename.c_str());
 	if (!openedfile)
		return ;


	while (getline(openedfile,row)){
		if (row[0] == '#')
			continue;
		window_name = erota(row,'\t');
		int a,b,c,d;
		piece = erota(row,'\t'); a = as_int(piece);
		piece = erota(row,'\t'); b = as_int(piece);
		piece = erota(row,'\t'); c = as_int(piece);
		piece = erota(row,'\t'); d = as_int(piece);
		if (window_name == "LAKRITSI_WINDOW"){
			move(a,b);resize(c,d);
			continue;
		}
		if (window_name == "WINDOW_OF_FIVE"){
			five_plotareas_window->place_window(a,b,c,d);
			continue;
		}
		if (window_name == "IONS_PER_X_BUNCHES_WINDOW"){
			//ions_per_x_bunches_window->place_window(a,b,c,d);
			yksi_plotarea_window->place_window(a,b,c,d);
			continue;
		}
// 		#ifdef HAVE_ONLINE
// 		if (window_name == "BUNCHES_PER_CLASS_WINDOW"){
			//bunches_per_class_window->place_window(a,b,c,d);
// 			toinen_yksi_plotarea_window->place_window(a,b,c,d);
// 			continue;
// 		}
// 		#endif
		#ifndef HAVE_ONLINE
		if (window_name == "OFFLINE_HISTOGRAM_WINDOW"){
			//offline_histogram->place_window(a,b,c,d);
			continue;
		}
		#endif
	}
	openedfile.close();
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
