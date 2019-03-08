#include "Functions.h"
#include <stdio.h>
#include <string.h>
#include "Places.h"

int search_by_name(char *input_name,  int lmark_type)
{
    int j = 1, bool_found = FALSE;
    LANDMARK lmark;
    FILE *fp_search_result;
    fp_search_result = fopen(".\\Temp\\search_result.bin", "wb");
    if(fp_search_result == NULL)
    {
        char errmsg[50];
        sprintf(errmsg, "Error %d: %s\n%s", ENOFILE, "Search_result.bin", strerror(ENOFILE));
        ErrorDialogue("Error", errmsg, 0);
        return -1;
    }

    for(j = 1; j <= NUM_AREA_G; j++)
    {
        FILE *fptr;
        char file[50];
        sprintf(file, ".\\Data\\Area%d\\%s.bin", j, GetLandmarkType(lmark_type));
        fptr = fopen(file, "rb");
        if(fptr == NULL)
        {
            char errmsg[70];
            sprintf(errmsg, "Error %d: Error in %s.\n%s", ENOFILE, file, strerror(ENOFILE));
            ErrorDialogue("File error", errmsg, 0);
            fclose(fp_search_result);
            return -1;
        }
        while(fread(&lmark, sizeof(lmark), 1, fptr))
        {
            if(!strcmp(lmark.name, "") || feof(fptr))
                break;
            char lmark_name_lower[50], input_name_lower[50];
            strcpy(lmark_name_lower, lmark.name);
            strcpy(input_name_lower, input_name);
            _strlwr(lmark_name_lower);
            _strlwr(input_name_lower);
            if(strstr(lmark_name_lower, input_name_lower))
            {
                //bool_found = TRUE;
                int fwrite_return;
                if((fwrite_return = fwrite(&lmark, sizeof(LANDMARK), 1, fp_search_result)))
                {
                    bool_found = FOUND;
                }
                else
                {
                    ErrorDialogue("Write error", "Cannot write to search file.\n", 0);
                    return -1;
                }
            }
        }
    }
    fclose(fp_search_result);
    return bool_found;
}
