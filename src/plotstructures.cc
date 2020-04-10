#include "plotstructures.hh"

PlotDataContainer::PlotDataContainer(): PlottingOptions()
{
	compressed_data_needs_to_be_generated = false;
	data_compression_factor = 1;
}

void PlotDataContainer::set_data(std::vector< Datapoint >* this_data)
{
	data = this_data;
	compressed_data_needs_to_be_generated = true;
}

std::vector< Datapoint >* PlotDataContainer::get_data(void )
{
	// std::cout << "get_data\n";
	if (data_compression_factor == 1)
		return data;
	
	if (compressed_data_needs_to_be_generated == true)
	{
		generate_compressed_data();
		// std::cout << "asked to compress\n";
	}
	return &modified_data;
}

std::vector< Datapoint* >* PlotDataContainer::get_data_within_range(double x1, double x2)
{
	if ( data->empty() )
		return &data_within_some_range;
	
	double from = std::min(x1,x2);
	double to = std::max(x1,x2);
	
	data_within_some_range.clear();
	
	for (std::vector<Datapoint>::iterator i = get_data()->begin(); i != get_data()->end(); ++i)
	{
		if (i->x_value > from && i->x_value < to)
		{
			Datapoint * toadd;
			toadd = &(*i);
			data_within_some_range.push_back(toadd);
		}
	}
	return &data_within_some_range;
}

void PlotDataContainer::set_data_compression_factor(int compression_factor)
{
	if (compression_factor != data_compression_factor)
	{
		data_compression_factor = compression_factor;
		compressed_data_needs_to_be_generated = true;
	}
}


void PlotDataContainer::generate_compressed_data()
{
	if (data->empty())
		return;

	if (data_compression_factor == 1)
	{
		compressed_data_needs_to_be_generated = false;
		return;
	}
	// std::cout << "generating..\n";
	modified_data.clear();
	int c(0);
	double x_sum(0),y_sum(0);
	Datapoint pointti; pointti.x_value_err=0; pointti.y_value_err = 0;
	for (vector<Datapoint>::const_iterator i = data->begin(); i != data->end(); i++) {
		x_sum += i->x_value;
		y_sum += i->y_value;
		c++;
		if (c == data_compression_factor) {
			pointti.x_value = x_sum/data_compression_factor; pointti.y_value = y_sum;
			modified_data.push_back(pointti);
			c = 0; x_sum = 0; y_sum = 0;
		}
	}
	
	// And the last datapoints (if there are less than < compression_factor) we just skip
	//pointti.x_value = runn; pointti.y_value = sum;
	//output.add_datapoint(pointti);
	compressed_data_needs_to_be_generated = false;
}



