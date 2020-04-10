/***************************************************************************
 *            mass_scan_settings.hh
 *
 *	- 	Structure to get useful data from scan_settings.dat
 *		saved with the scan
 *	-	Created 2006-06-04 by Tommi Eronen
 ****************************************************************************/

#ifndef _MASS_SCAN_SETTINGS_HH
#define _MASS_SCAN_SETTINGS_HH

#include <vector>
#include <iostream>

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"

struct PPG_CHANNEL{
	bool enabled,clear_enabled;
	int channel;
	double start, stop;
	std::string what_is;
};

struct RF_GENERATOR{
	int rf_number;
	std::string name;
	double freq,amp, phase;
	int mode;
	int burst_count;
};
struct EXPLAIN{
	int ch;
	std::string what;
};


class cMassScanSettings {
	private:
		std::vector<EXPLAIN> ppg_explanations;
		std::vector<EXPLAIN> rf_explanations;
		std::vector<PPG_CHANNEL> ppg_channels;
		std::vector<RF_GENERATOR> rf_generators;
	public:
		cMassScanSettings(){
			make_ppg_channel_vs_explanation();
			make_rf_generators_vs_explanation();
//			cout << "ConstrucT!!!\n";
		}
		~cMassScanSettings() {}
		bool load_settingfile(std::string filename);
		bool load_json_settingfile(const std::string & filename);
		void delete_old(void);
		void make_ppg_channel_vs_explanation(void);
		void make_rf_generators_vs_explanation(void);
		std::string get_ppg_channel_explanation(const int &ch);
		std::string get_rf_explanation(const int &rf_no);
		//Getting PPG info
		double get_duration(std::string ppg_name);
		double get_start(std::string ppg_name);
		double get_stop(std::string ppg_name);
		//Getting RF info
		double get_amplitude(std::string rf_name);
		double get_frequency(std::string rf_name);
};


#endif
