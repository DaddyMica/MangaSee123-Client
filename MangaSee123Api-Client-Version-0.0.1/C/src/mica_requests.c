// requests file for client 
#pragma warning(disable : 4996)
#include <stdio.h>
#include <cjson/cJSON.h> // for parsing JSON data
#include <curl/curl.h>   // http reqs 
// header file 
#include "mica_requests.h"
#include "console.h"

// Requests shit 
char* new_string_buffer(size_t size)
{	// quick luh func fr 
	return (char*)calloc(size, sizeof(char));
}

size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp)
{ // callback function to get response buffer 
	size_t total_size = size * nmemb;
	char* response = (char*)userp;
	strcat(response, (char*)contents);

	return total_size;
}

char* post_content(char* endpoint, cJSON* json_buffer)
{ // wrapped libcurl function
	// ya digg
	// takes a cJSON pointer and url 
	Conio_t* console = new_conio(); // new instance
	char* string_buffer = cJSON_Print(json_buffer);
	char* response_buffer = new_string_buffer(100); // u needa free dis 
	// curl shi 
	CURL* curl;
	CURLcode res;
	// new curl 
	curl = curl_easy_init();

	if (curl)
	{
		// make headr ptr
		struct curl_slist* headers = NULL;
		// fill out headers
		headers = curl_slist_append(headers, "Accept: application/json");
		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(headers, "charset: utf-8");
		// add url 
		curl_easy_setopt(curl, CURLOPT_URL, endpoint);
		// add buffer to json payload 
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, string_buffer);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_buffer);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		res = curl_easy_perform(curl); // perform request
		// check response buffer to ensure successful request
		if (response_buffer[0] == '\0' && response_buffer != NULL)
			return NULL; // char* is pointing to an empty string which means our req failed :(

		return response_buffer;
	}
	else
		return NULL;
}

char* get(char* endpoint)
{ // get request function... 
	CURL* curl;
	CURLcode res;

	curl = curl_easy_init();

	if (curl)
	{
		char* response_buffer = new_string_buffer(100);

		curl_easy_setopt(curl, CURLOPT_URL, endpoint);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_buffer);

		res = curl_easy_perform(curl);

		if (response_buffer[0] == '\0' && response_buffer != NULL)
			return NULL;

		return response_buffer;
	}

	return NULL;
}

char* get_host()
{ // bet bet
	char* response = get("https://api.myip.com/");
	cJSON* json = cJSON_Parse(response);
	cJSON* host = cJSON_GetObjectItemCaseSensitive(json, "ip");

	return host->valuestring;
}