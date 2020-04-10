#include <curl/curl.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
using namespace std;
//#define BUFFER_SIZE 1024

#include "curl_stuff.hh"

//------------------------------------------------------------------------------
size_t write_fun(void *ptr,  size_t size, size_t nmemb, void *stream)
{
    size_t left   = size*nmemb;
    char * buffer = (char *)stream;
    size_t written = 0;
    size_t len = 0;
    while ( left > 0 ) {
	if ( left >= BUFFER_SIZE ) {
	    len = BUFFER_SIZE - 1;
	} else {
	    len = left;
	}
	buffer[len] = '\0';
	memcpy(buffer, (char *)ptr+written, len);
//        cout << "write_fun BLAA ";
//	printf("%s", buffer);
	written += len;
	left    -= len;
    }
    return written;
}
//------------------------------------------------------------------------------
size_t header_fun(void * ptr,size_t size, size_t nmemb, void * stream)
{
    size_t left   = size*nmemb;
    char * buffer = (char *)stream;
    size_t written = 0;
    size_t len = 0;
    while ( left > 0 ) {
	if ( left >= BUFFER_SIZE ) {
	    len = BUFFER_SIZE - 1;
	} else {
	    len = left;
	}
	buffer[len] = '\0';
	memcpy(buffer, (char *)ptr+written, len);
//	cout << "header_fun BLAAP ";
//       printf("%s", buffer);
	written += len;
	left    -= len;
    }
    return written;
}
//------------------------------------------------------------------------------
bool FileFromWeb::getFileAsString(string urli,string &as_this){
//	strcpy(url, "http://www.phys.jyu.fi/research/igisol/trap/testi");
	curl_easy_setopt(curl, CURLOPT_URL, urli.c_str());

	if (curl_easy_perform(curl) == 0){
		as_this = buffer;
		as_this = as_this.substr(0,as_this.length()-1);
//		cout << as_this << "XXX\n";
		return 1;
	}
//	printf("curl_easy_perform returned %d\n", err);

	return 0;
}
