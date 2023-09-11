#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>

#include "manga.h"
#include "mica_requests.h"
#include "console.h"


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
	char* string_content = new_string_buffer(100);
	char* endp1 = new_string_buffer(50);
	char* endp2 = new_string_buffer(50);
	char* host = get_host();
	Conio_t* conio = new_conio();
	// concat the json content
	sprintf(string_content, " { \"auth_key\": \"%s\", \"client\": \"c-0.0.1\", \"host\": \"%s\", \"os\": \"win\" } ", auth_key, host);
	sprintf(endp1, "%s/api/gateway/authorize", tunnel);
	// parse it 
	cJSON* json_buffer = cJSON_Parse(string_content);
	char* response = post_content(endp1, json_buffer);
	// printf("%s\n", response);
	cJSON* parsed_res = cJSON_Parse(response);

	cJSON* new_api_key = cJSON_GetObjectItemCaseSensitive(parsed_res, "api_key");
	cJSON* new_uuid = cJSON_GetObjectItemCaseSensitive(parsed_res, "uuid");
	cJSON* verified = cJSON_GetObjectItemCaseSensitive(parsed_res, "authenticated");
	cJSON* host_status = cJSON_GetObjectItemCaseSensitive(parsed_res, "host_status");

	// verify new api key
	if (check_api_key(new_api_key->valuestring, new_uuid->valuestring, tunnel))
	{ // api key is valid!
		conio->log("[*] Verified Api Key [*]");
		// ya digg
		MangaSeeClient_t* client = (MangaSeeClient_t*)malloc(sizeof(MangaSeeClient_t*)); // new client 

		if (client == NULL)
			return NULL; // malloc failed 
		// add shit to obj 
		client->auth_key = auth_key;
		client->api_key = new_api_key->valuestring;
		client->uuid = new_uuid->valuestring;
		client->tunnel = tunnel;
		client->host = host;
		client->host_status = host_status->valueint;

		return client;
	}
	return NULL;
}