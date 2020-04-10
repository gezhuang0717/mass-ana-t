static double swappi;
#define SWAPP(a,b) {swappi=(a);(a)=(b);(b)=swappi;}

#define EPSILON 1e-30

#include "fitting1.hh"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <complex>

#include "mjonotpp.hh"
#include "util.hh"

typedef std::complex<double> dcmplx;

using namespace std;

double arsinh(double arg){
	return log(arg + sqrt(arg*arg+1));
}
//------------------------------------------------------------------------------
void FIT_FUNC::funcs(double x, double a[], double *y, double dydz[], int na, FIT_FUNC_TYPE type){
	double yval(0),diff,yval1;
	*y=0;
	//cout << "freq: " << a[0]/2.0/M_PI << endl;
	if (type == DOUBLE_PEAK_FIT){
//		cout << "tupla\n";
		yval=tof_for_two(x,a[0],a[1],a[2],a[3],a[4], a[5],a[6],a[7],a[8],a[9]);
		diff=a[0]*0.01e-7;
		yval1=tof_for_two(x,a[0]+diff,a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8],a[9]);
		dydz[0]=(yval1-yval)/diff;
		diff=a[1]/10000;
		yval1=tof_for_two(x,a[0],a[1]+diff,a[2],a[3],a[4],a[5],a[6],a[7],a[8],a[9]);
		dydz[1]=(yval1-yval)/diff;
		diff=0.01;
		yval1=tof_for_two(x,a[0],a[1],a[2]+diff,a[3],a[4], a[5],a[6],a[7],a[8],a[9]);
		dydz[2]=(yval1-yval)/diff;
		diff=(0.00005);
		yval1=tof_for_two(x,a[0],a[1],a[2],a[3]+diff,a[4], a[5],a[6],a[7],a[8],a[9]);
		dydz[3]=(yval1-yval)/diff;
		//diff=(0.000005);
		diff=a[4]/100.;
		yval1=tof_for_two(x,a[0],a[1],a[2],a[3],a[4]+diff, a[5],a[6],a[7],a[8],a[9]);
		dydz[4]=(yval1-yval)/diff;
		//diff=(0.5/2*M_PI);
		diff=a[5]/10000000.;
		yval1=tof_for_two(x,a[0],a[1],a[2],a[3],a[4], a[5]+diff,a[6],a[7],a[8],a[9]);
		dydz[5]=(yval1-yval)/diff;
		diff=a[6]/1000.;
		yval1=tof_for_two(x,a[0],a[1],a[2],a[3],a[4], a[5],a[6]+diff,a[7],a[8],a[9]);
		dydz[6]=(yval1-yval)/diff;
		diff=a[7]/1000.;
		yval1=tof_for_two(x,a[0],a[1],a[2],a[3],a[4], a[5],a[6],a[7]+diff,a[8],a[9]);
		dydz[7]=(yval1-yval)/diff;
		diff=a[8]*0.01e-7;
		yval1=tof_for_two(x,a[0],a[1],a[2],a[3],a[4], a[5],a[6],a[7],a[8]+diff,a[9]);
		dydz[8]=(yval1-yval)/diff;
		diff=a[9]/100.;
		yval1=tof_for_two(x,a[0],a[1],a[2],a[3],a[4], a[5],a[6],a[7],a[8],a[9]+diff);
		dydz[9]=(yval1-yval)/diff;
	}
	if (type == TRIPLE_PEAK_FIT){
//		cout << "tupla\n";
		yval=tof_for_three(x,a[0],a[1],a[2],a[3],a[4], a[5],a[6],a[7],a[8],a[9],a[10],a[11]);
		diff=a[0]*0.01e-7;
		yval1=tof_for_three(x,a[0]+diff,a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8],a[9],a[10],a[11]);
		dydz[0]=(yval1-yval)/diff;
		diff=a[1]/10000;
		yval1=tof_for_three(x,a[0],a[1]+diff,a[2],a[3],a[4],a[5],a[6],a[7],a[8],a[9],a[10],a[11]);
		dydz[1]=(yval1-yval)/diff;
		diff=0.01;
		yval1=tof_for_three(x,a[0],a[1],a[2]+diff,a[3],a[4], a[5],a[6],a[7],a[8],a[9],a[10],a[11]);
		dydz[2]=(yval1-yval)/diff;
		diff=(0.00005);
		yval1=tof_for_three(x,a[0],a[1],a[2],a[3]+diff,a[4], a[5],a[6],a[7],a[8],a[9],a[10],a[11]);
		dydz[3]=(yval1-yval)/diff;
		//diff=(0.000005);
		diff=a[4]/100.;
		yval1=tof_for_three(x,a[0],a[1],a[2],a[3],a[4]+diff, a[5],a[6],a[7],a[8],a[9],a[10],a[11]);
		dydz[4]=(yval1-yval)/diff;
		//diff=(0.5/2*M_PI);
		diff=a[5]/10000000.;
		yval1=tof_for_three(x,a[0],a[1],a[2],a[3],a[4], a[5]+diff,a[6],a[7],a[8],a[9],a[10],a[11]);
		dydz[5]=(yval1-yval)/diff;
		diff=a[6]/1000.;
		yval1=tof_for_three(x,a[0],a[1],a[2],a[3],a[4], a[5],a[6]+diff,a[7],a[8],a[9],a[10],a[11]);
		dydz[6]=(yval1-yval)/diff;
		diff=a[7]/1000.;
		yval1=tof_for_three(x,a[0],a[1],a[2],a[3],a[4], a[5],a[6],a[7]+diff,a[8],a[9],a[10],a[11]);
		dydz[7]=(yval1-yval)/diff;
		diff=a[8]*0.01e-7;
		yval1=tof_for_three(x,a[0],a[1],a[2],a[3],a[4], a[5],a[6],a[7],a[8]+diff,a[9],a[10],a[11]);
		dydz[8]=(yval1-yval)/diff;
		diff=a[9]/100.;
		yval1=tof_for_three(x,a[0],a[1],a[2],a[3],a[4], a[5],a[6],a[7],a[8],a[9]+diff,a[10],a[11]);
		dydz[9]=(yval1-yval)/diff;
		diff=a[10]*0.01e-7; /////
		yval1=tof_for_three(x,a[0],a[1],a[2],a[3],a[4], a[5],a[6],a[7],a[8],a[9],a[10]+diff,a[11]);
		dydz[10]=(yval1-yval)/diff;
		diff=a[11]/100.;
		yval1=tof_for_three(x,a[0],a[1],a[2],a[3],a[4], a[5],a[6],a[7],a[8],a[9],a[10],a[11]+diff);
		dydz[11]=(yval1-yval)/diff;
	}
	if (type == SINGLE_PEAK_FIT){
//		cout << "single. \n";
		yval=tof(x,a[0],a[1],a[2],a[3],a[4], a[5],a[6],a[7]);
		diff=a[0]*0.01e-7;
		yval1=tof(x,a[0]+diff,a[1],a[2],a[3],a[4],a[5],a[6],a[7]);
		dydz[0]=(yval1-yval)/diff;
		diff=a[1]/10000;
		yval1=tof(x,a[0],a[1]+diff,a[2],a[3],a[4],a[5],a[6],a[7]);
		dydz[1]=(yval1-yval)/diff;
		diff=0.01;
		yval1=tof(x,a[0],a[1],a[2]+diff,a[3],a[4], a[5],a[6],a[7]);
		dydz[2]=(yval1-yval)/diff;
		diff=(0.00005);
		yval1=tof(x,a[0],a[1],a[2],a[3]+diff,a[4], a[5],a[6],a[7]);
		dydz[3]=(yval1-yval)/diff;
		//diff=(0.000005);
		diff=a[4]/100.;
		yval1=tof(x,a[0],a[1],a[2],a[3],a[4]+diff, a[5],a[6],a[7]);
		dydz[4]=(yval1-yval)/diff;
		//diff=(0.5/2*M_PI);
		diff=a[5]/10000000.;
		yval1=tof(x,a[0],a[1],a[2],a[3],a[4], a[5]+diff,a[6],a[7]);
		dydz[5]=(yval1-yval)/diff;
		diff=a[6]/1000.;
		yval1=tof(x,a[0],a[1],a[2],a[3],a[4], a[5],a[6]+diff,a[7]);
		dydz[6]=(yval1-yval)/diff;
		diff=a[7]/1000.;
		yval1=tof(x,a[0],a[1],a[2],a[3],a[4], a[5],a[6],a[7]+diff);
		dydz[7]=(yval1-yval)/diff;
	}
	if (type == SINGLE_PEAK_WITH_CRAP){
//		cout << "normaali 1 with constant tof crap\n";
		yval=tof_with_constant_crap(x,a[0],a[1],a[2],a[3],a[4], a[5],a[6],a[7],a[8],a[9]);
		diff=a[0]*0.01e-7;
		yval1=tof_with_constant_crap(x,a[0]+diff,a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8],a[9]);
		dydz[0]=(yval1-yval)/diff;
		diff=a[1]/10000;
		yval1=tof_with_constant_crap(x,a[0],a[1]+diff,a[2],a[3],a[4],a[5],a[6],a[7],a[8],a[9]);
		dydz[1]=(yval1-yval)/diff;
		diff=0.01;
		yval1=tof_with_constant_crap(x,a[0],a[1],a[2]+diff,a[3],a[4], a[5],a[6],a[7],a[8],a[9]);
		dydz[2]=(yval1-yval)/diff;
		diff=(0.00005);
		yval1=tof_with_constant_crap(x,a[0],a[1],a[2],a[3]+diff,a[4], a[5],a[6],a[7],a[8],a[9]);
		dydz[3]=(yval1-yval)/diff;
		//diff=(0.000005);
		diff=a[4]/100.;
		yval1=tof_with_constant_crap(x,a[0],a[1],a[2],a[3],a[4]+diff, a[5],a[6],a[7],a[8],a[9]);
		dydz[4]=(yval1-yval)/diff;
		//diff=(0.5/2*M_PI);
		diff=a[5]/10000000.;
		yval1=tof_with_constant_crap(x,a[0],a[1],a[2],a[3],a[4], a[5]+diff,a[6],a[7],a[8],a[9]);
		dydz[5]=(yval1-yval)/diff;
		diff=a[6]/1000.;
		yval1=tof_with_constant_crap(x,a[0],a[1],a[2],a[3],a[4], a[5],a[6]+diff,a[7],a[8],a[9]);
		dydz[6]=(yval1-yval)/diff;
		diff=a[7]/1000.;
		yval1=tof_with_constant_crap(x,a[0],a[1],a[2],a[3],a[4], a[5],a[6],a[7]+diff,a[8],a[9]);
		dydz[7]=(yval1-yval)/diff;
		diff=a[8]/100.;
		yval1=tof_with_constant_crap(x,a[0],a[1],a[2],a[3],a[4], a[5],a[6],a[7],a[8]+diff,a[9]);
		dydz[8]=(yval1-yval)/diff;
		diff=a[9]/100.;
		yval1=tof_with_constant_crap(x,a[0],a[1],a[2],a[3],a[4], a[5],a[6],a[7],a[8],a[9]+diff);
		dydz[9]=(yval1-yval)/diff;
	}
	if (type == LINEAR_FIT){ //slope-0, constant-1
//		cout << "Using linear fit. is it intended?\n";
		yval  = x*a[0]+a[1];

		diff=a[0]*0.01e-2;
		yval1 = x*(a[0]+diff)+a[1];
		dydz[0] = (yval1-yval)/diff;

		diff=a[1]*1.0e-3;
		yval1 = x*a[0]+a[1]+diff;
		dydz[1] = (yval1-yval)/diff;
	}
	if (type == GAUSSIAN) { // x0-0, FWHM-1, height-2, bgr-3
//		cout << "gaa-sin\n";
		yval = a[2]*exp( -pow(x-a[0],2.0)*log(16)/pow(a[1],2.0))+a[3];

		diff=a[0]*1e-5;
		yval1 = a[2]*exp( -pow(x-(a[0]+diff),2.0)*log(16)/pow(a[1],2.0))+a[3];
		dydz[0] = (yval1-yval)/diff;

		diff=a[1]*1e-2;
		yval1 = a[2]*exp( -pow(x-a[0],2.0)*log(16)/pow(a[1]+diff,2.0))+a[3];
		dydz[1] = (yval1-yval)/diff;

		diff=a[2]*1e-2;
		yval1 = (a[2]+diff)*exp( -pow(x-a[0],2.0)*log(16)/pow(a[1],2.0))+a[3];
		dydz[2] = (yval1-yval)/diff;

		diff=a[3]*1e-4;
		yval1 = a[2]*exp( -pow(x-a[0],2.0)*log(16)/pow(a[1],2.0))+a[3]+diff;
		dydz[3] = (yval1-yval)/diff;
//		cout << "dydz 0:" << dydz[0] << " 1:" << dydz[1] << " 2:" << dydz[2] << " 3:" << dydz[3] << "\n";
	}
	if (type == SINGLE_RAMSEY) {
		yval=ramsey_tof(x,a[0],a[1],a[2],a[3],a[4], a[5]);
		diff=a[0]*0.01e-7;
		yval1=ramsey_tof(x,a[0]+diff,a[1],a[2],a[3],a[4],a[5]);
		dydz[0]=(yval1-yval)/diff;
		diff=a[1]*0.01;
		yval1=ramsey_tof(x,a[0],a[1]+diff,a[2],a[3],a[4],a[5]);
		dydz[1]=(yval1-yval)/diff;
		diff=a[2]*0.01;
		yval1=ramsey_tof(x,a[0],a[1],a[2]+diff,a[3],a[4], a[5]);
		dydz[2]=(yval1-yval)/diff;
		diff=a[3]*0.001;
		yval1=ramsey_tof(x,a[0],a[1],a[2],a[3]+diff,a[4], a[5]);
		dydz[3]=(yval1-yval)/diff;
		//diff=(0.000005);
		diff=a[4]/100.;
		yval1=ramsey_tof(x,a[0],a[1],a[2],a[3],a[4]+diff, a[5]);
		dydz[4]=(yval1-yval)/diff;
		//diff=(0.5/2*M_PI);
		diff=a[5]/100.;
		yval1=ramsey_tof(x,a[0],a[1],a[2],a[3],a[4], a[5]+diff);
		dydz[5]=(yval1-yval)/diff;
	}
	if (type == SINGLE_RAMSEY_WITH_DAMPING) {
		yval=ramsey_tof_with_damping(x,a[0],a[1],a[2],a[3],a[4], a[5], a[6]);
		diff=a[0]*0.01e-7;
		yval1=ramsey_tof_with_damping(x,a[0]+diff,a[1],a[2],a[3],a[4],a[5], a[6]);
		dydz[0]=(yval1-yval)/diff;
		diff=a[1]*0.01;
		yval1=ramsey_tof_with_damping(x,a[0],a[1]+diff,a[2],a[3],a[4],a[5], a[6]);
		dydz[1]=(yval1-yval)/diff;
		diff=a[2]*0.01;
		yval1=ramsey_tof_with_damping(x,a[0],a[1],a[2]+diff,a[3],a[4], a[5], a[6]);
		dydz[2]=(yval1-yval)/diff;
		diff=a[3]*0.001;
		yval1=ramsey_tof_with_damping(x,a[0],a[1],a[2],a[3]+diff,a[4], a[5], a[6]);
		dydz[3]=(yval1-yval)/diff;
		//diff=(0.000005);
		diff=a[4]/100.;
		yval1=ramsey_tof_with_damping(x,a[0],a[1],a[2],a[3],a[4]+diff, a[5], a[6]);
		dydz[4]=(yval1-yval)/diff;
		//diff=(0.5/2*M_PI);
		diff=a[5]/100.;
		yval1=ramsey_tof_with_damping(x,a[0],a[1],a[2],a[3],a[4], a[5]+diff, a[6]);
		dydz[5]=(yval1-yval)/diff;
		diff=a[6]/100.;
		yval1=ramsey_tof_with_damping(x,a[0],a[1],a[2],a[3],a[4], a[5], a[6]+diff);
		dydz[6]=(yval1-yval)/diff;
	}
	if (type == SINGLE_RAMSEY_WITH_CRAP) {
		yval=ramsey_tof_with_constant_crap(x,a[0],a[1],a[2],a[3],a[4], a[5], a[6], a[7]);
		diff=a[0]*0.01e-7;
		yval1=ramsey_tof_with_constant_crap(x,a[0]+diff,a[1],a[2],a[3],a[4],a[5], a[6], a[7]);
		dydz[0]=(yval1-yval)/diff;
		diff=a[1]*0.01;
		yval1=ramsey_tof_with_constant_crap(x,a[0],a[1]+diff,a[2],a[3],a[4],a[5], a[6], a[7]);
		dydz[1]=(yval1-yval)/diff;
		diff=a[2]*0.01;
		yval1=ramsey_tof_with_constant_crap(x,a[0],a[1],a[2]+diff,a[3],a[4], a[5], a[6], a[7]);
		dydz[2]=(yval1-yval)/diff;
		diff=a[3]*0.001;
		yval1=ramsey_tof_with_constant_crap(x,a[0],a[1],a[2],a[3]+diff,a[4], a[5], a[6], a[7]);
		dydz[3]=(yval1-yval)/diff;
		//diff=(0.000005);
		diff=a[4]/100.;
		yval1=ramsey_tof_with_constant_crap(x,a[0],a[1],a[2],a[3],a[4]+diff, a[5], a[6], a[7]);
		dydz[4]=(yval1-yval)/diff;
		//diff=(0.5/2*M_PI);
		diff=a[5]/100.;
		yval1=ramsey_tof_with_constant_crap(x,a[0],a[1],a[2],a[3],a[4], a[5]+diff, a[6], a[7]);
		dydz[5]=(yval1-yval)/diff;
		diff=a[6]/100.;
		yval1=ramsey_tof_with_constant_crap(x,a[0],a[1],a[2],a[3],a[4], a[5], a[6]+diff, a[7]);
		dydz[6]=(yval1-yval)/diff;
		diff=a[7]/100.;
		yval1=ramsey_tof_with_constant_crap(x,a[0],a[1],a[2],a[3],a[4], a[5], a[6], a[7]+diff);
		dydz[7]=(yval1-yval)/diff;
	}
	*y=yval;
	return;
}
//------------------------------------------------------------------------------
void FIT_FUNC::check_correctness(double a[],int how_many){
  //1. Change w- radius to positive
  if (a[3] < 0){
	a[5] = a[5]+M_PI;
	a[3] = (-a[3]);
  }
  //2. Change w+ radius to positive
  if (a[4] < 0){
	a[5] = a[5]+M_PI;
	a[4] = (-a[4]);
  }
  //3. Put phase between [0,2pi]
  a[5]=fmod(a[5],(2*M_PI));
  if (a[5] < 0)
	a[5] = a[5]+2*M_PI;
  return;
}
//------------------------------------------------------------------------------
double FIT_FUNC::ramsey_tof(double nu_rf, double nu_c, double energy, double rad_minus,
							double time_fringe, double time_wait, double excit_amplitude)
{
	double mu;
	double tof;
	mu=0;

/*	ofstream fileen;
	fileen.open("file1.txt", ios::app);
	fileen.precision(15);
	fileen << "nu_rf, nu_c " << omega_rf/2./M_PI << " " << omega_c/2./M_PI << ",axial, rad_minus" << energy << " " << rad_minus
		<< ", t_fringe, t_wait" << time_fringe << " " << time_wait << ", amplitude " << excit_amplitude << "\n";
*/
    mu=ramsey_2f_conversion(nu_rf,excit_amplitude, time_fringe,time_wait,nu_c,mass,
	                   rad_minus,0,0); //This has to be divided still with initial B-field value
//	cout << "Mu before 7: " << mu << " ";
	if (fast_tof) {
		if (field_points.empty() ) {
			cout << "error.. no field points! <tried to get B[0]>\n";
			return -1;
		}
		mu /= field_points.begin()->b_field0;
	}
	else {
		if (!has_simion_files_been_read) {
			cout << "error.. Simion fields not read.. <tried to get B[0]>\n";
			return -1;
		}
		mu /= b_field[0];
	}
	tof=get_tof(mu,mass,charge,energy);
//	cout << " after 7: " << mu << "\n";
/*	fileen << "mu, mass, energy, tof: " << mu << " " << mass << " " << energy << " " << tof/10. << "\n";
	fileen.close();*/
	return tof/10.;
}
//------------------------------------------------------------------------------
double FIT_FUNC::ramsey_tof_with_damping(double nu_rf, double nu_c, double energy, double rad_minus,
							double time_fringe, double time_wait, double excit_amplitude,double pressure)
{
	double mu;
	double tof;
	mu=0;

/*	ofstream fileen;
	fileen.open("file1.txt", ios::app);
	fileen.precision(15);
	fileen << "nu_rf, nu_c " << omega_rf/2./M_PI << " " << omega_c/2./M_PI << ",axial, rad_minus" << energy << " " << rad_minus
		<< ", t_fringe, t_wait" << time_fringe << " " << time_wait << ", amplitude " << excit_amplitude << "\n";
*/
//	double ramsey_2f_conversion_v2(double omega_rf,   double a_rf, double time_fringe,  double time_wait, double omega_c,
//					   double mass, double pressure, double rad_minus0, double rad_plus0, double phase);


    mu=ramsey_2f_conversion_v2(nu_rf,excit_amplitude, time_fringe,time_wait,nu_c,mass,
	                  pressure,rad_minus,0,0); //This has to be divided still with initial B-field value
//	cout << "Mu before 7: " << mu << " ";
	if (fast_tof) {
		if (field_points.empty() ) {
			cout << "error.. no field points! <tried to get B[0]>\n";
			return -1;
		}
		mu /= field_points.begin()->b_field0;
	}
	else {
		if (!has_simion_files_been_read) {
			cout << "error.. Simion fields not read.. <tried to get B[0]>\n";
			return -1;
		}
		mu /= b_field[0];
	}
	tof=get_tof(mu,mass,charge,energy);
//	cout << " after 7: " << mu << "\n";
/*	fileen << "mu, mass, energy, tof: " << mu << " " << mass << " " << energy << " " << tof/10. << "\n";
	fileen.close();*/
	return tof/10.;
}
//------------------------------------------------------------------------------
double FIT_FUNC::ramsey_tof_with_constant_crap(double nu_rf, double nu_c,
			double energy, double rad_minus, double time_fringe,
			double time_wait, double excit_amplitude,
			double tof_of_crap, double ratio)
{
	double mu;
	double tof;
	mu=0;

    mu=ramsey_2f_conversion(nu_rf,excit_amplitude, time_fringe,time_wait,nu_c,mass,
	                   rad_minus,0,0); //This has to be divided still with initial B-field value
//	cout << "Mu before 7: " << mu << " ";
	if (fast_tof) {
		if (field_points.empty() ) {
			cout << "error.. no field points! <tried to get B[0]>\n";
			return -1;
		}
		mu /= field_points.begin()->b_field0;
	}
	else {
		if (!has_simion_files_been_read) {
			cout << "error.. Simion fields not read.. <tried to get B[0]>\n";
			return -1;
		}
		mu /= b_field[0];
	}
	tof=get_tof(mu,mass,charge,energy);
//	cout << " after 7: " << mu << "\n";
/*	fileen << "mu, mass, energy, tof: " << mu << " " << mass << " " << energy << " " << tof/10. << "\n";
	fileen.close();*/
	return (tof_of_crap*10.*ratio/100. + (100.-ratio)/100.*tof)/10.;
}

//------------------------------------------------------------------------------
double FIT_FUNC::tof(double nu_rf,double nu_c,double pressure,double energy,double rad_min,
					 double rad_plus, double phase, double excit_time, double excit_amplitude)
{

	double mu;
	double tof;
	mu=0;
    mu=magnetic_moment(nu_rf,excit_amplitude,
	                   excit_time,nu_c,mass,pressure,
	                   rad_min,rad_plus, phase); //This has to be divided still with initial B-field value
	if (fast_tof) {
		if (field_points.empty() ) {
			cout << "error.. no field points! <tried to get B[0]>\n";
			return -1;
		}
		mu /= field_points.begin()->b_field0;
	}
	else {
		if (!has_simion_files_been_read) {
			cout << "error.. Simion fields not read.. <tried to get B[0]>\n";
			return -1;
		}
		mu /= b_field[0];
	}
	tof=get_tof(mu,mass,charge,energy);
	return tof/10.;
}
//------------------------------------------------------------------------------
double FIT_FUNC::tof_for_two(double nu_rf,double nu_c1, double pressure,double energy,
				double rad_min,double rad_plus, double phase, double excit_time, double excit_amplitude,
				double nu_c2, double ratio)
{
	double mu1(0), mu2(0);
	double tof1,tof2;
	mu1=magnetic_moment(nu_rf,excit_amplitude,excit_time,nu_c1,mass,pressure,
					   rad_min,rad_plus, phase);
	mu2=magnetic_moment(nu_rf,excit_amplitude,excit_time,nu_c2,mass,pressure,
					   rad_min,rad_plus, phase);
	if (fast_tof) {
		if (field_points.empty() ) {
			cout << "error.. no field points! <tried to get B[0]>\n";
			return -1;
		}
		mu1 /= field_points.begin()->b_field0;
		mu2 /= field_points.begin()->b_field0;
	}
	else {
		if (!has_simion_files_been_read) {
			cout << "error.. Simion fields not read.. <tried to get B[0]>\n";
			return -1;
		}
		mu1 /= b_field[0];
		mu2 /= b_field[0];
	}
	tof1=get_tof(mu1,mass,charge,energy);
	tof2=get_tof(mu2,mass,charge,energy);
//	cout << "Getting TOF for 2\n";
	return (tof2*ratio/100.0 + tof1*(1.0-ratio/100.0))/10.;
}
//------------------------------------------------------------------------------
double FIT_FUNC::tof_for_three(double nu_rf,double nu_c1, double pressure,double energy,
				double rad_min,double rad_plus, double phase, double excit_time, double excit_amplitude,
				double nu_c2, double weight2, double nu_c3, double weight3)
{
	double mu1(0), mu2(0), mu3(0);
	double tof1,tof2,tof3;
	mu1=magnetic_moment(nu_rf,excit_amplitude,excit_time,nu_c1,mass,pressure,
					   rad_min,rad_plus, phase);
	mu2=magnetic_moment(nu_rf,excit_amplitude,excit_time,nu_c2,mass,pressure,
					   rad_min,rad_plus, phase);
	mu3=magnetic_moment(nu_rf,excit_amplitude,excit_time,nu_c3,mass,pressure,
					   rad_min,rad_plus, phase);
	if (fast_tof) {
		if (field_points.empty() ) {
			cout << "error.. no field points! <tried to get B[0]>\n";
			return -1;
		}
		mu1 /= field_points.begin()->b_field0;
		mu2 /= field_points.begin()->b_field0;
		mu3 /= field_points.begin()->b_field0;
	}
	else {
		if (!has_simion_files_been_read) {
			cout << "error.. Simion fields not read.. <tried to get B[0]>\n";
			return -1;
		}
		mu1 /= b_field[0];
		mu2 /= b_field[0];
		mu3 /= b_field[0];
	}
	tof1=get_tof(mu1,mass,charge,energy);
	tof2=get_tof(mu2,mass,charge,energy);
	tof3=get_tof(mu3,mass,charge,energy);
//	cout << "Getting TOF for 3\n";
	// return (tof2*weight2/100.0 + tof1*(1.0-weight2/100.0))/10.;
	return ((tof3*weight3 + tof2*weight2 + tof1*(100.-weight2-weight3))/100.0)/10.;
}
//------------------------------------------------------------------------------
double FIT_FUNC::tof_with_constant_crap(double nu_rf,double nu_c1, double pressure,double energy,
				double rad_min,double rad_plus, double phase, double excit_time, double excit_amplitude,
				double tof_of_crap, double ratio)
{
	double mu(0);
	double tof1;
	mu=magnetic_moment(nu_rf,excit_amplitude,excit_time,nu_c1,mass,pressure,
					   rad_min,rad_plus, phase);
	if (fast_tof) {
		if (field_points.empty() ) {
			cout << "error.. no field points! <tried to get B[0]>\n";
			return -1;
		}
		mu /= field_points.begin()->b_field0;
	}
	else {
		if (!has_simion_files_been_read) {
			cout << "error.. Simion fields not read.. <tried to get B[0]>\n";
			return -1;
		}
		mu /= b_field[0];
	}
	tof1=get_tof(mu,mass,charge,energy);
	return (tof1*(1.0-ratio/100.0) + tof_of_crap*(ratio/100.0))/10.;
}
//------------------------------------------------------------------------------
/*
double FIT_FUNC::stefans_magnetic_moment(double omega_rf, double amplitude, double time_rf, double omega_c,
					   double mass,double pressure,double rad_min,double rad_plus1, double phase1)
{
	double energy,omega_b2,omega_b,k0,delta,theta,theta1,theta2;
	double sinh_real,sinh_img,cosh_real,cosh_img,x;
	double a_real,a_img,sum_real,sum_img;
	double phase,rad_plus;

	k0=amplitude/(2.*7*2.56e-04);
	delta=pressure*omega_c/(1000*7.*18.8e-4);
	phase=phase1;rad_plus=rad_plus1;
	theta=0.;
	{
		omega_b2=0.25*sqrt(((omega_c-omega_rf)*(omega_c-omega_rf)+(delta-k0)*(delta-k0))*
			((omega_c-omega_rf)*(omega_c-omega_rf)+(delta+k0)*(delta+k0)));
		omega_b=sqrt(omega_b2);
		theta1=atan((omega_rf-omega_c)/(delta-k0));
		if (theta1<0)
			theta1=3.1415926+theta1;
		theta2=atan((omega_rf-omega_c)/(delta+k0));
		if (theta2<0)
			theta2=3.1415926+theta2;

		theta=0.5*(theta1+theta2);
		x=omega_b*time_rf;
		sinh_real=0.5*(exp(x*cos(theta))*cos(x*sin(theta))-exp(-x*cos(theta))*cos(x*sin(theta)));
		sinh_img=0.5*(exp(x*cos(theta))*sin(x*sin(theta))+exp(-x*cos(theta))*sin(x*sin(theta)));
		cosh_real=0.5*(exp(x*cos(theta))*cos(x*sin(theta))+exp(-x*cos(theta))*cos(x*sin(theta)));
		cosh_img=0.5*(exp(x*cos(theta))*sin(x*sin(theta))-exp(-x*cos(theta))*sin(x*sin(theta)));
		a_real=(rad_plus*delta+rad_min*k0*cos(phase))/(2*omega_b);
		a_img=(rad_plus*(omega_rf-omega_c)+rad_min*k0*sin(phase))/(2*omega_b);
		sum_real=(rad_plus*cosh_real-sinh_real*(a_real*cos(theta)+a_img*sin(theta))-sinh_img*
			(a_real*sin(theta)-a_img*cos(theta)));
		sum_img=(rad_plus*cosh_img-sinh_img*(a_real*cos(theta)+a_img*sin(theta))+sinh_real*
			(a_real*sin(theta)-a_img*cos(theta)));
		energy=exp(-delta*time_rf)*(sum_real*sum_real+sum_img*sum_img);
		energy=energy*omega_c*omega_c/2;
	}
	energy=energy*mass*1e-08;
	return energy;
}
*/
//------------------------------------------------------------------------------
double FIT_FUNC::magnetic_moment(double nu_rf, double amplitude, double time_rf, double nu_c,
					   double mass,double pressure,double rad_minus0_mm, double rad_plus0_mm, double phase){
	if (abs(nu_c-nu_rf)<0.00001)
	  nu_rf=nu_c+0.00001;

  //2017-01-17 Change so that this only internally uses omega_X 
  double omega_c =  nu_c*2.*M_PI;
  double omega_rf = nu_rf*2.*M_PI;
  
  //2017-01-17 Change radii so that they are internally in m (called with mm)
  double rad_minus0 = rad_minus0_mm/1000.;
  double rad_plus0 = rad_plus0_mm/1000.;
  
  double theta,theta1,theta2,gamma,omega_b2,omega_b1,omega_b,k0,delta,x; //UU
  double omega_minus(500),omega_plus(omega_c-omega_minus); //UU
  double rad_plus, rad_minus;  //UU

    
	//k0=amplitude/(2.*7*2.56e-04);
	//k0 = amplitude/(2.*7*2.56e-04)/time_rf *11.3; // k0 in units of "conversion". Still a mystery how Stefan got 2*7*2.56e-04
	k0 = amplitude/time_rf * M_PI ; //2017-01-17 remove *1000.
	
	delta=pressure*omega_c/(1000*7.*18.8e-4);
    gamma=delta/(omega_plus-omega_minus);
	theta=0.;

	omega_b1 = 0.5*sqrt( pow((omega_rf-omega_c),2)+pow((gamma*omega_c-k0),2) );
	omega_b2 = 0.5*sqrt( pow((omega_rf-omega_c),2)+pow((gamma*omega_c+k0),2) );
	omega_b = sqrt(omega_b1*omega_b2);

	theta1=atan((omega_rf-omega_c)/(gamma*omega_c-k0));
	if (theta1<0)
	  theta1=M_PI+theta1;
	theta2=atan((omega_rf-omega_c)/(gamma*omega_c+k0));
	if (theta2<0)
	  theta2=M_PI+theta2;

	theta=0.5*(theta1+theta2);
	x=omega_b*time_rf;
	double y=0.5*(omega_rf-omega_c);
	double l=cosh(x*cos(theta))*cos(x*sin(theta)); //Re[cosh(xe^(i*theta))]
	double m=sinh(x*cos(theta))*sin(x*sin(theta)); //Im[cosh(xe^(i*theta))]
	double n=sinh(x*cos(theta))*cos(x*sin(theta)); //Re[sinh(xe^(i*theta))]
	double p=cosh(x*cos(theta))*sin(x*sin(theta)); //Im[sinh(xe^(i*theta))]
	double q1=.5/omega_b*(rad_plus0 *(gamma*omega_c*cos(theta)+2*y*sin(theta))+rad_minus0*k0*(cos(phase)*cos(theta)+sin(phase)*sin(theta)));
	double q2=.5/omega_b*(rad_minus0*(gamma*omega_c*cos(theta)+2*y*sin(theta))+rad_plus0 *k0*(cos(phase)*cos(theta)-sin(phase)*sin(theta)));

	double r1=.5/omega_b*(rad_plus0 *(-gamma*omega_c*sin(theta)+2*y*cos(theta))+rad_minus0*k0*(-cos(phase)*sin(theta)+sin(phase)*cos(theta)));
	double r2=.5/omega_b*(rad_minus0*(-gamma*omega_c*sin(theta)+2*y*cos(theta))+ rad_plus0*k0*(-cos(phase)*sin(theta)-sin(phase)*cos(theta)));

	double rad_plus_RE = exp(-delta*time_rf*.5)*(cos(y*time_rf)*(rad_plus0 *l-(q1*n-r1*p))-sin(y*time_rf)*(rad_plus0 *m-(r1*n+p*q1)));
	double rad_minus_RE= exp(-delta*time_rf*.5)*(cos(y*time_rf)*(rad_minus0*l+(q2*n-r2*p))-sin(y*time_rf)*(rad_minus0*m+(r2*n+p*q2)));

	double rad_plus_IM = exp(-delta*time_rf*.5)*(sin(y*time_rf)*(rad_plus0 *l-(q1*n-r1*p))+cos(y*time_rf)*(rad_plus0 *m-(r1*n+p*q1)));
	double rad_minus_IM= exp(-delta*time_rf*.5)*(sin(y*time_rf)*(rad_minus0*l+(q2*n-r2*p))+cos(y*time_rf)*(rad_minus0*m+(r2*n+p*q2)));

	rad_minus = sqrt(pow(rad_minus_RE,2) + pow(rad_minus_IM,2));
	rad_plus  = sqrt(pow(rad_plus_RE, 2) + pow(rad_plus_IM, 2));
	double energy_minus = 0.5*mass*1.660520e-27/1.602177e-19*pow((rad_minus*omega_minus),2);
	double energy_plus  = 0.5*mass*1.660520e-27/1.602177e-19*pow((rad_plus*omega_plus  ),2);
	double energy = energy_minus+energy_plus;
	return energy; //Returns radial energy in eV
}
//------------------------------------------------------------------------------
//2006-11-21 	Returns radial energy. Assumes that there is no rad_plus0, thus no phase either
//						Assumes symmetric pattern T_fringe + T_wait + T_fringe
//						Perhaps in future: Add conversion of w+.
double FIT_FUNC::ramsey_2f_conversion(double nu_rf, double conversions, double time_fringe, double time_tot, double nu_c,
					   double mass, double rad_minus0_mm,double rad_plus0_mm, double phase)
{
//	cout << "w_rf, a_rf, t_fringe, t_wait " << omega_rf << " " << a_rf << " " << time_fringe << " " << time_wait << "\n";
//	cout << "omega_c, mass, rad_minus0, rad_plus0, phase" << omega_c << " " << mass << " " << rad_minus0 << " " << rad_plus0 << " " << phase << "\n";

  //2017-04-25 Change time_wait -> time_tot
  double time_wait = time_tot - 2.*time_fringe;
  
  //2017-01-17 Change so that this only internally uses omega_X 
  double omega_c =  nu_c*2.*M_PI;
  double omega_rf = nu_rf*2.*M_PI;
  
  //2017-01-17 Change radii so that they are internally in m (called with mm)
  double rad_minus0 = rad_minus0_mm/1000.;
  double rad_plus0 = rad_plus0_mm/1000.;


	double delta = omega_rf - omega_c;
	//double g = a_rf * (M_PI/2./0.4) /0.028;
	double g = conversions /(time_fringe) * (M_PI)*250. /1000.; // a_rf in units of "conversion" //2017-01-17 add /1000.
	double omega_R = sqrt( pow(2.*g ,2.0) + pow(delta , 2.0)  );

	double conversion = pow(2.*g/omega_R  *sin(omega_R*time_fringe/2.) ,2.0)
							*pow(2.*              cos(delta*time_wait/2.)*cos(omega_R*time_fringe/2.)
								-2.*delta/omega_R*sin(delta*time_wait/2.)*sin(omega_R*time_fringe/2.)
							 ,2.0);
//	double rad_plus = conversion*rad_minus0;
	double omega_plus = omega_c; //Assumes w_+ ~ w_-
	double energy_plus  = conversion*0.5*mass*1.660520e-27/1.602177e-19*pow((rad_minus0*omega_plus  ),2.);
//	cout << "energy: " << energy_plus << "\n";
	return energy_plus;
}
//------------------------------------------------------------------------------
double FIT_FUNC::ramsey_2f_asymmetric_conversion(

	double nu_rf, double conversions, double time_fringe1,
	double time_tot, double nu_c,
	double mass, double rad_minus0_mm,double rad_plus0_mm, double phase, double time_fringe2)
{
  //2017-04-25 Change time_wait -> time_tot
  double time_wait = time_tot - time_fringe1 - time_fringe2;

  //2017-01-17 Change so that this only internally uses omega_X 
  double omega_c =  nu_c*2.*M_PI;
  double omega_rf = nu_rf*2.*M_PI;

  //2017-01-17 Change radii so that they are internally in m (called with mm)
  double rad_minus0 = rad_minus0_mm/1000.;
  double rad_plus0 = rad_plus0_mm/1000.;

	double delta = omega_rf - omega_c;
	//double g = a_rf * (M_PI/2./0.4) /0.028;
	double g = conversions /(time_fringe1 + time_fringe2) * (M_PI)*500. /1000.; // a_rf in units of "conversion" //2017-01-17 add /1000.
	double omega_R = sqrt( pow(2.*g ,2.0) + pow(delta , 2.0)  );


	//osia yhtalosta
	double aa1 = cos(delta*time_fringe1/2.0)*sin(omega_R*(time_fringe1+time_fringe2)/2.0);
	double aa2 = delta/omega_R*sin(delta*time_wait/2.0)*(cos(omega_R*(time_fringe1+time_fringe2)/2.0)
					- cos(omega_R*(time_fringe1-time_fringe2)/2.0) );
	double aa3 = pow(2.*g/omega_R*sin(delta*time_wait/2.0)*sin(omega_R*(time_fringe1+time_fringe2)/2.0),2.0);

	double conversion = pow(2.*g/omega_R,2.0)*pow(aa1+aa2,2.0)+aa3;
//	double rad_plus = conversion*rad_minus0;
	double omega_plus = omega_c; //Assumes w_+ ~ w_-
	double energy_plus  = conversion*0.5*mass*1.660520e-27/1.602177e-19*pow((rad_minus0*omega_plus  ),2.);
//	cout << "energy: " << energy_plus << "\n";
	return energy_plus;
}
//------------------------------------------------------------------------------
double FIT_FUNC::ramsey_2f_radius_conversion(double nu_rf, double a_rf, double time_fringe, double time_wait, double nu_c,
					   double mass, double rad_minus0_mm,double rad_plus0_mm, double phase)
{
  //2017-01-17 Change so that this only internally uses omega_X 
  double omega_c =  nu_c*2.*M_PI;
  double omega_rf = nu_rf*2.*M_PI;

  //2017-01-17 Change radii so that they are internally in m (called with mm)
  double rad_minus0 = rad_minus0_mm/1000.;
  double rad_plus0 = rad_plus0_mm/1000.;

  
	double delta = omega_rf - omega_c;
	double g = a_rf * (M_PI/2./0.4) /0.028;
	double omega_R = sqrt( pow(2.*g ,2.0) + pow(delta , 2.0)  );

	double conversion = pow(2.*g/omega_R  *sin(omega_R*time_fringe/2.) ,2.0)
							*pow(2.*              cos(delta*time_wait/2.)*cos(omega_R*time_fringe/2.)
								-2.*delta/omega_R*sin(delta*time_wait/2.)*sin(omega_R*time_fringe/2.)
							 ,2.0);
	return sqrt(conversion);
}

//------------------------------------------------------------------------------
double FIT_FUNC::get_tof(double mu, double mass, double charge, double energy)
{
	if ( fast_tof == true) {
		return get_tof_faster(mu,mass,charge,energy);
	}
	if (!has_simion_files_been_read) {
		cout << "Simion files are not read in yet!\n";
		return -1;
	}
	double tof,factor;
	double q;
	int i;
	tof=0;
	q=1;
	for (i=0;i<1600;i++)
	{
		factor=mass/(2*(energy-q*(e_field[i]-e_field[0])-mu*(b_field[i]-b_field[0])));
//		printf("\n factor %f",e_field[i]);
		tof+=sqrt(factor);
	}
//	printf("\n tof %f",tof);
	return tof;
}
//------------------------------------------------------------------------------
double FIT_FUNC::get_tof_faster(double mu, double mass, double charge, double ax_energy)
{
	double time_of_flight(0),q(charge),this_tof=0;
	double a,b,c,x1,x2;
//	cout << "getting fast tof..\n";
	cout.precision(10);
	//total energy = axial kinetic + electric potential + radial energy
	double total_energy = ax_energy+q*field_points[0].e_field0+mu*field_points[0].b_field0;
//	for (int i=0;i < number_of_field_points; i++){
	for (vector<FIELD_POINTS>::iterator i=field_points.begin(); i != field_points.end(); ++i) {
		if (field_points.empty() ) {
			cout << "THERE is no field points!! <tried to get tof>\n";
			return -1;
		}
		x1 = i->start*1.0;
		x2 = i->stop*1.0;
		a = - mu*i->b_field2;
		b = - mu*i->b_field1;
		c = total_energy -q*i->e_field0 - mu*i->b_field0;
//		cout << "a " << a << "\t";
//		cout << "b " << b << "\t";cout << "c " << c << "\n";
		if ( fabs(a) > EPSILON && fabs(b) > EPSILON) {
			if (a > 0) {
				if (b*b - 4.0*a*c > 0) {
					this_tof = 1.0/sqrt(a) *(	//Schaum 17.13.1
						  log(2.0*sqrt(a)*sqrt(a*pow(x2,2.0)+b*x2+c)+2.0*a*x2+b)
						- log(2.0*sqrt(a)*sqrt(a*pow(x1,2.0)+b*x1+c)+2.0*a*x1+b)  );
				} else if (b*b - 4.0*a*c < 0) {
					this_tof = 1.0/sqrt(a) *(
						  arsinh( (2.0*a*x2+b)/sqrt(4.0*a*c-b*b)   )
						- arsinh( (2.0*a*x1+b)/sqrt(4.0*a*c-b*b)   ) );
				//cout <<  "This tof0a: " << this_tof << "\n";
				}
			} else if (a < 0) {
				if ( (b*b -4.0*a*c) > 0) {
					this_tof = - 1.0 / sqrt (-a) *(
						  asin ((2.0*a*x2 +b ) / sqrt(b*b-4.0*a*c )  )
						- asin ((2.0*a*x1 +b ) / sqrt(b*b-4.0*a*c )  )	);
				} else {
//					string foo;
//					cout << "Ei ole!\n";
//					getline(cin,foo);
				}
				//cout <<  "This tof0b: " << this_tof << "\n";
			}
		} else if (fabs(a) <= EPSILON && fabs(b) > EPSILON) {
			// Schaum 17.2.1
			this_tof = 2.0 / sqrt(b) *(
				  sqrt(b*x2+c)
				- sqrt(b*x1+c)		);
			//cout <<  "This tof1: " << this_tof << "\n";
		} else if (fabs(a) <= EPSILON && fabs(b) <= EPSILON) {
			// I could do it!
			this_tof = 1.0 / sqrt(c) *(x2-x1);
//			cout <<  "This tof2: " << this_tof << "\n";
		}
		time_of_flight += this_tof;
	}

//	cout << "field_points:" << field_points.size() << "\tFinal tof: " << time_of_flight << "\n";
	return (time_of_flight*sqrt(mass/2.0));
}
//------------------------------------------------------------------------------
bool FIT_FUNC::put_as_number(const string &text, double &number){
	//for (int i=0;i<number_of_consts;i++){  //Checking, if already existing!
	for (vector<CONSTS>::iterator i=consts.begin(); i != consts.end(); ++i) {
		if (consts.empty() )
			break;
		if ( text[0] == i->character){
			number = i->value;
			return 1;
		}
	}
	if (luvuksi(text,number))
		return 1;
	return 0;
}
//------------------------------------------------------------------------------
int FIT_FUNC::get_approx_field(string field_file){
	boost::filesystem::ifstream file;
	open_local_or_rc_file(file, field_file.c_str());
	if (!file)
		return 3;
	field_points.clear();
	consts.clear();
	string rawrow,row,subrow;
	while (getline(file,rawrow))
	{
		row = erota(rawrow,'#',false);
		if (row[0] != '[')
			continue;
		subrow = erota(row,']');
		if (subrow == "[CONSTANTS")
			do{
				getline(file,rawrow);
				if (rawrow[0] == '[')
					break;
				if (rawrow[0] == '#')
					continue;
				row = erota(rawrow,'#',false);
				subrow = erota(row,'\t');
				CONSTS toAdd;
				toAdd.character = subrow[0];
				subrow = erota(row,'\t');
				luvuksi(subrow,toAdd.value);
				consts.push_back(toAdd);
			}	while (1);
		row = erota(rawrow,']');
		if (row == "[FIELDS")
			while (getline(file,rawrow))
			{
				if (row[0] == '#')
					continue;
				if (row == "")
					continue;
				row = erota(rawrow,'\t');
				FIELD_POINTS toAdd;
				luvuksi(row,toAdd.start);
				row = erota(rawrow,'\t');
				luvuksi(row,toAdd.stop);
				row = erota(rawrow,'\t');
				if (!put_as_number(row,toAdd.e_field0))
					return 1; //Returns error, if unknown constant
				row = erota(rawrow,'\t');
				if (!put_as_number(row,toAdd.b_field0))
					return 1; //Returns error, if unknown constant
				row = erota(rawrow,'\t');
				if (!put_as_number(row,toAdd.b_field1))
					return 1; //Returns error, if unknown constant
				row = erota(rawrow,'\t');
				if (!put_as_number(row,toAdd.b_field2))
					return 1; //Returns error, if unknown constant
				field_points.push_back(toAdd);
				std::cout << toAdd.start << "\t" << toAdd.stop << "\t" << toAdd.e_field0
						<< "\t" << toAdd.b_field0 << "\t" << toAdd.b_field1 << "\t"
						<< toAdd.b_field1 << "\t" << toAdd.b_field2 << "\n";
			}
	}
	file.close();
	return 0;
}
//------------------------------------------------------------------------------
void FIT_FUNC::print_fieldpoints(ostream &log){ //for debugging purposes
	cout << "\n";
	if ( field_points.empty() ) {
		cout << "THERE ARE NO FIELD POINTS!! Did you run lakritsi in right directory?\n";
		return;
	}
	for (vector<FIELD_POINTS>::iterator i=field_points.begin();i != field_points.end();++i){
		log << i->start << '\t';
		log << i->stop << '\t';
		log << i->e_field0 << '\t';
		log << i->b_field0 << '\t';
		log << i->b_field1 << '\t';
		log << i->b_field2 << '\t';
		log << endl;
	}
	return;
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//This reads the magnetic field from SIMION file
bool FIT_FUNC::get_fields(string e_field_file, string b_field_file, int startpoint=276, int endpoint=1599){
		FILE *fin;
	double data;
	long counter;
	double edata[1600],vdata[4400];
	char fname[100];
	long maxe;

    int start_pa=startpoint;//118;  //Simion potential array coordinate, where precision trap is
    int pa_break=endpoint;//1345; //Length of Simion array. After this, pot will be zero
    int stop_pa =1600;//1600; //Simion potential array coordinate, where tof is recorded



	strcpy(fname,e_field_file.c_str());
//	strcpy(fname,"/home/tomero/ohjelmat/potential_files/trap10v_slow_flatpot.pa0");
//	printf("\n E-field filename: %s \n",fname);
	if ( (fin=fopen(fname,"rb"))==0){
		fprintf(stderr,"Can't open %s for reading. File does not exist?\n",fname);
		return 1;
    }

	counter=0;
	size_t daa;
	daa = fread(&data, sizeof(double),1,fin);
	daa = fread(&data, sizeof(double),1,fin);
	daa = fread(&data, sizeof(double),1,fin);
	daa = fread(&data, sizeof(double),1,fin);

		if (fread(&edata[0], pa_break*sizeof(double),1,fin)!=1)
			printf("errorr");

	fclose(fin);
		for (counter=start_pa;counter<pa_break;counter++)
	{
		e_field[counter-start_pa]=edata[counter];
	}
		for (counter=pa_break-start_pa;counter<stop_pa;counter++) //rest of the pot array is zero
			e_field[counter]=0;

//		Next for-loop is for checking correctness of field file
//		for (counter=0;counter<(stop_pa-start_pa);counter++){
//		  cout << counter << "\t" << e_field[counter] << "\n";
//		}

	maxe=50;
//		strcpy(fname,"/home/tomero/ohjelmat/potential_files/mag.pa");
	strcpy(fname,b_field_file.c_str());
//	printf("\n B-field filename: %s \n",fname);
	if ( (fin=fopen(fname,"rb"))==0)
    {
		fprintf(stderr,"Can't open %s for reading. File does not exist?\n",fname);
		return 1;
    }

	counter=0;

	daa = fread(&data, sizeof(double),1,fin);
	daa = fread(&data, sizeof(double),1,fin);
	daa = fread(&data, sizeof(double),1,fin);
	daa = fread(&data, sizeof(double),1,fin);
	if (fread(&vdata[0], 4400*sizeof(double),1,fin)!=1)
		printf("\n %f",vdata[0]);

	fclose(fin);

	for (counter=2318;counter<3918;counter++){
	  b_field[counter-2318]=-((vdata[counter-1]-vdata[counter+1])/200.);
	}
	has_simion_files_been_read = true;
	return 0;
}
void FITTING::mrqmin(double x[], double y[], double sig[], int ndata, double a[], int ia[],
	 int ma, double **covar, double **alpha, double *chisq, double *alamda, FIT_FUNC_TYPE func_type)
{
	 int j,k,l;
	 static int mfit;
	 static double ochisq, *atry, *beta, *da, **oneda;
	 if (*alamda < 0.)
	 {
//		 cout << "Func type: " << func_type << endl;
		 atry=vector(0,ma);
		 beta=vector(0,ma);
		 da=vector(0,ma);
		 for (mfit=0,j=0;j<ma;j++)
			if (ia[j]) mfit++;
		 oneda=matrix(0,mfit,0,1);
		 *alamda=0.001;
		 mrqcof(x,y,sig,ndata,a,ia,ma,alpha,beta,chisq, func_type);
	 	 ochisq=(*chisq);
		 for (j=0;j<ma;j++)
			atry[j]=a[j];
	 }
	 for (j=0;j<mfit;j++)
	 {
		 for (k=0;k<mfit;k++)
				covar[j][k]=alpha[j][k];
		 covar[j][j]=alpha[j][j]*(1.0+(*alamda));
		 oneda[j][0]=beta[j];
	 }

	 gaussj(covar,mfit, oneda,1);

	 for (j=0;j<mfit;j++)
		da[j]=oneda[j][0];

	 if (*alamda == 0.0)
	 {
		 covsrt(covar,ma,ia,mfit);
		 covsrt(alpha,ma,ia,mfit);
		 free_matrix(oneda,0,mfit,0,1);
		 free_vector(da,0,ma);
		 free_vector(beta,0,ma);
		 free_vector(atry,0,ma);
		 return;
	 }
	 for (j=0,l=0;l<ma;l++)
		 if (ia[l]) {
			 atry[l]=a[l]+da[j];
			 j++;
		 }

	mrqcof(x,y,sig,ndata,atry,ia,ma,covar,da,chisq, func_type);

	if(*chisq < ochisq)
	{
		*alamda*=0.1;
		ochisq=(*chisq);
		for (j=0;j<mfit;j++)
		{
			for (k=0;k<mfit;k++)
				alpha[j][k]=covar[j][k];
			beta[j]=da[j];
		}
		for (l=0;l<ma;l++)
			a[l]=atry[l];
	}
	else
	{
		*alamda *= 10.0;
		*chisq =ochisq;
	}
	if ( (func_type == SINGLE_PEAK_FIT) || (func_type == DOUBLE_PEAK_FIT) )
		check_correctness(a,8);
 }


void FITTING::mrqcof(double x[], double y[], double sig[], int ndata, double a[], int ia[],
			int ma, double **alpha, double beta[], double *chisq, FIT_FUNC_TYPE func_type)
{
  int i,j,k,l,m,mfit=0;
  double ymod,wt,sig2i,dy,*dyda;
  dyda=vector(0,ma);
  for (j=0;j<ma;j++)
    if (ia[j]) mfit++;
    for (j=0;j<mfit;j++){
      for (k=0;k<=j;k++)
        alpha[j][k]=0.0;
      beta[j]=0.0;
    }
    *chisq=0.;
    for (i=0;i<ndata;i++){
      funcs(x[i],a,&ymod,dyda,ma, func_type);
      sig2i=1.0/(sig[i]*sig[i]);
      dy=y[i]-ymod;
      for (j=0,l=0;l<ma;l++){
        if (ia[l]){
      	  wt=dyda[l]*sig2i;
       	  for (k=0,m=0;m<=l;m++)
            if (ia[m]){
	      alpha[j][k] += wt*dyda[m];
	      k++;
	    }
        beta[j] += dy*wt;
        j++;
      }
    }
    *chisq +=dy*dy*sig2i;
  }
  for (j=1;j<mfit;j++)
    for (k=0;k<j;k++)
      alpha[k][j]=alpha[j][k];
  free_vector(dyda,0,ma);
}

void FITTING::covsrt(double **covar, int ma, int ia[] , int mfit){
  int i,j,k;
  for (i=mfit;i<ma;i++)
    for (j=0;j<i;j++)
      covar[i][j]=covar[j][i]=0.0;
  k=mfit-1;
  for (j=ma-1;j>=0;j--){
    if (ia[j]){
      for (i=0;i<ma;i++) SWAPP(covar[i][k],covar[i][j])
      for (i=0;i<ma;i++) SWAPP(covar[k][i],covar[j][i])
      k--;
    }
  }
}
void FITTING::gaussj(double **a, int n, double **b, int m)
{
  int *indxc,*indxr,*ipiv;
  int i,icol(0),irow(0),j,k,l,ll;
  double big,dum,pivinv;
  indxc=ivector(0,n);
  indxr=ivector(0,n);
  ipiv=ivector(0,n);
  for (j=0;j<n;j++)
    ipiv[j]=0;
  for (i=0;i<n;i++){
    big=0.0;
    for (j=0;j<n;j++)
      if (ipiv[j] != 1){
        for (k=0;k<n;k++){
	  if (ipiv[k] ==0){
	    if (fabs(a[j][k]) >= big){
	      big=fabs(a[j][k]);
	      irow=j;
	      icol=k;
            }
	  }
	}
      }
    ++(ipiv[icol]);
    if (irow != icol){
      for (l=0;l<n;l++) SWAPP(a[irow][l],a[icol][l])
      for (l=0;l<m;l++) SWAPP(b[irow][l],b[icol][l])
    }
    indxr[i]=irow;
    indxc[i]=icol;
    if (a[icol][icol] == 0.0){
//      nrerror("gaussj: singular matrix");
	  std::cout << "gaussj: singular matrix\n";
		free_ivector(ipiv,0,n);
  		free_ivector(indxr,0,n);
  		free_ivector(indxc,0,n);
	  return;
	}
    pivinv=1.0/a[icol][icol];
    a[icol][icol]=1.0;         //Kosh
    for (l=0;l<n;l++) a[icol][l] *=pivinv;
    for (l=0;l<m;l++) b[icol][l] *=pivinv;
    for (ll=0;ll<n;ll++)
      if (ll !=icol){
	dum=a[ll][icol];
	a[ll][icol]=0.0;
	for (l=0;l<n;l++) a[ll][l] -=a[icol][l]*dum;
	for (l=0;l<m;l++) b[ll][l] -= b[icol][l]*dum;
      }
  }
  for (l=n-1;l>=0;l--){
    if (indxr[l] != indxc[l])
      for (k=0;k<n;k++)
	SWAPP(a[k][indxr[l]],a [k][indxc[l]]);
  }
  free_ivector(ipiv,0,n);
  free_ivector(indxr,0,n);
  free_ivector(indxc,0,n);
}
double FIT_FUNC::funk(double a[], double x[], double y[], double sig[], int ndata)
{
  int i;
  double arg, ex, chisq,ytemp;
  chisq=0.;
  for (i=0;i<ndata;i++){
    arg=(x[i]-a[2])/(a[3]);
    ex=exp(-arg*arg/2.);
    ytemp=a[0]+a[1]*ex;
    ytemp=ytemp-y[i];
    chisq += ytemp*ytemp/(sig[i]*sig[i]);
  }
  return chisq;
}

//-----------------------------------------------------------------------------
std::string get_fit_function_type_as_text(FIT_FUNC_TYPE & type) {
	if (type == SINGLE_PEAK_FIT)
		return "Conventional_with_1_peak";
	if (type == SINGLE_RAMSEY)
		return "Ramsey_with_1_peak";
	if (type == SINGLE_RAMSEY_WITH_DAMPING)
		return "Ramsey_with_1_peak_and_damping";
	if (type == DOUBLE_PEAK_FIT)
		return "Conventional_with_2_peaks";
	if (type == SINGLE_PEAK_WITH_CRAP )
		return "Conventional_with_1_peak_and_crap";
	if (type == SINGLE_RAMSEY_WITH_CRAP)
		return "Ramsey_with_1_peak_and_crap";
	if (type == GAUSSIAN)
		return "Gaussian";
	if (type == LINEAR_FIT)
		return "Linear";
	if (type == TRIPLE_PEAK_FIT)
		return "Conventional_with_3_peaks";
	if (type == QUAD_PEAK_FIT)
		return "Conventional_with_4_peaks";
	return "unknown type";
}

//-----------------------------------------------------------------------------
//This is symmetric Ramsey fit fucntion with buffer gas damping. Sorted out by J. Hakala from PhD thesis of S. George, 2010
double FIT_FUNC::ramsey_2f_conversion_v2(
    double nu_rf, double a_rf, double time_fringe, double time_tot, double nu_c, double mass, double pressure, double rad_minus0_mm, double rad_plus0_mm, double phase)
{
  //2017-04-25 Change time_wait -> time_tot
  double time_wait = time_tot - 2.*time_fringe;
  
  //2017-01-17 Change so that this only internally uses omega_X 
  double omega_c =  nu_c*2.*M_PI;
  double omega_rf = nu_rf*2.*M_PI;

  //2017-01-17 Change radii so that they are internally in m (called with mm)
  double rad_minus0 = rad_minus0_mm/1000.;
  double rad_plus0 = rad_plus0_mm/1000.;

    dcmplx ii(0, 1);
    double omega_minus = 2 * M_PI * 180;
    double omega_plus  = omega_c - omega_minus;

    /* Frequency detuning: S. George, thesis, p. 54 */
    double detuning = nu_rf - omega_c;
    // double detuning_p2 = pow(detuning, 2.0);

    /* Coupling constant, g = q*C_1/(2*m*w_1) ? */
    // double g = a_rf * (M_PI / 2. / 0.4) / 0.028;
    double g = a_rf /(time_fringe) * (M_PI)*250. /1000.; // a_rf in units of "conversion" //2017-01-17 add /1000.
    double g_p2 = pow(g, 2.0);

    // double delta = pressure * omega_c / (1000 * 7. * 18.8e-4);
    // double gamma = delta / (omega_plus - omega_minus);
    double gamma_p2 = pow(pressure, 2.0);

    dcmplx detuning_dash(detuning, 2.0 * pressure);

    /* Rabi frequency of the interconversion */
    /* double omega_R = sqrt(4*g_p2 + detuning_p2); */
    // double omega_R_p2 = detuning_p2 + 4.0 * g_p2;
    dcmplx omega_R_dash = sqrt(pow(detuning_dash, 2.0) + 4.0 * g_p2);

    double omega_R_dash_abs_p2 = pow(abs(omega_R_dash), 2.0);

    // Eronen thesis (3.1)
    // double d_p2 = 2 * z0 * z0 + r0 * r0;
    // double omega_z_p2 = sqrt(q * U0 / (mass * d));
    double d_p2 = 26.4e-3;
    double omega_z_p2 = (1.602177e-19 * 10) / (d_p2 * 1.660520e-27) / mass;
    double omega_1_p2 = pow(omega_c, 2.0) - 2 * omega_z_p2;
    /* 4.11 */
    double gamma1_tilde = M_SQRT1_2
	* sqrt(sqrt(pow(omega_1_p2 - 4 * gamma_p2, 2.0)
		    + 16 * gamma_p2 * pow(omega_c, 2.0))
	       - (omega_1_p2 - 4 * gamma_p2));

    /* S. George, 4.49 */
    dcmplx conv_1(cos(dcmplx(.5 * detuning * time_wait, - pressure * time_wait)));
    /* S. George, 4.50 */
    dcmplx conv_2(sin(dcmplx(.5 * detuning * time_wait, - pressure * time_wait)));

    /* 4.43 */
    dcmplx ding = detuning_dash / omega_R_dash;

    /* S. George, 4.48 */
    double conversion =
	exp(- gamma1_tilde * (2 * time_fringe + time_wait))
	* (4 * g_p2 / omega_R_dash_abs_p2)
	* pow(
	    abs(conv_1 * sin(omega_R_dash * time_fringe)
		+ ding * conv_2 * (cos(omega_R_dash * time_fringe) - 1.0)),
	    2.0);

    double energy_plus =
        0.5 * mass * 1.660520e-27 / 1.602177e-19
	* conversion * pow((rad_minus0 * omega_plus), 2.);

    return energy_plus;
}
//-----------------------------------------------------------------------------
bool is_fit_func_type_konig_type(FIT_FUNC_TYPE & tama) {
	if (tama == SINGLE_PEAK_FIT || tama == DOUBLE_PEAK_FIT || tama == SINGLE_PEAK_WITH_CRAP ||
	    tama == TRIPLE_PEAK_FIT || tama == QUAD_PEAK_FIT )
	  return 1;
	else return 0;
}
//-----------------------------------------------------------------------------
bool is_fit_func_type_ramsey_type(FIT_FUNC_TYPE & tama) {
	if (tama == SINGLE_RAMSEY || tama == SINGLE_RAMSEY_WITH_CRAP || tama == SINGLE_RAMSEY_WITH_DAMPING)
	  return 1;
	else return 0;
}
//-----------------------------------------------------------------------------
std::string get_fit_parameter_name(FIT_FUNC_TYPE & func_type, int param_number)
{
  if ( is_fit_func_type_konig_type(func_type))
  {
    if ( param_number == 0 )
      return "Freq1 [Hz]";
    else if (param_number == 1)
      return "Pressure [mbar]";
    else if (param_number == 2)
      return "Ax_E(0) [eV]";
    else if (param_number == 3)
      return "R-(0) [mm]";
    else if (param_number == 4)
      return "R+(0) [mm]";
    else if (param_number == 5)
      return "Phase [rad]";
    else if (param_number == 6)
      return "T_RF [s]";
    else if (param_number == 7)
      return "Conv [1]";
    
    if ( func_type == DOUBLE_PEAK_FIT || func_type == TRIPLE_PEAK_FIT )
    {
      if ( param_number == 8 )
        return "Freq2 [Hz]";
      else if ( param_number == 9 )
        return "Peak2 % [%]";
    }
    
    if ( func_type == SINGLE_PEAK_WITH_CRAP )
    {
      if ( param_number == 8 )
        return "Crap TOF [us]";
      else if (param_number == 9 )
        return "Crap % [%]";
    }
    
    if ( func_type == TRIPLE_PEAK_FIT )
    {
      if ( param_number == 10 )
        return "Freq3 [Hz]";
      else if ( param_number == 11 )
        return "Peak3 % [%]";
    }
  }
  else if ( is_fit_func_type_ramsey_type(func_type) )
  {
    if ( param_number == 0 )
      return "Freq1 [Hz]";
    else if (param_number == 1)
      return "Ax_E(0) [eV]";
    else if (param_number == 2)
      return "R-(0) [mm]";
    else if (param_number == 3)
      return "T_RF [s]";
    else if (param_number == 4)
      return "T_wait [s]";
    else if (param_number == 5)
      return "Conv [1]";
    
    if ( func_type == SINGLE_RAMSEY_WITH_CRAP )
    {
      if ( param_number == 6 )
        return "Crap TOF [us]";
      else if ( param_number == 7 )
        return "Crap % [%]";
    }
    
    if ( func_type == SINGLE_RAMSEY_WITH_DAMPING )
    {
      if ( param_number == 6 )
        return "Press [mbar]";
    }
  }
  
  return "Unknown";
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

