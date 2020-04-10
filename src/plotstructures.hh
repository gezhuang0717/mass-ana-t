#ifndef __plotstructures_hh
#define __plotstructures_hh

#include "guiutil.hh"

class PlottingOptions
{
public:
    double color_alpha;
    double Cr ( void ) {
        return color.get_red_p();
    }
    double Cg ( void ) {
        return color.get_green_p();
    }
    double Cb ( void ) {
        return color.get_blue_p();
    }
    double Ca ( void ) {
        return color_alpha;
    }
    void set_cairo_rgb ( const Cairo::RefPtr<Cairo::Context> & ceerr ) {
        ceerr->set_source_rgb ( Cr(),Cg(),Cb() );
    }
    void set_cairo_rgba ( const Cairo::RefPtr<Cairo::Context> & ceerr ) {
        ceerr->set_source_rgba ( Cr(),Cg(),Cb(), Ca() );
    }
    void set_color_alpha ( double alapha=1.0 ){color_alpha = alapha;};
    GRAPH_TYPE graph_type;
    LEGEND_POSITION legend_position;
    Gdk::Color color;
    std::string data_name;

    PlottingOptions ( void ) {
        color_alpha = 1.0;
        color = Gdk::Color ( "Red" );
        graph_type = LINE;
        legend_position = TOP_LEFT;
    }
};


class PlotDataContainer : public PlottingOptions
{
private:
	bool compressed_data_needs_to_be_generated;
	int data_compression_factor;
    std::vector<Datapoint> * data;
    std::vector<Datapoint> modified_data;

    std::vector<Datapoint *> data_within_some_range;
    void generate_smoothed_data();
	void generate_compressed_data();
public:
    void set_data ( std::vector<Datapoint> * this_data );
    std::vector<Datapoint> * get_data ( void );

    std::vector<Datapoint * > * get_data_within_range ( double x1, double x2 );
	void set_data_compression_factor( int compression_factor = 1 );
    PlotDataContainer();
    ~PlotDataContainer(){ }
    
	bool operator == (const PlotDataContainer &a)
	{
		return data_name == a.data_name;
	}
};






#endif
