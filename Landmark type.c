/*
	//functions.c
	//contains functions to modify landmark details
*/
#include "Functions.h"
#include <string.h>


int CreateLandmarkType(char *name)
{
    char *lmark_type = (char*) calloc(strlen(name) + 2, sizeof(char));
    if(lmark_type == NULL)
    {
		DisplayMemoryAllocationError();
        exit(-1);
    }
    sprintf(lmark_type, "%s\n", name);
    //Checks if file exists
    FILE *fptr = fopen(".\\Data\\Landmark_list.txt", "r");
    if(fptr == NULL)
    {
		DisplayFileError(".\\Data\\Landmark_list.txt");
        free(lmark_type);
        return -1;
    }
    if(freopen(".\\Data\\Landmark_list.txt", "a", fptr) == NULL)
    {
		DisplayFileError(".\\Data\\Landmark_list.txt");
		free(lmark_type);
		return -1;
    }

    if(SearchLandmarkType(name) == NOT_FOUND)
    {
        fputs(lmark_type, fptr);
        fclose(fptr);
        free(lmark_type);
        CreateFolder();
        return 0;
    }
    else
    {
        free(lmark_type);
        fclose(fptr);
        return 1;
    }
}

char* GetLandmarkType(int sn)
{
    if(sn < 0)
    {
        char errmsg[50];
        sprintf(errmsg, "Error %d: %s to GetLandmarkType( ).", EINVAL, strerror(EINVAL));
        ErrorDialogue("Error", errmsg, 0);
        return "";
    }

    FILE *fptr = fopen(".\\Data\\Landmark_list.txt", "r");
    if(fptr == NULL)
    {
		DisplayFileError(".\\Data\\Landmark_list.txt");
        return "";
    }

    int sz = 50;
    int num_line = 1;
    char *line = (char*) calloc(sz, sizeof(char));
    if(line == NULL)
        {
			DisplayMemoryAllocationError();
			fclose(fptr);
            exit(-1);
        }
    while(fgets(line, sz, fptr))
    {
    	if(line[strlen(line) - 1] != '\n' && !feof(fptr))   //If complete line is not read
            {
                if(sz > MAXIMUM_INPUT_SIZE)
                {
                    char errmsg[50];
                    sprintf(errmsg, "Line %d of landmark list is too long..", num_line);
                    ErrorDialogue("Error", errmsg, 0);
					fclose(fptr);
					free(line);
                    return "";
                }

                fseek(fptr, -strlen(line), SEEK_CUR);
                sz *= 2;
				char *tmp_realloc;
				if ((tmp_realloc = realloc(line, sz)) == NULL)
				{
						DisplayMemoryAllocationError();
						fclose(fptr);
						free(line);
						exit(-1);
				}
				else
				{
					line = tmp_realloc;
				}
                continue;
            }

        if(num_line < sn)
        {
                num_line++;
        }
        else if(num_line == sn)
        {
            fclose(fptr);
            if(line[strlen(line)-1] == '\n')   //Removes \n at end if present
                line[strlen(line)-1] = '\0';
            if(!strcmp(line, ""))
            {
                free(line);
                return "";
            }
            else
                return line;
        }

    }
    fclose(fptr);
    free(line);
    return "";
}

int SearchLandmarkType(char* input)
{
    char *input_name = calloc(strlen(input) + 2, sizeof(char));
    if(input_name == NULL)
    {
		DisplayMemoryAllocationError();
        exit(-1);
    }
    sprintf(input_name, "%s\n", input);

    FILE *fptr = fopen(".\\Data\\Landmark_list.txt", "r");
    if(fptr == NULL)
    {
		DisplayFileError(".\\Data\\Landmark_list.txt");
        free(input_name);
        return -1;
    }

int sz = 50, num_line = 1;
    char *line = (char*) calloc(sz, sizeof(char));
    if(line == NULL)
    {
		DisplayMemoryAllocationError();
        free(input_name);
        fclose(fptr);
        exit(-1);
    }
    while(fgets(line, sz, fptr))
    {
        if(line[strlen(line) - 1] != '\n' && !feof(fptr))
        {
            if (sz > MAXIMUM_INPUT_SIZE)
			{
				free(input_name);
				free(line);
				fclose(fptr);
				 char errmsg[50];
				sprintf(errmsg, "Line %d of landmark list is too long.", num_line);
				ErrorDialogue("Error", errmsg, 0);
				return -1;
			}
			fseek(fptr, -strlen(line), SEEK_CUR);
			sz *= 2;
			char *tmp_realloc;
			if ((tmp_realloc = realloc(line, sz)) == NULL)
				{
						DisplayMemoryAllocationError();
						fclose(fptr);
						free(line);
						free(input_name);
						exit(-1);
				}
			else
			{
				line = tmp_realloc;
			}
        }

        if(!strcmpi(input_name, line))
        {
            free(input_name);
            free(line);
            fclose(fptr);
            return FOUND;
        }

    }

    free(input_name);
    free(line);
    fclose(fptr);
    return NOT_FOUND;
}




char* GetAreaName(int sn)
{
    if(sn < 0)
    {
        char errmsg[50];
        sprintf(errmsg, "Error %d: %s to GetAreaname( ).", EINVAL, strerror(EINVAL));
        ErrorDialogue("Error", errmsg, 0);
        return "";
    }

    FILE *fptr = fopen(".\\Data\\Area_list.txt", "r");

    if(fptr == NULL)
    {
		DisplayFileError(".\\Data\\Area_list.txt");
        return "";
    }

    int sz = 50;
    int num_line = 1;
	char *line = (char*)calloc(sz, sizeof(char));
	if (line == NULL)
	{
		DisplayMemoryAllocationError();
		fclose(fptr);
		exit(-1);
	}
    while(fgets(line, sz, fptr))
    {
		if (line[strlen(line) - 1] != '\n' && !feof(fptr)) //Complete line not read
		{
			if (sz > MAXIMUM_INPUT_SIZE)
			{
				char errmsg[50];
				sprintf(errmsg, "Line %d of landmark list is too long.", num_line);
				ErrorDialogue("Error", errmsg, 0);
				free(line);
				fclose(fptr);
				return "";
			}

			fseek(fptr, -strlen(line), SEEK_CUR);
			sz *= 2;
			char *tmp_realloc;
			if ((tmp_realloc = realloc(line, sz)) == NULL)
			{
				DisplayMemoryAllocationError();
				free(line);
				fclose(fptr);
				exit(-1);
			}
			else
			{
				line = tmp_realloc;
			}
			continue;
		}

        if(num_line < sn)
        {
                num_line++;
        }
        else if(num_line == sn)
        {
            fclose(fptr);
            if(line[strlen(line)-1] == '\n')
                line[strlen(line)-1] = '\0';
            if(strcmp(line, "") == STR_MATCH)
            {
                free(line);
                return "";
            }
            else
                return line;
        }
    }
    free(line);
    fclose(fptr);
    return "";


}






