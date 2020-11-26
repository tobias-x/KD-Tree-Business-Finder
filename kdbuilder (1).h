/*This program contains all functions and variable declarations which support 
the construction of the KD Treet*/

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <string.h>

#define MAX_ENTRY_LENGTH 128
#define MAX_LINE_LENGTH 512
#define MAX_DIM 2

//Determines the structure of each node
struct business {
	double* coor[MAX_DIM];
	char* censusYear;
	char* blockID;
	char* propertyID;
	char* baseProperty;
	char* CLUESmallArea;
	char* tradingName;
	char* industryCode;
	char* industryDescription;
    char* location;
	struct business* left; 
	struct business* right;
	struct business* parent;
	struct business* next;
};
typedef struct business busList;

//Function which ensures that memory has been succesfully allocated
void safemalloc(void* memoryAllocation) {
	if(memoryAllocation==NULL) {
		printf("malloc() error\n");
		exit(1);
	}
}

/*Function scans a string for a character and returns 
another string up to that point*/
int prevDelim;

char* cstrtok(char tempRefLine[], char delim) {
    int referenceChar=prevDelim;
    int inQuotes = 0;
    while(tempRefLine[referenceChar]) {
        //Checks to see whether the reference Char is in quotes
        if(tempRefLine[referenceChar]=='"') {
            inQuotes ^= 1;
        }
    
        //Checks to see whether the reference char is the 'delim' character
        if(tempRefLine[referenceChar]==delim && inQuotes == 0) {
            break;
        }
        referenceChar++;
    }
    char* dataEntry= (char*) malloc(sizeof(char)*MAX_ENTRY_LENGTH);
	safemalloc(dataEntry);
    
    //Adds every non-delim character to a string, and returns that string
    for(int CharPtr=0;CharPtr<(referenceChar-prevDelim);CharPtr++) {
        dataEntry[CharPtr]=tempRefLine[(CharPtr+prevDelim)];
    }
    dataEntry[(referenceChar-prevDelim)]='\000';
	
	//Remove quotes if they exist and compacts 'double' quotes to singles
	if(dataEntry[0]=='"') {
		int quoteRemove = strlen(dataEntry)-1;
		dataEntry[(quoteRemove)]='\000';
		int CharPtr;
		int qTracker=1;
		for(CharPtr=0;CharPtr<(quoteRemove);CharPtr++) {
			if(dataEntry[(CharPtr+qTracker)]=='"') {
				qTracker++;
			}
			dataEntry[CharPtr]=dataEntry[(CharPtr+qTracker)];
		}
	}
    prevDelim=referenceChar+1;
    return dataEntry;
}

/*Calculates the squared 'distance' between two points. There is no need
to square root because distances are only used for comparisons.*/
double calculateDistance(double x, double y, busList* node) {
	double x1 = x;
	double x2 = *(node->coor[0]);
	double y1 = y;
	double y2 = *(node->coor[1]);
	double distance = fabs((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
	return distance;
}
