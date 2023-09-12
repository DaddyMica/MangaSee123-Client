#pragma warning(disable : 4996) // CRT_SECURE_NO_WARNINGS
#include <stdio.h>				// yk
#include <stdlib.h>				// std lib -- for dynamic memory allocation

#include "manga.h"				// manga header file
#include "mica_requests.h"		// requests lib around libcurlx86
#include "console.h"			// CONIO_H MOMENT!!


// MangaSee123Client Code
// function to check api key
int check_api_key(char* api_key, char* uuid, char* tunnel)
{	// written by mica mane 
	char* endpoint = new_string_buffer(50);
	sprintf(endpoint, "%s/api/ping", tunnel);

	cJSON* json = cJSON_CreateObject();
	// add keys to object
	cJSON_AddStringToObject(json, "api_key", api_key);
	cJSON_AddStringToObject(json, "uuid", uuid);

	char* response = post_content(endpoint, json);
	cJSON* res_json = cJSON_Parse(response);
	cJSON* test = cJSON_GetObjectItemCaseSensitive(res_json, "test");

	if (test->valueint)
		return 1;
	else
		return 0;
}

MangaSeeClient_t* new_manga_client(const char* tunnel, const char* auth_key)
{ // auth key & tunnel is needed to generate a new api-key & uuid for secure communication
	// first validate tunnel & auth_key by getting new api key from the api
	char* string_content = new_string_buffer(100); // json buffer we're gna upload 2 da server
	// endpoints
	char* endp1    = new_string_buffer(50);
	char* host	   = get_host();
	Conio_t* conio = new_conio();
	// concat the json content
	sprintf(string_content, " { \"auth_key\": \"%s\", \"client\": \"c-0.0.1\", \"host\": \"%s\", \"os\": \"win\" } ", auth_key, host);
	// format the correct endpoint
	sprintf(endp1, "%s/api/gateway/authorize", tunnel); // http://127.0.0.1:80/api/gateway/authorize
	// parse it 
	cJSON* json_buffer = cJSON_Parse(string_content);
	char*  response    = post_content(endp1, json_buffer);
	// printf("%s\n", response);
	cJSON* parsed_res  = cJSON_Parse(response);
	// get the pointers in the json obj 
	cJSON* new_api_key = cJSON_GetObjectItemCaseSensitive(parsed_res, "api_key");
	cJSON* new_uuid	   = cJSON_GetObjectItemCaseSensitive(parsed_res, "uuid");
	cJSON* verified    = cJSON_GetObjectItemCaseSensitive(parsed_res, "authenticated");
	cJSON* host_status = cJSON_GetObjectItemCaseSensitive(parsed_res, "host_status");
	
	// verify new api key!
	if (check_api_key(new_api_key->valuestring, new_uuid->valuestring, tunnel))
	{ // api key is valid!
		conio->log("[*] Verified Api Key [*]");
		// ya digg
		MangaSeeClient_t* client = (MangaSeeClient_t*)malloc(sizeof(MangaSeeClient_t*)); // new client 

		if (client == NULL)
			return NULL; // malloc failed 
		// add shit to obj 
		client->auth_key	= auth_key;
		client->api_key		= new_api_key->valuestring;
		client->uuid		= new_uuid->valuestring;
		client->tunnel		= tunnel;
		client->host		= host;
		client->host_status = host_status->valueint;

		return client;
	}
	return NULL;
}

char* mangasee_GetPage(MangaSeeClient_t* _client, char* title, int chapter, int page)
{ // Function written to get a manga page (as a url) back from the server
	// Written & Brainstormed by Mica-Chan ya digg
	// I lick on that puss on a pill
	// might add cJSON_Array support to get full chapters/mangas with one request
	if (_client->verified_session && _client->host_status) // make sure we're on a verified session
	{
		char* endpoint = new_string_buffer(50); // alloc some mem 
		// DEBUG: char* key	= new_string_buffer(100);
		// format the endp
		sprintf(endpoint, "%s/api/mangas/%s/chapters/%d/pages/%d", _client->tunnel, title, chapter, page);
		// make the json object to upload to server
		cJSON* json_headers = cJSON_CreateObject();
		// add keys to object
		cJSON_AddStringToObject(json_headers, "api_key", _client->api_key);
		cJSON_AddStringToObject(json_headers, "uuid", _client->uuid);
		// make the request 
		char* response = post_content(endpoint, json_headers);
		// parse the response
		cJSON* parsed_res = cJSON_Parse(response);
		cJSON* manga_page = cJSON_GetObjectItemCaseSensitive(parsed_res, "url");

		if (response)
			return manga_page->valuestring;
	}
	return NULL;
}