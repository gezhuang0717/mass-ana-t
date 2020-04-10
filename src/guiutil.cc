#include <cstring>
#include <iostream>
#include <vector>

#include "guiutil.hh"

using std::string;
using std::vector;
using namespace boost::filesystem;

//------------------------------------------------------------------------------
void CTS_HISTOGRAM_OF_FREQ::pretty_print(std::ostream &os) {
//	if (counts_in_interval.empty() )
//		return;
	int d(0);
	os.precision(12);
//	os << center_freq << " " << (start_tof + tof_interval*(d-1)) << " 0\n";
	int counteri(0); vector<int>::iterator j;
	for (vector<int>::iterator i=counts_in_interval.begin(); i != counts_in_interval.end(); ++i) {
//		if (*i != 0)
		{
			//Was the previous empty?
//			j = i;
//			if (j != counts_in_interval.begin())
//				j--;
//			if (*j == 0)
//				os << center_freq << " " << (start_tof + tof_interval*(d-1)) << " 0\n";
			//Printing itself
			os << center_freq << " " << (start_tof + tof_interval*d + tof_interval/2.) << " " << *i << "\n";
			//Is the following empty?
/*			j++;j++;
			if (j != counts_in_interval.end()) {
				if (*j == 0) {
					j++;
					if (j != counts_in_interval.end()) {
						if (*j == 0)
							os << center_freq << " " << (start_tof + tof_interval*(d+1)) << " 0\n";
					}
				}
			}*/
		}
		d++;
	}
//	os << center_freq << " " << (start_tof + tof_interval*(d)) << " 0\n";
}
//------------------------------------------------------------------------------
double getDoubleFromEntry(Gtk::Entry &entri)
{
    Glib::ustring content = entri.get_text();
    return atof(content.c_str());
}
//------------------------------------------------------------------------------
int getIntegerFromEntry(Gtk::Entry &entri)
{
    Glib::ustring content = entri.get_text();
    return atoi(content.c_str());
}
//------------------------------------------------------------------------------
void putIntegerToEntry(Gtk::Entry &entri, int numb)
{
    char output[256];
    sprintf(output,"%d",numb);
    entri.set_text(output);
    return;
}
//------------------------------------------------------------------------------
void putDoubleToEntry(Gtk::Entry &entri, double numb,int style)
{
    entri.set_text(asString(numb, style));
    return;
}
//------------------------------------------------------------------------------
void update_gtk(void)
{
    while (Gtk::Main::events_pending()) {
	Gtk::Main::iteration();
    }
}
//------------------------------------------------------------------------------
void  setRangeToSpinbutton (Gtk::SpinButton &sp, const double &low, const double &high) {
	double current = sp.get_value();
	sp.set_range(low,high);
	sp.set_value(current);
}
//------------------------------------------------------------------------------
void DATASET::set_color_alpha(double alapha) {
	color_alpha = alapha;
	if (alapha < 0)
		color_alpha = 0;
	if (alapha > 1.0)
		color_alpha = 1.0;
}
//------------------------------------------------------------------------------
bool check_and_put_double_to_spinbutton(Gtk::SpinButton &sp,
								   const std::string &to_check,
								   const std::string &to_match,
								   const std::string &to_put)
{
	if (to_check != to_match)
		return 0;
	sp.set_value(as_double(to_put));
	return 1;
}
//------------------------------------------------------------------------------
bool check_and_put_boolean_to_checkbutton(Gtk::CheckButton &cb,
								   const std::string &to_check,
								   const std::string &to_match,
								   const std::string &to_put)
{
	if (to_check != to_match)
		return 0;
	if (to_put == "1")
		cb.set_active(1);
	else
		cb.set_active();
	return 1;

}
//------------------------------------------------------------------------------
std::string save_spinbutton_string(const Gtk::SpinButton &sp,
								   const std::string &id_string)
{
	return id_string+'\t'+asString(sp.get_value())+'\n';
}
//------------------------------------------------------------------------------
bool open_file_for_reading_and_popup_message_if_error(const std::string &filename, boost::filesystem::ifstream &ifstreami) 
{
    try {
	open_file_for_reading(filename, ifstreami);
    } catch(std::ifstream::failure & e) {
	Gtk::MessageDialog dialooki("File \"" + filename + "\" could not be opened!\n",false,Gtk::MESSAGE_ERROR);
	dialooki.set_title("What could be the wrong?");
	dialooki.run();
	return false;
    }
    return true;
}
//------------------------------------------------------------------------------
bool open_file_for_overwriting_and_popup_message_if_error(const std::string &filename, boost::filesystem::ofstream &ofstreami, bool overwrite_if_exists) 
{
	// Case 1. The clear case: Does not exist.
	if ( !boost::filesystem::exists( filename) )
	{
		// We go outside 
	}
	else // Case 2: Exists.
	{
		if (overwrite_if_exists) // Case 2a: But we can overwrite.
		{
			// We go outside
		}
		else // Case 2b: ask for overwrite permission
		{
			Gtk::MessageDialog dialooki("File \"" + filename + "\" already exists!\n\nOverwrite it?",false,Gtk::MESSAGE_WARNING, Gtk::BUTTONS_YES_NO);
			dialooki.set_title("File already exists!");
			dialooki.set_default_response(Gtk::RESPONSE_NO);
			int vastaus = dialooki.run();
			if (vastaus == Gtk::RESPONSE_NO) // We got NO as an answer
			{
				return false;
			}
			else // WE GOT YES AS an answer
			{
				// We go outside
			}
		}
	}
	// Now we are outside.
	
	try
	{
		open_file_for_writing(filename, ofstreami);
	}
	catch (std::ofstream::failure & e) // In case opening for writing fails
	{
		Gtk::MessageDialog dialooki("File \"" + filename + "\" could not be written!\n", false, Gtk::MESSAGE_ERROR);
		dialooki.set_title("What could be the wrong?");
		dialooki.run();
		return false;
	}
	// File was successfully opened.
	return true;

	
	
	
	
	/*
	
	
	
    try {
	try {
		cout << "Works so far0" << endl;
	    boost::filesystem::ifstream tmp;
	    //open_file_for_reading(filename, tmp);
		boost::filesystem::exists( filename )
		cout << "Works so far1" << endl;
	    // In case we overwrite anyway, i.e. overwrite_if_exists == false
	    if (false == overwrite_if_exists )
	    {
	      Gtk::MessageDialog dialooki("File \"" + filename + "\" already exists!\n\nOverwrite it?",false,Gtk::MESSAGE_WARNING, Gtk::BUTTONS_YES_NO);
	      dialooki.set_title("File already exists!");
	      dialooki.set_default_response(Gtk::RESPONSE_NO);
	      int vastaus = dialooki.run();
	      if (vastaus == Gtk::RESPONSE_NO)
	      {
		  tmp.close();
		  return false;
	      }
	    }
	} catch(std::ifstream::failure & e) {
		cout << "Works so far2" << endl;
	    open_file_for_writing(filename, ofstreami);
		cout << "Works so far3" << endl;
      }
    } catch(std::ofstream::failure & e) {
	cout << "Works so far4" << endl;	
	Gtk::MessageDialog dialooki("File \"" + filename + "\" could not be written!\n", false, Gtk::MESSAGE_ERROR);
	dialooki.set_title("What could be the wrong?");
	dialooki.run();
	return false;
    }
    return true;
	*/
}
//------------------------------------------------------------------------------
bool open_file_for_appending_and_popup_message_if_error(const std::string &filename, boost::filesystem::ofstream &ofstreami) {
	ofstreami.open(filename.c_str(), std::ios::app);
	if (!ofstreami) {
		Gtk::MessageDialog dialooki("File \"" + filename +"\" could not be written!\n",false,Gtk::MESSAGE_ERROR);
		dialooki.set_title("What could be the wrong?");
		dialooki.run();
		return false;
	}
	return true;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool BARS::delete_bar(Glib::ustring which_id) {
	if (bars.empty())
		return 0;
	bool tuhottiin_jotain(false);
	for (vector<BAR>::iterator i=bars.begin();i != bars.end(); ++i) {
		if (i->id == which_id) {
			bars.erase(i);
			i--;
			tuhottiin_jotain = true;
		}
	}
	return tuhottiin_jotain;
}
//------------------------------------------------------------------------------
bool BARS::replace_value_in_bar(const double & replacee, Glib::ustring which_id) {
	if (bars.empty())
		return 0;
	for (vector<BAR>::iterator i=bars.begin();i != bars.end(); ++i) {
		if (i->id == which_id) {
			i->value = replacee;
			return 1;
		}
	}
	return 0;
}
//------------------------------------------------------------------------------
double BARS::get_value_from_bar(Glib::ustring which_id) {
	if (bars.empty())
		return -6.6666e15;
	for (vector<BAR>::iterator i=bars.begin();i != bars.end(); ++i) {
		if (i->id == which_id) {
			return i->value;
		}
	}
}
//------------------------------------------------------------------------------
void  convert_histogram_and_compress(const vector<int> &input, DATASET &output, int compression_factor) {
	int c(0),sum(0),runn(0);
	Datapoint pointti; pointti.x_value_err=0; pointti.y_value_err = 0;
	for (vector<int>::const_iterator i = input.begin(); i != input.end(); i++) {
		sum += *i;
		c++;
		if (c == compression_factor) {
			pointti.x_value = runn*compression_factor; pointti.y_value = sum;
			output.add_datapoint(pointti);
			runn++; c = 0; sum = 0;
		}
	}
	pointti.x_value = runn; pointti.y_value = sum;
	output.add_datapoint(pointti);
}
