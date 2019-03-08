#include "Functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "Places.h"
#include <limits.h>

int AddLandmark(char *file_location, int area, int lmark_type)
{
    FILE *fptr = fopen(file_location, "rb");
    if(fptr == NULL)
    {
		DisplayFileError(file_location);
        return -1;
    }
    if(freopen(file_location, "ab", fptr) == NULL)
    {
		DisplayFileError(file_location);
        return -1;
    }
    LANDMARK lmark, lmark_searched;

	if ((StrInput(lmark.name, "Enter name of landmark: ", 50)) == EOF)
	{
	fclose(fptr);
	return 1;
	}

    fflush(stdin);
    FILE *fp_search;
    int search_result = search_by_name(lmark.name, lmark_type);
    if(search_result == EOF)
        return -1;
    else if(search_result == FOUND)
    {
        fp_search = fopen(".\\Temp\\search_result.bin", "rb");
        if(fp_search == NULL)
        {
			DisplayFileError(".\\Temp\\search_result.bin");
            fclose(fptr);
            return -1;
        }
        unsigned int sz = 50;
        char *landmarks = calloc(sz, sizeof(char));
        while(fread(&lmark_searched, sizeof(LANDMARK), 1, fp_search))
        {
            if((strlen(landmarks)+strlen(lmark_searched.name)) < sz-1)
            {
                strcat(landmarks, lmark_searched.name);
                strcat(landmarks, "\n");
            }
            else
            {
                sz *= 2;
                char *tmp_realloc;
                if((tmp_realloc = (char*) realloc(landmarks, sz*sizeof(char))) == NULL)
                {
                    free(landmarks);
                    fclose(fptr);
                    fclose(fp_search);
                    DisplayMemoryAllocationError();
                    exit(-1);
                }
                else
				{
					landmarks = tmp_realloc;
					fseek(fp_search, -sizeof(LANDMARK), SEEK_CUR);
					continue;
				}
            }
        }
        char *err_msg = calloc(strlen(lmark.name)+strlen(landmarks)+100, sizeof(char));
        int msgbox_return;
        sprintf(err_msg, "Following landmarks with partial match for \"%s\" already exists:\n%s Do you want to add another landmark with similar name?", lmark.name, landmarks);
        msgbox_return = ErrorDialogue("Name Conflict", err_msg, MB_YESNO);
        switch(msgbox_return)
        {
        case IDYES:
            break;
        case IDNO:
            fclose(fptr);
            fclose(fp_search);
            free(landmarks);
            return 1;
        }
        free(landmarks);
    }
    lmark.area = area;
    lmark.type = lmark_type;
    do
    {
        if(StrInput(lmark.address, "Enter address of landmark: ", 50) == EOF)
        {
            fclose(fptr);
            return 1;
        }
        if(strlen(lmark.address) <= 3)
        {
            int address_short = ErrorDialogue("Address Length", "Length of address must be greater than 3.", MB_RETRYCANCEL);
            if(address_short == IDCANCEL)
            {
                fclose(fptr);
                return 1;
            }
        }
        else
            break;
    }
    while(1);

    fp_search = fopen(".\\Temp\\search_result.bin", "rb");
    while(fread(&lmark_searched, sizeof(lmark_searched), 1, fp_search))
    {
        if(strcmpi(lmark_searched.address, lmark.address) != 0 && search_result == FOUND)
        {
            ErrorDialogue(0, "Cannot have two landmarks with same name and address.", 0);
            fclose(fptr);
            return 1;
        }
    }
    fflush(stdin);
    strcpy(lmark.phone, PhoneInput("Enter phone number: "));
    fflush(stdin);

    int write_rtn = fwrite(&lmark, sizeof(lmark), 1, fptr);
    if(write_rtn == 1)
    {
        printf("Successfully added landmark.\n");
        fclose(fptr);
        return 0;
    }
    return 1;
}

void DisplayLandmark(LANDMARK lmark)
{
    printf("Name: %s\n", lmark.name);
    printf("Address: %s\n", lmark.address);
    printf("Phone number: %s\n", lmark.phone);
}

int DeleteLandmark(char *file_path, char *lmark_name)
{
    FILE *finput, *ftemp;
    LANDMARK lmark;
    P_LANDMARK plmark = &lmark;

    finput = fopen(file_path, "rb");
    if(finput == NULL)
    {
        char errmsg[50];
        sprintf(errmsg, "Error %d: Error in %s.\n%s", ENOFILE, file_path, strerror(ENOFILE));
        ErrorDialogue("File error", errmsg, 0);
        return EOF;
    }
    ftemp = fopen(".\\Temp\\temp_delete.bin", "wb");
    if(ftemp == NULL)
    {
        char errmsg[50];
        sprintf(errmsg, "Error %d in DeleteLandmark: Error in %s.\n%s", ENOFILE, "temp file", strerror(ENOFILE));
        ErrorDialogue("File error", errmsg, 0);
        fclose(finput);
        return EOF;
    }


    while(!feof(finput))
    {
        int fread_rtn = fread(plmark, sizeof(lmark), 1, finput);
        if(fread_rtn != 1 && !feof(finput))
        {
            ErrorDialogue("Read error", "Landmark not read by DeleteLandmark( )", 0);
        }
        else if(fread_rtn == 0 && feof(finput))
        {
            break;
        }
        else if(strcmpi(lmark.name, lmark_name))
        {
            int fwrite_rtn = fwrite(plmark, sizeof(lmark), 1, ftemp);
            if(fwrite_rtn != 1)
            {
                ErrorDialogue("Write error", "Landmark not written by DeleteLandmark( )", 0);
            }
        }

    }
    fclose(ftemp);
    fclose(finput);

    finput = fopen(file_path, "wb");
    if(finput == NULL)
    {
        char errmsg[50];
        sprintf(errmsg, "Error %d: Error in %s.\n%s", ENOFILE, file_path, strerror(ENOFILE));
        ErrorDialogue("File error", errmsg, 0);
        return EOF;
    }
    ftemp = fopen(".\\Temp\\temp_delete.bin", "rb");
    if(ftemp == NULL)
    {
        char errmsg[50];
        sprintf(errmsg, "Error %d in DeleteLandmark: Error in %s.\n%s", ENOFILE, "temp file", strerror(ENOFILE));
        ErrorDialogue("File error", errmsg, 0);
        fclose(finput);
        return EOF;
    }

    while(!feof(ftemp))
    {
        int fread_rtn = fread(plmark, sizeof(lmark), 1, ftemp);
        if(fread_rtn != 1)
        {
            if(!feof(ftemp))
                ErrorDialogue("Read error", "Landmark not read by DeleteLandmark( )", 0);
            else
                break;
        }
        else if(strcmpi(lmark.name, lmark_name))
        {
            int fwrite_rtn = fwrite(plmark, sizeof(lmark), 1, finput);
            if(fwrite_rtn != 1)
            {
                ErrorDialogue("Write error", "Landmark not written by DeleteLandmark( )", 0);
            }
        }

    }

    fclose(ftemp);
    fclose(finput);
    remove(".\\Temp\\temp_delete.bin");
    return 0;
}


