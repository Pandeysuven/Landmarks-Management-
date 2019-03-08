#include "Functions.h"
#include <stdio.h>

void DisplayMemoryAllocationError()
{
	char errmsg[50];
	sprintf(errmsg, "Error %d: %s", ENOMEM, strerror(ENOMEM));
	MessageBox(NULL, errmsg, "Memory Allocation", MB_OK | MB_ICONEXCLAMATION);

}

void DisplayFileError(char *file_name)
{
	char errmsg[50];
	char *file = calloc(strlen(file_name) + 1, sizeof(char));
	if (file == NULL)
	{
		DisplayMemoryAllocationError();
		exit(-1);
	}
	strcpy(file, file_name);
	sprintf(errmsg, "Error %d: Error in %s.\n%s", ENOFILE, file, strerror(ENOFILE));
	MessageBox(NULL, errmsg, "File error", MB_OK | MB_ICONEXCLAMATION);
}
