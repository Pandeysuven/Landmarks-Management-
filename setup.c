//Function setup
//Sets title of output window
//Clears output window
#include "Functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <ctype.h>

int NUM_LANDMARK_TYPE_G;
int NUM_AREA_G;

void setup()
{
    system("title Landmark Management System");
	NUM_LANDMARK_TYPE_G = StripfromFile(".\\Data\\Landmark_list.txt");
	NUM_AREA_G = StripfromFile(".\\Data\\Area_list.txt");
	CreateFolder();
    system("cls");
}

int CreateFolder()
{
    FILE *fptr;
	system("IF NOT EXIST .\\Temp mkdir .\\Temp");
    for(int area_number = 1; area_number <= NUM_AREA_G; area_number++)
    {
        char mkdirs[70];
        sprintf(mkdirs, "IF NOT EXIST .\\Data\\Area%d mkdir .\\Data\\Area%d", area_number, area_number);
        system(mkdirs);

        char landmark_file[100];
		int landmark_number;
        for(landmark_number = 1; landmark_number < NUM_LANDMARK_TYPE_G; landmark_number++)
        {
            sprintf(landmark_file, ".\\Data\\Area%d\\%s.bin", area_number, GetLandmarkType(landmark_number));
            fptr = fopen(landmark_file, "rb");
            if(fptr == NULL)
            {
                fptr = fopen(landmark_file, "wb");
                if(fptr==NULL)
                {
                    char errmsg[150];
                    sprintf(errmsg, "Error: %d\nError creating file: %s.\n%s", EPERM, landmark_file, strerror(EPERM));
                    ErrorDialogue("File error", errmsg, 0);
                    exit(-1);
                }
            }
            fclose(fptr);
        }
    }
    return 0;
}

int StripfromFile(char *file_path)
{
    FILE *finput, *ftemp;
	int nlines = 0;
    finput =fopen(file_path, "r");
    if(finput == NULL)
    {
        char errmsg[100];
        sprintf(errmsg, "Error %d: %s\n%s", ENOFILE, file_path, strerror(ENOFILE));
        ErrorDialogue("File error", errmsg, 0);
        exit(-1);
    }
    ftemp = fopen(".\\Strip_temp.bin", "wb");
    if(ftemp == NULL)
    {
        char errmsg[100];
        sprintf(errmsg, "Error %d: %s\n%s", ENOFILE, "Temp file", strerror(ENOFILE));
        ErrorDialogue("File error", errmsg, 0);
        fclose(finput);
        exit(-1);
    }

    int line_sz = 50;
    char *line = (char*) calloc(line_sz, sizeof(char));
    char *realloc_temp;
    if(line == NULL)
    {
        char errmsg[100];
        sprintf(errmsg, "Error %d: %s", ENOMEM, strerror(ENOMEM));
        ErrorDialogue("Memory allocation error", errmsg, 0);
        fclose(finput);
        fclose(ftemp);
        exit(-1);
    }

    while(fgets(line, line_sz, finput))
    {
        if(!feof(finput) && line[strlen(line) - 1] != '\n')
        {
            if(line_sz>200)
            {
                char errmsg[50];
                sprintf(errmsg, "Error %d: Error in %s.\n%s", EFBIG, file_path, strerror(EFBIG));
                ErrorDialogue("File error", errmsg, 0);
                free(line);
                fclose(finput);
                fclose(ftemp);
                exit(-1);
            }
            line_sz *= 2;
            if((realloc_temp = (char*)realloc(line, line_sz)) == NULL)
            {
                char errmsg[50];
                sprintf(errmsg, "Error %d: %s", ENOMEM, strerror(ENOMEM));
                ErrorDialogue("Memory allocation error", errmsg, 0);
                free(line);
                fclose(finput);
                fclose(ftemp);
                exit(-1);
            }
            line = realloc_temp;
            fseek(finput, -strlen(line), SEEK_CUR);
            continue;
        }
        for(int c = 0; (line[c] != '\n' && line[c] != '\0'); c++)
        {
            if(!isspace(line[c]))
            {
                if(line[strlen(line)-1] != '\n')
                    strcat(line, "\n");
                fputs(line, ftemp);
                break;
            }
        }
    }
    fclose(finput);
    fclose(ftemp);

    finput = fopen(file_path, "w");
    if(finput == NULL)
    {
        char errmsg[100];
        sprintf(errmsg, "Error %d: %s\n%s", ENOFILE, file_path, strerror(ENOFILE));
        ErrorDialogue("File error", errmsg, 0);
        exit(-1);
    }
    ftemp = fopen(".\\Strip_temp.bin", "rb");
    if(ftemp == NULL)
    {
        char errmsg[100];
        sprintf(errmsg, "Error %d: %s\n%s", ENOFILE, "Temp file", strerror(ENOFILE));
        ErrorDialogue("File error", errmsg, 0);
        fclose(finput);
        exit(-1);
    }
    while(fgets(line, line_sz, ftemp))
    {
        fputs(line, finput);
		nlines++;
    }
    free(line);
    fclose(ftemp);
    fclose(finput);

    remove(".\\Strip_temp.bin");
    return nlines;
}


int StrInput(char *input_string, char *msg, int sz)
{
    int len, err = 0;
    fflush(stdin);
    while(1)
    {
        int c;
        len = 0;
        printf("%s", msg);
        while((c = getchar()) != '\n')
        {
            if(c == EOF)
                return -1;
            if(isalpha(c) || isspace(c))
            {
                if(len < sz - 1)
                {
                    err = 0;
                    input_string[len++] = c;
                }
                else
                {
                    printf("Input string too long.\n");
                    fflush(stdin);
                    err = 1;
                    break;
                }
            }
            else
            {
                printf("Invalid characters in input.\n");
                fflush(stdin);
                err = 1;
                break;

            }
        }

        if(err == 1)
        {
            continue;
        }

        if(len == 0)
        {
            printf("Input string is empty.\n");
            fflush(stdin);
            continue;
        }

        break;
    }

    input_string[len] = '\0';
    return len;
}

char* PhoneInput(char *msg)
{
    int len, limit = 10;
    char *input_string = calloc(limit+1, sizeof(char));
    if(input_string == NULL)
    {
        char errmsg[50];
        sprintf(errmsg, "Error %d: %s", ENOMEM, strerror(ENOMEM));
        ErrorDialogue("Memory allocation error", errmsg, 0);
        exit(-1);
    }

    while(1)
    {
        int c;
        len = 0;
        int pos = 0;
        printf("%s", msg);
        fflush(stdin);
        while((c = getchar()) != '\n' && len <= limit)
        {
            if(c == '-' && input_string[0] == '0' && input_string[1] == '1')
            {
                input_string[pos++] = c;
            }
            else if(isdigit(c) || c == '-')
            {
                if(c != '-')
                {
                    input_string[pos++] = c;
                    len++;
                }
            }
            else
            {
                printf("Please enter valid number.\n");
                fflush(stdin);
                continue;
            }
        }
        if(len == 0)
        {
            int empty_input_response = ErrorDialogue("Empty input", "Input is empty. Do you want to save landmark without phone number? ", MB_YESNO);
            if(empty_input_response == IDYES)
            {
                strcpy(input_string, "N/A");
                return input_string;
            }
            else
                continue;
        }
        else
            break;
    }
    input_string[len] = '\0';

    return input_string;
}

int ErrorDialogue(char *heading, char *error, int type)
{
    int option;
    if(type == 0)
    {
        option = MB_OK;
    }
    else
    {
        option = type;
    }
    int msgrtn = MessageBox(NULL, error, heading, option | MB_ICONEXCLAMATION);
    return msgrtn;
}
