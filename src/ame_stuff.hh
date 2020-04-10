#ifndef AME_STUFF_H
#define AME_STUFF_H

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <cstring>

struct hajorajat {
	int A;
	int betaminusrajaZ;
	int betaplusrajaZ;
};

struct ame_data_line_data
{
  double mass_excess;
  double mass_excess_err;
  bool is_mass_extrapolation;


  bool is_half_life_extrapolation;

  bool use_as_ndef;
  bool use_as_nrich;

  int A;
  int Z;

  ame_data_line_data() { memset(this, 0, sizeof (*this)); }

};

class ame_data_line : public ame_data_line_data {
  public:

	std::string isotope_name;

	std::string half_life_unit;
	std::string half_life;

	std::string data_source;

	ame_data_line(){}
};

class mass_table
{
  private:
	std::list<hajorajat> hajoamisrajat;
	std::string ame_filename, user_masses_filename, ref_masses_filename;


	void convert_to_mass_number_first(std::string & ar34); //converts ar34 -> 34Ar

	int get_A(const std::string & EleNum); //atom_string should be "Ar34" or "ar34"
	std::string get_elem(const std::string & EleNum); //atom_string should be "Ar34" or "ar34"

	bool read_in_ame_table(int from_A=1, int to_A=240);
	void update_mass_excess_from_other_files();


	std::list<ame_data_line> ame_data;

  public:
	mass_table(std::string i_ame_filename="nubase2016.txt",
			   std::string i_user_masses_filename="user_masses.txt",
			   std::string i_ref_masses_filename="ref_masses.txt");
	//This is the old in PostProcessor
	//const double get_mass_excess(const string &messy_element_name, double &me_error, string &found_element, bool &ok, string &source_of_value);

	//Just to get mass excess
	double get_mass_excess(const std::string &messy_element_name);
	//Just to get mass excess error
	double get_mass_excess_error(const std::string &messy_element_name);
	//get source where me was; AME or ref_masses.txt. Returns source of the FIRST.
	std::string get_mass_excess_source(const std::string &messy_element_name);


	void set_mass_table_filenames(std::string i_ame_filename="nubase2016.txt",
			   std::string i_user_masses_filename="user_masses.txt",
			   std::string i_ref_masses_filename="ref_masses.txt");

	void aseta_hajoamisrajat(const std::string & file = "hajorajat.txt");
	void set_each_to_nrich_or_ndef();


	//These methods are to be used to break up string "messy_name" like "ar34cl32o16_2_5"
	//to form A_B_C, where A are isotopes, B ion charge, C just filenumber

	std::string get_molecule_part(const std::string & messy_name);

	//returns number of atoms in molecule and atoms in vector atoms
	//returns negative value in case of error
	int break_molecule_string(const std::string & messy_name, std::vector<std::string> & atoms, bool output_as_mass_number_first = true);

	//returns charge state. From string "ar34cl34_2_5" will return 2
	//From string "ar34_2" will return 1
	int get_charge_state(const std::string & messy_name);

	//atom name should be "34Ar" or "34ar"
	double get_mass_excess_for_single_isotope(const std::string & NumEle);
	double get_mass_excess_error_for_single_isotope(const std::string & NumEle);
	std::string get_mass_excess_source_for_single_isotope(const std::string & NumEle);

	void testaa();

	//if half-life is zs,as,us or so,
	bool is_too_short_living(const std::string& t1_2);

	bool is_ndef(int Aa, int Zz);
	bool is_nrich(int Aa, int Zz);

};













#endif