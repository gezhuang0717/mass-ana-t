// generated 2005/9/13 18:07:12 EEST by tomero@a62-248-137-142.elisa-laajakaista.fi.(none)
// using glademm V2.0.0
//
// newer (non customized) versions of this file go to window1.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _WINDOW1_HH
#define _WINDOW1_HH

#include "config.h"

#include <gtkmm/entry.h>
#include <gtkmm/spinbutton.h>

#include "collection.hh"
// #include "postProcessor.hh"
#include "logoes.h"
#include "mass_scan_settings.hh"
#include "tickerer.hh"
#include "util.hh"
#include "window2.hh"
#include "ok_cancel_dialog_2.hh"
#include "mjonotpp.hh"
#include "loctm.hh"
#include "inputting.hh"
#include "ame_stuff.hh"
#include "eva_loader.hh"
#include "plotstructures.hh"
#include "fit_parameters.hh"

#ifdef HAVE_ONLINE
#include "Event_Data.h"
#include "online_histogrammer.hh"
#endif

#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <list>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/progressbar.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/stock.h>
#include <gtkmm/dialog.h>
#include <gtkmm/main.h>
#include <gtkmm/filechooser.h>
#include <gtkmm/colorselection.h>
#include <gtkmm/window.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/textview.h>
//
//boost filesystem headers
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#define KONIG1 "Konig - single peak"
#define KONIG2 "Konig - single peak + bgr"
#define KONIG3 "Konig - two peaks"
#define KONIG4 "Konig - 3 peaks"
#define KONIG5 "Konig - 4 peaks"
#define RAMSEY1 "Ideal Ramsey - single peak"
#define RAMSEY2 "Ideal Ramsey - single peak + bgr"
#define RAMSEY3 "Ramsey with damping - single peak"

class Window2;
#include "alarm_cb.hh"
#include "window1_headers.inc"

enum PRIMARY{
	Q_VALUE = 0,
	MASS_EXC = 1,
	FREQ_RATIO = 2
};

#ifdef HAVE_ONLINE
class Window1 : public Gtk::Window, public Online_Listener
#else
class Window1 : public Gtk::Window
#endif
{
#include "window1_members.inc"
	private:
		
		//!
		//!  Container pointers for data to shown on plot windows.
		//!
		std::vector<Datapoint> * tof_vs_scan_parameter_data, * tof_vs_scan_parameter_fit;
		std::vector<Datapoint> * counts_vs_scan_parameter_data;
		std::vector<Datapoint> * tof_histogram_data;
		std::vector<Datapoint> * tof_histogram_of_class_data;
		std::vector<Datapoint> * event_histogram_data;
		std::vector<Datapoint> * history_histogram_data;
		std::vector<Datapoint> * history_histogram_with_gates_data;
		std::vector<Datapoint> zclass_plot_data, zclass_extrapolation_plot_data; // Contained in here
		
		//!
		//!  Container for plotdata
		//! 
		PlotDataContainer tof_vs_scan_parameter_plot_container, tof_vs_scan_parameter_fit_plot_container;
		PlotDataContainer counts_vs_scan_parameter_plot_container;
		PlotDataContainer tof_histogram_plot_container, tof_histogram_of_class_container;
		PlotDataContainer event_histogram_plot_container;
		PlotDataContainer history_histogram_plot_container, history_histogram_with_gates_plot_container;
		PlotDataContainer zclass_plot_container, zclass_extrapolation_plot_container;
		std::vector<int> round_change_points; // for history plot round change points
		
		cEvaFileCollection evafiledirectory;

		std::list<cEvaFile *> ref_eva_files,meas_eva_files;

//		mass_table ame_mass_table;
//		mass_table * ame_mass_table_p;
		Gtk::ComboBoxText fit_function_combo_box;
		Gtk::ComboBoxText * fit_function_combo_box_p;
		void initialize_fit_function_combobox();
		int revision;
		bool very_first_loading;
		bool doing_fitting, is_in_constructor, suspend_online_updating;
		#ifdef HAVE_ONLINE
		Online_History online_histogrammer,class_histogrammer;
		Plottiarea *ions_per_x_bunches_window;
// 		Plottiarea *bunches_per_class_window;
		std::list<Bunch_Data> online_bunches, waiting_bunches;
		Glib::ustring window_title_without_meas_id, meas_id;
		#endif
		#ifndef HAVE_ONLINE
		Plottiarea *offline_histogram; bool offline_histogram_need_update;
		#endif
		Gtk::MessageDialog ok_apply_cancel_dialog;
		Glib::RefPtr<ok_cancel_dialog_2> save_windows_content_dialog;
		Gtk::FileChooserDialog *outputdir_dialog;
// 		post_process_stuffs stuffi;
//		std::vector<int> counts_in_bin,events,count_events;
		std::vector<int> files_to_skip,rounds_to_skip;
		int bin_interval_in_2d_contour;
		Gdk::RGBA inactive_files,error,ookke,default_widget_base_color;
		cMassScanSettings mass_scan_settings;
		std::vector<TWO_INTS> class_borders; //number where class ends. ie. {1,6,9}, classes are [ctslow,1], [2,6], [7,9], [10,ctsupp]
		PRIMARY primary_quantity;
		std::string current_version;
		bool locked;
		cFrequencies *data;
		ViisiPlottiaWindow * five_plotareas_window;
		YksiPlottiWindow * yksi_plotarea_window;
		Plottiarea a,b,c,d,e,f;
		#ifdef HAVE_ONLINE
// 		Plottiarea g;
// 		YksiPlottiWindow * toinen_yksi_plotarea_window; // This is the buncher per class window
		#endif
		Plottiarea *tof_vs_class; bool tof_vs_class_need_update;
		Plottiarea *cts_vs_class; bool cts_vs_class_need_update;
		Plottiarea *data_histogram; bool data_histogram_need_update;
		Plottiarea *fitted_class_results; bool fitted_class_results_need_update;
		Plottiarea *event_histogram; bool event_histogram_need_update;
		// Post_Processor *coma;
//		plotwindow *ploddi; //PLOTTMMM
		bool using_debugging;
		bool old_data_file_format_in_use;
		bool MS_windows;
		Glib::ustring outputdir;
		bool stop_now;
		Gdk::Color background_color_for_windows;  bool background_alpha;
		//post_process_stuffs test_stuff;
//before: Own definitions
		void on_use_bin_gate_as_tof_plot_range_toggled();
		void on_sn_gating_changed();
		void on_sn_apply_button_clicked();
		void set_background_colors();
		bool on_window1_window_state_event(GdkEventWindowState* event);
		bool on_window1_focus_in_event(GdkEventFocus* event);
		void on_choose_2d_plotcolor_activate();
		void on_choose_background_color_activate();
		void on_show_partly_toggled();
		void on_check_for_new_version_at_startup_activate();
		void on_save_data_of_each_window1_activate();
		void on_hide_small_windows_activate();
		void on_show_settings_button_clicked();
		void on_fit_or_not_changed();
		void on_class_borders_fixed_button_toggled();
		void on_gaussfit_range_changed();
		void on_fit_gaussian_button_clicked();
		void on_meas_time_change_entry_clicked();
		void on_post_process_button_clicked();
		void on_help_activated();
		void on_2d_use_button_toggled();
		void on_quit_activate();
		void on_save_to_ini_file_activate();
		void on_save_to_default_ini_file_activate();
		void on_windows_on_top_activate();
		void on_about_activate();//todo
		void on_load_settings_activate();
		void on_save_settings_activate();
		void on_choose_outdir_button_clicked();
		void on_outputdir_entry_changed();
		void on_get_total_ions_button_clicked();
		void on_general_boundaries_changed();
		void on_class_boundaries_changed();
		void on_edit_class_spinbutton_value_changed();
		void on_save_class_definition_button_clicked();
		void on_how_many_classes_spinbutton_value_changed();
		void autoDivideToClasses();
		void ShowDataHistogram();  //DRAW histogram
		void startSingleClassFitting();
		void copy_results_to_initial_values(void);
		void on_showclass_spinbutton_value_changed();
//		void on_filenumber_spinbutton_changed();
		void on_dataformat_changed();
		void on_primary_changed();
		void set_2nd_peak_fit(bool active=true);
		void set_3rd_peak_fit(bool active=true);
		void on_auto_divide_to_classes_button_clicked();
		void on_debugitus_button_clicked();
		void on_trap1_helper_toggled();
		void on_choose_bin_interval_in_2d_plot_activate();
		void on_load_window_positions_activate();
		void on_save_window_positions_activate();
		void on_statistics_toggled();
		void on_cts_window_changed();
		//void on_find_min_tof_button_clicked();
		//void on_find_max_tof_button_clicked();
		//void on_fit_crap_chooser_toggled();
		void on_windows_same_as_ref_cb_toggled();
		void on_fit_function_combobox_changed();
		void on_histogram_compression_activate();
		void on_shift_scanrange_button_clicked();
		void on_debug1_button_clicked();
		void on_debug2_button_clicked();
		void on_debug3_button_clicked();
    void on_send_to_elog_button_clicked();
    void on_elog_executable_chooser_button_clicked();
    void on_gnuplot_executable_chooser_button_clicked();
    void on_elog_workdir_chooser_button_clicked();
//After this: OWN MEMBER FUNCTIONS
		bool create_summary_plot_gnuplot_script(boost::filesystem::path &scriptfile, boost::filesystem::path &png_filename);
		bool are_we_fitting_konig(void);
		bool are_we_fitting_ramsey(void);
		void set_tof_plot_range_from_bin_gate_if_allowed();
		void warn_about_frequencies_having_different_amount_of_bunches();
		void validate_sn_spinbuttons(void);
		double get_signal_to_noise_ratio_for_class();
		bool load_rounds_to_skip(const std::string &skipfilename);
		void show_offline_histogram(void);
		STATISTICS_MODE get_statistics_mode();
		void freq_window_changed();
		void show_single_class_histogram();
		void set_2d_contour_tof_interval(int binss);
		void get_and_show_2d_contour_of_cts(void);
    		std::vector<int> get_files_to_skip();
    		std::vector<int> get_rounds_to_skip();
		std::vector<int> get_skips(Gtk::Entry &entri);
		bool skip_this_file(int this_one);
		void testia();
		void initialize_dialogs();
		void hide_windows();
		void reshow_windows();
		void initialize_child_windows(void);
		void set_was_class_div_ok_text(void);
		void make_event_histogram_bars(void);
		void set_countrate_effect_not_fitted();
		void get_class_borders_from_data();
		bool check_new_version(void);
		int redraw_windows_if_necessary(bool); //Redraws small windows, if needed. Return number of windows drawn
		void validateCountFitRanges(void);
		Glib::ustring get_2d_filename(Glib::ustring before, Glib::ustring after, int nummber);
		void validateWindowingSpinbuttons();
    		bool store_settings(Glib::ustring directory, Glib::ustring filename, bool append=false, bool rcfile=false);
		Glib::ustring getOutputDir(void);
		void validateClassEditionButtons(void);
		#ifdef HAVE_ONLINE
		int loadIniFile(const Glib::ustring & filename="lakritsi_online.ini");
		void loadWindowPositions(const Glib::ustring & filename="lakritsi_online_windows.ini");
		void saveWindowPositions(const Glib::ustring & filename="lakritsi_online_windows.ini");
		#endif
		#ifndef HAVE_ONLINE
		int loadIniFile(const Glib::ustring & filename="lakritsi.ini");
		void loadWindowPositions(const Glib::ustring & filename="lakritsi_windows.ini");
		void saveWindowPositions(const Glib::ustring & filename="lakritsi_windows.ini");
		#endif
		void setInitialFitValues(std::ostream &log);

		std::string showFitResults(void); //fitted params in boxes and DRAW class x
		void showFittedClassResults(void); //DRAW freq vs ions/bunch
		void showLinearExtrapolation(void);
		void ShowDebuggingStuff(bool=false);
		void move_windows_to_initial_positions(void);
// 		void get_values_for_post_process(post_process_stuffs &stuffs);
		Glib::ustring outputfile_warning(const Glib::ustring &outdir, const Glib::ustring &dataoutfilename,
									const Glib::ustring &element, int scan_number);
    bool create_summary_blaa_blaa_text(boost::filesystem::path& text_file);
    bool execute_gnuplot_script(boost::filesystem::path& scriptfile);

 public:
		Gtk::MessageDialog message_dialog;
		DATAFILEFORMAT get_data_file_format(void) {return data->getDataFileFormat();}
		void bin_or_cts_win_value_changed();
		void on_gating_changed();
		void set_startfreq_menuitems_for_tof_vs_class_window();
		void update_sn_menues_in_tof_vs_class_window();
		void update_cts_menues_in_cts_vs_tof_window();
		void update_events_menues_in_cts_vs_bunches_window();

		void set_sn_bars_to_tof_window();
		void save_rounds_to_skip();
		void split_many_files();
		void practically_update_all(void){freq_window_changed(); bin_or_cts_win_value_changed();}
//		Dialog1 *popupper;
		void when_post_processing_endeth(void);
		void set_active_file_colors();
		void set_number_of_rounds_in_file(int rounds);
		void showDataInfo();
		void SplitAndFitFile(void);
		bool make_class_division(bool force_automatic=false);
		void linear_fit_button_clicked();
		void LoadDataFile(bool append=false);
		int getNumberOfRoundsInFile(bool force=false);
		void DeleteData();
		void validateSpinButtons(void);
		int batchProcess(void);
		virtual void set_online_measurement_id(const std::string & id);
		#ifdef HAVE_ONLINE
 		//void update_online_scan_progress();
		virtual std::string get_desired_online_id();
 		void set_window_labels_with_meas_id();
		virtual bool online_bunchpack(const std::list<Bunch_Data> &data);
		virtual bool online_info(const Measurement_Info &info);
		virtual void online_stopped(void);
		virtual void online_ended(void);
		//void update_online_start_info(std::string inhvo);
		//void update_online_end_info(std::string inhvo);
 		#endif
		void redraw_all_small_windows();
    		~Window1();
	void set_colors();
    void export_data_clicked();
    void StartFitting();
    bool is_class_fitted();
    int get_bin_min();
    int get_bin_max();
    int get_cts_min();
    int get_cts_max();
    double get_freq_min();
    double get_freq_max();
    void process_eva_file_directory();
	
protected:
	int histogram_compression_factor;
	void on_tof_vs_class_range_changed(const double &,const double &,const double &,const double &,const Glib::ustring &);
	void on_tof_vs_class_singleclick(const double &,const double &,const Glib::ustring &);
	void on_tof_histogram_range_changed(const double & x1,const double & y1,const double & x2,const double & y2,const Glib::ustring & range_name);
	void on_count_histogram_range_changed(const double & x1,const double & y1,const double & x2,const double & y2,const Glib::ustring & range_name);
	///
	/// When the range was selected with a mouse.
	///
	void on_gaussian_fit_range_changed(const double& x1, const double& y1, const double& x2, const double& y2, const Glib::ustring&);
};




#endif
