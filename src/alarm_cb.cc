/***************************************************************************
 *            alarm_cb.cc
 *
 *  Wed May  2 10:54:15 2007
 *  Copyright  2007  User
 *  Email
 ****************************************************************************/

#include "alarm_cb.hh"

alarm_cb::~alarm_cb()
{
}

//Alarm cb stuff
void alarm_cb::set_consumer(Mass_Scanning::Consumer_ptr consumeri) {
		ACE_TRACE(ACE_TEXT("set_consumer"));
		//std::cout << "before blaa" << std::endl;
		consumer = consumeri;
		//std::cout << "after blaa\n";
	}
//------------------------------------------------------------------------------
void alarm_cb::event(void) {
	ACE_TRACE(ACE_TEXT("event"));
	std::cout.setf(ios::fixed);
	Mass_Scanning::Event_ptr event = this->consumer->get_event();
	//window->set_last_bunchpack_id(event->get_measurement_id());
	ol->set_online_measurement_id(event->get_measurement_id());
	std::string iidee = ol->get_desired_online_id();
	if ( !(iidee == "EVERYTHING" || iidee == event->get_measurement_id() ) ) {
	    	// std::cout << "id was: \"" << event->get_measurement_id() << "\" - not accepted. Should be \"" << 
	        // iidee << "\"\n";
		return;
	}
	else {
	    	// std::cout << "id was: \"" << event->get_measurement_id() << "\" - ACCEPTED.\n";
	}
	
	switch(event->get_type()) {
		case Event_Data::STOP_MSG: {
			ol->online_stopped();
			// std::cout << "STOP -event" << std::endl;
		}
		break;
	case Event_Data::START_MSG: {
			const Measurement_Info info = event->get_measurement_info();
#if 0
			std::cout << "Measurement Id: " << info.get_measurement_id() << std::endl;
			std::cout << "Bins: " << info.bins << std::endl;
			std::cout << "Bin width: " << info.bin_width << std::endl;
#endif
			if ( info.bins == 0 || info.bin_width == 0 ) {
			    abort();
			}

			//std::cout << info.settings << std::endl;			
			ol->online_info(info);
			// std::cout << "START -event" << std::endl;
	}
			break;
	case Event_Data::END_MSG: {
	    // std::cout << "END -event" << std::endl;
	    ol->online_ended();
		
	}
			break;
	case Event_Data::BUNCH_PACK: {
			const Bunch_Pack bpack = event->get_bunch_pack();
			const std::list<Bunch_Data> data = bpack.get_data();
			//std::cout << "iidee : " << event->get_measurement_id() << "\n";
#if 0
	    		std::cout << "BUNCH_PACK -event" << std::endl;
	    		std::cout << "measurement_id: |" << bpack.measurement_id << "|\n";
			std::list<Bunch_Data>::const_iterator foo;
			for(foo = data.begin(); foo != data.end(); foo++) {
					std::cout << "Seqno: " << foo->seqno << std::endl;
					std::cout << "Frequency: " << foo->frequency << std::endl;
					std::cout << "Timestamp: " << foo->timestamp << std::endl;
	
					std::list<Bin_Data>::const_iterator bar;
					for(bar = foo->data.begin(); 
							bar != foo->data.end(); 
							bar++) {
							std::cout << "  (bin, count) = ("
												<< bar->bin << ", " 
												<< bar->count << ")" << std::endl;
					}
			}
#endif
			ol->online_bunchpack(data);
	}
			break;
	default:
			cout << "We dont die here.\n";
			//abort();
	}
		 // window->progressbaari->pulse();
}
//------------------------------------------------------------------------------
void alarm_cb::alarm(void) {
			ACE_TRACE(ACE_TEXT("alarm"));
			signal_thingee();
}
