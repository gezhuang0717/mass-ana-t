#ifndef GUIUTIL_HH
#define GUIUTIL_HH

#include <gtkmm/builder.h>

#include <gtkmm/entry.h>
#include <gtkmm/main.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/messagedialog.h>
#include <iostream>
#include <vector>

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include "util.hh"
#include "mjonotpp.hh"
#include "compat.h"

using namespace std;

extern const char lakritsi_xml_data[];
extern const unsigned lakritsi_xml_size;

extern Glib::RefPtr<Gtk::Builder> gtk_builder;

enum AXIS {
	X = 0,
	Y = 1
};
enum GRAPH_TYPE {
	YERR = 0,
	LINE = 1,
	FILLED_LINE = 3,
	FANCY_HISTOGRAM = 4,
	STACKED = 5,
	SQUARES_WITHOUT_Y_VALUE = 6,
	CROSSES_WITHOUT_Y_VALUE = 7,
	MARKER_RECTANGLE = 8
};
enum LEGEND_POSITION {
	BOTTOM_LEFT = 0,
	BOTTOM_RIGHT = 1,
	TOP_LEFT = 3,
	TOP_RIGHT = 4
};

class ARROW{
	public:
	double x1,y1,x2,y2;
	double width;
	Gdk::Color color;
	ARROW(){color = Gdk::Color("Black"); width=1;}
};
class BAR{
	public:
	double value,width;
	Glib::ustring label;
	Glib::ustring id;
	Gdk::Color color;
	BAR(){color = Gdk::Color("Magenta"); width=1; value=1;}
	BAR(double valuei, Gdk::Color colori = Gdk::Color("Magenta"), Glib::ustring labeli ="", Glib::ustring idi ="", double widthi = 1){
		value = valuei; color = colori; label = labeli; id = idi; width = widthi;
	}

};
class BARS{
	public:
	std::vector<BAR> bars;
	void add_bar(BAR lisa){bars.push_back(lisa);}
	void delete_bars(){bars.clear();}
	bool delete_bar(Glib::ustring which_id);
	bool replace_value_in_bar(const double & replacee, Glib::ustring which_id); //returns 1 if success
	double get_value_from_bar(Glib::ustring which_id);
};

double 	getDoubleFromEntry	(Gtk::Entry &entri);
int 	getIntegerFromEntry	(Gtk::Entry &entri);
void 	putIntegerToEntry	(Gtk::Entry &entri, int numb);
void 	putDoubleToEntry	(Gtk::Entry &entri, double numb,int style=0);
void  setRangeToSpinbutton (Gtk::SpinButton &sp, const double &low, const double &high);

void	update_gtk			(void);
bool check_and_put_double_to_spinbutton(Gtk::SpinButton &sp,const std::string &to_check, const std::string &to_match, const std::string &to_put);
bool check_and_put_boolean_to_checkbutton(Gtk::CheckButton &cb, const std::string &to_check, const std::string &to_match, const std::string &to_put);
std::string save_spinbutton_string(const Gtk::SpinButton &sp, const std::string &id_string);
//Returns time needed in [us] to add in order to get corresponding phase

//2007-03-13 Fancy aid to plot 3d of counts vs freq vs tof
class CTS_HISTOGRAM_OF_FREQ{
  public:
	double half_distance_to_left_freq;
	double half_distance_to_right_freq;
	double center_freq;
	double start_tof;  //In micro s!!
	double tof_interval; //In micro s!!
	std::vector<int> counts_in_interval;
	void pretty_print(std::ostream &os);

};

struct TWO_INTS{
	int low;
	int high;
};

struct DATA{
    double data_x,data_y,data_y_err,data_x_err;

    DATA() { memset(this, 0, sizeof (*this)); }
};

class DATASET{
 public:
  DATASET() {
      color= Gdk::Color("Black"); color_alpha = 1.0; graph_type = YERR;
      legend_position = BOTTOM_LEFT;
  }
  ~DATASET(){}
	//double Cr,Cg,Cb,Ca; //Cairo rgba-colors
	double color_alpha;
	double Cr(void) {return color.get_red();}
	double Cg(void) {return color.get_green();}
	double Cb(void) {return color.get_blue();}
	double Ca(void) {return color_alpha;}
	void set_cairo_rgb(Cairo::RefPtr<Cairo::Context> & ceerr){
								ceerr->set_source_rgb(Cr(),Cg(),Cb() );
	}
	void set_cairo_rgba(Cairo::RefPtr<Cairo::Context> & ceerr){
								ceerr->set_source_rgba(Cr(),Cg(),Cb(), Ca() );
	}
	void add_datapoint(const Datapoint &datapoint){data.push_back(datapoint);}
	//void set_cairo_rgba(double red, double green, double blue, double alpha);
	void set_color_alpha(double alapha=1.0);
	std::vector<Datapoint> data;
	std::vector< std::vector<int> > multidata;
	GRAPH_TYPE graph_type;
	LEGEND_POSITION legend_position;
	Gdk::Color color;
	std::string data_name;
	bool show;
  bool operator==(DATASET &a) { //Might not work..
		if (data.empty() && a.data.empty())
			return true;
		if (data.size() != a.data.size())
			return false;
		std::vector<Datapoint>::iterator j=a.data.begin();
		for (std::vector<Datapoint>::iterator i=data.begin(); i != data.end(); ++i) {
			if (i->x_value != j->x_value)
				return false;
			if (i->y_value != j->y_value)
				return false;
			if (i->x_value_err != j->x_value_err)
				return false;
			if (i->y_value_err != j->y_value_err)
				return false;
			j++;
		}
		return true;
	}
};

void convert_histogram_and_compress(const std::vector<int> &input, DATASET &output, int compression_factor);

bool open_file_for_reading_and_popup_message_if_error(const std::string &filename, boost::filesystem::ifstream &ifstreami);
bool open_file_for_overwriting_and_popup_message_if_error(const std::string &filename, boost::filesystem::ofstream &ofstreami, bool overwrite_if_exists=false);
bool open_file_for_appending_and_popup_message_if_error(const std::string &filename, boost::filesystem::ofstream &ofstreami);
#endif
