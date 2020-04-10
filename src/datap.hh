// datap.h  For single datapoint
#include <iostream>

// 2008-01-02
// If flag 'use_this' has been set to 'false'
// getCounts(..) will return zero

class cDatapoint{
  private:
	int bin;
	int counts;
	int bunch_no;
	bool use_this;
	std::string time;
	void setValues(int bini, int countti, int bunch_number){
		bin=bini;
		counts=countti;
		bunch_no=bunch_number;
	}
  public:
	cDatapoint(){
		bin=0; counts=0, bunch_no=0;use_this = true;
	}
	bool is_active() {return use_this;}
	cDatapoint(const cDatapoint &datapointti){
		bin = datapointti.bin;
		counts = datapointti.counts;
		bunch_no = datapointti.bunch_no;
		time = datapointti.time;
		use_this = datapointti.use_this;
	}
	~cDatapoint(){}
	void setValues(int bini, int countti, int bunch_number, std::string time_str, bool activete=true){
		setValues(bini,countti,bunch_number);
		time = time_str;
		use_this = true;
	}
	int getBin(void){
		return bin;
	}
	int getCounts(void){
		if (use_this)
			return counts;
		else
			return 0;
	}
	int getBunch_no(void){
		return bunch_no;
	}
	std::string getTime(void){
		return time;
	}
	void set_time(const std::string &aika) {
		time = aika;
	}
	void set_use_this(bool using_this) {
		use_this = using_this;
	}
	bool is_really_zero_and_not_excluded(void) {
		if (counts == 0 && use_this)
			return true;
		else
			return false;
	}
};
