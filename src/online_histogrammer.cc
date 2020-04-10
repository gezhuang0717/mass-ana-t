/***************************************************************************
 *            online_histogrammer.cc
 *
 *  Sun May 20 23:34:09 2007
 *  Copyright  2007  User
 *  Email
 ****************************************************************************/

#include "online_histogrammer.hh"

static int
get_counts(const Bunch_Data & bunch)
{
    std::list<Bin_Data>::const_iterator bar;
    int counts(0);
    for(bar = bunch.data.begin(); bar != bunch.data.end(); bar++) {
	counts += bar->count;
    }
    return counts;
}

static int
get_counts(const Bunch_Data & bunch, int startbin, int stopbin)
{
    std::list<Bin_Data>::const_iterator bar;
    int counts(0);
    for(bar = bunch.data.begin(); bar != bunch.data.end(); bar++) {
	if (bar->bin >= startbin && bar->bin <= stopbin )
	    counts += bar->count;
    }
    return counts;
}

void Online_History::initialize_ions_per_x_bunches(std::list<Bunch_Data> *online_bunches, Plottiarea *window)
{
	bunches = online_bunches;
	this->window = window;
// 	dat1.graph_type = FANCY_HISTOGRAM;
// 	dat2.graph_type = FANCY_HISTOGRAM;
// 	dat1.color=Gdk::Color("Red");
// 	dat2.color=Gdk::Color("Blue");
	window->show_data(true);
}
//--------------------------------------------------------------------------
void Online_History::update_ions_per_x_bunches() {
	if (bunches->empty() )
		return;
	int counter(0),c1(0),gated_sum(0),all_sum(0),roundsit(1);
	online_history_histogram_data.clear(); online_history_histogram_with_gates_data.clear();
	Datapoint toAdd; toAdd.y_value_err = 0;
	//std::cout << "Here\n";
	//Next stuff for round marking 2008-01-04
	double first_frequency = as_double(bunches->begin()->frequency);
	//window->xbars.delete_bars();
	window->show_xbars(); cout.precision(11);
	window->xbars.delete_bar("rounds");
	for (std::list<Bunch_Data>::const_iterator foo = bunches->begin(); foo != bunches->end(); foo++) {
	        const Bunch_Data bunch = *foo;
	        //cout << "ero: |" << abs(as_double(bunch.frequency) - first_frequency) << "|\n";
		//cout << "first freq, now: " << first_frequency << " " << bunch.frequency << "\n";
		if ( abs(as_double(bunch.frequency) - first_frequency) < .001) {
			//cout << "Added a bar " << asString(roundsit) << "\n";
			//window->add_xbar(c1,Gdk::Color("Black"),asString(roundsit));
			window->xbars.add_bar(BAR(c1,Gdk::Color("Black"),asString(roundsit), "rounds"));
			//cout << "added\n";
			roundsit++;
		}
		gated_sum += get_counts(bunch, bin_min,bin_max);
		all_sum   += get_counts(bunch);
		counter++;
		if (counter == bunches_to_merge) {
			counter = 0;
			c1++;
			toAdd.x_value=c1; toAdd.y_value=gated_sum; online_history_histogram_with_gates_data.push_back(toAdd); gated_sum = 0;
			toAdd.x_value=c1; toAdd.y_value=all_sum;   online_history_histogram_data.push_back(toAdd);   all_sum = 0;
		}
	}
	c1++;
	toAdd.x_value=c1; toAdd.y_value=0; online_history_histogram_data.push_back(toAdd); gated_sum = 0;
	toAdd.x_value=c1; toAdd.y_value=0; online_history_histogram_with_gates_data.push_back(toAdd);   all_sum = 0;

	//std::cout << "there is " << dat1.data.size() << std::endl;
	
	online_history_histogram_with_gates_plot_container.set_data(&online_history_histogram_with_gates_data);
	online_history_histogram_with_gates_plot_container.graph_type = FANCY_HISTOGRAM;
	online_history_histogram_with_gates_plot_container.color = Gdk::Color("Red");
	online_history_histogram_with_gates_plot_container.color_alpha = 0.3;
	online_history_histogram_with_gates_plot_container.data_name = "gated history";
	window->add_data(&online_history_histogram_with_gates_plot_container);

	online_history_histogram_plot_container.set_data(&online_history_histogram_data);
	online_history_histogram_plot_container.graph_type = FANCY_HISTOGRAM;
	online_history_histogram_plot_container.color = Gdk::Color("Red");
	online_history_histogram_plot_container.color_alpha = 0.3;
	online_history_histogram_plot_container.data_name = "ungated history";
	window->add_data(&online_history_histogram_plot_container);

	
// 	dat1.set_color_alpha(0.2); dat2.set_color_alpha(0.2);
// 	window->input_data(dat1);
// 	window->input_data2(dat2);
	window->autosetPlottingBox();
	window->show_contents();
}
//--------------------------------------------------------------------------
void Online_History::set_bin_gate(int minimum, int maximum, bool update_too) {
	//std::cout << "Here\n";
	bin_min = minimum; bin_max = maximum;
	if (update_too)
		this->update_ions_per_x_bunches();
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
/*
void Online_History::initialize_bunches_per_class(std::list<Bunch_Data> *online_bunches, Plottiarea *window, std::vector<TWO_INTS> *class_boudaries)
{
	bunches = online_bunches;
	this->window = window;
	class_division = class_boudaries;
	dat1.graph_type = STACKED;
	dat1.color=Gdk::Color("Red");
	dat2.data.clear();
	window->show_data(true);
}
*/
//--------------------------------------------------------------------------
/*
void Online_History::update_bunches_per_class()
{
	if (class_division->empty() || bunches->empty() )
		return;
	vector<int> toAdd;
	vector< vector<int> > layers;

	int counter(0),more(0);
	for (std::vector<TWO_INTS>::const_iterator bar = class_division->begin(); bar != class_division->end(); bar++) {
		//cout << "Class: [" << bar->low << "," << bar->high << "]\n";
		for (std::list<Bunch_Data>::const_iterator foo = bunches->begin(); foo != bunches->end(); foo++) {
		        const Bunch_Data bunch = *foo;
			//cout << "Counts here: " << foo->get_counts();

		        if (get_counts(bunch) >= bar->low && get_counts(bunch) <= bar->high )
				more++;
			counter++;
			if (counter == bunches_to_merge) {
				//cout << "added " << more << "\n";
				toAdd.push_back(more);
				more = 0;
				counter = 0;
			}
		}
		toAdd.push_back(more);
		layers.push_back(toAdd);
		toAdd.clear();
		more = 0; counter = 0;
	}
	dat1.multidata = layers;
	window->input_data(dat1);
	window->setPlottingBox(0,0,layers.begin()->size(), bunches_to_merge+1);
	window->show_contents();
}
*/
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
