// unit.hh
// Consists of one frequency. Has pointer table of bin vs counts
#ifndef _UNIT_HH
#include <string>
#include <iterator>
#include <vector>
#include "datap.hh"
#include "plotstructures.hh"

#define _UNIT_HH
#define MAXDATAPOINTS 500
#define INCREASE_SIZE 100
#define SMALLEST_BIN 0
#define BIGGEST_BIN 20000

class cUnit{
 private:
  int datapointnumber;
  double average;
  double sdom;
  double frequency;

  double MCS_TOF_offset_in_bins;

  int totalcounts;
	int bunchnumber;
// 	int emptybunchnumber;
  cDatapoint **datapoints;
  int maxdatapoints;
  std::string time_str; //Time as string. Note that every datapoint have time there!
 public:
  bool is_active;
  cUnit(int imaxdatapoints = MAXDATAPOINTS){
    datapoints = new(std::nothrow) cDatapoint *[imaxdatapoints];
    average=0;
    sdom=0;
    totalcounts=0;
    datapointnumber=0;              //How many datapoints are in use
	bunchnumber=0;				//How many bunches there are
// 	emptybunchnumber=0;
    maxdatapoints=imaxdatapoints;
    frequency = 0;
	MCS_TOF_offset_in_bins = 0;
  }
  cUnit(const cUnit &uniitti)      //Copy constructor
  {
	is_active = true;
	datapoints = new(std::nothrow) cDatapoint *[uniitti.maxdatapoints];
//		datapoints = new(nothrow) cDatapoint *[uniitti.datapointnumber];
//	Change in future to this; table increment is needed too
    maxdatapoints = uniitti.maxdatapoints;
    frequency = uniitti.frequency;
    sdom = uniitti.sdom;
	time_str = uniitti.time_str;
    totalcounts = uniitti.totalcounts;
	bunchnumber = uniitti.bunchnumber;
// 	emptybunchnumber = uniitti.emptybunchnumber;
    datapointnumber = 0;

	MCS_TOF_offset_in_bins = uniitti.MCS_TOF_offset_in_bins;

    for (int i=0; i < uniitti.datapointnumber; i++){
      InsertDataPoint(*uniitti.datapoints[i]);
    }
  }
	~cUnit(void){
		for (int i=0; i < datapointnumber; i++){
			delete datapoints[i];
		}
		datapointnumber = 0;
		delete [] datapoints;
	}
	int increaseDatapointTableSize(int size_increment=INCREASE_SIZE);
	std::string read_file(const std::string &filename, const char delimiter, const int file_no); //OLD style data dir reading
	std::string setDataRow(const std::string &line, const char delimiter);
	std::string InsertDataPoint(const cDatapoint &toAdd);
	std::string addNewBunch(const cUnit &tobeadded, std::ostream &log); //Only this is used to add different bunch!
	std::string addOneEmptyBunch(const std::string time_string, std::ostream &log=std::cout);
	std::string InsertDataPoint(int bin, int counts, const int file_no, std::string time_string ="");
	int getCounts(void);
	int getCounts(int startbin, int stopbin);
	void putFrequency(double freq){
		frequency = freq;
	}
	double getFrequency(void) const{
		return frequency;
	}
	int searchForBinMatch(int search_bin); //returns datapointnumber if found, -1 otherwise
	//int addDataToExisting(const cUnit &tobeadded, std::ostream &log);
	int print_contents_of_this_class(std::ostream &in_here);
	std::string outputEachFileOfFrequency(std::ostream &outfile,int startbin,int stopbin);
	int getTotalBunchNumber(void);
//	int getTotalEventNumber(int startbin, int stopbin); //Not needed
	int getEventNumber(int startbin, int stopbin, int counts_wanted);
	void get_events(list< int >& to_here);
	int getCounts(int startbin, int stopbin, int min_cts, int max_cts,bool use_crap_window=true);
	double getTimeOfFlight(int startbin, int stopbin, int min_cts, int max_cts, bool use_crap_window=true);
	double getTimeOfFlightError(int startbin, int stopbin, int min_cts, int max_cts,bool use_crap_window=true);

	//2007-03-13 Adding the possibility for crap-window..
	int getCounts(int outer_startbin, int outer_stopbin, int inner_startbin, int inner_stopbin,
	              int min_cts, int max_cts);
	//2007-04-19 Adding the possibility for crap-window..
	double getTimeOfFlight(int inner_startbin, int inner_stopbin, int min_cts, int max_cts,
								int outer_startbin, int outer_stopbin);
	double getTimeOfFlightError(int inner_startbin, int inner_stopbin, int min_cts, int max_cts,
								int outer_startbin, int outer_stopbin);
	//2007-04-19 Method to check that outer_start < inner_start < inner_stop < outer_stop
	bool are_binnings_valid(int in_start,int in_stop,int out_start, int out_stop);

	int get_maximum_counts_in_bunch(void); // Returns number of ions of a bunch having most counts 9.8.2006
	void increaseBunchNumber(void){
		bunchnumber++;
	}
// 	void increaseEmptyBunchNumber(void){
// 		emptybunchnumber++;
// 	}
	void getHistogram(int counts_in_bin[], int max_bins);
	void getHistogram(std::vector<int> &in_here); // 9.8.2006
	void get_histogram(std::vector<Datapoint> * to_here);
	void putTimeString(std::string time_string){time_str = time_string;}
	const std::string getTimeString(void){return time_str;}
	int getCountsOfABunch(int bunch_no);
	//2007-08-03 appends to count histogram table.
	void append_to_cts_vs_bin_histogram(std::vector<int> & counts,int inner_startbin,
								int inner_stopbin, int min_cts, int max_cts, bool correct_with_mean = false,
								int outer_startbin = SMALLEST_BIN, int outer_stopbin = BIGGEST_BIN);
	void get_histogram_of_class(std::vector<Datapoint> * to_here,int inner_startbin,
								int inner_stopbin, int min_cts, int max_cts, bool correct_with_mean = false,
								int outer_startbin = SMALLEST_BIN, int outer_stopbin = BIGGEST_BIN);
	//Next method not implemented yet..
	//2008-01-02 To set exclusing flags to bunches; returns '1' if bunch was found.
	int set_bunch_not_active(int bunch_number);
	void set_all_bunches_active();
	//2008-01-03 Method to get empty bunches, not just those marked 'do not use'
	int get_number_of_really_empty_bunches();

	//2008-01-03 for histograming purposes
	int getCounts(int startbin, int stopbin, int bunch_number, int min_allowed_countss, int max_allowed_countsss);
	int getGetEventNumberAsTable(int ions_per_shot[], int events[], int start_index, int &stop_index, int max_index);
	void set_active(bool state);
	int set_unit_time_for_ions_if_not_set();
	void remove_datas();
	void set_MCS_TOF_offset_in_bins(double offset=0){MCS_TOF_offset_in_bins = offset;}
	double get_MCS_TOF_offset_in_bins(void){return MCS_TOF_offset_in_bins;}
	void shift_frequency(double amount);
};
//------------------------------------------------------------------------------
inline int cUnit::getCounts(int startbin, int stopbin, int bunch_number, int min_allowed_countss, int max_allowed_countss) {
	int inner(0),outer(0);
	//Checking first how many arrived in total without bin or count gating
	for (int i=0; i<datapointnumber; i++) {
		if (datapoints[i]->getBunch_no() != bunch_number)
			continue;
		outer += datapoints[i]->getCounts();
	}
	//If everything is desired, returning it
	if (stopbin == 0 && startbin == 0 && min_allowed_countss == 0 && max_allowed_countss == 0)
		return outer;

	//Making proper windowing if 0 used
	if (stopbin == 0)
		stopbin = 1000000;
	if (max_allowed_countss == 0)
		max_allowed_countss = 1000000;
	//If not falling to the count-window..
	//cout << "outer " << outer << "\n";
	if ( outer < min_allowed_countss || outer > max_allowed_countss) {
		//cout << "min_ctss, max_ctss " << min_allowed_countss << " " << max_allowed_countss << " ei osunut\n";
		return 0;
	}
	for (int i=0; i<datapointnumber; i++) {
		if (datapoints[i]->getBunch_no() != bunch_number)
			continue;
		if (datapoints[i]->getBin() >= startbin && datapoints[i]->getBin() <= stopbin) {
			inner += datapoints[i]->getCounts();
		}
	}
	//cout << "inneri: " << inner << "\n";
	return inner;
}

#endif
