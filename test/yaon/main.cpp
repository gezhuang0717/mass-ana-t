#include <fstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/yaon_parser.hpp>
#include <boost/exception/all.hpp>
#include <boost/foreach.hpp>
#include <boost/version.hpp>

#if BOOST_VERSION >= 105300
#include <boost/multiprecision/cpp_dec_float.hpp>
typedef boost::multiprecision::number<boost::multiprecision::cpp_dec_float<20> > float20;
#endif

#include <stdint.h>

void
get_mcs_stuff(const boost::property_tree::ptree & pt,
	      uint16_t & bins,
	      uint32_t & bin_width)
{
    bins = pt.get<uint16_t>("devices.mcs.bins per record");
    bin_width = pt.get<uint32_t>("devices.mcs.bin width");
}

struct timing_s {
    uint16_t channel;
    bool enabled;
    bool clear;

    double start;
    double stop;

    timing_s() : description("") { }

    std::string description;
};
typedef struct timing_s timing_t;

typedef std::vector<timing_t> timings_t;

void get_timings(const boost::property_tree::ptree & pt, timings_t & res)
{
    const boost::property_tree::ptree & timings = pt.get_child("devices.ppg.timings");
    for (boost::property_tree::ptree::const_iterator iter = timings.begin();
	 iter != timings.end();
	 ++iter) {
	const boost::property_tree::ptree::value_type child = *iter;
	timing_t t;

	try {
	    t.enabled = child.second.get<bool>("enabled");
	} catch (boost::property_tree::ptree_bad_path & ex) {
	    t.enabled = true;
	}
	t.clear = child.second.get<bool>("in clear cycle");

	t.channel = child.second.get<uint16_t>("channel");
	t.start = child.second.get<double>("start");
	t.stop = child.second.get<double>("stop");
	res.push_back(t);
    }
}

struct rf_s {
    uint16_t number;

    std::string shape;
    std::string trigger_mode;

    double amplitude;
    double frequency;
    double phase;
    double offset;

    uint32_t burst_count;

    bool enabled;
    bool high_impedance;

    rf_s() : trigger_mode("") { }
};
typedef struct rf_s rf_t;
typedef std::vector<rf_t> rfs_t;

void get_rfs(const boost::property_tree::ptree & pt, rfs_t & res)
{
    char buf[99];
    for (int i=1; i < 42; i++) {
	try {
	    sprintf(buf, "devices.trap rf %d", i);
	    const boost::property_tree::ptree & child = pt.get_child(buf);
	    rf_t rf;
	    rf.number = i;

	    rf.amplitude = child.get<double>("amplitude");
	    rf.frequency = child.get<double>("frequency");
	    rf.phase = child.get<double>("phase");
	    rf.offset = child.get<double>("offset");

	    rf.burst_count = child.get<uint32_t>("burst count");

	    rf.enabled = child.get<bool>("enabled");
	    rf.high_impedance = child.get<bool>("high impedance");

	    rf.trigger_mode = child.get<std::string>("trigger mode");
	    rf.shape = child.get<std::string>("shape");

	    res.push_back(rf);
	} catch (boost::property_tree::ptree_bad_path & ex) {
	    break;
	}
    }
}

int main(void)
{
    boost::property_tree::ptree pt;
    std::ifstream foo;

    foo.open("settings.json");

    read_yaon(foo, pt);
    int dev = pt.get<int>("devices.mcs.bin width");
    std::cout << dev << std::endl;

    double disclevel = pt.get<double>("devices.mcs.discriminator level");
    std::cout << disclevel << std::endl;

    BOOST_FOREACH (boost::property_tree::ptree::value_type & child,
		   pt.get_child("devices.ppg.timings")) {
	std::cout.setf(std::ios::fixed, std:: ios::floatfield);
	std::cout.precision(2);
	std::cout << "channel " << child.second.get<int>("channel")
		  << ", start " << child.second.get<double>("start")
		  << ", stop " << child.second.get<double>("stop");

#if BOOST_VERSION >= 105300
	float20 asdf(child.second.get<std::string>("stop"));
	float20 bar = child.second.get<float20>("stop");
	bar += float20("0.01");
	std::cout << " ... " << bar;
#endif
	std::cout << std::endl;
    }

    timings_t ts;
    get_timings(pt, ts);
    for(std::vector<timing_t>::const_iterator iter = ts.begin();
	iter != ts.end();
	++iter) {
	const timing_t t = *iter;
	std::cout.setf(std::ios::fixed, std:: ios::floatfield);
	std::cout.precision(2);
	std::cout << "channel " << t.channel
		  << ", start " << t.start
		  << ", stop " << t.stop;
	std::cout << std::endl;
    }

    rfs_t rfs;
    get_rfs(pt, rfs);
    for(std::vector<rf_t>::const_iterator iter = rfs.begin();
	iter != rfs.end();
	++iter) {
	const rf_t rf = *iter;
	if (rf.enabled) {
	    std::cout.setf(std::ios::fixed, std:: ios::floatfield);
	    std::cout.precision(2);
	    std::cout << "rf " << rf.number
		      << ", ampl " << rf.amplitude
		      << ", freq " << rf.frequency;
	    std::cout << std::endl;
	}
    }

    return 0;
}
