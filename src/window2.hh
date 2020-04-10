// generated 2005/9/13 21:46:47 EEST by tomero@a62-248-137-142.elisa-laajakaista.fi.(none)
// using glademm V2.0.0
//
// newer (non customized) versions of this file go to window2.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _WINDOW2_HH
#define _WINDOW2_HH

# include "config.h"
//# include "ok_cancel_dialog.hh"
# include <gtkmm/builder.h>
# include <sigc++/signal.h>
# include <gtkmm/dialog.h>
# include <gtkmm/drawingarea.h>
# include <gtkmm/entry.h>
# include <gtkmm/filechooserdialog.h>
# include <gtkmm/frame.h>
# include <gtkmm/paned.h>
# include <gtkmm/radiobutton.h>
# include <gtkmm/separator.h>
# include <gtkmm/spinbutton.h>

# include <vector>

# include "gdkmm/color.h"
#include <cairomm/context.h>
#include <gtkmm/uimanager.h>

# include "guiutil.hh"
# include "util.hh"
# include "plotstructures.hh"

#define MAXPOINTS 2500

/*
class RANGE : public sigc::trackable {
 private:
	bool is_entry;
 public:
	Gtk::SpinButton *lefti,*righti;
	Gtk::Entry *leftI, *rightI;
	Gtk::MenuItem *menuthingy;
	Glib::ustring name_of_range;

	double *left_value, *right_value;
	void range_selection_completed(){
		//cout << "tammonen: " << name_of_range << " ja onko entry " << is_entry << "\n";
		//cout << "left, rgiht: " << *left_value << "   " << *right_value << "\n";
		if (is_entry) {
			//cout << "daa :" << *left_value << "\n";
			//cout << "dii :" << asString(*left_value,9) << "\n";
			leftI->set_text(asString(*left_value,9));

		}
		else {
			//lefti->set_snap_to_ticks(true);
			lefti->set_value(*left_value);
			///lefti->set_value(5000);
			//cout << "tama get value " << lefti->get_value() << "\n";
		}
		if (left_value != right_value) {
			if (is_entry)
				rightI->set_text(asString(*right_value,9));
			else
				righti->set_value(*right_value);
			//cout << "tämä tehtiin\n";
		}
	}
	void create(Gtk::SpinButton *leftii, Gtk::SpinButton *rightii, Glib::ustring name_of_rangei){
		lefti = leftii; righti=rightii; name_of_range=name_of_rangei; is_entry = false;
	}
	void create(Gtk::Entry *leftii, Gtk::Entry *rightii, Glib::ustring name_of_rangei){
		leftI = leftii; rightI=rightii; name_of_range=name_of_rangei; is_entry = true;
	}
	void set_values_that_are_assigned_if_event_occurs(double *leftii, double *rightii) {
		left_value = leftii; right_value =rightii;
	}
	RANGE(Gtk::SpinButton *leftii, Gtk::SpinButton *rightii, Glib::ustring name_of_rangei) {
		create(leftii,rightii,name_of_rangei);
	}
	RANGE(Gtk::Entry *leftii, Gtk::Entry *rightii, Glib::ustring name_of_rangei) {
		create(leftii,rightii,name_of_rangei);
	}
};
*/
class optionit : public Gtk::Dialog {
	protected:
	Gtk::HBox YMaxBox,XMaxBox,YMinBox,XMinBox,LogsBox;
	Gtk::CheckButton YMaxToggle,XMaxToggle,YMinToggle,XMinToggle;
	Gtk::HSeparator rator;
	Gtk::CheckButton Logy;
	bool allow_user_to_toggle_logy;
	Gtk::SpinButton YMax,XMax,YMin,XMin;
	// Gtk::Dialog ok_dialog;
	
	public:
	void set_values(bool override_xmin, double overriden_xmin,
	 bool override_xmax, double overriden_xmax,
	 bool override_ymin, double overriden_ymin,
	 bool override_ymax, double overriden_ymax ) {
		YMax.set_value(overriden_ymax);
		YMaxToggle.set_active(override_ymax);
		XMax.set_value(overriden_xmax);
		XMaxToggle.set_active(override_xmax);
		XMin.set_value(overriden_xmin);
		XMinToggle.set_active(override_xmin);
		YMin.set_value(overriden_ymin);
		YMinToggle.set_active(override_ymin);
	};
	optionit(bool override_xmin=0, double overriden_xmin=0,
	 bool override_xmax=0, double overriden_xmax=0,
	 bool override_ymin=0, double overriden_ymin=0,
	 bool override_ymax=0, double overriden_ymax=0,
	 bool logy_scale=0) {

	add_button("Cancel",Gtk::RESPONSE_CANCEL);
	add_button("Apply",Gtk::RESPONSE_APPLY);


		YMax.set_range(-1e15,1e15);
		YMax.set_increments(1,10);
		YMax.set_digits(2);
		YMax.set_value(overriden_ymax);
		YMaxToggle.set_active(override_ymax);
		YMaxBox.show(); YMaxBox.set_homogeneous(false);
		YMaxToggle.show(); YMaxToggle.set_label("Use fixed Y_max");
		YMax.show();

		XMax.set_range(-1e15,1e15);
		XMax.set_increments(1,10);
		XMax.set_digits(2);
		XMax.set_value(overriden_xmax);
		XMaxToggle.set_active(override_xmax);
		XMaxBox.show(); XMaxBox.set_homogeneous(false);
		XMaxToggle.show(); XMaxToggle.set_label("Use fixed X_max");
		XMax.show();

		XMin.set_range(-1e15,1e15);
		XMin.set_increments(1,10);
		XMin.set_digits(2);
		XMin.set_value(overriden_xmin);
		XMinToggle.set_active(override_xmin);
		XMinBox.show(); XMinBox.set_homogeneous(false);
		XMinToggle.show(); XMinToggle.set_label("Use fixed X_min");
		XMin.show();

		YMin.set_range(-1e15,1e15);
		YMin.set_increments(1,10);
		YMin.set_digits(2);
		YMin.set_value(overriden_ymin);
		YMinToggle.set_active(override_ymin);
		YMinBox.show(); YMinBox.set_homogeneous(false);
		YMinToggle.show(); YMinToggle.set_label("Use fixed Y_min");
		YMin.show();

		allow_user_to_toggle_logy=1;
		Logy.set_active(logy_scale);
		Logy.set_label("Logarithmic y");
		LogsBox.pack_start(Logy);

		get_vbox()->pack_start(YMinBox, Gtk::PACK_SHRINK);
		YMinBox.pack_start(YMinToggle, Gtk::PACK_SHRINK);
		YMinBox.pack_start(YMin);

		get_vbox()->pack_start(YMaxBox, Gtk::PACK_SHRINK);
		YMaxBox.pack_start(YMaxToggle, Gtk::PACK_SHRINK);
		YMaxBox.pack_start(YMax);

		get_vbox()->pack_start(XMinBox, Gtk::PACK_SHRINK);
		XMinBox.pack_start(XMinToggle, Gtk::PACK_SHRINK);
		XMinBox.pack_start(XMin);

		get_vbox()->pack_start(XMaxBox, Gtk::PACK_SHRINK);
		XMaxBox.pack_start(XMaxToggle, Gtk::PACK_SHRINK);
		XMaxBox.pack_start(XMax);

		get_vbox()->pack_start(rator);
		get_vbox()->pack_start(LogsBox, Gtk::PACK_SHRINK);

		set_title("Set options");
		show_all_children();

		/*ok_dialog->ok_button->hide();
		ok_dialog->apply_button->set_label("Ok");
		ok_dialog->cancel_button->hide();
		ok_dialog->label->hide();	    */
	}
	//void run(void) { ok_dialog.run(); }
	//void hide(void) {ok_dialog.hide(); }
	//			void get_results(bool &toggledi, double &val){
	void get_results(	bool &ride_xmin, double &riden_xmin,
						bool &ride_xmax, double &riden_xmax,
						bool &ride_ymin, double &riden_ymin,
						bool &ride_ymax, double &riden_ymax,
						bool &use_log_y_scale) {
		ride_ymax = YMaxToggle.get_active(); riden_ymax = YMax.get_value();
		ride_ymin = YMinToggle.get_active(); riden_ymin = YMin.get_value();
		ride_xmax = XMaxToggle.get_active(); riden_xmax = XMax.get_value();
		ride_xmin = XMinToggle.get_active(); riden_xmin = XMin.get_value();
		use_log_y_scale=Logy.get_active();
	}
	void allow_user_to_toggle_log_scale(bool allow_change_to_log_scale=1) {
	  Logy.set_sensitive(allow_change_to_log_scale);
	}

};


class Plottiarea : public Gtk::DrawingArea
{
    virtual void init_signals(void);
	private:
		double drag_start_x;
		double drag_start_y;
		double drag_end_x;
		double drag_end_y;
		Glib::ustring default_window_tittel;
		Gtk::Frame *frame_labeli; bool is_frame_labeli_set;
		Gtk::Window *window_labeli; bool is_window_labeli_set;
		Glib::ustring plot_title;
		double range_l, range_r;
		bool mouse_button_is_depressed;
		optionit *set_axis_dialog;
		double a1,a2,a3,a4;
		Gtk::FileChooserDialog *save_dialog;
		Gdk::Color green,blue,red,black,contour_color;

		Gdk::Color background_color; double background_alpha;

    	Gtk::Menu * right_click_menu;
    	Glib::RefPtr<Gtk::UIManager> popup_uimanager;
    	Glib::RefPtr<Gtk::ActionGroup> popup_action_group;
    	Gtk::Menu * left_click_menu;

		Gtk::MenuItem *save_fitB;
		Gtk::MenuItem *save_datB;
		Gtk::MenuItem *save_new_form_dataB;
		Gtk::MenuItem *put_more_xtics;
		Gtk::MenuItem *put_more_ytics;
		Gtk::MenuItem *put_less_xtics;
		Gtk::MenuItem *put_less_ytics;
		Gtk::MenuItem *options;
		Gtk::MenuItem *save_pdfB, *save_psB, *save_svgB;

		Gtk::MenuItem *range1,*range2,*range3;

		Gtk::MenuItem *markersB;
		//Glib::ustring *outputdir;
		//Glib::ustring fail_default_dir;

		std::vector<ARROW> arrows;

		std::vector<CTS_HISTOGRAM_OF_FREQ> cts_contour; bool show_contour;
		bool showxbars; bool connectxbars;
		bool showybars; bool connectybars;

		bool showData, showData2, showData3;
		bool showFit;
		bool showErrorBars;
		bool showFitError, showLinearFit;
		bool override_xmin,override_xmax,override_ymin,override_ymax;
		double overriden_xmin,overriden_xmax,overriden_ymin,overriden_ymax;
		double xtics,ytics;

		bool log_y_scale;
		double xtic_label_expand,ytic_label_expand;

		int margin_l,margin_r, margin_u,margin_d; //pixels
		int drawingsize_x,drawingsize_y;
		double y_min,y_max,x_min,x_max; //in data (ie. 4501235 Hz)
		int mxtics, mytics;
		double slope,slope_err,constant, const_err, x_ave;

    		// Glib::RefPtr<Gdk::GC> gc;
		Cairo::RefPtr<Cairo::Context> cr;
		Cairo::RefPtr<Cairo::Surface> surface;

		bool after_input_checks(void); //After getting input data, checks for its validity
    void draw_data();
    void add_default_leftclickmenuitems();
//		std::vector<RANGE> ranges; //Those that can selected by pressing and dragging with left mousebutton
		std::vector<Glib::ustring> ranges;
//		std::vector<RANGE> pointmenu_items; //Those that are selected by just clicking left button
		std::vector<Glib::ustring> pointmenu_items;
		
		std::list<PlotDataContainer *> plot_datas;
		
	public:
		void add_data(PlotDataContainer* to_add, bool remove_existing_if_same_name_data_exists = true);
		void clear_plot_data_containers(void){ plot_datas.clear(); }
		void on_range_selection_finished(const Glib::ustring& name_of_range);
		void on_point_selection_finished(const Glib::ustring& name_of_point);

		void allow_user_to_toggle_log_scale(bool allow_user_to_toggle=1){set_axis_dialog->allow_user_to_toggle_log_scale(allow_user_to_toggle);}


		void set_default_window_title(Glib::ustring titel){default_window_tittel = titel;}
		void set_window_labeli(Gtk::Window *tanne);
		void set_frame_labeli(Gtk::Frame *tanne);

		void set_plot_title(const Glib::ustring &titteli){plot_title = titteli;}
		Glib::ustring get_plot_title(){return plot_title;}
		Plottiarea();
		BARS xbars,ybars;
		DATASET dat,dat2,dat3,fit; // NOT SO SAFE!!
		void set_xtics(double interval=5.0){xtics = interval;}
		void set_ytics(double interval=5.0){ytics = interval;}
		//void set_default_output_dir_reference(Glib::ustring *outdir_ref);
		bool on_drawingarea1_button_press_event(GdkEventButton *ev);
		bool on_drawingarea1_button_release_event(GdkEventButton *ev);

		void initialize_drawing(int width = -1, int height = -1);
		void drawingarea1_paint(int width = -1, int height = -1);
		void init_surface(void);
		void refresh_drawing();

		bool on_drawingarea1_configure_event(GdkEventConfigure * event);
#if GTKMM_MAJOR_VERSION < 3
		bool on_drawingarea1_expose_event(GdkEventExpose *event);
#else
    		bool on_drawingarea1_draw(const Cairo::RefPtr<Cairo::Context> & ctx);
#endif
		void on_window2_activate_focus();
		bool on_drawingarea1_motion_notify_event(GdkEventMotion * event);
		bool on_drawingarea1_leave_notify_event(GdkEventCrossing* event);

	//Next: Own functions
		~Plottiarea() {
			//delete save_dialog;
		}
		void override_yrange(double min, double max);
		void set_ymin_with_force(double to_this_value=0.0);
		void set_xmin_with_force(double to_this_value=0.0);
		void set_xmax_with_force(double to_this_value);
		void unset_override_yrange(void) {override_ymin = 0; override_ymax = 0; overriden_ymin = 0; overriden_ymax = 0; refresh_drawing();}
		void on_options_pressed(void);
		void on_save_pdf_pressed(void);
		void on_save_png_pressed(void);
		void on_save_svg_pressed(void);
		//void add_xbars(double x_values[], int how_many, Gdk::Color colori = Gdk::Color("Red")); // ||||
		//void add_xbar(double value, const Gdk::Color colori = Gdk::Color("Red"), Glib::ustring label = "");
		void show_xbars(bool show_it=true);
		void connect_xbars(bool connect=true);

		void add_ybars(double y_values[], int how_many); // --------------
		void add_ybar(double value, Gdk::Color colori = Gdk::Color("Red") );
		void add_ybar(double value, int colori );
		void show_ybars(bool show_it=true);
		void connect_ybars(bool connect=true);

		void add_arrow(double from_x, double from_y, double to_x, double to_y);
		void set_overrides(void);
		void more_xtics(){xtic_label_expand /= 2; show_contents(); }
		void less_xtics(){xtic_label_expand *= 2; show_contents(); }
		void more_ytics(){ytic_label_expand /= 2; show_contents(); }
		void less_ytics(){ytic_label_expand *= 2; show_contents(); }
		void on_save_fit_pressed(void);
		void on_save_dat_pressed(void);
		
		void on_save_new_form_data(void);
		
		bool save_something(DATASET &which_dataset, const std::string &outfilename);

	//	void set_color(const int colori);
		void draw_tic_label(double x, double y, double number,AXIS x_or_y);
		void nullify(void);
		void input_data(double x[], double y[], double y_err[], int how_many_points,const std::string &dataname, GRAPH_TYPE tyyppi=YERR);
//		void input_data(const DATASET &daatta); //Clears data2 if used!
		void clear_data2(void){dat2.data.clear();}
//		void input_data2(const DATASET &daatta);
//		void input_data3(const DATASET &daatta);
  	void input_fit(double x[], double y[], int how_many_points);
		void input_fit(const DATASET &daatta, bool draw_errorlines);
//		void input_linear_fit(double sloup, double konstant, int how_many_points);
//		void input_linear_fit_error(double a_error, double b_error, double x_aver, bool show_lin_fit_error=true);
		void input_bar_data(int data[], int how_many_points);
		void input_bar_data(std::vector<int> numbers);
		void input_bar_data2(std::vector<int> numbers);
		void show_data(bool yes_or_no = true){showData = yes_or_no;}
		void show_fit(bool yes_or_no){showFit = yes_or_no;}
		void show_contents(void);
		void set_mxtics(int how_many=4){mxtics = how_many;}
		void set_mytics(int how_many=5){mytics = how_many;}
		void autosetPlottingBox(void);
		void setPlottingBox(double min_x, double min_y, double max_x, double max_y);
		double lin_fit_err(double x);
		void get_bases(int &x_base, int &y_base);
		void expand_xtics(double expansion_factor);
		void expand_ytics(double expansion_factor);
		void set_graph_style(GRAPH_TYPE tyyppi){dat.graph_type = tyyppi;}
		void set_graph_style2(GRAPH_TYPE tyyppi){dat2.graph_type = tyyppi;}
		void set_dataname(const std::string &name, LEGEND_POSITION where = BOTTOM_LEFT);
		void set_dataname2(const std::string &name, LEGEND_POSITION where = BOTTOM_LEFT);
		void save_data(const std::string &filename);
		void save_fit(const std::string &filename);
		void set_xmin(double value);
		void set_xmax(double value);
		void set_ymin(double value);
		void set_ymax(double value);
		void set_some_cairo_stuff(double a, double b, double c, double d);
		void input_cts_contour(std::vector<CTS_HISTOGRAM_OF_FREQ> &contour);
		void set_contour_color(Gdk::Color);
		const Gdk::Color & get_contour_color(void){return contour_color;}
		void delete_cts_contour();
		double getX(double data_x, bool allow_outside=0); //converts the x from data (ie. 123456 Hz) to screen coordinate (ie. 5)
		double getY(double data_y, bool allow_outside=0); //converts the y from data (ie. 123456 Hz) to screen coordinate (ie. 5)
		bool square_shows_at_least_partly_in_plotting_area(double x1,double y1, double x2, double y2);
		void draw_arrows(void);
		void debugg();
		void set_background_color(const Gdk::Color &color, double alpha=0.2);
		void dump_cairostuff_to_psfile(const std::string & filename, int width = 600, int height = 400);
		void dump_cairostuff_to_pdffile(const std::string & filename, int width = 600, int height = 400);
		void dump_cairostuff_to_pngfile(const std::string & filename, int width = 600, int height = 400);
		void dump_cairostuff_to_svgfile(const std::string & filename, int width = 600, int height = 400);
		//Lisäjuttuja
		bool right_mousebutton_pressed(GdkEventButton *ev);
		bool left_mousebutton_pressed(GdkEventButton *ev);
		void testia();
		bool get_reverse_coordinates(const gdouble screen_x, const gdouble screen_y, double & data_x, double & data_y);
		bool get_reverse_coordinates(const gdouble screen_x, const gdouble screen_y, int & data_x, int & data_y);
		void create_ranges_menu(void);
		void create_point_menu(void);
		void show_range_selection_menu(void);
		void show_point_selection_menu(void);
//		void add_range(Gtk::SpinButton *leftii, Gtk::SpinButton *rightii, Glib::ustring name_of_rangei);
		void add_range(const Glib::ustring & name_of_rangei);
//		void add_choosable_range(const Glib::ustring & name_of_range);
		
		// This signal is emitted when user presses a mouse button
		// and moves the mouse and then releases
		// What is emitted are
		// data x smaller, data x bigger,
		// data y smaller, data y bigger
		// and the name of the range (what the menu says)
        sigc::signal<void, double, double, double, double, std::string> signal_range_selected;
		sigc::signal<void, double, double, std::string> signal_point_selected;

		// void add_singleclickmenuitem(Gtk::SpinButton *attachthis, Glib::ustring name_of_item);
		// void add_singleclickmenuitem(Gtk::Entry *attachthis, Glib::ustring name_of_item);
		void add_singleclickmenuitem(const Glib::ustring & name_of_item);
		void remove_ranges(); //those that can be selected with mouse, but not deleting default ones!!
		bool remove_range(const Glib::ustring & name_of_range_to_remove);
		bool remove_singleclickmenuitem(const Glib::ustring & name_of_item_to_remove);
		void remove_singleclickmenuitems(); //those that can be selected with mouse
		
		void on_default_leftclick_actions_process(const double& x1, const double& y1, const double& x2, const double& y2, const Glib::ustring&);
};


class ViisiPlottiaWindow : public Gtk::Window
{
	Plottiarea *plotarea_a,*plotarea_b,*plotarea_c,*plotarea_d,*plotarea_e;
	Gtk::Frame fa,fb,fc,fd,fe;
	Gtk::Label laabeli;
	Gtk::VPaned vpaned_1,vpaned_2;
	Gtk::HPaned hpaned_1,hpaned_2;
	public:
//# include "window2_members.inc"
		ViisiPlottiaWindow(Plottiarea *a,Plottiarea *b,Plottiarea *c,Plottiarea *d,Plottiarea *e, Glib::ustring default_window_titel);
		void set_default_window_title(const Glib::ustring &titteli);
		/*Plottiarea * get_plotarea_a(){return plotarea_a;}
		Plottiarea * get_plotarea_b(){return plotarea_b;}
		Plottiarea * get_plotarea_c(){return plotarea_c;}
		Plottiarea * get_plotarea_d(){return plotarea_d;}
		Plottiarea * get_plotarea_e(){return plotarea_e;}*/
		void place_window(int x, int y, int w, int h);

};

class YksiPlottiWindow : public Gtk::Window
{
	Plottiarea *plotarea_a;
	public:
//# include "window2_members.inc"
		YksiPlottiWindow(Plottiarea *a, Glib::ustring default_window_titel);
		void set_default_window_title(const Glib::ustring &titteli);
		void place_window(int x, int y, int w, int h);

};







#endif
