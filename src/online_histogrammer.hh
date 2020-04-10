/***************************************************************************
 *            online_histogrammer.hh
 *
 *  Sun May 20 23:33:29 2007
 *  Copyright  2007  User
 *  Email
 ****************************************************************************/

#ifndef  _ONLINE_HISTOGRAMMER
#define  _ONLINE_HISTOGRAMMER

#include "config.h"

#include "window2.hh"
#include "Event_Data.h"

class Online_History {
	private:
	int bunches_to_merge;
	int bin_min,bin_max;
	Plottiarea *window;
	Plottiarea windowi;
	YksiPlottiWindow * real_window;
	std::list<Bunch_Data> *bunches;
	std::vector<TWO_INTS> *class_division;
// 	DATASET dat1,dat2;

	std::vector<Datapoint> online_history_histogram_data;
	std::vector<Datapoint> online_history_histogram_with_gates_data;
	PlotDataContainer online_history_histogram_plot_container, online_history_histogram_with_gates_plot_container;

	public:
	Online_History() {bunches_to_merge = 5; window = &windowi; real_window = new YksiPlottiWindow(window,"");}
	void initialize_ions_per_x_bunches( std::list<Bunch_Data> *online_bunches, Plottiarea *window);
	void update_ions_per_x_bunches();
	void set_merging(int merge_n_bunches){bunches_to_merge = merge_n_bunches;}
	void set_bin_gate(int minimum, int maximum, bool update_too=true);
// 	void initialize_bunches_per_class( std::list<Bunch_Data> *online_bunches, Plottiarea *window, vector<TWO_INTS> *class_boudaries);
// 	void update_bunches_per_class();
};

#endif
