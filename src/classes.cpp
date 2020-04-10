#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <string.h>
#include <sstream>
#include "classes.h"
using namespace std;

CIsotope CIsotope::operator= (CIsotope isotope)
{
  isotope.A = A;
  isotope.mass = mass;
  isotope.elem = elem;
  isotope.ME = ME;
  return *this;
}

void CIsotope::make_isotope(string row)
{
  string substr1(row,10,6); // "186Tam", nubase2016.txt
  int t = substr1.find_first_not_of("0123456789");
  A = atoi(substr1.substr(0,t).c_str());
  elem = substr1.substr(t,substr1.size()-t+1);
//   std::cout << "elem: " << elem << "\n";
  string substr2(row,17,11); // mass excess nubase2016.txt
  size_t s = elem.find(" ");
  if (s != string::npos) elem.erase(s);
  ME = atof(substr2.c_str());
  mass = A * AMU + ME ;
//   cout << "elem: " << elem << ", A: " << A << ", ME:" << ME << ", mass: " << mass << "\n";
}

string CIsotope::get_name()
{
  char buffer[64];
  sprintf(buffer,"%d",A);
  string name = buffer + elem;
  return(name);
}

void CIsotope::unmake_name(string n)
{
  size_t s = n.find_first_of("1234567890");
  size_t t = n.find_last_of("1234567890");
  size_t u = n.find_first_not_of("1234567890");
  size_t v = n.find_last_not_of("1234567890");
  A = atoi(n.substr(s,t-s+1).c_str());
  elem = n.substr(u,v-u+1);
}

double CIsotope::calc_freq(const CRef &cref)
{
  return (cref.get_mass()-M_e) * cref.get_freq()/(mass-M_e);
}

void CRead::count_lines()
{
  ifstream xfile;
  string row;
  int counter = 0;
  xfile.open(filename.c_str());
  if (!xfile) cout << filename << " could not be opened to count lines\n";
  while (!xfile.eof())
    {
      getline(xfile,row);
      counter++;
    }
  xfile.close();
  lines = counter-1;
  //      cout << lines << " lines in " << filename << "\n";
}


string CRead::read_line(int linenumber)
{
  ifstream xfile;
  string row;
  xfile.open(filename.c_str());
  if (!xfile) cout << filename << " could not be opened\n";
  for (int i=0;i<linenumber+1;i++) getline(xfile,row);
  xfile.close();
  return(row);
}


CMolecule CMolecule::operator= (CMolecule molecule)
{
  molecule.name = name;
  molecule.mass = mass;
  return *this;
}

double CMolecule::get_freq(CRef cref, int q)
{
  double freq = (cref.get_mass()-M_e) * (q/1.)* cref.freq/(get_mass()-q*M_e);
//   cout << "q: " << q << ", el mass: " << M_e << "\n";
//   cout << "Ref  mass: " << cref.get_mass() << ", ref_freq:  " << cref.freq << "\n";
//   cout << "meas mass: " <<      get_mass() << ", meas_freq: " <<      freq << "\n";
//   cout << "Ref ion  mass: " << cref.get_mass()-M_e << ", ref_freq:  " << cref.freq << "\n";
//   cout << "meas ion mass: " <<      get_mass()-M_e << ", meas_freq: " <<      freq << "\n";
//   cout << "---\n";
  return freq;
}

CMolecule CMolecule::operator+ (CIsotope isotope)
{
  mass += isotope.get_mass();
  name += isotope.get_name();
  return *this;
}

void CMolecule::unmake_mol(string mol)
{
  if (mol.size()==0)
    return;
  size_t s = 0;
  size_t t = 0;
  do
  {
    mol = mol.substr(s,mol.size()-s);
    s = mol.find_first_not_of("1234567890");
    if (s==string::npos)
    {
      cref.push_back(atoi(mol.c_str()));
      cref[cref.size()-1].set_elem("StupidUsers");
      cref[cref.size()-1].name = cref[cref.size()-1].get_name();
      cref[cref.size()-1].read_ame();
      break;
    }
    t = mol.size();
    int tempA = atoi(mol.substr(0,s).c_str());
    mol = mol.substr(s,t-s);
    s = mol.find_first_of("1234567890");
    string tempelem = mol.substr(0,s);
    // Remove any and all higher case letters
    std::transform(tempelem.begin(), tempelem.end(), tempelem.begin(), ::tolower);
    cref.push_back(tempA);
    cref[cref.size()-1].set_elem(tempelem);
    cref[cref.size()-1].name = cref[cref.size()-1].get_name();
    cref[cref.size()-1].read_ame();
  } while ( s != string::npos);
}

void CMolecule::cluster(int size)
{
	mass = size*12*AMU;
	char csize[64];
	sprintf(csize,"%dxC12",size);
	name = csize;
}

void CRef::make_ref(string filename)
{
  ifstream xfile;
  xfile.open(filename.c_str());
  if (!xfile){cout << "could not read crap\n"; exit(0);}
  while(!xfile.eof()){
    string row;
    getline(xfile,row);
    if (row[0]=='?') continue;
    int s = row.find(':');
    int t = row.find('?');
    string substr1(row,0,s);
    string substr2(row,s+1,t-s-1); 
    if (substr1=="ref_mass") set_A(atoi(substr2.c_str()));
    if (substr1=="ref_element") set_elem(substr2);
    if (substr1=="ref_frequency") freq = atof(substr2.c_str());
  }
  xfile.close();
}

void CRef::read_ame()
{
  string nubtab("nubtab03.asc");
  
  // Switch to new ame2016
  nubtab = "nubase2016.txt";
  cout << "Reading AME file ... " << nubtab << "\n";
  char fname[256];
  string row;
  ifstream afile;
  int check =0;
  sprintf(fname,"%s",nubtab.c_str());
  afile.open(fname);
  if (!afile)
  {
    cout << fname << " could not be opened\n";
    exit(0);
  }
  while (!afile.eof() && check <1)
    {
      getline(afile,row);
      if (row.size()<30) continue;
      string substr1(row,10,6); //  " 23Mg"; substr1(row,11,6) for nubtab 03, 10 for nubase2016
//       string substr3(row,29,9);  
      
      // For debugging
//       cout << "substr1: " << substr1 << "\n";
      size_t s = substr1.find(" ");
      if (s != string::npos) substr1.erase(s);
      
      // We will make all letters lowercase
      
      std::transform(substr1.begin(),substr1.end(), substr1.begin(), ::tolower);
      
      if (substr1 == name)
	{
          string substr2(row,17,11); // Mass excess
// 	  cout << "substr2: " << substr2 << "\n";
	  set_ME(atof(substr2.c_str()));
	  check++;
	}
    }	  
  afile.close();
  set_mass(get_A() * AMU + get_ME());
  if (check==0) set_mass(0);
}


void CCont::make_masses(CRef cref)
{
  mass_lower = (cref.get_mass()-M_e)*q*cref.get_freq()/(cont_freq+cont_freq_error)+M_e;
  mass_higher = (cref.get_mass()-M_e)*q*cref.get_freq()/(cont_freq-cont_freq_error)+M_e;
  A_limit = (mass_higher/AMU)+3; 
}

int CCont::compare1(CMolecule molecule, int count, int i)
{
  int c = (molecule.get_mass()<i*(mass_lower-0.6*AMU)/(count*1.0))? 10:0 ;
  int d = (molecule.get_mass()>(mass_higher+0.6*AMU))? 1:0 ;
  return(d+c);
}

int CCont::compare2(CMolecule molecule)
{
  int c = (molecule.get_mass()>=mass_lower && molecule.get_mass()<=mass_higher)? 1:0 ;
  return(c);
}

void CTable::make_table(int stable)
{
//   if (stable == 0) {filename = "nubtab03.asc"; high_limit = 4300;} // Nubase03 stuff
//   if (stable == 1) {filename = "stable.asc"; high_limit = 257;}
  
  // This is for nubase 2016, filename nubase2016.txt
  if (stable == 0)
  {
    filename = "nubase2016.txt";
    high_limit = 5632;
  }
  else if (stable == 1)
  {
    filename = "stables.txt";
    high_limit = 255; 
  }
    
} 

void CTable::ini_isotopes(double limit)
{
  CRead tread(filename);
  int i=0;
  do
    {
      isotope.push_back(tread.read_line(i));
      i++;
    }while (isotope.at(i-1).get_A()<limit && i<high_limit-1);

  high_limit=i-1;
  // cout << high_limit << "high_limit adjusted\n";
}

