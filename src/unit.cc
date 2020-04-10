#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <math.h>
using namespace std;

#include "unit.hh"
#include "mjonotpp.hh"
#include "guiutil.hh"

//Constant outer window for binning:


//------------------------------------------------------------------------------
int cUnit::increaseDatapointTableSize(int size_increment) {
	cDatapoint **datapoints2;
	datapoints2 = new(nothrow) cDatapoint *[maxdatapoints+size_increment];
	for (int i=0;i<datapointnumber;i++){
		datapoints2[i]=datapoints[i];
	}
	delete [] datapoints;
	datapoints = datapoints2;
	maxdatapoints+=size_increment;
//	cout << "increased size :P\n";
	return 1;
}
//------------------------------------------------------------------------------
string cUnit::read_file(const string &filename, const char delimiter, const int file_no){
  string row,erotettu;
  ifstream openedfile(filename.c_str());
  if (!openedfile) return "File doesn't open/exist!";
  while ( getline(openedfile,row)){     //Fetch frequency from datafile
    if (row[0] != '#') break;           //If not a comment, skipping frequency hunt
    do{                                 //Search pattern "Frequency: "
      erotettu = erota(row,' ',0);
      if (erotettu == "Frequency:"){ //Searches for pattern "Frequency:"
        erotettu = erota(row,' ',0)+"E000"; //if found, next word is set as f
        sscanf(erotettu.c_str(),"%lf",&frequency);
		  //cout << "Freq: " << frequency << "\n";
        break;
      }
	  if (erotettu == "File"){		//Getting record time of bunch #1
		erotettu = erota(row,' ',0); //saved
		erotettu = erota(row,' ',0); //at
		erotettu = erota(row,'\r',0); //rest all the way to carriage return..
		time_str = erotettu;			 // Rest of it 2005-02-32 16:40:40
	  }

    } while (erotettu != "");
    if (frequency != 0)
      break;
  };
	cout.precision(12);
//	cout << "taajuus: " << frequency << "\n";
//	string dd;
//	cin >> dd;
  if (frequency == 0) return "Error from datafile read: no frequency\n";
  while ( getline(openedfile,row)){
	//cout << "rivi: " << row << "\n";
    if (row[0] == '#' || row == "" ) continue;
    if (setDataRow(row, delimiter) != "") //check if there is errors
      return setDataRow(row, delimiter);  //if, then return it
  }
//  cout << rivi;
  openedfile.close();
  return "";

}
//------------------------------------------------------------------------------
string cUnit::setDataRow  //gets line as par and converts it to a datapoint
     (const string &line,const char delimiter){
  string tempstring = line;
  string help;
  cDatapoint newpoint;
  int bin=-1,counts=-1;
		 //cout << "LIne: " << line << "\n";
/*  while (tempstring != "" && counts == -1){        //reading line if not empty
    do{                                         //this loop erases whitespaces
      help = erota(tempstring,delimiter);
    } while (help == "");
    if (bin == -1){
      sscanf(help.c_str(),"%d",&bin);
      continue;
    }
    if (bin != -1){
      sscanf(help.c_str(),"%d",&counts);
//		if (counts > 0)
    }
  }
*/
	while ( (help = erota(tempstring,delimiter) ) != "" )
	{
		if (help == "")
			continue;
		if (bin == -1) {
			bin = atoi(help.c_str() );
			continue;
		}
		if (bin != -1 && counts == -1) {
			counts = atoi(help.c_str() );
			break;
		}
	}
//  cout << "bin " << bin << " counts " << counts << "\n";

	if (counts > 0){		//No adding of useless zero-count points
		newpoint.setValues(bin, counts,1,time_str);
		//cout << "InsertDataPoint bin: " << bin << " counts " << counts << endl;
		InsertDataPoint(newpoint);
	}
  return "";
}
//------------------------------------------------------------------------------
string cUnit::InsertDataPoint(const cDatapoint &toAdd){
	cDatapoint *uusi;
	if (datapointnumber >= maxdatapoints){
//		std::cout << "Maximum number of datapoints reached (frequency " << frequency << ")\n";
		increaseDatapointTableSize();
//		return "Too many points!";
	}
//	cout << "added datapoint index: " << datapointnumber << ", while max is " << maxdatapoints << "\n";
	uusi = new cDatapoint(toAdd);
	datapoints[datapointnumber] = uusi;
	datapointnumber++;
	return "";
}
//------------------------------------------------------------------------------
string cUnit::InsertDataPoint(int bin, int counts, const int bunch_no, string time_string){
	cDatapoint uusi;
	uusi.setValues(bin, counts, bunch_no,time_string);
	InsertDataPoint(uusi);
	return "";
}
//------------------------------------------------------------------------------
// Returns standard deviation of the mean TOF
/*
double cUnit::getSdom(int startbin, int stopbin){
  double ave = getAverage(startbin,stopbin);
	if (getCounts(startbin,stopbin) < 1)
		return 1e5;
  double dev_from_ave2(0), sdev(0);
  for(int i=0;i<datapointnumber;i++){
		if (     (datapoints[i]->getBin() >= startbin )
			&& (datapoints[i]->getBin() <= stopbin  ) ){
      dev_from_ave2 = (datapoints[i]->getBin() - ave);
      dev_from_ave2 = dev_from_ave2 * dev_from_ave2;
      sdev=sdev + datapoints[i]->getCounts() * dev_from_ave2;
		}
  }
//	cout << "Countsit: " << getCounts() << "\n";
  return (sqrt( sdev  / getCounts(startbin,stopbin) / (getCounts(startbin,stopbin)-1) ));
}
*/
//------------------------------------------------------------------------------
/*
double cUnit::getSdom(void){
  double ave = getAverage();
  double dev_from_ave2(0), sdev(0);
	for(int i=0;i<datapointnumber;i++){
    dev_from_ave2 = (datapoints[i]->getBin() - ave);
    dev_from_ave2 = dev_from_ave2 * dev_from_ave2;
    sdev=sdev + datapoints[i]->getCounts() * dev_from_ave2;
  }
  return (sqrt( sdev  / getCounts() / (getCounts()-1) ));
}*/
//------------------------------------------------------------------------------
// Returns total number of counts
int cUnit::getCounts(void){
  int counts(0);
  for(int i=0;i<datapointnumber;i++){
    counts = counts+datapoints[i]->getCounts();
  }
  return counts;
}
//------------------------------------------------------------------------------
int cUnit::getCounts(int startbin, int stopbin){
  int counts(0);
  for(int i=0;i<datapointnumber;i++){
		if (     (datapoints[i]->getBin() >= startbin )
		  && (datapoints[i]->getBin() <= stopbin  ) ){
      counts = counts+datapoints[i]->getCounts();
    }
  }
  return counts;
}
//------------------------------------------------------------------------------
/*
double cUnit::getAverage(void){ //returns TOF
  return getMoment(1);
}
*/
//------------------------------------------------------------------------------
/*
double cUnit::getAverage(int startbin, int stopbin){ //returns TOF
  return getMoment(startbin,stopbin,1);
}
*/
//------------------------------------------------------------------------------
//int cUnit::addDataToExisting(const cUnit &tobeadded, ostream &log){
	//cout.precision(10);
	//cout << "Before merging:\n";
	//print_contents_of_this_class();
	//cout << "This adds to existing data.\n";
	//cout << "there are " << tobeadded.datapointnumber << " bins to add\n";
//  for (int i=0;i<tobeadded.datapointnumber;i++){
	//cout << "After " << i << " rounds:\n";
	//cout << "Resulting in following:\n";
	//cout << "datapointnumber " << datapointnumber << "\n";
	//print_contents_of_this_class();
//    InsertDataPoint(*tobeadded.datapoints[i]);
//  }
//  return 0;
//}
//------------------------------------------------------------------------------
/*
double cUnit::getMoment(int startbin, int stopbin, double n_th_moment){ //returns Nth moment of tof distribution
  if (getCounts(startbin,stopbin) < 1)
    return -1;  //If there is no counts, return -1 as an error
//	if (getCounts(startbin,stopbin) = 1)	 EntÃ¤s jos counts = 1? TOF-error=0?
//		return 10000;
  double sum_thingy(0);
  for(int i=0;i<datapointnumber;i++){
	if (     (datapoints[i]->getBin() >= startbin )
		  && (datapoints[i]->getBin() <= stopbin  ) ){
    sum_thingy = sum_thingy+
             pow(datapoints[i]->getBin(), n_th_moment )* datapoints[i]->getCounts();
    }
  }
  return (sum_thingy*1.0)/getCounts(startbin,stopbin);
}
*/
//------------------------------------------------------------------------------
/*
double cUnit::getMoment(double n_th_moment){ //returns Nth moment of tof distribution
  if (getCounts() < 1)
    return -1;  //If there is no counts, return -1 as an error
  double sum_thingy(0);
  for(int i=0;i<datapointnumber;i++){
    sum_thingy = sum_thingy+
             pow(datapoints[i]->getBin(), n_th_moment )* datapoints[i]->getCounts();
  }
  return (sum_thingy*1.0)/getCounts();
//  return getMoment(0,datapointnumber,n_th_moment);
}
*/
//------------------------------------------------------------------------------
int cUnit::searchForBinMatch(int bin_searched){
	for (int i=0; i<datapointnumber; i++){
		if (bin_searched == datapoints[i]->getBin() )
			return i;
	}
	return -1;
}
//------------------------------------------------------------------------------
int cUnit::print_contents_of_this_class(ostream &in_here){
	in_here.precision(12);
	in_here << "Class content printouter--------------\n";
	in_here << "Frequency: " << frequency << "\n";
	in_here << "Time: " << time_str << "\n";
	in_here << "Datapointnumber: " << datapointnumber << "\n";
  for(int i=0;i<datapointnumber;i++){
		in_here << "i " << i << " bin " << datapoints[i]->getBin()
				 << " counts: " << datapoints[i]->getCounts()
				 << " Bunch_no: " << datapoints[i]->getBunch_no()
	  			 << " Time: " << datapoints[i]->getTime()
		  		 << " Is active: " << datapoints[i]->is_active()
	  			 << "\n";
	}
	in_here << "maxdatapoints: " <<maxdatapoints << "\n";
	in_here << "Number of total bunches: " << getTotalBunchNumber() << "\n";
// 	in_here << "Number of empty bunches: " << emptybunchnumber << "\n";
	in_here << "Class content printouter----------END--\n";
	return 0;
}
//------------------------------------------------------------------------------
string cUnit::outputEachFileOfFrequency(ostream &outfile,int startbin,int stopbin){
	int counts=0,startfile;
	int i=0;
//	print_contents_of_this_class();
	for (i=0;i<datapointnumber;i++) {
		startfile= datapoints[i]->getBunch_no();
		cout << "i is " << i << " startfile is " << startfile << "\n";
		if ( (datapoints[i]->getBin() <= startbin ) || (datapoints[i]->getBin() >= stopbin  ) ){
			cout << "skipping " << i << "\n";
			continue;
		}
		counts = 0;
		while ( (startfile == datapoints[i]->getBunch_no()) && (i<datapointnumber) ){
			if ( (datapoints[i]->getBin() > startbin ) && (datapoints[i]->getBin() < stopbin  ) ){
				counts = counts+datapoints[i]->getCounts();
			}
//			cout << " ii " << i << " ";
			i++;
		};
		i--;
		cout << "result i " << i << " file " << startfile
				 << " counts " << counts << "\n";
		cout << "tiedostoon " << startfile << " " << counts << "\n";
		if (i==datapointnumber)
			break;
	};
	return "";
}

//------------------------------------------------------------------------------
string  cUnit::addNewBunch(const cUnit &tobeadded, ostream &log){
	if (tobeadded.datapointnumber == 0){
		InsertDataPoint(0,0,bunchnumber+1,tobeadded.time_str);
// 		emptybunchnumber++;
		bunchnumber++;
//		log << "Added empty bunch to frequency " << frequency << "\n";
		return "";
	}

	for (int i=0;i<tobeadded.datapointnumber;i++){
		if (tobeadded.datapoints[i]->is_active() != true) {
			cout << "Hey, buggg!\n";
			tobeadded.datapoints[i]->set_use_this(true);
		}

		InsertDataPoint(tobeadded.datapoints[i]->getBin(),tobeadded.datapoints[i]->getCounts(),(bunchnumber+1),tobeadded.time_str );
	}
	bunchnumber++;
	return "";
}
//------------------------------------------------------------------------------
int cUnit::getTotalBunchNumber(void){
	if (datapointnumber == 0)
		return 0;//emptybunchnumber;
	return (datapoints[datapointnumber-1]->getBunch_no());// + emptybunchnumber);
}
//------------------------------------------------------------------------------
/*int cUnit::getTotalEventNumber(int startbin, int stopbin){
	int events(0);
	if (datapointnumber == 0)
		return 0;
	for (int i=0; i<datapointnumber; i++){
		if ( (startbin >= datapoints[i]->getBin() ) && (stopbin < datapoints[i]->getBin() ) )
			events = events+datapoints[i]->getCounts();
	}
	return events;
}*/
//------------------------------------------------------------------------------
//Returns how many bunches there are on defined bin interval [startbin,stopbin]
//that have counts_wanted amount of counts in
int cUnit::getEventNumber(int startbin, int stopbin, int counts_wanted){
	if (datapointnumber == 0)
  {
// 		if (counts_wanted == 0) //2008-01-03 Not quite getting it..
// 			return emptybunchnumber;
// 		else
			return 0;
	}
	//2008-01-03 if wanting zeroes, returning # 'real zero bunches'
	if (counts_wanted == 0) {
		return get_number_of_really_empty_bunches();
	}
	int count_number(0),events(0);
	int bunch_at_hand = datapoints[0]->getBunch_no();
	for (int i=0; i<datapointnumber; i++){
		if ( (startbin <= datapoints[i]->getBin() ) && (stopbin >= datapoints[i]->getBin() ) ){
			if ( bunch_at_hand == datapoints[i]->getBunch_no()){
				count_number += datapoints[i]->getCounts();
			}
			else{
				if ( count_number == counts_wanted)
					events++;
				count_number = datapoints[i]->getCounts();
				bunch_at_hand = datapoints[i]->getBunch_no();
			}
		}
	}
	if ( count_number == counts_wanted)
		events++;
//	if ( counts_wanted == 0)
//		events += emptybunchnumber;
	return events;
}

void cUnit::get_events(list< int >& to_here)
{
	int count_number(0),events(0);
	int bunch_at_hand = datapoints[0]->getBunch_no();
	for (int i=0; i<datapointnumber; i++)
	{
		if ( bunch_at_hand == datapoints[i]->getBunch_no()){
			count_number += datapoints[i]->getCounts();
		}
		else
		{
			to_here.push_back(count_number);
			count_number = datapoints[i]->getCounts();
			bunch_at_hand = datapoints[i]->getBunch_no();
		}
	}
	to_here.push_back(count_number);
}

//------------------------------------------------------------------------------
int cUnit::getCounts(int startbin, int stopbin, int min_cts, int max_cts, bool use_crap_window){
	return getCounts(SMALLEST_BIN, BIGGEST_BIN, startbin,stopbin, min_cts,max_cts);
}
//------------------------------------------------------------------------------
double cUnit::getTimeOfFlight(int startbin, int stopbin, int min_cts, int max_cts,bool use_crap_window){
	return getTimeOfFlight(startbin,stopbin, min_cts,max_cts,SMALLEST_BIN, BIGGEST_BIN);


	int tof(0),count_number(0),tot_count_number(0),total_tof(0);
	if (datapointnumber == 0)
		return -1;
	int bunch_at_hand = datapoints[0]->getBunch_no();
	for (int i=0; i<datapointnumber; i++){
		if ( (startbin <= datapoints[i]->getBin() ) && (stopbin >= datapoints[i]->getBin() ) ){
			if ( bunch_at_hand == datapoints[i]->getBunch_no()){
				count_number += datapoints[i]->getCounts();
				tof += (datapoints[i]->getBin()*datapoints[i]->getCounts());
			}
			else{
				if ( (count_number >= min_cts) && (count_number <= max_cts) ){
					tot_count_number += count_number;
					total_tof += tof;
				}
				count_number = datapoints[i]->getCounts();
				bunch_at_hand = datapoints[i]->getBunch_no();
				tof = (datapoints[i]->getBin()*datapoints[i]->getCounts());
			}
		}
	}
	if ( (count_number >= min_cts) && (count_number <= max_cts) ){
		tot_count_number += count_number;
		total_tof += tof;
	}
	if (tot_count_number == 0)
		return -1;
	return (total_tof*1.0)/(tot_count_number*1.0);
}
//------------------------------------------------------------------------------
double cUnit::getTimeOfFlightError(int startbin, int stopbin, int min_cts, int max_cts,bool use_crap_window){
	return getTimeOfFlightError(startbin,stopbin, min_cts,max_cts,SMALLEST_BIN, BIGGEST_BIN);
	/*
	double averageTOF=getTimeOfFlight(startbin, stopbin, min_cts, max_cts);
	double SumDevSq(0),totalSumDevSq(0);
	int count_number(0),tot_count_number(0);
	if (datapointnumber == 0)
		return -1;
	int bunch_at_hand = datapoints[0]->getBunch_no();
	for (int i=0; i<datapointnumber; i++){
		if ( (startbin <= datapoints[i]->getBin() ) && (stopbin >= datapoints[i]->getBin() ) ){
			if ( bunch_at_hand == datapoints[i]->getBunch_no()){
				count_number += datapoints[i]->getCounts();
				SumDevSq += pow((averageTOF-datapoints[i]->getBin()),2)*datapoints[i]->getCounts();
			}
			else{
				if ( (count_number >= min_cts) && (count_number <= max_cts) ){
					tot_count_number += count_number;
					totalSumDevSq += SumDevSq;
				}
				count_number = datapoints[i]->getCounts();
				bunch_at_hand = datapoints[i]->getBunch_no();
				SumDevSq =  pow((averageTOF-datapoints[i]->getBin()),2)*datapoints[i]->getCounts();
			}
		}
	}
	if ( (count_number >= min_cts) && (count_number <= max_cts) ){
		tot_count_number += count_number;
		totalSumDevSq += SumDevSq;
	}
	if (tot_count_number == 0)
		return -1;
	if (tot_count_number == 1) //Note: If only one count: Error is 300 bins..
		return 300;
	if ( fabs(totalSumDevSq) < 0.00001) {
		cout << "This causes trouble! there is a point that has many ions only in a SINGLE bin!\n"
			<< "Setting that point error to 100.";
		return 100;
	}
	return sqrt(totalSumDevSq / (tot_count_number) / (tot_count_number -1));*/
}

//------------------------------------------------------------------------------
void cUnit::getHistogram(int counts_in_bin[], int max_bins){
	int bini;
	for (int i=0;i<datapointnumber;i++){
		bini = datapoints[i]->getBin();
		if (bini <= max_bins)
			counts_in_bin[bini] += datapoints[i]->getCounts();
	}
	return;
}
//------------------------------------------------------------------------------
void cUnit::getHistogram(vector<int> &in_here) {
	int bini;
	for (int i=0;i<datapointnumber;i++){
		bini = datapoints[i]->getBin();
		in_here[bini] += datapoints[i]->getCounts();
	}
	return;
}

void cUnit::get_histogram(vector< Datapoint >* to_here)
{
	int bini;
	std::vector<Datapoint>::iterator iter;
	for (int i=0;i<datapointnumber;i++){
		bini = datapoints[i]->getBin();
		iter = to_here->begin(); advance(iter,bini);
		iter->y_value += datapoints[i]->getCounts();
	}
}

//------------------------------------------------------------------------------
string cUnit::addOneEmptyBunch(const string time_string, ostream &log){
	InsertDataPoint(0,0,bunchnumber+1,time_string);
// 	emptybunchnumber++;
	bunchnumber++;
	return "";
}
//------------------------------------------------------------------------------
int cUnit::getCountsOfABunch(int bunch_no){
	if (bunch_no > bunchnumber)
		return -1;
	if (bunch_no < 1) {
		cout << "invalid!\n";
		return -2;
	}
	int total_counts(0);
	for (int i=0;i<datapointnumber;i++){
		if (datapoints[i]->getBunch_no() != bunch_no){
			if (total_counts > 0)
				break;
			else
				continue;
		}
		total_counts += datapoints[i]->getCounts();
	}

	return total_counts;
}
//------------------------------------------------------------------------------
int cUnit::getGetEventNumberAsTable(int ions_per_shot[], int events[],
               int start_index, int &stop_index, int max_index)
{




	return 1;
}
//------------------------------------------------------------------------------
int cUnit::get_maximum_counts_in_bunch(void) {
	if (datapointnumber < 1)
		return 0;
	int current_maximum(0);
	int bunch_at_hand(datapoints[0]->getBunch_no());
	int now(0);
	for (int i=0;i<datapointnumber;i++){
		if (datapoints[i]->getBunch_no() == bunch_at_hand)
			now += datapoints[i]->getCounts();
		else {
			if ( now > current_maximum)
				current_maximum = now;
			bunch_at_hand = datapoints[i]->getBunch_no();
			now = datapoints[i]->getCounts();
		}
	}
	if ( now > current_maximum)
		current_maximum = now;
	return current_maximum;
}
//------------------------------------------------------------------------------
//2007-03-13 Adding the possibility for crap-window..
int cUnit::getCounts(int outer_startbin, int outer_stopbin, int inner_startbin, int inner_stopbin,
	              int min_cts, int max_cts)
{
	int count_number_outer(0), count_number_inner(0), tot_count_number_outer(0), tot_count_number_inner(0);
	if (datapointnumber == 0)
		return 0;
	int bunch_at_hand = datapoints[0]->getBunch_no();
	for (int i=0; i<datapointnumber; i++){
		if ( (outer_startbin <= datapoints[i]->getBin() ) && (outer_stopbin >= datapoints[i]->getBin() ) ){
			if ( bunch_at_hand == datapoints[i]->getBunch_no()){
				count_number_outer += datapoints[i]->getCounts();
				if ( (inner_startbin <= datapoints[i]->getBin() ) && (inner_stopbin >= datapoints[i]->getBin() ) ){
					count_number_inner += datapoints[i]->getCounts();
				}
			}
			else{  //So datapoints[i] belongs to next bunch than [i-1]
				if ( (count_number_outer >= min_cts) && (count_number_outer <= max_cts) ){
					tot_count_number_outer += count_number_outer;
					tot_count_number_inner += count_number_inner;
				}
				count_number_outer = datapoints[i]->getCounts();
				bunch_at_hand = datapoints[i]->getBunch_no();
				if ( (inner_startbin <= datapoints[i]->getBin() ) && (inner_stopbin >= datapoints[i]->getBin() ) ){
					count_number_inner = datapoints[i]->getCounts();
				}
				else {
					count_number_inner = 0;
				}
			}
		}
	}
	if ( (count_number_outer >= min_cts) && (count_number_outer <= max_cts) ){
		tot_count_number_outer += count_number_outer;
		tot_count_number_inner += count_number_inner;
	}
	//cout << "Inner, outer counts " << tot_count_number_inner << ", " << tot_count_number_outer << "\n";
	return tot_count_number_inner;
}
//------------------------------------------------------------------------------
//2007-04-19 This will return tof by using outer bin to calculate in which class
//					 the tof belongs and only calculates tof using inner binning boundaries.
double cUnit::getTimeOfFlight(int inner_startbin, int inner_stopbin, int min_cts, int max_cts,
								int outer_startbin, int outer_stopbin)
{
	if ( !are_binnings_valid(inner_startbin,inner_stopbin,outer_startbin,outer_stopbin)) {
		cout << "Something strange with the inner-outer bin-windows!\n";
		return -666;
	}


	int tof(0),count_number_inner(0),count_number_outer(0),
						tot_count_number_inner(0),total_tof(0);
	if (datapointnumber == 0)
		return -1;

	int bunch_at_hand = datapoints[0]->getBunch_no();
	for (int i=0; i<datapointnumber; i++){
		if ( (outer_startbin <= datapoints[i]->getBin() ) && (outer_stopbin >= datapoints[i]->getBin() ) ){
			if ( bunch_at_hand == datapoints[i]->getBunch_no()){
				count_number_outer += datapoints[i]->getCounts(); //Add in any case to outer #counts
				//If it is inside the inner binning window.. use for tof-calc
				if ( (inner_startbin <= datapoints[i]->getBin() ) && (inner_stopbin >= datapoints[i]->getBin() ) ){
					tof += (datapoints[i]->getBin()*datapoints[i]->getCounts());
					count_number_inner += datapoints[i]->getCounts();
				}
			}
			else{
				if ( (count_number_outer >= min_cts) && (count_number_outer <= max_cts) ){
					tot_count_number_inner += count_number_inner;
					total_tof += tof;
				}
				count_number_inner = 0;
				tof = 0;
				bunch_at_hand = datapoints[i]->getBunch_no();
				count_number_outer = datapoints[i]->getCounts();
				if ( (inner_startbin <= datapoints[i]->getBin() ) && (inner_stopbin >= datapoints[i]->getBin() ) ){
					tof = (datapoints[i]->getBin()*datapoints[i]->getCounts());
					count_number_inner = datapoints[i]->getCounts();
				}
			}
		}
	}
	if ( (count_number_outer >= min_cts) && (count_number_outer <= max_cts) ){
		tot_count_number_inner += count_number_inner;
		total_tof += tof;
	}
	if (tot_count_number_inner == 0)
		return -1;
	return (total_tof*1.0)/(tot_count_number_inner*1.0) + MCS_TOF_offset_in_bins;
}
//------------------------------------------------------------------------------
double cUnit::getTimeOfFlightError(int inner_startbin, int inner_stopbin, int min_cts, int max_cts,
								int outer_startbin, int outer_stopbin)
{
	if ( !are_binnings_valid(inner_startbin,inner_stopbin,outer_startbin,outer_stopbin)) {
		cout << "Something strange with the inner-outer bin-windows!\n";
		return -666;
	}
	double averageTOF=getTimeOfFlight(inner_startbin, inner_stopbin, min_cts, max_cts,
																			outer_startbin, outer_stopbin);
	if (averageTOF<0)
		return -2;
	double SumDevSq(0),totalSumDevSq(0);
	int tof(0),count_number_inner(0),count_number_outer(0),
						tot_count_number_inner(0),total_tof(0);
	if (datapointnumber == 0)
		return -1;

	int bunch_at_hand = datapoints[0]->getBunch_no();
	for (int i=0; i<datapointnumber; i++){
		if ( (outer_startbin <= datapoints[i]->getBin() ) && (outer_stopbin >= datapoints[i]->getBin() ) ){
			if ( bunch_at_hand == datapoints[i]->getBunch_no()){
				count_number_outer += datapoints[i]->getCounts(); //Add in any case to outer #counts
				//If it is inside the inner binning window.. use for tof-calc
				if ( (inner_startbin <= datapoints[i]->getBin() ) && (inner_stopbin >= datapoints[i]->getBin() ) ){
					tof += (datapoints[i]->getBin()*datapoints[i]->getCounts());
					count_number_inner += datapoints[i]->getCounts();
					SumDevSq += pow((averageTOF-datapoints[i]->getBin()),2)*datapoints[i]->getCounts();
				}
			}
			else{
				if ( (count_number_outer >= min_cts) && (count_number_outer <= max_cts) ){
					tot_count_number_inner += count_number_inner;
					total_tof += tof;
					totalSumDevSq += SumDevSq;
				}
				count_number_inner = 0;
				tof = 0;
				SumDevSq = 0;
				bunch_at_hand = datapoints[i]->getBunch_no();
				count_number_outer = datapoints[i]->getCounts();
				if ( (inner_startbin <= datapoints[i]->getBin() ) && (inner_stopbin >= datapoints[i]->getBin() ) ){
					tof = (datapoints[i]->getBin()*datapoints[i]->getCounts());
					count_number_inner = datapoints[i]->getCounts();
					SumDevSq += pow((averageTOF-datapoints[i]->getBin()),2)*datapoints[i]->getCounts();
				}
			}
		}
	}
	if ( (count_number_outer >= min_cts) && (count_number_outer <= max_cts) ){
		tot_count_number_inner += count_number_inner;
		total_tof += tof;
		totalSumDevSq += SumDevSq;
	}
	if (tot_count_number_inner == 0)
		return -1;
	if (tot_count_number_inner == 1) //Note: If only one count: Error is 300 bins..
		return 300;
	if ( fabs(totalSumDevSq) < 0.00001) {
		cout << "This causes trouble! there is a point that has many ions only in a SINGLE bin!\n"
			<< "Setting that point error to 100.";
		return 100;
	}
	return sqrt(totalSumDevSq / (tot_count_number_inner) / (tot_count_number_inner -1));
}
//------------------------------------------------------------------------------
bool cUnit::are_binnings_valid(int in_start,int in_stop,int out_start, int out_stop) {
//	cout << "out start, in start " << out_start << "  " << in_start << "\n";
//	cout << "in stop, out stop " << in_stop << "  " << out_stop << "\n";
	if (out_start > in_start || in_start > in_stop || in_stop > out_stop )
		return false;
	return true;
}
//------------------------------------------------------------------------------
void cUnit::append_to_cts_vs_bin_histogram(vector<int> & countsit,int inner_startbin,
								int inner_stopbin, int min_cts, int max_cts, bool correct_with_mean,
								int outer_startbin, int outer_stopbin)
{
	int aver(0);
	if (correct_with_mean)
		aver = static_cast<int>(getTimeOfFlight(inner_startbin,inner_stopbin,min_cts,max_cts,outer_startbin,outer_stopbin)+0.5);
	list<TWO_INTS> maybe_append,yes_appended; TWO_INTS toAdd;
	int count_number_outer(0), count_number_inner(0), tot_count_number_outer(0), tot_count_number_inner(0);
	if (datapointnumber == 0)
		return;
	int bunch_at_hand = datapoints[0]->getBunch_no();
	for (int i=0; i<datapointnumber; i++){
		if ( (outer_startbin <= datapoints[i]->getBin() ) && (outer_stopbin >= datapoints[i]->getBin() ) ){
			if ( bunch_at_hand == datapoints[i]->getBunch_no()){
				count_number_outer += datapoints[i]->getCounts();
				if ( (inner_startbin <= datapoints[i]->getBin() ) && (inner_stopbin >= datapoints[i]->getBin() ) ){
					count_number_inner += datapoints[i]->getCounts();
					toAdd.low = datapoints[i]->getBin();
					if (correct_with_mean)
						toAdd.low += -aver+1000;
					toAdd.high = datapoints[i]->getCounts();
					maybe_append.push_back(toAdd);
				}
			}
			else{  //So datapoints[i] belongs to next bunch than [i-1]
				if ( (count_number_outer >= min_cts) && (count_number_outer <= max_cts) ){
					tot_count_number_outer += count_number_outer;
					tot_count_number_inner += count_number_inner;
					if ( !(maybe_append.empty()) ) {
						yes_appended.insert(yes_appended.end(), maybe_append.begin(), maybe_append.end());
					}
				}
				maybe_append.clear();
				count_number_outer = datapoints[i]->getCounts();
				bunch_at_hand = datapoints[i]->getBunch_no();
				if ( (inner_startbin <= datapoints[i]->getBin() ) && (inner_stopbin >= datapoints[i]->getBin() ) ){
					count_number_inner = datapoints[i]->getCounts();
					toAdd.low = datapoints[i]->getBin();
					if (correct_with_mean)
						toAdd.low += -aver+1000;
					toAdd.high = datapoints[i]->getCounts();
					maybe_append.push_back(toAdd);
				}
				else {
					count_number_inner = 0;
				}
			}
		}
	}
	if ( (count_number_outer >= min_cts) && (count_number_outer <= max_cts) ){
		tot_count_number_outer += count_number_outer;
		tot_count_number_inner += count_number_inner;
		if ( !(maybe_append.empty()) ) {
			yes_appended.insert(yes_appended.end(), maybe_append.begin(), maybe_append.end());
			maybe_append.clear();
		}
	}

	for (list<TWO_INTS>::iterator i = yes_appended.begin(); i != yes_appended.end(); i++ ) {
		countsit[i->low] += i->high;
	}

	//cout << "Inner, outer counts " << tot_count_number_inner << ", " << tot_count_number_outer << "\n";
	return;

}
//------------------------------------------------------------------------------
void cUnit::get_histogram_of_class(std::vector<Datapoint> * to_here,int inner_startbin,
								int inner_stopbin, int min_cts, int max_cts, bool correct_with_mean,
								int outer_startbin, int outer_stopbin)
{
	int aver(0);
	if (correct_with_mean)
		aver = static_cast<int>(getTimeOfFlight(inner_startbin,inner_stopbin,min_cts,max_cts,outer_startbin,outer_stopbin)+0.5);
	list<TWO_INTS> maybe_append,yes_appended; TWO_INTS toAdd;
	int count_number_outer(0), count_number_inner(0), tot_count_number_outer(0), tot_count_number_inner(0);
	if (datapointnumber == 0)
		return;
	int bunch_at_hand = datapoints[0]->getBunch_no();
	for (int i=0; i<datapointnumber; i++){
		if ( (outer_startbin <= datapoints[i]->getBin() ) && (outer_stopbin >= datapoints[i]->getBin() ) ){
			if ( bunch_at_hand == datapoints[i]->getBunch_no()){
				count_number_outer += datapoints[i]->getCounts();
				if ( (inner_startbin <= datapoints[i]->getBin() ) && (inner_stopbin >= datapoints[i]->getBin() ) ){
					count_number_inner += datapoints[i]->getCounts();
					toAdd.low = datapoints[i]->getBin();
					if (correct_with_mean)
						toAdd.low += -aver+1000;
					toAdd.high = datapoints[i]->getCounts();
					maybe_append.push_back(toAdd);
				}
			}
			else{  //So datapoints[i] belongs to next bunch than [i-1]
				if ( (count_number_outer >= min_cts) && (count_number_outer <= max_cts) ){
					tot_count_number_outer += count_number_outer;
					tot_count_number_inner += count_number_inner;
					if ( !(maybe_append.empty()) ) {
						yes_appended.insert(yes_appended.end(), maybe_append.begin(), maybe_append.end());
					}
				}
				maybe_append.clear();
				count_number_outer = datapoints[i]->getCounts();
				bunch_at_hand = datapoints[i]->getBunch_no();
				if ( (inner_startbin <= datapoints[i]->getBin() ) && (inner_stopbin >= datapoints[i]->getBin() ) ){
					count_number_inner = datapoints[i]->getCounts();
					toAdd.low = datapoints[i]->getBin();
					if (correct_with_mean)
						toAdd.low += -aver+1000;
					toAdd.high = datapoints[i]->getCounts();
					maybe_append.push_back(toAdd);
				}
				else {
					count_number_inner = 0;
				}
			}
		}
	}
	if ( (count_number_outer >= min_cts) && (count_number_outer <= max_cts) ){
		tot_count_number_outer += count_number_outer;
		tot_count_number_inner += count_number_inner;
		if ( !(maybe_append.empty()) ) {
			yes_appended.insert(yes_appended.end(), maybe_append.begin(), maybe_append.end());
			maybe_append.clear();
		}
	}

	std::vector<Datapoint>::iterator iter;
	for (list<TWO_INTS>::iterator i = yes_appended.begin(); i != yes_appended.end(); i++ ) {
		// to_here[i->low] += i->high;
		
		iter = to_here->begin(); advance(iter,i->low);
		iter->y_value += i->high;

	}

	//cout << "Inner, outer counts " << tot_count_number_inner << ", " << tot_count_number_outer << "\n";
	return;

}
//------------------------------------------------------------------------------
int cUnit::set_bunch_not_active(int bunch_number)
{
	bool was_bunch_found(0);
	for (int i=0; i<datapointnumber; i++){
		if (datapoints[i]->getBunch_no() == bunch_number) {
			datapoints[i]->set_use_this(false);
			was_bunch_found = true;
		}
	}
	if (was_bunch_found)
		return 1;
	else
		return 0;
}
//------------------------------------------------------------------------------
void cUnit::set_all_bunches_active() {
	for (int i=0; i<datapointnumber; i++){
		datapoints[i]->set_use_this(true);
	}
}
//------------------------------------------------------------------------------
int cUnit::get_number_of_really_empty_bunches() {
	int coutingg(0);
	for (int i=0; i<datapointnumber; i++){
		if (datapoints[i]->is_really_zero_and_not_excluded() == true)
			coutingg ++;
	}
	return coutingg;
}
//------------------------------------------------------------------------------
void cUnit::set_active(bool state) {
	for (int i=0; i<datapointnumber; i++){
		datapoints[i]->set_use_this(state);
	}
}
//------------------------------------------------------------------------------
void cUnit::remove_datas() {
	for (int i=0; i < datapointnumber; i++){
		delete datapoints[i];
	}
	datapointnumber = 0;
}
//------------------------------------------------------------------------------
int cUnit::set_unit_time_for_ions_if_not_set() {
	for (int i=0; i<datapointnumber; i++){
		if (datapoints[i]->getTime() == "")
			datapoints[i]->set_time(time_str);
	}

}
//------------------------------------------------------------------------------
void cUnit::shift_frequency(double amount)
{
	frequency += amount;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
