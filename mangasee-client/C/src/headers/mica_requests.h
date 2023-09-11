#pragma once
#include <cjson/cJSON.h>
// Requests shit 
char* new_string_buffer(size_t size);
// callback for getting response buffer
size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp);
// post function
char* post_content(char* endpoint, cJSON* json_buffer);
// get function
char* get(char* endpoint);
// ip grab.
char* get_host();