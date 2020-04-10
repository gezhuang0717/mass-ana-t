#ifndef UTIL_HH
#define UTIL_HH

#include <iostream>
#include <vector>

#include <glibmm/ustring.h>

#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#undef BOOST_NO_CXX11_SCOPED_ENUMS

#include "mjonotpp.hh"

using namespace std;

enum STATISTICS_MODE {
	INDIVIDUAL_STATISTICS = 0,
	SUM_STATISTICS = 1,
	SUM_STATISTICS2 = 2
};

enum DATAFILEFORMAT {
	DATAFORMAT_OLD = 1, //The format used up to ~2006, where many files are saved
	DATAFORMAT_NEW = 0, //Format used with labview mass scanner "bunches.dat"
	DATAFORMAT_NEWER = 2, //Format outputted from "python" parallel scanner
	DATAFORMAT_EVA = 15 // the Penning trap "family" format, used at ISOLTRAP, SHIPTRAP...
};

class Datapoint
{
public:
	double x_value;
	double y_value;
	
	double x_value_err;
	double y_value_err;

	Datapoint(double ix=0, double iy=0, double ix_err=0, double iy_err=0)
	{
		x_value = ix;
		y_value = iy;
		x_value_err = ix_err;
		y_value_err = iy_err;
	}

	Datapoint(const Datapoint & from_this)
	{
		x_value = from_this.x_value;
		y_value = from_this.y_value;
		x_value_err = from_this.x_value_err;
		y_value_err = from_this.y_value_err;
	}
	bool operator< (const Datapoint &a)
	{
		if (x_value < a.x_value)
			return true;
		return false;
	}
	void pretty_print(std::ostream & stream_here)
	{
		stream_here << "x:" << x_value << "|y:" << y_value << "|\n";
	}
};
/*
struct DATA{
    double data_x,data_y,data_y_err,data_x_err;

    DATA() { memset(this, 0, sizeof (*this)); }
};*/

std::string asString	(int number);
std::string asString	(unsigned int number);
std::string asString	(double number, int style=0);
double	w		(double delta);
int 	as_int		(const double &numb);
double	as_double	(const std::string &s);
bool	as_bool		(const std::string &s);
int	as_int		(const std::string &s);
std::string as_0f	(double numb);
std::string as_1f	(double numb);
std::string as_3f	(double numb);
std::string as_2f	(double numb);
double get_frequency_from_string (const std::string &stringi);
double get_phase	(double frequency,double angle);

void siivoa_valilyonnit_pois(std::string & stringi);

void open_file_for_reading(const boost::filesystem::path &path, boost::filesystem::ifstream &ifstreami);
void open_file_for_writing(const boost::filesystem::path &path, boost::filesystem::ofstream &ofstreami);
void open_file_for_appending(const boost::filesystem::path &path, boost::filesystem::ofstream &ofstreami);

void get_filenames( const boost::filesystem::path & directory, const std::string & file_extension, std::list<std::string> & matching_files);

void open_local_or_rc_file(boost::filesystem::ifstream & ifs,
			   const Glib::ustring & filename,
			   std::ios_base::openmode mode = std::ios_base::in);
void open_local_or_rc_file(boost::filesystem::ofstream & ofs,
			   const Glib::ustring & filename,
			   std::ios_base::openmode mode = std::ios_base::out);

Glib::ustring local_or_rc_file(const Glib::ustring & filename);
boost::filesystem::path get_rc_dir(void);
boost::filesystem::path get_share_dir(void);
bool init_rc_dir(void);

std::string format_time_YYMMDD_hhmmss(boost::posix_time::ptime iTime);

std::string double_backslashify(const string &string_to_add_double_backslashes);

#endif

