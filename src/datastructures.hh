#ifndef __datastructures_hh
#define __datastructures_hh

/// Introducing new classes for:
///

/// DATASTRUCTURES TO HOLD THE DATA
/// 1) The whole thing
/// 2) Frequency
/// 3) Bunch
/// 4) Detected ions in a bin

/// FILTERING, eg. bin window, count window, frequency window, class division

#define MAXBINS 20000
#define MAX_IONS_PER_BUNCH 40000

#include <list>
#include <vector>
#include <boost/date_time/posix_time/ptime.hpp>

class Bunch;

enum DATAFILEFORMAT{
	V3 = 3, // This is LabView version saved since something like 2007
	V4 = 4  // The new simultaneous-two-species scanned data files
};

/// FILTERING

/// Universal filter. Contains bin, counts and frequency window.
class Filter
{
private:
	unsigned int counts_min, counts_max;
	unsigned int bin_min, bin_max;
	unsigned long frequency_mHz_min, frequency_mHz_max;
public:
	Filter();

	/// Set methods

	void set_bin_window(unsigned int min, unsigned int max);
	void set_count_window(unsigned int min, unsigned int max);
	void set_frequency_window(unsigned long min, unsigned long max);

	unsigned int get_bin_min(void) const {return bin_min;}
	unsigned int get_bin_max(void) const {return bin_max;}
	unsigned long get_frequency_mHz_min(void) const {return frequency_mHz_min;}
	unsigned long get_frequency_mHz_max(void) const {return frequency_mHz_max;}
	unsigned int get_counts_min(void) const {return counts_min;}
	unsigned int get_counts_max(void) const {return counts_max;}

	bool operator == (const Filter & rhs); /// Returns false if any of the window boundaries are different
};

/// DATA STRUCTURES

/// The most basic container. Given the bin where ion was seen and number of ions.
class IonsInBin
{
public:
	unsigned int bin;
	unsigned int counts;
	IonsInBin(unsigned int ibin, unsigned int icounts){bin = ibin; counts = icounts;}
};


/// An ion bunch. Contains several IonsInBin. This contains all ions that were
/// detected in a single bunch.
class Bunch
{
private:
	std::list<IonsInBin> ions_in_bins;
	boost::posix_time::ptime timestamp;

	/// Internally stored stuff that get updated when needed.
	/// Existence of these should help to make the class faster.
	unsigned int all_counts;

public:
	Bunch(){all_counts = 0;}
	/// Add an "ions in bin"
	void add_ions(const IonsInBin & to_add);

	void set_bunch_time(const boost::posix_time::ptime &time_of_bunch){timestamp = time_of_bunch;}

	/// Returns number of counts within a specific bin interval. Use get_all_counts()
	/// to get all counts.
	unsigned int get_counts(unsigned int startbin, unsigned int stopbin);
	/// Get all counts, regardless of any filter setting
	unsigned int get_all_counts(void);

	/// Returns TOF (in bins), using only the ions within the given bin interval.
	   double get_tof(unsigned int startbin = 0, unsigned int stopbin = 20000);

	/// Operators for sorting with respect to time
	bool operator < (const Bunch & rhs) { return timestamp < rhs.timestamp;}
	bool operator == (const Bunch & rhs) { return timestamp == rhs.timestamp;}
};

class Frequency
{
private:
	Filter * filter;
	std::list<Bunch> bunches;
	long int frequency_mHz;

	//std::vector<CountrateClass> * countrate_classes;

	/// Flag variable in case something has changed and a recalculation is triggered
	bool processing_needed;

	/// In case
	void process_any_and_all_changes_stuff(void);
public:
	Frequency(unsigned int ini_frequency_mHz, Filter * filter);

	/// Add a new bunch
	void add_bunch(const Bunch & bunch_to_add);

	/// Set a filter.
	void set_filter(Filter* ifilter);

	unsigned int get_counts();
	unsigned int get_all_counts();
	double get_tof();
	double get_tof_standard_deviation(void);
	double get_tof_standard_deviation_of_mean(void);
	long unsigned int get_frequency() const;

	void get_count_distribution(std::list< IonsInBin >& distribution, const Filter& filter);

};

class Frequencies
{
private:
	Filter filter;
	std::list<Frequency> frequencies;

public:
	Frequencies();

	void add_bunch(const Bunch & bunch_to_add, unsigned int bunch_frequency_mHz);

	void set_filter( const Filter& ifilter);

	void get_all_counts_as_function_of_tof(std::list<IonsInBin> & distribution);
	void get_counts_as_function_of_tof(std::list<IonsInBin> & distribution);
	void get_tof_matrix(std::list<IonsInBin> & distribution);

};

#endif