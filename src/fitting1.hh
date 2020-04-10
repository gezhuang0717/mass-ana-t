#ifndef _FITTING1_HH
#define _FITTING1_HH

#include <iostream>
#include <vector>

#include "nrutil.hh"

enum FIT_FUNC_TYPE{
	SINGLE_PEAK_FIT = 0,
	DOUBLE_PEAK_FIT = 1,
	LINEAR_FIT	= 2,
	GAUSSIAN	= 3,
	SINGLE_RAMSEY	= 4,
	SINGLE_PEAK_WITH_CRAP = 5, // Konig type
	SINGLE_RAMSEY_WITH_CRAP = 6,
	SINGLE_RAMSEY_WITH_DAMPING = 7,
	TRIPLE_PEAK_FIT = 11,
	QUAD_PEAK_FIT = 12,
};

struct CONSTS{
	char character;
	double value;
};
struct FIELD_POINTS{
	int start;
	int stop;
	double e_field0;
	double e_field1;
	double e_field2;
	double b_field0;
	double b_field1;
	double b_field2;
};
class FIT_FUNC{  //some of its methods are not called
    double mass,charge;
	std::vector<CONSTS> consts;
	std::vector<FIELD_POINTS> field_points;
    double e_field[1600],b_field[1600];
	bool fast_tof;			//true, if using faster tof calculation (fields.txt)
	bool has_simion_files_been_read;
  public:
    void funcs(double, double [], double *, double [], int, FIT_FUNC_TYPE);
  //--------------------------------------------------------------------
    void check_correctness(double fit_params[],int how_many);
  // This corrects w+ and w- radius to positive ones and gives out
  // appropriate phases.
  //
  double funk( double[], double [], double [], double [], int);
  double magnetic_moment(double, double, double,double,double,double,double,double, double);
//	double ramsey_2f_conversion(double omega_rf, double amplitude, double time_fringe, double omega_c,
//					   double mass,double rad_minus0_mm,double time_wait); // Returns radial energy

	double ramsey_2f_conversion(double nu_rf, double conversions, double time_fringe, double time_wait, double nu_c,
					   double mass, double rad_minus0_mm,double rad_plus0_mm, double phase);
	double ramsey_2f_conversion_v2(double nu_rf,   double a_rf, double time_fringe,  double time_wait, double nu_c,
					   double mass, double pressure, double rad_minus0_mm, double rad_plus0_mm, double phase);
	double ramsey_2f_radius_conversion(double nu_rf, double a_rf, double time_fringe, double time_wait, double nu_c,
					   double mass, double rad_minus0_mm,double rad_plus0_mm, double phase);
	double ramsey_2f_asymmetric_conversion(
		double nu_rf, double conversions, double time_fringe1,
		double time_tot, double nu_c,
		double mass, double rad_minus0_mm,double rad_plus0_mm, double phase, double time_fringe2);

//   double stefans_magnetic_moment (double, double, double,double,double,double,double,double, double);
  double get_tof(double,double,double,double);
	double ramsey_tof(double nu_rf, double nu_c, double energy, double rad_min,
							double time_fringe, double time_wait, double excit_amplitude);
	double ramsey_tof_with_damping(double nu_rf, double nu_c, double energy, double rad_min,
							double time_fringe, double time_wait, double excit_amplitude, double pressure);

	double ramsey_tof_with_constant_crap(double nu_rf, double nu_c, double energy, double rad_min,
					double time_fringe, double time_wait, double excit_amplitude,
					double tof_of_crap, double ratio);
	double get_tof_faster(double mu, double mass,double charge, double ax_energy);
 	double tof_for_two(double nu_rf,double nu_c1, double pressure,double energy,
				double rad_min,double rad_plus, double phase, double excit_time, double excit_amplitude,
				double nu_c2, double ratio);
 	double tof_for_three(double nu_rf,double nu_c1, double pressure,double energy,
				double rad_min,double rad_plus, double phase, double excit_time, double excit_amplitude,
				double nu_c2, double weight2, double nu_c3, double weight3);
	double tof_with_constant_crap(double nu_rf,double nu_c1, double pressure,
		double energy,	double rad_min,double rad_plus, double phase,
		double excit_time, double excit_amplitude, double tof_of_crap, double ratio);
  bool get_fields(std::string e_field_file, std::string b_field_file, int startpoint, int endpoint);
  int get_approx_field(std::string field_file = "fields.txt");
  double tof(double, double, double, double,double,double, double, double,double);
  void set_ini_values1(double imass, double itime_rf, double iamplitude){
    mass = imass;
  }
	void use_fast_tof(bool use_fast=true) {fast_tof = use_fast;}
	void print_fieldpoints(std::ostream &log=std::cout); //for debugging purposes
	bool put_as_number(const std::string &text, double &number);
	void setMass(double masse, double chargge){
		mass = masse;
		charge = chargge;
	}
    FIT_FUNC(){           //Constructor, inadequate
      mass=114;
		fast_tof= true;
		has_simion_files_been_read = false;
//		number_of_consts=0;
//		number_of_field_points=0;
    }
    FIT_FUNC(double imass, double itime_rf, double iamplitude){//more adequate
      set_ini_values1(imass,itime_rf,iamplitude);
		fast_tof=true;
		has_simion_files_been_read = false;
//		number_of_consts=0;
//		number_of_field_points=0;
	}
    ~FIT_FUNC(){}
};

class FITTING : NRUTIL, public FIT_FUNC {
          void mrqcof(double [], double[], double[], int, double [],
          int[], int, double **, double [],double*, FIT_FUNC_TYPE);
    void covsrt(double **, int , int [] , int );
    void gaussj(double **, int , double **, int );
    double amotry(double **, double [], double [], int , int , double ,
    double [], double [], double [], int , int );
  public:
    FITTING(double imass, double itime_rf, double iamplitude){ //Constructor
      set_ini_values1(imass,itime_rf,iamplitude);
    }
    void amoeba(double **, double [], int , double  , int *,
    		 double [], double [], double [], int );
    void mrqmin(double [], double [], double [], int , double [], int [],
	 int , double **, double **, double *, double *, FIT_FUNC_TYPE);

};

std::string get_fit_function_type_as_text(FIT_FUNC_TYPE & type);
bool is_fit_func_type_konig_type(FIT_FUNC_TYPE & tama);
bool is_fit_func_type_ramsey_type(FIT_FUNC_TYPE & tama);
std::string get_fit_parameter_name(FIT_FUNC_TYPE & func_type, int param_number);

#endif
