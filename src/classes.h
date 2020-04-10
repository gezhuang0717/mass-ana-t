#ifndef CLASSES_H 
#define CLASSES_H

#include <vector>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <string.h>
#include <sstream>
#include <time.h>

#include <algorithm>

using namespace std;
#define AMU 931494.0038 //AME2016 in keV90
#define M_e 510.99889651 //AME2016 in keV90

class CRef;

class CIsotope
{
  int A;
  string elem;
  double mass;
  double ME;
 public:
  CIsotope(){}
  CIsotope(string row){make_isotope(row);}
  CIsotope(string el, int a, double me)
  {
    set_elem(el);
    set_A(a);
    set_ME(me);
    set_mass(a * AMU + me);
//     cout << "el: " << el << ", a: " << a << ", me: " << me << "\n";
  }
  void make_isotope(string);
  void set_mass(double m) {mass=m;}
  void set_ME(double m) {ME=m;}
  void set_A(int a){A=a;}
  void set_elem(string e){elem=e;}
  string get_elem(){return elem;}
  double get_mass() const {return mass;}
  double get_ME(){return ME;}
  int get_A(){return A;}
  CIsotope operator= (CIsotope isotope);
  string get_name();
  void unmake_name(string);
  double calc_freq(const CRef &);
};

class CRef : public CIsotope
{
  string name;
  double freq;
 public:
  CRef(){}
  CRef(string n, double f):
    name(n),
    freq(f)
      {unmake_name(name);read_ame();}
  CRef(int a){set_A(a);}
  void read_ame();
  void set_name(string n){name=n;unmake_name(name);read_ame();}
  void set_freq(double f){freq=f;}
  double get_freq() const {return freq;}
  void make_ref(string);
  friend class CMolecule;
};


class CRead{
  string filename;
  int lines;
 public:
  void count_lines();
  string read_line(int);
  int get_lines(){return lines;}
  CRead(string fn){filename=fn;count_lines();}
  CRead(){}
};

class CMolecule {
  vector<CRef> cref;
  string name;
  double mass;
  //  double freq;
 public:
  CMolecule() : name(""),mass(0){}
    //  CMolecule () {}
    CMolecule operator= (CMolecule molecule);
    double get_mass(){return mass;}
    double get_freq(CRef cref, int charge);
    CMolecule operator+ (CIsotope isotope);
    void unmake_mol(string);
    string get_name(){return name;}
    int num(){return cref.size();}
    CRef get_ref(int i){return cref.at(i);}
    void set_null(){name = "";mass = 0;}
	void cluster(int);
	void set_name(string n){name=n;}
	void set_mass(double m){mass=m;}
};

class CCont
{
  double cont_freq;
  double cont_freq_error;
  int count;
  int q;
  double mass_lower;
  double mass_higher;
  double A_limit;
 public:
  CCont(){}
  CCont(double cont, double er, int num,int charge):
    cont_freq(cont),
    cont_freq_error(er),
	count(num),
	q(charge){}
  void make_cont(string);
  void make_masses(CRef);
  int get_count(){return count;}
  double get_lower(){return mass_lower;}
  double get_higher(){return mass_higher;}
  double get_A_limit(){return A_limit;}
  int compare1(CMolecule,int,int);
  int compare2(CMolecule);
  void set_count(int c){count=c;}
};

class CTable : public CIsotope
{
  int high_limit;
  string filename;
  vector<CIsotope> isotope;
  int linecount;
 public:
  CTable(): high_limit(257),filename("stable.asc"),linecount(0){}
	CTable(int stable): high_limit(257),filename("stable.asc"),linecount(0){make_table(stable);}
	CTable(const CTable& ctable): high_limit(ctable.high_limit),filename(ctable.filename),isotope(ctable.isotope),linecount(0){}
//		CTable(vector<CIsotope> iso):isotope(iso){}
	void make_table(int);
	void ini_isotopes(double);
	void set_iso(vector<CIsotope> iso, int i){isotope=iso;high_limit=i;}
	int get_line(){return linecount;}
	void inc_line(){linecount++;}
	void reset(){linecount=0;}
	CIsotope get_isotope(){return isotope.at(linecount);}
	int get_limit(){return high_limit;}
};


class CCalibrate
{
  CRef trap1;
  CRef trap2;
  CRef timing1;
  CRef timing2;
  double time1;
  double time2;
 public:
  CCalibrate(){}
  CCalibrate(string iso1, string iso2, string iso3, string iso4, double f1, double f2, double t1, double t2):
    time1(t1), 
    time2(t2)
      {trap1.set_name(iso1);trap2.set_name(iso2);timing1.set_name(iso3);timing2.set_name(iso4);trap1.set_freq(f1); trap2.set_freq(f2);}
    CRef get_trap1(){return trap1;}
    CRef get_trap2(){return trap2;}
    CRef get_timing1(){return timing1;}
    CRef get_timing2(){return timing2;}
    double get_time1(){return time1;}
    double get_time2(){return time2;}
};

#endif /* CLASSES_H */
