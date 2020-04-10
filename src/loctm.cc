//
// loctm.cpp
// C++ class wrapper for ANSI-C struct tm
//
// written by David Nugent
//
// This code is public domain. Use for any purpose is unrestricted.
//

#include <iostream>
#include <strstream>
#include <iomanip>
#include <ctime>

#include "mjonotpp.hh"

# include "loctm.hh"
using namespace std;

char loc_tm::timech = ':';
char loc_tm::datech = '-';

int loc_tm::datefmt = loc_tm::d_IntlShort;
int loc_tm::timefmt = loc_tm::t_International;


loc_tm::loc_tm (time_t t)
{
    if (t == 0)
        t = time(0);
    *(tm *)this = *localtime(&t);
}

loc_tm::loc_tm (tm const & t)
{
    *((tm *)this) = t;
}

loc_tm::loc_tm (const string time_str_ISO)
{ //format: 2005-04-04 12:13:14
	*this = time_str_ISO;
}


loc_tm &loc_tm::operator= (tm const & t)
{
    *((tm *)this) = t;
    return *this;
}
loc_tm &loc_tm::operator= (string time_str_ISO)//format: 2005-04-04 12:13:14
{
	string whole,piece;
	struct tm time_struct;
	whole = time_str_ISO;
	int numbero;
	piece = erota(whole,'-');luvuksi(piece,numbero);
	time_struct.tm_year = numbero  - 1900;
	piece = erota(whole,'-');luvuksi(piece,numbero);
    time_struct.tm_mon = numbero - 1;
	piece = erota(whole,' ');luvuksi(piece,numbero);
    time_struct.tm_mday = numbero;
	piece = erota(whole,':');luvuksi(piece,numbero);
    time_struct.tm_hour = numbero;
	piece = erota(whole,':');luvuksi(piece,numbero);
    time_struct.tm_min = numbero;
	luvuksi(whole,numbero);
    time_struct.tm_sec = numbero;
    time_struct.tm_isdst = -1;
	*this = time_struct;
	return *this;
}


int
loc_tm::compare (loc_tm const & t) const
{
    return compare ((time_t)t);
}

int
loc_tm::compare (time_t const tt) const
{
    time_t tx = (time_t)*this;
    return (tx == tt) ? 0 : (tx > tt) ? 1 : -1;
}

loc_tm::operator time_t (void) const
{
    return mktime ((tm *)this);
}

int
loc_tm::is_valid (void) const
{
    static int maxd[] =
    {
        31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };

    return (tm_year > 0 &&
            tm_mon >= 0 && tm_mon < 12 &&
            tm_mday > 0 && tm_mday <= maxd[tm_mon] &&
            tm_wday < 7 && tm_yday < 367 &&
            tm_sec < 60 && tm_min < 60 && tm_hour < 24);
}

void
loc_tm::tSfx (ostream & os, int fmt, char ch) const
{
    if (fmt & t_SepAbbrev)
        os << ch;
    if (fmt & t_SepChar)
        os << timech;
    if (fmt & t_SepSpace)
        os << ' ';
}

void
loc_tm::pHour (ostream & os, int fmt) const
{
    if (fmt & t_Hour)
    {
        int hour = tm_hour;
        if (!(fmt & t_24hour))
        {
            if (hour > 12)
                hour -= 12;
            else if (!hour && (fmt & t_AmPm))
                hour += 12;
        }
        os << setw((fmt & t_PadHour) ? 2 : 0);
        os << setfill((fmt & t_ZeroHour) ? '0' : ' ');
        os << hour;
        if (!(fmt & t_Mins))
            fmt &= t_SepAbbrev;
        tSfx (os, fmt, 'h');
    }
}

void
loc_tm::pMins (ostream & os, int fmt) const
{
    if (fmt & t_Mins)
    {
        int min = tm_min;
        int dig = 2;
        if (!(fmt & t_Hour))
        {
            min += (tm_hour * 60);
            dig += 2;
        }
        os << setw((fmt & t_PadMins) ? dig : 0);
        os << setfill((fmt & t_ZeroMins) ? '0' : ' ');
        os << min;
        if (!(fmt & t_Secs))
            fmt &= t_SepAbbrev;
        tSfx (os, fmt, 'm');
    }
}

void
loc_tm::pSecs (ostream & os, int fmt) const
{
    if (fmt & t_Secs)
    {
        int sec = tm_sec;
        int dig = 2;
        if (!(fmt & (t_Hour|t_Mins)))
        {
            sec += ((tm_hour * 60) + tm_min) + 60;
            dig += 3;
        }
        os << setw((fmt & t_PadSecs) ? dig : 0);
        os << setfill((fmt & t_ZeroSecs) ? '0' : ' ');
        os << sec;
        if (fmt & t_AmPm)
            fmt &= ~t_SepChar;
        else
            fmt &= (t_SepAbbrev|t_SepSpace);
        tSfx (os, fmt, 's');
    }
}


ostream &
loc_tm::printTime (ostream & os, int fmt) const
{
    ostrstream pTmp;
    pHour (pTmp, fmt);
    pMins (pTmp, fmt);
    pSecs (pTmp, fmt);
    if (fmt & t_AmPm)
        pTmp << (tm_hour > 11 ? "pm" : "am");
    pTmp << ends;

    char const * p = pTmp.str();
    pTmp.rdbuf()->freeze(0);
    return os << p;
}

void
loc_tm::dSfx (ostream & os, int fmt) const
{
    if (fmt & d_SepChar)
        os << datech;
    if (fmt & d_SepSpace)
        os << ' ';
}

void
loc_tm::pYear (ostream & os, int fmt) const
{
    if (fmt & d_Year)
    {
        int year = tm_year;
        int dig;
        if (fmt & d_YearShort)
        {
            dig = 2;
            year %= 100;
        }
        else
        {
            dig = 4;
            if (year < 200)
                year += 1900;
        }
        os << setw((fmt & d_PadYear) ? dig : 0);
        os << setfill((fmt & d_ZeroYear) ? '0' : ' ');
        os << year;
        if ((fmt & d_YearFirst))
        {
            fmt &= (d_SepChar|d_SepSpace);
            dSfx (os, fmt);
        }
    }
}

void
loc_tm::pMonth (ostream & os, int fmt) const
{

    static const char * _months[] =
    {
        "January", "February", "March", "April",
        "May", "June", "July", "August", "September",
        "October", "November", "December"
    };

    if (fmt & d_Month)
    {
        int mon = (tm_mon % 12);
        if (fmt & d_MonText)
        {
            char const * tmon = _months[mon];
            if (!(fmt & d_PadMon))
                os << tmon;
            else
                for (int x = 0; x < 3; ++x)
                    os << tmon[x];
        }
        else
        {
            ++mon;
            os << setw((fmt & d_PadMon) ? 2 : 0);
            os << setfill((fmt & d_ZeroMon) ? '0' : ' ');
            os << mon;
        }
        if (((fmt & d_Year) && !(fmt & d_YearFirst)) ||
            ((fmt & d_Day) && (fmt & d_MonFirst)))
        {
            fmt &= (d_SepChar|d_SepSpace);
            dSfx (os, fmt);
        }
    }
}


void
loc_tm::pDate (ostream & os, int fmt) const
{
    if (fmt & d_Day)
    {
        int day = tm_mday;
        os << setw((fmt & d_PadDay) ? 2 : 0);
        os << setfill((fmt & d_ZeroDay) ? '0' : ' ');
        os << day;
        if (!(fmt & d_YearFirst) || !(fmt & d_MonFirst))
        {
            fmt &= (d_SepChar|d_SepSpace);
            dSfx (os, fmt);
        }
    }
}


ostream &
loc_tm::printDate (ostream & os, int fmt) const
{
    ostrstream pTmp;

    static const char * _days[] =
    {
        "Sunday", "Monday", "Tuesday", "Wednesday",
        "Thursday", "Friday", "Saturday"
    };

    if (fmt & d_DayOfWeek)
    {
        int day = tm_wday % 7;
        char const * p = _days[day];
        if (fmt & d_PadDay)
            for (int x = 0; x < 3; ++x)
                pTmp << p[x];
        else
        {
            pTmp << p;
            if (fmt & d_DMY)
                pTmp << ',';
        }
        if ((fmt & d_DMY) && fmt & d_SepSpace)
            pTmp << ' ';
    }
    if (fmt & d_YearFirst)
        pYear (pTmp, fmt);
    if (fmt & d_MonFirst)
        pMonth (pTmp, fmt);
    pDate (pTmp, fmt);
    if (!(fmt & d_MonFirst))
        pMonth (pTmp, fmt);
    if (!(fmt & d_YearFirst))
        pYear (pTmp, fmt);
    pTmp << ends;

    char const * p = pTmp.str();
    pTmp.rdbuf()->freeze(0);
    return os << p;
}

//Just a function to get average of times...
string loc_tm::AverageWith(const loc_tm &a){
	long int aa = time_t (a);// cout << "aa " << aa << endl;
	long int bb = time_t (*this);// cout << "this " << bb << endl;
	long int aver = aa/2+bb/2;// cout << "aver " << aver << endl;
	loc_tm ave(aver);

	ostringstream oss;
	oss << loc_date(ave,loc_tm::d_oma) << " " << loc_time(ave,loc_tm::t_International) << ' ';
	string as_string;
	as_string.append( ((string)oss.str()).c_str(),19 );
	return as_string;
}
string loc_tm::get_time_as_string() {
	ostringstream oss;
	oss << loc_date(*this,loc_tm::d_oma) << " " << loc_time(*this,loc_tm::t_International) << ' ';
	string as_string;
	as_string.append( ((string)oss.str()).c_str(),19 );
	return as_string;

}
string loc_tm::get_just_time_as_string() {
	ostringstream oss;
	oss << loc_time(*this,loc_tm::t_International) << ' ';
	string as_string;
	as_string.append( ((string)oss.str()).c_str(),8 );
	return as_string;

}
