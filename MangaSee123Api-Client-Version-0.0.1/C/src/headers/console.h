#pragma once


// For console shit
typedef struct Conio { // for debugging
	void (*log)(const char* content);
} Conio_t;

// print func
void log_f(const char* content);
// new conio struct
Conio_t* new_conio();
