/***************************************************************************
 *            mass_scan_settings.cc
 *
 *  Sun Jun  4 13:59:18 2006
 *  Copyright  2006  User
 *  Email
 ****************************************************************************/

#include "mass_scan_settings.hh"
#include "mjonotpp.hh"

#include <fstream>
#include <iostream>

#include <boost/foreach.hpp>

using namespace std;

bool cMassScanSettings::load_settingfile(string filename) {
	delete_old();
	ifstream file;
	file.open(filename.c_str() );
	if (!file) {
		cout << "Could not open settingfile: " << filename << "\n";
		return 0;
	}
	string row,piece;
	bool found_sequence(false);
	while (getline(file,row)) { //Search for PPG sequency
		if (row[0] == '#')
			continue;
		if (row != "[ppg sequence]")
			continue;
		else {
			//cout << "found PPG times\n";
			found_sequence=true;
			break;
		}
	}
	if (!found_sequence) {
		cout << "ppg-pattern not found!\n";
		return 0;
	}
	//Get channel information
	while (getline(file,row)) {
		PPG_CHANNEL toAdd;
		if (row[0] == '[')
			break;
		if (row[0] == '#')
			continue;
		piece = erota(row,' '); //'channel'
		if (piece != "channel")			continue;
		piece = erota(row,','); //'16'
		luvuksi(piece,toAdd.channel); //cout << "channel: " << ch << "\n";
		piece = erota(row,' '); // empty
		piece = erota(row,' '); // 'enabled'
		piece = erota(row,','); //'1'
		if (piece == "1")
			toAdd.enabled = 1;
		else
			toAdd.enabled = 0;
		piece = erota(row,' '); // empty
		piece = erota(row,' '); // 'start'
		piece = erota(row,','); // '620000.0'
		luvuksi(piece,toAdd.start); // cout << "start: " << start << "\n";
		piece = erota(row,' '); // empty
		piece = erota(row,' '); // 'stop'
		piece = erota(row,','); // '820000.0'
		luvuksi(piece,toAdd.stop); // cout << "stop: " << stop << "\n";
		piece = erota(row,' '); // empty
		piece = erota(row,' '); // 'clear'
		piece = erota(row,','); // '1'
		if (piece == "1")
			toAdd.clear_enabled = 1;
		else
			toAdd.clear_enabled = 0;
		toAdd.what_is = get_ppg_channel_explanation(toAdd.channel);
		ppg_channels.push_back(toAdd);
//		cout << "what: " << toAdd.what_is << " start,stop " << toAdd.start << toAdd.stop << " ch: " << toAdd.channel << "\n";
	}
	//Getting RF generators
	found_sequence=false;
	while (getline(file,row)) { //Search for RF generators
		if (row[0] == '#')
			continue;
		if (row != "[trap rf]")
			continue;
		else {
//			cout << "found RFs\n";
			found_sequence=true;
			break;
		}
	}
	if (!found_sequence) {
		cout << "RFs not found!\n";
		return 0;
	}
	int generator_number(1);
	while (getline(file,row)) {
		RF_GENERATOR toAdd;
		if (row[0] == '[')
			break;
		if (row[0] == '#')
			continue;
		piece = erota(row,' '); //'frequency'
		if (piece != "frequency")			continue;
		piece = erota(row,','); //'1700.0'
		luvuksi(piece,toAdd.freq);
		piece = erota(row,' '); // empty
		piece = erota(row,' '); // 'amplitude'
		piece = erota(row,','); //'0.600'
		luvuksi(piece,toAdd.amp);
		piece = erota(row,' '); // empty
		piece = erota(row,' '); // 'trigger'
		piece = erota(row,' '); // 'mode'
		piece = erota(row,','); // '1'
		luvuksi(piece,toAdd.mode);
		piece = erota(row,' '); // empty
		piece = erota(row,' '); // 'burst'
		piece = erota(row,' '); // 'count'
		piece = erota(row,','); // '1'
		luvuksi(piece,toAdd.burst_count);
		piece = erota(row,' '); // empty
		piece = erota(row,' '); // 'phase'
		piece = erota(row,','); // '0.000'
		luvuksi(piece,toAdd.phase);
		toAdd.rf_number = generator_number;
		toAdd.name = get_rf_explanation(toAdd.rf_number);
		rf_generators.push_back(toAdd); generator_number++;
//		cout << toAdd.freq << toAdd.amp << toAdd.mode << toAdd.burst_count << toAdd.phase << toAdd.name << "\n";
	}
	file.close();
	return 1;
}
//------------------------------------------------------------------------------
void cMassScanSettings::delete_old(void) {
	ppg_channels.clear();
	rf_generators.clear();
	return;
}
//------------------------------------------------------------------------------
void cMassScanSettings::make_ppg_channel_vs_explanation(void) {
	ppg_explanations.clear();
	EXPLAIN toAdd;
	toAdd.ch = 16; toAdd.what = "Beam gate"; ppg_explanations.push_back(toAdd);
	toAdd.ch = 10; toAdd.what = "RFQ extraction"; ppg_explanations.push_back(toAdd);
	toAdd.ch = 5; toAdd.what = "Trap1 injection"; ppg_explanations.push_back(toAdd);
	toAdd.ch = 3; toAdd.what = "Trap1 magnetron"; ppg_explanations.push_back(toAdd);
	toAdd.ch = 4; toAdd.what = "Trap1 cyclotron"; ppg_explanations.push_back(toAdd);
	toAdd.ch = 2; toAdd.what = "Trap1 extraction"; ppg_explanations.push_back(toAdd);
	toAdd.ch = 6; toAdd.what = "Trap2 Injection"; ppg_explanations.push_back(toAdd);
	toAdd.ch = 7; toAdd.what = "Trap2 magnetron"; ppg_explanations.push_back(toAdd);
	toAdd.ch = 8; toAdd.what = "Trap2 cyclotron"; ppg_explanations.push_back(toAdd);
	toAdd.ch = 1; toAdd.what = "Trap2 extraction"; ppg_explanations.push_back(toAdd);
	toAdd.ch = 9; toAdd.what = "MCS trigger"; ppg_explanations.push_back(toAdd);
}
//------------------------------------------------------------------------------
void cMassScanSettings::make_rf_generators_vs_explanation(void) {
	rf_explanations.clear();
	EXPLAIN toAdd;
	toAdd.ch = 1; toAdd.what = "Trap1 magnetron"; rf_explanations.push_back(toAdd);
	toAdd.ch = 2; toAdd.what = "Trap1 cyclotron"; rf_explanations.push_back(toAdd);
	toAdd.ch = 3; toAdd.what = "Trap2 magnetron"; rf_explanations.push_back(toAdd);
	toAdd.ch = 4; toAdd.what = "Trap2 cyclotron"; rf_explanations.push_back(toAdd);
	toAdd.ch = 5; toAdd.what = "Gaussian"; rf_explanations.push_back(toAdd);
	toAdd.ch = 6; toAdd.what = "Cleaning"; rf_explanations.push_back(toAdd);
}
//------------------------------------------------------------------------------
std::string cMassScanSettings::get_ppg_channel_explanation(const int &ch) {
	if (ppg_explanations.empty())
		return "No PPG channel explanations available..";
	for (vector<EXPLAIN>::iterator i= ppg_explanations.begin(); i !=ppg_explanations.end(); ++i ) {
//		cout << "verrokki: " << ch << " tahan: " <<i->ch << "\n";
		if ( i->ch == ch)
			return i->what;
	}
	return "Unknown PPG";
}
//------------------------------------------------------------------------------
std::string cMassScanSettings::get_rf_explanation(const int &rf_no) {
	if (rf_explanations.empty())
		return "No RF explanations available..";
	for (vector<EXPLAIN>::iterator i= rf_explanations.begin(); i !=rf_explanations.end(); ++i ) {
//		cout << "verrokki: " << ch << " tahan: " <<i->ch << "\n";
		if ( i->ch == rf_no)
			return i->what;
	}
	return "Unknown RF";

}
//------------------------------------------------------------------------------
double cMassScanSettings::get_duration(string ppg_name) {
	if (ppg_channels.empty()) {
		cout << "No scan settings available! Too old scan you having?\n";
		return -1;
	}
	for (vector<PPG_CHANNEL>::iterator i=ppg_channels.begin(); i !=ppg_channels.end(); ++i) {
//		cout << "verrokki: " << ppg_name << " tahan: " << i->what_is << "\n";
		if (ppg_name == i->what_is )
			return (i->stop - i->start)/1.0e6;
	}
	return -1;
}
//------------------------------------------------------------------------------
double cMassScanSettings::get_frequency(string rf_name) {

	return -1;
}
//------------------------------------------------------------------------------
double cMassScanSettings::get_amplitude(string rf_name) {
	if (ppg_channels.empty()) {
		cout << "No scan settings available! Too old scan you having?\n";
		return -1;
	}
	for (vector<RF_GENERATOR>::iterator i=rf_generators.begin(); i !=rf_generators.end(); ++i) {
//		cout << "verrokki: " << ppg_name << " tahan: " << i->what_is << "\n";
		if (rf_name == i->name )
			return i->amp*1.0e3;
	}
	return -1;
}

//------------------------------------------------------------------------------
bool cMassScanSettings::load_json_settingfile(const string& filename)
{
	boost::property_tree::ptree root;
	
	boost::property_tree::read_json("/home/tomero/data_in/2014_05_27_yields_U/A125_A123/m123_1/settings.json", root);
	
	BOOST_FOREACH(const boost::property_tree::ptree::value_type& child, root.get_child(""))
  { cout << child.first << endl; }
}

//------------------------------------------------------------------------------
