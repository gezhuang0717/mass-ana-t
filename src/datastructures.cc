#include "datastructures.hh"


void Bunch::add_ions(const IonsInBin& to_add)
{
	ions_in_bins.push_back(to_add);

	/// Update internally
	all_counts += to_add.counts;
}

unsigned int Bunch::get_counts(unsigned int startbin, unsigned int stopbin)
{
	unsigned int total_counts = 0;

	/// Iterate through all elements
	for (std::list<IonsInBin>::iterator i = ions_in_bins.begin(); i != ions_in_bins.end(); ++i)
	{
		/// In case the tof is within bin, add counts
		if ( i->bin >= startbin && i->bin <= stopbin )
			total_counts += i->counts;
	}

	return total_counts;
}

unsigned int Bunch::get_all_counts(void )
{
	/// all_counts get updated when new ions are added.
	return all_counts;
	/*
	unsigned int total_counts = 0;
	/// Iterate through all elements
	for (std::list<IonsInBin>::iterator i = ions_in_bins.begin(); i != ions_in_bins.end(); ++i)
	{
		total_counts += i->counts;
	}

	return total_counts; */
}

double Bunch::get_tof(unsigned int startbin, unsigned int stopbin)
{
	double bin_x_counts;
	for (std::list<IonsInBin>::iterator i = ions_in_bins.begin(); i != ions_in_bins.end(); ++i)
	{
		bin_x_counts += i->bin * i->counts;
	}
	bin_x_counts /= get_counts(startbin, stopbin);
}


Frequency::Frequency(unsigned int ini_frequency_mHz, Filter* ifilter)
{
	processing_needed = false;
	frequency_mHz = ini_frequency_mHz;
	ifilter = filter;
}

void Frequency::add_bunch(const Bunch& bunch_to_add)
{
	bunches.push_back(bunch_to_add);
	processing_needed = true;
}

unsigned long Frequency::get_frequency() const
{
	return frequency_mHz;
}

unsigned int Frequency::get_counts()
{
	/// Check if we're outside the frequency filter.
	if ( frequency_mHz <= filter->get_frequency_mHz_min() || frequency_mHz >= filter->get_frequency_mHz_max()  )
		return 0;

	/// And if we're inside freq filter, get the counts.

	unsigned int counts = 0;
	if ( bunches.size() != 0 )
	{
		for (std::list<Bunch>::iterator i = bunches.begin(); i != bunches.begin(); ++i)
		{
			/// 1st check if bunch is within the countrate class. For countrate class assignation,
			/// the counts can also be outside bin-window.
			unsigned int counts_in_this_bunch = i->get_all_counts();
			if ( counts_in_this_bunch >= filter->get_counts_min() && counts_in_this_bunch <= filter->get_counts_max() )
			{
				/// Add counts that are within bin-window of &filter
				counts += i->get_counts(filter->get_bin_min(), filter->get_bin_max());
			}
		}
	}

	return counts;
}

double Frequency::get_tof()
{
	/// Check if we're outside the frequency filter.
	if ( frequency_mHz <= filter->get_frequency_mHz_min() || frequency_mHz >= filter->get_frequency_mHz_max()  )
		return -1.0;

	/// And if we're inside freq filter, get the tof.
	///
	/// Each bunch has mean tof and weight (counts)
	/// To get the mean tof of all bunches within all the filters (counts, bin) we need two accumulators:
	double tof_x_counts(0.);
	unsigned int counts(0);

	unsigned int tof = 0;
	if ( bunches.size() != 0 )
	{
		for (std::list<Bunch>::iterator i = bunches.begin(); i != bunches.begin(); ++i)
		{
			/// 1st check if bunch is within the countrate class. For countrate class assignation,
			/// the counts can also be outside bin-window.
			unsigned int all_counts_in_this_bunch = i->get_all_counts();

			/// In case we're within the wanted count-window
			if ( all_counts_in_this_bunch >= filter->get_counts_min() && all_counts_in_this_bunch <= filter->get_counts_max() )
			{
				/// Get counts from the wanted bin-window
				unsigned int counts_in_this_bunch = i->get_counts(filter->get_bin_min(), filter->get_bin_max());
				/// Get TOF from the wanted bin-window
				double tof_of_this_bunch = i->get_tof(filter->get_bin_min(), filter->get_bin_max());

				/// Add to the accumulators:
				tof_x_counts += counts_in_this_bunch * tof_of_this_bunch;
				counts += counts_in_this_bunch;
			}
		}
		/// The final value is
		return (tof_x_counts / (counts*1.0) );
	}

	return -2;
}


void Frequencies::add_bunch(const Bunch& bunch_to_add, unsigned int bunch_frequency_mHz)
{

	/// First, try to find the frequency.
	bool frequency_found = false;

	/// Only try to find the frequency if there are existing ones:
	if ( frequencies.size() != 0)
	{
		/// Iterate through all different frequencies, search for a frequency match
		for (std::list<Frequency>::iterator i = frequencies.begin(); i != frequencies.end(); ++i)
		{
			/// If found, add bunch to that frequency and break out of for-loop
			if ( i->get_frequency() == bunch_frequency_mHz )
			{
				i->add_bunch(bunch_to_add);
				frequency_found = true;
				break;
			}
		}
	}
	/// In case no frequency was found, add a new frequency.
	if ( frequency_found == false )
	{
		frequencies.push_back(Frequency(bunch_frequency_mHz,&filter));
		frequencies.back().add_bunch(bunch_to_add);
	}
}

Frequencies::Frequencies()
{

}

void Frequencies::get_all_counts_as_function_of_tof(std::list< IonsInBin >& distribution)
{

}

void Frequencies::get_counts_as_function_of_tof(std::list< IonsInBin >& distribution)
{

}

void Frequencies::get_tof_matrix(std::list< IonsInBin >& distribution)
{

}

void Frequencies::set_filter(const Filter & ifilter)
{
	/// in case the filter is the same - no need to update
	if (filter == ifilter)
		return;
	filter = ifilter;

	/// Trigger recalculation of all since filter was changed
	if (frequencies.size() > 0 )
	{
	}
}

unsigned int Frequency::get_all_counts()
{

}

void Frequency::set_filter(Filter * ifilter)
{
	filter = ifilter;
}

Filter::Filter()
{
	bin_min = 0;
	bin_max = MAXBINS;

	counts_min = 0;
	counts_max = MAX_IONS_PER_BUNCH;

	frequency_mHz_min = 0;
	frequency_mHz_max = 1e11;
}

void Filter::set_bin_window(unsigned int min, unsigned int max)
{
	/// Sanity check
	if (min > max)
	{
		std::cerr << "Error setting bin window. Low bound is bigger than high.\n";
		return;
	}
	else
	{
		bin_min = min;
		bin_max = max;
	}
}

void Filter::set_count_window(unsigned int min, unsigned int max)
{
	/// Sanity check
	if (min > max)
	{
		std::cerr << "Error setting count window. Low bound is bigger than high.\n";
		return;
	}
	else
	{
		counts_min = min;
		counts_max = max;
	}
}

void Filter::set_frequency_window(long unsigned int min, long unsigned int max)
{
	/// Sanity check
	if (min > max)
	{
		std::cerr << "Error setting frequency window. Low bound is bigger than high.\n";
		return;
	}
	else
	{
		frequency_mHz_min = min;
		frequency_mHz_max = max;
	}
}

bool Filter::operator==(const Filter& rhs)
{

	if ( bin_max != rhs.bin_max || bin_min != rhs.bin_min ||
		 counts_min != rhs.counts_min || counts_max != rhs.counts_max ||
		 frequency_mHz_min != rhs.frequency_mHz_min || frequency_mHz_max != rhs. frequency_mHz_max
	   )
		return false;
	return true;
}
