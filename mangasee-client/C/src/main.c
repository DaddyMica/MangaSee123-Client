// Official MangaSee123Api Wrapper In C
// Made by King Slime
// The Blueprint
#include <stdio.h>

// custom header files 
#include "manga.h"
#include "console.h"

// Entry point
void main(void)
{ // Dependencies - libcurl, cJSON
	// Made by mica
	Conio_t* conio = new_conio(); // logger

	char* tunnel   = "http://127.0.0.1:80"; // normally it would be on a regular tunnel but i was debuggging on locahost
	char* auth_key = "tb_is_johns_son";		// 16 byte long authentication key for unlocking api accesss 

	MangaSeeClient_t* manga_client = new_manga_client(tunnel, auth_key); // new client

	conio->log("[*] Successful [*]"); // success!
	conio->log(manga_client->api_key);
	conio->log(manga_client->uuid);
	conio->log(manga_client->host);

	return;
}