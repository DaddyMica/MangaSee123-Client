#pragma once

#ifndef MANGA_H
#define MANGA_H

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
// MangaSee123Client Code
// function to check api key
int check_api_key(char* api_key, char* uuid, char* tunnel);
// function to create new client 
MangaSeeClient_t* new_manga_client(const char* tunnel, const char* auth_key);

#endif // MANGA_H