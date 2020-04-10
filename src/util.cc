#include <cstring>
#include <iostream>
#include <vector>
#include <sstream>

#include "util.hh"

#if defined(WIN32) || defined(__MINGW32__)
#include <windows.h>
#include <winreg.h>
#endif

using std::string;
using std::vector;
using namespace boost::filesystem;

//------------------------------------------------------------------------------
std::string asString(double numb, int style) {
    char output[256];
    if (style == 1){
	sprintf(output,"%.4e",numb);
    }
    if (style == 2){
	sprintf(output,"%.0lf",numb);
    }
    if (style == 7){ //For freq ratio outputting
	sprintf(output,"%.10lf",numb);
    }
    if (style == 8){ //For freq ratio error
	sprintf(output,"%.2e",numb);
    }
    if (style == 9){
	sprintf(output,"%.2f",numb);
    }

    if (style == 0)  //Default
	sprintf(output,"%.4lf",numb);

	return output;
}
//------------------------------------------------------------------------------
std::string asString(int numb){
	char output[256];
	sprintf(output,"%d",numb);
	return output;
}
//------------------------------------------------------------------------------
std::string asString(unsigned int numb){
	char output[256];
	sprintf(output,"%d",numb);
	return output;
}
//------------------------------------------------------------------------------
std::string as_1f(double numb) {
	char output[256];
	sprintf(output,"%.1f",numb);
	return output;
}//------------------------------------------------------------------------------
std::string as_3f(double numb) {
	char output[256];
	sprintf(output,"%.3f",numb);
	return output;
}
//------------------------------------------------------------------------------
std::string as_2f(double numb) {
	char output[256];
	sprintf(output,"%.2f",numb);
	return output;
}//------------------------------------------------------------------------------
std::string as_0f(double numb) {
	char output[256];
	sprintf(output,"%.0f",numb);
	return output;
}
//------------------------------------------------------------------------------
double w(double delta){
	if (delta < 1e-14)
		return 1.0e11;
	return 1.0/delta/delta;
}
//------------------------------------------------------------------------------
int		as_int				(const double &numb) {
	return static_cast<int>(numb + 0.5);
}

//------------------------------------------------------------------------------
double get_phase	(double frequency,double angle) {
	return (angle/360./frequency*1.0e6);
}
//------------------------------------------------------------------------------
double	as_double		(const std::string &s) {
	return atof(s.c_str());
}
//------------------------------------------------------------------------------
bool as_bool		(const std::string &s) {
	if ( s == "1")
		return 1;
	else return 0;
}
//------------------------------------------------------------------------------
int		as_int				(const std::string &s) {
	return atoi(s.c_str());

}
//------------------------------------------------------------------------------
double get_frequency_from_string (const std::string &stringi) {
	string blaa = stringi;
	string joo = erota(blaa,' ');
	joo = erota(blaa,' ');
	joo = erota(blaa,' ');
	return as_double(joo);
}
//------------------------------------------------------------------------------
void open_file_for_reading(const boost::filesystem::path &path,
			   boost::filesystem::ifstream &ifstreami)
{
    std::ios::iostate ostate = ifstreami.exceptions();
    ifstreami.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    ifstreami.open(path);
    ifstreami.exceptions(ostate);
}
//------------------------------------------------------------------------------
void open_file_for_writing(const boost::filesystem::path &path,
			   boost::filesystem::ofstream &ofstreami)
{
    std::ios::iostate ostate = ofstreami.exceptions();
    ofstreami.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    ofstreami.open(path);
    ofstreami.exceptions(ostate);
}
//------------------------------------------------------------------------------
void open_file_for_appending(const boost::filesystem::path &path,
			     boost::filesystem::ofstream &ofstreami)
{
    std::ios::iostate ostate = ofstreami.exceptions();
    ofstreami.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    ofstreami.open(path, std::ios::app);
    ofstreami.exceptions(ostate);
}
//------------------------------------------------------------------------------
void siivoa_valilyonnit_pois(std::string & pala) {
	//cout << "tällanen: \"" << pala << "\"\n";
	if (pala.find_first_not_of(" ") == string::npos) {
		pala = "";
		return;
	}
	pala = pala.substr(pala.find_first_not_of(" "));
	if (pala.find_first_of(" ") == string::npos)
		return;
	else
 		pala=pala.substr(0,pala.find_first_of(" "));

}

void get_filenames( const boost::filesystem::path & directory, const std::string & file_extension, std::list<std::string> & matching_files)
{
	//std::cout << "Dir:" << directory << "\n";
	if( boost::filesystem::exists( directory ) )
	{
		boost::filesystem::directory_iterator end ;
		for( boost::filesystem::directory_iterator iteri(directory) ; iteri != end ; ++iteri )
		{
			if ( boost::filesystem::is_regular_file( *iteri ) )
			{
				//std::cout << iteri->path().c_str() << " (file)\n" ;
				//std::cout << iteri->path().parent_path().c_str() << "<- parent path\n";
				//std::cout << iteri->path().extension().c_str() << "<-extension\n";
				//std::cout << "|" << iteri->path().extension().string() << "|" << file_extension << "|\n";
				if (iteri->path().extension().string() == file_extension)
				{
					//std::cout << iteri->path().string() << " (file)\n" ;
					matching_files.push_back(iteri->path().string());
				}
				//std::cout << iteri->path().stem().c_str() << "<-stem\n";
			}
		}
	}
}

const char * data_files[] = {
    "fields_ISOLTRAP.txt",
    "fields_JYFLTRAP.txt",
    "fields.txt",
    "hajorajat.txt",
    "lakritsi.ini",
    "nubtab03.asc",
    "nubase.mas12",
    "ramseyer.ini",
    "ref_masses.txt",
    NULL
};

path get_share_dir(void)
{
#if defined(WIN32) || defined(__MINGW32__)
    HKEY hkey;
    int err;
    err = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		       TEXT("SOFTWARE\\JYFLTRAP\\Lakritsi"),
		       0, KEY_QUERY_VALUE, &hkey);
    if (err == ERROR_SUCCESS) {
	DWORD typ = REG_SZ;
	WCHAR buffer[1024];
	DWORD len = sizeof(buffer);
	memset(buffer, 0, sizeof(buffer));

	err = RegQueryValueEx(hkey, TEXT("InstallDir"), 0, &typ,
			      (LPBYTE)buffer, &len);
	if (err == ERROR_SUCCESS) {
	    std::wstring val(buffer);
	    return path(buffer);
	}
    }

    if (boost::filesystem::exists(path("C:/Program Files/Lakritsi (x86)"))) {
	return path("C:/Program Files/Lakritsi (x86)");
    }

    if (boost::filesystem::exists("C:/Program Files/Lakritsi")) {
	return path("C:/Program Files/Lakritsi");
    }
    return path("");
#else
    return path(LAKRITSI_DATA_DIR);
#endif
}

path get_rc_dir(void)
{
#if defined(WIN32) || defined(__MINGW32__)
    path confdir = std::getenv("USERPROFILE");
    confdir /= path("Application Data");
    confdir /= path("Lakritsi");

#if 0
    char buffer[1024];
    memset(buffer, 0, 1024);
    wcstombs(buffer, confdir.c_str(), 1024);

    std::cout << "user data dir: " << buffer << std::endl;
#endif
#else
    path confdir(getenv("HOME"));
    confdir /= path(".lakritsi");
#endif
    return confdir;
}

bool init_rc_dir(void)
{
    boost::system::error_code ec;
    bool ok;

    path sharedir = get_share_dir();
    path rcdir = get_rc_dir();

    if (not exists(rcdir, ec)) {
	ok = create_directory(rcdir);
	if (not ok) return false;
    }

    const char ** df = data_files;
    while (*df != NULL) {
	path from = sharedir / path(*df);
	path to = rcdir / path(*df);

	if (exists(from) and not exists(to, ec)) {
	    copy_file(from, to, copy_option::fail_if_exists);
	}

	df++;
    }
}

Glib::ustring
local_or_rc_file(const Glib::ustring & filename)
{
   boost::system::error_code ec;

   path file = path(filename.c_str());
   if (not exists(file, ec)) {
      file = get_rc_dir() / path(filename.c_str());
   }
   return Glib::ustring::format(file.native().c_str());
}

void
open_local_or_rc_file(
    boost::filesystem::ifstream & ifs,
    const Glib::ustring & filename,
    std::ios_base::openmode mode)
{
    boost::system::error_code ec;

    path file = path(filename.c_str());
    if (not exists(file, ec)) {
	file = get_rc_dir() / path(filename.c_str());
    }
    ifs.open(file.native(), mode);
}

void
open_local_or_rc_file(
    boost::filesystem::ofstream & ofs,
    const Glib::ustring & filename,
    std::ios_base::openmode mode)
{
    boost::system::error_code ec;

    path file = path(filename.c_str());
    if (not exists(file, ec)) {
	file = get_rc_dir() / path(filename.c_str());
    }
    ofs.open(file.native(), mode);
}


std::string
format_time_YYMMDD_hhmmss(
  boost::posix_time::ptime iTime)
{
  
  boost::posix_time::time_facet * facet(new boost::posix_time::time_facet("%Y%m%d_%H%M%S"));
  
  ostringstream oss;
  oss.imbue(std::locale(oss.getloc(), facet));
  oss << iTime;
  return oss.str();
}


std::string double_backslashify(const string &string_to_add_double_backslashes)
{
  std::string better_str;
  for(std::string::const_iterator it = string_to_add_double_backslashes.begin(); it != string_to_add_double_backslashes.end(); ++it) {
    better_str += *it;
	if (*it == '\\')
	  better_str += *it;
  }
  return better_str;
}