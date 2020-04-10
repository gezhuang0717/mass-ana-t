// generated 2006/1/3 23:30:10 EET by tomero@a84-231-125-139.elisa-laajakaista.fi.(none)
// using glademm V2.6.0
//
// newer (non customized) versions of this file go to postProcessor.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _POSTPROCESSOR_HH
#define _POSTPROCESSOR_HH

# include "config.h"

#include <gtkmm/comboboxtext.h>
#include <gtkmm/radiobutton.h>

# include "window2.hh"
# include "loctm.hh"
// # include "dialog1.hh"
# include "ame_stuff.hh"

class Fit_Parameters;
class Inputting;
#include "postProcessor_headers.inc"

#include <vector>
#include <algorithm>
#include <iostream>

struct param_s {
    double v;
    double d;
    double dd;

    double w() const {
	if (d < 1e-14)
	    return 1.0e11;
	return 1.0/d/d;
    };
};

typedef struct param_s param_t;

struct post_process_data {
    param_t freq;	// ddfreq with b-field fluctuation
    param_t freq2;	// second frequency
    param_t fraction; 	// freq2/(sigma freq)
    param_t press;
    param_t axial;
    param_t magn;
    param_t cycl;
    param_t phase;
    param_t a_rf;
    param_t t_rf;
    param_t freq_ratio;		// freq ratio
    param_t q_value;  		// Q-value
    param_t m_exc;		// mass excess
    param_t interpolated_ref_freq;
    double_t q_weight,q_weight_q, m_weight, m_weight_m; //UNNORMALISED weight; and val*w
    int scan_number;				//ie. Zr97_10 ( number 10)
    int A;		//Atomic number A
    int q;		//charge state q
    int scans_number; //How many scans done ie. zr97_1...zr97_10 -> 10
    double ions_per_shot;
    double counts_in_class;
    double counts_in_all_classes;
    param_t z_slope;
    param_t z_const;
    double fit_chisq;
    int laku_versio;

    post_process_data() { memset(this, 0, sizeof (*this)); }

    typedef std::vector<param_t post_process_data::*> members;
    members get_data_members() const {
	std::vector<param_t post_process_data::*> tmp;
	tmp.push_back(&post_process_data::freq);
	tmp.push_back(&post_process_data::freq2);
	tmp.push_back(&post_process_data::press);
	tmp.push_back(&post_process_data::axial);
	tmp.push_back(&post_process_data::magn);
	tmp.push_back(&post_process_data::cycl);
	tmp.push_back(&post_process_data::phase);
	tmp.push_back(&post_process_data::a_rf);
	tmp.push_back(&post_process_data::t_rf);
	tmp.push_back(&post_process_data::z_const);
	tmp.push_back(&post_process_data::z_slope);
	return tmp;
    }

    static void get_axial(const post_process_data & data,
			  double & value, double & err) {
	value = data.axial.v; err = data.axial.d;
    }
    static void get_counts_in_all_classes(const post_process_data & data,
				    double & value, double & err) {
	value = data.counts_in_all_classes; err = 0;
    }

    static void get_counts_in_class(const post_process_data & data,
				    double & value, double & err) {
	value = data.counts_in_class; err = 0;
    }

    static void get_cyclotron(const post_process_data & data,
			      double & value, double & err) {
	value = data.cycl.v; err = data.cycl.d;
    }
    static void get_fraction(const post_process_data & data,
			      double & value, double & err) {
	value = data.fraction.v; err = data.fraction.d;
    }
    static void get_frequency(const post_process_data & data,
			      double & value, double & err) {
	value = data.freq.v; err = data.freq.d;
    }
    static void get_frequency2(const post_process_data & data,
			       double & value, double & err) {
	value = data.freq2.v; err = data.freq2.d;
    }
    static void get_ions_per_shot(const post_process_data & data,
				  double & value, double & err) {
	value = data.ions_per_shot; err = 0;
    }
    static void get_magnetron(const post_process_data & data,
			      double & value, double & err) {
	value = data.magn.v; err = data.magn.d;
    }
    static void get_phase(const post_process_data & data,
			  double & value, double & err) {
	value = data.phase.v; err = data.phase.d;
    }
    static void get_pressure(const post_process_data & data,
			  double & value, double & err) {
	value = data.press.v; err = data.press.d;
    }
    static void get_rf_amplitude(const post_process_data & data,
				 double & value, double & err) {
	value = data.a_rf.v; err = data.a_rf.d;
    }
    static void get_rf_time(const post_process_data & data,
			    double & value, double & err) {
	value = data.t_rf.v; err = data.t_rf.d;
    }
    static void get_z_slope(const post_process_data & data,
			    double & value, double & err) {
	value = data.z_slope.v; err = data.z_slope.d;
    }
    static void get_z_const(const post_process_data & data,
			    double & value, double & err) {
	value = data.z_const.v; err = data.z_const.d;
    }
    static void get_fit_chisq(const post_process_data & data,
			      double & value, double & err) {
	value = data.fit_chisq; err = 0;
    }
};

class post_process_stuffs : public post_process_data {
	public:

	std::string time_of_scan; //2006-12-12 14:33:12
	std::string element;  //ie zr97_3

	std::string output_directory;
	std::string post_process_filename;
	std::string meas_out_file;
	std::string ref_out_file;


	post_process_stuffs(){}
	~post_process_stuffs(){}
	int get_time_diff(const post_process_stuffs &to_this); //Returns < 0 if arg was before itself and > 0 if arg was after
	bool operator<(const post_process_stuffs &verrokki) const {
		if ( time_t(loc_tm(time_of_scan)) - time_t(loc_tm(verrokki.time_of_scan)) < 0)
			return true;
		else
			return false;
	}
	bool operator>(const post_process_stuffs &verrokki) const {
		if ( time_t(loc_tm(time_of_scan)) - time_t(loc_tm(verrokki.time_of_scan)) < 0)
			return false;
		else
			return true;
	}
	bool operator==(const post_process_stuffs &verrokki) const {
		if ( time_t(loc_tm(time_of_scan)) - time_t(loc_tm(verrokki.time_of_scan)) == 0)
			return true;
		return false;
	}

};

struct ref_meas_block{ //used as: ref1 meas1 ref2 OR REVERSE meas1 ref1 meas2
	std::vector<post_process_stuffs>::iterator meas1,meas2,ref1,ref2;
};

struct ONE_SET{ //Used to store valid sets of data, ie. r-m-r-m-r
	std::vector<int> refs, meass;
};
class FIRST_LAST_FREQS{
	public:
	std::vector<ref_meas_block>::iterator first_block, last_block,first_rev_block, last_rev_block;
	bool only_one_reverse_pair_or_less;

	FIRST_LAST_FREQS(){only_one_reverse_pair_or_less = true;}
};
struct VALUE_AND_ERROR{
	double value, error;
};

enum WHICH_FILE{
	REF = 0,
	MEAS = 1
};

post_process_stuffs get_average(const std::vector<post_process_stuffs> &);

struct Post_Processor_data {
		double mass_excess, dmass_excess, mass_excess_chisq;
		double q_value, dq_value, q_value_chisq, ddq_value;
		double freq_ratio, dfreq_ratio, freq_ratio_chisq, ddfreq_ratio,dddfreq_ratio,ddddfreq_ratio;
		//2007-09-25 Birge ratio stuff
		double inner_error,outer_error;
		double m_e,u_in_keV,e;
		int revision;
    Post_Processor_data() { memset(this, 0, sizeof (*this)); }
};

class Post_Processor : public Gtk::Window, private Post_Processor_data
{
#include "postProcessor_members.inc"
	private:
		// Stuff for upper plot window
		PlotDataContainer upper_plot_data1_container, upper_plot_data2_container;
		std::vector<Datapoint> upper_plot_data1, upper_plot_data2;

		// Stuff for lower plot window
		PlotDataContainer lower_plot_data1_container, lower_plot_data2_container;
		std::vector<Datapoint> lower_plot_data1, lower_plot_data2;
		
	      	post_process_stuffs default_post_process_stuff;
		mass_table ame_mass_table;
		mass_table * ame_mass_table_p;

		bool is_reading_settingfile;
//		Dialog1 * dialoki;
		Gdk::RGBA red_color, default_widget_base_color;
		Gtk::ComboBoxText otherItems, ref_comboboxtext, meas_comboboxtext;
		//YksiPlottiWindow * shower_window;
		Plottiarea *shower, *shower2;
		Plottiarea showerr, showerr2;
		std::string nubtab_filename;
		std::string used_error;

		double get_interpolated_freq(const post_process_stuffs &ref1, const post_process_stuffs &ref2, const std::string &time_to_interpolate);
	  double get_interpolated_freq_err(const post_process_stuffs &ref1, const post_process_stuffs &ref2, const std::string &time_to_interpolate);
		double get_time_between_references(const post_process_stuffs &ref1, const post_process_stuffs &ref2);
	public:
    		//void init(post_process_stuffs *iStuff);
		void init();
		void init_mass_table(mass_table * mass_table);
		~Post_Processor();
		virtual bool on_delete_event 	(	GdkEventAny* 	event 	 );

		bool get_index(WHICH_FILE ref_or_meas, int index_number, std::vector<post_process_stuffs>::iterator &index_iter);
		post_process_stuffs *connector;
		post_process_stuffs ref,meas; //FOR AVERAGE VALUES
		std::vector<post_process_stuffs> references, measurements;
		std::vector<post_process_stuffs>::iterator iter,last_ref,first_ref, first_meas, last_meas;
//		std::vector<ONE_SET> all_sets; //contains sets of pairs and reverse pairs
		std::vector<FIRST_LAST_FREQS> boundaries;
		std::vector<ref_meas_block> pairs;		  //ref1-meas1-ref2
		std::vector<ref_meas_block> reverse_pairs; //having meas1-ref1-meas2
		bool files_exist(bool popup_window_if_not_existing=false);
		double get_ref_ion_mass(void);
		double get_ref_ion_mass_err(void);
		void print_mass_excess(void);
		void print_results_on_screen(void);
		bool process_file(WHICH_FILE ref_or_meas,std::string overridden_filename ="");
		bool set_data_line(const std::string &row, WHICH_FILE ref_or_meas);
		void clear_containers(WHICH_FILE);
		void set_on_screen(const post_process_stuffs &tama, WHICH_FILE ref_or_meas);
		double get_meas_A(void);
		double get_ref_A(void);
		//double get_mass_excess(const string &messy_element_name, double &me_error, string &found_element, bool &ok, string &source_of_value);
		int divide_to_sets(void); //Divides data to sets, return number of sets //2006-08-18
		int assing_pairs(void);		//Returns number of good pairs
		int assing_reverse_pairs(void);		//Returns number of good reverse pairs
		int assign_nearest_ref_to_meas(void);
		void print_pairs(std::ostream &in_here);
		void print_reverse_pairs(std::ostream &in_here);
		bool calculate_all_sets(void);
		bool calculate_via_pairs(void);
		void output_individual_measurements(const std::string &filename);
//		void output_gnuplot_help_stuff(const string &filename);
		void draw_connections(void);
		void show_ions_per_shot();
		void show_counts_in_class();
		void show_counts_in_all_classes();
		void show_z_slope();
		void show_z_const();
		void show_chisq();
		void redraw_what_is_active_now();
		void do_everything(void);
		void move_windows_to_initial_positions(void);
		int get_number_of_pairs(void);
		std::vector<std::string> get_filenames_from_entry(Gtk::Entry &entri);
                void store_settings(Glib::ustring filename, bool rcfile=false);
		int  restore_settings(Glib::ustring filename);
		void initialize_comboboxes();
		void set_input_path_and_ref_or_meas_files(Gtk::Entry* ref_or_meas_entry);
		//SIGNALS AFTER
		void on_ref_files_chooser_clicked();
		void on_meas_files_chooser_clicked();
		void on_recalculation_requested(void);
		void on_save_settings_activate();
		void on_load_settings_activate();
		void on_pp_save_to_ini_file_activate();
		void on_pp_save_to_default_ini_file_activate();
		void on_bring_helper_window_on_top_activate(void);
		void on_quit_coma_activate(void);
		void on_just_close_clicked(void);
		void on_otherItems_changed(void);
		void on_use_z_extrapolated_toggled(void);
		void on_load_files_button_clicked(void);
		void on_show_dependencies_pressed(void);
		void on_press_pressed();
		void on_axial_pressed();
		void on_a_rf_pressed();
		void on_t_rf_pressed();
		void on_phase_pressed();
		void on_cycl_pressed();
		void on_magn_pressed();
		void on_freq_pressed();
		void on_freq2_pressed();
		void on_fraction_pressed();
		void on_other_items_pressed();
		void on_radiobutton_qval_pressed();
		void on_radiobutton_mass_pressed();
		void on_radiobutton_freq_pressed();
		void on_export_button_clicked();
		void on_get_ame_button_clicked();
		void on_recalc_button_clicked();
		bool on_me_entry_key_press_event(GdkEventKey *ev);
		void on_do2_button_clicked(); //debugger
		void on_class_spinbutton_value_changed();
		void on_what_to_use_changed();
		void what_version_is_COMA(int versio) {revision=versio;}

		typedef sigc::signal<void> type_signal_close;
 		type_signal_close signal_close();
protected:
    type_signal_close m_signal_close;

    void do_stuff(DATASET & daatta,
		  void (*fp)(const post_process_data &, double &, double &),
		  double y_offset = 0.0,
		  const char * extra = NULL);

    void set_ybars(double meas, double dmeas, double ref, double dref) {
	shower->add_ybar(meas, 2);
	shower->add_ybar(meas - dmeas, 2);
	shower->add_ybar(meas + dmeas, 2);

	shower->add_ybar(ref, 1);
	shower->add_ybar(ref - dref, 1);
	shower->add_ybar(ref + dref, 1);
	shower->show_ybars(true);
	shower->set_xtics(2);
	shower->set_mxtics(5);
    }
};
#endif
