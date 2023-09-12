// Official MangaSee123Api Wrapper In C
// Made by King Slime
// The Blueprint
// DM me on cord or tele if you're a c#/c++ dev
// disc: 1044962476543979590
// tg: @micaih8niggas
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
					// secret dev auth_key -- dont use!
	char* auth_key = "tb_is_johns_son";		// 16 byte long authentication key for unlocking api accesss 
	
	// got a ratchet in my coat, got a girl doin coke
	MangaSeeClient_t* manga_client = new_manga_client(tunnel, auth_key); // new client

	// new_manga_client_f does the 2 step handshake to attain/verify a new api key and a new uuid that goes along with the api key and host
	// manga_client->api_key; // access both on the client locally 
	// manga_client->uuid;

	// get a page of manga		 // client ptr   title   ch  pg 
	char* page = mangasee_GetPage(manga_client, "Naruto", 1, 1); // returns uri of the Naruto manga page 1 of chapter 1 from mangasee123.com
	
	return;
}