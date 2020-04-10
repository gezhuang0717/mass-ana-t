// generated 2006/2/16 22:11:32 EET by jahakala@pingviini.(none)
// using glademm V2.6.0
//
// newer (non customized) versions of this file go to fit_parameters.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "fit_parameters.hh"

#include "fit_parameters_init.inc"

void
Fit_Parameters::create_widget(Fit_Parameters *& win,
			      const Glib::RefPtr<Gtk::Builder> & builder)
{
    builder->get_widget_derived("fit_parameters", win);
}

Fit_Parameters::Fit_Parameters(GtkTable * obj, const Glib::RefPtr<Gtk::Builder> & builder) : Gtk::Table(obj)
{
    init_widgets(builder);
    init_signals();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Fit_Parameters::on_fit_or_not_changed() {
	fitted_ferr_entry->set_sensitive(ff_button->get_active() );
	fitted_perr_entry->set_sensitive(fp_button->get_active() );
	fitted_axerr_entry->set_sensitive(fax_button->get_active() );
	fitted_magnerr_entry->set_sensitive(fmagn_button->get_active() );
	fitted_cyclerr_entry->set_sensitive(fcycl_button->get_active() );
	fitted_fierr_entry->set_sensitive(ffi_button->get_active() );
	fitted_t_rferr_entry->set_sensitive(ft_rf_button->get_active() );
	fitted_t_waiterr_entry->set_sensitive(ft_wait_button->get_active() );
	fitted_a_rferr_entry->set_sensitive(fa_rf_button->get_active() );
/*	if (ff_button->get_active() )
		fitted_ferr_entry->show();
	else
		fitted_ferr_entry->hide();
	*/
}
//------------------------------------------------------------------------------
void Fit_Parameters::on_auto_t_tot_button_toggled()
{
	if (autofreq_button->get_active()  )
		f_entry->set_sensitive(false);
	else
		f_entry->set_sensitive(true);

	if (auto_t_tot_button->get_active()  )
		t_tot_entry->set_sensitive(false);
	else
		t_tot_entry->set_sensitive(true);

	if (auto_conv_button->get_active()  )
		a_rf_entry->set_sensitive(false);
	else
		a_rf_entry->set_sensitive(true);


}
//------------------------------------------------------------------------------
/*void Fit_Parameters::get_values_for_post_process(post_process_stuffs & stuffs)
{
    stuffs.freq.v = getDoubleFromEntry(*f_entry);
    stuffs.press.v = getDoubleFromEntry(*p_entry);
    stuffs.axial.v = getDoubleFromEntry(*ax_entry);
    stuffs.magn.v = getDoubleFromEntry(*magn_entry);
    stuffs.cycl.v = getDoubleFromEntry(*cycl_entry);
    stuffs.phase.v = getDoubleFromEntry(*fi_entry);
    stuffs.t_rf.v = getDoubleFromEntry(*t_rf_entry);
    stuffs.a_rf.v = getDoubleFromEntry(*a_rf_entry);
}*/
//------------------------------------------------------------------------------
/*void Fit_Parameters::set_values_from_post_process(const post_process_stuffs & stuffs)
{
    putDoubleToEntry(*f_entry, stuffs.freq.v);
    putDoubleToEntry(*p_entry, stuffs.press.v, 1);
    putDoubleToEntry(*ax_entry, stuffs.axial.v);
    putDoubleToEntry(*magn_entry, stuffs.magn.v);
    putDoubleToEntry(*cycl_entry, stuffs.cycl.v, 1);
    putDoubleToEntry(*fi_entry, stuffs.phase.v);
    putDoubleToEntry(*t_rf_entry, stuffs.t_rf.v);
    putDoubleToEntry(*a_rf_entry, stuffs.a_rf.v);
}*/
//------------------------------------------------------------------------------
void Fit_Parameters::copy_results_to_initial_values()
{
    putDoubleToEntry(*f_entry, getDoubleFromEntry(*fitted_f_entry));
    putDoubleToEntry(*p_entry, getDoubleFromEntry(*fitted_p_entry), 1);
    putDoubleToEntry(*ax_entry, getDoubleFromEntry(*fitted_ax_entry));
    putDoubleToEntry(*magn_entry,
		     getDoubleFromEntry(*fitted_magn_entry));
    putDoubleToEntry(*cycl_entry,
		     getDoubleFromEntry(*fitted_cycl_entry), 1);
    putDoubleToEntry(*fi_entry, getDoubleFromEntry(*fitted_fi_entry));
    putDoubleToEntry(*t_tot_entry,
		     getDoubleFromEntry(*fitted_t_rf_entry));
    putDoubleToEntry(*t_RF_ramsey_entry,
		     getDoubleFromEntry(*fitted_t_wait_entry));
    putDoubleToEntry(*a_rf_entry,
		     getDoubleFromEntry(*fitted_a_rf_entry));
}
//------------------------------------------------------------------------------
void Fit_Parameters::pretty_print(std::ostream & os, const char DL)
{
    os << "FREQ" << DL << f_entry->get_text() << DL
	<< ff_button->get_active() << DL
	<< autofreq_button->get_active() << "\n";
    os << "#Others, 2 arguments PARAM	VALUE	FITTED?\n";
    os <<
	"#Units: PRESS [mbar], AXIAL [eV], MAGN [mm], CYCL [mm], PHASE [rad], TIME [s], AMPL [mV]\n";
    os << "PRESS" << DL << p_entry->get_text() << DL
       << fp_button->get_active() << "\n";
    os << "AXIAL" << DL << ax_entry->get_text() << DL
       << fax_button->get_active() << "\n";
    os << "MAGN" << DL << magn_entry->get_text() << DL
       << fmagn_button->get_active() << "\n";
    os << "CYCL" << DL << cycl_entry->get_text() << DL
       << fcycl_button->get_active() << "\n";
    os << "PHASE" << DL << fi_entry->get_text() << DL
       << ffi_button->get_active() << "\n";
    os << "TIME" << DL << t_tot_entry->get_text() << DL
       << ft_rf_button->get_active() << "\n";
    os << "TIME_WAIT" << DL << t_RF_ramsey_entry->get_text() << DL
       << ft_wait_button->get_active() << "\n";
    os << "AMPL" << DL << a_rf_entry->get_text() << DL
       << fa_rf_button->get_active() << "\n";
	os << "#Are T_RF and A_RF got automatically from settings?\n";
	os << "AUTOMAGIC" << DL << auto_t_tot_button->get_active() << DL << auto_conv_button->get_active() << "\n";
//	os << "CRAP" << DL << crap_tof_entry->get_text() << DL << fcrap_tof_button->get_active() << "\n";
//	os << "CRAP_FRACTION" << DL << crap_fraction_entry->get_text() << DL << fcrap_fraction_button->get_active() << "\n";


}
//------------------------------------------------------------------------------
void Fit_Parameters::set_crap_boxes_sensitivity(bool sensitivity) {
	crap_tof_entry->set_sensitive(sensitivity);
	fcrap_tof_button->set_sensitive(sensitivity);
	fitted_crap_tof_entry->set_sensitive(sensitivity);
	fitted_crap_toferr_entry->set_sensitive(sensitivity);

	crap_fraction_entry->set_sensitive(sensitivity);
	fcrap_fraction_button->set_sensitive(sensitivity);
	fitted_crap_fraction_entry->set_sensitive(sensitivity);
	fitted_crap_fractionerr_entry->set_sensitive(sensitivity);
}

//------------------------------------------------------------------------------
void Fit_Parameters::on_auto_conv_button_toggled()
{

}
//------------------------------------------------------------------------------
void Fit_Parameters::on_autofreq_button_toggled()
{

}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
