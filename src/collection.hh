//This handles different frequencies. Sort of collection of frequencies.
#ifndef _COLLECTION_HH
#define _COLLECTION_HH

#include "config.h"

#include <string>
#include <vector>
#include <gtkmm/progressbar.h>

#include "unit.hh"
#include "fitting1.hh"
#include "nrutil.hh"
#include "loctm.hh"
#include "window2.hh"
#include "util.hh"
#include "eva_loader.hh"
#include "plotstructures.hh"

#ifdef HAVE_ONLINE
#include "Mass_Scanning_Consumer.h"
#endif

#define MAXFREQUENCIES 400
#define MAXCLASSES 20
#define MAXFITVARIABLES 20

class cFrequencies{
 private:
  Gtk::ProgressBar *load_bar;
  Gtk::ProgressBar *fit_bar;
 #ifdef HAVE_ONLINE
  Gtk::ProgressBar *online_bar;
 #endif
  /// Data containers for derived data
  std::vector<Datapoint> tof_vs_frequency_data, tof_vs_frequency_fit;
  std::vector<Datapoint> counts_vs_frequency_data;
  std::vector<Datapoint> average_counts_vs_frequency_data;
  std::vector<Datapoint> tof_histogram_data;
  std::vector<Datapoint> tof_histogram_data_for_one_class;
  std::vector<Datapoint> event_histogram_data;
  std::vector<Datapoint> history_histogram_data;
  std::vector<Datapoint> history_histogram_with_gates_data;
  
  std::vector<CTS_HISTOGRAM_OF_FREQ> two_dim_contour_of_counts;
  /// Extra infos. For temporary use only.
  std::string extra_info_str_1, extra_info_str_2;
  double extra_info_dbl_1, extra_info_dbl_2;

	int bin_interval_in_2d_contour;
 	Gtk::MessageDialog *popupper;
 	FIT_FUNC_TYPE tof_function_to_be_fitted;
  // int data_file_format;  //0=NEW, 1=OLD, 2=feb09
  DATAFILEFORMAT data_file_format;
  int frequencynumber;
  double bin_width;			//Getting this from data file [us]
  int max_bin_number;		//Getting this from data file
  int number_of_bunches_in_one_spectrum; //Getting this from data file; if in single bunch mode, should be 1
  int MCS_recording_offset_in_bins; //In case MCS is triggered later than the bunch leaves the trap
  int maxfrequencies;
  std::vector<cUnit *> frequencies;
  bool data_in_order;
  char delimiter;               //datafile delimiter, default ' '
  int class_min_cts[MAXCLASSES];
  int class_max_cts[MAXCLASSES];
  int number_of_classes;
  std::string e_field_filename;
  std::string b_field_filename;
  bool are_field_files_ok;
  int startpoint,endpoint; //Points to use to integrate e-field
  std::string start_time; // Start time of the sweep "2012-10-12 12:23:45"
  std::string end_time; // End time of the sweep "2012-10-12 14:23:45"
  int number_of_rounds_in_file;
  boost::filesystem::path data_file;
  bool number_of_rounds_in_file_checked;
  bool is_crap_with_constant_tof_used_in_fitting;
// Results from fit
  double fitparam[MAXCLASSES][MAXFITVARIABLES];
  double fitparam_err[MAXCLASSES][MAXFITVARIABLES];
  bool was_param_fitted[MAXCLASSES][MAXFITVARIABLES];
  double fit_chisq[MAXCLASSES];
  bool is_class_fitted[MAXCLASSES];
//

  int max_fit_variables;
  //bool fit_two_peaks;
  short number_of_peaks_to_fit;
  int bin_start,bin_stop;	//bin window
  int cts_start,cts_stop;   //cts window
  bool automatic_frequency_finding;
  double mass; //A of ions
  double charge; //charge state of ions
// Copied from toffit
  NRUTIL nrutil;
  FITTING *fitting;//(92,0.4,0.030);  //HERE1 mass [u], time_rf[s],amplitude[V] ####################################################################
  FIT_FUNC *fit_func;//(92,0.4,0.030);//HERE2 too; time and amplitude are just ini values, fitted later ########################################
  double *x,*y,*chi_vec,*sig,*a,**covar,**alpha,chisq,alamda;
  int ndata, *ia,ma,mfit;
  int skip_start; //how many to skip from start
  int skip_end;  // ..
// Fitting linear. Using same names as
  double *x2,*y2,*chi_vec2,*sig2,*a2,**covar2,**alpha2,chisq2,alamda2;
  int ndata2,*ia2,ma2,mfit2;
  //Results from linear fit
  double slope, slope_err, constant, constant_err, slope_constant_covariance;
  double linear_extrapolation_x;
  bool is_countrate_effect_fitted;
	bool data_is_in_single_bunch_mode;
 public:
	 STATISTICS_MODE stat_mode;
	void set2d_contour_tof_interval(int binss) {
		bin_interval_in_2d_contour = binss;
	}
  cFrequencies(int imaxfrequencies, Gtk::MessageDialog *popper, char idelimiter = '\t'){
    number_of_peaks_to_fit = 1;
	  data_file_format = DATAFORMAT_NEW;
		setPopUpper(popper);
		tof_function_to_be_fitted = SINGLE_PEAK_FIT;
		data_is_in_single_bunch_mode = false;
		is_countrate_effect_fitted = false;

		MCS_recording_offset_in_bins = 0;

		resetData();
		fitting  = new(std::nothrow) FITTING(1,1,1);
		fit_func = new(std::nothrow) FIT_FUNC(1,1,1);
		frequencies = std::vector<cUnit *>(imaxfrequencies);
		frequencynumber=0;              //How many frequencies are in use
		maxfrequencies=imaxfrequencies;
		delimiter = idelimiter;
		data_in_order = false;
		number_of_classes = 0;
		max_bin_number = 0;
		preInitializeTofFitter();//MAXFREQUENCIES, MAXFITVARIABLES);
		preInitializeLinearFitter();//MAXCLASSES,2);
		setAllDataNotFitted();
		linear_extrapolation_x = 0;
		are_field_files_ok=false;
		number_of_rounds_in_file_checked=false;
		ma=0;
		fitting->get_approx_field("fields.txt");
		if (fit_func->get_approx_field("fields.txt") != 0) {
			popper->set_title("Foo Bar!");
			popupper->set_message("Something wrong with the field file\n\"fields.txt\"");
			popupper->run(); std::cout << "Dying badly because fields are bad.\n";
			abort();
			popper->hide();

		}
	//	fitting->print_fieldpoints();
	//	fit_func->print_fieldpoints();
		set2d_contour_tof_interval(5);
	} //End constructor
  cFrequencies(const cFrequencies &frekvens){      //Copy constructor OUT-OF-DATE
      frequencies = std::vector<cUnit *>(frekvens.maxfrequencies);
//    frequencies = new(std::nothrow) cUnit *[frekvens.frequencynumber];
//	Change in future to this; table increment is needed too
    maxfrequencies = frekvens.maxfrequencies;
    frequencynumber = frekvens.frequencynumber;
    delimiter = frekvens.delimiter;
    data_in_order = false;
	number_of_classes = frekvens.number_of_classes;
	for (int i=0;i<number_of_classes;i++){
		class_min_cts[i] = frekvens.class_min_cts[i];
		class_max_cts[i] = frekvens.class_max_cts[i];
	}
  }
  ~cFrequencies(void){ //Destructor
	delete fitting;
	delete fit_func;
    for (int i=0; i < frequencynumber; i++){
      delete frequencies[i];
    }
      frequencynumber = 0;
	remove_fit_things();
  }
  	//NOTE: Overriden filename; old = .dat , new = /bunches.dat
	void setLoadProgressBar(Gtk::ProgressBar *baari) { load_bar = baari;}
	void setFitProgressBar(Gtk::ProgressBar *baari) { fit_bar = baari;}
	void setPopUpper(Gtk::MessageDialog *popupikkuna){popupper = popupikkuna;}
	int getNumberOfRoundsInFile(const boost::filesystem::path bunchfilename, bool force=false, std::ostream &log=std::cout);
	int getNumberOfRoundsInFile_v3(const boost::filesystem::path bunchfilename, bool force=false, std::ostream &log=std::cout);
	int getNumberOfRoundsInFile_v4(const boost::filesystem::path bunchfilename, bool force=false, std::ostream &log=std::cout);
  	void resetData();
	std::vector<TWO_INTS> get_class_borders_from_data();

	std::string readDirectory(const boost::filesystem::path bunchfilename, int start_round = 0, int end_round = 0, ostream& log = std::cout);
	std::string readDirectory(const boost::filesystem::path bunchfilename,
                           const std::vector<int> &round_skip_list, std::ostream & log = std::cout);
	std::string readDirectory_v1(const std::string &prefix, const std::string &suffix, //For reading dir in old file format
                       const std::string &dirname, std::ostream &log=std::cout,
                       const int startfileno=1, const int endfileno=60000);
	std::string readDirectory_v2(const std::string &dirname,const std::string &filename, std::ostream &log=std::cout);
	std::string readDirectory_v3(const boost::filesystem::path bunchfilename, std::ostream &log=std::cout); //Reading the NEWEST KIND OF bunchFILE, may2005->
	std::string readDirectoryPartly_v3(const boost::filesystem::path bunchfilename,
									int start_round, int end_round, std::ostream &log);
	std::string readDirectory_v4(const boost::filesystem::path bunchfilename, std::ostream &log=std::cout); //Reading the NEWEST KIND OF bunchFILE, feb2009->
	std::string readDirectoryPartly_v4(const boost::filesystem::path bunchfilename,
									int start_round, int end_round, std::ostream &log);
	std::string outputToFile(const std::string &outfile, std::ostream &log, const char outdelimiter){
		return outputToFile(outfile,-100000,1000000,outdelimiter,log);
	}
	std::string outputToFile(const std::string &outfile, int startbin,
                      int stopbin, const char outdelimiter, std::ostream &log);
	std::string outputCountsToFile(const std::string &outfile, int startbin, int stopbin,
  					  const char outdelimiter, std::ostream &log); //Outputs counts of each frequency
	std::string outputTotalCountsToFile(const std::string &name_of_data, const std::string &outfile, int startbin, int stopbin, std::ostream &log);
	int getTotalCounts(int startbin, int stopbin); //Return total no of counts of whole data
	std::string addFreqFromFile(const std::string &datafilename,int i, std::ostream &log); //Creates dataunit that corresponds to one f
 	std::string addFrequencyUnit(const cUnit &toAdd);
	int increaseFrequencyTableSize(int size_increment=50);
 	int searchForFrequencyMatch(double verrokki); //Returns frequencynumber if match, otherwise -1
	int getFrequencyNumber(void);
	double getFreqHavingMinimumTof(int class_number);
	double getFreqHavingMaximumTof(int class_number);
	int order_data(void); //For putting frequencies in ascending order
	std::string outputCountsFromEachFile(const std::string &outfile, int startbin, int stopbin, std::ostream &log);
	std::string printAllOut(std::ostream &to_here,int what_frequency);
	std::string outputEventNumberToFile(const std::string &outfile, int startbin,
                      int stopbin, int max_events, const char outdelimiter, std::ostream &log);
	int getTotalCounts(int startbin, int stopbin, int min_cts, int max_cts, std::ostream &log =std::cout);
	std::string outputClassToFile(const std::string &outfile, int startbin,
                      int stopbin, int min_cts, int max_cts, const char outdelimiter, std::ostream &log);
	std::string printHistogram(int startbin, int stopbin,int max_events, std::ostream &log);
	std::string divideAutomaticallyToClasses(int startbin, int stopbin, int min_cts, int max_cts, int how_many_classes,
						std::ostream &log);
	std::string divideManuallyToClasses(std::vector<TWO_INTS> boundaries);
	void initializeFit_fast_tof(double masse, double chargge ) {
		mass = masse;
		charge = chargge;
		fitting->use_fast_tof(true);
		fit_func->use_fast_tof(true);
		fitting->setMass(mass,charge);
		fit_func->setMass(mass,charge);
	}
	int initializeFitSimion(double masse, std::string e_field_file, std::string b_field_file, int start_pa=276, int stop_pa=1599 ){
		mass = masse; e_field_filename = e_field_file; b_field_filename = b_field_file;
		startpoint = start_pa; endpoint = stop_pa;
		fitting->setMass(mass,charge);
		fit_func->setMass(mass,charge);
		if ( fitting->get_fields(e_field_filename,b_field_filename,startpoint,endpoint) )
			return 1;
		if ( fit_func->get_fields(e_field_filename,b_field_filename,startpoint,endpoint) )
			return 1;
		are_field_files_ok=true;
		return 0;
	}
	bool do_not_load_first_bunch_after_freq_change;
	bool initializeSingleFit(double f, bool ff, double p, bool fp, double ax, bool fax,
							double magn, bool fmagn,
							double cycl, bool fcycl, double fi, bool ffi,
							double t_rf, bool ft_rf, double a_rf, bool fa_rf);
	bool initializeSecondPeakForFitting(double f2, bool ff2, double fractio, bool ffractio);
	bool initializeThirdPeakForFitting(double f3, bool ff3, double weight3, bool fweight3);
 // Ramsey initialisation
	bool initializeSingleRamseyFit(double f, bool ff, double ax, bool fax,
							double magn, bool fmagn, double t_fringe, bool ft_fringe,
							double t_wait, bool ft_wait, double a_rf, bool fa_rf);
	bool initializeRamseyDamping(double p, bool fp);
	bool is_2nd_peak_in_the_game(void){if (number_of_peaks_to_fit > 1) return true; else return false;}
	bool is_3rd_peak_in_the_game(void){if (number_of_peaks_to_fit > 2) return true; else return false;}
	void preInitializeTofFitter(int max_freqs=MAXFREQUENCIES, int max_fit_variables=MAXFITVARIABLES);
	void preInitializeLinearFitter(int max_freqs=MAXFREQUENCIES, int max_fit_variables=2);
	int Fit(int class_number,std::ostream &log,bool use_auto_freq=true, int skip_from_start=0, int skip_from_end=0);

	std::string fitAllClasses(std::ostream &log, int skip_start, int skip_end);
// Outputting to files
	std::string outputFitsAndDats(const std::string &outdir, const std::string &element_name, const int scan_number,
		 const std::string &fit_file_suffix =".fit",const std::string &dat_file_suffix =".dat");
	std::string outputGnuplotFile(const std::string &outdir, std::string gp_file_name,
		 std::string element_name, int scan_number, bool one_outfile_only =true, std::string terminal_suffix =".ps",
		 std::string fit_file_suffix =".fit", std::string dat_file_name=".dat",
		 bool append =true, std::string header ="");
	std::string outputAllFittedThings(const std::string &outdir,const std::string &file_name,
		 const std::string element_name, int scan_number, bool overwrite, std::string skipped_rounds, double signal_to_noise, int lakritsiversion);

// ---end Outputting to files
	std::string getMCSdata(void);
	double getBin_width(void){		return bin_width;}
	int getMaxBins(void){ 			return max_bin_number;}
	int getNumberOfBunchesInOneSpectrum(void){ return number_of_bunches_in_one_spectrum;}
	int getTotalBunchNumber(void);
	int getTotalCounts(void);
	int getNumberOfClasses(void){	return number_of_classes;}
	std::string getClassDivResult(void);
	std::string wasClassDivOk(void);
	bool getFitResults(const int class_no, double params[], int &number_of_fit_vars); //Returns 1 if ok (class is fitted)
	bool getFitErrors(const int class_no, double params[], bool was_it_fitted[], int &number_of_fit_vars, double &khisq);//Returns 1 if ok (class is fitted)
	bool getDataUsedInFit(const int class_number, double x_coord[], double y_coord[], double y_coord_error[], int &number_of_freqs);
//	bool getFitAsPoints(const int class_number, double x_coord[], double y_coord[], int number_of_points_wanted,double how_much_outrange=0);
//	bool getFitAsPoints(const int class_number, std::vector<Datapoint> & datas, int number_of_points_wanted=151,double how_much_outrange=0);
	double getMinimumFrequency(void);
	double getMaximumFrequency(void);
	bool isClassFitted(int class_number);
	std::string getHistogram(int counts_in_bin[], int max_bins);
	std::string getHistogram(std::vector<int> &in_here);
	double getAverageCountsInClass(const int class_no, std::ostream &log=std::cout);
	double getWeightedAverageCountsInClasses(std::ostream &log=std::cout);
	double getAverageCountsInClasses(bool only_for_fitted_classes=false,std::ostream &log=std::cout);
	int getNumberOfFittedClasses(void);
	int getCountsInClass(const int class_no, std::ostream &log=std::cout);
	void getFittedValueAndChisqAdjustedError(int class_no, double &value, double &value_error,int fitvarnumber=0);
	void InitializeLinearFit(double slope, bool fslope, bool autoslope,
																			 double constant, bool fconstant, bool autoconstant,int y_variable=0, std::ostream &log=std::cout);
	bool FitLinear(double &result_slope, double &result_constant, int y_variable=0, std::ostream &log=std::cout);
	void setInterpolatedCountrateEffect_x(double x){linear_extrapolation_x = x;}
	double getInterpolatedCountrateEffectValue(std::ostream &log =std::cout);
	double getInterpolatedCountrateEffectValueError(std::ostream &log =std::cout);
	void remove_fit_things(void);
	void setAllDataNotFitted(void);
	bool isCountrateEffectFitted(void){return is_countrate_effect_fitted;}
	double getInterpolatedCountrateEffectValue(const double interpolate_to ,std::ostream &log=std::cout);
	double getInterpolatedCountrateEffectValueError(const double interpolate_to ,std::ostream &log=std::cout);
	void getLinearFitErrors(double &sloup_err, double &konstant_err, double &covariance, std::ostream &log =std::cout);
	std::string getAverageTime(void);
	void printContentOfFrequency(int freq_no, std::ostream &log=std::cout);
	void printFitFields(std::ostream &log=std::cout){fitting->print_fieldpoints();}
	//int getEventsAsTable(int events[], int max_events); //call max_events with getMaxEvents(...) not to get too much
	int getEventsAsTable(std::vector<int> &in_here); //9.8.2006
	int getMaxEvents(void); // 9.8.2006: Returns number of ions in biggest bunch
//	bool getCountsOfFrequencies(double counts[], double freq[], int &max_frequencies,int startbin,int stopbin,int min_cts, int max_cts);
//	bool getCountsOfClass(double counts[], double freq[], int &max_frequencies,int class_no);
	int getLowerEventBoundary(int class_no);
	int getUpperEventBoundary(int class_no);
	int setLowerEventBoundary(int class_no,int value);
	int setUpperEventBoundary(int class_no,int value);
	int setClassNotFitted(int class_no){is_class_fitted[class_no]=false; return 1;}
	void deleteData(void);
	void setDataFileFormat(DATAFILEFORMAT format=DATAFORMAT_NEW) {data_file_format=format;}
	DATAFILEFORMAT getDataFileFormat(void) {return data_file_format;}
	void setBinAndCountWindowManually(int BinMin,int BinMax,int CtsMin,int CtsMax);
	bool getLinearFit(DATASET &fitpoints, double range_min, double range_max, int how_many_points=100);
//	bool getCountsOfClass(DATASET &dataset, int class_no);
//	bool getCountsOfFrequencies(DATASET &dataset, int startbin, int stopbin, int min_cts, int max_cts);
	bool setAverageTime(std::string time_string);
	void FitGaussian(	double range_min,double range_max, int class_used,
								double ini_f, double ini_fwhm, double ini_h, double ini_bgr,
								bool   fit_f, bool fit_fwhm, bool fit_h, bool fit_bgr,
								double &res_f, double &res_fwhm, double &res_h, double &res_bgr,
								double &dres_f, double &dres_fwhm, double &dres_h, double &dres_bgr,
								double &chisq, DATASET &outdata,int outpoints_wanted=100);
	FIT_FUNC_TYPE get_fit_func_type(void);
	std::vector<int> getClassUpperBoundary(void);
	int set_frequency_range(double min=-1.0, double max=-1.0);
	bool is_data_collected_in_single_bunch_mode_v3(const boost::filesystem::path & bunchfile, std::ostream &log);
	bool is_data_collected_in_single_bunch_mode_v4(const boost::filesystem::path & bunchfile, std::ostream &log);
	bool is_data_collected_in_single_bunch_mode(void);
	std::vector<CTS_HISTOGRAM_OF_FREQ> * get_2d_contour_of_counts(
								int class_no, double freq_min, double freq_max, int tof_min_in_bins, int tof_max_in_bins, int tof_interval=40);

	//2007-04-23
	void print_2d_counts(std::ostream &here, int class_no);
	//2007-08-05 Towards sum_statistics
	void get_cts_vs_bin_histogram_of_class(std::vector<int> & countsit,int class_numbero, bool shift_with_mean_tof = false);
	double get_average_tof_of_all_ions_in_class(int class_numbero,bool shift_with_mean_tof = false);
	double get_sd_of_all_ions_in_class(int class_numbero, bool shift_with_mean_tof);
	double get_sd_of_all_ions_in_class(int class_numbero, STATISTICS_MODE statist_mode);
	double get_time_of_flight_error(int freq_index, int class_no, double total_sdom = -1);

	//2008-01-02 This method will set bunches that are in vector<int> to be not used; return number of bunches that were set 'not' active
	int set_bunches_that_will_be_skipped(const std::vector<int> &skipping_these_bunches);
	void set_all_bunches_active();
	//2008-01-03 To form a 'offline histogram'
	void form_a_history_of_counts(int startbin, int stopbin, int min_ctss, int max_ctss, std::vector<int> &in_here, int sum_up_this_many_bunches,  std::vector<int> & round_changing_points);
	
	bool does_all_freqs_have_same_amount_of_bunches();
	void initializeConstantCrapForSinglePeakFit(double tof_of_crap, bool fit_tof_of_crap, double fraction_of_crap, bool fit_fraction_of_crap);
	void add_bunch(const cUnit & toadd);
	bool set_P_row(const std::string fields[], int how_many);
	bool set_I_row(const std::string fields[], int how_many);
	void split_feb09_datarow_to_fields(const std::string rowwi, std::string fields[], int &n);
	bool is_this_v3_bunchfile(const boost::filesystem::path & bunchfilename);
	bool is_this_v4_bunchfile(const boost::filesystem::path & bunchfilename);
	bool is_some_parameter_to_fit_initially_zero();
	int show_eva_data(int number, std::list< cEvaFile* >& evafiles);
	int get_number_of_rounds_from_eva_data(int number, std::list< cEvaFile* >& evafiles);
	double get_MCS_recording_offset_in_bins(void){return MCS_recording_offset_in_bins;}
	bool getDataUsedInFit(const int class_number, std::vector<Datapoint> & datas);
	
	std::vector<Datapoint> * get_history_of_counts(int startbin, int stopbin, int min_ctss, int max_ctss,
							 int sum_up_this_many_bunches, vector< int >& round_changing_points);

	std::vector<Datapoint> * get_full_history_of_counts(int sum_up_this_many_bunches, std::vector<int> & round_changing_points);
	std::vector<Datapoint> * get_histogram();
	std::vector<Datapoint> * get_histogram_of_class(int class_numbero, bool shift_with_mean_tof = false);
	
	std::vector<Datapoint> * get_event_histogram();
	
	std::vector<Datapoint> * get_counts_vs_frequency(int class_number);
  std::vector<Datapoint> * get_average_counts_vs_frequency(int class_number);
	std::vector<Datapoint> * get_tof_vs_frequency(int class_number);

	std::vector<Datapoint> * getFitAsPoints(const int class_number, int number_of_points_wanted=151,double how_much_outrange=0);
	

  std::string get_fit_results_as_text(int class_number, string line_end="\n");
	
	void shift_scanrange(double amount=0.0);
	
	#ifdef HAVE_ONLINE
	int insert_online_bunchpack(const std::list<Bunch_Data> &data); //Returns bunch #
	void set_MCS_data(const int max_binss, const int bin_widthh_as_set_on_MCS);
	void set_online_progressbar(Gtk::ProgressBar *baari) { online_bar = baari;}
//	void set_MCS_data(const int max_binss, const double bin_widthh);
	#endif
};
#endif
