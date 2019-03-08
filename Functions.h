#ifndef FUNCTION_H
#define FUNCTION_H

#define _CRT_SECURE_NO_WARNINGS
extern int NUM_AREA_G;
extern int NUM_LANDMARK_TYPE_G;
extern char** MENU_OPTIONS_G;

#include <windows.h>
#include "Places.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <conio.h>
#include <string.h>

#define ARROW_UP 72
#define ARROW_DOWN 80
#define ARROW_LEFT 75
#define ARROW_RIGHT 77
#define KEY_Q_SMALL 113
#define KEY_Q_CAPITAL 81
#define ENTERKEY 13
#define WHITE 7
#define RED 4
#define HIGHLIGHTED 135
#define COLOR_NORMAL 7
#define NUM_AREAS 5
#define NOT_FOUND 0
#define FOUND 1
#define STR_MATCH 0
#define MATCH_FOUND 0
#define MATCH_NOT_FOUND 1
#define OPTION_LIMIT 50
#define KEY_ESC 27
#define RTN_ESC (OPTION_LIMIT+KEY_ESC)
#define MAXIMUM_INPUT_SIZE 500

#ifndef ENOFILE
	#define ENOFILE ENOENT
#endif

//#define strcmpi _strcmpi
//#define strcmpi strcmp
#define getch _getch
#define getche _getche


//graphics.c
void GoToXY(int x, int y);
COORD GoToMiddle(int, int);
void CreateTable(int length, int breadth);

int CreateLandmarkType(char *);
char* GetAreaName(int sn);
char* GetLandmarkType(int sn);
int SearchLandmarkType(char* inputname);

int GetMenuSelection(char *heading, char (*options)[50], int num_options);
int GetMenuSelectionDouble(char *heading, char**options, int num_options);

int search_by_area();
int search_by_name(char *name, int lmark_type);
int modify_records();

int AddLandmark(char *file_location, int area, int lmarktype);
void DisplayLandmark(LANDMARK lmark);
int DeleteLandmark(char *file_path, char *lmark_name);

void setup();
int CreateFolder();
int StrInput(char *_string, char *msg, int sz);
int StripfromFile(char *file_path);
char* PhoneInput(char *);
int ErrorDialogue(char *heading, char *error, int type);

void DisplayMemoryAllocationError();
void DisplayFileError(char *file_name);
#endif // FUNCTION_H
