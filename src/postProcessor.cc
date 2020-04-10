// generated 2006/1/3 23:30:10 EET by tomero@a84-231-125-139.elisa-laajakaista.fi.(none)
// using glademm V2.6.0
//
// newer (non customized) versions of this file go to postProcessor.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "postProcessor.hh"
//#include "dialog1.hh"
#include "mjonotpp.hh"
#include "loctm.hh"
#include "util.hh"
#include <gtkmm/main.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/dialog.h>
#include <gtkmm/stock.h>
#include <sigc++/sigc++.h>
#include <fstream>
#include <iostream>
#include <cmath>

#include <map>
using namespace std;

#include "postProcessor_init.inc"

void
Post_Processor::create_widget(Post_Processor *& win,
			      const Glib::RefPtr<Gtk::Builder> & builder)
{
    builder->get_widget_derived("postProcessor", win);
}


Post_Processor::Post_Processor(GtkWindow * obj, const Glib::RefPtr<Gtk::Builder> & builder) : Gtk::Window(obj),
    ame_mass_table(local_or_rc_file("nubase2016.txt"))
{
	//std::cout << "blaa\n";
	upper_plot_data1_container.set_data(&upper_plot_data1);
	upper_plot_data2_container.set_data(&upper_plot_data2);

	lower_plot_data1_container.set_data(&lower_plot_data1);
	lower_plot_data2_container.set_data(&lower_plot_data2);
	//shower = &showerr;
	//shower_window = new YksiPlottiWindow(&showerr,"Coma plot");
	//shower_window->show();
	//shower_window->place_window(300,0,500,300);
	//std::cout << "blaa3\n";
	
	init_widgets(builder);
	init_mass_table(&ame_mass_table);

	init_signals();
	//std::cout << "blaa4\n";
	//std::cout << "blaa5\n";
	init();
	present();
	
	shower->add_data(&upper_plot_data1_container);
	shower->add_data(&upper_plot_data2_container);
	shower2->add_data(&lower_plot_data1_container);
	shower2->add_data(&lower_plot_data2_container);
	//std::cout << "blaa2\n";
	
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Post_Processor::~Post_Processor() {
//	cout << "Tuhottiin..\n";
	//delete shower_window;
}
//------------------------------------------------------------------------------
bool Post_Processor::on_delete_event(GdkEventAny* event) {
   	m_signal_close.emit();
	return 1;
}
//------------------------------------------------------------------------------
Post_Processor::type_signal_close Post_Processor::signal_close()
{
	//We plan to kill coma?
  
	// This in case of postprocess-way from within lakritsi
	//return m_signal_close;
  
  	hide();
     
}

//------------------------------------------------------------------------------
//Gets average values of fit parameters of vector <post_process_stuffs>.
//Even frequencies are averaged (note: this is not THE WAY to produce correct
// values.
// Supposedly constant values are got from first element (ie. A)
// Number of scans is TOTAL OF SCANS, NOT AVERAGE

static void
dummy(param_t & a, const param_t & b)
{
    a.v += b.v * b.w();
    a.d += b.w();
}

static void
dummy2(param_t & a)
{
    // a.press = a.press / a.dpress; a.dpress = 1.0/sqrt(a.dpress);
    a.v = a.v / a.d;
    a.d = 1.0 / sqrt(a.d);
}

post_process_stuffs get_average(const std::vector<post_process_stuffs> & vegdor)
{
	post_process_stuffs a;

	std::vector<post_process_stuffs>::const_iterator iter;
	typedef post_process_data::members::const_iterator foo;
	const post_process_data::members tmp = a.get_data_members();

	for (iter=vegdor.begin(); iter != vegdor.end(); ++iter) {
	    // Getting weighted average of each
	    for (foo iter2=tmp.begin(); iter2 != tmp.end(); ++iter2) {
		param_t post_process_data::*member = *iter2;
		dummy(a.*member, *iter.*member);
	    }
	    a.fit_chisq += iter->fit_chisq;	//Just normal average
	    a.scans_number++;
	}
	for (foo iter=tmp.begin(); iter != tmp.end(); ++iter) {
	    param_t post_process_data::*member = *iter;
	    dummy2(a.*member);
	}

	// a.freq = a.freq / a.freq.d; a.freq.d = 1.0/sqrt(a.freq.d);
	a.fit_chisq /= a.scans_number;

	//Things got from first element
	a.A = vegdor.begin()->A; //mass
	a.q = vegdor.begin()->q;
	a.element = vegdor.begin()->element; //element ie. zr97_1
	//Scans

	return a;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Post_Processor::init_mass_table(mass_table* mass_table_i)
{
	ame_mass_table_p = mass_table_i;
//	mass_table_stuff.break_molecule_string("ar34cl36K35");
}

void Post_Processor::init(void){//post_process_stuffs* iStuff) {
    //Window2::create_widget(shower);
    shower = &showerr;
    shower2 = &showerr2;
    cout << "init\n";

    graphs_paned->add1(showerr);
   	graphs_paned->add2(showerr2);
	graphs_paned->set_position(300);
	is_reading_settingfile = false;
	restore_settings("coma.ini");
	std::string build_string = std::string(GIT_VERSION) + ", compiled " + std::string(COMPILE_DATE);
	set_title("C.O.M.A. = Complicated Offline Mass Analyser " + build_string);
	//Nature constants
	u_in_keV = 931494.009;
	m_e = 510.998902;
	e = 1.60217653E-019; //Coulombs
	// nubtab_filename = "nubtab03.asc";
	move_windows_to_initial_positions();
//	resize(1000,400);
	//show_all_children();
	shower->show();
	shower2->show();
	shower->expand_xtics(0.5);
	//shower->set_title("Joo");
//	shower->set_default_output_dir_reference(&(Glib::ustring(iStuff->output_directory)));
// 	ref_file_entry-> set_text(iStuff->ref_out_file);
// 	meas_file_entry->set_text(iStuff->meas_out_file);
// 	dataset_name_entry->set_text(iStuff->post_process_filename);
// 	out_dir_entry->set_text(iStuff->output_directory);
// 	connector = iStuff;
	string	found_element;

	// cout << ame_mass_table_p->get_mass_excess("ar34cl34") << ":\n";
	
	red_color.set("red");
	default_widget_base_color = get_st_normal_bg_color(*out_dir_entry);

	
	// We don't load anything automatically when the program starts.
	/*
	if (!files_exist(true)) {
//		cout <<"naa\n";

	}
	else {
		process_file(REF); //Just to get mass exces..
		bool ok;
		string source_of_value;
//		mass_excess = mass_table_stuff->get_mass_excess(references.begin()->element, dmass_excess, found_element, ok, source_of_value);

//		cout <<"n\n";
//		if ( fabs(dmass_excess + 1.00) < 0.0000001)
		if (!ok) {
			st_normal_bg_color(*me_entry, red_color);
			st_normal_bg_color(*me_err_entry, red_color);
			ame_ok->set_text("("+ found_element + " not found!)");

		}
		else{
			st_normal_bg_color(*me_entry, default_widget_base_color);
			st_normal_bg_color(*me_err_entry, default_widget_base_color);
			//if (found_element == "cc") {
			//	ame_ok->set_text("Carbon cluster");
			//}
			//else {
				ame_ok->set_text("("+source_of_value+" "+found_element+")");
			//}
			ame_element_entry->set_text(found_element);
		}
		print_mass_excess();
//		on_recalc_button_clicked();
	}
	*/
	
	// Is the following needed here?
	// on_use_z_extrapolated_toggled();

	initialize_comboboxes();
	radiobutton_qval->set_active();
//	redraw_what_is_active_now();

	shower->dat.color = Gdk::Color("Black");
	shower->dat.color = Gdk::Color("Blue");
	upper_plot_data1_container.color = Gdk::Color("Black");
	upper_plot_data2_container.color = Gdk::Color("Blue");
	
	radiobutton11->set_active();

//	do_everything();

	freq_ratio_chisq = -1;
	mass_excess_chisq = -1;
//	redraw_what_is_active_now();
	update_gtk();
	resize(1000,400);

	//cout << mass_table_stuff->get_mass_excess_source("cc12") << ":\n";
	//cout << mass_table_stuff->get_mass_excess_source("xe129") << ":\n";
}
//------------------------------------------------------------------------------
void Post_Processor::redraw_what_is_active_now() {
	if (radiobutton11->get_active() )
		on_freq_pressed();
	if (radiobutton10->get_active() )
		on_press_pressed();
	if (radiobutton9->get_active() )
		on_axial_pressed();
	if (radiobutton8->get_active() )
		on_magn_pressed();
	if (radiobutton7->get_active() )
		on_cycl_pressed();
	if (radiobutton6->get_active() )
		on_phase_pressed();
	if (radiobutton5->get_active() )
		on_t_rf_pressed();
	if (radiobutton4->get_active() )
		on_a_rf_pressed();
//	if (radiobutton21->get_active() )
//		on_freq2_pressed();
//	if (radiobutton22->get_active() )
//		on_fraction_pressed();
	if (radiobutton23->get_active() )
		on_other_items_pressed();
	if (radiobutton20->get_active() )
		on_show_dependencies_pressed();
	if (radiobutton_qval->get_active() )
		on_radiobutton_qval_pressed();
	if (radiobutton_mass->get_active() )
		on_radiobutton_mass_pressed();
	if (radiobutton_freq->get_active() )
		on_radiobutton_freq_pressed();

}
//------------------------------------------------------------------------------
void Post_Processor::on_use_z_extrapolated_toggled(void) {
	//
	//ref_class_spinbutton->set_sensitive(!use_z_extrapolated->get_active());
	//meas_class_spinbutton->set_sensitive(!use_z_extrapolated->get_active());
	if (use_z_extrapolated_for_ref->get_active()) {
		ref_class_spinbutton->set_value(0);
	}
	if (use_z_extrapolated_for_meas->get_active()) {

		meas_class_spinbutton->set_value(0);
	}
	ref_class_spinbutton->set_sensitive(!use_z_extrapolated_for_ref->get_active());
	meas_class_spinbutton->set_sensitive(!use_z_extrapolated_for_meas->get_active());

	on_load_files_button_clicked();
}
//------------------------------------------------------------------------------
void Post_Processor::print_pairs(ostream &in_here) {
	if (!files_exist())
		return;
	vector<ref_meas_block>::iterator iter;
	for (iter = pairs.begin(); iter != pairs.end(); ++iter) {
		in_here << "Meas: "  << iter->meas1->scan_number
				<< ", ref1: " << iter->ref1->scan_number
				<< ", ref2: " << iter->ref2->scan_number <<  "\n";
	}
	in_here << "Last ref: " << last_ref->scan_number << ", first ref: " << first_ref->scan_number << "\n";
}
//------------------------------------------------------------------------------
void Post_Processor::print_reverse_pairs(ostream &in_here) {
	if (!files_exist())
		return;
	vector<ref_meas_block>::iterator iter;
	for (iter = reverse_pairs.begin(); iter != reverse_pairs.end(); ++iter) {
		in_here << "Ref1: "  << iter->ref1->scan_number
				<< ", meas1: " << iter->meas1->scan_number
				<< ", meas2: " << iter->meas2->scan_number <<  "\n";
	}
}
//------------------------------------------------------------------------------
bool Post_Processor::calculate_all_sets(void)
{
    if (get_number_of_pairs() <= 0) {
	cout << "There are no suitable pairs to calculate..\n";
	return false;
    }
    return calculate_via_pairs();
}
//------------------------------------------------------------------------------
bool Post_Processor::calculate_via_pairs(void)
{
    double x_weight_sum(0),x_weight_x_sum(0);
    for (vector<ref_meas_block>::iterator iter=pairs.begin(); iter != pairs.end(); ++iter) {
	//cout << "calculating.\n";
	//bfield_fluctuation
	double fluct(0);
	if (PG_mode_cb->get_active() == false) {
	    fluct = get_time_between_references(*(iter->ref1),*(iter->ref2)) *
		bfield_fluct_spinbutton->get_value() * 1.0e-10 * iter->meas1->freq.v;
	} else {
	    fluct = 0;
	}
	iter->meas1->freq.dd = sqrt(pow(iter->meas1->freq.d, 2.0) + pow(fluct, 2.0)); //B-field fluct only to meas
	iter->ref1->freq.dd = iter->ref1->freq.d;
	if (PG_mode_cb->get_active() == false) {
	    iter->ref2->freq.dd = iter->ref1->freq.d; //t�� osoitti iter->ref1->freq.d; korjattu 22.3.2009
	}
	if (PG_mode_cb->get_active() == false) {
	    iter->meas1->interpolated_ref_freq.v = 1.0 * get_interpolated_freq(*(iter->ref1), *(iter->ref2), iter->meas1->time_of_scan);
	    iter->meas1->interpolated_ref_freq.d = 1.0 * get_interpolated_freq_err(*(iter->ref1), *(iter->ref2), iter->meas1->time_of_scan);
	    iter->meas1->freq_ratio.v = 1.0 * iter->meas1->interpolated_ref_freq.v / iter->meas1->freq.v;
	    // Freq ratio error
	    iter->meas1->freq_ratio.d = iter->meas1->freq_ratio.v * sqrt (
		pow(iter->meas1->interpolated_ref_freq.d / iter->meas1->interpolated_ref_freq.v, 2.0) +
		pow(iter->meas1->freq.dd / iter->meas1->freq.v, 2.0));
	} else {
	    iter->meas1->freq_ratio.v = iter->ref1->freq.v / iter->meas1->freq.v;
	    iter->meas1->freq_ratio.d = sqrt(pow(iter->ref1->freq.d / iter->ref1->freq.v, 2.0) + pow(iter->meas1->freq.d / iter->meas1->freq.v, 2.0));
	}
	iter->meas1->freq_ratio.dd = iter->meas1->freq_ratio.d;

	iter->meas1->q_value.v = ((iter->meas1->freq_ratio.v - 1.0) * get_ref_ion_mass());
	iter->meas1->q_value.d = sqrt(
	    pow((iter->meas1->freq_ratio.v - 1.0) * get_ref_ion_mass_err(), 2.0)
	    + pow(get_ref_ion_mass() * iter->meas1->freq_ratio.d, 2.0));
	iter->meas1->m_exc.v = get_ref_ion_mass() * iter->meas1->freq_ratio.v + meas_q_spinbutton->get_value_as_int() * m_e - (get_meas_A() * 1.0) * u_in_keV;
		// Mass excess error (statistical)
	iter->meas1->m_exc.d = 1.0 * sqrt(
	    pow(get_ref_ion_mass_err() * iter->meas1->freq_ratio.v, 2.0)
	    + pow(get_ref_ion_mass() * iter->meas1->freq_ratio.d, 2.0));
	x_weight_sum += w(iter->meas1->freq_ratio.d);
	x_weight_x_sum += w(iter->meas1->freq_ratio.d) * iter->meas1->freq_ratio.v;

	}
	//Statistical weigted average of freq. ratio with B-field fluct
	freq_ratio = x_weight_x_sum / x_weight_sum;
	dfreq_ratio = 1.0/sqrt(x_weight_sum);

	//Getting stat chisq
	freq_ratio_chisq = 0; int nnn(0);
	for (vector<ref_meas_block>::iterator iter=pairs.begin(); iter != pairs.end(); ++iter) {
	    freq_ratio_chisq += pow((iter->meas1->freq_ratio.v - freq_ratio) / (iter->meas1->freq_ratio.d), 2.0);
	    nnn++;
	}
	freq_ratio_chisq /= (nnn*1.0);

	//If chisq > 1, normalize all individual freq ratios with sqrt(chisq) --> ddfreq_ratio individuals
	ddfreq_ratio = dfreq_ratio;
	if (freq_ratio_chisq > 1.0) {
	    ddfreq_ratio = dfreq_ratio * sqrt(freq_ratio_chisq);
	    for (vector<ref_meas_block>::iterator iter=pairs.begin(); iter != pairs.end(); ++iter) {
		iter->meas1->freq_ratio.dd = iter->meas1->freq_ratio.d * sqrt(freq_ratio_chisq);
	    }
	}

	//Birge ratio. ddfreq_ratio is chisq corrected. All individual ddfreq_ratios also.
	int ext_n(0); double ext_yla(0),ext_ala(0);
	for (vector<ref_meas_block>::iterator iter=pairs.begin(); iter != pairs.end(); ++iter) {
	    ext_yla += 1.0/pow(iter->meas1->freq_ratio.dd, 2.0) * pow(iter->meas1->freq_ratio.v - freq_ratio, 2.0);
	    //ext_ala: summing weights
	    ext_ala += 1.0/pow(iter->meas1->freq_ratio.dd, 2.0);
	    ext_n ++;
	}
	inner_error = ddfreq_ratio;
	outer_error = sqrt(ext_yla / (((ext_n-1) * 1.0) * ext_ala));
	// cout << "inner, outer " << inner_error << "  " << outer_error << "\n";
	if (inner_error > outer_error) {
	    dddfreq_ratio = inner_error;
	    used_error = "inner";
	} else {
	    dddfreq_ratio = outer_error;
	    used_error = "outer";
	}

	//From now on, to calculate anything for f-ratio, use freq_ratio and dddfreq_ratio
	//dddfreq_ratio is with b-field fluct, chisq corrected, and largest of inner_error & outer_error
	// but not residual or mass dep. errors

	//----------------------------
	//2009-10-26 ALL uncertainties to freq ratio: stat + mass_dep + residual
	ddddfreq_ratio = freq_ratio * sqrt(
	    pow(dddfreq_ratio / freq_ratio, 2.0)
	    + pow((get_ref_A() - get_meas_A()) * mass_dep_error_spinbutton->get_value() * 1.0e-9, 2.0)
	    + pow(residual_syst_error_spinbutton->get_value() * 1.0e-9, 2.0)
	);

	//
	//This ddddfreq_ratio with residual
	//ddddfreq_ratio = sqrt(pow(dddfreq_ratio, 2.0) + pow(residual_syst_error_spinbutton->get_value() * 1.0e-9*freq_ratio, 2.0));

	q_value = (freq_ratio - 1.0) * get_ref_ion_mass();

	//dq_value (all systematic stuff were already in in ddddfreq_ratio)
	dq_value = sqrt(
	    pow((freq_ratio-1.0) * get_ref_ion_mass_err(), 2.0)
	    + pow(get_ref_ion_mass() * ddddfreq_ratio, 2.0));
	ddq_value = dq_value;
	// dq_value with residual systematic
	//ddq_value = sqrt(pow(dq_value, 2.0) +
	//		pow(residual_syst_error_spinbutton->get_value() * 1.0e-9*freq_ratio *get_ref_ion_mass(), 2.0));

	// mass excess
	mass_excess = get_ref_ion_mass() * freq_ratio + meas_q_spinbutton->get_value_as_int() * m_e -get_meas_A() * u_in_keV;

	dmass_excess = sqrt(
		pow(get_ref_ion_mass() * ddddfreq_ratio, 2.0)
		+ pow(freq_ratio*get_ref_ion_mass_err(), 2.0));

	// dmass_excess with residual syst + mass_dependent err
	//dmass_excess = sqrt(
	//	pow(get_ref_ion_mass() * dddfreq_ratio, 2.0) +
	//	pow(freq_ratio*get_ref_ion_mass_err(), 2.0) +
		//pow(countrate_error_spinbutton->get_value()/pairs.begin()->meas1->freq *meas_ion_mass, 2.0) +
	//	pow(residual_syst_error_spinbutton->get_value() * 1.0e-9*freq_ratio *get_ref_ion_mass(), 2.0) +
	//	pow(mass_dep_error_spinbutton->get_value() * 1.0e-9*get_ref_ion_mass() * (get_meas_A()-get_ref_A()), 2.0)
	//);

	print_results_on_screen();

	return true;
}
//------------------------------------------------------------------------------
void Post_Processor::print_results_on_screen(void) {
	if (!files_exist())
		return;

	putDoubleToEntry(*birge_ratio_entry,outer_error/inner_error);
	putDoubleToEntry(*f_ratio_chisq,freq_ratio_chisq);

	putDoubleToEntry(*f_ratio_result,freq_ratio,7);
	putDoubleToEntry(*inner_error_entry,inner_error*1.0e9,9);
	putDoubleToEntry(*outer_error_entry,outer_error*1.0e9,9);

	//Q-value stuff
	putDoubleToEntry(*q_val_result,q_value);
	putDoubleToEntry(*dq_val_result,ddq_value);

	putDoubleToEntry(*m_exc_result,mass_excess);
	putDoubleToEntry(*dm_exc_result,dmass_excess);
	putDoubleToEntry(*df_ratio_result,ddddfreq_ratio*1e9,9);

}
//------------------------------------------------------------------------------
double Post_Processor::get_ref_ion_mass(void) {
	return ref_a_spinbutton->get_value_as_int() * 1.0*u_in_keV - ref_q_spinbutton->get_value_as_int() *  m_e +
		getDoubleFromEntry(*me_entry);
}
//------------------------------------------------------------------------------
double Post_Processor::get_ref_ion_mass_err(void) {
	return getDoubleFromEntry(*me_err_entry);
}
//------------------------------------------------------------------------------
double Post_Processor::get_ref_A(void) {
	return ref_a_spinbutton->get_value_as_int();
}
//------------------------------------------------------------------------------
double Post_Processor::get_meas_A(void) {
	return meas_a_spinbutton->get_value_as_int();
}
//------------------------------------------------------------------------------
int Post_Processor::assing_reverse_pairs(void) {
	if (!files_exist())
		return -1;
	reverse_pairs.clear();
	std::vector<post_process_stuffs>::iterator meas_iter;
	std::vector<post_process_stuffs>::iterator ref_iter;
	int goods(0);
	//New style: there could be many sets

	ref_meas_block toAdd;
	vector<FIRST_LAST_FREQS>::iterator i = boundaries.begin();
	while (1) {
		if (i== boundaries.end())
			break;
		int pair_number_in_set(0);
//		cout << "setin eka: " << i->first_block->meas1->scan_number << "\n";
//		cout << "setin vika: " << i->last_block->meas1->scan_number << "\n";

		vector<ref_meas_block>::iterator j=i->first_block;
		while (1) {
//			cout << "Tämä meas1 :" << j->meas1->scan_number << "\n";
			pair_number_in_set++;
			if (j == i->last_block)
				break;
			j++;
		}
//		cout << "Pairs in here: " << pair_number_in_set << "\n";
		if (pair_number_in_set < 2) {
			i++;
			continue;
		}
		else {
			i->only_one_reverse_pair_or_less = false;
			for (vector<ref_meas_block>::iterator j=i->first_block; j != i->last_block;++j) {
				toAdd.meas1 = j->meas1;
				toAdd.ref1  = j->ref1;
				j++;
				toAdd.meas2 = j->meas1;
				j--;
				reverse_pairs.push_back(toAdd);
				if (j==i->first_block) {
					i->first_rev_block = (reverse_pairs.end()-1);
//					cout << "GGSGSDGD eka \n";
				}
				{
					j++;
					if (j==i->last_block) {
						i->last_rev_block = (reverse_pairs.end()-1);
						//cout << "GGSGSDGD vika\n";
					}
					j--;
				}
				goods++;
			}
		}
		i++;
	}
	//cout << "Tan se tekee\n";
	for (vector<FIRST_LAST_FREQS>::iterator i=boundaries.begin(); i !=boundaries.end();++i) {
		if (i->only_one_reverse_pair_or_less)
			continue;
//		else
//			cout << "Reversi eka vika ref" << i->first_block->ref1->scan_number << " " << i->last_block->ref1->scan_number << "\n";
	}
//	cout << "Settejä: " << boundaries.size() << "\n";
//	cout << "Reverse pairs: " << goods << "\n";
	return goods;
}

//------------------------------------------------------------------------------
int Post_Processor::assign_nearest_ref_to_meas(void) {
	if (!files_exist())
		return -1;
	pairs.clear();
	if (references.size() < 1 || measurements.size() < 1) {
		Gtk::MessageDialog blaa("no references or measurements..");
		blaa.run();
		blaa.hide();
		return -2;
	}
	std::vector<post_process_stuffs>::iterator meas_iter;
	std::vector<post_process_stuffs>::iterator ref_iter;

	for (meas_iter = measurements.begin(); meas_iter != measurements.end(); ++meas_iter) {
		ref_meas_block toAdd;
		toAdd.meas1 = meas_iter;
		int timediff(0), smaller_timediff(10000000);

		for (ref_iter = references.begin(); ref_iter != references.end(); ++ref_iter) {
			timediff = abs(loc_tm(meas_iter->time_of_scan) - loc_tm(ref_iter->time_of_scan));
			if (timediff < smaller_timediff) {
				smaller_timediff = timediff;
				toAdd.ref1 = ref_iter;
			}
		}
		pairs.push_back(toAdd);
	}
	//cout << "referej� " << references.size() << ", measureja " << measurements.size()
	//	<< ", paireja " << pairs.size() << "\n";
	putIntegerToEntry(*good_ratios,pairs.size());

	return pairs.size();
}
//------------------------------------------------------------------------------
int Post_Processor::assing_pairs(void) {
	if (!files_exist())
		return -1;
	pairs.clear();
	//cout << "measurements.size() " << measurements.size() << "\n";
	//cout << "references.size() " << references.size() << "\n";
	if (references.size() < 2) {
		Gtk::MessageDialog blaa("There is only 1 reference or less!\nWe want more!\n");
		blaa.set_title("What could be the wrong?");
		blaa.run();
		blaa.hide();
		return -1;
	}
	std::vector<post_process_stuffs>::iterator meas_iter;
	std::vector<post_process_stuffs>::iterator ref_iter;
	bool ref1_ok(0),ref2_ok(0);

	for (meas_iter = measurements.begin(); meas_iter != measurements.end(); ++meas_iter) {
		ref_meas_block toAdd;
		ref1_ok=0;ref2_ok=0;
		int time1(100000), time2(-100000);
//		cout << "time " << meas_iter->time_of_scan << "\n\n\n";
		for (ref_iter = references.begin(); ref_iter != references.end(); ++ref_iter) {
			int temptime = time_t(loc_tm(meas_iter->time_of_scan)) - time_t(loc_tm(ref_iter->time_of_scan)) ;
//			cout << "Time " << temptime << "\n";
			if (temptime > 0) { //If ref comes before meas, fTRUE
				if (temptime < time1) {  //If ref is closer to meas then TRUE
					time1= temptime;
					toAdd.ref1=ref_iter;
					ref1_ok=true;

				}
			}
			else { //If ref comes after meas, DO THIS
				if (temptime > time2) {
					time2=temptime;
//					cout << "index: " << ref_iter->scan_number << ", temptime " << temptime << ", time2:" << time2 <<"\n";
					toAdd.ref2=ref_iter;
					ref2_ok=true;
					}
			}
		}
		if (ref1_ok && ref2_ok) {
			toAdd.meas1=meas_iter;
			//cout << "meas time: " <<  toAdd.meas1->time_of_scan << "\n";
			pairs.push_back(toAdd);
		}
	}
	if (pairs.size() == 0) {
		Gtk::MessageDialog blaa("There are no references to suit the measurement!\nNo Q-values et al., calculated.\n");
		blaa.run();
		blaa.hide();
		return -1;
	}
//	cout << "ggg\n";
	std::vector<ref_meas_block>::iterator i;
	std::vector<ref_meas_block>::iterator eka,vika;
	eka = pairs.begin(); vika=pairs.begin(); //In this stage, measurements, but will be converted
	for (i = pairs.begin(); i != pairs.end(); ++i) {  //Getting last and first reference
		if (time_t(loc_tm(i->meas1->time_of_scan)) < time_t(loc_tm((eka)->meas1->time_of_scan))) {
			eka = i;			//cout << "joo\n";
		}
		if (time_t(loc_tm(i->meas1->time_of_scan)) > time_t(loc_tm((vika)->meas1->time_of_scan))) {
			vika = i;
		}
	}
//	cout << "ddd\n";
	//Now set from ref_meas_block
	first_ref = eka->ref1; first_meas = eka->meas1;
	last_ref = vika->ref2; last_meas = vika->meas1;


	//Adding b-field fluctuation to each frequency
	//2007-09-24 STOP!!!! Not to do that!
	//double b_field_fluct = bfield_fluct_spinbutton->get_value();
	for (i = pairs.begin(); i != pairs.end(); ++i) {  //Getting last and first reference
		i->meas1->freq.dd = i->meas1->freq.d;
		i->ref1->freq.dd = i->ref1->freq.d;
		i->ref2->freq.dd = i->ref2->freq.d;

	}
//	cout << "REfs ok " << ref1_ok << ref2_ok << "\n\n";
//	cout << "Index of meas: " << meas_iter->scan_number << ", ref1: " << (ref1)->scan_number << ", ref2: " << (ref2)->scan_number <<"\n";
	putIntegerToEntry(*good_ratios,pairs.size());
	return pairs.size();
}
//------------------------------------------------------------------------------
void Post_Processor::print_mass_excess(void) {
	putDoubleToEntry(*me_entry, mass_excess);
	putDoubleToEntry(*me_err_entry, dmass_excess);
}
//------------------------------------------------------------------------------
/*double Post_Processor::get_mass_excess(const string &messy_element_name, double &me_error, string &found_element, bool &ok, string &source_of_value) {
	boost::filesystem::ifstream file;
	file.open(nubtab_filename.c_str());
	if (!file)
		return -2;

	//Get element name from the mess that looks like: zr97_10
	string nice_element(messy_element_name),piece,piece2;
	if (messy_element_name == "") {
		found_element = "";
		me_error = -1;
		ok=false;
		return -1;
	}
	piece = erota(nice_element,'_'); //piece == zr97
	if (piece == "") {
		found_element = "";
		me_error = -1;
		ok=false;
		return -1;
	}
	int point1 = piece.find_first_of("0123456789");
	int point2 = piece.find_last_of("0123456789");
	string number_part ;
	try{
		number_part =piece.substr(point1,point2-point1+1);
//		cout << "numb: " << number_part  <<"\n";
	}
	catch (...) {
		number_part = "";
	}
	point1 = piece.find_first_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
	point2 = piece.find_last_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
	string element_part;//=piece.substr(0,point);
	try{
		element_part =piece.substr(point1,point2-point1+1);
		element_part[0]=(toupper(element_part[0]));
//		cout << "elem: " << element_part  <<"\n";
	}
	catch (...) {
		element_part = "";
	}


	nice_element=number_part+element_part;  // 97zr
	//If it is ie "11cc" then it is carbon cluster with mass 11*12=132
	if (element_part == "Cc") {
		//cout << "It is cc!!\n";
		nice_element = element_part;
		ok = true;
		found_element = nice_element;
		me_error = 0.0;
		source_of_value = "carbon cluster";
		return 0.0;
	}
//	cout << "nice1 '" << nice_element << "'\n";
	//End OF ELEMENT CONVERSION

	string row, me_string, me_err_string;
	double me;

	//Next: try to find reference from file "user_masses.txt" before everything
	boost::filesystem::ifstream refs_file;
	refs_file.open("user_masses.txt");
	if (!refs_file) {
		Gtk::MessageDialog dialooki("Could not find \"user_masses.txt\"\n\nYou can add your own reference mass excesses there. It uses same data format as \"ref_masses.txt\".\n\nYou should create at least an empty file named \"user_masses.txt\" to get rid of this message.",false,Gtk::MESSAGE_ERROR);
		dialooki.set_title("What could be the wrong?");
		dialooki.run();
	}
	while (getline(refs_file,row)) {
		if (row[0] == '#' || row == "")
			continue;
		piece = row.substr(0,row.find_first_of('\t'));
		//cout << "piece: \"" << piece << "\"\n";
		if (piece == nice_element) {
			piece2 = row.substr(row.find_first_of('\t')+1);
			//cout << "piece2: \"" << piece2 << "\"\n";
			me = as_double(piece2.substr(0,piece2.find_first_of('\t')));
			me_error = as_double(piece2.substr(piece2.find_first_of('\t')+1));
			//cout << "me, me_err " << me << " " << me_error << "\n";
			refs_file.close();
			found_element = nice_element;
			source_of_value = "user_masses.txt";
			ok=true;
			return me;
		}
	}
	refs_file.close();


	//Next: try to find reference from file "ref_masses.txt" first before AME
	boost::filesystem::ifstream refs2_file;
	refs2_file.open(local_or_rc_file("ref_masses.txt"));
	if (!refs2_file) {
		Gtk::MessageDialog dialooki("Could not find \"ref_masses.txt\"\n\nYou can download it from http://research.jyu.fi/igisol/trap/lakritsi/",false,Gtk::MESSAGE_ERROR);
		dialooki.set_title("What could be the wrong?");
		dialooki.run();
	}
	while (getline(refs2_file,row)) {
		if (row[0] == '#' || row == "")
			continue;
		piece = row.substr(0,row.find_first_of('\t'));
		//cout << "piece: \"" << piece << "\"\n";
		if (piece == nice_element) {
			piece2 = row.substr(row.find_first_of('\t')+1);
			//cout << "piece2: \"" << piece2 << "\"\n";
			me = as_double(piece2.substr(0,piece2.find_first_of('\t')));
			me_error = as_double(piece2.substr(piece2.find_first_of('\t')+1));
			//cout << "me, me_err " << me << " " << me_error << "\n";
			refs2_file.close();
			found_element = nice_element;
			source_of_value = "ref_masses.txt";
			ok=true;
			return me;
		}
	}
	refs2_file.close();


	//NEXT: Finding it from AME tables
	int point;
	while (getline(file,row)) {
		piece = row.substr(11,6); //Element part of nubtab03 table -> '97Zr  '
		piece2 = erota(piece,' '); //Getting rid of whitespaces -> '97Zr'
//		cout << " piece2 '" << piece2 << "'\n";
		if (piece2 == nice_element) {
			//cout << "We have a match!\n";
			piece = row.substr(18,11); //Getting me-value
			point = piece.find_first_not_of(" "); piece = piece.substr(point); //Deleting leading whitespace
			point = piece.find_first_of(" "); me_string = piece.substr(0,point); //Deleting following whitespace

			piece = row.substr(29,9); //Getting me-err-value
			point = piece.find_first_not_of(" "); piece = piece.substr(point); //Deleting leading whitespace
			point = piece.find_first_of(" "); me_err_string = piece.substr(0,point); //Deleting following whitespace
//			cout << " piece '" << piece << "'\n";
			break;
		}
	}
	file.close();
	if (me_string == "") {
		cout << "Could not find matching element! Tried this: '" << nice_element << "'\n";
		found_element = nice_element;
		me_error = -1;
		me = -1;
		ok=false;
	}
	else {
		bool succesful = luvuksi(me_string, me);
		luvuksi(me_err_string, me_error);
		if (!succesful) {
			me = -2;
			ok = false;
		}
		else {
			ok=true;
			source_of_value = "AME03";
		}
	}
	found_element = nice_element;
	return me;
}
*/
//------------------------------------------------------------------------------
bool Post_Processor::get_index(WHICH_FILE ref_or_meas, int index_number, std::vector<post_process_stuffs>::iterator &index_iter) {
	if (!files_exist())
		return false;
	std::vector<post_process_stuffs>::iterator iter;
	std::vector<post_process_stuffs> *vegdor;
	if (ref_or_meas == REF)
		vegdor = &references;
	else
		vegdor = &measurements;
	for (iter=vegdor->begin();iter != vegdor->end();++iter) {
		if (iter->scan_number == index_number) {
			index_iter = iter;
			return true;
		}

	}
	return false; //So couldn't find a match
}
//------------------------------------------------------------------------------
bool Post_Processor::process_file(WHICH_FILE ref_or_meas, std::string overridden_filename)
{
	vector<std::string> filenames;
	if (!files_exist())
		return false;
	boost::filesystem::ifstream file;
	int class_no;
	clear_containers(ref_or_meas);
	string filename;
	if (ref_or_meas == REF) {
		filenames = get_filenames_from_entry(*ref_file_entry);
		//filename += ref_file_entry->get_text();
		class_no = ref_class_spinbutton->get_value_as_int();
	}
	else {
		filenames = get_filenames_from_entry(*meas_file_entry);
		//filename += meas_file_entry->get_text();
		class_no = meas_class_spinbutton->get_value_as_int();
	}
	int max_classes(0);
	boost::filesystem::path base_path(out_dir_entry->get_text().c_str());
	for (vector<std::string>::iterator  i = filenames.begin(); i != filenames.end(); ++i) {
		boost::filesystem::path full_file_path = base_path;
		full_file_path /= *i;
		// filename = out_dir_entry->get_text();
		// filename += *i;
		file.open(full_file_path.c_str());
		if (!file)
			return false;

	//	cout << "class no: " << class_no << "\n";
		string row;
		int line_in_block(0);
		max_classes = 0;
		while (getline(file,row)) {
			if (row[0] == '#')
				continue;
			if (row == "" || row.size() < 5) {
				if (line_in_block > max_classes)
					max_classes = line_in_block;
				line_in_block = 0;
				continue;
			}
			if (line_in_block == class_no) {
				set_data_line(row,ref_or_meas);
			}
			line_in_block++;
		}


		file.close();
	}

//	ave = get_average(stuufs);
	if (ref_or_meas == REF) {
		ref = get_average(references);
		ref_class_spinbutton->set_range(0,max_classes-1);
		set_on_screen(ref,ref_or_meas);
	}
	else {
		meas = get_average(measurements);
		meas_class_spinbutton->set_range(0,max_classes-1);
		set_on_screen(meas,ref_or_meas);
	}

	sort(references.begin(),references.end());
	sort(measurements.begin(),measurements.end());
	return true;
}
//------------------------------------------------------------------------------
void Post_Processor::set_on_screen(const post_process_stuffs &tama,WHICH_FILE ref_or_meas) {
	if (ref_or_meas == MEAS) {
		meas_a_spinbutton->set_value(tama.A);
		meas_q_spinbutton->set_value(tama.q);
		putIntegerToEntry(*meas_scans_entry,tama.scans_number);


	}
	else {
		ref_a_spinbutton->set_value(tama.A);
		ref_q_spinbutton->set_value(tama.q);
		putIntegerToEntry(*ref_scans_entry,tama.scans_number);
	}

}
//------------------------------------------------------------------------------

void Post_Processor::clear_containers(WHICH_FILE ref_or_meas) {
	if (ref_or_meas == REF)
		references.clear();
	if (ref_or_meas == MEAS)
		measurements.clear();
}
//------------------------------------------------------------------------------
bool Post_Processor::set_data_line(const std::string &row, WHICH_FILE ref_or_meas)
{
	post_process_stuffs toAdd;
	toAdd.freq2.v = -2;
	toAdd.freq2.d = -5; //For failsafe, it those do not exist in file
	toAdd.fraction.v = -2; toAdd.fraction.d = -5;
	toAdd.z_slope.v = -2;  toAdd.z_slope.d = -5;
	toAdd.z_const.v = -2;  toAdd.z_const.d = -5;
	toAdd.q = 1;
	string piece;
	string rivi=row;
	double number;

	double reso = 0.0;
	double reso_err = 0.0;
	double reso_with_Z = 0.0;
	double reso_with_Z_err = 0.0;
	bool use_z_if_available;
	if (ref_or_meas == REF)
		use_z_if_available = use_z_extrapolated_for_ref->get_active();
	else
		use_z_if_available = use_z_extrapolated_for_meas->get_active();

	//cout << "URRRRGSH \n";
	for (int i=1;i<70;i++) {

		piece = erota(rivi,'\t',false);
		//cout << "i: " << i << " content \"" << piece << "\"\n"; //DEBUG purposes
		if (piece == "") {
			//cout << "i: " << i << " content \"" << piece << "\" OHITETTIIN \n"; //DEBUG purposes
			continue;		//Putting as string
		}
		if (i==17) {toAdd.time_of_scan = piece; continue;}
		if (i==1) {toAdd.element = piece; continue;}

		//REST OF THOSE ONLY AS NUMBERS!!
		//If there is something that is not number will be skipped
		if (piece.find_first_not_of("01234567890-") == 0) {
			//cout << "Ohitettiin kentt� " << i << ", joka sisalsi " << piece << "\n";
			continue;
		}
		if ((i >= 6) && (i <= 8)) {
			//cout << "Ohitettiin kentt� " << i << ", joka sisalsi " << piece << "\n";
			continue;
		}

		// Putting as int
		if (i==2) { int num; luvuksi(piece,num);	toAdd.scan_number= num;	}
		if (i==35) { int num; luvuksi(piece,num);	toAdd.A= num;	}
		if (i==47) { int num; luvuksi(piece,num);	toAdd.q = num;	}
		if (i==53) { int num; luvuksi(piece,num);	toAdd.laku_versio = num;}

		// Putting as double
		luvuksi(piece,number);
		if (i==11) {	reso = number;		} //freq of single class
		if (i==12) {	reso_err = number;	} //dfreq of single class
		if (i==14) {	reso_with_Z = number;		} //Z-classed freq
		if (i==15) {	reso_with_Z_err = number;	} //Z-classed dfreq


		if (i==18) {	toAdd.t_rf.v = number;		}
		if (i==19) {	toAdd.t_rf.d = number;	}
		if (i==20) {	toAdd.press.v = number;	}
		if (i==21) {	toAdd.press.d = number;	}
		if (i==22) {	toAdd.axial.v = number;	}
		if (i==23) {	toAdd.axial.d = number;	}
		if (i==24) {	toAdd.magn.v = number;		}
		if (i==25) {	toAdd.magn.d = number;	}
		if (i==26) {	toAdd.cycl.v = number;		}
		if (i==27) {	toAdd.cycl.d = number;	}
		if (i==28) {	toAdd.phase.v = number;	}
		if (i==29) {	toAdd.phase.d = number;	}
		if (i==30) {	toAdd.a_rf.v = number;		}
		if (i==31) {	toAdd.a_rf.d = number;	}
		if (i==40) {	toAdd.freq2.v = number;	}
		if (i==41) {	toAdd.freq2.d = number;	}
		if (i==42) {	toAdd.fraction.v = number;	}
		if (i==43) {	toAdd.fraction.d = number;	}

		if (i==36) {	toAdd.z_slope.v = number;	}
		if (i==37) {	toAdd.z_slope.d = number;	}
		if (i==38) {	toAdd.z_const.v = number;	}
		if (i==39) {	toAdd.z_const.d = number;	}

		if (i==5) { toAdd.ions_per_shot = number;}
		if (i==9) { toAdd.counts_in_class = number;}
		if (i==10)  {toAdd.counts_in_all_classes = number;}

		if (i==16) { toAdd.fit_chisq = number;}



	}
	toAdd.freq.v = reso;
	if (ref_or_meas == REF) {
		toAdd.freq.d = reso_err * ref_nonZ_err_multiplier->get_value();
//		cout << "daa\n";
	}
	if (ref_or_meas == MEAS) {
		toAdd.freq.d = reso_err * meas_nonZ_err_multiplier->get_value();
	}

	//If z-class wanted and it really was used:
	if (use_z_if_available && reso_with_Z > 0) {
		toAdd.freq.v = reso_with_Z;
		toAdd.freq.d = reso_with_Z_err;
	}


	if (ref_or_meas == REF) {
		bool really_add = true;
		//cout << "ref scan number " << toAdd.scan_number  << "\n";
		if (toAdd.scan_number % 2 == 1 && ref_comboboxtext.get_active_text() == "REFS: evens") {
			really_add = false;
		}
		if (toAdd.scan_number % 2 == 0 && ref_comboboxtext.get_active_text() == "REFS: odds") {
			really_add = false;
		}
		if (really_add) {
			references.push_back(toAdd);
			//cout << "this ref was added\n";
		}
	}
	else {
		bool really_add = true;
		//cout << "meas scan number " << toAdd.scan_number  << "\n";
		if (toAdd.scan_number % 2 == 1 && meas_comboboxtext.get_active_text() == "MEAS: evens") {
			really_add = false;
		}
		if (toAdd.scan_number % 2 == 0 && meas_comboboxtext.get_active_text() == "MEAS: odds") {
			really_add = false;
		}
		if (really_add) {
			measurements.push_back(toAdd);
			//cout << "this meas was added\n";
		}
	}
	return true;
}
//------------------------------------------------------------------------------
bool Post_Processor::files_exist(bool popup_window_if_not_existing) {
	boost::filesystem::ifstream testi;
	string reffile,error_out;

	vector<std::string> files_to_check = get_filenames_from_entry(*ref_file_entry);
	if (files_to_check.size() == 0) {
		if (popup_window_if_not_existing) {
			Gtk::MessageDialog dialooki("No reference files defined..\n",false,Gtk::MESSAGE_ERROR);
			dialooki.set_title("What could be the wrong?");
			dialooki.run();
		}
		return false;
	}
	bool some_does_not_exist(false);

	boost::filesystem::path base_path(out_dir_entry->get_text().c_str());
	for (vector<std::string>::iterator i=files_to_check.begin(); i != files_to_check.end(); ++i) {
		boost::filesystem::path full_file_path = base_path;
		full_file_path /= *i;

		// reffile = out_dir_entry->get_text(); reffile += *i;
		//testi.open(reffile.c_str());
		if (!boost::filesystem::exists(full_file_path))
		{
			error_out = "Reference: " + *i + "\n";
			some_does_not_exist = true;
		}
		//testi.close();
	}

	files_to_check = get_filenames_from_entry(*meas_file_entry);
	if (files_to_check.size() == 0) {
		if (popup_window_if_not_existing) {
			Gtk::MessageDialog dialooki("No measurement files defined..\n",false,Gtk::MESSAGE_ERROR);
			dialooki.set_title("What could be the wrong?");
			dialooki.run();
		}
		return false;
	}

	for (vector<std::string>::iterator i=files_to_check.begin(); i != files_to_check.end(); ++i) {
		boost::filesystem::path full_file_path = base_path;
		full_file_path /= *i;

		//reffile = out_dir_entry->get_text(); reffile += *i;
		//testi.open(reffile.c_str());
		if (!boost::filesystem::exists(full_file_path))
		{
			error_out = "Measurement: " + *i + "\n";
			some_does_not_exist = true;
		}
		testi.close();
	}

	if (some_does_not_exist && popup_window_if_not_existing) {
		if (error_out != "") {
			Gtk::MessageDialog dialooki("These file(s) could not be opened:\n\n"+error_out,false,Gtk::MESSAGE_ERROR);
			dialooki.set_title("What could be the wrong?");
			dialooki.run();
		}
	}
	return !some_does_not_exist;
}
//------------------------------------------------------------------------------
void Post_Processor::on_class_spinbutton_value_changed() {
	if (!files_exist())
		return;
	process_file(REF);
	process_file(MEAS);
	on_recalc_button_clicked();
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool Post_Processor::on_me_entry_key_press_event(GdkEventKey *ev) {
	ame_ok->set_text("(Set manually)");
//	cout << "Yes, was changed\n";
	return false;
}
//------------------------------------------------------------------------------
void Post_Processor::on_recalc_button_clicked() {
	if (is_reading_settingfile)
		return;
	if (!files_exist())
		return;
	// std::cout << "taalla\n";
//	process_file(REF); process_file(MEAS);
	//if ((assing_pairs() > 0) && (assing_reverse_pairs() >0)) {
	if ((assing_pairs() > 0 && (PG_mode_cb->get_active() == false))) {
			putIntegerToEntry(*pairs_entry, assing_pairs());
	}
	if (PG_mode_cb->get_active()) {
		putIntegerToEntry(*pairs_entry, assign_nearest_ref_to_meas());
	}
		//putIntegerToEntry(*rev_pairs_entry, assing_reverse_pairs());
		//calculate_lots();
	calculate_all_sets(); //CORRECTED 2007-09-25
	// std::cout << "taalla2\n";

	calculate_all_sets();
	// std::cout << "taalla3\n";
	redraw_what_is_active_now();
	// std::cout << "taalla4\n";
}
//------------------------------------------------------------------------------
void Post_Processor::on_get_ame_button_clicked() {
	string found_element; bool ok;
	string source_of_value;
	if (ame_element_entry->get_text() == "")
		return;
//	mass_excess = ame_mass_table_p->get_mass_excess(ame_element_entry->get_text(), dmass_excess, found_element, ok, source_of_value);
	try {
	  mass_excess = ame_mass_table_p->get_mass_excess(ame_element_entry->get_text());
	  dmass_excess = ame_mass_table_p->get_mass_excess_error(ame_element_entry->get_text());
	  source_of_value = ame_mass_table_p->get_mass_excess_source(ame_element_entry->get_text());
	  found_element = ame_mass_table_p->get_molecule_part(ame_element_entry->get_text());
	ok = true;
	}
	catch (...) {
	  ok = false;
	  mass_excess = 9999999;
	  dmass_excess = 999999;
	}
	  print_mass_excess();
//	if (fabs(dmass_excess + 1.00) < 0.0000001)
	if (!ok) {
		ame_ok->set_text("(" + found_element + " not found!)");
		st_normal_bg_color(*me_entry, red_color);
		st_normal_bg_color(*me_err_entry, red_color);
	}
	else {
		//if (found_element == "Cc") {
		//	ame_ok->set_text("Carbon cluster");
		//}
		//else {
			ame_ok->set_text("("+source_of_value+" "+found_element+")");
			//ame_ok->set_text("(AME03,"+found_element+")");
		//}
		st_normal_bg_color(*me_entry, default_widget_base_color);
		st_normal_bg_color(*me_err_entry, default_widget_base_color);
		on_recalc_button_clicked();
	}
}
//------------------------------------------------------------------------------
void Post_Processor::on_export_button_clicked() {
	//if (!files_exist())
	//	return;
	if (pairs.size()==0) {
		Gtk::MessageDialog dialooki("No data to export!",false,Gtk::MESSAGE_ERROR);
		dialooki.set_title("What could be the wrong?");
		dialooki.run();
		return;
	}
	//output_individual_measurements(string(out_dir_entry->get_text() + "freq_ratios.txt"));
	output_individual_measurements(string(out_dir_entry->get_text() + fratio_filename_entry->get_text()));
//	output_gnuplot_help_stuff(string(out_dir_entry->get_text() + aver_filename_entry->get_text()));
	boost::filesystem::ofstream out;
	string filename	= out_dir_entry->get_text(); filename += export_filename->get_text();
	//out.open(filename.c_str(), ios::app);
	if (!open_file_for_appending_and_popup_message_if_error(filename,out)) {
		cout << "Error crash kaboom\n";
		return;
	}
	out.precision(14);
	char DL = '\t';
	out << dataset_name_entry->get_text() << DL;
	out << bfield_fluct_spinbutton->get_value() << DL;
	out << freq_ratio << DL;
	out << ddddfreq_ratio << DL;
	out << freq_ratio_chisq << DL;
	out << q_value << DL;
	out << ddq_value << DL;
	out << q_value_chisq << DL;
	out << mass_excess << DL;
	out << dmass_excess << DL;
	out << mass_excess_chisq << DL;
	out << get_ref_A() << DL;
	out << get_meas_A() << DL;
	out << me_entry->get_text() << DL;
	out << me_err_entry->get_text() << DL;
	out << pairs.size() << DL;
	//out << countrate_error_spinbutton->get_value() << DL;
	out << residual_syst_error_spinbutton->get_value() << DL;
	out << mass_dep_error_spinbutton->get_value() << DL;
	out << outer_error << DL;
	out << birge_ratio_entry->get_text() << DL;
	out << used_error << DL;
	out << ref_q_spinbutton->get_value_as_int() << DL;
	out << meas_q_spinbutton->get_value_as_int() << DL;
	out <<	"\n";
	out.close();
}
//------------------------------------------------------------------------------
void Post_Processor::output_individual_measurements(const string &filename) {
	boost::filesystem::ofstream out;
	//out.open(filename.c_str(), ios::app);
	if (!open_file_for_appending_and_popup_message_if_error(filename,out)) {
//	if (!out) {
		cout << "Error crash kaboom(freq ratios et al)\n";
		return;
	}
	char DL='\t';
	out.precision(14);
	loc_tm time_now(0);
	out << "#-----------Outputted: " << time_now.get_time_as_string() << " ----------------\n";
	out << "#Directory used: " << out_dir_entry->get_text() << "\n";
	out << "#Reference file: " << ref_file_entry->get_text() << "\n";
	out << "#Measurement file: " << meas_file_entry->get_text() << "\n";
	vector<ref_meas_block>::iterator i;
	for (i = pairs.begin(); i != pairs.end(); ++i) {
		out << i->meas1->time_of_scan << DL;
		out << i->meas1->freq_ratio.v << DL;
		out << i->meas1->freq_ratio.d << DL;
		out << i->meas1->q_value.v << DL;
		out << i->meas1->q_value.d << DL;
		out << i->meas1->m_exc.v << DL;
		out << i->meas1->m_exc.d << DL;
		//out << countrate_error_spinbutton->get_value() << DL;
		out << residual_syst_error_spinbutton->get_value() << DL;
		out << mass_dep_error_spinbutton->get_value() << DL;
		out << bfield_fluct_spinbutton->get_value() << DL;
		//Add time between references in minutes
		if (!PG_mode_cb->get_active())
			out << get_time_between_references(*(i->ref1),*(i->ref2)) << DL;
		else
			out << "-1" << DL;
		out << i->meas1->laku_versio << DL;
		out << revision;
		out << "\n";
	}
	out << "\n";
	out.close();
}
//------------------------------------------------------------------------------
#if 0
void Post_Processor::output_gnuplot_help_stuff(const string &filename) {
	ofstream out;
	if ((!open_file_for_overwriting_and_popup_message_if_error(filename, out)))
		return;
/*	out.open(filename.c_str());
	if (!out) {
		cout << "Error crash kaboom (gnuplot help stuff)\n";
		return;
	}	*/
	out.precision(14);
	//Freq ratio
	out << "#" << dataset_name_entry->get_text() << " = " << freq_ratio << "; ";
	out << "d" << dataset_name_entry->get_text() << " = " << dfreq_ratio << "; ";
	out << "chi" << dataset_name_entry->get_text() << " = " << freq_ratio_chisq << " #";
	out << "Frequency ratio\n";
	//Q-value
	out << " " << dataset_name_entry->get_text() << " = " << q_value << "; ";
	out << "d" << dataset_name_entry->get_text() << " = " << dq_value << "; ";
	out << "chi" << dataset_name_entry->get_text() << " = " << q_value_chisq << " #";
	out << "Q-value\n";
	//Mass excess
	out << "#" << dataset_name_entry->get_text() << " = " << mass_excess << "; ";
	out << "d" << dataset_name_entry->get_text() << " = " << dmass_excess << "; ";
	out << "chi" << dataset_name_entry->get_text() << " = " << mass_excess_chisq << " #";
	out << "Mass excess\n\n";

	//labels
	out.precision(8);
	out << "# LABELS FOR Q-VALUES\n";
	out << "set label 01 \"" << q_value << " {/Symbol \\261} ";
	out.precision(4);
	out << dq_value <<"\" at first \"" << first_meas->time_of_scan << "\", graph 0.1\n";
	out.precision(4);
	out << "set label 02 \"chisq = "<< q_value_chisq << "\" at first \"" << first_meas->time_of_scan << "\", graph 0.05\n";
	out.precision(14);
	//average line
	out << "set arrow 01";
	out << " from first \"" << first_meas->time_of_scan << "\", first " << dataset_name_entry->get_text();
	out << " to first \""   <<  last_meas->time_of_scan << "\", first " << dataset_name_entry->get_text();
	out << " nohead lt 2 lw 2\n";
	//average-daverage line
	out << "set arrow 02";
	out << " from first \"" << first_meas->time_of_scan << "\", first " << dataset_name_entry->get_text() << "-d" << dataset_name_entry->get_text();
	out << " to first \""   <<  last_meas->time_of_scan << "\", first " << dataset_name_entry->get_text() << "-d" << dataset_name_entry->get_text();
	out << " nohead lt 2 lw 1\n";
	//average+daverage line
	out << "set arrow 03";
	out << " from first \"" << first_meas->time_of_scan << "\", first " << dataset_name_entry->get_text() << "+d" << dataset_name_entry->get_text();
	out << " to first \""   <<  last_meas->time_of_scan << "\", first " << dataset_name_entry->get_text() << "+d" << dataset_name_entry->get_text();
	out << " nohead lt 2 lw 1\n\n";
	//Plot line
	out << " p \"" << fratio_filename_entry->get_text() << "\" u 1:5:6 t '" << dataset_name_entry->get_text() << "' pt 4\n";
	out.close();
}
#endif
//------------------------------------------------------------------------------
void Post_Processor::on_radiobutton_qval_pressed()
{
	if (!files_exist())
		return;
	DATASET daatta;
	daatta.data_name="Q-value";
	shower2->set_plot_title(daatta.data_name);
	daatta.graph_type=YERR;
	Datapoint toAdd;
	double start_time(0);
	for (vector<ref_meas_block>::iterator i= pairs.begin(); i !=pairs.end();++i) {
		if (i== pairs.begin()) {
			start_time = time_t(loc_tm(i->meas1->time_of_scan));
		}
		toAdd.x_value = (time_t(loc_tm(i->meas1->time_of_scan)) -start_time)/60.0;
		toAdd.y_value = i->meas1->q_value.v;
		toAdd.y_value_err = i->meas1->q_value.d;
		daatta.data.push_back(toAdd);
	}

	//lower_plot_frame->set_label(shower2->get_plot_title());
	//shower2->set_frame_labeli(lower_plot_frame);

	shower2->nullify(); daatta.color = Gdk::Color("Blue");
	shower2->add_ybar(q_value);shower2->add_ybar(q_value-ddq_value);shower2->add_ybar(q_value+ddq_value);
	shower2->show_ybars(true);
	shower2->set_xtics(2); shower2->set_mxtics(5);

	//shower2->input_data(daatta);
	lower_plot_data1 = daatta.data;
	lower_plot_data1_container.color = daatta.color;
	lower_plot_data1_container.graph_type = YERR;
	
	shower2->autosetPlottingBox();
	shower2->show_data(true);
	shower2->show_contents();
}
//------------------------------------------------------------------------------
void Post_Processor::on_radiobutton_mass_pressed()
{
	if (!files_exist())
		return;
	DATASET daatta;
	daatta.data_name="mass excess";
	shower2->set_plot_title(daatta.data_name);
	daatta.graph_type=YERR;
	Datapoint toAdd;
	double start_time(0);
	for (vector<ref_meas_block>::iterator i= pairs.begin(); i !=pairs.end();++i) {
		if (i== pairs.begin()) {
			start_time = time_t(loc_tm(i->meas1->time_of_scan));
		}
		toAdd.x_value = (time_t(loc_tm(i->meas1->time_of_scan)) -start_time)/60.0;
		toAdd.y_value = i->meas1->m_exc.v;
//		cout << "me: " <<i->meas1->m_exc << " err: " <<i->meas1->dm_exc << "\n";
		toAdd.y_value_err = i->meas1->m_exc.d;
		daatta.data.push_back(toAdd);
	}

	//lower_plot_frame->set_label(shower2->get_plot_title());
	//shower2->set_frame_labeli(lower_plot_frame);

	shower2->nullify(); daatta.color = Gdk::Color("Blue");
	shower2->add_ybar(mass_excess);shower2->add_ybar(mass_excess-dmass_excess);shower2->add_ybar(mass_excess+dmass_excess);
	shower2->show_ybars(true);
	shower2->set_xtics(2); shower2->set_mxtics(5);

	//shower2->input_data(daatta);
	lower_plot_data1 = daatta.data;
	lower_plot_data1_container.color = daatta.color;
	lower_plot_data1_container.graph_type = YERR;

	
	shower2->autosetPlottingBox();
	shower2->show_data(true);
	shower2->show_contents();
}

//------------------------------------------------------------------------------
void Post_Processor::on_radiobutton_freq_pressed() //Frequency ratio
{
	if (!files_exist())
		return;
	DATASET daatta;
	daatta.data_name="frequency ratio";
	shower2->set_plot_title(daatta.data_name);
	daatta.graph_type=YERR;
	Datapoint toAdd;
	double start_time(0);
	for (vector<ref_meas_block>::iterator i= pairs.begin(); i !=pairs.end();++i) {
		if (i== pairs.begin()) {
			start_time = time_t(loc_tm(i->meas1->time_of_scan));
		}
		toAdd.x_value = (time_t(loc_tm(i->meas1->time_of_scan)) -start_time)/60.0;
		toAdd.y_value = i->meas1->freq_ratio.v;
//		cout << "me: " <<i->meas1->m_exc << " err: " <<i->meas1->dm_exc << "\n";
		toAdd.y_value_err = i->meas1->freq_ratio.d;
		daatta.data.push_back(toAdd);
	}

	// lower_plot_frame->set_label(shower2->get_plot_title());
	// shower2->set_frame_labeli(lower_plot_frame);


	shower2->nullify(); daatta.color = Gdk::Color("Blue");
	shower2->add_ybar(freq_ratio);shower2->add_ybar(freq_ratio-ddddfreq_ratio);shower2->add_ybar(freq_ratio+ddddfreq_ratio);
	shower2->show_ybars(true);
	shower2->set_xtics(2); shower2->set_mxtics(5);

	//shower2->input_data(daatta);
	lower_plot_data1 = daatta.data;
	lower_plot_data1_container.data_name = daatta.data_name;
	lower_plot_data1_container.color = daatta.color;
	lower_plot_data1_container.graph_type = YERR;

	shower2->autosetPlottingBox();
	shower2->show_data(true);
	shower2->show_contents();
}

void add_stuff(DATASET & daatta, double start_time,
	       Datapoint & toAdd, void (*fp)(const post_process_data &,
					double &,
					double &),
	       vector<post_process_stuffs> & foos,
	       double y_offset = 0.0)
{
    for (vector<post_process_stuffs>::iterator i=foos.begin();
	 i != foos.end(); ++i) {
	toAdd.x_value = (time_t(loc_tm(i->time_of_scan)) - start_time)/60.0;
	fp(*i, toAdd.y_value, toAdd.y_value_err);
	toAdd.y_value += y_offset;
	daatta.data.push_back(toAdd);
    }
}

void Post_Processor::do_stuff(DATASET & daatta,
			      void (*fp)(const post_process_data &,
					 double &,
					 double &),
			      double y_offset,
			      const char * extra)
{
    shower->set_plot_title(daatta.data_name);
    daatta.graph_type=YERR;
    Datapoint toAdd;
    shower->nullify();
    // upper_plot_frame->set_label(shower->get_plot_title());
    // shower->set_frame_labeli(upper_plot_frame);

    double start_time(0);
    if (measurements.size() > 0) {
	start_time = time_t(loc_tm(measurements[0].time_of_scan));
    }
    add_stuff(daatta, start_time, toAdd, fp, measurements, y_offset);
    if (extra) {
	daatta.data_name = "meas (" + string(extra) + " Hz)";
    } else {
	daatta.data_name = "meas";
    }
    // shower->input_data(daatta);
    shower->clear_plot_data_containers();
    upper_plot_data1 = daatta.data;
    upper_plot_data1_container.data_name = daatta.data_name;
    upper_plot_data1_container.graph_type = YERR;
    shower->add_data(&upper_plot_data1_container);

    daatta.data.clear(); daatta.color = Gdk::Color("Blue");
    add_stuff(daatta, start_time, toAdd, fp, references);
    daatta.data_name = "ref";

    upper_plot_data2 = daatta.data;
    upper_plot_data2_container.data_name = daatta.data_name;
    upper_plot_data2_container.graph_type = YERR;
    shower->add_data(&upper_plot_data2_container);

    //shower->input_data2(daatta);
}

//------------------------------------------------------------------------------
void Post_Processor::on_axial_pressed()
{
	if (!files_exist())
	    return;

	DATASET daatta;
	daatta.data_name = "Axial energy";

	do_stuff(daatta, &post_process_stuffs::get_axial);
	set_ybars(meas.axial.v, meas.axial.d, ref.axial.v, ref.axial.d);

	shower->autosetPlottingBox();
	shower->show_data(true);
	shower->show_contents();
}
//------------------------------------------------------------------------------
void Post_Processor::on_press_pressed()
{
	if (!files_exist())
		return;
	DATASET daatta;
	daatta.data_name = "Pressure";

	do_stuff(daatta, &post_process_stuffs::get_pressure);
	set_ybars(meas.press.v, meas.press.d, ref.press.v, ref.press.d);

	shower->autosetPlottingBox();
	shower->show_data(true);
	shower->show_contents();

	putDoubleToEntry(*ref_aver_entry, ref.press.v);
	putDoubleToEntry(*meas_aver_entry, meas.press.v);
}
//------------------------------------------------------------------------------
void Post_Processor::on_cycl_pressed()
{
	if (!files_exist())
		return;
	DATASET daatta;
	daatta.data_name = "cyclotron radius";

	do_stuff(daatta, &post_process_stuffs::get_cyclotron);
	set_ybars(meas.cycl.v, meas.cycl.d, ref.cycl.v, ref.cycl.d);

	shower->autosetPlottingBox();
	shower->show_data(true);
	shower->show_contents();

	putDoubleToEntry(*ref_aver_entry,ref.cycl.v);
	putDoubleToEntry(*meas_aver_entry,meas.cycl.v);
}
//------------------------------------------------------------------------------
void Post_Processor::on_phase_pressed()
{
	if (!files_exist())
		return;
	DATASET daatta;
	daatta.data_name = "phase";

	do_stuff(daatta, &post_process_stuffs::get_phase);
	set_ybars(meas.phase.v, meas.phase.d, ref.phase.v, ref.phase.d);

	shower->autosetPlottingBox();
	shower->show_data(true);
	shower->show_contents();

	putDoubleToEntry(*ref_aver_entry,ref.phase.v);
	putDoubleToEntry(*meas_aver_entry,meas.phase.v);
}
//------------------------------------------------------------------------------
void Post_Processor::on_t_rf_pressed()
{
	if (!files_exist())
		return;
	DATASET daatta;
	daatta.data_name = "T_rf";

	do_stuff(daatta, &post_process_stuffs::get_rf_time);
	set_ybars(meas.t_rf.v, meas.t_rf.d, ref.t_rf.v, ref.t_rf.d);

	shower->autosetPlottingBox();
	shower->show_data(true);
	shower->show_contents();

	putDoubleToEntry(*ref_aver_entry,ref.t_rf.v);
	putDoubleToEntry(*meas_aver_entry,meas.t_rf.v);
}
//------------------------------------------------------------------------------
void Post_Processor::on_a_rf_pressed()
{
	if (!files_exist())
		return;
	DATASET daatta;
	daatta.data_name = "RF-amplitude";

	do_stuff(daatta, &post_process_stuffs::get_rf_amplitude);
	set_ybars(meas.a_rf.v, meas.a_rf.d, ref.a_rf.v, ref.a_rf.d);

	shower->autosetPlottingBox();
	shower->show_data(true);
	shower->show_contents();

	putDoubleToEntry(*ref_aver_entry, ref.a_rf.v);
	putDoubleToEntry(*meas_aver_entry, meas.a_rf.v);
}
//------------------------------------------------------------------------------
void Post_Processor::on_magn_pressed()
{
	if (!files_exist())
		return;
	DATASET daatta;
	daatta.data_name = "Magnetron radius";

	do_stuff(daatta, &post_process_stuffs::get_magnetron);
	set_ybars(meas.magn.v, meas.magn.d, ref.magn.v, ref.magn.d);

	shower->autosetPlottingBox();
	shower->show_data(true);
	shower->show_contents();

	putDoubleToEntry(*ref_aver_entry, ref.magn.v);
	putDoubleToEntry(*meas_aver_entry, meas.magn.v);
}
//------------------------------------------------------------------------------
void Post_Processor::on_freq_pressed()
{
	if (!files_exist())
		return;
	DATASET daatta;

	daatta.data_name = "Frequency";
	char blaa[100]; sprintf(blaa, "%.3f", ref.freq.v - meas.freq.v);
	do_stuff(daatta, &post_process_stuffs::get_frequency,
		 ref.freq.v - meas.freq.v, blaa);

	shower->show_ybars(true);
	shower->set_xtics(2); shower->set_mxtics(5);

	shower->autosetPlottingBox();
	shower->show_data(true);
	shower->show_contents();

	putDoubleToEntry(*ref_aver_entry,ref.freq.v);
	putDoubleToEntry(*meas_aver_entry,meas.freq.v);
}
//------------------------------------------------------------------------------
void Post_Processor::on_freq2_pressed()
{
	if (!files_exist())
		return;
	DATASET daatta;

	daatta.data_name = "Frequency 2";
	char blaa[100]; sprintf(blaa, "%.3f", ref.freq2.v - meas.freq2.v);
	do_stuff(daatta, &post_process_stuffs::get_frequency2,
		 ref.freq2.v - meas.freq2.v, blaa);

	shower->show_ybars(true);
	shower->set_xtics(2); shower->set_mxtics(5);

	shower->autosetPlottingBox();
	shower->show_data(true);
	shower->show_contents();

	putDoubleToEntry(*ref_aver_entry, ref.freq2.v);
	putDoubleToEntry(*meas_aver_entry, meas.freq2.v);

}
//------------------------------------------------------------------------------
void Post_Processor::on_fraction_pressed()
{
	if (!files_exist())
		return;
	DATASET daatta;
	daatta.data_name = "freq2_fraction";

	do_stuff(daatta, &post_process_stuffs::get_fraction);

	shower->show_ybars(false);
	shower->set_xtics(2); shower->set_mxtics(5);

	shower->autosetPlottingBox();
	shower->show_data(true);
	shower->show_contents();

	ref_aver_entry->set_text("-");
	meas_aver_entry->set_text("-");

}
//------------------------------------------------------------------------------
void Post_Processor::on_otherItems_changed()
{  //Selecting from list
	ref_aver_entry->set_text(""); meas_aver_entry->set_text("");

	if (radiobutton23->get_active()) {
		on_other_items_pressed();
	}
}
//------------------------------------------------------------------------------
void Post_Processor::show_ions_per_shot()
{
//	cout << "ions/shot\n";
	if (!files_exist())
		return;

	DATASET daatta;
	daatta.data_name = "ions/shot";
	do_stuff(daatta, &post_process_stuffs::get_ions_per_shot);

	shower->show_ybars(false);
	shower->set_xtics(2); shower->set_mxtics(5);

	shower->autosetPlottingBox();
	shower->show_data(true);
	shower->show_contents();

	ref_aver_entry->set_text("-");
	meas_aver_entry->set_text("-");

}
//------------------------------------------------------------------------------
void Post_Processor::show_counts_in_class()
{
	if (!files_exist())
		return;

	DATASET daatta;
	daatta.data_name = "counts_in_class";
	do_stuff(daatta, &post_process_stuffs::get_counts_in_class);

	shower->show_ybars(false);
	shower->set_xtics(2); shower->set_mxtics(5);

	shower->autosetPlottingBox();
	shower->show_data(true);
	shower->show_contents();

	ref_aver_entry->set_text("-");
	meas_aver_entry->set_text("-");
}
//------------------------------------------------------------------------------
void Post_Processor::show_counts_in_all_classes()
{
	if (!files_exist())
		return;

	DATASET daatta;
	daatta.data_name = "counts_in_class ";
	do_stuff(daatta, &post_process_stuffs::get_counts_in_all_classes);

	shower->show_ybars(false);
	shower->set_xtics(2); shower->set_mxtics(5);

	shower->autosetPlottingBox();
	shower->show_data(true);
	shower->show_contents();

	ref_aver_entry->set_text("-");
	meas_aver_entry->set_text("-");

}
//------------------------------------------------------------------------------
void Post_Processor::show_z_slope()
{
	if (!files_exist())
		return;
	DATASET daatta;
	daatta.data_name = "Z-class slope";

	do_stuff(daatta, &post_process_stuffs::get_z_slope);
	set_ybars(meas.z_slope.v, meas.z_slope.d, ref.z_slope.v, ref.z_slope.d);

	shower->autosetPlottingBox();
	shower->show_data(true);
	shower->show_contents();

	putDoubleToEntry(*ref_aver_entry, ref.z_slope.v * 1000.);
	putDoubleToEntry(*meas_aver_entry, meas.z_slope.v * 1000.);
}
//------------------------------------------------------------------------------
void Post_Processor::show_z_const()
{
	if (!files_exist())
		return;
	DATASET daatta;

	daatta.data_name = "Z-class Zero";
	char blaa[100]; sprintf(blaa, "%.3f", ref.z_const.v - meas.z_const.v);
	do_stuff(daatta, &post_process_stuffs::get_z_const,
		 ref.z_const.v - meas.z_const.v, blaa);

	shower->show_ybars(false);
	shower->set_xtics(2); shower->set_mxtics(5);

	shower->autosetPlottingBox();
	shower->show_data(true);
	shower->show_contents();

	putDoubleToEntry(*ref_aver_entry, ref.z_const.v);
	putDoubleToEntry(*meas_aver_entry, meas.z_const.v);
}
//------------------------------------------------------------------------------
void Post_Processor::show_chisq(void)
{
	if (!files_exist())
		return;
	DATASET daatta;
	daatta.data_name = "Chisq of fit";

	do_stuff(daatta, &post_process_stuffs::get_fit_chisq);

	shower->show_ybars(false);
	shower->set_xtics(2); shower->set_mxtics(5);

	shower->autosetPlottingBox();
	shower->show_data(true);
	shower->show_contents();
}
//------------------------------------------------------------------------------
void Post_Processor::on_other_items_pressed()
{ //If radiobutton pressed
	//cout << "text in : " << otherItems.get_active_text() << "\n";
	if (otherItems.get_active_text() == "Other...")
		return;
	if (otherItems.get_active_text() == "ions/shot") {
		show_ions_per_shot();
		return;
	}
	if (otherItems.get_active_text() == "#ions(class)") {
		show_counts_in_class();
		return;
	}
	if (otherItems.get_active_text() == "#ions(all)") {
		show_counts_in_all_classes();
		return;
	}
	if (otherItems.get_active_text() == "Z slope (mHz/ion per bunch)") {
		show_z_slope();
		return;
	}
	if (otherItems.get_active_text() == "Z const (Hz)") {
		show_z_const();
		return;
	}
	if (otherItems.get_active_text() == "Chisq") {
		show_chisq();
		return;
	}
	if (otherItems.get_active_text() == "two peak ratio") {
		on_fraction_pressed();
		return;
	}
	if (otherItems.get_active_text() == "frequency2 (Hz)") {
		on_freq2_pressed();
		return;
	}

	cout << "huh? nothing done!\n";
}

//------------------------------------------------------------------------------
void Post_Processor::on_show_dependencies_pressed(void)
{
	if (!files_exist())
		return;
	DATASET daatta;
//	daatta.data_name="q-value";
	daatta.graph_type=YERR;
	Datapoint toAdd;
	shower->nullify();

	double start_time(0);
	if (measurements.size() > 0) {
	    start_time = time_t(loc_tm(measurements[0].time_of_scan));
	}

	add_stuff(daatta, start_time, toAdd,
		  &post_process_stuffs::get_frequency,
		  measurements);
	for (vector<Datapoint>::iterator i= daatta.data.begin();
	     i != daatta.data.end();
	     ++i) {
	    i->y_value += ref.freq.v - meas.freq.v;
	}

	char blaa[100]; sprintf(blaa,"%.3f",ref.freq.v-meas.freq.v);
	daatta.data_name = ("meas (+" + string(blaa) + " Hz)");
	//shower->input_data(daatta);
	upper_plot_data1 = daatta.data;
	upper_plot_data1_container.data_name = daatta.data_name;

	daatta.data.clear(); daatta.color = Gdk::Color("Blue");
	add_stuff(daatta, start_time, toAdd,
		  &post_process_stuffs::get_frequency,
		  references);
	daatta.data_name = "ref";
	//shower->input_data2(daatta);
	
	upper_plot_data2 = daatta.data;
	upper_plot_data2_container.data_name = daatta.data_name;
	
	daatta.data.clear();
	double x1,x2,y1,y2;
	for (vector<ref_meas_block>::iterator i=pairs.begin(); i != pairs.end(); ++i) {
		x1 = (time_t(loc_tm(i->ref1->time_of_scan)) -start_time)/60.0;
		x2 = (time_t(loc_tm(i->meas1->time_of_scan)) -start_time)/60.0;
		y1 = i->ref1->freq.v + i->ref1->freq.d;
		y2 = i->meas1->freq.v + ref.freq.v - meas.freq.v;
		shower->add_arrow(x1,y1,x2,y2);
		y1 = i->ref1->freq.v-i->ref1->freq.d;
		shower->add_arrow(x1,y1,x2,y2);

		if (!PG_mode_cb->get_active()) {
			x1 = (time_t(loc_tm(i->ref2->time_of_scan)) -start_time)/60.0;
			y1 = i->ref2->freq.v+i->ref2->freq.d;
			shower->add_arrow(x1,y1,x2,y2);
			y1 = i->ref2->freq.v-i->ref2->freq.d;
			shower->add_arrow(x1,y1,x2,y2);
		}

	}

	shower->show_ybars(true);
	shower->set_xtics(2); shower->set_mxtics(5);

	shower->autosetPlottingBox();
	shower->show_data(true);
	shower->show_contents();

}
//------------------------------------------------------------------------------
void Post_Processor::on_load_files_button_clicked(void)
{
	// cout << "onnaako?\n";
	if (files_exist(true)) {
//		process_file(REF); process_file(MEAS);
//		on_recalc_button_clicked();
		cout << "yes\n";
		do_everything();
		cout << "done everything\n";
		bool ok(0);
		string found_element;
		string source_of_value;
//		mass_excess = uff->get_mass_excess(references.begin()->element, dmass_excess, found_element, ok, source_of_value);
		try {
		  mass_excess = ame_mass_table_p->get_mass_excess(references.begin()->element);
		  dmass_excess = ame_mass_table_p->get_mass_excess_error(references.begin()->element);
		  source_of_value = ame_mass_table_p->get_mass_excess_source(references.begin()->element);
		  found_element = ame_mass_table_p->get_molecule_part(references.begin()->element);
		  ok = true;
		} catch (...) {
		  ok = false;
		  mass_excess = 9999999;
		  dmass_excess = 999999;
		}
		print_mass_excess();
		// cout <<"n\n";
//		if (fabs(dmass_excess + 1.00) < 0.0000001)
		if (!ok) {
			st_normal_bg_color(*me_entry, red_color);
			st_normal_bg_color(*me_err_entry, red_color);
			ame_ok->set_text("("+ found_element + " not found!)");

		} else {
			st_normal_bg_color(*me_entry, default_widget_base_color);
			st_normal_bg_color(*me_err_entry, default_widget_base_color);
			//if (found_element == "Cc") {
			//	ame_ok->set_text("Carbon cluster");
			//}
			//else {
				ame_ok->set_text("("+source_of_value+" "+found_element+")");
				//ame_ok->set_text("(AME03,"+found_element+")");
			//}
			ame_element_entry->set_text(found_element);
		}
		print_mass_excess();
		// std::cout << "hahko\n";
		on_recalc_button_clicked();
		// std::cout << "hahkoa\n";
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Post_Processor::on_do2_button_clicked()
{ //debugger
	do_everything();

	print_pairs(cout);
	print_reverse_pairs(cout);

	calculate_all_sets();
	cout << "refer:\n";
}
//------------------------------------------------------------------------------
int Post_Processor::divide_to_sets()
{
	if (pairs.size() < 2) {
		cout << "Only 1 pair or less..\n";
		return -1;
	}
	boundaries.clear(); //To mark each set's boundary. 2006-08-22 TE
	FIRST_LAST_FREQS toAdd;
	vector<ref_meas_block>::iterator j; //iterator i-1
	for (vector<ref_meas_block>::iterator i=pairs.begin();i != pairs.end(); ++i) { //Go through all measurements
		if (i == pairs.begin()) { //Mark the 1st of the pair in the set
			toAdd.first_block = pairs.begin();
			i++;
		}
		j = i-1;
		if (i->ref1 == j->ref2) { //If this and the previous is same, then it belongs to same set.
			continue;
		}
		else {
			toAdd.last_block = j; //If not, mark the end of it
			boundaries.push_back(toAdd);
//			cout << "Lisättiin\n";
			toAdd.first_block = i;  //To start a new set
		}

/*		cout << "Joo, nämä ovat peräkkäin: " << i->meas1->scan_number << " ";
		i--;	cout << i->meas1->scan_number << "\n";		i++;
		cout << "Yhteinen referenssi: " << i->ref1->scan_number << " ";
		i--;	cout << i->ref2->scan_number << "\n";		i++;*/
	}
	toAdd.last_block = j+1;
	boundaries.push_back(toAdd);
//	cout << "Lisättiin\n";


	for (vector<FIRST_LAST_FREQS>::iterator i= boundaries.begin(); i != boundaries.end(); ++i) {
//		cout << "Setti, eka meas: " << i->first_block->meas1->scan_number << " vika : " << i->last_block->meas1->scan_number << "\n";

		vector<ref_meas_block>::iterator j=i->first_block;
/*		while (1) {
			cout << "Tämä meas1 :" << j->meas1->scan_number << "\n";
			if (j == i->last_block)
				break;
			j++;
		}	*/
	}
	return 0;
}
//------------------------------------------------------------------------------
void Post_Processor::do_everything(void)
{
	references.clear(); measurements.clear();
	boundaries.clear(); pairs.clear(); reverse_pairs.clear();
	process_file(REF); process_file(MEAS);
	if (references.size() < 2 && (PG_mode_cb->get_active() == false)) {
		cout << "There is only 1 pair or less.\n";
		return;
	}
	sort(references.begin(), references.end());
	sort(measurements.begin(), measurements.end());
	if (PG_mode_cb->get_active() == false)
		putIntegerToEntry(*pairs_entry, assing_pairs());
	else
		putIntegerToEntry(*pairs_entry, assign_nearest_ref_to_meas());
	divide_to_sets();
	//putIntegerToEntry(*rev_pairs_entry, assing_reverse_pairs());

//	calculate_all_sets();
	on_recalc_button_clicked();
//	redraw_what_is_active_now();
}
//------------------------------------------------------------------------------
int post_process_stuffs::get_time_diff(const post_process_stuffs &to_this)
{
	return (time_t(loc_tm(time_of_scan)) - time_t(loc_tm(to_this.time_of_scan)));
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int Post_Processor::get_number_of_pairs(void)
{
	return pairs.size();
}
//------------------------------------------------------------------------------
void Post_Processor::move_windows_to_initial_positions(void)
{
	int coma_x,coma_y,small_x,small_y;
	int screen_width;
	Glib::RefPtr<Gdk::Screen> screeni = get_screen();
	screen_width = screeni->get_width();
	get_size(coma_x,coma_y);
	move(0,0);
	small_x = (screen_width - coma_x);
	small_y = coma_y/2;
	//shower		->resize(small_x,small_y);
	//shower     	->move(coma_x,0);
	return;
}
//------------------------------------------------------------------------------
void Post_Processor::on_just_close_clicked(void)
{
	m_signal_close.emit();
}
//------------------------------------------------------------------------------
double Post_Processor::get_interpolated_freq(const post_process_stuffs &ref1, const post_process_stuffs &ref2, const string &time_to_interpolate)
{
	// Make t0 = 0, first reference
	// If t0 is ref1, t2 is ref2, t1 is time where to interpolate t0 ==== 0!!!!
	//
	long t2 = time_t(loc_tm(ref2.time_of_scan)) - time_t(loc_tm(ref1.time_of_scan));
	long t1 = time_t(loc_tm(time_to_interpolate)) - time_t(loc_tm(ref1.time_of_scan));

	if (t1 < 0 || t1 > t2) {
	}

	return ( 		1.0/t2 * ( ref1.freq.v*(t2 - t1) + ref2.freq.v*t1) 	);

}
//------------------------------------------------------------------------------
double Post_Processor::get_interpolated_freq_err(const post_process_stuffs &ref1, const post_process_stuffs &ref2, const string &time_to_interpolate)
{
	// Make t0 = 0, first reference
	// If t0 is ref1, t2 is ref2, t1 is time where to interpolate t0 ==== 0!!!!
	//
	long t2 = time_t(loc_tm(ref2.time_of_scan)) - time_t(loc_tm(ref1.time_of_scan));
	long t1 = time_t(loc_tm(time_to_interpolate)) - time_t(loc_tm(ref1.time_of_scan));

	return (1.0/t2 * sqrt(pow((t2-t1) *  1.0 * ref1.freq.dd, 2.0) + pow(t1*ref2.freq.dd, 2.0)));
}
//------------------------------------------------------------------------------
void Post_Processor::on_bring_helper_window_on_top_activate(void)
{
	//shower->present();
}
//------------------------------------------------------------------------------
void Post_Processor::on_quit_coma_activate(void)
{
	m_signal_close.emit();
	hide();
}
//------------------------------------------------------------------------------
double Post_Processor::get_time_between_references(const post_process_stuffs &ref1, const post_process_stuffs &ref2)
{
	long t2 = time_t(loc_tm(ref2.time_of_scan));
	long t1 = time_t(loc_tm(ref1.time_of_scan));
	return ((t2-t1) * 1.0)/60.0;
}
//------------------------------------------------------------------------------
vector<std::string> Post_Processor::get_filenames_from_entry(Gtk::Entry &entri)
{
	std::string kaikki = entri.get_text();
	vector<std::string> returnee;
	while ( kaikki != "") {
		string yksi = erota(kaikki,';');
		returnee.push_back(yksi);
		//cout << "Lisattiin: \"" << yksi << "\"\n";
	}
	//cout << "Filuja:" << returnee.size() << "Ei muuta.\n";
	return returnee;
}
//------------------------------------------------------------------------------
void Post_Processor::on_save_settings_activate()
{
	Glib::ustring outfilename;
	Gtk::FileChooserDialog dialog("Save COMA settings...", Gtk::FILE_CHOOSER_ACTION_SAVE);

	Glib::RefPtr<Gtk::FileFilter> filter_any =
	    get_file_filter("*.coma", "*.coma");
	Glib::RefPtr<Gtk::FileFilter> filter_settings =
	    get_file_filter("*.*", "*.*");
	file_chooser_add_filter(dialog, filter_settings);
	file_chooser_add_filter(dialog, filter_any);

	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);
	dialog.set_current_folder( out_dir_entry->get_text());
	dialog.set_default_response(Gtk::RESPONSE_OK);
	int result = dialog.run();
	if (result == Gtk::RESPONSE_OK) {
		outfilename = dialog.get_filename();
	}
	else
		return;
	store_settings(outfilename);
}
//------------------------------------------------------------------------------
void Post_Processor::on_load_settings_activate()
{
	Gtk::FileChooserDialog dialog("Load COMA settings...", Gtk::FILE_CHOOSER_ACTION_OPEN);

	Glib::RefPtr<Gtk::FileFilter> filter_any =
	    get_file_filter("*.coma", "*.coma");
	Glib::RefPtr<Gtk::FileFilter> filter_settings =
	    get_file_filter("*.*", "*.*");
	file_chooser_add_filter(dialog, filter_settings);
	file_chooser_add_filter(dialog, filter_any);

	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
	dialog.set_current_folder(out_dir_entry ->get_text());
	dialog.set_default_response(Gtk::RESPONSE_OK);
	dialog.set_select_multiple(false);
	int result = dialog.run();
	if (result == Gtk::RESPONSE_OK) {
		restore_settings(dialog.get_filename());
	}
	return;
}
//------------------------------------------------------------------------------
void Post_Processor::on_pp_save_to_ini_file_activate()
{
       store_settings("coma.ini", false);
}
//------------------------------------------------------------------------------
void Post_Processor::on_pp_save_to_default_ini_file_activate()
{
       store_settings("coma.ini", true);
}
//------------------------------------------------------------------------------
void Post_Processor::store_settings(Glib::ustring filename, bool rcfile)
{
	boost::filesystem::ofstream settingsfile;
	//settingsfile.open("coma.ini");// filename.c_str());
	if (rcfile) {
	    if (!open_file_for_overwriting_and_popup_message_if_error(
		    local_or_rc_file(filename.c_str()), settingsfile)) {
		return;
	    }
	} else {
	    if (!open_file_for_overwriting_and_popup_message_if_error(filename, settingsfile)) {
		return;
	    }
	}
	char DL = '\t';
	loc_tm current_time(0);
	settingsfile.precision(10);
	settingsfile << "#Settings saved at " << loc_date(current_time,loc_tm::d_oma) << " " << loc_time(current_time,loc_tm::t_International) << "\n";
	settingsfile << "DIRECTORY"  << DL << out_dir_entry->get_text() << "\n";
	settingsfile << "REFERENCES" << DL << ref_file_entry->get_text() << "\n";
	settingsfile << "MEASUREMENTS" << DL << meas_file_entry->get_text() << "\n";
	settingsfile << "DATASET_NAME" << DL << dataset_name_entry->get_text() << "\n";
	settingsfile << "B_FLUCTUATION" << DL << bfield_fluct_spinbutton->get_text() << "\n";
	settingsfile << "MASS_DEP_ERROR" << DL << mass_dep_error_spinbutton->get_text() << "\n";
	settingsfile << "RESIDUAL_ERROR" << DL << residual_syst_error_spinbutton->get_text() << "\n";

	settingsfile << "FINAL_RESULTS" << DL << export_filename->get_text() << "\n";
	settingsfile << "INDIVIDUAL_RESULTS" << DL << fratio_filename_entry->get_text() << "\n";
//	settingsfile << "GNUPLOT_STUFF" << DL << aver_filename_entry->get_text() << "\n";

	settingsfile << "MASS_EXCESS" << DL << me_entry->get_text() << "\n";
	settingsfile << "MASS_EXCESS_ERR" << DL << me_err_entry->get_text() << "\n";

	settingsfile << "PG_MODE" << DL << PG_mode_cb->get_active() << "\n";
	settingsfile.close();

}
//------------------------------------------------------------------------------
int Post_Processor::restore_settings(Glib::ustring filename) {
	boost::filesystem::ifstream openedfile;
	double tempdbl;

	double me(0), me_err(0);
	bool meX(0), me_errX(0);

	int rownumber(0);
	string errorstring;
	openedfile.open(local_or_rc_file(filename.c_str()).c_str());
	if (!openedfile) {
		Gtk::MessageDialog dialooki("  Unable to load COMA settingfile:\n\n  " + filename + "  ",false,Gtk::MESSAGE_ERROR);
		dialooki.set_title("What could be the wrong?");
		dialooki.run();
		return -1;
	}
	std::string row;
	std::string piece;
	int invalids(0);
	is_reading_settingfile=true;
	while (getline(openedfile,row)) {
		rownumber++;
		if (row[0] == '#')
			continue;
		piece = erota(row,'\t');
		if (piece == "DIRECTORY") {
			out_dir_entry->set_text(row); continue;
		}
		if (piece == "REFERENCES") {
			ref_file_entry->set_text(row); continue;
		}
		if (piece == "MEASUREMENTS") {
			meas_file_entry->set_text(row); continue;
		}
		if (piece == "DATASET_NAME") {
			dataset_name_entry->set_text(row); continue;
		}
		if (piece == "FINAL_RESULTS") {
			export_filename->set_text(row); continue;
		}
		if (piece == "INDIVIDUAL_RESULTS") {
			fratio_filename_entry->set_text(row); continue;
		}
//		if (piece == "GNUPLOT_STUFF") {
//			aver_filename_entry->set_text(row); continue;
//		}
		if (piece == "MASS_EXCESS") {
			me_entry->set_text(row); continue;
		}
		if (piece == "MASS_EXCESS_ERR") {
			me_err_entry->set_text(row); continue;
		}
		if (piece == "B_FLUCTUATION") {
			luvuksi(row,tempdbl);
			bfield_fluct_spinbutton->set_value(tempdbl); continue;
		}
		if (piece == "MASS_DEP_ERROR") {
			luvuksi(row,tempdbl);
			mass_dep_error_spinbutton->set_value(tempdbl); continue;
		}
		if (piece == "RESIDUAL_ERROR") {
			luvuksi(row,tempdbl);
			residual_syst_error_spinbutton->set_value(tempdbl); continue;
		}
		if (piece == "MASS_EXCESS") {
			luvuksi(row,tempdbl);
			me = tempdbl; meX=1; continue;
		}
		if (piece == "MASS_EXCESS_ERR") {
			luvuksi(row,tempdbl);
			me_err = tempdbl; me_errX=1; continue;
		}
		if (piece == "PG_MODE") {
			if (row == "1")	PG_mode_cb->set_active();
			else 				PG_mode_cb->set_active(false);
			continue;
		}


		invalids++;
		errorstring.append("line ");
		char num[5];
		sprintf(num,"%d",rownumber);
		errorstring.append(num);
		errorstring.append("\n");
	}
	openedfile.close();
	update_gtk();
	//on_reload_files_button_clicked();
	if (meX)
		putDoubleToEntry(*me_entry,me);
	if (me_errX)
		putDoubleToEntry(*me_err_entry,me_err);
	//on_recalc_button_clicked();
	if (invalids > 0) {
		std::cout << "Invalid lines in the COMA setting file: " << invalids << endl;
		std::cout << errorstring;
	}
	is_reading_settingfile=false;
	return invalids;
}
//------------------------------------------------------------------------------
void Post_Processor::on_what_to_use_changed() {

}
//------------------------------------------------------------------------------
void Post_Processor::initialize_comboboxes() {
	otherItems.append("Other...");
	otherItems.append("frequency2 (Hz)");
	otherItems.append("two peak ratio");
	otherItems.append("ions/shot");
	otherItems.append("#ions(class)");
	otherItems.append("#ions(all)");
	otherItems.append("Z slope (mHz/ion per bunch)");
	otherItems.append("Z const (Hz)");
	otherItems.append("Chisq");
	//table12->attach(otherItems,0,1,16,17,Gtk::SHRINK,Gtk::AttachOptions(),0,0);
	//table12->attach(otherItems,0,1,16,17,Gtk::SHRINK,Gtk::SHRINK,0,0);
	other_items_box->pack_start(otherItems,0,0);
	otherItems.show();
	otherItems.set_active_text("Other...");
	otherItems.signal_changed().connect(sigc::mem_fun(*this, &Post_Processor::on_otherItems_changed), false);

	meas_comboboxtext.append("MEAS: all");
	meas_comboboxtext.append("MEAS: odds");
	meas_comboboxtext.append("MEAS: evens");
	meas_comboboxtext.set_active_text("MEAS: all");
	//hbox3->pack_end (meas_comboboxtext, 0, 0);
	files_table->attach(meas_comboboxtext,3,4,1,2,Gtk::SHRINK,Gtk::SHRINK,0,0);
	meas_comboboxtext.show();
	meas_comboboxtext.signal_changed().connect(sigc::mem_fun(*this, &Post_Processor::on_recalculation_requested), false);

	ref_comboboxtext.append("REFS: all");
	ref_comboboxtext.append("REFS: odds");
	ref_comboboxtext.append("REFS: evens");
	ref_comboboxtext.set_active_text("REFS: all");
	//hbox2->pack_end (ref_comboboxtext, 0, 0);
	files_table->attach(ref_comboboxtext,3,4,0,1,Gtk::SHRINK,Gtk::SHRINK,0,0);
	ref_comboboxtext.show();
	ref_comboboxtext.signal_changed().connect(sigc::mem_fun(*this, &Post_Processor::on_recalculation_requested), false);

}
//------------------------------------------------------------------------------
void Post_Processor::on_recalculation_requested(void) {
	//cout << "recalculation requested\n";
	on_load_files_button_clicked();
}
//------------------------------------------------------------------------------
void Post_Processor::draw_connections(void)
{

}
//------------------------------------------------------------------------------
void Post_Processor::on_meas_files_chooser_clicked()
{
	set_input_path_and_ref_or_meas_files(meas_file_entry);
}
//------------------------------------------------------------------------------
void Post_Processor::on_ref_files_chooser_clicked()
{
	set_input_path_and_ref_or_meas_files(ref_file_entry);
}
//------------------------------------------------------------------------------
void Post_Processor::set_input_path_and_ref_or_meas_files(Gtk::Entry* ref_or_meas_entry)
{
	// Letting user to choose multiple files. Taking care that only files were chosen..
	Gtk::FileChooserDialog dialog("Choose file(s)", Gtk::FILE_CHOOSER_ACTION_OPEN);

	Gtk::FileFilter filter_text;
	filter_text.set_name("Text files");
	filter_text.add_mime_type("text/plain");
	dialog.add_filter(filter_text);

	Gtk::FileFilter filter_any;
	filter_any.set_name("Any files");
	filter_any.add_pattern("*");
	dialog.add_filter(filter_any);

	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
	dialog.set_current_folder(out_dir_entry ->get_text() );
	dialog.set_default_response(Gtk::RESPONSE_OK);
	dialog.set_select_multiple(true);
	int result = dialog.run();
	if (result == Gtk::RESPONSE_OK) {
		std::vector<std::string> files = dialog.get_filenames();
		std::vector<boost::filesystem::path> files_as_paths;
		// First remove non-files
		for (std::string i : files ) 
		{
			boost::filesystem::path a_file(i.c_str());
			if ( boost::filesystem::is_regular_file(a_file) )
			{
				files_as_paths.push_back(a_file);
			}
		}
		
		// Do we have anything in the vector of paths?
		// If not, escaping
		if (files_as_paths.empty())
			return;
		
		// Then split to path part and files
		// Take the directory from the first
		out_dir_entry->set_text(files_as_paths.front().parent_path().string());
		
		// And make semicolon-separated list of the files
		std::string filelist;
		for (boost::filesystem::path i : files_as_paths )
		{
			filelist += i.filename().string() + ";";
		}
		ref_or_meas_entry->set_text(filelist);
	}
}

//------------------------------------------------------------------------------
