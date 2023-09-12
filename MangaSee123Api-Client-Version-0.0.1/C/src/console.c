#include <stdio.h>

#include "console.h"

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