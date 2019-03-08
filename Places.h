#ifndef PLACES_H
#define PLACES_H

typedef struct _LANDMARK
{
    char phone[11];
    int area;
    char name[50];
    int type;
    char address[50];


} LANDMARK, *P_LANDMARK;


#endif // PLACES_H


/*
	if (temp == NULL)
	{

	}

	if ( fptr == NULL )
    {
    	char errmsg[50];
sprintf(errmsg, "Error %d: Error in Landmark_list.txt.\n%s", errno, strerror(errno));
ErrorDialogue("File error", errmsg, 0);
        free( temp );
        exit(-1);
    }

    if (num_options > 50)
	{
		char errmsg[50];
		sprintf(errmsg, "Error %d while retrieving types of landmark.\n%s", E2BIG, strerror(E2BIG));
		ErrorDialogue("Error", errmsg, 0);
		exit(-1);
	}









*/
