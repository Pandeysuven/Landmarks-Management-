#include "Functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char a[100] = "Apple";

	char **options2 = (char**) calloc(50, sizeof(char*));
	for (int l = 0; l < 50; l++)
	{
		options2[l] = (char*) calloc(100, sizeof(char));
	}
	strcpy(options2[0], "a1");
	strcpy(options2[1], "b2");
	strcpy(options2[2], "c3");
	int jpt = GetMenuSelection("Test", options2, 3);
	setup();     //checks for folder, sets title

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    char options_main[][50] = {"Search by name", "Search by area", "Add/Modify records" };
    char options[OPTION_LIMIT][50];
    LANDMARK lmark;
    int j = 1, num_options = 0,  selected_landmark;
    char *input_name = calloc(50, sizeof(char));
    if(input_name == NULL)
    {
		DisplayMemoryAllocationError();
        exit(-1);
    }


    while(1)
    {
        system("cls");
        int MenuSelection = GetMenuSelection("Main Menu", options_main, 3);
    	system("cls");

        if(MenuSelection == 0 || MenuSelection == '\b' || MenuSelection == RTN_ESC)
        {
            free(input_name);
            return 0;
        }

        switch(MenuSelection)
        {
        case 1:
            num_options = 0;
            for(j = 1; j < NUM_LANDMARK_TYPE_G; j++)       //List of landmark in options[]
            {
                if(num_options > 50)
                {
                    char errmsg[50];
                    sprintf(errmsg, "Error %d while retrieving types of landmark.\n%s", E2BIG, strerror(E2BIG));
                    ErrorDialogue("Error", errmsg, 0);
                    exit(-1);
                }
                strcpy(options[num_options], GetLandmarkType(j));
                num_options++;
            }
            selected_landmark = GetMenuSelection("Select type of landmark", options, num_options);
            if(selected_landmark == 0 || selected_landmark == '\b'|| selected_landmark == RTN_ESC)
				break;

			if (StrInput(input_name, "Input name of landmark: ", 50) == EOF)
			{
				break;
			}
			fflush(stdin);
			system("cls");
		    int search_result;
			if( (search_result = search_by_name(input_name, selected_landmark)) == NOT_FOUND)
            {
				printf("Cannot find landmark.\n");
            }
            else if(search_result == EOF)
            {
                break;
            }
            else
            {
                FILE *fpsearch = fopen(".\\Temp\\search_result.bin", "rb");
                if(fpsearch == NULL)
                {
                    char errmsg[50];
                    sprintf(errmsg, "Error %d: Error in Search_list.bin.\n%s", ENOFILE, strerror(ENOFILE));
                    ErrorDialogue("File error", errmsg, 0);
                    free(input_name);
                    return -1;
                }
                while(fread(&lmark, sizeof(LANDMARK), 1, fpsearch))
                {
                    DisplayLandmark(lmark);
                    printf("\n");
                }
            }

            printf("Press q to exit.\nPress any key to return to main menu.");
            int input = getch();
            if(input == KEY_Q_CAPITAL || input == KEY_Q_SMALL)
            {
                free(input_name);
                exit(0);
            }
            break;

        case 2:
			if (search_by_area())
				break;

            printf("Press q to exit.\nPress any key to return to main menu.");
            input = getch();
            if(input == KEY_Q_CAPITAL || input == KEY_Q_SMALL)
            {
                free(input_name);
                exit(0);
            }
            break;

        case 3:
            modify_records();
            break;
        }
    }
    return 0;


}
