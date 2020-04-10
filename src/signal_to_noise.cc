
#include "window1.hh"
#include "fit_parameters.hh"
#include "inputting.hh"

#define LOCK 	bool locked_here;	if (locked)	locked_here = false;	else { locked_here = true; locked = true;}
#define RELEASE redraw_windows_if_necessary(locked_here);

//------------------------------------------------------------------------------
void Window1::on_sn_gating_changed() {
	validate_sn_spinbuttons();
	get_signal_to_noise_ratio_for_class();
}
//------------------------------------------------------------------------------
void Window1::on_sn_apply_button_clicked()  {
	get_signal_to_noise_ratio_for_class();
	// S/N = (TOF1 - TOF2) / SQRT (sigma1^2 + sigma2^2)
}
//------------------------------------------------------------------------------
double Window1::get_signal_to_noise_ratio_for_class() {
	LOCK
	double base_sd, reso_sd, base_tof, reso_tof;

	data->set_all_bunches_active();

	int base_freq_point_no(data->getFrequencyNumber()), reso_freq_point_no(data->getFrequencyNumber());
	//First, get stuff for base line
	if (inputting->ref_radiobutton->get_active() ) {
		//base_freq_point_no -= data->set_frequency_range(ref_sn_base_sp->get_value()-ref_sn_base_win_sp->get_value(),
		//												ref_sn_base_sp->get_value()+ref_sn_base_win_sp->get_value());
		base_freq_point_no -= data->set_frequency_range(ref_sn_base_from_sp->get_value(), ref_sn_base_to_sp->get_value());
	}
	else {
		//base_freq_point_no -= data->set_frequency_range(meas_sn_base_sp->get_value()-meas_sn_base_win_sp->get_value(),
		//												meas_sn_base_sp->get_value()+meas_sn_base_win_sp->get_value());
		base_freq_point_no -= data->set_frequency_range(meas_sn_base_from_sp->get_value(), meas_sn_base_to_sp->get_value());
	}
	base_sn_fp_label->set_text(asString(base_freq_point_no));
	data->set_bunches_that_will_be_skipped(get_rounds_to_skip());

	base_sd = data->get_sd_of_all_ions_in_class(showclass_spinbutton->get_value_as_int(), SUM_STATISTICS);
	base_sn_sigma_label->set_text(asString(base_sd*data->getBin_width(),9));

	base_tof = data->get_average_tof_of_all_ions_in_class(showclass_spinbutton->get_value_as_int(), false);

	base_sn_ions_label->set_text(asString(data->getTotalCounts(
						get_bin_min(),
						get_bin_max(),
						min_z_spinbutton->get_value_as_int(),
						max_z_spinbutton->get_value_as_int(),
						std::cout)));

	//Same for resonance
	data->set_all_bunches_active();
	if (inputting->ref_radiobutton->get_active() ) {
		//reso_freq_point_no -= data->set_frequency_range(ref_sn_reso_sp->get_value()-ref_sn_reso_win_sp->get_value(),
		//												ref_sn_reso_sp->get_value()+ref_sn_reso_win_sp->get_value());
		reso_freq_point_no -= data->set_frequency_range(ref_sn_reso_from_sp->get_value(), ref_sn_reso_to_sp->get_value());
	}
	else {
		//reso_freq_point_no -= data->set_frequency_range(meas_sn_reso_sp->get_value()-meas_sn_reso_win_sp->get_value(),
		//												meas_sn_reso_sp->get_value()+meas_sn_reso_win_sp->get_value());
		reso_freq_point_no -= data->set_frequency_range(meas_sn_reso_from_sp->get_value(), meas_sn_reso_to_sp->get_value());
	}
	reso_sn_fp_label->set_text(asString(reso_freq_point_no));
	data->set_bunches_that_will_be_skipped(get_rounds_to_skip());

	reso_sd = data->get_sd_of_all_ions_in_class(showclass_spinbutton->get_value_as_int(), SUM_STATISTICS);
	reso_sn_sigma_label->set_text(asString(reso_sd*data->getBin_width(),9));

	reso_tof = data->get_average_tof_of_all_ions_in_class(showclass_spinbutton->get_value_as_int(), false);

	reso_sn_ions_label->set_text(asString(data->getTotalCounts(
						get_bin_min(),
						get_bin_max(),
						min_z_spinbutton->get_value_as_int(),
						max_z_spinbutton->get_value_as_int(),
						std::cout)));


	//Calculate results
	tof_diff_label->set_text(asString( abs(base_tof - reso_tof)*data->getBin_width(), 9));
	double sn;
	if (reso_sd < 0 || base_sd < 0)
		sn = -1;
	else
		sn = abs(base_tof - reso_tof) / sqrt( pow(reso_sd,2.0) + pow(base_sd,2.0));

	sn_label->set_text(asString(sn,9));

	data->set_all_bunches_active();
	data->set_frequency_range(get_freq_min(), get_freq_max() );
	data->set_bunches_that_will_be_skipped(get_rounds_to_skip());

	set_sn_bars_to_tof_window();
	tof_vs_class->show_xbars();
	tof_vs_class_need_update = true;
	putDoubleToEntry(*sn_entry, sn, 9);
	RELEASE
	return base_freq_point_no;
}
//------------------------------------------------------------------------------
void Window1::validate_sn_spinbuttons(void) {

	/*double maxxi = ref_sn_base_max_sp->get_value();
	ref_sn_base_max_sp->set_range(ref_sn_base_min_sp->get_value(),1e8);
	ref_sn_base_max_sp->set_value(maxxi);

	maxxi = ref_sn_reso_max_sp->get_value();
	ref_sn_reso_max_sp->set_range(ref_sn_reso_min_sp->get_value(),1e8);
	ref_sn_reso_max_sp->set_value(maxxi);

	maxxi = meas_sn_base_max_sp->get_value();
	meas_sn_base_max_sp->set_range(meas_sn_base_min_sp->get_value(),1e8);
	meas_sn_base_max_sp->set_value(maxxi);

	maxxi = meas_sn_reso_max_sp->get_value();
	meas_sn_reso_max_sp->set_range(meas_sn_reso_min_sp->get_value(),1e8);
	meas_sn_reso_max_sp->set_value(maxxi);*/

	//In case the area was chosen with mouse
	//cout << "TÄÄ EI PELITÄ  haloo2\n";
	//cout << meas_sn_base_from_sp->get_value() << ", " << meas_sn_base_to_sp->get_value() << "\n";
/*	if ( abs(meas_sn_base_from_sp->get_value() - meas_sn_base_win_sp->get_value()) < 1000) {
		double aver = (meas_sn_base_sp->get_value() + meas_sn_base_win_sp->get_value())/2.;
		double width = abs(meas_sn_base_sp->get_value() - meas_sn_base_win_sp->get_value())/2.;
		meas_sn_base_sp->set_value(aver); meas_sn_base_win_sp->set_value(width);
		cout << "haloo\n";
	}*/
}
//------------------------------------------------------------------------------
void Window1::set_sn_bars_to_tof_window() {
	LOCK
	tof_vs_class_need_update=true;
	//tof_vs_class->xbars.delete_bars(); //BAARS
	if (inputting->ref_radiobutton->get_active() ) {
		double value1 = ref_sn_base_from_sp->get_value();
		double value2 = ref_sn_base_to_sp->get_value();
		double value3 = ref_sn_reso_from_sp->get_value();
		double value4 = ref_sn_reso_to_sp->get_value();
		if (!tof_vs_class->xbars.replace_value_in_bar(value1,"baseline1") ) {
			BAR baari(value1, Gdk::Color("Cyan"), "baseline", "baseline1");
			tof_vs_class->xbars.add_bar(baari);
		}
		if (!tof_vs_class->xbars.replace_value_in_bar(value2,"baseline2") ) {
			BAR baari(value2, Gdk::Color("Cyan"), "", "baseline2");
			tof_vs_class->xbars.add_bar(baari);
		}
		if (!tof_vs_class->xbars.replace_value_in_bar(value3,"resonance1") ) {
			BAR baari(value3, Gdk::Color("Red"), "resonance", "resonance1");
			tof_vs_class->xbars.add_bar(baari);
		}
		if (!tof_vs_class->xbars.replace_value_in_bar(value4,"resonance2") ) {
			BAR baari(value4, Gdk::Color("Red"), "", "resonance2");
			tof_vs_class->xbars.add_bar(baari);
		}
		//tof_vs_class->add_xbar(ref_sn_base_sp->get_value()-ref_sn_base_win_sp->get_value(), Gdk::Color("Cyan"), "baseline");
		//tof_vs_class->add_xbar(ref_sn_base_sp->get_value()+ref_sn_base_win_sp->get_value(), Gdk::Color("Cyan"), "");
		//tof_vs_class->add_xbar(ref_sn_reso_sp->get_value()-ref_sn_reso_win_sp->get_value(), Gdk::Color("Red"), "resonance");
		//tof_vs_class->add_xbar(ref_sn_reso_sp->get_value()+ref_sn_reso_win_sp->get_value(), Gdk::Color("Red"), "");
	}
	else {
		double value1 = meas_sn_base_from_sp->get_value();
		double value2 = meas_sn_base_to_sp->get_value();
		double value3 = meas_sn_reso_from_sp->get_value();
		double value4 = meas_sn_reso_to_sp->get_value();
		if (!tof_vs_class->xbars.replace_value_in_bar(value1,"baseline1") ) {
			BAR baari(value1, Gdk::Color("Cyan"), "baseline", "baseline1");
			tof_vs_class->xbars.add_bar(baari);
		}
		if (!tof_vs_class->xbars.replace_value_in_bar(value2,"baseline2") ) {
			BAR baari(value2, Gdk::Color("Cyan"), "", "baseline2");
			tof_vs_class->xbars.add_bar(baari);
		}
		if (!tof_vs_class->xbars.replace_value_in_bar(value3,"resonance1") ) {
			BAR baari(value3, Gdk::Color("Red"), "resonance", "resonance1");
			tof_vs_class->xbars.add_bar(baari);
		}
		if (!tof_vs_class->xbars.replace_value_in_bar(value4,"resonance2") ) {
			BAR baari(value4, Gdk::Color("Red"), "", "resonance2");
			tof_vs_class->xbars.add_bar(baari);
		}
		//tof_vs_class->add_xbar(meas_sn_base_sp->get_value()-meas_sn_base_win_sp->get_value(), Gdk::Color("Cyan"), "baseline");
		//tof_vs_class->add_xbar(meas_sn_base_sp->get_value()+meas_sn_base_win_sp->get_value(), Gdk::Color("Cyan"), "");
		//tof_vs_class->add_xbar(meas_sn_reso_sp->get_value()-meas_sn_reso_win_sp->get_value(), Gdk::Color("Red"), "resonance");
		//tof_vs_class->add_xbar(meas_sn_reso_sp->get_value()+meas_sn_reso_win_sp->get_value(), Gdk::Color("Red"), "");
	}

	RELEASE
}
//------------------------------------------------------------------------------
/*void Window1::on_find_min_tof_button_clicked() {
	double freqq = data->getFreqHavingMinimumTof(showclass_spinbutton->get_value_as_int());
	if (inputting->ref_radiobutton->get_active() ) {
		ref_sn_reso_sp->set_value(freqq);
	}
	else {
		meas_sn_reso_sp->set_value(freqq);
	}
}
//------------------------------------------------------------------------------
void Window1::on_find_max_tof_button_clicked() {
	double freqq = data->getFreqHavingMaximumTof(showclass_spinbutton->get_value_as_int());
	if (inputting->ref_radiobutton->get_active() ) {
		ref_sn_base_sp->set_value(freqq);
	}
	else {
		meas_sn_base_sp->set_value(freqq);
	}

}*/
//------------------------------------------------------------------------------
void Window1::update_sn_menues_in_tof_vs_class_window() {
	if (inputting->ref_radiobutton->get_active()) {
		tof_vs_class->remove_ranges();
//		tof_vs_class->add_range(ref_sn_base_from_sp,ref_sn_base_to_sp,"baseline for S/N calculation (ref)");
//		tof_vs_class->add_range(ref_sn_reso_from_sp,ref_sn_reso_to_sp,"resonance for S/N calculation (ref)");
	}
	else {
		tof_vs_class->remove_ranges();
//		tof_vs_class->add_range(meas_sn_base_from_sp,meas_sn_base_to_sp,"baseline for S/N calculation (meas)");
//		tof_vs_class->add_range(meas_sn_reso_from_sp,meas_sn_reso_to_sp,"resonance for S/N calculation (meas)");
	}
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


