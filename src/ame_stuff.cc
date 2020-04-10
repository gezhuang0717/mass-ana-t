#include <cmath>

#include "ame_stuff.hh"

#include "mjonotpp.hh"
#include "util.hh"

using namespace std;

//------------------------------------------------------------------------------
mass_table::mass_table(string i_ame_filename, string i_user_masses_filename, string i_ref_masses_filename)
{
  set_mass_table_filenames(i_ame_filename,i_user_masses_filename,i_ref_masses_filename);
  aseta_hajoamisrajat();
  read_in_ame_table(5,240);
  update_mass_excess_from_other_files();
  testaa();
}
//------------------------------------------------------------------------------
void mass_table::set_mass_table_filenames(string i_ame_filename, string i_user_masses_filename, string i_ref_masses_filename)
{
  ame_filename = i_ame_filename;
  user_masses_filename = i_user_masses_filename;
  ref_masses_filename = i_ref_masses_filename;
}
//------------------------------------------------------------------------------
/*const double mass_table::get_mass_excess(const std::string& messy_element_name, double& me_error, string& found_element, bool& ok, string& source_of_value)
{
	ifstream file;
	file.open(ame_filename.c_str());
	if (!file) {
		std::cerr << "Could not find " << ame_filename << "\n";
	}
	//Get element name from the mess that looks like: zr97_10
	string nice_element(messy_element_name),piece,piece2;
	if (messy_element_name == "") {
		found_element = "";
		me_error = -1;
		ok=false;
		return -1;
	}
	piece = erota(nice_element,'_'); //piece == zr97
	if (piece == "") {
		found_element = "";
		me_error = -1;
		ok=false;
		return -1;
	}
	int point1 = piece.find_first_of("0123456789");
	int point2 = piece.find_last_of("0123456789");
	string number_part ;
	try{
		number_part =piece.substr(point1,point2-point1+1);
//		cout << "numb: " << number_part  <<"\n";
	}
	catch (...) {
		number_part = "";
	}
	point1 = piece.find_first_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
	point2 = piece.find_last_of( "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
	string element_part;//=piece.substr(0,point);
	try{
		element_part =piece.substr(point1,point2-point1+1);
		element_part[0]=(toupper(element_part[0]));
//		cout << "elem: " << element_part  <<"\n";
	}
	catch (...) {
		element_part = "";
	}


	nice_element=number_part+element_part;  // 97zr
	//If it is ie "11cc" then it is carbon cluster with mass 11*12=132
	if (element_part == "Cc") {
		//cout << "It is cc!!\n";
		nice_element = element_part;
		ok = true;
		found_element = nice_element;
		me_error = 0.0;
		source_of_value = "carbon cluster";
		return 0.0;
	}
//	cout << "nice1 '" << nice_element << "'\n";
	//End OF ELEMENT CONVERSION

	string row, me_string, me_err_string;
	double me;

	//Next: try to find reference from file "user_masses.txt" before everything
	ifstream refs_file;
	refs_file.open(user_masses_filename.c_str());
	if (!refs_file) {
		std::cerr << "Could not find \"user_masses.txt\"\n\nYou can add your own reference mass excesses there. It uses same data format as \"ref_masses.txt\".\n\nYou should create at least an empty file named \"user_masses.txt\" to get rid of this message.";
	}
	while (getline(refs_file,row)) {
		if (row[0] == '#' || row == "")
			continue;
		piece = row.substr(0,row.find_first_of('\t'));
		//cout << "piece: \"" << piece << "\"\n";
		if (piece == nice_element) {
			piece2 = row.substr(row.find_first_of('\t')+1);
			//cout << "piece2: \"" << piece2 << "\"\n";
			me = as_double(piece2.substr(0,piece2.find_first_of('\t')));
			me_error = as_double(piece2.substr(piece2.find_first_of('\t')+1));
			//cout << "me, me_err " << me << " " << me_error << "\n";
			refs_file.close();
			found_element = nice_element;
			source_of_value = "user_masses.txt";
			ok=true;
			return me;
		}
	}
	refs_file.close();


	//Next: try to find reference from file "ref_masses.txt" first before AME
	ifstream refs2_file;
	refs2_file.open(local_or_rc_file(ref_masses_filename.c_str()));
	if (!refs2_file) {
		std::cerr << "Could not find \"ref_masses.txt\"\n\nYou can download it from http://research.jyu.fi/igisol/trap/lakritsi/";
	}
	while (getline(refs2_file,row)) {
		if (row[0] == '#' || row == "")
			continue;
		piece = row.substr(0,row.find_first_of('\t'));
		//cout << "piece: \"" << piece << "\"\n";
		if (piece == nice_element) {
			piece2 = row.substr(row.find_first_of('\t')+1);
			//cout << "piece2: \"" << piece2 << "\"\n";
			me = as_double(piece2.substr(0,piece2.find_first_of('\t')));
			me_error = as_double(piece2.substr(piece2.find_first_of('\t')+1));
			//cout << "me, me_err " << me << " " << me_error << "\n";
			refs2_file.close();
			found_element = nice_element;
			source_of_value = "ref_masses.txt";
			ok=true;
			return me;
		}
	}
	refs2_file.close();


	//NEXT: Finding it from AME tables
	int point;
	while (getline(file,row)) {
		piece = row.substr(11,6); //Element part of nubtab03 table -> '97Zr  '
		piece2 = erota(piece,' '); //Getting rid of whitespaces -> '97Zr'
//		cout << " piece2 '" << piece2 << "'\n";
		if (piece2 == nice_element) {
			//cout << "We have a match!\n";
			piece = row.substr(18,11); //Getting me-value
			point = piece.find_first_not_of(" "); piece = piece.substr(point); //Deleting leading whitespace
			point = piece.find_first_of(" "); me_string = piece.substr(0,point); //Deleting following whitespace

			piece = row.substr(29,9); //Getting me-err-value
			point = piece.find_first_not_of(" "); piece = piece.substr(point); //Deleting leading whitespace
			point = piece.find_first_of(" "); me_err_string = piece.substr(0,point); //Deleting following whitespace
//			cout << " piece '" << piece << "'\n";
			break;
		}
	}
	file.close();
	if (me_string == "") {
		cout << "Could not find matching element! Tried this: '" << nice_element << "'\n";
		found_element = nice_element;
		me_error = -1;
		me = -1;
		ok=false;
	}
	else {
		bool succesful = luvuksi(me_string, me);
		luvuksi(me_err_string, me_error);
		if (!succesful) {
			me = -2;
			ok = false;
		}
		else {
			ok=true;
			source_of_value = "AME03";
		}
	}
	found_element = nice_element;
	return me;
}*/
//------------------------------------------------------------------------------
int mass_table::break_molecule_string(const std::string& messy_name, vector< string >& atoms, bool output_as_mass_number_first)
{
  std::string mol = get_molecule_part(messy_name);

  if (mol.size()==0) return 0;
  size_t s = 0;
  size_t t = 0;
  string sinkle;
do
  {
  mol = mol.substr(s,mol.size()-s);
  s = mol.find_first_of("1234567890");
  if (s==string::npos)
  {
	return -1;
  }
  t = mol.size();
  int tempA = atoi(mol.substr(0,s).c_str());
  sinkle = mol.substr(0,s);
//try
//{
  mol = mol.substr(s,t-s);
  s = mol.find_first_not_of("1234567890");
  string tempelem = mol.substr(0,s);
  sinkle += mol.substr(0,s);
  if (output_as_mass_number_first)
	convert_to_mass_number_first(sinkle);
  //std::cout << sinkle << "\n";
  atoms.push_back(sinkle);
  } while ( s != string::npos);
}
//------------------------------------------------------------------------------
std::string mass_table::get_molecule_part(const std::string & messy_name)
{
  size_t a;

  a = messy_name.find_first_of("_");
  return messy_name.substr(0,a);

}
//------------------------------------------------------------------------------
void mass_table::testaa()
{
  string ar34="ar34";
  //convert_to_mass_number_first(ar34);
  //cout << "ar23:" << ar34 << ":\n";
  //cout << get_molecule_part("ar34cl34k35_4_3") << "\n";
/*  cout << get_charge_state("ar34cl34k35_5") << "\n";
  std::vector<std::string> moleks;
  cout << break_molecule_string("ar34cl34k35_4_3",moleks) << "\n";

  for (vector<string>::iterator ii = moleks.begin(); ii != moleks.end(); ii++) {
	cout << *ii << "\n";
	cout << ":" << get_A(*ii) << "::" << get_elem(*ii) << ":\n";
  }
*/
  return;
  for (list<ame_data_line>::iterator ii = ame_data.begin(); ii != ame_data.end(); ii++) {
	std::cout << "A:" << ii->A << ", Z:" << ii->Z
			  << ", ME:" << ii->mass_excess
			  << ", isotope:" << ii->isotope_name
			  << ", is_ndef:" << ii->use_as_ndef
			  << ", is_nrich:" << ii->use_as_nrich << "\n";
  }

}
//------------------------------------------------------------------------------
int mass_table::get_A(const std::string& EleNum)
{
  size_t s;
  s = EleNum.find_first_of("01234567890");
  return as_int(EleNum.substr(s));
}
//------------------------------------------------------------------------------
string mass_table::get_elem(const std::string& EleNum)
{
  size_t s;
  s = EleNum.find_first_of("01234567890");
  if (s == string::npos)
	return "StupidUser";
  std::string to_upp = EleNum.substr(0,s);
  to_upp[0] = toupper(to_upp[0]);
  return to_upp;
}

//------------------------------------------------------------------------------
int mass_table::get_charge_state(const std::string& messy_name)
{
  size_t s;
  s = messy_name.find_first_of("_");
  if (s == string::npos)
	return 0;
  std::string jalkiosa = messy_name.substr(s+1);
  s = jalkiosa.find_first_of("_");
  if ( s == string::npos)
	return 1;
  jalkiosa = jalkiosa.substr(0,s);
  //cout << "blaa:" << jalkiosa << ":\n";
  return as_int(jalkiosa);
}
//------------------------------------------------------------------------------
bool mass_table::read_in_ame_table(int from_A, int to_A)
{
  ame_data.clear();
  ifstream iffi;
  iffi.open(ame_filename.c_str());
  if (!iffi) {
	std::cerr << "Could not open " << ame_filename << "\n";
  }
  else {
	string row,pala;
	ame_data_line to_add;
	//to_add.data_source = "AME03";
	to_add.data_source = ame_filename;
	while (getline(iffi,row)) {
	  // In case row is too short. E.g. in nubase.mass12 60Gai, skipping.
	  if (row.size() < 80)
	    continue;


	  //Mass number A
	  pala = row.substr(0,3);
	  to_add.A = as_int(pala);

	  //Out of range of A what we want?
	  if (to_add.A < from_A || to_add.A > to_A)
		continue;

	  // 2003 2013
	  //if ( is_too_short_living(row.substr(69,2) ) )
	  // 2016
	  if ( is_too_short_living(row.substr(68,2) ) )
		continue;

	  //half-life unit 2003 2013
	  //to_add.half_life_unit = row.substr(69,2);
	  //half-life unit 2016
	  to_add.half_life_unit = row.substr(68,2);

	  //half-life 2003 2013
	  //pala = row.substr(61,7);
	  //half-life 2016
	  pala = row.substr(60,7);
	  siivoa_valilyonnit_pois(pala);
	  to_add.half_life = pala;


	  //mass excess error 2013, 2003
	  //pala = row.substr(29,9);
	  // mass excess error 2016
	  pala = row.substr(28,9);
	  to_add.mass_excess_err = as_double(pala);
	  //mass excess
	  // For nubtab03 and 2012
	  //pala = row.substr(18,11); //Mass excess string, is there #-sign?
	  // For nubase2016.txt
	  pala = row.substr(17,11); //Mass excess string, is there #-sign?
	  if (pala.find_first_of("#") == string::npos) {
		to_add.is_mass_extrapolation = false;
	  }
	  else {
		pala = pala.substr(0,pala.find_first_of("#"));
		to_add.is_mass_extrapolation = true;
	  }
	  to_add.mass_excess = as_double(pala);


	  //isotope 2003 2013
	  //pala = row.substr(11,6);
	  //isotope 2016
	  pala = row.substr(10,6);
	  siivoa_valilyonnit_pois(pala);
	  to_add.isotope_name = pala;

	  //proton number Z
	  pala = row.substr(4,3);
	  siivoa_valilyonnit_pois(pala);
	  to_add.Z = as_int(pala);

	  ame_data.push_back(to_add);
	}
	set_each_to_nrich_or_ndef();
  }

}
//------------------------------------------------------------------------------
bool mass_table::is_too_short_living(const std::string & t1_2)
{
  if (t1_2 == "ys" || t1_2 == "zs" || t1_2 == "as" || t1_2 == "fs" || t1_2 == "ps" || t1_2 == "ns" || t1_2 == "us")
	return 1;
  return 0;
}

//------------------------------------------------------------------------------
void mass_table::aseta_hajoamisrajat(const std::string & file) {
//	cout << "Blaa\n";
	boost::filesystem::ifstream ifile;

	open_local_or_rc_file(ifile, file.c_str());
	if (!ifile) {
		cout << "Ei voinu avata rajatietofileä!\n";
		return;
	}
	std::string row,pala; hajorajat toAdd;
	hajoamisrajat.clear();
	while (getline(ifile,row)) {
// 		cout << ":::" << row << "\n";
		if (row[0] == '#')
			continue;
		pala = erota(row,'\t'); toAdd.A = as_int(pala);
		pala = erota(row,'\t'); toAdd.betaplusrajaZ = as_int(pala);
		pala = erota(row,'\t'); toAdd.betaminusrajaZ = as_int(pala);
		hajoamisrajat.push_back(toAdd);
// 		cout << "Asetettiin: " << toAdd.A << " " << toAdd.betaplusrajaZ << " " << toAdd.betaminusrajaZ << "\n";
	}
	ifile.close();
}
//------------------------------------------------------------------------------
void mass_table::set_each_to_nrich_or_ndef()
{
  for (std::list<ame_data_line>::iterator i = ame_data.begin(); i!= ame_data.end(); i++) {
	if (is_ndef(i->A,i->Z))
	  i->use_as_ndef = 1;
	if (is_nrich(i->A,i->Z))
	  i->use_as_nrich = 1;
  }
}

//------------------------------------------------------------------------------
bool mass_table::is_ndef(int Aa, int Zz)
{
	if (hajoamisrajat.empty() ) {
		std::cerr << "Ei löytynyt hajorajoja!\n";
		return 0;
	}
	for (list<hajorajat>::iterator 	i=hajoamisrajat.begin(); i != hajoamisrajat.end(); ++i) {
		if (i->A != Aa)
			continue;
		if (Zz >=  i->betaplusrajaZ)
		  return 1;
		return 0;
	}

	//if not found from hajorajafile, return 1 anyway
	return 1;
}
//------------------------------------------------------------------------------
bool mass_table::is_nrich(int Aa, int Zz)
{
	if (hajoamisrajat.empty() ) {
		std::cerr << "Ei löytynyt hajorajoja!\n";
		return 0;
	}
	for (list<hajorajat>::iterator 	i=hajoamisrajat.begin(); i != hajoamisrajat.end(); ++i) {
		if (i->A != Aa)
			continue;
		if (Zz <=  i->betaminusrajaZ)
		  return 1;
		return 0;
	}

	//if not found from hajorajafile, return 1 anyway
	return 1;
}
//------------------------------------------------------------------------------
void mass_table::update_mass_excess_from_other_files()
{
	//Next: try to find reference from file "ref_masses.txt" first before AME
	string row, piece,piece2,piece3,isotope;
	ifstream refs2_file;
	refs2_file.open(local_or_rc_file(ref_masses_filename).c_str());
	if (!refs2_file) {
		std::cerr << "Could not find \"ref_masses.txt\"\n\nYou can download it from http://research.jyu.fi/igisol/trap/lakritsi/";
	}
	while (getline(refs2_file,row)) {
		if (row[0] == '#' || row == "")
			continue;
		piece = row.substr(0,row.find_first_of('\t'));
		size_t s = piece.find_first_not_of("01234567890");
		piece2=piece.substr(0,s);
		piece3=piece.substr(s);
		isotope=piece2+piece3; //AME and ref_masses is in form "34Ar"

		//Next get the mass excess
		piece= row.substr(row.find_first_of('\t')+1); //piece=ME+ME_err
		piece2= piece.substr(piece.find_first_of('\t')+1); //piece2 = ME_err
//		cout << "peice2:" << piece2 << ":\n";
		piece= piece.substr(0,piece.find_first_of('\t')); //piece=ME
//		cout << "peice:" << piece << ":\n";


		for (std::list<ame_data_line>::iterator i = ame_data.begin(); i != ame_data.end(); i++) {
		  if (i->isotope_name == isotope) {
//			std::cout << "A:" << i->A << ", Z:" << i->Z
//			  << ", ME:" << i->mass_excess
//			  << ", ME_err:" << i->mass_excess_err << "\n";
			i->mass_excess = as_double(piece);
			i->mass_excess_err = as_double(piece2);
			i->data_source = ref_masses_filename;
//			std::cout << "A:" << i->A << ", Z:" << i->Z
//			  << ", ME:" << i->mass_excess
//			  << ", ME_err:" << i->mass_excess_err
//			  << ", isotope:" << i->isotope_name
//			  << ", is_ndef:" << i->use_as_ndef
//			  << ", is_nrich:" << i->use_as_nrich << "\n";

		  }
		}

	}
	refs2_file.close();
}
//------------------------------------------------------------------------------
double mass_table::get_mass_excess(const std::string& messy_element_name)
{
  vector<std::string> isotopes;
  break_molecule_string(messy_element_name,isotopes);
  double me=0;
  for (vector<std::string>::iterator i = isotopes.begin(); i != isotopes.end(); i++) {
	//cout << "isotopes:" << *i << ":\n";
	me += get_mass_excess_for_single_isotope(*i);
  }
  return me;
}

//------------------------------------------------------------------------------
void mass_table::convert_to_mass_number_first(string& ar34)
{
  string p1 = ar34.substr(0,ar34.find_first_of("1234567890"));
  string p2 = ar34.substr(ar34.find_first_of("1234567890"));
  //cout << "p1:" << p1 << ":p2:" << p2 << ":\n";
  p1[0]=toupper(p1[0]);
  ar34 = p2+p1;

}

//------------------------------------------------------------------------------
double mass_table::get_mass_excess_for_single_isotope(const std::string& NumEle)
{

  //In case of carbon cluster
  size_t s;
  s = NumEle.find_first_not_of("0123456789");
  string elem = NumEle.substr(s);
  if (elem == "Cc" || elem == "cc")
	return 0;
  //cout << "ele:" << elem << ":\n";

  //otherwise
  for (std::list<ame_data_line>::iterator i = ame_data.begin();i!=ame_data.end();i++) {
	if (i->isotope_name == NumEle) {
	  //cout << "tama|" << NumEle << ":\n";
	  return i->mass_excess;
	}
  }
  throw std::logic_error("mass_table::get_mass_excess_for_single_isotope, " + NumEle);
  return 666666666;
}
//------------------------------------------------------------------------------
double mass_table::get_mass_excess_error(const std::string& messy_element_name)
{
  vector<std::string> isotopes;
  break_molecule_string(messy_element_name,isotopes);
  double me_err_sq=0;
  for (vector<std::string>::iterator i = isotopes.begin(); i != isotopes.end(); i++) {
	//cout << "isotopes:" << *i << ":\n";
	me_err_sq += pow(get_mass_excess_error_for_single_isotope(*i),2);
  }
  return sqrt(me_err_sq);

}

//------------------------------------------------------------------------------
double mass_table::get_mass_excess_error_for_single_isotope(const std::string& NumEle)
{
  //In case of carbon cluster
  size_t s;
  s = NumEle.find_first_not_of("0123456789");
  string elem = NumEle.substr(s);
  if (elem == "Cc" || elem == "cc")
	return 0;
  //cout << "ele:" << elem << ":\n";

  //otherwise
  for (std::list<ame_data_line>::iterator i = ame_data.begin();i!=ame_data.end();i++) {
	if (i->isotope_name == NumEle) {
	  //cout << "tama|" << NumEle << ":\n";
	  return i->mass_excess_err;
	}
  }
  throw std::logic_error("mass_table::get_mass_excess_error_for_single_isotope, " + NumEle);
  return 6666666;
}

//------------------------------------------------------------------------------
string mass_table::get_mass_excess_source(const std::string& messy_element_name)
{
  vector<std::string> isotopes;
  break_molecule_string(messy_element_name,isotopes);
  if (isotopes.empty())
      throw std::logic_error("No isotopes found");
  return ( get_mass_excess_source_for_single_isotope(*isotopes.begin()));


}
//------------------------------------------------------------------------------
string mass_table::get_mass_excess_source_for_single_isotope(const std::string& NumEle)
{
  //In case of carbon cluster
  size_t s;
  s = NumEle.find_first_not_of("0123456789");

  if ( s == string::npos || s == 0) {
        throw std::logic_error("at mass_table::get_mass_excess_source_for_single_isotope, " + NumEle);
	return "idiot";
  }
  string elem = NumEle.substr(s);
  if (elem == "Cc" || elem == "cc")
	return "carbon cluster";

  //otherwise
  for (std::list<ame_data_line>::iterator i = ame_data.begin();i!=ame_data.end();i++) {
	if (i->isotope_name == NumEle) {
	  return i->data_source;
	}
  }
  throw std::logic_error("mass_table::get_mass_excess_source_for_single_isotope, " + NumEle);
  return "not found";
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
