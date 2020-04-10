//
#include <fstream>
#include <cmath>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <vector>
#include <list>
#include <gtkmm/progressbar.h>

#define EPSILON 0.0002
// #define PII 3.141592653589793

#include "plotstructures.hh"
#include "collection.hh"
#include "mjonotpp.hh"
#include "loctm.hh"
#include "guiutil.hh"

#include "eva_loader.hh"

using namespace std;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int cFrequencies::increaseFrequencyTableSize(int size_increment) {
	frequencies.resize(frequencies.size() + size_increment);
	maxfrequencies += size_increment;
//	cout << "increased sizes :P\n";
	return 1;
}
//------------------------------------------------------------------------------
int cFrequencies::set_frequency_range(double min, double max) {
	int freqs_outside(0);
	if ( (min < 0.1 ) && (max < 0.1) )
		for (int i=0; i<frequencynumber;i++) {
			frequencies[i]->set_active(true);
		}
	else {
		if (max < 0.1)
			max = 1e15;
		for (int i=0; i<frequencynumber;i++) {
			if ( (frequencies[i]->getFrequency() < min) || (frequencies[i]->getFrequency() > max)) {
				frequencies[i]->set_active(false);
				freqs_outside++;
			}
			else
				frequencies[i]->set_active(true);
		}
	}
/*	for (int i=0; i<frequencynumber;i++) {
			cout << frequencies[i]->getFrequency() << " is active:" << frequencies[i]->is_active << "\n";
	}*/
	return freqs_outside;
}

//------------------------------------------------------------------------------
vector<int> cFrequencies::getClassUpperBoundary(void) {
	vector<int> boundaries;
	if (number_of_classes < 1)
		return boundaries;
	for (int i=0; i<number_of_classes;i++) {
		boundaries.push_back(class_max_cts[i]);
	}
	return boundaries;
}
//------------------------------------------------------------------------------
bool cFrequencies::getLinearFit(DATASET &fitpoints, double range_min, double range_max, int how_many_points){
	if (!is_countrate_effect_fitted || how_many_points < 1) { //if not eligible to get it
		return false;
	}
//	cout << "range min max " << range_min << " " << range_max << "\n";
	fitpoints.data.clear();
	if (range_min >= range_max) //If range is strange returning false
		return false;
	Datapoint toAdd; //slope, slope_err, constant, constant_err;
	for (double x=range_min; x <= range_max+EPSILON ; x += (range_max - range_min)/(how_many_points*1.0) ){
		toAdd.x_value = x;
		//2007-01-18
		//toAdd.y_value = constant + slope*x;
		toAdd.y_value = getInterpolatedCountrateEffectValue(x);

//		toAdd.y_value_err = sqrt ( pow(slope_err *(getWeightedAverageCountsInClasses(cout)-x) ,2.0) + pow (constant_err ,2.0)   );
		//Changed to have new method of getting error, 2007-01-16
		toAdd.y_value_err = getInterpolatedCountrateEffectValueError(x);
//		cout << "x,y,y_err " << toAdd.x_value << " " << toAdd.y_value << " " << toAdd.y_value_err << "\n";
		fitpoints.data.push_back(toAdd);
	}
//	cout << getAverageCountsInClasses(cout) << "=aveCountsInClasses\n";
	return true;
}
//------------------------------------------------------------------------------
void cFrequencies::resetData() {
	bin_width = -1;
	max_bin_number=-1;
	number_of_bunches_in_one_spectrum=-1;


}
//------------------------------------------------------------------------------
int cFrequencies::getNumberOfRoundsInFile(
			 const boost::filesystem::path bunchfilename, bool force, ostream& log)
{
	if (is_this_v3_bunchfile(bunchfilename.string())) {  //That is the new one (may 2005-> )
		//cout << "\n nneeen here\n";
//		cout << dirname << filename_or_suffix << "joo\n";
		return getNumberOfRoundsInFile_v3(bunchfilename, force, log);
	}
	//if (data_file_format == 2) {  //feb 2009->
	if (is_this_v4_bunchfile(bunchfilename)) { //New, since february 2009
		return getNumberOfRoundsInFile_v4(bunchfilename, force, log);
	}

	return -1;
}
//------------------------------------------------------------------------------
string cFrequencies::readDirectory(const boost::filesystem::path bunchfilename,
			int start_round, int end_round, ostream &log)
{
	//std::string dirname = bunchfilename.parent_path().string();
	//std::string filename_or_suffix = bunchfilename.filename().string();
	if (data_file_format == DATAFORMAT_EVA)
	{
		std::cout << "This is wrong method... Eva files are treated specially\n";
		// Use method   show_eva_data(...)  instead.
	}




	load_bar->set_fraction(0.0);
	//if (data_file_format == 0) {  //That is the new one (may 2005-> )
	if (is_this_v3_bunchfile(bunchfilename)) {  //That is the new one (may 2005-> )
		if (start_round == 0 && end_round == 0)
			return readDirectory_v3(bunchfilename,log);
		else
			return readDirectoryPartly_v3(bunchfilename,start_round,end_round,log);
	}
	if (data_file_format == 1) {  //OLd and original
		if (start_round == 0 && end_round == 0)
//			return readDirectory_v1("",filename_or_suffix, dirname+"/",log);
			return readDirectory_v1("",".dat", bunchfilename.parent_path().string()+"/",log);
		else {
			log << "Partial reading of old format not implemented yet...\n";
			return "Not supported yet!\n";
		}
	}
	//if (data_file_format == 2) { //New, since february 2009
	if (is_this_v4_bunchfile(bunchfilename)) { //New, since february 2009
		if (start_round == 0 && end_round == 0)
			return readDirectory_v4(bunchfilename,log);
		else
			return readDirectoryPartly_v4(bunchfilename,start_round,end_round,log);
	}


	return "No reading done.. strange\n";
}
//------------------------------------------------------------------------------
string cFrequencies::readDirectory(const boost::filesystem::path bunchfilename,
                           const vector<int> &round_skip_list, ostream & log) {
	if (data_file_format == 1) {
		return "Only supported for new datafileformat!\n";
	}
	//Assuming that the new file format is in use.
	if (round_skip_list.empty() )
		return readDirectory_v3(bunchfilename,log);

	//TODO

	return "Not valid yet!!\n";

}
//------------------------------------------------------------------------------
string cFrequencies::readDirectory_v1(const string &prefix,
                const string &suffix, const string &dirname, ostream &log,
                const int startfileno, const int endfileno)
{
	bool fetched_things=false;
	setAllDataNotFitted();
	string filename,iinstring;
	boost::filesystem::ifstream thisisopen;
	char temp[20];
	int i(startfileno), missing(0),skipped(0),files_opened=0;
									// 'i' is the file number
  do{
	sprintf(temp,"%d",i);
    //    cout << dirname;
    filename=dirname+prefix+temp+suffix;
    thisisopen.clear();
    thisisopen.open(filename.c_str() );
    if (thisisopen.fail() ){  //if file could not be opened
      log << "could not read " << filename << "\n";
      missing++;                        //it is reported missing
      if (missing > 4){                 //if too many, breaking
        log << "Guess, that all files have been read... \n";
		missing=0;
        break;
      }
      i++;
      continue;
    }
    else{
      if (missing != 0){
        skipped=skipped+missing;
        missing = 0;
      }
      files_opened++;
    }
	/////////////////////
	string row, erotettu, time_string;
	if (!fetched_things) {
		while ( getline(thisisopen,row)) {     												//Fetch frequency from datafile
			if (row[0] != '#') break;           //If not a comment, skipping frequency hunt
			do{                                 //Search pattern "Frequency: "
				erotettu = erota(row,' ',0);
				if (erotettu == "File"){		//Getting record time of bunch #1
					erotettu = erota(row,' ',0); //saved
					erotettu = erota(row,' ',0); //at
					erotettu = erota(row,'\r',0); //rest all the way to carriage return..
					time_string = erotettu;			 // Rest of it 2005-02-32 16:40:40
					start_time = erotettu;
					end_time = erotettu;
					break;
				}
				if (erotettu == "width:"){ //Search for bin width
					erotettu = erota(row,' ',0);
					sscanf(erotettu.c_str(),"%lf",&bin_width);
					bin_width *= 1e6;
					break;
				}
				if (erotettu == "record:"){ //Search for bins per record ie. 2048
					erotettu = erota(row,' ',0);
					sscanf(erotettu.c_str(),"%d",&max_bin_number);
					break;
				}
				if (erotettu == "Records"){ //Search for records per scan: ie. 1 (<->single bunch mode)
					erotettu = erota(row,' ',0);
					erotettu = erota(row,' ',0);
					erotettu = erota(row,' ',0);
					sscanf(erotettu.c_str(),"%d",&number_of_bunches_in_one_spectrum);
					break;
				}
			} while (erotettu != ""); //Breaking up line until it is empty
	//		if (new_freq != 0)
	//			break;
		};	//Finding freq until found
		fetched_things = true;
	}
	////////////////////////////////////////////77


    thisisopen.close();
    addFreqFromFile(filename,i,log);
    i++;
  } while (i<(endfileno+1));
//  } while (1);



  sprintf(temp,"%d",files_opened-(missing+skipped));
  string temp_string = temp;
  data_in_order = false;
  if (skipped >0)
    log << "Total of " << skipped << " files where missing.\n";
  return ("Total of " + temp_string + " files opened.\n");
}
//------------------------------------------------------------------------------
string cFrequencies::readDirectory_v2(const string &dirname,const string &filename, ostream &log){
	setAllDataNotFitted();
	boost::filesystem::ifstream openedfile;
	string row,erotettu;
	double new_freq(0);

	openedfile.open((dirname+filename).c_str() );
  if (!openedfile) return "File doesn't open/exist!\n (v2 reading of dir)\n";
  while( getline(openedfile,row)){ //Getting lines until end of file
	  new_freq=0;											//Resetting frequency
		do {     												//Fetch frequency from datafile
			if (row[0] != '#') break;           //If not a comment, skipping frequency hunt
			do{                                 //Search pattern "Frequency: "
				erotettu = erota(row,' ',0);
				if (erotettu == "Frequency:"){ //Searches for pattern "Frequency:"
					erotettu = erota(row,' ',0)+"E000"; //if found, next word is set as freq
					sscanf(erotettu.c_str(),"%lf",&new_freq);
					break;
				}
			} while (erotettu != ""); //Breaking up line until it is empty
			if (new_freq != 0)
				break;
		} while ( getline(openedfile,row));	//Finding freq until found
		getline(openedfile,row);
		while (row[0] == '#'){		//Skipping all the way to datapoints
			getline(openedfile,row);
		};

		cUnit tobeadded;
		tobeadded.putFrequency(new_freq);
		while ( row[0] != '\f'){
			tobeadded.setDataRow(row,'\t');
			getline(openedfile,row);
		}
		if ( searchForFrequencyMatch(new_freq) == -1 )
			addFrequencyUnit(tobeadded);
		else{
		 frequencies[searchForFrequencyMatch(new_freq)]->addNewBunch(tobeadded,log);
		}
	};
	openedfile.close();

	return "";
}
//------------------------------------------------------------------------------
string cFrequencies::readDirectory_v3(const boost::filesystem::path bunchfilename, ostream &log){
	if (!is_this_v3_bunchfile(bunchfilename))
		return "This is not v3 bunchfile:\n"+bunchfilename.string() +"\n";
	int row_count(0), n_th_bunch_after_freq_change(1);
	bool frequency_number_is_found(false);
	int running=1;
	int total_rounds = getNumberOfRoundsInFile(bunchfilename);
	setAllDataNotFitted();
	boost::filesystem::ifstream openedfile;
	string row,erotettu;
	double new_freq(0);
	double first_freq_of_round(0); //for progressbar

	if (!(open_file_for_reading_and_popup_message_if_error(bunchfilename.string(),openedfile))) {
		return "File " + bunchfilename.string() + " doesn't open/exist!\n(tried to read whole dir)\n";
	}
	new_freq=0;											//Resetting frequency
	log.precision(12);
// Part 1. Getting frequency from the very beginning header
// And adding rows corresponding to it
	string time_string;

	while ( getline(openedfile,row)) {     												//Fetch frequency from datafile
		row_count++;
		if (row[0] != '#') break;           //If not a comment, skipping frequency hunt
		do{                                 //Search pattern "Frequency: "
			erotettu = erota(row,' ',0);
			if (erotettu == "Frequency:"){ //Searches for pattern "Frequency:"
				erotettu = erota(row,' ',0);//if found, next word is set as freq

//				cout << erotettu << "\n";
//				string foo;
//				getline(cin, foo);
				new_freq = atof(erotettu.c_str());
				first_freq_of_round = new_freq;
//				sscanf(erotettu.c_str(),"%lf",&new_freq);
				break;
			}
			if (erotettu == "File"){		//Getting record time of bunch #1
				erotettu = erota(row,' ',0); //saved
				erotettu = erota(row,' ',0); //at
				erotettu = erota(row,'\r',0); //rest all the way to carriage return..
				time_string = erotettu;			 // Rest of it 2005-02-32 16:40:40
				start_time = erotettu;
				break;

			}
			if (erotettu == "width:"){ //Search for bin width
				erotettu = erota(row,' ',0);
				sscanf(erotettu.c_str(),"%lf",&bin_width);
				bin_width *= 1e6;
				break;
			}
			if (erotettu == "record:"){ //Search for bins per record ie. 2048
				erotettu = erota(row,' ',0);
				sscanf(erotettu.c_str(),"%d",&max_bin_number);
				break;
			}
			if (erotettu == "Records"){ //Search for records per scan: ie. 1 (<->single bunch mode)
				erotettu = erota(row,' ',0);
				erotettu = erota(row,' ',0);
				erotettu = erota(row,' ',0);
				sscanf(erotettu.c_str(),"%d",&number_of_bunches_in_one_spectrum);
				break;
			}
		} while (erotettu != ""); //Breaking up line until it is empty

//		if (new_freq != 0)
//			break;
	};	//Finding freq until found
//	row -string is the line having row[0] != '#'
	{
		cUnit tobeadded;
		tobeadded.putFrequency(new_freq);
		tobeadded.putTimeString(time_string);
		while ( row[0] != '\f'){
			tobeadded.setDataRow(row,'\t');
			if (!getline(openedfile,row))
				break;
			row_count++;
		}
		if (!do_not_load_first_bunch_after_freq_change) {
			if ( searchForFrequencyMatch(new_freq) == -1 ) //If it is not a new freq, making one new
				addFrequencyUnit(tobeadded);
			else {
				frequencies[searchForFrequencyMatch(new_freq)]->addNewBunch(tobeadded,log);
				frequency_number_is_found=true;
			}
		}
		else {
			cout << "This bunch was skipped: freq: " << tobeadded.getFrequency() << ", time: " << tobeadded.getTimeString() << "\n";
		}
	}
// Part 2. Adding all the rest bunches
	while ( getline(openedfile, row)){ //This is the data collection time - line
		row_count++;
//		std::cout << "rivi: " << row << endl;
		erotettu = erota(row,' ',0); // #
		erotettu = erota(row,' ',0); // Data
		erotettu = erota(row,' ',0); // Collection
		erotettu = erota(row,' ',0); // Time:
		erotettu = erota(row,'\r',0); // 2005-02-02 13:30:40
		time_string = erotettu;
//		log << "Bunch:" << row << "\n";
		if (!getline(openedfile, row)) //NEXT ROW AFTER COLLECTION TIME -line
			break;
		row_count++;
		if (row[0] == '#'){
			bool frequency_found = false;
			do{                                 //Search pattern "Frequency: "
				erotettu = erota(row,' ',0);
				if (erotettu == "Frequency:"){ //Searches for pattern "Frequency:"
					erotettu = erota(row,' ',0);//if found, next word is set as freq
					new_freq = atof(erotettu.c_str());//erotettu.c_str());
					if ( new_freq == first_freq_of_round ) {
						running++;
						load_bar->set_fraction( (running*1.0)/(total_rounds*1.0));
						update_gtk();
					}
					frequency_found = true;
					break;
				}
			} while (erotettu != ""); //Breaking up line until it is empty
			n_th_bunch_after_freq_change = 1;
			if (!frequency_found)
				return "Sumthin' wronk in da bunch file! row: " + asString(row_count) + "\n";
			//Since this row was #-starting, will read next row 2006-09-28
			if (!getline(openedfile,row))
				break;
			row_count++;
		}
		else {   //So freq did not change -- 2006-09-28
			n_th_bunch_after_freq_change++;
		}
		{
			cUnit tobeadded;
			tobeadded.putFrequency(new_freq);
			tobeadded.putTimeString(time_string);
			while ( row[0] != '\f'){
	//			cout <<"rowi: " <<  row << "\n";
				tobeadded.setDataRow(row,'\t');
				if (!getline(openedfile,row))
					break;
				row_count++;
			}
			//If it is 1st bunch after freq change, discard it if do_not_load_first_bunch_after_freq_change == true
			//2006-09-28
			if ( !((do_not_load_first_bunch_after_freq_change && (n_th_bunch_after_freq_change == 1) ) )) {
				if ( searchForFrequencyMatch(new_freq) == -1 )
					addFrequencyUnit(tobeadded);
				else{
					frequencies[searchForFrequencyMatch(new_freq)]->addNewBunch(tobeadded,log);
					frequency_number_is_found = true;
					load_bar->set_pulse_step(1.0/number_of_rounds_in_file);
				}
			}
		}
	}
	end_time = time_string;
	openedfile.close();
	order_data();
	return "";
}
//------------------------------------------------------------------------------
int cFrequencies::getNumberOfRoundsInFile_v3(const boost::filesystem::path bunchfilename, bool force, ostream& log){
	if ( !is_this_v3_bunchfile(bunchfilename))
		return -2;
//	cout << data_file << "==data_file\n";
//	cout << dirname << filename << "==dir+file\n";
	if (force)
		number_of_rounds_in_file_checked = false;
	if (data_file == (bunchfilename) ) {
		if (number_of_rounds_in_file_checked) {
//			cout << number_of_rounds_in_file << "==roundeja tsekkaamatta.\n";
			return number_of_rounds_in_file;
		}
	}

	boost::filesystem::ifstream openedfile;
	openedfile.open((bunchfilename).c_str());
  	if (!openedfile){
		//cout << "File " << dirname << filename << " doesn't open/exist!\n(tried to read number of rounds in)\n";
		number_of_bunches_in_one_spectrum = -1;
		number_of_rounds_in_file_checked=false;
		return -1;
	}
	string row;
	bool eka_found = false;
	double eka_freq = -1.0;
	int countt(0);
	while (getline(openedfile,row)) {
		if (row.size() < 10)
			continue;

		if (row.find("Frequency:") == string::npos) {
			continue;
		}
		if (eka_found == false) {
			eka_freq = get_frequency_from_string(row);
			eka_found = true;
		}
		if (get_frequency_from_string(row) == eka_freq)
			countt++;
	}
	openedfile.close();
	//cout << countt << " verran taajuutta.\n";
	data_is_in_single_bunch_mode = is_data_collected_in_single_bunch_mode_v3(bunchfilename,log);

	data_file = bunchfilename; number_of_rounds_in_file_checked = true;
	number_of_rounds_in_file = countt;
	return countt;
}
//------------------------------------------------------------------------------
bool cFrequencies::is_data_collected_in_single_bunch_mode_v3(const boost::filesystem::path& bunchfile, ostream& log){
	boost::filesystem::ifstream s;
	s.open((bunchfile).c_str());
	string row;
	while (getline (s,row) ) {
		if ( row[0] != '\f')
			continue;
		else {
			if (!getline (s,row) ) //Data collection time -line
				break;
			if (!getline (s,row) ) //Next to data collection time -line
				break;

			//If line next to data collection time line is not starting with #, then there is no change
			//in scan parameter, ie it is not in single bunch mode.
			if (row[0] != '#')
				return false;
			else
				return true;
		}

	}
	s.close();
	log << "Could not determine, whether single bunches or not.\n";
	return false;
}//------------------------------------------------------------------------------
bool cFrequencies::is_data_collected_in_single_bunch_mode_v4(const boost::filesystem::path & bunchfile, std::ostream &log){
	boost::filesystem::ifstream s;
	s.open((bunchfile).c_str());
	string row;
	int n(0);
	string fields[10];
	while (getline(s,row)) {
		n++;
		if (n > 200)
			break;
		if ( row[0] == '#' ) {
			continue;
		}
		int n(1);
		split_feb09_datarow_to_fields(row,fields,n);
		if (fields[0] == "P") {
			if (fields[1] != "mcs")
				continue;
			else {
				if (fields[3] == "records per scan") {
					if (fields[4] == "1") {
						s.close();
						return true;
					}
				}
			}
		}
	}
	s.close();
	log << "Could not determine, whether single bunches or not (datafile v4).\n";
	return false;
}
//------------------------------------------------------------------------------
bool cFrequencies::is_data_collected_in_single_bunch_mode(void) {
	return data_is_in_single_bunch_mode;
}
//------------------------------------------------------------------------------

bool cFrequencies::setAverageTime(string time_string){
	start_time = time_string;
	end_time = time_string;
	return true;
};
//------------------------------------------------------------------------------
string cFrequencies::readDirectoryPartly_v3(const boost::filesystem::path bunchfilename,
						int start_round, int end_round, ostream &log)
{
	load_bar->set_fraction(0.0);
	setAllDataNotFitted();
	boost::filesystem::ifstream openedfile;
	string row,erotettu;
	double new_freq(0),ref_freq(0);
	int current_round(1);
	//openedfile.open((dirname+filename).c_str() );
  	//if (!openedfile)
	//  return "File " + dirname + filename + " doesn't open/exist!\n(tried to read part dir)\n";
	if (!(open_file_for_reading_and_popup_message_if_error(bunchfilename.string(),openedfile))) {
		return "File " + bunchfilename.string() + " doesn't open/exist!\n(tried to read part dir)\n";
	}
	new_freq=0;											//Resetting frequency
	log.precision(12);
// Part 1. Getting frequency from the very beginning header
// And adding rows corresponding to it
	string time_string;
	while ( getline(openedfile,row)) {     												//Fetch frequency from datafile
		if (row[0] != '#') break;           //If not a comment, skipping frequency hunt
		do{                                 //Search pattern "Frequency: "
			erotettu = erota(row,' ',0);
			if (erotettu == "Frequency:"){ //Searches for pattern "Frequency:"
				erotettu = erota(row,' ',0);//if found, next word is set as freq

//				cout << erotettu << "\n";
//				string foo;
//				getline(cin, foo);
				new_freq = atof(erotettu.c_str());
				ref_freq = new_freq;
//				sscanf(erotettu.c_str(),"%lf",&new_freq);
				break;
			}
			if (erotettu == "File"){		//Getting record time of bunch #1
				erotettu = erota(row,' ',0); //saved
				erotettu = erota(row,' ',0); //at
				erotettu = erota(row,'\r',0); //rest all the way to carriage return..
				time_string = erotettu;			 // Rest of it 2005-02-32 16:40:40
				//start_time = erotettu;
				break;

			}
			if (erotettu == "width:"){ //Search for bin width
				erotettu = erota(row,' ',0);
				sscanf(erotettu.c_str(),"%lf",&bin_width);
				bin_width *= 1e6;
				break;
			}
			if (erotettu == "record:"){ //Search for bins per record ie. 2048
				erotettu = erota(row,' ',0);
				sscanf(erotettu.c_str(),"%d",&max_bin_number);
				break;
			}
			if (erotettu == "Records"){ //Search for records per scan: ie. 1 (<->single bunch mode)
				erotettu = erota(row,' ',0);
				erotettu = erota(row,' ',0);
				erotettu = erota(row,' ',0);
				sscanf(erotettu.c_str(),"%d",&number_of_bunches_in_one_spectrum);
				break;
			}
		} while (erotettu != ""); //Breaking up line until it is empty
//		if (new_freq != 0)
//			break;
	};	//Finding freq until found
//	row -string is the line having row[0] != '#'
	{
		cUnit tobeadded;
		tobeadded.putFrequency(new_freq);
		tobeadded.putTimeString(time_string);
		while ( row[0] != '\f'){
			tobeadded.setDataRow(row,'\t');
			if (!getline(openedfile,row)){
				break;
			}
		}
		if (start_round <= 1){
			start_time = time_string;
			if ( searchForFrequencyMatch(new_freq) == -1 ) //If it is not a new freq, making one new
				addFrequencyUnit(tobeadded);
			else
				frequencies[searchForFrequencyMatch(new_freq)]->addNewBunch(tobeadded,log);
		}
	}
// Part 2. Adding all the rest bunches
	while ( getline(openedfile, row)){ //This is the data collection time - line
		erotettu = erota(row,' ',0); // #
		erotettu = erota(row,' ',0); // Data
		erotettu = erota(row,' ',0); // Collection
		erotettu = erota(row,' ',0); // Time:
		erotettu = erota(row,'\r',0); // 2005-02-02 13:30:40
		time_string = erotettu;
		if (!getline(openedfile, row)) //NEXT ROW AFTER COLLECTION TIME -line
			break;
		if (row[0] == '\f'){		//if it is end char, (freq did not change) this bunch is not empty.
									//Single bunch mode, but same freq scanned many times before change
			frequencies[searchForFrequencyMatch(new_freq)]->addOneEmptyBunch(time_string,log);
			cout << "should not happen\n";
			continue;
		}
		if (row[0] == '#'){
			do{                                 //Search pattern "Frequency: "
				erotettu = erota(row,' ',0);
				if (erotettu == "Frequency:"){ //Searches for pattern "Frequency:"
					erotettu = erota(row,' ',0);//if found, next word is set as freq
					new_freq = atof(erotettu.c_str());//erotettu.c_str());
					if ( fabs(new_freq-ref_freq) < 0.00001){
						current_round++;
						load_bar->set_fraction(((current_round)*1.0)/(end_round*1.0)); update_gtk();
					}
					break;
				}
			} while (erotettu != ""); //Breaking up line until it is empty
			if (erotettu == "")
				return "Sumthin' wronk in da bunch file!\n";
		}
		if (current_round > end_round)
			break;
		if (current_round < start_round){
			while ( row[0] != '\f')
				if (!getline(openedfile,row))
					break;
			continue;
		}
		{
//			log << ".";
			cUnit tobeadded;
			if (start_round == current_round)
				start_time = time_string;
			tobeadded.putFrequency(new_freq);
			tobeadded.putTimeString(time_string);
			while ( row[0] != '\f'){
				tobeadded.setDataRow(row,'\t');
				if (!getline(openedfile,row))
					break;
			}
			if ( searchForFrequencyMatch(new_freq) == -1 )
				addFrequencyUnit(tobeadded);
			else{
			 frequencies[searchForFrequencyMatch(new_freq)]->addNewBunch(tobeadded,log);
			}
			//load_bar->pulse(); update_gtk();
		}
	}
	end_time = time_string;
	openedfile.close();
	order_data();
	log << "\n";
	log << "Frequencies: " << frequencynumber << "\n\n";
//	frequencies[0]->print_contents_of_this_class(std::cout);
	log << "Start: \"" << start_time << "\"\n";
	log << "End: \"" << end_time << "\"\n";
	log << "ave: \"" << getAverageTime() << "\"\n";


//	fitting->get_approx_field("fields.txt");
//	fitting->print_fieldpoints();
//	fit_func->get_approx_field("fields.txt");
//	fit_func->print_fieldpoints();

	//cout << "Total number of rounds in file: " << getNumberOfRoundsInFile(dirname,filename) << endl;
	return "";
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
string cFrequencies::addFreqFromFile(const string &datafilename,int file_no, ostream &log){
  cUnit tobeadded;
  tobeadded.read_file(datafilename, delimiter, file_no);
  //log << "Read file " << datafilename << "\n";
  //log << ".";
  double new_freq = tobeadded.getFrequency();
  if ( searchForFrequencyMatch(new_freq) == -1 )
    addFrequencyUnit(tobeadded);
  else{
//	frequencies[searchForFrequencyMatch(new_freq)]->addDataToExisting(tobeadded,log);
	frequencies[searchForFrequencyMatch(new_freq)]->addNewBunch(tobeadded,log);
  }
  return "";
}
//------------------------------------------------------------------------------
//USE ONLY TO CREATE NEW; if adding bunches, use cUnit::addNewBunch
string cFrequencies::addFrequencyUnit(const cUnit &toAdd){
	cUnit *uusi;
	if (frequencynumber >= maxfrequencies)
		increaseFrequencyTableSize(50);
		//return "Too many frequencies!";
	uusi = new cUnit(toAdd);
	frequencies[frequencynumber] = uusi;

	uusi->set_unit_time_for_ions_if_not_set();
	if (uusi->getCounts() == 0){
		uusi->addOneEmptyBunch(uusi->getTimeString());
	}
	else
		uusi->increaseBunchNumber();

	frequencynumber++;
	data_in_order=false;
	return "";
}
//------------------------------------------------------------------------------
int cFrequencies::searchForFrequencyMatch(double verrokki){
	cout.precision(12);
  for (int i=0;i<frequencynumber;i++){
    if ( fabs( frequencies[i]->getFrequency() - verrokki  ) < EPSILON ){
//			cout << "Taajuudet " << frequencies[i]->getFrequency() << " ja " << verrokki << " tasmaavat.\n";
      return i;
		}
  }
//	cout << "Taajuutta" << verrokki << " ei löytyny.\n";
  return -1;
}
//------------------------------------------------------------------------------
int cFrequencies::getFrequencyNumber(void){
  return frequencynumber;
}
//------------------------------------------------------------------------------
/*
string cFrequencies::outputToFile(const string &outfile,
        int startbin, int stopbin, const char outdelimiter, ostream &log){
  if (frequencynumber==0)
    return "Something is amiss... Strange, ZERO frequencies! Bug beep erg!\n";
  if(!data_in_order)
    order_data();
  ofstream file(outfile.c_str());
  if (!file)
    return "Error opening writefile!";
  log << "Opened file " << outfile << " for writing \n";
  log << "Outputting from bin " << startbin << " to " << stopbin << "\n";
//  file << "#f tof err counts\n";   //Header line
  file.precision(10);
  int i;
  for (i=0;i<frequencynumber; i++){
	if (frequencies[i]->is_active==false) continue; //LIMITATION
    if (frequencies[i]->getCounts(startbin,stopbin) < 1)
      continue;
    file << frequencies[i]->getFrequency()                        << outdelimiter
         << frequencies[i]->getAverage(startbin,stopbin)*bin_width << outdelimiter
         << frequencies[i]->getSdom(startbin,stopbin)*bin_width    << outdelimiter
         << endl;
  }
  log << "\nTotal of " << i << " different frequencies.\n";
  file.close();
  return "";
}*/
//------------------------------------------------------------------------------
string cFrequencies::outputCountsToFile(const string &outfile,
         int startbin, int stopbin, const char outdelimiter, ostream &log){
  if (frequencynumber==0)
    return "Something is amiss... Strange, ZERO frequencies! Bug beep erg!\n";
  if(!data_in_order)
    order_data();
  int i;
  boost::filesystem::ofstream file;//(outfile.c_str());

  if (!open_file_for_overwriting_and_popup_message_if_error(outfile, file)) {
      return "There was terror attack when opening counts_out_file!\n";
  }
  log << "Opened file " << outfile << " for writing counts \n";
  log << "Outputting COUNTS from bin " << startbin << " to " << stopbin <<
  	     "to file " << outfile << " \n";
  file.precision(10);
  for (i=0;i<frequencynumber;i++){
	if (frequencies[i]->is_active==false) continue; //LIMITATION
    file << frequencies[i]->getFrequency()                        << outdelimiter
         << frequencies[i]->getCounts(startbin,stopbin)           << endl;
  }
  return "";
}
//------------------------------------------------------------------------------
int cFrequencies::order_data(void){
	if (data_in_order)
		return 0;
	cout.precision(12);
  for (int i=0;i<frequencynumber;i++){
	  for (int j=(i+1);j<frequencynumber;j++){
    	if ( ( frequencies[j]->getFrequency() < frequencies[i]->getFrequency()  ) ){
			cUnit *swapper;
			swapper = frequencies[i];
			frequencies[i]=frequencies[j];
			frequencies[j]=swapper;
		}
	  }
  }
	data_in_order=true;
  return 0;
}
//------------------------------------------------------------------------------
string cFrequencies::outputCountsFromEachFile(const string &outfile, int startbin, int stopbin, ostream &log){
	boost::filesystem::ofstream each_freq_file;
	if (!open_file_for_appending_and_popup_message_if_error(outfile, each_freq_file))
		return "Error outputting counts from each file!\n";
	//each_freq_file.open(outfile.c_str());
	for (int i=0;i<frequencynumber;i++){
		if (frequencies[i]->is_active==false) continue; //LIMITATION
		cout << frequencies[i]->outputEachFileOfFrequency(each_freq_file,startbin,stopbin);
	}
	each_freq_file.close();
	return "";
}
//------------------------------------------------------------------------------
double cFrequencies::getFreqHavingMinimumTof(int class_number){
	double minimum_tof_so_far(1e11);
	double freq=frequencies[0]->getFrequency();  //what if [0]->is_active==false?
	for (int i=0;i < frequencynumber;i++){
		if (frequencies[i]->is_active==false) continue; //LIMITATION
		if (frequencies[i]->getTimeOfFlight(bin_start,bin_stop,class_min_cts[class_number],class_max_cts[class_number]) < 0)
			continue;
		if (frequencies[i]->getTimeOfFlight(bin_start,bin_stop,class_min_cts[class_number],class_max_cts[class_number]) < minimum_tof_so_far){
			minimum_tof_so_far = frequencies[i]->getTimeOfFlight(bin_start,bin_stop,class_min_cts[class_number],class_max_cts[class_number]);
			freq = frequencies[i]->getFrequency();
		}
	}
	return (freq-0.001);
}
//------------------------------------------------------------------------------
double cFrequencies::getFreqHavingMaximumTof(int class_number){
	double maximum_tof_so_far(0.1);
	double freq=frequencies[0]->getFrequency();  //what if [0]->is_active==false?
	for (int i=0;i < frequencynumber;i++){
		if (frequencies[i]->is_active==false) continue; //LIMITATION
		if (frequencies[i]->getTimeOfFlight(bin_start,bin_stop,class_min_cts[class_number],class_max_cts[class_number]) < 0)
			continue;
		if (frequencies[i]->getTimeOfFlight(bin_start,bin_stop,class_min_cts[class_number],class_max_cts[class_number]) > maximum_tof_so_far){
			maximum_tof_so_far = frequencies[i]->getTimeOfFlight(bin_start,bin_stop,class_min_cts[class_number],class_max_cts[class_number]);
			freq = frequencies[i]->getFrequency();
		}
	}
	return (freq-0.001);
}
//------------------------------------------------------------------------------
string cFrequencies::outputTotalCountsToFile(const string &name_of_data, const string &outfile, int startbin, int stopbin, ostream &log){
	boost::filesystem::ofstream file;
	if (frequencynumber == 0)
		return "There are no Frequencies. EXIting, TOTAL counts not written!";
	//file.open( outfile.c_str(),ios::app);
	if (!open_file_for_appending_and_popup_message_if_error(outfile, file))
		return ("Al Gida made an attack! File " + outfile + " could not\n be opened for writing TOTAL counts!\n");
	log << "Opened file " << outfile << " for writing TOTAL counts \n";
	file << name_of_data << " " << getTotalCounts(startbin,stopbin) << " " << frequencynumber << "\n";
	file.close();
	return "";
}
//------------------------------------------------------------------------------
int cFrequencies::getTotalCounts(int startbin, int stopbin){
	int total_number = 0;
	for (int i=0;i<frequencynumber;i++){
	         total_number += frequencies[i]->getCounts(startbin,stopbin);
	}

	return total_number;
}
//------------------------------------------------------------------------------
string cFrequencies::printAllOut(ostream &to_here, int what_frequency){
	int start,stop;
	if (what_frequency >= frequencynumber){
		char num[100];
		sprintf(num,"%d",what_frequency);
		string freq = "Frequency number ";
		freq.append(num);
		freq.append(" does not exist!\n");
		return freq;
	}
	if (what_frequency < 0){
		start=0,
		stop=frequencynumber;
	}
	else{
		start = what_frequency;
		stop  = what_frequency+1;
	}

	for (int i=start;i<stop;i++){
		to_here << "============Number: " <<  i << "\n";
		to_here << "Includes data: \n";
		frequencies[i]->print_contents_of_this_class(to_here);
	}
	return "";
}
//------------------------------------------------------------------------------
string cFrequencies::outputEventNumberToFile(const string &outfile, int startbin,
                      int stopbin, int max_events, const char outdelimiter, ostream &log){
	if (frequencynumber == 0)
		return "There are no Frequencies. EXIting, EVENTS counts not written!";
  	boost::filesystem::ofstream file;
	if (!open_file_for_overwriting_and_popup_message_if_error(outfile, file) )
		return ("Al Gida made an attack! File " + outfile + " could not\n be opened for writing EVe-nts!\n");
	log << "Opened file " << outfile << " for writing EVENTS \n";
	int total_events(0);
	int eventnumber(0);
	file << "#ions/bunch how_many_bunches" << "\n";
//	for (int events=0;events<max_events+1;events++){
	for (int events=0;events<max_events+1;events++){
		for (int i=0;i<frequencynumber;i++){
		if (frequencies[i]->is_active==false) continue; //LIMITATION
//		for (int i=12;i<13;i++){
			eventnumber += frequencies[i]->getEventNumber(startbin,stopbin,events);
		}
		file << events << outdelimiter << eventnumber << "\n";
//		cout << "öh" << events << outdelimiter << eventnumber << "\n";
		total_events += eventnumber;
		eventnumber=0;
	}
	file << "# Total different frequencies in file: " << frequencynumber << "\n";
	file << "# Total events here: " << total_events << "\n";
	file.close();
	return "";
}
//------------------------------------------------------------------------------
string cFrequencies::outputClassToFile(const string &outfile, int startbin,
                      int stopbin, int min_cts, int max_cts, const char outdelimiter, ostream &log){

												cout << "using outdated method outputClassToFile. Aborting.\n";
	abort();
	if (frequencynumber==0)
		return "Something is amish... Strange, subZERO frequencies! Bug beep erg!\n";
	if(!data_in_order)
		order_data();
	boost::filesystem::ofstream file;//(outfile.c_str());
	if (!open_file_for_overwriting_and_popup_message_if_error(outfile, file) )
		return "Error opening class writefile!";
	log << "\nOpened file " << outfile << " for writing \n";
	log << "Outputting from bin " << startbin << " to " << stopbin
		<< ", class " << min_cts << "-" << max_cts << "\n";
	file.precision(10);
	int i;
	double tof, tof_err;
	for (i=0;i<frequencynumber; i++){
		if (frequencies[i]->is_active==false) continue; //LIMITATION
		tof     = frequencies[i]->getTimeOfFlight(startbin,stopbin,min_cts,max_cts);
		//This is crap!!! dont use!
		tof_err = frequencies[i]->getTimeOfFlightError(startbin,stopbin,min_cts,max_cts);

		if ((tof < 0.01) || (tof_err < 0.000001))
			file << "#";
		file	<< frequencies[i]->getFrequency()	<< outdelimiter
				<< tof*bin_width						<< outdelimiter
				<< tof_err*bin_width					<< outdelimiter
				<< endl;
	}
	log << "\nTotal of " << i << " different frequencies.\n";
	file.close();
	return "";
}
//------------------------------------------------------------------------------
string cFrequencies::printHistogram(int startbin, int stopbin,int max_events, ostream &log){
	int eventnumber(0), total_events(0),total_counts(0),countss;
	string event,how_many,counts;
	char number1[5],number2[5],number3[5];
	for (int events=0;events<max_events+1;events++){
		sprintf(number1,"%d",events);
		event.append(number1);
		event.append("\t");
		for (int i=0;i<frequencynumber;i++){
			if (frequencies[i]->is_active==false) continue; //LIMITATION
			eventnumber += frequencies[i]->getEventNumber(startbin,stopbin,events);
		}
		sprintf(number2,"%d",eventnumber);
		how_many.append(number2);
		how_many.append("\t");
		countss= events*eventnumber;
		sprintf(number3,"%d",countss);
		counts.append(number3);
		counts.append("\t");
//		cout << "öh" << events << outdelimiter << eventnumber << "\n";
		total_events += eventnumber;
		total_counts += eventnumber*events;
		eventnumber=0;
	}
/*	log << "detected\t" << event		<< "\n";
	log << "bunch # \t" << how_many	<< "\n";
	log << "counts  \t" << counts	<< "\n";
	log << "TOTAL AMOUNT OF COUNTS: " << total_counts << "\n";
	log << " TOTAL BUNCHES: " << total_events << "\n";
	log << " OTHER METHOD:  " << getTotalCounts(startbin,stopbin,0,999,log) << "\n"; */
	return "";
}
//------------------------------------------------------------------------------
int cFrequencies::getTotalCounts(int startbin, int stopbin, int min_cts, int max_cts, ostream &log){
	int total_counts(0);
	for (int i=0; i < frequencynumber; i++){
		if (frequencies[i]->is_active==false) continue; //LIMITATION
		total_counts += frequencies[i]->getCounts(startbin,stopbin,min_cts,max_cts);
	}
	return total_counts;
}
//------------------------------------------------------------------------------
string cFrequencies::divideAutomaticallyToClasses(int startbin, int stopbin, int min_cts, int max_cts, int how_many_classes, ostream &log){
	setAllDataNotFitted();
	if (how_many_classes < 1){
		log << "Tried to divide to 0 classes or less. Exciting.\n";
		return "Tried to divide to 0 classes or less. Exciting.\n";
	}

	else{
		cts_start = min_cts;
		cts_stop = max_cts;
		class_max_cts[how_many_classes-1] = max_cts;
		int counts_so_far(0);
		class_min_cts[0] = min_cts;
		int total_counts = getTotalCounts(startbin,stopbin,min_cts,max_cts,log);
//		cout << "Got total counts: " << total_counts << "\n";
		int counts_in_class = total_counts / how_many_classes;
		int class_in_hand(0);
		for (int i=min_cts;i <= max_cts; i++){
			if (i == 0) continue;
			counts_so_far += getTotalCounts(startbin,stopbin,i,i,log);
			if ((counts_so_far + getTotalCounts(startbin,stopbin,i+1,i+1,log)/4) > counts_in_class ){
				counts_so_far = 0;
				class_max_cts[class_in_hand]=i;
				class_in_hand++;
				class_min_cts[class_in_hand]=(i+1);
			}
		}
		class_max_cts[how_many_classes-1] = max_cts;
	}
	for (int i=0;i < MAXCLASSES;i++){
		is_class_fitted[i] = false;
	}
	number_of_classes=how_many_classes;
	bin_start= startbin; bin_stop = stopbin;
	//log << "Class division:\n";
	//log << "startbin: " << startbin << " stopbin: " << stopbin << " min_counts: " << min_cts;
	//log << " max_counts: " << max_cts << " Divide to " << how_many_classes << " class\n";
	//log << "class\tmin-max\tcounts_in_class\n";
	//for (int i=0; i < how_many_classes; i++){
	//	log << i << ":\t" << class_min_cts[i] << "-" << class_max_cts[i] << '\t';
	//	log	<< getTotalCounts(startbin,stopbin,class_min_cts[i],class_max_cts[i],log) << "\n";
	//}
	if (how_many_classes > 1)
		if (class_min_cts[how_many_classes-1] == 0)
				log << "WARNING! CLASS DIVISION WAS BAD; Try less amount of classes.\n";
	return "";
}
//------------------------------------------------------------------------------
string cFrequencies::divideManuallyToClasses(vector<TWO_INTS> boundaries) {
	if (boundaries.empty() )
		return "No boundaries!\n";
	int counter(0);
	for (vector<TWO_INTS>::iterator i=boundaries.begin(); i != boundaries.end(); ++i) {
		class_min_cts[counter] = i->low;
		class_max_cts[counter] = i->high;
		counter++;
	}
	number_of_classes = counter;
	return "";
}
//------------------------------------------------------------------------------
vector<TWO_INTS> cFrequencies::get_class_borders_from_data() {
	vector<TWO_INTS> returnee;
	TWO_INTS toAdd;
	for (int i=0; i < number_of_classes; i++) {
		toAdd.low = class_min_cts[i];
		toAdd.high = class_max_cts[i];
		returnee.push_back(toAdd);
	}
	return returnee;
}
//------------------------------------------------------------------------------
bool cFrequencies::initializeSingleFit(double f, bool ff, double p, bool fp, double ax, bool fax,
 double magn, bool fmagn,
 double cycl, bool fcycl, double fi, bool ffi,
 double t_rf, bool ft_rf, double a_rf, bool fa_rf){
	//fit_two_peaks = false;
	number_of_peaks_to_fit = 1;
	is_crap_with_constant_tof_used_in_fitting = false;
	tof_function_to_be_fitted = SINGLE_PEAK_FIT;
	a[0] = f;				ia[0] = ff;
	a[1] = p;				ia[1] = fp;
	a[2] = ax;				ia[2] = fax;
	a[3] = magn;	ia[3] = fmagn;
	a[4] = cycl;	ia[4] = fcycl;
	a[5] = fi;				ia[5] = ffi;
	a[6] = t_rf;			ia[6] = ft_rf;
	a[7] = a_rf;	ia[7] = fa_rf;
	//Next: non-fitted
	//a[8] = 6666666.6*2*PII; a[9]=1;
	//ia[8] = 0; ia[9]=0;

	ma = 8;
	mfit = 0;
	for (int i = 0;i<ma;i++){
		if (ia[i]) mfit++;
	}
	return 0;
}

//------------------------------------------------------------------------------
bool cFrequencies::initializeSecondPeakForFitting(double f2, bool ff2, double fractio, bool ffractio) {
	number_of_peaks_to_fit = 2;
	tof_function_to_be_fitted = DOUBLE_PEAK_FIT;
	a[8] = f2;	ia[8] = ff2;
	a[9] = fractio; ia[9] = ffractio;
	ma = 10;
	mfit = 0;
	for (int i = 0;i<ma;i++){
		if (ia[i]) mfit++;
	}
	return 1;
}
//------------------------------------------------------------------------------
bool cFrequencies::initializeThirdPeakForFitting(double f3, bool ff3, double weight3, bool fweight3)
{
	number_of_peaks_to_fit = 3;
	tof_function_to_be_fitted = TRIPLE_PEAK_FIT;
	a[10] = f3;	ia[10] = ff3;
	a[11] = weight3; ia[11] = fweight3;
	ma = 12;
	mfit = 0;
	for (int i = 0;i<ma;i++){
		if (ia[i]) mfit++;
	}
	return 1;

}
//------------------------------------------------------------------------------
bool cFrequencies::initializeSingleRamseyFit(double f, bool ff, double ax, bool fax,
 double magn, bool fmagn, double t_fringe, bool ft_fringe,
 double t_tot, bool ft_wait, double a_rf, bool fa_rf)
{
	number_of_peaks_to_fit = 1;
	is_crap_with_constant_tof_used_in_fitting = false;
	tof_function_to_be_fitted = SINGLE_RAMSEY;
	a[0] = f;				ia[0] = ff;
	a[1] = ax;				ia[1] = fax;
	a[2] = magn;	ia[2] = fmagn;
	a[3] = t_fringe;			ia[3] = ft_fringe;
	a[4] = t_tot;			ia[4] = ft_wait;
	a[5] = a_rf;	ia[5] = fa_rf;
	//Next: non-fitted
	//a[8] = 6666666.6*2*PII; a[9]=1;
	//ia[8] = 0; ia[9]=0;

	ma = 6;
	mfit = 0;
	for (int i = 0;i<ma;i++){
		if (ia[i]) mfit++;
	}
	//Make rest of the fitvars to be -1
	a[6]=-1; a[7] = -1; a[8] = -1; a[9] = -1;

	return 1;
}
//------------------------------------------------------------------------------
bool cFrequencies::initializeRamseyDamping(double p, bool fp) {
	tof_function_to_be_fitted = SINGLE_RAMSEY_WITH_DAMPING;
	a[6] = p; ia[6] = fp;

	ma = 7;
	mfit = 0;
	for (int i = 0;i<ma;i++){
		if (ia[i]) mfit++;
	}
	//Make rest of the fitvars to be -1
	a[7] = -1; a[8] = -1; a[9] = -1;

	return 1;
}
//------------------------------------------------------------------------------
void cFrequencies::initializeConstantCrapForSinglePeakFit(double tof_of_crap, bool fit_tof_of_crap, double fraction_of_crap, bool fit_fraction_of_crap) {
	is_crap_with_constant_tof_used_in_fitting = true;
	//If it was normal resonance we want with crap
	if (tof_function_to_be_fitted == SINGLE_PEAK_FIT) {
		tof_function_to_be_fitted = SINGLE_PEAK_WITH_CRAP;
		a[8] = tof_of_crap;	ia[8] = fit_tof_of_crap;
		a[9] = fraction_of_crap; ia[9] = fit_fraction_of_crap;
		ma = 10;
		mfit = 0;
		for (int i = 0;i<ma;i++){
			if (ia[i]) mfit++;
		}
		return;
	}
	//If it was Ramsey resonance with crap
	if (tof_function_to_be_fitted == SINGLE_RAMSEY) {
		tof_function_to_be_fitted = SINGLE_RAMSEY_WITH_CRAP;
		a[6] = tof_of_crap;	ia[6] = fit_tof_of_crap;
		a[7] = fraction_of_crap; ia[7] = fit_fraction_of_crap;
		ma = 8;
		mfit = 0;
		for (int i = 0;i<ma;i++){
			if (ia[i]) mfit++;
		}
	}
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void cFrequencies::preInitializeTofFitter(int number_of_freqs, int number_of_fit_vars){
	x=nrutil.vector(0,number_of_freqs);
	y=nrutil.vector(0,number_of_freqs);
	sig=nrutil.vector(0,number_of_freqs);
	a=nrutil.vector(0,number_of_fit_vars);
	chi_vec=nrutil.vector(0,number_of_fit_vars);
	ia=nrutil.ivector(0,number_of_fit_vars);
	covar=nrutil.matrix(0,number_of_fit_vars,0,number_of_fit_vars);
	alpha=nrutil.matrix(0,number_of_fit_vars,0,number_of_fit_vars);
	for (int i=0;i<number_of_fit_vars;i++){
		a[i]=-1;
		ia[i]=0;
	}
	return;
}
//------------------------------------------------------------------------------
void cFrequencies::preInitializeLinearFitter(int number_of_freqs, int number_of_fit_vars){
	x2=nrutil.vector(0,number_of_freqs);
	y2=nrutil.vector(0,number_of_freqs);
	sig2=nrutil.vector(0,number_of_freqs);
	a2=nrutil.vector(0,number_of_fit_vars);
	chi_vec2=nrutil.vector(0,number_of_fit_vars);
	ia2=nrutil.ivector(0,number_of_fit_vars);
	covar2=nrutil.matrix(0,number_of_fit_vars,0,number_of_fit_vars);
	alpha2=nrutil.matrix(0,number_of_fit_vars,0,number_of_fit_vars);
	return;
}
//------------------------------------------------------------------------------
int cFrequencies::Fit(int class_number,ostream &log, bool use_auto_freq, int skip_from_start, int skip_from_end){
	fit_bar->set_fraction(0.0);
	fit_bar->set_text("");
	if (class_number >= number_of_classes){
		std::cout << "Class number " << class_number << " does not exist! Not fitted!\n";
		return -1;
	}
	if (class_min_cts[class_number] > class_max_cts[class_number] ) {
		std::cout << "Class is strange.. not fitted.\n";
		return -2;
	}
	if (getTotalCounts(bin_start,bin_stop, class_min_cts[class_number], class_max_cts[class_number] ) < 1 ) {
		std::cout << "No counts in class-- not fitted.\n";
		return -3;
	}
	fit_bar->set_text( "Fitting class " + asString(class_number));
/*	if (!are_field_files_ok){
		log << "Field files are non-existent or someting!\n";
		log << "Files should be:\n";
		log << e_field_filename << endl;
		log << b_field_filename << endl;
		return 2;
	}*/
	//2006-01-22 popupping an error, if less fitpoints than free parameters
	if (use_auto_freq && (number_of_peaks_to_fit == 1) &&  (get_fit_func_type() == SINGLE_PEAK_FIT || get_fit_func_type() == SINGLE_PEAK_WITH_CRAP)   ){
		a[0] = getFreqHavingMinimumTof(class_number);
		cout << "Guess for initial f_c: " << a[0] << "\n";
	}
	ndata=0;
	int steps(0);
	bool did_it_converge=false;
	double freq_test;
	for (int i=0;i < ma;i++){
//		cout << "fitvar " << i << " " << a[i] << " fitted: " << ia[i] << endl;
	}
	double total_sdom = get_sd_of_all_ions_in_class(class_number,stat_mode);

	for (int d=skip_from_start;d < (frequencynumber - skip_from_end);d++){
		if (frequencies[d]->is_active==false) continue; //LIMITATION
		x[ndata]=frequencies[d]->getFrequency();
		y[ndata]=frequencies[d]->getTimeOfFlight(bin_start,bin_stop,class_min_cts[class_number],class_max_cts[class_number])*bin_width;
		sig[ndata] = get_time_of_flight_error(d,class_number,total_sdom);
		if (y[ndata] > 0){
			ndata++;
//			cout << "Accepted\n";
		}
	}
	if ( ndata <= mfit ) {
		popupper->set_message("Too few datapoints to make the fit!\nClass #:"+asString(class_number)+"\nDatapoints: "+asString(ndata)+"\nFree parameters: " +asString(mfit)+"\n");
		//Gtk::Justification keskitys = popupper->dialog_head->get_justify();
		//popupper->dialog_head->set_justify(Gtk::JUSTIFY_LEFT);
		popupper->run();
		popupper->hide();
		//popupper->set_justify(keskitys);
		return -4;
	}



/*	for (int ab=0;ab < ndata;ab++){
		cout << "x:" << x[ab]/2/PII << "\t\t y:" << y[ab] << "\t\t err: " << sig[ab] << "\n";
	}*/
	for (int ab=0;ab < ma;ab++){
		cout << "a[" << ab << "]=" << a[ab] << ", Fit it: " << ia[ab] << "\n";
	}
//	cout.precision(13);
//	cout << "a[0] " << a[0]/2/PII << "\n";
//	cout << "ma: " << ma << " ndata " << ndata << "\n";
//	cout << "over here\n";

	int outer_steps = 45;
	int inner_steps =  5;
	int total_steps = (inner_steps +1) * outer_steps;
	int steps_done(1);
	cout << "Fit func type is: " << get_fit_func_type() << " (SINGLE_RAMSEY =4)\n";
	for (int j=0; j<outer_steps;j++){  //To avoid getting stuck
		alamda=-1;   //If alamda=-1, starts to fit from previous fit
		freq_test=a[0]; //stores a[0] to freq_test. Tests whether a[0] changes.
		fitting->mrqmin(x,y,sig,ndata,a,ia,ma,covar,alpha,&chisq,&alamda,get_fit_func_type()); steps++;

		for (int i=0;i<inner_steps;i++){   //HERE how many iterations
			fitting->mrqmin(x,y,sig,ndata,a,ia,ma,covar,alpha,&chisq,&alamda,get_fit_func_type());
			fit_bar->set_fraction( (steps_done*1.0)/(total_steps*1.0) );
			update_gtk();
			steps_done++;
		}
		alamda=0;
		fitting->mrqmin(x,y,sig,ndata,a,ia,ma,covar,alpha,&chisq,&alamda,get_fit_func_type()); steps++;//Gives out cov-matix if alamda=0
		fit_bar->set_fraction( (steps_done*1.0)/(total_steps*1.0) );
		update_gtk();
		steps_done++;

//		log << ".";
/*		{
			log.precision(13);
			log << "f:" <<a[0]/2/PII << " p";
			log.precision(5);
			log  << a[1] << " ax" << a[2] << " w-:" << a[3]*1e3 << " w+:"
				<< a[4]*1e3 << " fi:" << a[5] << " t:" << a[6]*1e3
				<< " A:" << a[7]*1e3;
			log.precision(3); cout << " Chi2/DoF: " << chisq/(ndata-mfit) << endl;
		}*/
		if (fabs(freq_test-a[0]) < 0.0000001){ //If a[0] doesn't change, then quitting
			did_it_converge = true;
			fit_bar->set_text("Converged; stop.");
			log << "\nThat was class " << class_number << ".\n";
			log << "Fit converged, stopping. There were " << steps << " fitting steps.\n";
			log << "Chisq: " << chisq/(ndata-mfit) << "\n";
			break;
		}
	}
	if ( !did_it_converge) {
		fit_bar->set_text( asString(steps_done) + " steps, not converged");
	}
//	std::cout << "Täällä.\n";
	for (int goo=0;goo<10; goo++) {
		fitparam[class_number][goo] = -1;
		fitparam_err[class_number][goo] = -1;
	}
	for (int goo=0;goo < ma; goo++){
//		cout << "HOHOHOO\n";
		if ( (goo == 0) || (goo == 8) || (goo == 10 && get_fit_func_type() == TRIPLE_PEAK_FIT )){
			fitparam_err[class_number][goo] = (sqrt(covar[goo][goo]));
			fitparam[class_number][goo] = a[goo];
		}
		else{
			fitparam[class_number][goo] = a[goo];
			fitparam_err[class_number][goo] = (sqrt(covar[goo][goo]));
		}
		if (!ia[goo])
			fitparam_err[class_number][goo] = 0;
		was_param_fitted[class_number][goo] = ia[goo];
		fit_chisq[class_number] = (chisq/(ndata-mfit));
	}
	is_class_fitted[class_number]=true;
	is_countrate_effect_fitted = false;
//	std::cout << "Täällä.\n";
	return 0;
}
//------------------------------------------------------------------------------
string cFrequencies::outputGnuplotFile(const string &outdir, string gp_file_name,
		string element_name, int scan_number, bool one_outfile_only, string terminal_suffix,
		string fit_file_suffix, string dat_file_suffix,
		 bool append, string header)
{
	boost::filesystem::ofstream plot;
	if (append) {
		//plot.open((outdir+gp_file_name).c_str(), ios::app);
		if (!open_file_for_appending_and_popup_message_if_error(outdir+gp_file_name,plot))
			return "Could not append to gnuplot outputfile\n";
	}
	else {
		//plot.open((outdir+gp_file_name).c_str());
		if (!open_file_for_overwriting_and_popup_message_if_error(outdir+gp_file_name,plot))
			return "Could not write to gnuplot outputfile\n";
	}
	plot << header << "\n";
	plot << "#-----------Plot of " << element_name << ", scan " << scan_number << "\n";
	for (int i=0;i<number_of_classes;i++){
		if (!one_outfile_only){
			plot << "set out \"" << element_name << scan_number << "_" << i << terminal_suffix  << "\"\n";
		}
		plot << "set title \"" << element_name << ", scan " << scan_number << "\"\n";

		if (is_class_fitted[i]) {
			// Next: frequency
			plot.precision(13);
			plot << "set label 1 \"Freq:    " << fitparam[i][0];
			plot.precision(5);
			if (was_param_fitted[i][0])
				plot  << " +- " << fitparam_err[i][0] << " Hz";
			else
				plot << " Hz (FIXED)";
			plot << "\" at graph 0.02,0.4 left font \"Courier,8\"\n";
			// Next: pressure
			plot << "set label 2 \"Pressure:" << fitparam[i][1];
			if (was_param_fitted[i][1])
				plot  << " +- " << fitparam_err[i][1] << " mbar";
			else
				plot << " mbar (FIXED)";
			plot << "\" at graph 0.02,0.35 left font \"Courier,8\"\n";
			// Next: axial energy
			plot << "set label 3 \"axial E: " << fitparam[i][2];
			if (was_param_fitted[i][2])
				plot << " +- " << fitparam_err[i][2] << " eV";
			else
				plot << " eV (FIXED)";
			plot << "\" at graph 0.02,0.30 left font \"Courier,8\"\n";
			// Next: magnetron radius
			plot << "set label 4 \"w- rad:  " << fitparam[i][3];
			if (was_param_fitted[i][3])
				plot << " +- " << fitparam_err[i][3] << " mm";
			else
				plot << " mm (FIXED)";
			plot << "\" at graph 0.02,0.25 left font \"Courier,8\"\n";
			// Next: red cyclotron radius
			plot << "set label 5 \"w+ rad:  " << fitparam[i][4];
			if (was_param_fitted[i][4])
				plot << " +- " << fitparam_err[i][4] << " mm";
			else
				plot << " mm (FIXED)";
			plot << "\" at graph 0.02,0.20 left font \"Courier,8\"\n";
			// Next: w- w+ phasediff
			plot << "set label 6 \"phase:   " << fitparam[i][5];
			if (was_param_fitted[i][5])
				plot << " +- " << fitparam_err[i][5] << " rad";
			else
				plot << " rad (FIXED)";
			plot << "\" at graph 0.02,0.15 left font \"Courier,8\"\n";
			// Next: Excitation time
			plot << "set label 7 \"Time:    " << fitparam[i][6];
			if (was_param_fitted[i][6])
				plot << " +- " << fitparam_err[i][6] << " s";
			else
				plot << " s (FIXED)";
			plot << "\" at graph 0.02,0.10 left font \"Courier,8\"\n";
			// Next: Amplitude
			plot << "set label 8 \"Amplitude: " << fitparam[i][7];
			if (was_param_fitted[i][7])
				plot << " +- " << fitparam_err[i][7] << " mV";
			else
				plot << " mV (FIXED)";
			plot << "\" at graph 0.02,0.05 left font \"Courier,8\"\n";

			// Next: frequency2
			if (number_of_peaks_to_fit > 1){
					plot.precision(10);
				plot << "set label 14 \"Freq2:    " << fitparam[i][8];
				plot.precision(5);
				if (was_param_fitted[i][8])
				  plot  << " +- " << fitparam_err[i][8] << " Hz";
				else
				  plot << " Hz (FIXED)";
				plot << "\" at graph 0.98,0.2 right font \"Courier,8\"\n";
				// Next: ratio of isomerz
				plot.precision(3);
				plot << "set label 15 \"Ratio:    " << fitparam[i][9];
				if (was_param_fitted[i][9])
				  plot  << " +- " << fitparam_err[i][9];
				else
				  plot << " (FIXED)";
				plot << "\" at graph 0.98,0.15 right font \"Courier,8\"\n";
				plot.precision(11);

				//Mark the 2nd freq point
				plot << "set arrow 11 from first " << fitparam[i][8] << ", graph 1"
					 << " to first " << fitparam[i][8] << ", graph 0 nohead lw 2\n";
			}
		}
		// Next: BIN-WINDOW
		plot << "set label 20 \"Bin-Window:   " << bin_start << "-" << bin_stop;
		plot.precision(4);
		plot << "  (" << bin_start*bin_width << "-" << bin_stop*bin_width << " us)\"";
		plot << "at graph 0.98,0.4 right font \"Courier,8\"\n";
		// Next: Class
		plot << "set label 21 \"Class:  " << i << "/" << (number_of_classes-1);
		plot << "  " << class_min_cts[i] << "-" << class_max_cts[i] << " counts/bunch\"";
		plot << "at graph 0.98,0.35 right font \"Courier,8\"\n";
		// Next: #ions Class
		plot << "set label 22 \"Ions in class/total:  " << getCountsInClass(i);
		plot << "/" << getTotalCounts(bin_start,bin_stop,cts_start,cts_stop) <<    "\"";
		plot << "at graph 0.98,0.30 right font \"Courier,8\"\n";
		// Next: Time of recording
		plot << "set label 23 \"Data collection time: " << getAverageTime();
		plot <<     "\"";
		plot << "at graph 0.98,0.25 right font \"Courier,8\"\n";



		// Next: khi^2/DoF
		if (is_class_fitted[i]) {
			plot << "set label 9 \"Chi^2/DoF: " << fit_chisq[i];
			plot << "\" at graph 0.98,0.05 right font \"Courier,8\"\n";
		}
		// Next: Time of plot
		//  plot << "set label 10 \"" << ctime(&rawtime);
		//  plot << "\" at graph 0.98,0.05 right font \"Courier,8\"\n";

		// Next: make line to mark w_c that is, a[0]
		plot.precision(11);

		if (is_class_fitted[i]) {
			plot << "set arrow 10 from first " << fitparam[i][0] << ", graph 1"
				 << " to first " << fitparam[i][0] << ", graph 0 nohead lw 2\n";
		}

		// Finally: plot the whole thing
		plot << "plot \"" << element_name << scan_number << "_" << i << dat_file_suffix << "\" t \"data\" w ye pt 7, \""
			 << element_name << scan_number << "_" << i << fit_file_suffix << "\" t \"fit\" w l\n";


	 } //End for
	plot.close();
	return "";
}
//------------------------------------------------------------------------------
string cFrequencies::get_fit_results_as_text(int class_number, string line_end)
{
  if (!is_class_fitted[class_number])
    return "";
  
  string s;
  cout << "ma " << ma << endl;
  cout << "func " << tof_function_to_be_fitted << endl;
  for (int i=0; i < ma; i++)
  {
//     cout << get_fit_parameter_name(tof_function_to_be_fitted, i) << endl;
    s += get_fit_parameter_name(tof_function_to_be_fitted, i) + ": ";
    if ( was_param_fitted[class_number][i] )
      s += as_3f(fitparam[class_number][i]) + " +/- " + as_3f(fitparam_err[class_number][i]);
    else
      s += as_3f(fitparam[class_number][i]) + " FIXED";
    
    s+= line_end;
  }
  s += "chisq/N = " + as_3f(fit_chisq[class_number]) + line_end;
  
  return s;
}

//------------------------------------------------------------------------------
string cFrequencies::outputFitsAndDats(const string &outdir, const string &element_name, const int scan_number,
		const string &fit_file_suffix,const string &dat_file_suffix)
{
	char scan_num[10];	sprintf(scan_num,"%d", scan_number);
	char class_num[10];
	boost::filesystem::ofstream fit_out,dat_out,dat2d_out;
	for (int i=0;i<number_of_classes;i++){
		sprintf(class_num,"%d", i);
		//fit_out.open((outdir+element_name+scan_num+"_"+class_num+fit_file_suffix).c_str());
		//dat_out.open((outdir+element_name+scan_num+"_"+class_num+dat_file_suffix).c_str());
		//dat2d_out.open((outdir+element_name+scan_num+"_"+class_num+"_2d"+dat_file_suffix).c_str());
		//if (!fit_out) {
		if (!open_file_for_overwriting_and_popup_message_if_error(
				outdir+element_name+scan_num+"_"+class_num+fit_file_suffix,fit_out,true)){
			return ("File " + outdir + element_name+scan_num+"_"+class_num+fit_file_suffix +
						" could not be opened for writing!\n");
		}
		//if (!dat_out) {
		if (!open_file_for_overwriting_and_popup_message_if_error(
				outdir+element_name+scan_num+"_"+class_num+dat_file_suffix,dat_out,true)){
			return ("File " + outdir + element_name+scan_num+"_"+class_num+dat_file_suffix +
						" could not be opened for writing!\n");
		}
		//if (!dat2d_out) {
		if (!open_file_for_overwriting_and_popup_message_if_error(
				outdir+element_name+scan_num+"_"+class_num+"_2d"+dat_file_suffix,dat2d_out,true)){
			return ("File " + outdir + element_name+scan_num+"_"+class_num+"_2d"+dat_file_suffix +
						" could not be opened for writing!\n");
		}
		fit_out << "#Window:\t" << bin_start << "-" << bin_stop << " bins, ";
		fit_out << bin_start*bin_width << "-" << bin_stop*bin_width << " us.\n ";
		fit_out << "#Class: \t" << i << ", [";
		fit_out << class_min_cts[i] << "-" << class_max_cts[i] << "]\n";

		double jyy;
		cout.precision(12);

		if (is_class_fitted[i] ) {
			double plotting_x_offset(1.5);
			if (is_fit_func_type_ramsey_type(tof_function_to_be_fitted))
				plotting_x_offset /= (2.*a[3] + a[4])*2.;
			else
				plotting_x_offset /= a[6];
			for (double aks=(getMinimumFrequency()-plotting_x_offset);aks<(getMaximumFrequency()+plotting_x_offset);aks=aks+0.02*0.8*plotting_x_offset){
				fit_func->funcs(aks,fitparam[i],&jyy,chi_vec,0,get_fit_func_type());
				fit_out.precision(12);
				fit_out << aks << " " << jyy << "\n";
			}
		}
		else
			fit_out << "# CLASS IS NOT FITTED.\n";



		if (frequencynumber==0)
			return "Something is amish... Strange, subZERO frequencies! Bug beep erg!\n";
	//	log << "Outputting from bin " << startbin << " to " << stopbin
	//		<< ", class " << min_cts << "-" << max_cts << "\n";
		dat_out.precision(10);
		int j;

		double tof, tof_err, tot_sd = get_sd_of_all_ions_in_class(i,stat_mode);
		for (j=0;j<frequencynumber; j++){
			if (frequencies[j]->is_active==false) continue; //LIMITATION
			tof     = frequencies[j]->getTimeOfFlight(bin_start,bin_stop,class_min_cts[i],class_max_cts[i])*bin_width;

			//Changed 6.8.2007
			//tof_err = frequencies[j]->getTimeOfFlightError(bin_start,bin_stop,class_min_cts[i],class_max_cts[i]);
			tof_err = get_time_of_flight_error(j,i,tot_sd);
			if ((tof < 0.01) || (tof_err < 0.000001))
				dat_out << "#";
//2008-08-07 BUG DISCOVERED!!! tof_err is multiplied twice with bin_width!! Should be only once!
//			dat_out	<< frequencies[j]->getFrequency()	<< ' '
//					<< tof*bin_width						<< ' '
//					<< tof_err*bin_width					<< ' '
			dat_out	<< frequencies[j]->getFrequency()	<< ' '
					<< tof						<< ' '
					<< tof_err					<< ' ';

			/// Extra info.
			dat_out << extra_info_dbl_1 << ' ';

			dat_out	<< endl;
		}
	print_2d_counts(dat2d_out,i);
	fit_out.close();
	dat_out.close();
	dat2d_out.close();
	//	log << "\nTotal of " << i << " different frequencies.\n";
	}
	//cout << "Täällä\n";

	//2007-04-23 outputting 2d-stuff

	return "";
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
string cFrequencies::getMCSdata(void){
	string data;
	char num[100];
	data.append("Number of frequencies: ");
	sprintf(num,"%d",frequencynumber);
	cout << "Hier: 1\n";
	data.append(num);
	data.append("\n");
	sprintf(num,"%d",getTotalBunchNumber());
	cout << "Hier: 2\n";
	data.append("Number of bunches:     ");
	data.append(num);
	data.append("\n");
	sprintf(num,"%d",getTotalCounts());
	cout << "Hier: 3\n";
	data.append("Number of counts:      ");
	data.append(num);
	data.append("\n");
	sprintf(num,"%lf",bin_width);
	cout << "Hier: 4\n";
	data.append("Bin width [us]:        ");
	data.append(num);
	data.append("\n");
	sprintf(num,"%d",max_bin_number);
	cout << "Hier: 5\n";
	data.append("Max bins:              ");
	data.append(num);
	data.append("\n");
	sprintf(num,"%d",number_of_bunches_in_one_spectrum);
	cout << "Hier: 6\n";
	data.append("#Bunches/spectrum:     ");
	data.append(num);
	data.append("\n");
	return data;
}
//------------------------------------------------------------------------------
int cFrequencies::getTotalBunchNumber(void){
	int bunches(0);
	for (int i=0;i < frequencynumber;i++){
		bunches += frequencies[i]->getTotalBunchNumber();
	}
	return bunches;
}
//------------------------------------------------------------------------------
int cFrequencies::getTotalCounts(void){
	int counts(0);
	for (int i=0;i < frequencynumber;i++){
		counts += frequencies[i]->getCounts();
	}
	return counts;
}
//------------------------------------------------------------------------------
string cFrequencies::getClassDivResult(void){
	string res;
	stringstream nulli;
	//char num1[15], num2[15], num3[15];
	for (int i=0; i< number_of_classes; i++){
	//	sprintf(num1,"%d",class_min_cts[i]);
	//	sprintf(num2,"%d",class_max_cts[i]);
	//	sprintf(num3,"%d",getTotalCounts(bin_start,bin_stop,class_min_cts[i],class_max_cts[i],nulli));
//		res.append("[");res.append(num1);res.append("-");res.append(num2);
//		res.append("]: ");res.append(num3);res.append(" ");
		res.append("[");
		res.append(asString(class_min_cts[i]));
		res.append("-");
		res.append(asString(class_max_cts[i]));
		res.append("]:");
		res.append(asString(getTotalCounts(bin_start,bin_stop,class_min_cts[i],class_max_cts[i],nulli)));
		res.append(" ");
	}
	return res;
}
//------------------------------------------------------------------------------
string cFrequencies::wasClassDivOk(void){
	if (number_of_classes == 0)
		return "Zero classes!";
	for (int i=0;i < number_of_classes - 1;i++)
		if (class_max_cts[i] >= class_min_cts[i+1])
			return "Overlapping classes!";
	for (int i=0;i < number_of_classes;i++)
		if (class_max_cts[i] < class_min_cts[i])
			return "Lower boundary larger than upper in some class!";
	for (int i=0;i < number_of_classes;i++)
		if ( getCountsInClass(i,cout) <= 0 )
			return "Some class has zero counts!";

	if (number_of_classes > 1)
		if (class_min_cts[number_of_classes-1] == 0)
			return "Last class bad.";
	bool is_ok=false;
	for (int i= cts_start; i <= cts_stop;i++) {
		for (int j=0;j < number_of_classes;j++) {
			if ( (i >= class_min_cts[j]) && (i <= class_max_cts[j]) )
				is_ok=true;
		}
		if (!is_ok)
			return "Ok, but not all counts used..";
		is_ok=false;
	}
	return "Ok.";
}
//------------------------------------------------------------------------------
string cFrequencies::fitAllClasses(ostream &log, int skip_from_start=0, int skip_from_end=0){

	return "";
}
//------------------------------------------------------------------------------
bool cFrequencies::getFitResults(const int class_no, double params[], int &number_of_fit_vars){
	number_of_fit_vars = ma;
	if (class_no > number_of_classes) {
		std::cout << "Illegal Class! That class does not exist\nCONTACT PROGRAMMER\n";
		return 0;
	}
//	cout << "Class no: " << class_no << endl;
	if (is_class_fitted[class_no]) {
		for (int i=0;i<ma;i++){
			params[i]= fitparam[class_no][i];
			//		cout << "params[" <<i <<"] " << params[i] << endl;
		}
	}
	else {
		for (int i=0;i<ma;i++)
			params[i]= -1;
	}
	return true;
}
//------------------------------------------------------------------------------
bool cFrequencies::getFitErrors(const int class_no, double params[], bool was_it_fitted[], int &number_of_fit_vars, double &khisq){
	number_of_fit_vars = ma;
	if (class_no > number_of_classes) {
		std::cout << "Illegal Class! That class does not exist\nCONTACT PROGRAMMER\n";
		return 0;
	}
	if (is_class_fitted[class_no]) {
		for (int i=0;i<ma;i++){
			params[i]= fitparam_err[class_no][i];
			was_it_fitted[i] = was_param_fitted[class_no][i];
		}
		khisq = fit_chisq[class_no];
	}
	else {
		for (int i=0;i<ma;i++){
			params[i]= -1;
			was_it_fitted[i] = was_param_fitted[class_no][i];
		}
		khisq = 666666;
		std::cout << "Check otherwise that class is fitted! THIS WAS NOT!\n";

	}
	return true;
}
//------------------------------------------------------------------------------
bool cFrequencies::getDataUsedInFit(
			const int class_number, double x_coord[], double y_coord[], double y_coord_error[], int &number_of_freqs)
{
	if (class_number > number_of_classes) {
		std::cout << "Illegal class! (getDataUsedInFit)\n";
		return false;
	}
	int blub(0);

	double total_sd = get_sd_of_all_ions_in_class(class_number,stat_mode);

	for (int i=0; i <frequencynumber; i++){
		if (frequencies[i]->is_active==false) continue; //LIMITATION
		x_coord[blub] = frequencies[i]->getFrequency();
		y_coord[blub] = frequencies[i]->getTimeOfFlight(bin_start,bin_stop,class_min_cts[class_number],class_max_cts[class_number])*bin_width;
		//y_coord_error[blub]=frequencies[i]->getTimeOfFlightError(bin_start,bin_stop,class_min_cts[class_number],class_max_cts[class_number])*bin_width;
		/*if (stat_mode == INDIVIDUAL_STATISTICS) {
			y_coord_error[blub]=frequencies[i]->getTimeOfFlightError(bin_start,bin_stop,class_min_cts[class_number],class_max_cts[class_number])*bin_width;
		}
		if (stat_mode == SUM_STATISTICS || stat_mode == SUM_STATISTICS2) {
			y_coord_error[blub] = total_sdom / sqrt(frequencies[i]->getCounts(bin_start,bin_stop,class_min_cts[class_number],class_max_cts[class_number])*1.0)*bin_width;
		}	*/
		y_coord_error[blub] = get_time_of_flight_error(i,class_number,total_sd);
		if (y_coord[blub] > 0)
			blub++;
	}
	number_of_freqs = blub;
	return true;
}
//------------------------------------------------------------------------------
bool cFrequencies::getDataUsedInFit(const int class_number, vector<Datapoint> & datas)
{
	datas.clear();
	if (class_number > number_of_classes) {
		std::cout << "Illegal classzzz! (getDataUsedInFit)\n";
		return false;
	}
	int blub(0);
	Datapoint toAdd;
	double total_sd = get_sd_of_all_ions_in_class(class_number,stat_mode);

	for (int i=0; i <frequencynumber; i++){
		toAdd.y_value = 0; toAdd.x_value = 0; toAdd.x_value_err = 0; toAdd.y_value_err = 0;
		if (frequencies[i]->is_active==false) continue; //LIMITATION
		toAdd.x_value = frequencies[i]->getFrequency();
		toAdd.y_value = frequencies[i]->getTimeOfFlight(bin_start,bin_stop,class_min_cts[class_number],class_max_cts[class_number])*bin_width;
		toAdd.y_value_err = get_time_of_flight_error(i,class_number,total_sd);
		if (toAdd.y_value > 0)
			datas.push_back(toAdd);
	}
	return true;
}
//------------------------------------------------------------------------------
//2016-03-30
vector< Datapoint >* cFrequencies::getFitAsPoints(const int class_number, int number_of_points_wanted, double how_much_outrange)
{
	tof_vs_frequency_fit.clear(); // in case fit does not exist, the returned container is empty
	if (class_number > number_of_classes) {		//If class does not exists
		std::cout << "This should not be happening! (getFitAsPoints)\n";
		return &tof_vs_frequency_fit;
	}
	if (!is_class_fitted[class_number]) {		//If class is not yet fitted, returning false
		return &tof_vs_frequency_fit;
	}
	
	// Fit does exists, let's do it!
	Datapoint toAdd; toAdd.x_value=0;toAdd.y_value=0;toAdd.x_value_err=0;toAdd.y_value_err=0;
	double step=( (getMaximumFrequency() - getMinimumFrequency()+2.0*how_much_outrange)/(1.0*number_of_points_wanted) );

	double jyy; //This is the y-coordinate [tof]; aks is the x-coordinate [Hz]
	int point(0);
// 	fitparam[class_number][0] *= (2.0*);
// 	fitparam[class_number][8] *= (2.0*PII);
	if (get_fit_func_type() == TRIPLE_PEAK_FIT)
	{
// 	  fitparam[class_number][10] *= (2.0*PII);
	}

	for (double aks = (getMinimumFrequency()-how_much_outrange);
				aks < (getMaximumFrequency()+how_much_outrange);
				aks += step)
	{
		if (point > number_of_points_wanted) {
			cout << "FIT OUTSIDE LEGAL RANGE! CONTACT THE PROGRAMMER!\n";
			break;
		}
		fit_func->funcs(aks,fitparam[class_number],&jyy,chi_vec,0,get_fit_func_type());
		toAdd.x_value = aks;
		toAdd.y_value = jyy;
//		cout << "Point #" << point << " x,y: " << x_coord[point] << " " << y_coord[point] << "\n";
		tof_vs_frequency_fit.push_back(toAdd);

	}
// 	fitparam[class_number][0] /= (2.0*PII);
// 	fitparam[class_number][8] /= (2.0*PII);
	if (get_fit_func_type() == TRIPLE_PEAK_FIT)
	{
// 	  fitparam[class_number][10] /= (2.0*PII);
	}
	return &tof_vs_frequency_fit;
}
/*
//------------------------------------------------------------------------------
bool cFrequencies::getFitAsPoints(const int class_number, vector<Datapoint> & datas, int number_of_points_wanted,double how_much_outrange)
{
	datas.clear();
	Datapoint toAdd; toAdd.x_value=0;toAdd.y_value=0;toAdd.x_value_err=0;toAdd.y_value_err=0;
	if (class_number > number_of_classes) {		//If class does not exists
		std::cout << "This should not be happening! (getFitAsPoints)\n";
		return false;
	}
	if (!is_class_fitted[class_number]) {		//If class is not yet fitted, returning false
		return false;
	}
	double step=( (getMaximumFrequency() - getMinimumFrequency()+2.0*how_much_outrange)/(1.0*number_of_points_wanted) );

	double jyy; //This is the y-coordinate [tof]; aks is the x-coordinate [Hz]
	int point(0);
	fitparam[class_number][0] *= (2.0*PII);
	fitparam[class_number][8] *= (2.0*PII);
	if (get_fit_func_type() == TRIPLE_PEAK_FIT)
	{
	  fitparam[class_number][10] *= (2.0*PII);
	}

	for (double aks = (getMinimumFrequency()-how_much_outrange);
				aks < (getMaximumFrequency()+how_much_outrange);
				aks += step)
	{
		if (point > number_of_points_wanted) {
			cout << "FIT OUTSIDE LEGAL RANGE! CONTACT THE PROGRAMMER!\n";
			break;
		}
		fit_func->funcs(aks*2.0*PII,fitparam[class_number],&jyy,chi_vec,0,get_fit_func_type());
		toAdd.x_value = aks;
		toAdd.y_value = jyy;
//		cout << "Point #" << point << " x,y: " << x_coord[point] << " " << y_coord[point] << "\n";
		datas.push_back(toAdd);

	}
	fitparam[class_number][0] /= (2.0*PII);
	fitparam[class_number][8] /= (2.0*PII);
	if (get_fit_func_type() == TRIPLE_PEAK_FIT)
	{
	  fitparam[class_number][10] /= (2.0*PII);
	}
	return true;
}
*/
//------------------------------------------------------------------------------

double cFrequencies::getMinimumFrequency(void){
	if (frequencynumber  == 0)
		return 0;
	//double freq=frequencies[0]->getFrequency();
	double freq = 1e25;
	for (int i=0;i<frequencynumber;i++){
		if (frequencies[i]->is_active==false) continue; //LIMITATION
		if (frequencies[i]->getFrequency() < freq)
			freq = frequencies[i]->getFrequency();
	}
	return freq;
}
//------------------------------------------------------------------------------
double cFrequencies::getMaximumFrequency(void){
	if (frequencynumber  == 0)
		return 0;
	//double freq=frequencies[0]->getFrequency();
	double freq = -1e25;
	for (int i=0;i<frequencynumber;i++){
		if (frequencies[i]->is_active==false) continue; //LIMITATION
		if (frequencies[i]->getFrequency() > freq)
			freq = frequencies[i]->getFrequency();
	}
	return freq;
}
//------------------------------------------------------------------------------
string cFrequencies::getHistogram(int counts_in_bin[], int max_bins){
	if (frequencynumber < 1)
		return "No data! -> No histogram!\n";
	for (int i=0;i<frequencynumber;i++){
		if (frequencies[i]->is_active==false) continue; //LIMITATION
		frequencies[i]->getHistogram(counts_in_bin,max_bins);
	}
	return "";
}
//------------------------------------------------------------------------------
string cFrequencies::getHistogram(vector<int> &in_here) {
	if (frequencynumber < 1)
		return "No data! -> No histogram!\n";
	in_here.clear();
	for (int i=0; i <= getMaxBins();i++)  //Make it 1 bigger..
		in_here.push_back(0);
	for (int i=0;i<frequencynumber;i++){
		if (frequencies[i]->is_active==false) continue; //LIMITATION
		frequencies[i]->getHistogram(in_here);
	}
	return "";
}


std::vector<Datapoint> * cFrequencies::get_histogram()
{
	// This is perhaps stupid to do; The histogram should be generated/updated when it is
	// ready so there is no need to regenerate whenever it is asked.
	tof_histogram_data.clear();
	
	// First we create an array of zeroes.
	for (int i=0; i <= getMaxBins();i++)  //Make it 1 bigger..
		tof_histogram_data.push_back(Datapoint(i,0,0,0));
	
	// Then we add each frequency histogram on top of it
	for (int i=0;i<frequencynumber;i++){
		if (frequencies[i]->is_active==false) continue; //LIMITATION
		frequencies[i]->get_histogram(&tof_histogram_data);
	}
	return &tof_histogram_data;
}

std::vector<Datapoint> * cFrequencies::get_histogram_of_class(
	int class_numbero, bool shift_with_mean_tof)
{
	tof_histogram_data_for_one_class.clear();
	// First we create an array of zeroes.
	for (int i=0; i <= getMaxBins();i++)  //Make it 1 bigger..
	{
		tof_histogram_data_for_one_class.push_back(Datapoint(i,0,0,0));
		if (shift_with_mean_tof) //Increasing the table size; it could be that once shifting around will make something outrange
			tof_histogram_data_for_one_class.push_back(Datapoint(i,0,0,0));
	}
	
	for (int d=0;d < frequencynumber;  d++){
		if (frequencies[d]->is_active==false) continue; //LIMITATION
		//frequencies[d]->append_to_cts_vs_bin_histogram(countsit,bin_start,bin_stop,
		//				class_min_cts[class_numbero],class_max_cts[class_numbero],shift_with_mean_tof);
		frequencies[d]->get_histogram_of_class(&tof_histogram_data_for_one_class,bin_start,bin_stop,
						class_min_cts[class_numbero],class_max_cts[class_numbero],shift_with_mean_tof);
	}
	return &tof_histogram_data_for_one_class;
}


//------------------------------------------------------------------------------
double cFrequencies::getAverageCountsInClass(const int class_no, ostream &log){
//	cout << "ahhahrar\n";
	if (class_no > number_of_classes){
		log << "Class does not exist!\n";
		return -1;
	}
	int total_counts(0);
	int weighted(0);
	for (int i = class_min_cts[class_no]; i <= class_max_cts[class_no]; i++){
		total_counts += getTotalCounts(bin_start,bin_stop,i,i,log);
		weighted += getTotalCounts(bin_start,bin_stop,i,i,log)*i;
	}
//	log << "Count in class " << class_no << " " << (weighted*1.0)/(total_counts*1.0) << "\n";
	return (weighted*1.0)/(total_counts*1.0);
}
//------------------------------------------------------------------------------
int cFrequencies::getCountsInClass(const int class_no, ostream &log){
	if (class_no > number_of_classes){
		log << "Class does not exist!\n";
		return -1;
	}
	return getTotalCounts(bin_start,bin_stop,class_min_cts[class_no],class_max_cts[class_no],log);
}
//------------------------------------------------------------------------------
void cFrequencies::getFittedValueAndChisqAdjustedError(int class_no, double &value, double &value_error, int fitvarnumber){
	if (class_no > number_of_classes)
		return;
	value = fitparam[class_no][fitvarnumber];
	if (fit_chisq[class_no] > 1)
		value_error = fitparam_err[class_no][fitvarnumber] * sqrt(fit_chisq[class_no]);
	else
		value_error = fitparam_err[class_no][fitvarnumber];
	return;
}
//------------------------------------------------------------------------------
void cFrequencies::InitializeLinearFit(double slope, bool fslope, bool autoslope,
																			 double constant, bool fconstant, bool autoconstant, int y_variable,ostream &log)
{
	//2006-01-19 this method added. To be able to control linear fit.
	ia2[0] = fslope;
	ia2[1] = fconstant;


	if (autoslope)
		a2[0] = 0.0000016;
	else
		a2[0] = slope;

	if (autoconstant) {
		for (int d=0;d < number_of_classes;d++) {
			if (isClassFitted(d)){
				a2[1] = fitparam[d][0];
				break;
			}
		}
	}
	else {
		a2[0] = constant;
	}

	ndata2=0;
	ma2=2;
	mfit2 = 0;
	for (int i = 0;i<ma2;i++){
		if (ia2[i]) mfit2++;
	}
	for (int d=0;d < number_of_classes;d++) {
		if (isClassFitted(d)){
			// 2007-01-18 modified to have x-axis crossing where there is most counts
			//x2[ndata2] = getAverageCountsInClass(d,log);
			x2[ndata2] = getAverageCountsInClass(d,log) - getWeightedAverageCountsInClasses(log);
			getFittedValueAndChisqAdjustedError(d,y2[ndata2],sig2[ndata2],y_variable);
			ndata2++;
		}
	}

}
//------------------------------------------------------------------------------
bool cFrequencies::FitLinear(double &result_slope, double &result_constant, int y_variable, ostream &log){
	if (ndata2 < 1){
//		log << "No fitted classes! Fit data first!\n";
		return 0;
	}
	if (ndata2 == 1){
		log << "There is only one class. Fit linear to that?!\n";
		return 0;
	}
	int counter(0);
	log.precision(13);
	for (int ab=0;ab < ndata2;ab++){
//		log << "x:" << x2[ab] << "\t\t y:" << y2[ab] << "\t\t err: " << sig2[ab] << "\n";
	}
	log.precision(13);
	for (int j=0; j<100;j++){  //To avoid getting stuck
		alamda2=-1;   //If alamda=-1, starts to fit from previous fit
		for (int i=0;i<30;i++){   //HERE how many iterations
			fitting->mrqmin(x2,y2,sig2,ndata2,a2,ia2,ma2,covar2,alpha2,&chisq2,&alamda2,LINEAR_FIT);
			counter++;
		}
		alamda2=0;
		fitting->mrqmin(x2,y2,sig2,ndata2,a2,ia2,ma2,covar2,alpha2,&chisq2,&alamda2,LINEAR_FIT);
		counter++;
		log.precision(13);
//		log << "slope: " <<a2[0] << " const: " << a2[1];
//		log.precision(5);log << " Chi2/DoF: " << chisq2/(ndata2-mfit2) << endl;
	}
	slope = a2[0];		slope_err = sqrt(covar2[0][0]);
	constant = a2[1];	constant_err = sqrt(covar2[1][1]);
	slope_constant_covariance = covar2[1][0];
	log.precision(13);
	log << "Results:\tslope " << slope << " +- " << sqrt(covar2[0][0]) << "\n";
	log << "\t\tconstant " << constant << " +- " << sqrt(covar2[1][1]) << "\n";
	log << "\t\tCovar[1][0] " << slope_constant_covariance << "\n";
	log << "There were " << counter << " fitting steps\n";
	result_slope = slope;
	result_constant = constant;
	is_countrate_effect_fitted = true;

	return 1;
}
//------------------------------------------------------------------------------
void cFrequencies::remove_fit_things(void){
	nrutil.free_matrix(alpha,0,MAXFITVARIABLES,0,MAXFITVARIABLES);
	nrutil.free_matrix(covar,0,MAXFITVARIABLES,0,MAXFITVARIABLES);
	nrutil.free_ivector(ia,0,MAXFITVARIABLES);
	nrutil.free_vector(chi_vec,0,MAXFITVARIABLES);
	nrutil.free_vector(a,0,MAXFITVARIABLES);
	nrutil.free_vector(sig,0,MAXFREQUENCIES);
	nrutil.free_vector(y,0,MAXFREQUENCIES);
	nrutil.free_vector(x,0,MAXFREQUENCIES);

	nrutil.free_matrix(alpha2,0,2,0,2);
	nrutil.free_matrix(covar2,0,2,0,2);
	nrutil.free_ivector(ia2,0,2);
	nrutil.free_vector(chi_vec2,0,2);
	nrutil.free_vector(a2,0,2);
	nrutil.free_vector(sig2,0,MAXCLASSES);
	nrutil.free_vector(y2,0,MAXCLASSES);
	nrutil.free_vector(x2,0,MAXCLASSES);
}
//------------------------------------------------------------------------------
void cFrequencies::setAllDataNotFitted(void){
	for (int i=0;i<MAXCLASSES;i++){
		is_class_fitted[i]=false;
/*		fit_chisq[i]=-1;
		for (int j=0;j<MAXFITVARIABLES;j++){
			fitparam[i][j]=-1;
			fitparam_err[i][j]=-1;
//			cout << "i:" << i << " j:" << j << "->" << fitparam[i][j] << endl;
		}*/
	}
//	cout << "HERE;\n";
	is_countrate_effect_fitted=false;
	return;
}
//------------------------------------------------------------------------------
double cFrequencies::getInterpolatedCountrateEffectValue(const double interpolate_to,ostream &log){
	if (!is_countrate_effect_fitted){
		log << "Countrate effect not fitted yet!\n";
		return -1;
	}
	//18.1.2007 Modified to have y-axis crossing in average counts
	//return (slope*interpolate_to + constant);
	return (slope*(interpolate_to-getWeightedAverageCountsInClasses(log)) + constant);
}
//------------------------------------------------------------------------------
double cFrequencies::getInterpolatedCountrateEffectValueError(const double interpolate_to,ostream &log){
	if (!is_countrate_effect_fitted){
		log << "Countrate effect not fitted yet!\n";
		return -1;
	}
	double x_deviation = fabs(getWeightedAverageCountsInClasses(log) - interpolate_to);
	return sqrt ( pow(slope_err *x_deviation ,2.0) + pow (constant_err ,2.0));
	/* ----------------------------------------------------*/
	/* The _hopefully_ better way of getting it 2007-01-15 */
	double weight1(0),y_ave(0);
	for (int i=0;i<number_of_classes;i++) { //Getting the average y-value
		if (!isClassFitted(i) )
			continue;
		y_ave		+= w(fitparam_err[i][0])* fitparam[i][0] ;
		weight1	+= w(fitparam_err[i][0]);
	}
	y_ave /= weight1;

	double SSy(0), SSx(0),S_hat(0), Sxy(0);
	for (int i=0;i<number_of_classes;i++) { //Getting the square-deviation from y-value
		if (!isClassFitted(i) )
			continue;
		SSy			+= w(fitparam_err[i][0])* pow( (fitparam[i][0] - y_ave), 2.0);
		SSx			+= w(fitparam_err[i][0])* pow(getAverageCountsInClass(i) - getAverageCountsInClasses(true) , 2.0);
		Sxy			+= w(fitparam_err[i][0])* (fitparam[i][0] - y_ave )* (getAverageCountsInClass(i) - getAverageCountsInClasses(true) );
		S_hat		+= w(fitparam_err[i][0])* pow( fitparam[i][0] - (constant + slope*getAverageCountsInClass(i)),2.0);
	}
/*	SSy		/= weight1;
	SSx		/= weight1;
	Sxy 	/= weight1;
	S_hat	/= weight1;*/
	cout << "uudellainen slope, constant: " << Sxy/SSx << "  " << y_ave - Sxy/SSx*getAverageCountsInClasses(true) << "\n";
	cout << "SSx, SSy " << SSx << "   " << SSy << "\n";
	cout << "slope: " << slope << ", const: " << constant << "\n";
	 //S_hat = SSy - slope*slope * SSx;
	double s_e_squared; //We need the variance; version 1.
	if (getNumberOfFittedClasses() > 2)
		s_e_squared = S_hat / ((getNumberOfFittedClasses() -2)*1.0) ;
	else {
		cout << "2 or less classes!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11 \n";
		s_e_squared = 1e12;
	}
	//We need the variance; version 2.
/*	s_e_squared = 0;
	for (int i=0; i<number_of_classes; i++ ) {
		if (!isClassFitted(i) )
			continue;
		s_e_squared += pow(fitparam_err[i][0],2.0);
	}
	s_e_squared /= getNumberOfFittedClasses()*1.0;
	*/
	double variance_sq =
	//Versio 1, kirja
//				s_e_squared*( 1.0 + 1.0 / (getNumberOfFittedClasses()*1.0)
//					+ pow( interpolate_to - getAverageCountsInClasses(true) ,2.0) / SSx );
	//Versio 2, originin manuaali
			1.963*sqrt(s_e_squared*(1.0/weight1 + pow( interpolate_to - getAverageCountsInClasses(true) ,2.0)/ SSx ));
	return variance_sq;
}
//------------------------------------------------------------------------------
//Returns how many classes has been fitted. 2007-01-16.
int cFrequencies::getNumberOfFittedClasses(void) {
	int adder(0);
	for (int i=0; i<number_of_classes;i++) {
		if (isClassFitted(i) )
			adder++;
	}
	return adder;
}
//------------------------------------------------------------------------------
//This method was renamed 2007-01-16. Thinking, it should not be weighted by counts!
// If there is less counts, then y-value is less precise...
double cFrequencies::getWeightedAverageCountsInClasses(ostream &log){
	double weight(0.0), weightsum(0.0);
	for (int i=0;i<number_of_classes;i++){
		weight += getCountsInClass(i,log);
		weightsum += getAverageCountsInClass(i,log)*getCountsInClass(i,log);
	}
//	log << "Weighted average of all classes [counts/bunch] " << (weightsum/weight) << "\n";
	return weightsum/weight;
}
//------------------------------------------------------------------------------
//This method added 2007-01-16.
// Average number of ions in the whole data.
double cFrequencies::getAverageCountsInClasses(bool only_for_fitted_classes, ostream &log){
	double sum(0.0);
	int this_many(0);
	for (int i=0;i<number_of_classes;i++){
		if ( isClassFitted(i) && only_for_fitted_classes ) {
			this_many++;
			sum += getAverageCountsInClass(i,log);
		}
	}
	//log << "Average # ions of all classes "<< (sum/(this_many*1.0)) << "\n";
	return sum/(this_many*1.0);
}
//------------------------------------------------------------------------------
string cFrequencies::getAverageTime(void){
	loc_tm beg(start_time),end(end_time);
	return end.AverageWith(beg);
}

//------------------------------------------------------------------------------
string cFrequencies::outputAllFittedThings(const string &outdir,const string &file_name,
		const string element_name, int scan_number, bool overwrite, std::string skipped_rounds, double signal_to_noise,
		int lakuversio)
{
	boost::filesystem::ofstream out;
	if (overwrite)
		out.open( (outdir+file_name).c_str());
	else
		out.open( (outdir+file_name).c_str(), ios::app);
	const char DL='\t';
	out.precision(13);
	for (int i=0; i<number_of_classes;i++){
		out << element_name << scan_number << DL;
		out << scan_number << DL;
		out << number_of_classes << DL;
		out << i << DL;
		out << getAverageCountsInClass(i) << DL;
		out << class_min_cts[i] << "-" << class_max_cts[i] << DL;
		out << bin_start << "-" << bin_stop << DL;
		out << (bin_start*bin_width) << "-" << (bin_stop*bin_width) << DL;
		out << getCountsInClass(i) << DL;
		out << getTotalCounts(bin_start,bin_stop,cts_start,cts_stop) << DL;
		out << fitparam[i][0] << DL;
		if (fit_chisq[i] > 1)
			out << (fitparam_err[i][0])*sqrt(fit_chisq[i]) << DL;
		else
			out << (fitparam_err[i][0]) << DL;
		out << linear_extrapolation_x << DL;
		out << getInterpolatedCountrateEffectValue() << DL;
		out << getInterpolatedCountrateEffectValueError()  << DL;
		out << fit_chisq[i] << DL;
		out << getAverageTime() << DL;
		//T_RF
		if ( is_fit_func_type_konig_type(tof_function_to_be_fitted) ) {
		  out << fitparam[i][6] << DL;
		  out << fitparam_err[i][6] << DL;
		}
		else if ( is_fit_func_type_ramsey_type(tof_function_to_be_fitted) ){
		  out << fitparam[i][3] << DL;
		  out << fitparam_err[i][3] << DL;
		}
		//pressure
		if ( is_fit_func_type_konig_type(tof_function_to_be_fitted) ) {
		  out << fitparam[i][1] << DL;
		  out << fitparam_err[i][1] << DL;
		}
		if ( tof_function_to_be_fitted == SINGLE_RAMSEY || tof_function_to_be_fitted == SINGLE_RAMSEY_WITH_CRAP ){
		  out << "-1" << DL;
		  out << "-1" << DL;
		}
		else if ( tof_function_to_be_fitted == SINGLE_RAMSEY_WITH_DAMPING) {
		  out << fitparam[i][6] << DL;
		  out << fitparam_err[i][6] << DL;
		}
		//axial E
		if ( is_fit_func_type_konig_type(tof_function_to_be_fitted) ) {
		  out << fitparam[i][2] << DL;
		  out << fitparam_err[i][2] << DL;
		}
		else if ( is_fit_func_type_ramsey_type(tof_function_to_be_fitted) ){
		  out << fitparam[i][1] << DL;
		  out << fitparam_err[i][1] << DL;
		}
		//magnetron R_0
		if ( is_fit_func_type_konig_type(tof_function_to_be_fitted) ) {
		  out << fitparam[i][3] << DL;
		  out << fitparam_err[i][3] << DL;
		}
		else if ( is_fit_func_type_ramsey_type(tof_function_to_be_fitted) ){
		  out << fitparam[i][2] << DL;
		  out << fitparam_err[i][2] << DL;
		}
		//cyclotron R_0
		if ( is_fit_func_type_konig_type(tof_function_to_be_fitted) ) {
		  out << fitparam[i][4] << DL;
		  out << fitparam_err[i][4] << DL;
		}
		else if ( is_fit_func_type_ramsey_type(tof_function_to_be_fitted) ){
		  out << "-1" << DL;
		  out << "-1" << DL;
		}
		//Phase
		if ( is_fit_func_type_konig_type(tof_function_to_be_fitted) ) {
		  out << fitparam[i][5] << DL;
		  out << fitparam_err[i][5] << DL;
		}
		else if ( is_fit_func_type_ramsey_type(tof_function_to_be_fitted) ){
		  out << "-1" << DL;
		  out << "-1" << DL;
		}
		//A_RF 2016-03-24-> Conversion
		if ( is_fit_func_type_konig_type(tof_function_to_be_fitted) ) {
		  out << fitparam[i][7] << DL;
		  out << fitparam_err[i][7] << DL;
		}
		else if ( is_fit_func_type_ramsey_type(tof_function_to_be_fitted) ){
		  out << fitparam[i][5] << DL;
		  out << fitparam_err[i][5] << DL;
		}

		// Output which parameter was fitted and which not
		out << 'X'; // excel import fixer char
		for (int j=0;j<ma;j++){
			if (was_param_fitted[i][j])
				out << '1';
			else
				out << '0';
		}
		out << DL;
		out << number_of_bunches_in_one_spectrum << DL;
		out << getTotalBunchNumber() << DL;
		out << mass << DL;
		
		// Countrate effect
		if (!is_countrate_effect_fitted){
			out << -1 << DL;	//slope
			out << -1 << DL;	//slope err
			out << -1 << DL;	//const
			out << -1 << DL;	//const err
		}
		else {
			out << slope << DL; //
			out << slope_err << DL;
			out << constant << DL;
			out << constant_err << DL;
		}
		//Here comes two peak fit stuff
		if (number_of_peaks_to_fit > 1) {
			out << fitparam[i][8] << DL;
			if (fit_chisq[i] > 1)
				out << (fitparam_err[i][8])*sqrt(fit_chisq[i]) << DL;
			else
				out << (fitparam_err[i][8]) << DL;
			out << fitparam[i][9] << DL;  //fraction
			out << fitparam_err[i][9] << DL; //fraction

		}
		else {
			out << "-1" << DL << "-1" << DL << "-1" << DL << "-1" << DL;
		}
		//out << "\n";
		//2007-08-06 Adding info on about what statistics calculation method was used
		if (stat_mode == INDIVIDUAL_STATISTICS)
			out << "individual_stat" << DL;
		if (stat_mode == SUM_STATISTICS)
			out << "sum_stat" << DL;
		if (stat_mode == SUM_STATISTICS2)
			out << "sum_stat_with_mean_corrected" << DL;
		out << "\"" << skipped_rounds << "\"" << DL;
		out << signal_to_noise << DL;
		out << charge << DL;
		out << get_fit_function_type_as_text(tof_function_to_be_fitted) << DL;
		if (is_crap_with_constant_tof_used_in_fitting) {
			if (tof_function_to_be_fitted == SINGLE_PEAK_WITH_CRAP) {
				out << fitparam[i][8] << DL;
				out << fitparam_err[i][8] << DL;
				out << fitparam[i][9] << DL;
				out << fitparam_err[i][9] << DL;
			}
			else if (tof_function_to_be_fitted == SINGLE_RAMSEY_WITH_CRAP) {
				out << fitparam[i][6] << DL;
				out << fitparam_err[i][6] << DL;
				out << fitparam[i][7] << DL;
				out << fitparam_err[i][7] << DL;
			}
		}
		else {
			out << "x" << DL << "x" << DL << "x"  << DL << "x" << DL;
		}
		//out << lakuversio << DL;
		out << GIT_VERSION << DL;
		//2016-03-22 Freq3 stuff
		//Here comes three peak fit stuff
		if (number_of_peaks_to_fit > 2) {
			out << fitparam[i][10] << DL;
			if (fit_chisq[i] > 1)
				out << (fitparam_err[i][10])*sqrt(fit_chisq[i]) << DL;
			else
				out << (fitparam_err[i][10]) << DL;
			out << fitparam[i][11] << DL;  //fraction of freq3
			out << fitparam_err[i][11] << DL; //fraction freq3 err

		}
		else {
			out << "-1" << DL << "-1" << DL << "-1" << DL << "-1" << DL;
		}
		out << "\n"; //End of class
	}
	out << "\n"; //Makes empty line between different files

	out.close();
	return "";
}
//------------------------------------------------------------------------------
void cFrequencies::getLinearFitErrors(double &sloup_err, double &konstant_err, double &covariance, ostream &log)
{
	if (!is_countrate_effect_fitted){
		log << "Countrate effect not fitted yet! (trying to get lin_fit_errors)\n";
		return;
	}
	covariance = slope_constant_covariance;
	sloup_err = slope_err; konstant_err = constant_err;
	return;
}
//------------------------------------------------------------------------------
double cFrequencies::getInterpolatedCountrateEffectValue(ostream &log){
	if (!is_countrate_effect_fitted){
		log << "Countrate effect not fitted yet! (trying to get extrapolated countrate eff)\n";
		return -1;
	}
	return getInterpolatedCountrateEffectValue(linear_extrapolation_x);
	}
//------------------------------------------------------------------------------
double cFrequencies::getInterpolatedCountrateEffectValueError(ostream &log){
	if (!is_countrate_effect_fitted){
		log << "Countrate effect not fitted yet! (trying to get extrapolated countrate eff_err)\n";
		return -1;
	}
	return getInterpolatedCountrateEffectValueError(linear_extrapolation_x);
}
//------------------------------------------------------------------------------
void cFrequencies::printContentOfFrequency(int freq_no, ostream &log){
	if (freq_no > frequencynumber){
		log << "invalid freq\n";
		return;
	}
	frequencies[freq_no]->print_contents_of_this_class(log);
	return;
}
//------------------------------------------------------------------------------
/*int cFrequencies::getEventsAsTable(int events[], int max_events){
	for (int i=0;i<frequencynumber;i++){
		if (frequencies[i]->is_active==false) continue; //LIMITATION
		for (int j=0;j<=max_events;j++){
			events[j] += frequencies[i]->getEventNumber(0,max_bin_number,j);
		}
	}
	return 1;
}*/
//------------------------------------------------------------------------------
int cFrequencies::getEventsAsTable(vector<int> &in_here) {
	int mex_ev=getMaxEvents();
	if (frequencynumber < 1)
		return -1;
	in_here.clear();
	for (int i=0; i <= mex_ev;i++)  //Make it 1 bigger..
		in_here.push_back(0);
	for (int i=0;i<frequencynumber;i++){
		if (frequencies[i]->is_active==false) continue; //LIMITATION
		for (int j=0;j<=mex_ev;j++){
			in_here[j] += frequencies[i]->getEventNumber(0,max_bin_number,j);
		}
	}
	return 1;
}
//------------------------------------------------------------------------------
int cFrequencies::getMaxEvents(void){
	int current_maximum(0);
	for (int i=0;i<frequencynumber;i++){
		if (frequencies[i]->is_active==false) continue; //LIMITATION
/*		for (int j=0;j<search_maximum;j++){
			if (frequencies[i]->getEventNumber(0,max_bin_number,j) > 0)
				if (j > current_maximum)
					current_maximum = j;
		}*/
		//9.8.2006: Change to get real maximum
		if (frequencies[i]->get_maximum_counts_in_bunch() > current_maximum)
			current_maximum = frequencies[i]->get_maximum_counts_in_bunch();
	}
//	cout << "CUrrent max:" << current_maximum << "sdfsgf \n";
	return current_maximum;
}
//------------------------------------------------------------------------------
/*
bool cFrequencies::getCountsOfFrequencies(double counts[], double freq[], int &max_frequencies,int startbin,int stopbin,int min_cts, int max_cts){
	for (int i=0; i < frequencynumber;i++){
		if (frequencies[i]->is_active==false) continue; //LIMITATION
		if (i > max_frequencies)
			break;
		counts[i]=frequencies[i]->getCounts(startbin,stopbin,min_cts,max_cts)*1.0;
		freq[i]=frequencies[i]->getFrequency();
	}
	max_frequencies = frequencynumber;
	return 1;
}
*/
//------------------------------------------------------------------------------
/*
bool cFrequencies::getCountsOfFrequencies(DATASET &dataset, int startbin, int stopbin, int min_cts, int max_cts) {
	Datapoint toAdd;
	for (int i=0; i < frequencynumber;i++){
		if (frequencies[i]->is_active==false) continue; //LIMITATION
		toAdd.y_value=frequencies[i]->getCounts(startbin,stopbin,min_cts,max_cts)*1.0;
		toAdd.y_value_err = sqrt(toAdd.y_value);
		toAdd.x_value=frequencies[i]->getFrequency();
		dataset.data.push_back(toAdd);
	}
	return 1;
}
*/
//------------------------------------------------------------------------------
/*
bool cFrequencies::getCountsOfClass(double counts[], double freq[], int &max_frequencies,int class_no){
	if (class_no > number_of_classes)
		return 0;
	if (class_no < 0)
		return 0;
	return getCountsOfFrequencies(counts,freq,max_frequencies,bin_start,bin_stop, class_min_cts[class_no], class_max_cts[class_no]);
}
*/
//------------------------------------------------------------------------------
/*
bool cFrequencies::getCountsOfClass(DATASET &dataset, int class_no) {
	if (class_no > number_of_classes)
		return 0;
	return getCountsOfFrequencies(dataset,bin_start,bin_stop, class_min_cts[class_no], class_max_cts[class_no]);
}
*/
//------------------------------------------------------------------------------
int cFrequencies::getLowerEventBoundary(int class_no){
	if (class_no > number_of_classes)
		return -1;
	return class_min_cts[class_no];

}
//------------------------------------------------------------------------------
int cFrequencies::getUpperEventBoundary(int class_no){
	if (class_no > number_of_classes)
		return -1;
	return class_max_cts[class_no];
}
//------------------------------------------------------------------------------
int cFrequencies::setLowerEventBoundary(int class_no,int value){
	if (class_no > number_of_classes)
		return 3;
	class_min_cts[class_no]=value;
	return 1;

}
//------------------------------------------------------------------------------
int cFrequencies::setUpperEventBoundary(int class_no,int value){
	if (class_no > number_of_classes)
		return 3;
	class_max_cts[class_no]=value;
	return 1;
}
//------------------------------------------------------------------------------
void cFrequencies::deleteData(void){
	for (int i=0;i<frequencynumber;i++){
		delete frequencies[i];
	}
	frequencynumber=0;
	number_of_classes = 0;
	setAllDataNotFitted();
	return;
}
//------------------------------------------------------------------------------
bool cFrequencies::isClassFitted(int class_number)
{
	if (class_number > number_of_classes) {
		std::cout << "Class does not exist! (asking whether class is fitted)\n";
		return false;
	}
	return is_class_fitted[class_number];
}
//------------------------------------------------------------------------------
void cFrequencies::setBinAndCountWindowManually(int BinMin,int BinMax,int CtsMin,int CtsMax){
	bin_start = BinMin; bin_stop = BinMax;
	cts_start = CtsMin; cts_stop = CtsMax;
}

//------------------------------------------------------------------------------
void cFrequencies::FitGaussian(	double range_min,double range_max, int class_used,
								double ini_f, double ini_fwhm, double ini_h, double ini_bgr,
								bool   fit_f, bool fit_fwhm, bool fit_h, bool fit_bgr,
								double &res_f, double &res_fwhm, double &res_h, double &res_bgr,
								double &dres_f, double &dres_fwhm, double &dres_h, double &dres_bgr,
								double &chisqblub, DATASET &outdata,int outpoints_wanted){
	//Initialise
	double *x3=nrutil.vector(0,frequencynumber+1);
	double *y3=nrutil.vector(0,frequencynumber+1);
	double *sig3=nrutil.vector(0,frequencynumber+1);
	double *a3=nrutil.vector(0,frequencynumber+1);
	double *chi_vec3=nrutil.vector(0,4);
	int *ia3=nrutil.ivector(0,4);
	double **covar3=nrutil.matrix(0,4,0,4);
	double **alpha3=nrutil.matrix(0,4,0,4);
	double chisq3, alamda3;
	int ndata3,ma3,mfit3;


	a3[0] = ini_f;			ia3[0] = fit_f;
	a3[1] = ini_fwhm;		ia3[1] = fit_fwhm;
	a3[2] = ini_h;			ia3[2] = fit_h;
	a3[3] = ini_bgr;		ia3[3] = fit_bgr;
//	cout << "fits: " <<ia3[0] << ia3[1] << ia3[2] << ia3[3] << "\n";
	ndata3=0;
	ma3=4;
	mfit3 = 0;
	int counter(0);
	for (int i = 0;i<ma3;i++){
		if (ia3[i]) mfit3++;
	}
	for (int d=0;d < frequencynumber;d++) {
		if ( (frequencies[d]->getFrequency() >= range_min) && (frequencies[d]->getFrequency() <= range_max)){
			x3[ndata3] = frequencies[d]->getFrequency();
			y3[ndata3] = frequencies[d]->getCounts(bin_start,bin_stop,class_min_cts[class_used],class_max_cts[class_used]);
			sig3[ndata3] = sqrt(y3[ndata3]);
			if (y3[ndata3] == 0)
				ndata3--;
			ndata3++;
		}
	}
	ostream &log = cout;
	if (ndata3 < 1){
		log << "No countspoints\n";
		return;
	}
	if (ndata3 == 1){
		log << "There is only one point. Fit gaussian to that?!\n";
		return;
	}
	log.precision(13);
//	for (int ab=0;ab < ndata3;ab++){
//		log << "x:" << x3[ab] << "\t\t y:" << y3[ab] << "\t\t err: " << sig3[ab] << "\n";
//	}
	log.precision(13);
	for (int j=0; j<40;j++){  //To avoid getting stuck
		alamda3=-1;   //If alamda=-1, starts to fit from previous fit
		for (int i=0;i<5;i++){   //HERE how many iterations
			fitting->mrqmin(x3,y3,sig3,ndata3,a3,ia3,ma3,covar3,alpha3,&chisq3,&alamda3,GAUSSIAN);
			counter++;
		}
		alamda3=0;
		fitting->mrqmin(x3,y3,sig3,ndata3,a3,ia3,ma3,covar3,alpha3,&chisq3,&alamda3,GAUSSIAN);
//		fitting->mrqmin(x, y, sig, ndata, a, ia, ma, covar, alpha, &chisq, &alamda, SINGLE_PEAK_FIT);
		counter++;
		log.precision(13);
//		log << "slope: " <<a2[0] << " const: " << a2[1];
	}
/*	log.precision(5);
	log << "\n Chi2/DoF: " << chisq3/(ndata3-mfit3) << endl;
	log.precision(13);
	log << "Results:\t 0 " << a3[0] << " +- " << sqrt(covar3[0][0]) << "\n";
	log << "\t\t 1 " << a3[1] << " +- " << sqrt(covar3[1][1]) << "\n";
	log << "\t\t 2 " << a3[2] << " +- " << sqrt(covar3[2][2]) << "\n";
	log << "\t\t 3 " << a3[3] << " +- " << sqrt(covar3[3][3]) << "\n";*/
	log << "There were " << counter << " fitting steps\n";

	res_f = a3[0],  res_fwhm = a3[1],  res_h = a3[2],  res_bgr = a3[3],
	dres_f=sqrt(covar3[0][0]),  dres_fwhm=sqrt(covar3[1][1]),  dres_h=sqrt(covar3[2][2]),  dres_bgr=sqrt(covar3[3][3]);
	chisqblub = chisq3/(ndata3-mfit3);

	double jyy; //This is the y-coordinate [cts]; aks is the x-coordinate [Hz]
	int point(0);
	outdata.data.clear();
	Datapoint toAdd;
	for (double aks = range_min; aks <= range_max; aks += (range_max-range_min)/(outpoints_wanted+1.0) ) {
		if (point > outpoints_wanted) {
			break;
		}
    	fit_func->funcs(aks,a3,&jyy,chi_vec,0,GAUSSIAN);
		toAdd.x_value = aks;
		toAdd.y_value = jyy;
		outdata.data.push_back(toAdd);
		point++;
	}







	nrutil.free_matrix(alpha3,0,4,0,4);
	nrutil.free_matrix(covar3,0,4,0,4);
	nrutil.free_ivector(ia3,0,4);
	nrutil.free_vector(chi_vec3,0,4);
	nrutil.free_vector(a3,0,4);
	nrutil.free_vector(sig3,0,frequencynumber+1);
	nrutil.free_vector(y3,0,frequencynumber+1);
	nrutil.free_vector(x3,0,frequencynumber+1);
}
//------------------------------------------------------------------------------
FIT_FUNC_TYPE cFrequencies::get_fit_func_type(void) {
	return tof_function_to_be_fitted;
}
//------------------------------------------------------------------------------
vector<CTS_HISTOGRAM_OF_FREQ> * cFrequencies::get_2d_contour_of_counts
    (int class_no, double freq_min, double freq_max, int tof_min_in_bins, int tof_max_in_bins, int tof_interval_in_bins)
{
	two_dim_contour_of_counts.clear();
	if (frequencynumber == 0)
		return &two_dim_contour_of_counts;
	CTS_HISTOGRAM_OF_FREQ toAdd;
	toAdd.start_tof = (tof_min_in_bins+MCS_recording_offset_in_bins)*bin_width;
	//std::cout << "MCS offse: " << MCS_recording_offset_in_bins << "\n";
	//std::cout << "start_tof: " << toAdd.start_tof << "\n";
	toAdd.tof_interval = tof_interval_in_bins * bin_width;
	int no_of_valid_freqs(0);
	for (int i=0; i < frequencynumber; i++ ) {
		//Check if freq is not in the interval
		if (frequencies[i]->getFrequency() < freq_min || frequencies[i]->getFrequency() > freq_max )
			continue;
		no_of_valid_freqs++;
		toAdd.center_freq = frequencies[i]->getFrequency();

		//First freq point gets treated differently
		if (no_of_valid_freqs == 1) {
			toAdd.half_distance_to_left_freq =  (frequencies[i]->getFrequency() - freq_min ) /2.;
		}
		else {
			toAdd.half_distance_to_left_freq =  (frequencies[i]->getFrequency() -
																						frequencies[i-1]->getFrequency() ) /2.;
			two_dim_contour_of_counts.back().half_distance_to_right_freq = toAdd.half_distance_to_left_freq;
		}
		//
		for ( int toffi=tof_min_in_bins; toffi <= tof_max_in_bins; toffi += tof_interval_in_bins) {
			toAdd.counts_in_interval.push_back (
				frequencies[i]->getCounts(bin_start,bin_stop,toffi,toffi+tof_interval_in_bins-1,class_min_cts[class_no],class_max_cts[class_no])
			);
		}
		two_dim_contour_of_counts.push_back(toAdd);
		toAdd.counts_in_interval.clear();
	}
	//Last freq point gets treated differently
	if (!two_dim_contour_of_counts.empty()) {
		two_dim_contour_of_counts.back().half_distance_to_right_freq =
	         (freq_max - frequencies[frequencynumber-1]->getFrequency() ) / 2.;
	}
	return &two_dim_contour_of_counts;
}
//------------------------------------------------------------------------------
void cFrequencies::print_2d_counts(ostream &here, int class_no) {
	vector<CTS_HISTOGRAM_OF_FREQ> * contour = get_2d_contour_of_counts(class_no,
														-100,1e11,0,max_bin_number,bin_interval_in_2d_contour);

	if (contour->empty())
		return;
	for (vector<CTS_HISTOGRAM_OF_FREQ>::iterator i= contour->begin(); i != contour->end(); ++i) {
		i->pretty_print(here); here << "\n";
	}
}
//------------------------------------------------------------------------------
#ifdef HAVE_ONLINE
int cFrequencies::insert_online_bunchpack(const std::list<Bunch_Data> &data) {
	if (bin_width < 0 || max_bin_number < 0) {
		popupper->set_message("No headers to set MCS bin_width and\n max bin number!\n\nSetting to 0.640 and 2048");
		// Gtk::Justification keskitys = popupper->dialog_head->get_justify();
		// popupper->dialog_head->set_justify(Gtk::JUSTIFY_LEFT);
		popupper->run();
		popupper->hide();
		// popupper->dialog_head->set_justify(keskitys);
		set_MCS_data(2048,5);
	}
	std::list<Bunch_Data>::const_iterator foo;
	int this_big_bunchpack(0);
	for(foo = data.begin(); foo != data.end(); foo++) {
		cUnit tobeadded;
		this_big_bunchpack++;
		tobeadded.putFrequency(atof(foo->frequency.c_str()));
//		cout << "Adding freq: " << foo->frequency_mhz/1000. << "\n";
		tobeadded.putTimeString(foo->timestamp);
//		std::cout << "Seqno: " << foo->seqno << std::endl;
//		std::cout << "Frequency: " << foo->frequency_mhz << std::endl;
//		std::cout << "Timestamp: " << foo->timestamp << std::endl;

		std::list<Bin_Data>::const_iterator bar;
		cDatapoint toAdd;
		for(bar = foo->data.begin(); bar != foo->data.end(); bar++) {
			toAdd.setValues(bar->bin,bar->count,1,foo->timestamp);
			tobeadded.InsertDataPoint(toAdd);
			/*std::cout << "  (bin, count) = ("
								<< bar->bin << ", "
								<< bar->count << ")" << std::endl;*/
		}

		if ( searchForFrequencyMatch(atof(foo->frequency.c_str())) == -1 )
			addFrequencyUnit(tobeadded);
		else {
		    frequencies[searchForFrequencyMatch(atof(foo->frequency.c_str()))]->addNewBunch(tobeadded,cout);
		}
	}
	order_data();
	return this_big_bunchpack;
}
#endif
//------------------------------------------------------------------------------
#ifdef HAVE_ONLINE
void cFrequencies::set_MCS_data(const int max_binss, const int bin_widthh_as_set_on_MCS) {
	max_bin_number = max_binss;
	bin_width = 0.020*pow(2.0,bin_widthh_as_set_on_MCS);
	if (bin_widthh_as_set_on_MCS == 0)
		bin_width = 0.005;
	//bin_width *= 1.0e-6;
	//cout << "BIn width: " << bin_width << "\n";
}
#endif
//------------------------------------------------------------------------------
void cFrequencies::get_cts_vs_bin_histogram_of_class(vector<int> & countsit,
                   int class_numbero, bool shift_with_mean_tof)
{
	countsit.clear();
	for (int i=0; i <= max_bin_number; i++) {
		countsit.push_back(0);
		if (shift_with_mean_tof) //Increasing the table size; it could be that once shifting around will make something outrange
			countsit.push_back(0);
	}

	for (int d=0;d < frequencynumber;  d++){
		if (frequencies[d]->is_active==false) continue; //LIMITATION
		frequencies[d]->append_to_cts_vs_bin_histogram(countsit,bin_start,bin_stop,
						class_min_cts[class_numbero],class_max_cts[class_numbero],shift_with_mean_tof);
	}
}


//------------------------------------------------------------------------------




//------------------------------------------------------------------------------
double cFrequencies::get_average_tof_of_all_ions_in_class(int class_numbero,bool shift_with_mean_tof) {
	vector<int> histo;
	get_cts_vs_bin_histogram_of_class(histo,class_numbero, shift_with_mean_tof);
	int tot_cts(0), tot_cts_x_bin(0), countteri(0);
	for (vector<int>::iterator i=histo.begin(); i != histo.end(); i++) {
		tot_cts += *i;
		tot_cts_x_bin += countteri*(*i);
		countteri++;
	}
	return (tot_cts_x_bin*1.0)/(tot_cts*1.0);
}
//------------------------------------------------------------------------------
double cFrequencies::get_sd_of_all_ions_in_class(int class_numbero, bool shift_with_mean_tof) {
	double averageTOF = get_average_tof_of_all_ions_in_class(class_numbero, shift_with_mean_tof);
	vector<int> histo;	get_cts_vs_bin_histogram_of_class(histo,class_numbero, shift_with_mean_tof);

	//Printing histo to file, should not be needed
	//
	/*
	int summ(0),k(0),l(0);
	boost::filesystem::ofstream file;
	file.open("/tmp/sum_statti");
	file << "#averageTOF " << averageTOF << "\n";
	for (vector<int>::iterator ii=histo.begin(); ii != histo.end(); ii++) {
		summ += *ii;
		k++; l++;
		if (k == 5) {
			file << l << "\t" << summ << "\n";
			k = 0; summ = 0;
		}
	}
	file.close();
	//
	//
	*/
	int tot_cts(0), countteri(0); double SumDevSq(0);
	for (vector<int>::iterator ii=histo.begin(); ii != histo.end(); ii++) {
		tot_cts += *ii;
		SumDevSq += pow((averageTOF-countteri*1.0),2)*(*ii);
		countteri++;
	}
	if (tot_cts <= 1)
		return -1;
	//cout << "standard deviation: " << sqrt(SumDevSq / ((tot_cts-1)*1.0) ) << "\n";
	return sqrt(SumDevSq / ((tot_cts-1)*1.0) );

}
//------------------------------------------------------------------------------
double cFrequencies::get_time_of_flight_error(int freq_index, int class_number, double total_sd) {
	if (stat_mode == INDIVIDUAL_STATISTICS) {
		return frequencies[freq_index]->getTimeOfFlightError(bin_start,bin_stop,class_min_cts[class_number],class_max_cts[class_number])*bin_width;
	}
	if ( total_sd < 0) {
		if (stat_mode == SUM_STATISTICS) {
			total_sd = get_sd_of_all_ions_in_class(class_number, false);
		}
		if (stat_mode == SUM_STATISTICS2) {
			total_sd = get_sd_of_all_ions_in_class(class_number, true);
		}
	}
	if (stat_mode == SUM_STATISTICS || stat_mode == SUM_STATISTICS2) {
		return total_sd / sqrt(frequencies[freq_index]->getCounts(bin_start,bin_stop,class_min_cts[class_number],class_max_cts[class_number])*1.0)*bin_width;
	}
	return -1;
}
//------------------------------------------------------------------------------
double cFrequencies::get_sd_of_all_ions_in_class(int class_numbero, STATISTICS_MODE statist_mode) {
	if (stat_mode == SUM_STATISTICS) {
		return get_sd_of_all_ions_in_class(class_numbero, false);
	}
	if (stat_mode == SUM_STATISTICS2) {
		return get_sd_of_all_ions_in_class(class_numbero, true);
	}

	return -1;
}
//------------------------------------------------------------------------------
int cFrequencies::set_bunches_that_will_be_skipped(const vector<int> &skipping_these_bunches) {
	int this_many(0);
	if (skipping_these_bunches.empty())
		return 0;
	for (vector<int>::const_iterator ii = skipping_these_bunches.begin(); ii != skipping_these_bunches.end(); ++ii) {
		for (int i=0; i < frequencynumber; i++) {
			this_many += frequencies[i]->set_bunch_not_active(*ii);
		}
	}
	return this_many;
}
//------------------------------------------------------------------------------
void cFrequencies::form_a_history_of_counts(int startbin, int stopbin, int min_ctss, int max_ctss, std::vector<int> &in_here, int sum_up_this_many_bunches, std::vector<int> & round_changing_points) {
	//In case everything is wanted, give startbin and stopbin 0)
	//Also min_ctss, max_ctss == 0 both is special case
	int number_of_summed_bunches_now(0), counts_now(0), bunch_number_now(0);
	order_data();
	in_here.clear(); round_changing_points.clear();
	if (frequencynumber == 0)
		return;
	int how_many_rounds_there_is = frequencies[0]->getTotalBunchNumber();
	for (int roundsit=1; roundsit <= how_many_rounds_there_is; roundsit++) {
		round_changing_points.push_back(in_here.size());
		for (int i=0; i < frequencynumber; i++) {
			counts_now += frequencies[i]->getCounts(startbin,stopbin,roundsit, min_ctss, max_ctss);
			number_of_summed_bunches_now ++;
			if (number_of_summed_bunches_now == sum_up_this_many_bunches) {
				in_here.push_back(counts_now);
				counts_now = 0;
				number_of_summed_bunches_now = 0;
			}
		}
	}
	in_here.push_back(0);
}
//------------------------------------------------------------------------------
void cFrequencies::set_all_bunches_active() {
	for (int i=0; i < frequencynumber; i++) {
		frequencies[i]->set_all_bunches_active();
	}
}
//------------------------------------------------------------------------------
bool cFrequencies::does_all_freqs_have_same_amount_of_bunches() {
	if (frequencynumber == 0)
		return 1;
	int dooo=frequencies[0]->getTotalBunchNumber();
	for (int i=0; i < frequencynumber; i++) {
		if (dooo != frequencies[i]->getTotalBunchNumber() )
			return false;
		dooo = frequencies[i]->getTotalBunchNumber();
	}
	return true;
}
//------------------------------------------------------------------------------
int cFrequencies::getNumberOfRoundsInFile_v4(const boost::filesystem::path bunchfilename, bool force, ostream& log){
	if (!is_this_v4_bunchfile(bunchfilename))
		return -2;
	if (force)
		number_of_rounds_in_file_checked = false;
	if (data_file == (bunchfilename) ) {
		if (number_of_rounds_in_file_checked) {
//			cout << number_of_rounds_in_file << "==roundeja tsekkaamatta.\n";
			return number_of_rounds_in_file;
		}
	}

	boost::filesystem::ifstream openedfile;
	openedfile.open((bunchfilename).c_str());
  	if (!openedfile){
		//cout << "File " << dirname << filename << " doesn't open/exist!\n(tried to read number of rounds in)\n";
		number_of_bunches_in_one_spectrum = -1;
		number_of_rounds_in_file_checked=false;
		return -1;
	}
	string row, fields[10];
	bool eka_found = false;
	string eka_freq = "";
	int countt(0);
	while (getline(openedfile,row)) {
		if (row[0] != 'B')
			continue;
		int n(1);
		split_feb09_datarow_to_fields(row,fields,n);

		if (eka_found == false) {
			eka_freq = fields[2];
			//cout << "eka freq: " << eka_freq << "\n";
			eka_found = true;
		}
		if (fields[2] == eka_freq) {
			//cout << "match\n";
			countt++;
		}
	}
	openedfile.close();
	//cout << countt << " verran taajuutta.\n";
	data_is_in_single_bunch_mode = is_data_collected_in_single_bunch_mode_v4(bunchfilename,log);

	data_file = bunchfilename; number_of_rounds_in_file_checked = true;
	number_of_rounds_in_file = countt;
	return countt;
}

//------------------------------------------------------------------------------
//Reading since feb 2009
string cFrequencies::readDirectory_v4(const boost::filesystem::path bunchfilename, ostream &log) {
	if (!is_this_v4_bunchfile(bunchfilename))
		return "This is not v4 bunchfile:\n"+bunchfilename.string() +"\n";
	double scan_start_time(0);

	boost::filesystem::ifstream filex;
	//if (!(open_file_for_reading_and_popup_message_if_error(dirname+filename,filex))) {
	if (!(open_file_for_reading_and_popup_message_if_error(bunchfilename.string(),filex))) {
		return "File " + bunchfilename.string() + " doesn't open/exist!\n(tried to read whole dir)\n";
	}
	string row, fields[10];
	cUnit tobeadded;
	bool on_eka=true;
	while (getline(filex,row)) {
		if ( row[0] == '#' ) {
			continue;
		}
		int n(1);
		split_feb09_datarow_to_fields(row,fields,n);
		if ( row[0] == 'B') {
			//tobeadded.print_contents_of_this_class(std::cout);
			if (!on_eka) {
				add_bunch(tobeadded);
				tobeadded.remove_datas();
			}
			on_eka=false;
			start_time=tobeadded.getTimeString();
		}

		if (fields[0] == "I"){
//				set_I_row(fields,n);
			continue;
		}
		if (fields[0] == "P"){
			set_P_row(fields,n);
			continue;
		}
		if (fields[0] == "T"){
			start_time = fields[1];
			scan_start_time = as_double(fields[2]);
			continue;
		}
		if (fields[0] == "B"){
			tobeadded.putFrequency(as_double(fields[2]));
			tobeadded.putTimeString( loc_tm(floor(as_double(fields[1]) + scan_start_time)).get_time_as_string());
		}
		if (fields[0] == "."){
				cDatapoint toAdd;
				toAdd.setValues(as_int(fields[1]),as_int(fields[2]),1,"");
				tobeadded.InsertDataPoint(toAdd);
		}
	}
	add_bunch(tobeadded);
	end_time=tobeadded.getTimeString();
	cout << "mik� on\n";
	filex.close();
	//bin_width = 0.640e-6;
	//max_bin_number = 1024;
	return "";
}
//------------------------------------------------------------------------------
string cFrequencies::readDirectoryPartly_v4(const boost::filesystem::path bunchfilename, int start_round, int end_round, ostream& log) {

	double scan_start_time(0);

	boost::filesystem::ifstream filex;
	//if (!(open_file_for_reading_and_popup_message_if_error(dirname+filename,filex))) {
	if (!(open_file_for_reading_and_popup_message_if_error(bunchfilename.string(),filex))) {
		return "File " + bunchfilename.string() + " doesn't open/exist!\n(tried to read whole dir)\n";
	}
	string row, fields[10];
	cUnit tobeadded;
	bool on_eka=true;
	bool eka_bunchi_tallennettu=false; //Aikaa varten
	string eka_freq;
	int round_number(0);
	while (getline(filex,row)) {
		if ( row[0] == '#' ) {
			continue;
		}
		if (round_number > end_round+2) {
			cout << "nyt riitti\n";
			break;
		}
		int n(1);
		split_feb09_datarow_to_fields(row,fields,n);
		if (row[0] == 'T'){
			//timebase = fields[1]; //stringi
			scan_start_time = as_double(fields[2]); //seconds since unix epoch, ms precision
			continue;
		}
		if ( row[0] == 'B') {
			//tobeadded.print_contents_of_this_class(std::cout);
			if (on_eka) {
				eka_freq = fields[2];
				round_number++;
			}
			else {
				if (round_number >= start_round && round_number <= end_round) {
					if (!eka_bunchi_tallennettu) {
						start_time=tobeadded.getTimeString();
						eka_bunchi_tallennettu = true;
					}
					add_bunch(tobeadded);
					end_time=tobeadded.getTimeString();
					tobeadded.remove_datas();
				}
				if (fields[2] == eka_freq)
					round_number++;
			}
			on_eka=false;
		}
		if (round_number < start_round || round_number > end_round) {
			if (row[0] == 'B' || row[0] == '.')
				continue;
		}
		if (fields[0] == "I"){
//				set_I_row(fields,n);
			continue;
		}
		if (fields[0] == "P"){
			set_P_row(fields,n);
			continue;
		}
		if (fields[0] == "B"){
			tobeadded.putFrequency(as_double(fields[2]));
			tobeadded.putTimeString( loc_tm(floor(as_double(fields[1]) + scan_start_time)).get_time_as_string());
		}
		if (fields[0] == "."){
				cDatapoint toAdd;
				toAdd.setValues(as_int(fields[1]),as_int(fields[2]),1,"");
				tobeadded.InsertDataPoint(toAdd);
		}
	}
	add_bunch(tobeadded);
	cout << "mik� on\n";
	filex.close();
	//bin_width = 0.640e-6;
	//max_bin_number = 1024;
	return "";
}
//------------------------------------------------------------------------------
void cFrequencies::split_feb09_datarow_to_fields(const std::string row, std::string fields[], int & n) {
		fields[0] = row[0];
		string loppuosa = row.substr(3);
		while (loppuosa.find_first_of(",)") != string::npos) {
			int d = loppuosa.find_first_of(",)");
			//cout << "|" << loppuosa.substr(0,d) << "|\n";
			fields[n] = loppuosa.substr(0,d);
			n++;
			loppuosa = loppuosa.substr(d+1);
			if (loppuosa[0] == ' ')
				loppuosa = loppuosa.substr(1);
			//cout << "loppuosa " << loppuosa << "\n";
			if (n > 10)
				break;
		}
		/*cout << "kent�t, n=" << n << ": \n";
		for (int k=0;k<n;k++) {
			cout << "|" << fields[k];
		}
		cout << "|\n";*/
}
//------------------------------------------------------------------------------
void cFrequencies::add_bunch(const cUnit & toadd) {
	if ( searchForFrequencyMatch(toadd.getFrequency()) == -1 ) { //If it is not a new freq, making one new
		addFrequencyUnit(toadd);
	}
	else {
		frequencies[searchForFrequencyMatch(toadd.getFrequency())]->addNewBunch(toadd,std::cout);
	}
}
//------------------------------------------------------------------------------
bool cFrequencies::set_P_row(const std::string fields[], int how_many) {
	if (how_many < 4) {
		cout << "Bad P-row in datafile\n";
		return false;
	}
	if ( fields[1] == "mcs") {
		if (fields[2] == "bin width") {
			bin_width = as_double(fields[3])/1000.;
			cout << "bin width set as " << bin_width << "\n";
			return true;
		}
		else {
			if (fields[2] == "mcs" && fields[3] == "bin width") {
				bin_width = as_double(fields[4])/1000.;
				cout << "bin width set as " << bin_width << "\n";
				return true;
			}
		}
		if (fields[2] == "bins per record") {
			max_bin_number = as_int(fields[3]);
			cout << "bins per record set as " << max_bin_number << "\n";
			return true;
		}
		else {
			if (fields[2] == "mcs" && fields[3] == "bins per record") {
				max_bin_number = as_int(fields[4]);
				cout << "bins per record set as " << max_bin_number << "\n";
				return true;
			}
		}
		if (fields[3] == "records per scan") {
			number_of_bunches_in_one_spectrum = as_int(fields[4]);
			cout << "records per scan set as " << number_of_bunches_in_one_spectrum << "\n";
			return true;
		}
	}
	else {
		//cout << "Not an mcs row in datafile\n";
	}
	for (int i=0;i<how_many;i++) {
		//cout << i << "  |" << fields[i] << "|\n";
	}
	return false;
}
//------------------------------------------------------------------------------
bool cFrequencies::set_I_row(const std::string fields[], int how_many) {

	return false;
}
//------------------------------------------------------------------------------
bool cFrequencies::is_this_v3_bunchfile(const boost::filesystem::path& bunchfilename) {
	boost::filesystem::ifstream iffi;
	iffi.open(bunchfilename.c_str());
	if (!iffi)
		return false;
	string row;
	if (!getline(iffi,row)) {
		iffi.close();
		return false;
	}
	iffi.close();
	//cout << "daa: " << row.substr(0,16) << "\n";
	if (row.substr(0,16) == "# File saved at:") {
		//cout << "This is v3 bunchfile\n";
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
bool cFrequencies::is_this_v4_bunchfile(const boost::filesystem::path& bunchfilename) {
	boost::filesystem::ifstream iffi;
	iffi.open(bunchfilename.c_str());
	if (!iffi)
		return false;
	string row;
	if (!getline(iffi,row)) {
		iffi.close();
		return false;
	}
	iffi.close();
	//cout << "daa: |" << row.substr(0,23) << "|\n";
	if (row.substr(0,23) == "I (version, ascii, 2.0)") {
		//cout << "This is v4 bunchfile\n";
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
bool cFrequencies::is_some_parameter_to_fit_initially_zero()
{
	for (int i = 0;i<ma;i++){
		if (ia[i] == 1 && a[i] == 0)
		  return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------
int cFrequencies::show_eva_data(int number, std::list<cEvaFile *> & evafiles)
{
	deleteData();

	if (evafiles.size() <= number) //First scan is nr. 0
		return -1;

	std::list<cEvaFile *>::iterator iter;
	iter = evafiles.begin();
	advance(iter,number);
	//std::cout << (*iter)->time_of_sweep << "\n";;

	for (std::list<cUnit>::iterator i = (*iter)->bunches.begin(); i != (*iter)->bunches.end(); ++i)
	{
		//i->print_contents_of_this_class(cout);
		add_bunch(*i);
	}
	bin_width = (*iter)->get_bin_width();
	max_bin_number = (*iter)->get_MAX_MCS_channels();
	MCS_recording_offset_in_bins = (*iter)->get_MCS_recording_offset_in_bins();
	//std::cout << "MCS offset [us]: " << MCS_recording_offset_in_bins << "\n";
	data_is_in_single_bunch_mode = true;
	number_of_bunches_in_one_spectrum = 1;

	start_time = (*iter)->start_time_of_sweep;
	end_time = (*iter)->end_time_of_sweep;
	// std::cout << "Start time: " << start_time << "\n";
	// std::cout << "End time  : " << end_time << "\n";

	/// Assing extra infos, if any
	extra_info_dbl_1 = (*iter)->extra_info_double_1;
	extra_info_dbl_2 = (*iter)->extra_info_double_2;
	extra_info_str_1 = (*iter)->extra_info_string_1;
	extra_info_str_2 = (*iter)->extra_info_string_2;

	return (*iter)->scan_rounds;
}

//------------------------------------------------------------------------------
int cFrequencies::get_number_of_rounds_from_eva_data(int number, std::list< cEvaFile* >& evafiles)
{
	if (evafiles.size() <= number) //First scan is nr. 0
		return -1;

	std::list<cEvaFile *>::iterator iter;
	iter = evafiles.begin();
	advance(iter,number);

	return (*iter)->scan_rounds;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// int cFrequencies::getEventsAsTable(vector<int> &in_here) {
std::vector<Datapoint> * cFrequencies::get_event_histogram()
{
	event_histogram_data.clear();
	if (frequencynumber < 1)
		return &event_histogram_data;


	std::list<int> all_events;
	int empty_bunches(0);
	for (int i=0;i<frequencynumber;i++){
		if (frequencies[i]->is_active==false) continue; //LIMITATION
		
		std::list<int> events;
		frequencies[i]->get_events(events);
		all_events.merge(events);
		
		//empty_bunches += frequencies[i]->get_number_of_really_empty_bunches();
		
	}
	
	event_histogram_data.push_back(Datapoint(0,empty_bunches*1.0,0,0));
	//std::sort (all_events.begin(), all_events.end());
	all_events.sort();
	for (int j= 1; j <= all_events.back() +1; j++)
	{
		event_histogram_data.push_back(Datapoint(j,0,0,0));
	}
	for (std::list<int>::iterator iter = all_events.begin(); iter != all_events.end(); ++iter)
	{
		std::vector<Datapoint>::iterator k = event_histogram_data.begin();
		advance(k,*iter);
		k->y_value += 1.0;
	}

	
//	Datapoint last_zero(event_histogram_data.size()*1.0,0,0,0);
//	event_histogram_data.push_back(last_zero);
	return &event_histogram_data;
}
//------------------------------------------------------------------------------
std::vector< Datapoint >* cFrequencies::get_counts_vs_frequency(int class_number)
{
	counts_vs_frequency_data.clear();
	if (frequencynumber < 1)
		return &counts_vs_frequency_data;
	
	for (int i=0; i < frequencynumber;i++){
		if (frequencies[i]->is_active==false) continue; //LIMITATION
		
		double cts = frequencies[i]->getCounts(bin_start,bin_stop,class_min_cts[class_number],class_max_cts[class_number])*1.0;
		counts_vs_frequency_data.push_back(Datapoint(frequencies[i]->getFrequency(),cts,0,std::sqrt(cts)));
	}
	return &counts_vs_frequency_data;
}
//------------------------------------------------------------------------------
std::vector< Datapoint >* cFrequencies::get_average_counts_vs_frequency(int class_number)
{
  average_counts_vs_frequency_data.clear();
  if (frequencynumber < 1)
    return &average_counts_vs_frequency_data;
  
  for (int i=0; i < frequencynumber;i++){
    if (frequencies[i]->is_active==false) continue; //LIMITATION
    
    double cts = frequencies[i]->getCounts(bin_start,bin_stop,class_min_cts[class_number],class_max_cts[class_number])*1.0/frequencies[i]->getTotalBunchNumber();
    average_counts_vs_frequency_data.push_back(Datapoint(frequencies[i]->getFrequency(),cts,0,std::sqrt(cts)/frequencies[i]->getTotalBunchNumber()));
  }
  return &average_counts_vs_frequency_data;
}
//------------------------------------------------------------------------------
std::vector< Datapoint >* cFrequencies::get_tof_vs_frequency(int class_number)
{
	tof_vs_frequency_data.clear();
	if (frequencynumber < 1)
		return &tof_vs_frequency_data;

	
	double total_sd = get_sd_of_all_ions_in_class(class_number,stat_mode);
	for (int i=0; i <frequencynumber; i++){
		if (frequencies[i]->is_active==false) continue; //LIMITATION
		
		Datapoint toAdd;
		toAdd.x_value = frequencies[i]->getFrequency();
		toAdd.y_value = frequencies[i]->getTimeOfFlight(bin_start,bin_stop,class_min_cts[class_number],class_max_cts[class_number])*bin_width;
		toAdd.y_value_err = get_time_of_flight_error(i,class_number,total_sd);
		if (toAdd.y_value > 0)
			tof_vs_frequency_data.push_back(toAdd);
	}
	
	return &tof_vs_frequency_data;
}

std::vector<Datapoint> * cFrequencies::get_history_of_counts(int startbin, int stopbin,
												 int min_ctss, int max_ctss,
												 int sum_up_this_many_bunches,  std::vector<int> & round_changing_points)
{
	history_histogram_with_gates_data.clear();
	round_changing_points.clear();
	if (frequencynumber == 0)
		return &history_histogram_with_gates_data;
	
	
	int number_of_summed_bunches_now(0), counts_now(0), bunch_number_now(0);
	order_data();
	
	int how_many_rounds_there_are = frequencies[0]->getTotalBunchNumber();
	for (int roundsit=1; roundsit <= how_many_rounds_there_are; roundsit++) {
		round_changing_points.push_back(history_histogram_with_gates_data.size());
		for (int i=0; i < frequencynumber; i++) {
			counts_now += frequencies[i]->getCounts(startbin,stopbin,roundsit, min_ctss, max_ctss);
			number_of_summed_bunches_now ++;
			if (number_of_summed_bunches_now == sum_up_this_many_bunches) {
				history_histogram_with_gates_data.push_back(Datapoint(history_histogram_with_gates_data.size()*1.0,counts_now,0,0));
				counts_now = 0;
				number_of_summed_bunches_now = 0;
			}
		}
	}
	return &history_histogram_with_gates_data;
}

std::vector<Datapoint> * cFrequencies::get_full_history_of_counts(int sum_up_this_many_bunches, std::vector<int> & round_changing_points)
{
	history_histogram_data.clear();
	round_changing_points.clear();
	if (frequencynumber == 0)
		return &history_histogram_data;
	
	
	int number_of_summed_bunches_now(0), counts_now(0), bunch_number_now(0);
	order_data();
	
	int how_many_rounds_there_are = frequencies[0]->getTotalBunchNumber();
	for (int roundsit=1; roundsit <= how_many_rounds_there_are; roundsit++) {
		round_changing_points.push_back(history_histogram_data.size());
		for (int i=0; i < frequencynumber; i++) {
			counts_now += frequencies[i]->getCounts(0,0,roundsit, 0, 0);
			number_of_summed_bunches_now ++;
			if (number_of_summed_bunches_now == sum_up_this_many_bunches) {
				history_histogram_data.push_back(Datapoint(history_histogram_data.size()*1.0,counts_now,0,0));
				counts_now = 0;
				number_of_summed_bunches_now = 0;
			}
		}
	}
	return &history_histogram_data;
}

void cFrequencies::shift_scanrange(double amount)
{
	if (frequencynumber < 1)
		return;
	for (int i=0; i < frequencynumber; i++) {
		frequencies[i]->shift_frequency(amount);
	}
	
}
