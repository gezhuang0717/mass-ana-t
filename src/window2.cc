// generated 2005/9/13 21:46:47 EEST by tomero@a62-248-137-142.elisa-laajakaista.fi.(none)
// using glademm V2.0.0
//
// newer (non customized) versions of this file go to window2.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "window2.hh"

#include "guiutil.hh"
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/stock.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/box.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/window.h>

#include <gdkmm/window.h>
#include <gdkmm/general.h>
#include <cairomm/context.h>
#include <cairomm/surface.h>

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>


using namespace std;

/*#include "window2_init.inc"
void
Window2::create_widget(Window2 *& win)
{
    Glib::RefPtr<Gnome::Glade::Xml> xml = Gnome::Glade::Xml::create_from_buffer(draawingareaa_glade_data, draawingareaa_glade_size, "window2");
    xml->get_widget_derived("window2", win);
}

Window2::Window2(GtkWindow * obj, const Glib::RefPtr<Gnome::Glade::Xml> & xml) : Gtk::Window(obj)
*/
ViisiPlottiaWindow::ViisiPlottiaWindow(Plottiarea *a,Plottiarea *b,Plottiarea *c,Plottiarea *d,Plottiarea *e, Glib::ustring default_window_titteli)
{
	plotarea_a = a;
	plotarea_b = b;
	plotarea_c = c;
	plotarea_d = d;
	plotarea_e = e;

	set_default_window_title(default_window_titteli);

	a->set_frame_labeli(&fa);
	b->set_frame_labeli(&fb);
	c->set_window_labeli(this);
	d->set_window_labeli(this);
	e->set_window_labeli(this);


	fa.set_label(a->get_plot_title());
	fb.set_label(b->get_plot_title());
	fc.set_label(c->get_plot_title());
	fd.set_label(d->get_plot_title());
	fe.set_label(e->get_plot_title());

	fa.set_label_align(0.5);
	fb.set_label_align(0.5);
	fc.set_label_align(0.5);
	fd.set_label_align(0.5);
	fe.set_label_align(0.5);

	//a->set_window_labeli(this);

	fa.add(*plotarea_a);
	fb.add(*plotarea_b);
	fc.add(*plotarea_c);
	fd.add(*plotarea_d);
	fe.add(*plotarea_e);

	laabeli.set_text("daa");
	add(vpaned_1);

	//laabeli.show();
	//add(laabeli);
	//cout << "eventit " << plotarea_a->get_events() << "\n";
	vpaned_1.add1(fa);
	vpaned_1.add2(vpaned_2);
	vpaned_2.add1(fb);
	vpaned_2.add2(hpaned_1);
	hpaned_1.add2(hpaned_2);
	hpaned_1.add1(fc);
	hpaned_2.add1(fd);
	hpaned_2.add2(fe);

	//hpaned1->add2(laabeli);
	//hpaned1->set_position(59);

	resize(600,700);
	vpaned_1.set_position(200);
	vpaned_2.set_position(200);
	hpaned_1.set_position(200);
	hpaned_2.set_position(200);
	show_all_children();

}
void ViisiPlottiaWindow::set_default_window_title(const Glib::ustring &titteli) {
	set_title(titteli);
	plotarea_c->set_default_window_title(titteli);
	plotarea_d->set_default_window_title(titteli);
	plotarea_e->set_default_window_title(titteli);
}

YksiPlottiWindow::YksiPlottiWindow(Plottiarea *a, Glib::ustring default_window_titteli)
{

	plotarea_a = a;
	set_default_window_title(default_window_titteli);


	a->set_window_labeli(this);

	add(*a);


}
void YksiPlottiWindow::set_default_window_title(const Glib::ustring &titteli) {
	set_title(titteli);
	plotarea_a->set_default_window_title(titteli);
}



Plottiarea::Plottiarea() : left_click_menu(0)
{
    init_signals();
	add_events(Gdk::ALL_EVENTS_MASK);

    set_double_buffered(false);
	mouse_button_is_depressed = false;
background_color = Gdk::Color("White"); background_alpha = 0;
	a1=1;a2=1;a3=1;a4=1; //Debugging stuff, not to worry
	green.set_green(65535);
	blue.set_blue(65535);
	red.set_red(65535);
	black.set_rgb_p(0,0,0);
	show_contour = false;
	contour_color.set_rgb_p(0,0,1.0);

	popup_uimanager = Gtk::UIManager::create();
	popup_action_group = Gtk::ActionGroup::create();
	Glib::RefPtr<Gtk::ActionGroup> tmp(popup_action_group);

	tmp->add(Gtk::Action::create("MenuMarkers", "Markers"));
	tmp->add(Gtk::Action::create("MenuSave", "Save"));
	tmp->add(Gtk::Action::create("MenuExport", "Export"));
	tmp->add(Gtk::Action::create("more_xtics", "More xtics"),
		 sigc::mem_fun(*this, &Plottiarea::more_xtics));
	tmp->add(Gtk::Action::create("less_xtics", "Less xtics"),
		 sigc::mem_fun(*this, &Plottiarea::less_xtics));
	tmp->add(Gtk::Action::create("more_ytics", "More ytics"),
		 sigc::mem_fun(*this, &Plottiarea::more_ytics));
	tmp->add(Gtk::Action::create("less_ytics", "Less ytics"),
		 sigc::mem_fun(*this, &Plottiarea::less_ytics));
	tmp->add(Gtk::Action::create("save_data", "Data"),
		 sigc::mem_fun(*this, &Plottiarea::on_save_dat_pressed));
	tmp->add(Gtk::Action::create("save_fit", "Fit"),
		 sigc::mem_fun(*this, &Plottiarea::on_save_fit_pressed));
	tmp->add(Gtk::Action::create("export_pdf", "PDF"),
		 sigc::mem_fun(*this, &Plottiarea::on_save_pdf_pressed));
	tmp->add(Gtk::Action::create("export_png", "PNG"),
		 sigc::mem_fun(*this, &Plottiarea::on_save_png_pressed));
	tmp->add(Gtk::Action::create("export_svg", "SVG"),
		 sigc::mem_fun(*this, &Plottiarea::on_save_svg_pressed));
	tmp->add(Gtk::Action::create("set_axis", "Set axis"),
		 sigc::mem_fun(*this, &Plottiarea::on_options_pressed));

	tmp->add(Gtk::Action::create("save_new_form_data", "SHAVE DATA"),
		 sigc::mem_fun(*this, &Plottiarea::on_save_new_form_data));

	//tmp->remove(tmp->get_action("save_new_form_data")); // to remove menuitem
	popup_uimanager->insert_action_group(tmp);

	Glib::ustring ui_info =
	    "<ui>"
	    "  <popup action='PopupMenu'>"
	    "    <menu action='MenuMarkers'>"
	    "      <menuitem action='MarkersAll'/>"
	    "      <menuitem action='MarkersRemoveAll'/>"
	    "    </menu>"
	    "    <menuitem action='more_xtics'/>"
	    "    <menuitem action='less_xtics'/>"
	    "    <menuitem action='more_ytics'/>"
	    "    <menuitem action='less_ytics'/>"
	    "    <menu action='MenuSave'>"
	    "      <menuitem action='save_data'/>"
	    "      <menuitem action='save_fit'/>"
	    "      <menuitem action='save_new_form_data'/>"
	    "    </menu>"
//	    "    <menu action='MenuSave2'>"
//	    "    </menu>"
	    "    <menu action='MenuExport'>"
	    "      <menuitem action='export_pdf'/>"
	    "      <menuitem action='export_png'/>"
	    "      <menuitem action='export_svg'/>"
	    "    </menu>"
	    "    <menuitem action='set_axis'/>"
	    "  </popup>"
	    "</ui>";

	popup_uimanager->add_ui_from_string(ui_info);
	right_click_menu = (Gtk::Menu *)popup_uimanager->get_widget("/PopupMenu");
	save_fitB = (Gtk::MenuItem *)popup_uimanager->get_widget("/PopupMenu/MenuSave/save_fit");
	save_datB = (Gtk::MenuItem *)popup_uimanager->get_widget("/PopupMenu/MenuSave/save_data");
	save_new_form_dataB = (Gtk::MenuItem *)popup_uimanager->get_widget("/PopupMenu/MenuSave/save_new_form_data");

	//Saving dialog 2006�11-09
	save_dialog = new Gtk::FileChooserDialog("Please choose output file (fit)", Gtk::FILE_CHOOSER_ACTION_SAVE);
	save_dialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	save_dialog->add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);
	save_dialog->set_default_response(Gtk::RESPONSE_OK);

	//Rest
	override_xmin=false;override_xmax=false;override_ymin=false;override_ymax=false;
	overriden_xmin=0;overriden_xmax=0;overriden_ymin=0;overriden_ymax=0;
	showData = false; showFit  = false;
	showData2= false;	showData3= false;
	showErrorBars = true; showFitError = false;
	showLinearFit = false;
	showxbars = false; connectxbars = false;
	showybars = false; connectybars = false;
	log_y_scale = false;
	show();
	//move(0,0);
	set_xtics(); set_ytics(); set_mxtics(); set_mytics();
	margin_l = 50; margin_r=10; margin_u = 5;margin_d=30;
	xtic_label_expand=4,ytic_label_expand=1;
	//y_min =1e11;y_max=-1e11;x_min=1e11;x_max=-1e11;
	y_min=0; x_min=0; y_max=1.0; x_max=1.;

	// gc = get_style()->get_black_gc();
	//init_pixmap();

	//fail_default_dir = ".";
	//outputdir = &fail_default_dir;
	autosetPlottingBox();
	//default legend positions:
	dat.legend_position = BOTTOM_LEFT;
	dat2.legend_position = BOTTOM_LEFT;
	fit.legend_position = BOTTOM_LEFT;

	set_axis_dialog = new optionit();

	is_frame_labeli_set =false;
	is_window_labeli_set =false;
	
	
	// Add default menutimes
	add_default_leftclickmenuitems();
	
	// Let local signal handler get the events from choosings
	signal_range_selected.connect(sigc::mem_fun(*this, &Plottiarea::on_default_leftclick_actions_process));



}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Plottiarea::on_options_pressed(void) {
//	cout << "y_max override, value " << override_ymax <<  " " << overriden_ymax << "\n";


	double a,b,c,d;
	/* This sets default values to the boxes in options. If  */
	/* override is not set, default value is set to be current value. */
	if (override_xmin)		a = overriden_xmin;
	else		a = x_min;
	if (override_xmax)		b = overriden_xmax;
	else		b = x_max;
	if (override_ymin)		c = overriden_ymin;
	else		c = y_min;
	if (override_ymax)		d = overriden_ymax;
	else		d = y_max;

	set_axis_dialog->set_values(	override_xmin,a,
						override_xmax,b,
						override_ymin,c,
						override_ymax,d);

/*	optionit dialoki(	override_xmin,overriden_xmin,
						override_xmax,overriden_xmax,
						override_ymin,overriden_ymin,
						override_ymax,overriden_ymax);	*/
	if ( set_axis_dialog->run() == Gtk::RESPONSE_APPLY )
	{
		set_axis_dialog->get_results(override_xmin,overriden_xmin,
						override_xmax,overriden_xmax,
						override_ymin,overriden_ymin,
						override_ymax,overriden_ymax, log_y_scale);
		set_overrides();
		autosetPlottingBox();
		refresh_drawing();
	}
	set_axis_dialog->hide();

}
//------------------------------------------------------------------------------
void Plottiarea::add_arrow(double from_x, double from_y, double to_x, double to_y) {
	ARROW toAdd;
	toAdd.x1 = from_x;	toAdd.y1 = from_y; toAdd.x2 = to_x; toAdd.y2 = to_y;
	arrows.push_back(toAdd);
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*
void Plottiarea::set_color( colori) {
	switch(colori) {
		//cout << "Setting color to " << colori << "\n";
		case 0: gc->set_rgb_fg_color(red); break;
		case 1: gc->set_rgb_fg_color(blue); break;
		case 2: gc->set_rgb_fg_color(black); break;
		case 3: gc->set_rgb_fg_color(green); break;
	}
}*/
//------------------------------------------------------------------------------
/*void Plottiarea::add_xbars(double x_values[], int how_many, Gdk::Color color){
	xbars.delete_bars();
	BAR toAdd;
	for (int i=0;i<how_many;i++) {
		toAdd.value = x_values[i];
		toAdd.color = color;
		xbars.add_bar(toAdd);
	}
}*/
//------------------------------------------------------------------------------
void Plottiarea::show_xbars(bool show_it){
	showxbars = show_it;
}
//------------------------------------------------------------------------------
void Plottiarea::show_ybars(bool show_it){
	showybars = show_it;
}
//------------------------------------------------------------------------------
void Plottiarea::connect_xbars(bool connect){
	connectxbars = connect;
}
//------------------------------------------------------------------------------
//void Plottiarea::set_default_output_dir_reference(Glib::ustring *outdir_ref){
//	outputdir = outdir_ref;
//	save_dialog->set_current_folder( *outputdir );
//	return;
//}
//------------------------------------------------------------------------------
void Plottiarea::on_save_fit_pressed(void){
	Glib::ustring outfilename;
	save_dialog->set_title("Please choose output file (fit)");
	int result = save_dialog->run();
	save_dialog->hide();
	if (result == Gtk::RESPONSE_OK) {
		outfilename = save_dialog->get_filename();
	}
	else
		return;
	save_something(fit,outfilename);
}
//------------------------------------------------------------------------------
void Plottiarea::on_save_dat_pressed(void){
	Glib::ustring outfilename;
//	Gtk::FileChooserDialog dialog("Please choose output file (data)", Gtk::FILE_CHOOSER_ACTION_SAVE);
	save_dialog->set_title("Please choose output file (data) "+dat.data_name);
//	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
//	dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);
	int result = save_dialog->run();
	save_dialog->hide();
	if (result == Gtk::RESPONSE_OK) {
		outfilename = save_dialog->get_filename();
		save_something(dat,outfilename);
	}

	if (dat2.data.empty() )
		return;

	save_dialog->set_title("Please choose output file (data2) "+dat2.data_name);
	result = save_dialog->run();
	save_dialog->hide();
	if (result == Gtk::RESPONSE_OK) {
		outfilename = save_dialog->get_filename();
	}
	else
		return;
	save_something(dat2,outfilename);


}
//------------------------------------------------------------------------------
bool Plottiarea::save_something(DATASET &which_dataset, const string &outfilename) {
	if (!(which_dataset.data.size() < 1 ) ) {
		boost::filesystem::ofstream outfile;
		if (!open_file_for_appending_and_popup_message_if_error(outfilename, outfile))
			return 0;
		//outfile.open(outfilename.c_str());
		outfile.precision(13);
		for (const Datapoint & dp: which_dataset.data){
			if ((fit == dat) && !showFitError ) {
				outfile << dp.x_value << "\t" << dp.y_value << "\n";
			}
			else
				outfile << dp.x_value << "\t" << dp.y_value << "\t" << dp.y_value_err << "\n";
		}
		outfile.close();
	}
	return true;
}
//------------------------------------------------------------------------------
void Plottiarea::refresh_drawing()
{
	show_contents();
}

// This is called when resizing happens
bool Plottiarea::on_drawingarea1_configure_event(GdkEventConfigure * event)
{
	//cout << "configure eventti\n";
    init_surface();
    drawingarea1_paint();

    return true;
}

void Plottiarea::init_surface(void)
{
    Gdk::Rectangle alloc = get_allocation();
    surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32,
					  alloc.get_width(),
					  alloc.get_height());
    cr = Cairo::Context::create(surface);
    cr->rectangle(0, 0, alloc.get_width(), alloc.get_height());
    cr->set_source_rgb(1.0, 1.0, 1.0);
    cr->fill();
    cr->stroke();
}


void Plottiarea::show_contents(void)
{
    init_surface();
    drawingarea1_paint();
}

//------------------------------------------------------------------------------
#if GTKMM_MAJOR_VERSION < 3
bool Plottiarea::on_drawingarea1_expose_event(GdkEventExpose *event)
{
    //cout << "expose eventti\n";
    Cairo::RefPtr<Cairo::Context> ctx = get_window()->create_cairo_context();
    ctx->set_source(surface, 0, 0);

    ctx->rectangle(event->area.x, event->area.y,
		   event->area.width, event->area.height);
    ctx->clip();
    ctx->paint();
    return false;
}
#else
bool Plottiarea::on_drawingarea1_draw(const Cairo::RefPtr<Cairo::Context> & ctx)
{
    ctx->set_source(surface, 0, 0);
    ctx->paint();
    return false;
}
#endif

//------------------------------------------------------------------------------
void Plottiarea::on_window2_activate_focus()
{
    cout << "activate focus \n";
    show_contents();
}
//------------------------------------------------------------------------------
//After this: OWN FUNCTIONS
/*void Plottiarea::add_xbar(double value, const Gdk::Color colori, Glib::ustring labeli) {
	BAR toAdd;
	toAdd.value = value; toAdd.color=colori;
	toAdd.label = labeli;
	xbars.add_bar(toAdd);
}*/
//------------------------------------------------------------------------------
void Plottiarea::add_ybar(double value, const Gdk::Color colori) {
	BAR toAdd;
	toAdd.value = value; toAdd.color=colori;
	ybars.add_bar(toAdd);
}
//------------------------------------------------------------------------------
void Plottiarea::add_ybar(double value, int colori ) {
	if (colori == 1)
		add_ybar(value, Gdk::Color("Blue"));
	else
		add_ybar(value,Gdk::Color("Black"));
}
//------------------------------------------------------------------------------
void Plottiarea::input_data(double x[], double y[], double y_err[], int how_many_points,
							const string &dataname, GRAPH_TYPE tyyppi)
{
	arrows.clear();
	dat.data.clear();
	dat.data_name = dataname;
	dat.graph_type = tyyppi;
	Datapoint toAdd;
	for (int i=0;i<how_many_points;i++){
		toAdd.x_value = x[i];
		toAdd.y_value = y[i];
		toAdd.y_value_err = y_err[i];
		dat.data.push_back(toAdd);
	}
	return;
}
/*
//------------------------------------------------------------------------------
void Plottiarea::input_data(const DATASET &daatta) {
	arrows.clear();
	dat.data.clear();
	dat.multidata.clear();
	dat2.data.clear();
	dat3.data.clear();
	showData2 = false; showData3 = false;
	dat = daatta;
	after_input_checks();
}
//------------------------------------------------------------------------------
void Plottiarea::input_data2(const DATASET &daatta) {
	dat2.data.clear();
	dat2 = daatta;
	//Default color: blue
	//dat2.color = Gdk::Color("Blue");
	showData2 = true;
	after_input_checks();
}
//------------------------------------------------------------------------------
void Plottiarea::input_data3(const DATASET &daatta) {
	dat3.data.clear();
	dat3 = daatta;
	showData3 = true;
	after_input_checks();
}
//------------------------------------------------------------------------------
*/
//------------------------------------------------------------------------------
void Plottiarea::input_fit(double x[], double y[], int how_many_points)
{
	Datapoint toAdd;
	fit.data.clear();
	fit.color = Gdk::Color("Blue");
	for	(int i=0;i<how_many_points;i++){
		toAdd.x_value = x[i];
		toAdd.y_value = y[i];
		fit.data.push_back(toAdd);
	}
	return;
}
//------------------------------------------------------------------------------
void Plottiarea::input_fit(const DATASET &daatta, bool draw_errorlines) {
	fit.data.clear();
	fit = daatta;
	showFitError = draw_errorlines;
/*	int j(0);
	for (vector<Datapoint>::iterator i=fit.data.begin();i != fit.data.end(); ++i){
		fit_x[j] = i->x_value;
		fit_y[j] = i->y_value;
		j++;
	}
	fitpointnumber = j;	*/
	after_input_checks();
}
//------------------------------------------------------------------------------
void Plottiarea::input_bar_data(int bar_data[], int how_many_points){
	dat.data.clear();
	dat.graph_type = LINE;
	dat.color = Gdk::Color("Blue");
	Datapoint toAdd;
	for (int i=0;i<how_many_points;i++){
		toAdd.x_value=i;
		toAdd.y_value=bar_data[i];
		toAdd.y_value_err=0;
		dat.data.push_back(toAdd);
	}
	showErrorBars = false;
	showFit = false;
	showData = true;
	after_input_checks();
	return;
}
//------------------------------------------------------------------------------
void Plottiarea::input_bar_data(vector<int> numbers) {
	dat.data.clear();
	dat.graph_type = LINE;
	dat.color = Gdk::Color("Blue");
	Datapoint toAdd;
	int num(0);
	for (int i: numbers) {
		toAdd.x_value=num;
		toAdd.y_value=i;
		toAdd.y_value_err=0;
		dat.data.push_back(toAdd);
		num++;
	}
	showErrorBars = false;
	showFit = false;
	showData = true;
	after_input_checks();
	return;

}
//------------------------------------------------------------------------------
void Plottiarea::input_bar_data2(vector<int> numbers) {
	dat2.data.clear();
	dat2.graph_type = LINE;
	dat2.color = Gdk::Color("Green");
	Datapoint toAdd;
	int num(0);
	for (int i: numbers) {
		toAdd.x_value=num;
		toAdd.y_value=i;
		toAdd.y_value_err=0;
		dat2.data.push_back(toAdd);
		num++;
	}
	showErrorBars = false;
	showFit = false;
	showData = true;
	after_input_checks();
	return;

}
//------------------------------------------------------------------------------
void Plottiarea::autosetPlottingBox(void){
	y_min =1e11;y_max=-1e11;x_min=1e11;x_max=-1e11;
	vector<Datapoint>::iterator i;
	vector<Datapoint> *blub;
  for (int datasetti=1; datasetti < 4;datasetti++) {
		if (datasetti == 1)
			blub = &dat.data;
		if (datasetti == 2)
			blub = &dat2.data;
		if (datasetti == 3)
			blub = &dat3.data;
		for ( i=blub->begin();i != blub->end();i++){ //Getting area boundaries from data
			if (blub->empty())
				break;
			if ( (i->y_value - i->y_value_err) < y_min)
				y_min = (i->y_value - i->y_value_err);
			if ( (i->y_value + i->y_value_err) > y_max)
				y_max = (i->y_value + i->y_value_err);
			if ( i->x_value  < x_min )
				x_min = i->x_value;
			if ( i->x_value  > x_max )
				x_max = i->x_value;
	//		cout << "count1 " << count <<"\n";
	//		count++;
		}
	}
	if ( showFitError && !(fit.data.empty()) ) {
		for ( i=fit.data.begin();i != fit.data.end();++i){
			if ( (i->y_value - i->y_value_err) < y_min)
				y_min = (i->y_value - i->y_value_err);
			if ( (i->y_value + i->y_value_err) > y_max)
				y_max = (i->y_value + i->y_value_err);
			if ( i->x_value  < x_min )
				x_min = i->x_value;
			if ( i->x_value  > x_max )
				x_max = i->x_value;
		}
	}
	for (PlotDataContainer * pdc: plot_datas) {
		// cout << pdc->data_name << " <-- Data name, size " << pdc->get_data()->size() << endl;
		if (pdc->get_data()->empty())
			continue;
		for (Datapoint & dp: *pdc->get_data()) {
			if ( (dp.y_value - dp.y_value_err) < y_min)
				y_min = (dp.y_value - dp.y_value_err);
			if ( (dp.y_value + dp.y_value_err) > y_max)
				y_max = (dp.y_value + dp.y_value_err);
			if ( dp.x_value  < x_min )
				x_min = dp.x_value;
			if ( dp.x_value  > x_max )
				x_max = dp.x_value;
		}
		
	}
	//cout << "done\n";
	//std::cout << "x_interval, y_interval" << x_interval << "  " << y_interval << "\n";
//	std::cout << "xbase, ybase" << x_base << "  " << y_base << "\n";

// Put some slack between datapoints and marginals 2006-09-28
	if (dat.graph_type == YERR && !dat.data.empty() ) {
		x_min -= -(x_min - x_max)*0.02;
		x_max += -(x_min - x_max)*0.02;
		y_min -= -(y_min - y_max)*0.02;
		y_max += -(y_min - y_max)*0.02;
	}
	/*if (dat.graph_type == HISTOGRAM) {
		x_max += -(x_min - x_max)*0.02;
		y_max += -(y_min - y_max)*0.02;
	}*/
	if (dat.graph_type == FANCY_HISTOGRAM && !dat.data.empty() ) {
//		x_max += 2.5;
		//cout << "ymax ennen " << y_max << "; log y " << log_y_scale << "\n";
		if (log_y_scale) {
			y_max = y_max*1.2;
			//cout << "ymax jalkeen " << y_max << "\n";
		}
		else {
			y_max += -(y_min - y_max)*0.02;
		}
	}

  //If all values are about the same..
	if (x_max - x_min < 1e-12) {
		x_min /= 3;
		x_max *= 3;
		if (x_min < 1e-12)
			x_min = -1;
		if (x_max < 1e-12)
			x_max = 1;
	}
	if (y_max - y_min < 1e-12) {
		y_min /= 3;
		y_max *= 3;
		if (y_min < 1e-12)
			y_min = -1;
		if (y_max < 1e-12)
			y_max = 1;
	}


	set_overrides();
	// cout << "x_min, x_max" << x_min << " " << x_max << "y, min max " << y_min << " " << y_max << "\n";
	return;
}
//
//------------------------------------------------------------------------------
void Plottiarea::set_overrides(void) {
//If OVERRIDEN:
	if (override_ymax) {
		y_max = overriden_ymax;
	}
	if (override_ymin) {
		y_min = overriden_ymin;
	}
	if (override_xmax) {
		x_max = overriden_xmax;
	}
	if (override_xmin) {
		x_min = overriden_xmin;
	}
}
//------------------------------------------------------------------------------
void Plottiarea::expand_xtics(double expansion_factor){
	xtic_label_expand = expansion_factor;
}
//------------------------------------------------------------------------------
void Plottiarea::expand_ytics(double expansion_factor){
	ytic_label_expand = expansion_factor;
}
//------------------------------------------------------------------------------
void Plottiarea::draw_tic_label(double x, double y, double number,AXIS x_or_y){
	//cout << "LAabeli : " << x << "  " << y << ", numbero: " << number << "\n";
//	int a,b; get_size(a,b);
//	if ( x < 0 || y < 0 || x > a || y > b) {
//		cout << "Some label off-screen!\n";
//	}
	if (y < 0) {
		cout << "Some label off-screen!\n";
		return;
	}
	char numbero[100];
	int base_x,base_y,baze(X);
	get_bases(base_x,base_y);
	if (x_or_y == X)
		baze = base_x;
	if (x_or_y == Y)
		baze = base_y;

	sprintf(numbero,"%.0f",number);
	if (baze > 0) {
		sprintf(numbero,"%.0f",number);
	} else {
 		char format[10];
	    	baze = (-baze) + 1;
	    	sprintf(format, "%%.%df", baze);
		sprintf(numbero, format, number);
	}
//	cout << "Title: " << get_title() << "\n";
//	cout << "number: "<< number << "\n";
//	cout << "numbero: " << numbero << "\n";
//	cout << "Koordinaatit: " << x << "  " << y << "\n";
	//Glib::RefPtr<Pango::Layout> pangolayout = create_pango_layout(numbero);
	//pixmap->draw_layout(gc,x,y,pangolayout);
	//2007-05-11 With Cairo
	cr->save();
	cr->move_to(x,y+12);
	cr->set_source_rgb(0.0,0,0);
	//cr->set_font_size(12);
	cr->show_text(numbero);
  //cr->show_text("Blaa");
	cr->stroke();
	cr->restore();
	return;
}
//------------------------------------------------------------------------------
void Plottiarea::drawingarea1_paint(int iwidth, int iheight)
{
	double u1(0),v1,u2,v2,v3(0);

	cr->select_font_face("Monospace",Cairo::FONT_SLANT_NORMAL,Cairo::FONT_WEIGHT_NORMAL );
	cr->set_font_size(10);
	initialize_drawing(iwidth, iheight);
	//Contour plot
	if (show_contour) {
		cr->save();
		cr->set_line_width(0.0);
		cr->set_source_rgba(1.0,0.0,0.0,0.5);
//		cr->rectangle(100,100,100,200); cr->stroke();
		//Getting maximum counts in a box
		int maximus(0);
		for (const CTS_HISTOGRAM_OF_FREQ & histo: cts_contour) {
			for (int du: histo.counts_in_interval) {
				if (du > maximus)
					maximus = du;
			}
		}
		for (const CTS_HISTOGRAM_OF_FREQ & histo: cts_contour) {
			double lower_y_value = histo.start_tof;
			cr->set_line_width(0);
			for (int du: histo.counts_in_interval) {
				lower_y_value += histo.tof_interval;
				if (du == 0)
					continue;
				double redX = (contour_color.get_red_p() - background_color.get_red_p() )*(du*1.0)/maximus*1.0 + background_color.get_red_p();
				double greenX = (contour_color.get_green_p() - background_color.get_green_p() )*(du*1.0)/maximus*1.0 + background_color.get_green_p();
				double blueX = (contour_color.get_blue_p() - background_color.get_blue_p() )*(du*1.0)/maximus*1.0 + background_color.get_blue_p();

				//cr->set_source_rgba(contour_color.get_red_p(),contour_color.get_green_p(),contour_color.get_blue_p(),(du*1.)/(maximus*1.));
				cr->set_source_rgb(redX,greenX,blueX);

				u1 = getX(histo.center_freq-histo.half_distance_to_left_freq,1)-0.5;
				v1 = getY(lower_y_value-histo.tof_interval,1)-0.5;
				u2 = getX(histo.center_freq+histo.half_distance_to_right_freq,1);
				v2 = getY(lower_y_value,1);
				//Check if square is going outside the plot area
				if (!square_shows_at_least_partly_in_plotting_area(u1,v1,u2,v2))
					continue;
				//Make sure that we dont draw outside the plot area
				if (u1 < margin_l)	u1 = margin_l;
				if (u2 < margin_l)	u2 = margin_l;
				if (u1 > drawingsize_x + margin_l)	u1 = drawingsize_x + margin_l;
				if (u2 > drawingsize_x + margin_l)	u2 = drawingsize_x + margin_l;
				if (v1 < margin_u)	v1 = margin_u;
				if (v2 < margin_u)	v2 = margin_u;
				if (v1 > drawingsize_y + margin_u)	v1 = drawingsize_y + margin_u;
				if (v2 > drawingsize_y + margin_u)	v2 = drawingsize_y + margin_u;
				//if (cr->Cairo::Context::get_target()->Cairo::Surface::get_type() == Cairo::SURFACE_TYPE_PDF)
				//	cr->rectangle(u1,v1,u2-u1-1,v2-v1);
				//else
				cr->rectangle(u1,v1,u2-u1-0.5,v2-v1-0.5);
				cr->fill_preserve();
				cr->stroke();
			}
		}
		cr->restore();
	}
	// Contour plot end

	//Plotting datasets
	DATASET * datasettipointteri = &dat;
	for (int datasetti=1;datasetti < 4; datasetti++) {
		if (!showData)
			break;
		if (datasetti == 1 ) {
			datasettipointteri = &dat;
		}
		if (datasetti == 2 && (!dat2.data.empty())) {
			datasettipointteri = &dat2;
		}
		if (datasetti == 3 && (!dat3.data.empty())) {
			datasettipointteri = &dat3;
		}
		//cout << get_title() << " Graph type: " << datasettipointteri->graph_type << "\n";
		if (showData) {  //SHOWING DATA POINTS
			if (datasettipointteri->data.empty()) {
				//if ( !(datasettipointteri->graph_type == STACKED && !(datasettipointteri->multidata.empty()) ) ) {
					//cout << get_title() << " broke away\n";
				if (datasettipointteri->graph_type == STACKED)
					if (datasettipointteri->multidata.empty() )
						break;
				if (datasettipointteri->graph_type != STACKED)
						break;
			}
			switch(datasettipointteri->graph_type) {
			case YERR: {
				datasettipointteri->set_cairo_rgb(cr);
//				cout << "yerr, datasetti " << datasetti << ", colori: " << datasettipointteri->color << "\n";
				for (const Datapoint & dp: datasettipointteri->data) {
					u1 = getX(dp.x_value);
					v1 = getY(dp.y_value+dp.y_value_err);
					v2 = getY(dp.y_value-dp.y_value_err);
					v3 = getY(dp.y_value);
					//pixmap->draw_line(gc, u1, v1, u1,v2);
					cr->move_to(u1,v1); cr->line_to(u1,v2);
					if (showErrorBars){
						//pixmap->draw_line(gc, u1-2, v1, u1+2,v1); //Top error bar line
						cr->move_to(u1-2,v1); cr->line_to(u1+2,v1);
						//pixmap->draw_line(gc, u1-2, v2, u1+2,v2); //Bottom error bar line
						cr->move_to(u1-2,v2); cr->line_to(u1+2,v2);
					}
					//pixmap->draw_rectangle(gc,true,u1-2,v3-2,5,5); //Data dot
					cr->rectangle(u1-2,v3-2,4,4);
					cr->fill_preserve();
					cr->stroke();
				}
			}
			    break;
			case LINE: {
				//cout << "Line drawing\n";
				cr->save();
				datasettipointteri->set_cairo_rgb(cr);
				cr->move_to(getX(datasettipointteri->data.begin()->x_value),getY(datasettipointteri->data.begin()->x_value));
				for (const Datapoint & dp: datasettipointteri->data) {
					u1 = getX(dp.x_value);
					v1 = getY(dp.y_value);
					cr->line_to(u1,v1);
				}
				cr->stroke();
				cr->restore();
			}
			    break;
			case FILLED_LINE: {
				cr->set_line_width(.5);
				datasettipointteri->set_cairo_rgb(cr);

				//Moving to the first point
				cr->move_to(getX(datasettipointteri->data.begin()->x_value),getY(datasettipointteri->data.begin()->y_value));
				//Plotting the rest of the points
				for (vector<Datapoint>::iterator iter=datasettipointteri->data.begin();iter != datasettipointteri->data.end();++iter){
					iter++;
					if (iter == datasettipointteri->data.end() )
						break;
					cr->line_to(getX(iter->x_value), getY(iter->y_value));
					iter--;
				}
				cr->close_path();
				cr->save();
				datasettipointteri->set_cairo_rgba(cr);
				cr->fill_preserve();
				cr->restore();
				cr->stroke();
			}
			    break;
			case FANCY_HISTOGRAM: {
				datasettipointteri->set_cairo_rgb(cr);
				cr->set_line_width(.5);
				u1 = getX(datasettipointteri->data.begin()->x_value);
				v1 = getY(0);
				cr->move_to(u1,v1);
//				if (dat.data_name == "bin_histogram")
//					cout << "Move to: " << u1 << "   " << v1 << "\n";
				double width;
				double x1 = datasettipointteri->data.begin()->x_value; //cout << "datasize " << datasettipointteri->data.size() << "\n";
				double x2;
				if (datasettipointteri->data.size() == 1) {
					x2 = x1+1;
				}
				else
					x2 = (++datasettipointteri->data.begin())->x_value;
				width = x2-x1;//				cout << "x2 - x1 " << x2-x1 <<"\n";
				for (vector<Datapoint>::iterator iter=datasettipointteri->data.begin();iter != datasettipointteri->data.end();++iter){
					if ( iter == datasettipointteri->data.begin() )
						u1 = getX(iter->x_value);
					else {
						u1 = getX(iter->x_value-width/2.);
					}
					cr->line_to(u1,getY(iter->y_value));
					cr->line_to(getX(iter->x_value+width/2.),getY(iter->y_value));
					iter++;
					if (iter == datasettipointteri->data.end() )
						break;
					cr->line_to(getX(iter->x_value-width/2.),getY(iter->y_value));
					iter--;
				}
				cr->close_path();
				cr->save();
				datasettipointteri->set_cairo_rgba(cr);
				cr->set_line_width(0.0);
				cr->fill_preserve();
				cr->restore();
				cr->stroke();

			}
			    break;
			case STACKED: {
				cr->save();
				cr->set_line_width(0.5);
				//cout << "it is stacked.\n";
				if (datasettipointteri->multidata.empty() )
					break;
				int c1(0);
				vector<int> cts_below; int layers(0);
				for (vector< vector<int> >::iterator iter=datasettipointteri->multidata.begin();iter != datasettipointteri->multidata.end();iter++){
					cr->move_to(getX(0), getY(0));
					for (vector<int>::iterator foo = iter->begin(); foo != iter->end(); foo++) {
						//cout << "line-to " << c1 << "  " << *foo << endl;

						if (iter == datasettipointteri->multidata.begin() ) {
							cts_below.push_back(*foo);
							if ( foo == iter->begin() )
								cr->line_to(getX(c1),getY(*foo));
							else
								cr->line_to(getX(c1-0.5),getY(*foo));
							cr->line_to(getX(c1+0.5),getY(*foo));
						}
						else {
							if ( foo == iter->begin() )
								cr->line_to(getX(c1),getY(*foo+cts_below[c1]));
							else
								cr->line_to(getX(c1-0.5),getY(*foo+cts_below[c1]));
							cr->line_to(getX(c1+0.5),getY(*foo+cts_below[c1]));
							cts_below[c1] += *foo;
						}


						c1++;
					}
					layers++;
					datasettipointteri->set_color_alpha((layers*1.0)/datasettipointteri->multidata.size() );
					//cout << "Set alpha " << (layers*1.0)/datasettipointteri->multidata.size() << "\n";
					datasettipointteri->set_cairo_rgba(cr);
					cr->stroke();
					c1 = 0;
				}
				cr->restore();
			}
			    break;
			default:
			    throw std::invalid_argument("Invalid graph type.");
			}

			//Legend using Cairo
			cr->save();
			datasettipointteri->set_cairo_rgb(cr);
			if (datasettipointteri->legend_position == BOTTOM_LEFT) {
				//std::cout << "Datasetti: " << datasetti << "  naame: " << datasettipointteri->data_name << "\n";
				if (datasetti == 1) {
					cr->move_to(margin_l+5,drawingsize_y+margin_u-18+12);
				}
				if (datasetti == 2)
					cr->move_to(margin_l+5,drawingsize_y+margin_u-35+12);
			} else if (datasettipointteri->legend_position == TOP_LEFT) {
				if (datasetti == 1) {
					cr->move_to(margin_l+5,margin_u+35+12);
				}
				if (datasetti == 2) {
					cr->move_to(margin_l+5,margin_u+18+12);
				}
			}
			if (datasettipointteri->data_name != "") {
				cr->show_text(datasettipointteri->data_name);
				//cr->show_text("Blaa");
				cr->stroke();
			}
			cr->restore();

			//End of setting legend


	//		cout << "dat.data_name " << dat.data_name << ", dat.legend_position " << dat.legend_position << "\n";
	//		cout << "dat2.data_name " << dat2.data_name << ", dat2.legend_position " << dat2.legend_position << "\n";
		}
	}
	if (showFit){
		vector<Datapoint>::iterator iter2;
		fit.set_cairo_rgb(cr);
		cr->set_line_width(1);
		cr->move_to(getX(fit.data.begin()->x_value),getY(fit.data.begin()->y_value));
		for (vector<Datapoint>::iterator iter=fit.data.begin();iter != fit.data.end();iter++){
			u1 = getX(iter->x_value);
			v1 = getY(iter->y_value);
			cr->line_to(u1,v1);
		}
		cr->stroke();
		if (showFitError && !(fit.data.empty()) ){ //Fancier drawing..
			fit.set_cairo_rgba(cr);
			cr->set_line_width(1.0);
			vector<Datapoint>::iterator i=fit.data.begin();
			cr->move_to(getX(fit.data.begin()->x_value),getY(fit.data.begin()->y_value+fit.data.begin()->y_value_err) );
			//Drawing upper error bar
			while (1) {
				u1 = getX(i->x_value);
				v1 = getY(i->y_value+ i->y_value_err);
				cr->line_to(u1,v1);
				i++;

				if ( i == fit.data.end() )
					break;
			}
			cr->stroke();
			cr->move_to(getX(fit.data.begin()->x_value),getY(fit.data.begin()->y_value-fit.data.begin()->y_value_err) );
			//Drawing lower error bar
			i = fit.data.begin();
			while (1) {
				u1 = getX(i->x_value);
				v1 = getY(i->y_value- i->y_value_err);
				cr->line_to(u1,v1);
				//cout << "line_to: " << u1 << "   " << v1 << "\n";
				i++;
				if ( i == fit.data.end() )
					break;
			}
			cr->stroke();

			cr->move_to(getX(fit.data.begin()->x_value),getY(fit.data.begin()->y_value+fit.data.begin()->y_value_err) );
			i = fit.data.begin();
			while (1) {
				u1 = getX(i->x_value);
				v1 = getY(i->y_value+ i->y_value_err);
				cr->line_to(u1,v1);
				//cout << "line_to: " << u1 << "   " << v1 << "\n";
				i++;
				if ( i == fit.data.end() )
					break;
			}
			i--;
			v1 = getY(i->y_value - i->y_value_err); i--;
			cr->line_to(u1,v1);
			while (1) {
				u1 = getX(i->x_value);
				v1 = getY(i->y_value- i->y_value_err);
				cr->line_to(u1,v1);
				if ( i == fit.data.begin() )
					break;
				i--;
			}
			cr->set_line_width(0.0);
			cr->set_source_rgba(fit.Cr(),fit.Cg(),fit.Cb(),0.1);
			cr->fill_preserve();
		}//Fancier
	}
	if (showxbars) {
		for (vector<BAR>::iterator i=xbars.bars.begin();i != xbars.bars.end();++i) {
			if (xbars.bars.empty())
				break;
			u1 = getX(i->value);
			cr->set_source_rgb(i->color.get_red_p(),i->color.get_green_p(),i->color.get_blue_p());
			cr->set_line_width(i->width);
			cr->move_to(u1,margin_u);
			cr->line_to(u1,drawingsize_y+margin_u);
			cr->stroke();
			//2008-01-03 bar label
			if (i->label != "") {
				cr->move_to(u1+2,margin_u+15);
				cr->show_text(i->label);
				cr->stroke();
			}
			//cout << "label " << i->label << ", id " << i->id << "\n";
		}
	}
//	cout << "showybars " << showybars << "\n";
	if (showybars) {
		for (vector<BAR>::iterator i=ybars.bars.begin();i != ybars.bars.end();++i) {
			if (ybars.bars.empty())
				break;
			v3 = getY(i->value);
			cr->set_source_rgb(i->color.get_red_p(),i->color.get_green_p(),i->color.get_blue_p());
			cr->set_line_width(i->width);
			cr->move_to(margin_l,v3);
			cr->line_to(drawingsize_x+margin_l,v3);
		cr->stroke();
		}
	}
	draw_arrows();
	draw_data();
	cr->set_line_width(1);cr->set_source_rgb(0,0,0);
	cr->rectangle(margin_l+0.5,margin_u+0.5,drawingsize_x,drawingsize_y);
	cr->stroke();
	
	queue_draw();
	return;
}

void Plottiarea::draw_data()
{
    if ( plot_datas.empty() )
        return;
    // std::cout << "--------\nSomething changed: " << something_changed << "\n";
    //std::cout << "plot_datas.size() " << plot_datas.size() << "\n";
    // std::cout << "Unit: << " << x_unit << "\n------END\n";
    double u1 ( 0 ),v1,u2,v2,v3 ( 0 );
    int plotcount(0);
	if (! plot_datas.empty())
    for ( std::list<PlotDataContainer *>::reverse_iterator i_p=plot_datas.rbegin(); i_p != plot_datas.rend(); ++i_p ) {
        PlotDataContainer * i = *i_p;
		//std::cout << "size:" << i->get_data()->size() << std::endl;
		if ( i->get_data()->empty() )
		{
			// std::cout << "on empty\n";
			continue;
		}
		if ( i->graph_type == LINE ) {
            //cout << "Line drawing\n";
            cr->save();
            i->set_cairo_rgba ( cr );
            cr->move_to ( getX ( i->get_data()->begin()->x_value ),getY ( i->get_data()->begin()->y_value ) );
            for ( std::vector<Datapoint>::const_iterator iter=i->get_data()->begin(); iter != i->get_data()->end(); ++iter ) {
                u1 = getX ( iter->x_value );
                v1 = getY ( iter->y_value );
                cr->line_to ( u1,v1 );
                //std::cout << "u1:" << u1 << "; v1:" << v1 << ";\n";
            }
            cr->stroke();
            cr->restore();
			continue;
        }

        if ( i->graph_type == YERR ) {
            cr->save();
            i->set_cairo_rgba ( cr );
            for ( std::vector<Datapoint>::const_iterator iter=i->get_data()->begin(); iter != i->get_data()->end(); ++iter ) {
                u1 = getX ( iter->x_value );
                v1 = getY ( iter->y_value+iter->y_value_err );
                v2 = getY ( iter->y_value-iter->y_value_err );
                v3 = getY ( iter->y_value );
                cr->move_to ( u1,v1 );
                cr->line_to ( u1,v2 ); // main vertical bar
                // std::cout << "move to u1:" << u1 << "::v1:" << v1 << "::v2:" << v2 <<"\n";

                if ( i->get_data()->size() < 300 ) {
                    cr->move_to ( u1-1,v1 );
                    cr->line_to ( u1+1,v1 ); // top bar
                    cr->move_to ( u1-1,v2 );
                    cr->line_to ( u1+1,v2 ); // bottom bar
                    cr->rectangle ( u1-1,v3-1,2,2 ); // Dot
                    cr->fill_preserve();
                } else {
                    // std::cout << "move to " << u1-1 << ":::" << v3 << "\n";
                    cr->move_to ( u1-1,v3 );
                    cr->rel_line_to ( 2,0 );
                }
                //std::cout << "dah\n";
            }

            cr->stroke();
            cr->restore();
        }

        if ( i->graph_type == SQUARES_WITHOUT_Y_VALUE )
		{
            cr->save();
            i->set_cairo_rgba ( cr );
            for ( std::vector<Datapoint>::const_iterator iter=i->get_data()->begin(); iter != i->get_data()->end(); ++iter ) {
                u1 = getX ( iter->x_value ) + 0.5;
                v1 = margin_u+4+0.5;
                // cr->rectangle(u1-1,v1-1,2,2);
                //cr->move_to(u1-1,v1-1);

                cr->move_to ( u1-1,v1 );
                cr->rel_line_to ( 2,0 );

            }

            cr->stroke();
            cr->restore();
        }
        if ( i->graph_type == CROSSES_WITHOUT_Y_VALUE )
		{
            cr->save();
            i->set_cairo_rgba ( cr );
            for ( std::vector<Datapoint>::const_iterator iter=i->get_data()->begin(); iter != i->get_data()->end(); ++iter ) {
                u1 = getX ( iter->x_value );
                v1 = margin_u+8+0.5;
                cr->move_to ( u1-1.5,v1-1.5 );
                cr->line_to ( u1+1.5,v1+1.5 );
                cr->move_to ( u1-1.5,v1+1.5 );
                cr->line_to ( u1+1.5,v1-1.5 );
                //std::cout << "dah\n";
            }

            cr->stroke();
            cr->restore();
        }
        if (i->graph_type == FANCY_HISTOGRAM) {
			cr->save();
			cr->set_line_width(1.0);
			i->set_cairo_rgba ( cr );
			u1 = getX(i->get_data()->begin()->x_value);
			v1 = getY(0);
			cr->move_to(u1,v1);
			double width;
			double x1 = i->get_data()->begin()->x_value;
			double x2;
			if (i->get_data()->size() == 1) {
				x2 = x1+1.0;
			}
			else
				x2 = (++i->get_data()->begin())->x_value;
			width = x2-x1;//				cout << "x2 - x1 " << x2-x1 <<"\n";
			for ( std::vector<Datapoint>::const_iterator iter=i->get_data()->begin(); iter != i->get_data()->end(); ++iter )
			{
				// std::cout << "x: " << iter->x_value << ", y: " << iter->y_value << "\n";
				if ( iter == i->get_data()->begin() )
					u1 = getX(iter->x_value);
				else {
					u1 = getX(iter->x_value-width/2.);
				}
				cr->line_to(u1,getY(iter->y_value));
				cr->line_to(getX(iter->x_value+width/2.),getY(iter->y_value));
				iter++;
				if (iter == i->get_data()->end() )
				{
					iter--;
					cr->line_to(getX(iter->x_value+width/2.),getY(0));
					break;
					
				}
				cr->line_to(getX(iter->x_value-width/2.),getY(iter->y_value));
				iter--;
			}
			cr->close_path();
			cr->fill_preserve();
			i->set_cairo_rgb(cr);
			cr->stroke();
			cr->restore();
		}
        
        
        if ( i->graph_type == MARKER_RECTANGLE )
	{
            //std::cout << "dah2\n";
            cr->save();
            i->set_cairo_rgba ( cr );
            //cr->set_source_rgba(i->Cr(),i->Cg(),i->Cb(), 0.2 );
            for ( std::vector<Datapoint>::const_iterator iter=i->get_data()->begin(); iter != i->get_data()->end(); ++iter ) {
                u1 = getX ( iter->x_value );
                v1 = margin_u+4+0.5;
                cr->rectangle ( u1-2,margin_u+2,5,drawingsize_y-2 );
                cr->fill_preserve();
                //std::cout << "dah\n";
            }

            cr->stroke();
            cr->restore();
        }
	if ( i->graph_type == FILLED_LINE)
	{
		cr->save();
		cr->set_line_width(.5);
		i->set_cairo_rgb(cr);

		//Moving to the first point
		cr->move_to(getX(i->get_data()->begin()->x_value),getY(i->get_data()->begin()->y_value));
		//Plotting the rest of the points
		for (vector<Datapoint>::iterator iter=i->get_data()->begin();iter != i->get_data()->end();++iter){
			iter++;
			if (iter == i->get_data()->end() )
				break;
			cr->line_to(getX(iter->x_value), getY(iter->y_value));
			iter--;
		}
		cr->close_path();
		cr->save();
		i->set_cairo_rgba(cr);
		cr->fill_preserve();
		cr->restore();
		cr->stroke();
		cr->restore();
	}
	// Legend
	cr->save();
	i->set_cairo_rgb(cr);
	if (i->legend_position == BOTTOM_LEFT) {
		//std::cout << "Datasetti: " << datasetti << "  naame: " << datasettipointteri->data_name << "\n";
		cr->move_to(margin_l+5,drawingsize_y+margin_u+1-18*plotcount+12);
		//cr->move_to(margin_l+5,drawingsize_y+margin_u-35+12);
	} else if (i->legend_position == TOP_LEFT) {
		cr->move_to(margin_l+5,margin_u+1+17*plotcount+12); // This way the order is reversed but who gares 
		//cr->move_to(margin_l+5,margin_u+18+12);
	}
	if (i->data_name != "") {
		cr->show_text(i->data_name);
		//cr->show_text("Blaa");
		cr->stroke();
	}
	cr->restore();	
	
	// End legend

	plotcount++;
    }
}

//------------------------------------------------------------------------------
void Plottiarea::nullify(void){
	dat.data.clear();
	dat2.data.clear();
	dat3.data.clear();
	cts_contour.clear();
	fit.data.clear();
	arrows.clear();
	xbars.delete_bars();
	ybars.delete_bars();
	showxbars = false;
	showybars = false;
	showData = false;
	showData2= false;
	showFit = false;
	showLinearFit = false;
	showFitError = false;
	setPlottingBox(0,1,0,1);
	show_contents();
}
//------------------------------------------------------------------------------
void Plottiarea::setPlottingBox(double min_x, double min_y, double max_x, double max_y){
	if (min_x > -0.001){
		x_min = min_x;
	}
	if (min_y > -0.001){
		y_min = min_y;
	}
	if (max_x > -0.001){
		x_max = max_x;
	}
	if (max_y > -0.001){
		y_max = max_y;
	}
	return;
}
//------------------------------------------------------------------------------
double Plottiarea::lin_fit_err(double x){
	return sqrt ( pow(slope_err *(x_ave-x) ,2) + pow (const_err ,2)   );

}
//------------------------------------------------------------------------------
void Plottiarea::get_bases(int &x_base, int &y_base){
	double x_interval = fabs(x_max - x_min);
	double y_interval = fabs(y_min - y_max);
	if ( x_interval < 1e-13) {
		x_interval = x_max/100.0;
	}
	if ( y_interval < 1e-13) {
		y_interval = y_max/100.0;
	}
//	cout << "y_interval " << y_interval << "\n";
	for (int base=20; base > -20; base--){
//		std::cout << "base  subtract " << base << "  " << z << "\n";
		if  ((x_interval - 1.0*pow(10.0,base)) < 0  )
			continue;
		x_base = base;
		if  (x_interval > 1.8*pow(10.0,base) )
			x_base++;
//			std::cout << "joo\n";
		break;
	}
	for (int base=20; base > -20; base--){
		if  ((y_interval - 1.0*pow(10.0,base)) < 0  )
			continue;
		y_base = base;
		if  (y_interval > 1.8*pow(10.0,base) )
			y_base++;
		break;
	}
//	std::cout << "xbase ybase " << x_base << "  " << y_base << "\n";
/*	if ((x_base < 20) || x_base > 20)
		x_base = 0;
	if ((y_base < 20) || y_base > 20)
		y_base = 0;*/
	return;
}
//------------------------------------------------------------------------------
void Plottiarea::set_dataname(const string &name, LEGEND_POSITION where) {
	dat.data_name = name;
	dat.legend_position = where;
}
//------------------------------------------------------------------------------
void Plottiarea::set_dataname2(const string &name, LEGEND_POSITION where) {
	dat2.data_name = name;
	dat2.legend_position = where;
}
//------------------------------------------------------------------------------
bool Plottiarea::after_input_checks(void) {
	bool bad(false);
	if (dat.legend_position < 0 || dat.legend_position > 4) {
		dat.legend_position= BOTTOM_LEFT;
		bad = true;
	}
	if (dat2.legend_position < 0 || dat2.legend_position > 4) {
		dat2.legend_position=BOTTOM_LEFT;
		bad = true;
	}
	return bad;
}
//------------------------------------------------------------------------------
void ViisiPlottiaWindow::place_window(int x, int y, int w, int h) {
	move(x,y);
	resize(w,h);
}
//------------------------------------------------------------------------------
void YksiPlottiWindow::place_window(int x, int y, int w, int h) {
	move(x,y);
	resize(w,h);
}
//------------------------------------------------------------------------------
void Plottiarea::save_data(const string &filename) {
	save_something(dat,filename);
}
//------------------------------------------------------------------------------
void Plottiarea::save_fit(const string &filename) {
	save_something(fit,filename);
}
//------------------------------------------------------------------------------
void Plottiarea::set_some_cairo_stuff(double a, double b, double c, double d) {
	a1=a;a2=b;a3=c;a4=d;
}
//------------------------------------------------------------------------------
void Plottiarea::input_cts_contour(std::vector<CTS_HISTOGRAM_OF_FREQ> &contour) {
	cts_contour.clear();
	cts_contour = contour;
	show_contour = true;
}
//------------------------------------------------------------------------------
void Plottiarea::delete_cts_contour() {
	cts_contour.clear();
	show_contour = false;

}
//------------------------------------------------------------------------------
void Plottiarea::set_xmin(double value) {
	x_min = value;
}
//------------------------------------------------------------------------------
void Plottiarea::set_xmax(double value) {
	x_max = value;
}
//------------------------------------------------------------------------------
void Plottiarea::set_ymin(double value) {
	y_min = value;
}
//------------------------------------------------------------------------------
void Plottiarea::set_ymax(double value) {
	y_max = value;
}
//------------------------------------------------------------------------------
void Plottiarea::set_contour_color(Gdk::Color kolori) {
	contour_color = kolori;
	show_contents();
}
//------------------------------------------------------------------------------
void Plottiarea::debugg() {

}
//------------------------------------------------------------------------------
void Plottiarea::initialize_drawing(int iwidth, int iheight) {
	if ( iwidth < 0 || iheight < 0 ) {
		//get_size(drawingsize_x,drawingsize_y);  //gets window dimensions
		Gtk::Allocation allocation = get_allocation();
		drawingsize_x = allocation.get_width();
		drawingsize_y = allocation.get_height();
	}
	else {
		drawingsize_x = iwidth; drawingsize_y = iheight;
	}
	cr->set_source_rgb(background_color.get_red_p(),background_color.get_green_p(),
                      background_color.get_blue_p());
	cr->paint_with_alpha(background_alpha);
	cr->set_line_width(1.0);




	int x_base = 1;
	int y_base = 1;
	get_bases(x_base,y_base); //Gets least significant digit in tics
	set_xtics( pow(10.0,x_base-1.0)*xtic_label_expand); //cout << "x_base " << x_base << " xtic_label_expand " << xtic_label_expand << "\n";
	set_ytics( pow(10.0,y_base-1.0)*ytic_label_expand);

	int marginal(0);
	if (y_base < 0)
		marginal += abs(y_base);

	for (int base=20; base > -20; base--){ //This will push left margin more right so that number still fits
		if  (( fabs(y_max) - 1.0*pow(10.0,base)) < 0  )
			continue;
		marginal += abs(base);
//		std::cout << "tämä : " << base << "\n";
		break;
	}
	margin_l = marginal*10+20;


	drawingsize_x -= margin_l+margin_r;
	drawingsize_y -= margin_u+margin_d;
	cr->set_line_width(1.0);
	cr->set_source_rgb(0,0,0);

	//Drawing tics
	for (double i=floor(x_min);i<x_max;i+=xtics){		//Major x tics
		if (i < (x_min-2.0*xtics))
			continue;
		if ( (i >= x_min) && (i <= x_max)){
			cr->move_to(getX(i,1),drawingsize_y+margin_u);
			cr->line_to(getX(i,1),drawingsize_y+margin_u+10);
			draw_tic_label(getX(i)-5,drawingsize_y+margin_u+10,i,X);
		}
		for (double j = i; j < i+xtics;j+=(xtics/(mxtics*1.0))){ //Minor x tics
			if (j > x_max)
				break;
			if (j < x_min)
				continue;
			cr->move_to(getX(j,1),drawingsize_y+margin_u);
			cr->line_to(getX(j,1),drawingsize_y+margin_u+5);
		}
	}

	for (double i=floor(y_min);i<y_max;i+=ytics){		//Major y tics
		if (i < (y_min-2.0*ytics))
			continue;
		if ( (i >= y_min) && (i <= y_max)){
			cr->move_to(margin_l,getY(i,1));
			cr->line_to(margin_l-10,getY(i,1));
			//pixmap->draw_line(gc,margin_l,getY(i),margin_l-10,getY(i));
			draw_tic_label(0,getY(i)-5,i,Y);
		}
		for (double j = i; j < i+ytics;j+=(ytics/(mytics*1.0))){ //Minor y tics
			if (j > y_max)
				break;
			if (j < y_min)
				continue;
			cr->move_to(margin_l,getY(j,1));
			cr->line_to(margin_l-5,getY(j,1));
			//pixmap->draw_line(gc,margin_l,getY(j),margin_l-5,getY(j));
		}
	}
	cr->stroke();
}
//------------------------------------------------------------------------------
double Plottiarea::getX(double x_value, bool allow_outside) {
	if (!allow_outside) {
		if (x_value < x_min)
			x_value = x_min;
		if (x_value > x_max)
			x_value = x_max;
	}
	return static_cast<int>((drawingsize_x / (x_max - x_min) )*(x_value-x_min)+margin_l+0.5)+0.5;
}
//------------------------------------------------------------------------------
double Plottiarea::getY(double y_value, bool allow_outside) {
	if (!allow_outside) {
		if (y_value < y_min)
			y_value = y_min;
		if (y_value > y_max)
			y_value = y_max;
	}
	if (!log_y_scale) {
		return static_cast<int>(drawingsize_y*(1.0-(y_value- y_min)/(y_max-y_min))+margin_u+0.5) +0.5;
	}
	else {
		double y_min_tmp = y_min;
		if (y_value <= 0 && y_min_tmp <= 0) {
			return drawingsize_y + margin_u + 0.5;
		}
	    if (y_value <= 0) {
		  y_value = 0.11;
		}
		if (y_min_tmp <=0) {
		 y_min_tmp = 0.1;
		}
		double num =static_cast<int>(drawingsize_y*(1.0-(log10(y_value)- log10(y_min_tmp))/(log10(y_max)-log10(y_min_tmp)))+margin_u+0.5) +0.5;
		return num;
	}

}
//------------------------------------------------------------------------------
bool Plottiarea::square_shows_at_least_partly_in_plotting_area(double x1,double y1, double x2, double y2) {
	//NOTE: Screen coordinates! 0,0 is left,up!
	double x_1(x1), x_2(x2), y_1(y1), y_2(y2);
	if (x_1 > x_2) {
		double vaihto = x_1;		x_1 = x_2;		x_2 = vaihto;
	}
	if (y_1 > y_2) {
		double vaihto = y_1;		y_1 = y_2;		y_2 = vaihto;
	}
	if (x_2 < margin_l || (x_1 > margin_l+drawingsize_x))
		return false;
	if (y_2 < margin_u || (y_1 > margin_u+drawingsize_y))
		return false;


	return true;
}
//------------------------------------------------------------------------------
void Plottiarea::draw_arrows(void) {
	cr->save();
	cr->set_line_width(0.4);
	cr->set_source_rgb(0,.6,0);
	cr->set_line_cap(Cairo::LINE_CAP_SQUARE);
	//cr->paint_with_alpha(.4);
	if ( !(arrows.empty() )) {
		for (vector<ARROW>::iterator i = arrows.begin(); i != arrows.end();++i) {
			//u1 =static_cast<int>((a / (x_max - x_min) )*(i->x1-x_min)+margin_l+0.5); //center x on screen
			//v1 =static_cast<int>(b*(1.0-(i->y1- y_min)/(y_max-y_min))+margin_u+0.5); //center y
			//u2 =static_cast<int>((a / (x_max - x_min) )*(i->x2-x_min)+margin_l+0.5); //center x on screen
			//v2 =static_cast<int>(b*(1.0-(i->y2- y_min)/(y_max-y_min))+margin_u+0.5); //center y
			//pixmap->draw_line(gc, u1, v1, u2,v2);
			cr->move_to(getX(i->x1),getY(i->y1));
			cr->line_to(getX(i->x2),getY(i->y2));
			cr->stroke();
		}
	}
	cr->restore();
}
//------------------------------------------------------------------------------
void Plottiarea::set_background_color(const Gdk::Color &color, double alphaxx) {
	background_color = color;
	background_alpha = alphaxx;
	drawingarea1_paint();
}
//------------------------------------------------------------------------------
void Plottiarea::dump_cairostuff_to_psfile(const std::string & filename, int width, int height) {
	Cairo::RefPtr<Cairo::Context> store;
	Cairo::RefPtr<Cairo::PsSurface> surface = Cairo::PsSurface::create(filename, width, height);
	store = Cairo::Context::create(surface);
	cr.swap(store);
	cr->save();
	drawingarea1_paint(width,height);
	cr->restore();
	cr->show_page();
	store.swap(cr);
}
//------------------------------------------------------------------------------
void Plottiarea::dump_cairostuff_to_pdffile(const std::string & filename, int width, int height) {
	Cairo::RefPtr<Cairo::Context> store;
	Cairo::RefPtr<Cairo::PdfSurface> surface = Cairo::PdfSurface::create(filename, width, height);
	store = Cairo::Context::create(surface);
	cr.swap(store);
	cr->save();
	drawingarea1_paint(width,height);
	cr->restore();
	cr->show_page();
	store.swap(cr);
}
//------------------------------------------------------------------------------
void Plottiarea::on_save_pdf_pressed(void) {
	save_dialog->set_title("Export as PDF...");
	save_dialog->set_current_name(".pdf");
	int result = save_dialog->run();
	save_dialog->hide();
	if (result != Gtk::RESPONSE_OK)
		return;
	dump_cairostuff_to_pdffile(save_dialog->get_filename());
}
//------------------------------------------------------------------------------
void Plottiarea::on_save_png_pressed(void) {
	save_dialog->set_title("Export as PNG...");
	save_dialog->set_current_name(".png");
	int result = save_dialog->run();
	save_dialog->hide();
	if (result != Gtk::RESPONSE_OK)
		return;
	dump_cairostuff_to_pngfile(save_dialog->get_filename());
}
//------------------------------------------------------------------------------
void Plottiarea::dump_cairostuff_to_pngfile(const std::string & filename, int width, int height) {
	Cairo::RefPtr<Cairo::Context> store;
	Cairo::RefPtr<Cairo::ImageSurface> surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, width, height);
	store = Cairo::Context::create(surface);
	cr.swap(store);
	cr->save();
	drawingarea1_paint(width,height);
	surface->write_to_png(filename);
	cr->restore();
	store.swap(cr);
}
//------------------------------------------------------------------------------
void Plottiarea::on_save_svg_pressed(void) {
	save_dialog->set_title("Export as SVG...");
	save_dialog->set_current_name(".svg");
	int result = save_dialog->run();
	save_dialog->hide();
	if (result != Gtk::RESPONSE_OK)
		return;
	dump_cairostuff_to_svgfile(save_dialog->get_filename());
}
//------------------------------------------------------------------------------
void Plottiarea::dump_cairostuff_to_svgfile(const std::string & filename, int width, int height) {
	Cairo::RefPtr<Cairo::Context> store;
	Cairo::RefPtr<Cairo::SvgSurface> surface = Cairo::SvgSurface::create(filename, width, height);
	store = Cairo::Context::create(surface);
	cr.swap(store);
	cr->save();
	drawingarea1_paint(width,height);
	cr->restore();
	cr->show_page();
	store.swap(cr);
}

//------------------------------------------------------------------------------
void Plottiarea::set_frame_labeli(Gtk::Frame *tanne) {
	frame_labeli = tanne;
	is_frame_labeli_set = true;

}
void Plottiarea::set_window_labeli(Gtk::Window *tanne) {
	window_labeli = tanne;
	is_window_labeli_set = true;

}
//------------------------------------------------------------------------------
void Plottiarea::override_yrange(double min, double max) {
	overriden_ymin = min; overriden_ymax = max;
	override_ymin = 1; override_ymax = 1;
	set_overrides();
	refresh_drawing();
}
//------------------------------------------------------------------------------
void Plottiarea::set_xmin_with_force(double to_this_value)
{
	overriden_xmin = to_this_value;
	override_xmin = 1;
	set_overrides();
	refresh_drawing();
}
//------------------------------------------------------------------------------
void Plottiarea::set_xmax_with_force(double to_this_value)
{
	overriden_xmax = to_this_value;
	override_xmax = 1;
	set_overrides();
	refresh_drawing();
}
//------------------------------------------------------------------------------
void Plottiarea::set_ymin_with_force(double to_this_value)
{
	overriden_ymin = to_this_value;
	override_ymin = 1;
	set_overrides();
	refresh_drawing();
}

//------------------------------------------------------------------------------



void Plottiarea::add_data(PlotDataContainer* to_add, bool remove_existing_if_same_name_data_exists)
{
	if (remove_existing_if_same_name_data_exists && plot_datas.size() > 0)
	{
		for (std::list<PlotDataContainer *>::iterator i = plot_datas.begin(); i != plot_datas.end(); ++i)
		{
			if (to_add == (*i) ) // In case the pointers point to same location
			{
				// cout << to_add << " ggg " << (*i) << "\n";
				i = plot_datas.erase(i);
			}
		}
	}
	plot_datas.push_back(to_add);
}












void Plottiarea::init_signals(void)
{
	//cout << "initoidaan drawingarean signaalit\n";
	show();
    signal_button_press_event().connect(
      sigc::mem_fun(*this, &Plottiarea::on_drawingarea1_button_press_event), false);
    signal_configure_event().connect(
      sigc::mem_fun(*this, &Plottiarea::on_drawingarea1_configure_event), false);

#if GTKMM_MAJOR_VERSION < 3
    signal_expose_event().connect(
	sigc::mem_fun(*this, &Plottiarea::on_drawingarea1_expose_event),
	false);
#else
    signal_draw().connect(
      sigc::mem_fun(*this, &Plottiarea::on_drawingarea1_draw), false);
#endif

    signal_motion_notify_event().connect(
      sigc::mem_fun(*this, &Plottiarea::on_drawingarea1_motion_notify_event), false);
    signal_leave_notify_event().connect(
      sigc::mem_fun(*this, &Plottiarea::on_drawingarea1_leave_notify_event), false);
    signal_button_release_event().connect(
      sigc::mem_fun(*this, &Plottiarea::on_drawingarea1_button_release_event), false);

}
