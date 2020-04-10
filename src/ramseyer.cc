// generated 2007/2/11 17:43:27 EET by tomero@marakatti.(none)
// using glademm V2.6.0
//
// newer (non customized) versions of this file go to ramseyer.cc_new

// This file is for your program, I won't touch it again!

#include <iostream>
#include <fstream>

#include "config.h"
#include "ramseyer.hh"
#include "util.hh"
#include "fitting1.hh"
#include "mjonotpp.hh"
#include "loctm.hh"

#define FREEZE_BUTTONS
//set_button_sensitivity(false);
#define UNFREEZE_BUTTONS
//set_button_sensitivity(true);

#define PII 3.141592653589793

#include "ramseyer_init.inc"

using namespace std;

void
Ramseyer::create_widget(Ramseyer *& win,
			const Glib::RefPtr<Gtk::Builder> & builder)
{
    builder->get_widget_derived("ramseyer", win);
}

Ramseyer::Ramseyer(GtkWindow * obj, const Glib::RefPtr<Gtk::Builder> & builder) : Gtk::Window(obj)
// Ramseyer::Ramseyer(GtkVBox * obj, const Glib::RefPtr<Gtk::Builder> & builder) : Gtk::VBox(obj)
{
    init_widgets(builder);
    init_signals();
    f1_plot_container.set_data(&f1_data);
    f2_plot_container.set_data(&f2_data);
    f3_plot_container.set_data(&f3_data);
    //Window2::create_widget(ramseywindow);
//    ramseywindow = &ramsey_plot;
    show_all_children();
    resize(700,300);
    ramsey_graph.show_xbars();
    fit_func = new FIT_FUNC;
    //ramseywindow->set_plot_title("Ramsey");
	//ramseywindow->move(600,100);
    //ramseywindow->iconify();
    on_enabled_toggled();

    present();
    ramseyer_content_area->pack_start(ramsey_graph);
    plotting_is_active=false;
    std::string build_string = std::string(GIT_VERSION) + ", compiled " + std::string(COMPILE_DATE);
    set_title("Ramseyer " + build_string);

    
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Ramseyer::on_ramsey_value_changed() {
	FREEZE_BUTTONS
	update_gtk();
	validate_spinbuttons();

	plot_stuff_v2();
	UNFREEZE_BUTTONS
}
//------------------------------------------------------------------------------
void Ramseyer::on_ramsey_make_range_clicked() {
	update_gtk();
	if (plotting_is_active)
		return;
	plotting_is_active=true;
	double min,max;
	min = rams_freq1_sp->get_value();	max = min;
	if ( rams_freq2_sp->get_value() > max && freq2_enabled->get_active() )
		max = rams_freq2_sp->get_value();
	if ( rams_freq3_sp->get_value() > max && freq3_enabled->get_active() )
		max = rams_freq3_sp->get_value();

	if ( rams_freq2_sp->get_value() < min && freq2_enabled->get_active() )
		min = rams_freq2_sp->get_value();
	if ( rams_freq3_sp->get_value() < min && freq3_enabled->get_active() )
		min = rams_freq3_sp->get_value();

	ramsey_from_sp->set_value(min-ramsey_plus_minus_range_sp->get_value());
	ramsey_to_sp->set_value(max+ramsey_plus_minus_range_sp->get_value());
	validate_spinbuttons();
	update_gtk();
	plot_stuff_v2();
	plotting_is_active=false;
}
//------------------------------------------------------------------------------
void Ramseyer::on_rams_freq_changed() {
	update_gtk();
	plot_stuff_v2();
}
//------------------------------------------------------------------------------
void Ramseyer::on_ramsey_plotrange_changed() {
	update_gtk();
	plot_stuff_v2();
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Ramseyer::validate_spinbuttons() {
	setRangeToSpinbutton(*ramsey_fringe_sp,1.0,ramsey_time_sp->get_value()/2.);
	update_gtk();
}
//------------------------------------------------------------------------------
/*void Ramseyer::plot_stuff() {
//	if (plotting_is_active)
//		return;
//	plotting_is_active = true;
	Datapoint toAdd1,toAdd2,toAdd3;
	f1.data.clear(); f2.data.clear(); f3.data.clear();
	if (transmission_show_radiob->get_active() ) {
		f1.graph_type=FILLED_LINE; f2.graph_type=FILLED_LINE; f3.graph_type=FILLED_LINE;
		f1.set_color_alpha(0.2); f2.set_color_alpha(0.2); f3.set_color_alpha(0.2);
	}
	if (radius_show_radiob->get_active() ) {
		f1.graph_type=LINE; f2.graph_type=LINE; f3.graph_type=LINE;
	}
	f1.color= Gdk::Color("Red");
	f2.color= Gdk::Color("Blue"); f3.color= Gdk::Color("Black");
	int points_to_plot=1000;
	double step = (ramsey_to_sp->get_value()-ramsey_from_sp->get_value())/(points_to_plot*1.0)*2.*PII;

	double a_rf = 0.028 *400. / (ramsey_fringe_sp->get_value()*2.)*ramsey_conversions_sp->get_value();

	double time_wait = ramsey_time_sp->get_value() - 2.*ramsey_fringe_sp->get_value();

	if (ramsey_from_sp->get_value() < ramsey_to_sp->get_value() ) {
		  // Inserting first zeroes..
			toAdd1.y_value_err = 0;
			toAdd1.x_value = ramsey_from_sp->get_value(); toAdd1.y_value = 0;
			f1.data.push_back(toAdd1);
			if (freq2_enabled->get_active() ) {
				f2.data.push_back(toAdd1);
			}
			if (freq3_enabled->get_active() ) {
				f3.data.push_back(toAdd1);
			}

		toAdd1.y_value_err = 0;toAdd2.y_value_err = 0;toAdd3.y_value_err = 0;
		for (double freq=ramsey_from_sp->get_value()*2.*PII; freq <= ramsey_to_sp->get_value()*2.*PII; freq += step) {
			toAdd1.x_value = freq/2.0/PII; toAdd2.x_value = freq/2.0/PII; toAdd3.x_value = freq/2.0/PII;
			toAdd1.y_value = (fit_func->ramsey_2f_radius_conversion(
					freq, a_rf, ramsey_fringe_sp->get_value()/1000., time_wait/1000., rams_freq1_sp->get_value()*2.*PII,
							 66., 0.05,0 , 0));
			if (transmission_show_radiob->get_active() )
				toAdd1.y_value = get_transmission(toAdd1.y_value)*freq1_abund_sp->get_value()/100.;
			f1.data.push_back(toAdd1);
			if (freq2_enabled->get_active() ) {
				toAdd2.y_value = (fit_func->ramsey_2f_radius_conversion(
						freq, a_rf, ramsey_fringe_sp->get_value()/1000., time_wait/1000., rams_freq2_sp->get_value()*2.*PII,
								 66., 0.05,0 , 0));
				if (transmission_show_radiob->get_active() )
					toAdd2.y_value = get_transmission(toAdd2.y_value)*freq2_abund_sp->get_value()/100.;
				f2.data.push_back(toAdd2);
			}
			if (freq3_enabled->get_active() ) {
				toAdd3.y_value = (fit_func->ramsey_2f_radius_conversion(
						freq, a_rf, ramsey_fringe_sp->get_value()/1000., time_wait/1000., rams_freq3_sp->get_value()*2.*PII,
								 66., 0.05,0 , 0));
				if (transmission_show_radiob->get_active() )
					toAdd3.y_value = get_transmission(toAdd3.y_value)*freq3_abund_sp->get_value()/100.;
				f3.data.push_back(toAdd3);
			}

//			cout << "x y " << toAdd1.x_value << "  " << toAdd1.y_value << "\n";
		}
		{  // Inserting last zeroes..
			toAdd1.y_value_err = 0;
			toAdd1.x_value = ramsey_to_sp->get_value(); toAdd1.y_value = 0;
			f1.data.push_back(toAdd1);
			if (freq2_enabled->get_active() ) {
				f2.data.push_back(toAdd1);
			}
			if (freq3_enabled->get_active() ) {
				f3.data.push_back(toAdd1);
			}
		}

	}
	else {
		//std::cout << "Plotting range is bad, m'kay?\n";
	}
	//Bars
	//NYTH
	//ramseywindow->xbars.delete_bars(); //BAARS
	ramsey_graph.xbars.delete_bar("f1");
	ramsey_graph.xbars.delete_bar("f2");
	ramsey_graph.xbars.delete_bar("f3");
	ramsey_graph.xbars.add_bar(BAR(rams_freq1_sp->get_value(), Gdk::Color("Red"), "freq1", "f1" ));
	if (freq2_enabled->get_active() )
		ramsey_graph.xbars.add_bar(BAR(rams_freq2_sp->get_value(), Gdk::Color("Blue"), "freq2", "f2" ));
	if (freq3_enabled->get_active() )
		ramsey_graph.xbars.add_bar(BAR(rams_freq3_sp->get_value(), Gdk::Color("Black"), "freq3", "f3" ));
	ramsey_graph.set_xtics(5);
	ramsey_graph.set_mxtics(5);
	//Feeding data to plotwindow
	ramsey_graph.input_data(f1);
	if (freq2_enabled->get_active() )
		ramsey_graph.input_data2(f2);
	if (freq3_enabled->get_active() )
		ramsey_graph.input_data3(f3);
	ramsey_graph.autosetPlottingBox();
	ramsey_graph.show_data(true);
	ramsey_graph.show_contents();

	update_gtk();
//	plotting_is_active=false;
}*/
//------------------------------------------------------------------------------
void Ramseyer::plot_stuff_v2() {
//	if (plotting_is_active)
//		return;
//	plotting_is_active = true;
  cout << "Here" << endl;
	Datapoint toAdd1,toAdd2,toAdd3;
	f1_data.clear(); f2_data.clear(); f3_data.clear();
	if (transmission_show_radiob->get_active() ) {
		f1_plot_container.graph_type=FILLED_LINE;
		f2_plot_container.graph_type=FILLED_LINE;
		f3_plot_container.graph_type=FILLED_LINE;
		f1_plot_container.set_color_alpha(0.2);
		f2_plot_container.set_color_alpha(0.2);
		f3_plot_container.set_color_alpha(0.2);
	}
	if (radius_show_radiob->get_active() )
	{
		f1_plot_container.graph_type=LINE;
		f2_plot_container.graph_type=LINE;
		f3_plot_container.graph_type=LINE;
		f1_plot_container.set_color_alpha(1);
		f2_plot_container.set_color_alpha(1);
		f3_plot_container.set_color_alpha(1);
	}
	f1_plot_container.color= Gdk::Color("Red");
	f2_plot_container.color= Gdk::Color("Blue");
	f3_plot_container.color= Gdk::Color("Black");
	int points_to_plot=1000;
	double step = (ramsey_to_sp->get_value()-ramsey_from_sp->get_value())/(points_to_plot*1.0);

	double a_rf = 0.028 *400. / (ramsey_fringe_sp->get_value()*2.)*ramsey_conversions_sp->get_value();

	double time_wait = ramsey_time_sp->get_value() - 2.*ramsey_fringe_sp->get_value();

	if (ramsey_from_sp->get_value() < ramsey_to_sp->get_value() ) {
		  // Inserting first zeroes..
			toAdd1.y_value_err = 0;
			toAdd1.x_value = ramsey_from_sp->get_value(); toAdd1.y_value = 0;
			f1_data.push_back(toAdd1);
			if (freq2_enabled->get_active() ) {
				f2_data.push_back(toAdd1);
			}
			if (freq3_enabled->get_active() ) {
				f3_data.push_back(toAdd1);
			}

		toAdd1.y_value_err = 0;toAdd2.y_value_err = 0;toAdd3.y_value_err = 0;
		for (double freq=ramsey_from_sp->get_value(); freq <= ramsey_to_sp->get_value(); freq += step) {
			toAdd1.x_value = freq; toAdd2.x_value = freq; toAdd3.x_value = freq;
			toAdd1.y_value = (fit_func->ramsey_2f_radius_conversion(
					freq, a_rf, ramsey_fringe_sp->get_value()/1000., time_wait/1000., rams_freq1_sp->get_value(),
							 66., 0.05,0 , 0));
			if (transmission_show_radiob->get_active() )
				toAdd1.y_value = get_transmission(toAdd1.y_value)*freq1_abund_sp->get_value()/100.;
			f1_data.push_back(toAdd1);
			if (freq2_enabled->get_active() ) {
				toAdd2.y_value = (fit_func->ramsey_2f_radius_conversion(
						freq, a_rf, ramsey_fringe_sp->get_value()/1000., time_wait/1000., rams_freq2_sp->get_value(),
								 66., 0.05,0 , 0));
				if (transmission_show_radiob->get_active() )
					toAdd2.y_value = get_transmission(toAdd2.y_value)*freq2_abund_sp->get_value()/100.;
				f2_data.push_back(toAdd2);
			}
			if (freq3_enabled->get_active() ) {
				toAdd3.y_value = (fit_func->ramsey_2f_radius_conversion(
						freq, a_rf, ramsey_fringe_sp->get_value()/1000., time_wait/1000., rams_freq3_sp->get_value(),
								 66., 0.05,0 , 0));
				if (transmission_show_radiob->get_active() )
					toAdd3.y_value = get_transmission(toAdd3.y_value)*freq3_abund_sp->get_value()/100.;
				f3_data.push_back(toAdd3);
			}

//			cout << "x y " << toAdd1.x_value << "  " << toAdd1.y_value << "\n";
		}
		{  // Inserting last zeroes..
			toAdd1.y_value_err = 0;
			toAdd1.x_value = ramsey_to_sp->get_value(); toAdd1.y_value = 0;
			f1_data.push_back(toAdd1);
			if (freq2_enabled->get_active() ) {
				f2_data.push_back(toAdd1);
			}
			if (freq3_enabled->get_active() ) {
				f3_data.push_back(toAdd1);
			}
		}

	}
	else {
		//std::cout << "Plotting range is bad, m'kay?\n";
	}
	//Bars
	//NYTH
	//ramseywindow->xbars.delete_bars(); //BAARS
	ramsey_graph.xbars.delete_bar("f1");
	ramsey_graph.xbars.delete_bar("f2");
	ramsey_graph.xbars.delete_bar("f3");
	ramsey_graph.xbars.add_bar(BAR(rams_freq1_sp->get_value(), Gdk::Color("Red"), "freq1", "f1" ));
	if (freq2_enabled->get_active() )
		ramsey_graph.xbars.add_bar(BAR(rams_freq2_sp->get_value(), Gdk::Color("Blue"), "freq2", "f2" ));
	if (freq3_enabled->get_active() )
		ramsey_graph.xbars.add_bar(BAR(rams_freq3_sp->get_value(), Gdk::Color("Black"), "freq3", "f3" ));
	ramsey_graph.set_xtics(5);
	ramsey_graph.set_mxtics(5);
	//Feeding data to plotwindow
	
	ramsey_graph.clear_plot_data_containers();
	ramsey_graph.add_data(&f1_plot_container);
	if (freq2_enabled->get_active() )
		ramsey_graph.add_data(&f2_plot_container);
	if (freq3_enabled->get_active() )
		ramsey_graph.add_data(&f3_plot_container );
	ramsey_graph.autosetPlottingBox();
	ramsey_graph.show_data(true);
	ramsey_graph.show_contents();

	update_gtk();
//	plotting_is_active=false;
}
//------------------------------------------------------------------------------
void Ramseyer::on_enabled_toggled() {
	rams_freq2_sp->set_sensitive(freq2_enabled->get_active() );
	rams_freq3_sp->set_sensitive(freq3_enabled->get_active() );
	update_gtk();
	plot_stuff_v2();
}
//------------------------------------------------------------------------------
double Ramseyer::get_transmission(double radius) {
	if (radius > cutoff_radius_sp->get_value() )
		return 0;
	if (radius < 0 )
		return -1;

	return ( (1 - radius / cutoff_radius_sp->get_value()))*100.;

	return 0;
}
//------------------------------------------------------------------------------
void Ramseyer::on_debug_values_changed() {
	ramsey_graph.set_some_cairo_stuff(db1->get_value(),db2->get_value(),db3->get_value(),db4->get_value());
	ramsey_graph.show_contents();
}
//------------------------------------------------------------------------------
int Ramseyer::load_ini_file(const Glib::ustring &filename) {
	ifstream openedfile;
	int tempnumero;
	int rownumber(0);
	string errorstring;
	openedfile.open(filename.c_str());
  if (!openedfile) {
		cout << "Could not open ramseyer.ini file!!\n";
		return -1;
	}
	std::string row;
	std::string piece;
	int invalids(0);
	while (getline(openedfile,row)){
		rownumber++;
		if (row[0] == '#')
			continue;
		piece = erota(row,'\t');
		if (piece == "ELEM1"){
			piece = erota(row,'\t');luvuksi(piece,tempnumero);rams_freq1_sp->set_value(tempnumero);
			piece = erota(row,'\t');luvuksi(piece,tempnumero);freq1_abund_sp->set_value(tempnumero);
			continue;
		}
		if (piece == "ELEM2"){
			piece = erota(row,'\t');luvuksi(piece,tempnumero);rams_freq2_sp->set_value(tempnumero);
			piece = erota(row,'\t');luvuksi(piece,tempnumero);freq2_abund_sp->set_value(tempnumero);
			piece = erota(row,'\t');
			if (piece == "1")
				freq2_enabled->set_active(1);
			else
				freq2_enabled->set_active(0);
			continue;
		}
		if (piece == "ELEM3"){
			piece = erota(row,'\t');luvuksi(piece,tempnumero);rams_freq3_sp->set_value(tempnumero);
			piece = erota(row,'\t');luvuksi(piece,tempnumero);freq3_abund_sp->set_value(tempnumero);
			piece = erota(row,'\t');
			if (piece == "1")
				freq3_enabled->set_active(1);
			else
				freq3_enabled->set_active(0);
			continue;
		}
		if (piece == "TOTAL_TIME"){
			piece = erota(row,'\t');luvuksi(piece,tempnumero);ramsey_time_sp->set_value(tempnumero);
			continue;
		}
		if (piece == "FRINGE_TIME"){
			piece = erota(row,'\t');luvuksi(piece,tempnumero);ramsey_fringe_sp->set_value(tempnumero);
			continue;
		}
		if (piece == "CONVERSIONS"){
			piece = erota(row,'\t');luvuksi(piece,tempnumero);ramsey_conversions_sp->set_value(tempnumero);
			continue;
		}
		if (piece == "RANGE"){
			piece = erota(row,'\t');luvuksi(piece,tempnumero);ramsey_from_sp->set_value(tempnumero);
			piece = erota(row,'\t');luvuksi(piece,tempnumero);ramsey_to_sp->set_value(tempnumero);
			continue;
		}
		if (piece == "RANGE_TO_MAKE"){
			piece = erota(row,'\t');luvuksi(piece,tempnumero);ramsey_plus_minus_range_sp->set_value(tempnumero);
			continue;
		}
		if (piece == "TRANSMISSION_FACTOR"){
			piece = erota(row,'\t'); luvuksi(piece,tempnumero);cutoff_radius_sp->set_value(tempnumero);
			continue;
		}
		if (piece == "SHOW_RADIUS"){
			piece = erota(row,'\t');
			if (piece == "1")
				radius_show_radiob->set_active(1);
			else
				transmission_show_radiob->set_active(1);
			continue;
		}
		invalids++;
		errorstring.append("line ");
		char num[5];
		sprintf(num,"%d",rownumber);
		errorstring.append(num);
		errorstring.append("\n");
	}
	//cout << "invalids: " << invalids <<"\n";
	if (invalids > 0)
		std::cout << "Invalid lines in ini-file: " << invalids << endl;
		std::cout << errorstring;
	//cout << "blaa\n";
	return invalids;
}
//------------------------------------------------------------------------------
bool Ramseyer::save_ini_file(const Glib::ustring &filename) {
	ofstream settingsfile;
	settingsfile.open(filename.c_str());
	if (!settingsfile) {
		std::cout << "Could not open the ramseyer settingsfile " << filename << std::endl;
		return 0;
	}
	char DL = '\t';
	loc_tm current_time(0);
	settingsfile.precision(14);
	settingsfile << "#Settings saved at " << loc_date(current_time,loc_tm::d_oma) << " " << loc_time(current_time,loc_tm::t_International) << "\n";
	settingsfile << "ELEM1"	<< DL << rams_freq1_sp->get_value() << DL << freq1_abund_sp->get_value() << "\n";
	settingsfile << "ELEM2"	<< DL << rams_freq2_sp->get_value() << DL << freq2_abund_sp->get_value() << DL << freq2_enabled->get_active() << "\n";
	settingsfile << "ELEM3"	<< DL << rams_freq3_sp->get_value() << DL << freq3_abund_sp->get_value() << DL << freq3_enabled->get_active() << "\n";
	settingsfile << "TOTAL_TIME" << DL << ramsey_time_sp->get_value() << "\n";
	settingsfile << "FRINGE_TIME" << DL << ramsey_fringe_sp->get_value() << "\n";
	settingsfile << "CONVERSIONS" << DL << ramsey_conversions_sp->get_value() << "\n";
	settingsfile << "RANGE" << DL << ramsey_from_sp->get_value() << DL << ramsey_to_sp->get_value() << "\n";
	settingsfile << "RANGE_TO_MAKE" << DL << ramsey_plus_minus_range_sp->get_value() << "\n";
	settingsfile << "TRANSMISSION_FACTOR" << DL << cutoff_radius_sp->get_value() << "\n";
	settingsfile << "SHOW_RADIUS" << DL << radius_show_radiob->get_active() << "\n";

	return 1;
}
//------------------------------------------------------------------------------
void Ramseyer::set_button_sensitivity(bool activate) {
	freq1_abund_sp->set_sensitive(activate);
	freq2_abund_sp->set_sensitive(activate);
	freq3_abund_sp->set_sensitive(activate);

	rams_freq1_sp->set_sensitive(activate);
	rams_freq2_sp->set_sensitive(activate);
	rams_freq3_sp->set_sensitive(activate);

	ramsey_time_sp->set_sensitive(activate);
	ramsey_fringe_sp->set_sensitive(activate);
	ramsey_conversions_sp->set_sensitive(activate);

	ramsey_from_sp->set_sensitive(activate);
	ramsey_to_sp->set_sensitive(activate);
	ramsey_plus_minus_range_sp->set_sensitive(activate);

	cutoff_radius_sp->set_sensitive(activate);
}
//------------------------------------------------------------------------------
