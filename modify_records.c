#include "Functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int modify_records()
{
    char options_main[][50] = { "Add a new landmark", "Edit existing landmark", "Add new type of landmark"};
    char *inputString = (char*) calloc(100, sizeof(char));
    if(inputString == NULL)
    {
        char errmsg[50];
        sprintf(errmsg, "Error %d: %s", errno, strerror(errno));
        ErrorDialogue("Memory allocation error", errmsg, 0);
        exit(-1);
    }

	while (1)

    system("cls");
    int selected_option = GetMenuSelection("Add/Modify Landmark", options_main, 3), input;
    switch(selected_option)
    {
    case 0:
        free(inputString);
        return 0;
    case '\b':
        free(inputString);
        return 0;
    case 27:
        free(inputString);
        return 0;
    case 1:
    {
        system("cls");
        char options[20][50];
        char *temp;
        char file[50];
        int num_options = 0, j, selected_landmark, selected_area;

        for(j = 1; strcmp(GetAreaName(j), "") != STR_MATCH; j++)
        {
            char area[50];
            strcpy(area, GetAreaName(j));
            strcpy(options[num_options++], area);
        }
        selected_area = GetMenuSelection("Select Area", options, num_options);
        if(selected_area == 0 || selected_area == '\b' || selected_area == RTN_ESC)
        {
            free(inputString);
            return 0;
        }
        num_options = 0;
        j = 1;
        {
            while((strcmp((temp = GetLandmarkType(j++)), "")) != STR_MATCH)
            {
                strcpy(options[num_options], temp);
                num_options++;
                system("cls");
            }
            selected_landmark = GetMenuSelection("Select Landmark Type", options, num_options);

            if(selected_landmark == 0 || selected_landmark == '\b' || selected_landmark == RTN_ESC)
                return 0;
        }
        system("cls");
        sprintf(file, ".\\Data\\Area%d\\%s.bin", selected_area, GetLandmarkType(selected_landmark));
        AddLandmark(file, selected_area, selected_landmark);
        free(temp);

        printf("Press q to exit.\nPress any key to return to main menu.");
        input = getch();
        fflush(stdin);

        if(input == KEY_Q_CAPITAL || input == KEY_Q_SMALL)
        {
            free(inputString);
            exit(0);
        }
        free(inputString);
        return 0;
    }

    case 2:
        system("cls");
        char   options[][50] = {"Edit existing landmark\n", "Delete landmark"};
        int selected_edit_option = GetMenuSelection("Edit Landmark", options, 2);
        if(selected_edit_option == 0 || selected_edit_option == '\b' || selected_edit_option == RTN_ESC)
            break;
        //system("cls");
        if(StrInput(inputString, "Enter landmark name: ", 50) == EOF)
            return 1;
        fflush(stdin);
        LANDMARK lmark;
        int num_options = 0;
        FILE *fp_search;
        for(int i = 1; strcmp(GetLandmarkType(i), "") != STR_MATCH; i++)
        {
            if(search_by_name(inputString, i) == FOUND)
            {
                fp_search = fopen(".\\Temp\\search_result.bin", "rb");
                if(fp_search == NULL)
                {
                    char errmsg[50];
                    sprintf(errmsg, "Error %d: Error in Search_result.bin\n%s", errno, strerror(errno));
                    ErrorDialogue("File error", errmsg, 0);
                    return -1;
                }
                while(fread(&lmark, sizeof(LANDMARK), 1, fp_search) == 1)
                {
                    strcpy(options[num_options++], lmark.name);
                }
				fclose(fp_search);
            }
        }
        system("cls");
        int selected_landmark = GetMenuSelection("Select landmark to be edited", options, num_options);
        if(selected_landmark == 0 || selected_landmark == '\b' || selected_landmark == RTN_ESC)
            break;
		fp_search = fopen(".\\Temp\\search_result.bin", "rb");
        for(int k = 0; k < selected_landmark; k++)
        {
            fread(&lmark, sizeof(LANDMARK), 1, fp_search);
        }
        char file[50];
        sprintf(file, ".\\Data\\Area%d\\%s.bin", lmark.area, GetLandmarkType(lmark.type));

        if(selected_edit_option == 1)
        {
            DeleteLandmark(file, lmark.name);
            AddLandmark(file, lmark.area, lmark.type);
            break;
        }

        else if(selected_edit_option == 2)
        {
            DeleteLandmark(file, lmark.name);
            break;
        }

        printf("Press q to exit.\nPress any key to return to main menu.");
        input = getch();
        fflush(stdin);

        if(input == KEY_Q_CAPITAL || input == KEY_Q_SMALL)
        {
            free(inputString);
            exit(0);
        }
        if(input == '\b')
        {
            break;
        }
        free(inputString);
        return 0;

    case 3:
        system("cls");
        GoToXY(0, 0);
        if(StrInput(inputString, "Input name of landmark type: ", 50) == EOF)
            return 1;
        fflush(stdin);

        if(SearchLandmarkType(inputString))
        {
            printf("Landmark %s already exists.\n", inputString);
        }
        else
        {
            CreateLandmarkType(inputString);
            printf("Successfully created new Landmark type.\n");

        }
        printf("Press q to exit.\nPress any key to return to main menu.");
        input = getch();
        fflush(stdin);
        if(input == KEY_Q_CAPITAL || input == KEY_Q_SMALL)
        {
            free(inputString);
            exit(0);
        }
        if(input == '\b')
        {
            break;
        }
        free(inputString);
        return 0;
    }
    free(inputString);
    return 0;



}


