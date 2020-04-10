#ifndef __eva_loader_hh
#define __eva_loader_hh

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/progress.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <list>
#include "unit.hh"
#include "loctm.hh"

class cEvaFile;

/// Ion details like A, q and element_name
class cIonDetails
{

public:
	int mass_number;
	int charge_state;
	std::string element;

	cIonDetails();
	~cIonDetails();

	bool operator==(const cIonDetails & rhs);
};

class cSimilarScans : public cIonDetails
{
public:
	std::list<cEvaFile *> similar_evafiles;
	int number_of_similar_scans;

	// Constructors set the number_of_similar_scans to 1.
    cSimilarScans(void){number_of_similar_scans = 1;}
    cSimilarScans(const cIonDetails & ion_details);
};

class cMCSDetails
{
protected:
	double bin_width;
	int MAX_MCS_channels;
	double MCS_recording_offset_in_bins;

public:
	cMCSDetails();
	~cMCSDetails();

	double get_bin_width(void){return bin_width;}
	int get_MAX_MCS_channels(void){return MAX_MCS_channels;}
	int get_MCS_recording_offset_in_bins(void){return MCS_recording_offset_in_bins;}
};

class cEvaFile : public cIonDetails, public cMCSDetails
{
protected:

	std::string smart_parse(const std::string &line, const std::string & identifier_str, const std::string & valid_ending_characters = " ,\n");


public:
	std::list<cUnit> bunches;

	std::string start_time_of_sweep, end_time_of_sweep; // Format "2012-10-25 22:33:44"
	boost::filesystem::path filename;
	int scan_rounds;

	std::string extra_info_string_1;
	std::string extra_info_string_2;

	double extra_info_double_1;
	double extra_info_double_2;

	cEvaFile();
	~cEvaFile();

	bool operator<(const cEvaFile &rhs) {return start_time_of_sweep < rhs.start_time_of_sweep;}
	bool process_evafile(const std::string& filename);
	cIonDetails get_ion_details();

};

class cEvaFileCollection
{
public:
	std::list<cEvaFile> evafiles;
	boost::filesystem::path directory;

	bool add_evafile(const std::string& filename);

	std::list<cSimilarScans> get_ion_species_types(void);

	cEvaFileCollection();
	~cEvaFileCollection();
};

/// This is a line in a dialog with radiobutton. This line is " (*) Gd154, 1+, 7 files"
class choise
{
public:
	Gtk::RadioButton * radiobutton;

	choise();
	~choise();
	choise(const choise & other);
	void operator=(const choise & other);

	cSimilarScans *ss;


	choise(cSimilarScans *p, Gtk::RadioButtonGroup grp);
};

#endif
