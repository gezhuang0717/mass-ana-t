#ifndef _CURL_STUFF_HH   /* Jottei haittaa vaikka kytetn useita kertoja!       */
#define _CURL_STUFF_HH

#include <curl/curl.h>
#include <cstdio>
#include <string>
#include <iostream>
#include <sstream>

#define BUFFER_SIZE 1024

size_t header_fun(void * ptr,size_t size, size_t nmemb, void * stream);
size_t write_fun(void * ptr,size_t size, size_t nmemb, void * stream);

class FileFromWeb {
	private:
    	CURL *curl;
		int option;
   		char buffer[BUFFER_SIZE];
   		char header[BUFFER_SIZE];
   		char url[BUFFER_SIZE];
		int blaa;
	public:
		FileFromWeb() {		//Default constructor
 			curl = curl_easy_init();
    		option = 1;
			// curl_easy_setopt(curl, CURLOPT_VERBOSE, &option);
			curl_easy_setopt(curl, CURLOPT_NOPROGRESS, &option);

			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_fun);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);

			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_fun);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, header);


		}
		~FileFromWeb() {
		    curl_easy_cleanup(curl);
		}
		bool getFileAsString(std::string url, std::string &as_this);
};
#endif
