#include "Functions.h"
#include <stdio.h>
#define OPTION_NEXT 0


int search_by_area()
{
    int j = 1, num_options = 0, selected_area, num_areas = 0, num_lmark_type = 0;
    /*
    char **options[50][50];
	for (j = 0; j <= NUM_AREA_G+2; j++)
	{
		options[j] = (char*)calloc(50,sizeof(char));
	}
	*/
	char options[50][50];
    LANDMARK lmark;

    for(j = 1; j <= NUM_AREA_G; j++)      //Sets area name to array of options
    {
        char area[50];
        strcpy(area, GetAreaName(j));
        strcpy(options[num_options++], area);
    }
    selected_area = GetMenuSelection("Select area", options, num_options);
    if(selected_area == 0 || selected_area == '\b' || selected_area == RTN_ESC)
        return 1;

    for(j = 1; j < NUM_LANDMARK_TYPE_G; j++)      //List of landmark in options[]
    {
        FILE *fpfile;
        char file[50];
        int selected_lmark;
        sprintf(file, ".\\Data\\Area%d\\%s.bin", selected_area, GetLandmarkType(j));
        fpfile = fopen(file, "rb");
        if(fpfile == NULL)
        {
            printf("Error opening file %s", file);
            return -1;
        }
        num_options = 0;
        if(j!= num_lmark_type)
        {
            strcpy(options[num_options++], "Next");
        }
        while(!feof(fpfile))
        {
            int fread_rtn = fread(&lmark, sizeof(lmark), 1, fpfile);
            if(strcmp(lmark.name, "") != 0 && fread_rtn == 1)
                strcpy(options[num_options++], lmark.name);
        }
        if(j > 1)
        {
            strcpy(options[num_options++], "Previous");
        }
        selected_lmark = GetMenuSelection(GetLandmarkType(j), options, num_options);

        if(selected_lmark == 1 && j != num_lmark_type)   //if next is selected
            continue;
		else if ( (selected_lmark == num_options && j > 1) )
		{
				j -= 2;
			continue;
		}
        else if(selected_lmark == '\b')
        {
			if (j == 1)
				return 1;
            j -= 2;
            continue;
        }
        else if(selected_lmark == RTN_ESC || selected_lmark == 0)
        {
            return 1;
        }
        if(j != num_lmark_type)
        {
            selected_lmark--; //Since all except last have "Next" as first option
        }
        fseek(fpfile, sizeof(LANDMARK) * (selected_lmark-1), SEEK_SET);
        if(fread(&lmark, sizeof(lmark), 1, fpfile) == 1)
        {
            DisplayLandmark(lmark);
        }
        break;


    }
    return 0;



}
