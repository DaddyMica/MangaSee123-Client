#pragma warning(disable : 4996)
// Official MangaSee123Api Wrapper In C
// Made by King slime
// basic blueprint for wrapper  
#include <stdio.h>
#include <stdlib.h>
// 3rd party libs 
#include <cjson/cJSON.h> // for parsing JSON data
#include <curl/curl.h>   // for http requests

// Struct for main client
typedef struct MangaSeeClient {
	const char* auth_key;
	const char* api_key;
	const char* uuid;
	const char* tunnel;
	const char* host;
	int   host_status;
	// verify da session g 
	int verified_session;
	// TODO: add func ptrs to client l8r
} MangaSeeClient_t;

// For console shit
typedef struct Conio { // for debugging
	void (*log)(const char* content);
} Conio_t;

void log_f(const char* content)
{
	printf("%s\n", content);
}

Conio_t* new_conio()
{	// allocate new client pointer
	Conio_t* conio = (Conio_t*)malloc(sizeof(Conio_t*));
	// check new memory
	if (conio != NULL)
		conio->log = &log_f;

	return conio;
}	

// Requests shit 
char* new_string_buffer(size_t size)
{	// quick luh func fr 
	return (char*)calloc(size, sizeof(char));
}

size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) 
{ // callback function to get response buffer 
	size_t total_size = size * nmemb;
	char*  response   = (char*)userp;
	strcat(response, (char*)contents);

	return total_size;
}

char* post_content(char* endpoint, cJSON* json_buffer)
{ // wrapped libcurl function
	Conio_t* console       = new_conio(); // new instance
	char*    string_buffer = cJSON_Print(json_buffer);
	char* response_buffer  = new_string_buffer(100); // u needa free dis 
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
	char*  response = get("https://api.myip.com/");
	cJSON* json     = cJSON_Parse(response);
	cJSON* host     = cJSON_GetObjectItemCaseSensitive(json, "ip");

	return host->valuestring;
}

// MangaSee123Client Code
// function to check api key
int check_api_key(char* api_key, char* uuid, char* tunnel)
{
	char* endpoint = new_string_buffer(50);
	sprintf(endpoint, "%s/api/ping", tunnel);

	cJSON* json = cJSON_CreateObject();
	// add keys to object
	cJSON_AddStringToObject(json, "api_key", api_key);
	cJSON_AddStringToObject(json, "uuid", uuid);

	char* response = post_content(endpoint, json);
}

MangaSeeClient_t* new_manga_client(const char* tunnel, const char* auth_key)
{ // auth key & tunnel is needed to generate a new api-key & uuid for secure communication
	// first validate tunnel & auth_key by getting new api key from the api
	char* string_content = new_string_buffer(100);
	char* endp1			 = new_string_buffer(50);
	char* endp2          = new_string_buffer(50);
	char* host			 = get_host();
	// concat the json content
	sprintf(string_content, " { \"auth_key\": \"%s\", \"client\": \"c-0.0.1\", \"host\": \"%s\", \"os\": \"win\" } ", auth_key, host);
	sprintf(endp1, "%s/api/gateway/authorize", tunnel);
	// parse it 
	cJSON* json_buffer   = cJSON_Parse(string_content);
	char* response       = post_content(endp1, json_buffer);
	// printf("%s\n", response);
	cJSON* parsed_res    = cJSON_Parse(response);

	cJSON* new_api_key = cJSON_GetObjectItemCaseSensitive(parsed_res, "api_key");
	cJSON* new_uuid    = cJSON_GetObjectItemCaseSensitive(parsed_res, "uuid");
	cJSON* verified    = cJSON_GetObjectItemCaseSensitive(parsed_res, "authenticated");
	cJSON* host_status = cJSON_GetObjectItemCaseSensitive(parsed_res, "host_status");

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

// Entry point
void main(void)
{ // Dependencies - libcurl, cJSON
	// Made by mica
	Conio_t* conio = new_conio();

	char* tunnel   = "http://127.0.0.1:80";
	char* auth_key = "tb_is_johns_son";

	MangaSeeClient_t* manga_client = new_manga_client(tunnel, auth_key);

	conio->log("[*] Successful [*]");
	conio->log(manga_client->api_key);
	conio->log(manga_client->uuid);
	conio->log(manga_client->host);

	free(manga_client);
	free(conio);
	return;
}
