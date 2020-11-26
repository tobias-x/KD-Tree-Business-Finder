/*map2.c
* 
* Created by Edward Tobias Franks (efranks@student.unimelb.edu.au) 
* 17/09/2020
*
* Builds a 2-dimensional 'KD Tree' of locations specified in csv file
* using kdbuilder.c. Then prints the details of all locations within 
* a specified radius of a location inputted by the user. 
* 
* To run the program, compile and then type:
* ./map2 {input}.csv {ouptput}.txt
* 
* Parameters such as MAX_ENTRY_LENGTH and MAX_LINE_LENGTH can be adjusted 
* in "kdbuilder.h". This program must be run in the same directory as 
* "kdbuilder.h" and "kdbuilder.c".
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "kdbuilder.h"
#include "kdbuilder.c"

void main(int argc, char **argv) {
    //Builds a KD Tree using the values stored in the CSV file
    busList* kdRoot = buildKD(argv[1]);    
	
	double total;
	double counter;
	
	//Open output file in order to write to it
	FILE *outputFile = fopen(argv[2], "w");
	safemalloc(outputFile);
    printf("Please search query:\n");

	/*Searches for all businesses within each of the radii specified and 
	prints to output*/
	double xCor, yCor, radius;
    while(scanf("%lf %lf %lf",&xCor,&yCor,&radius)==3) {
		resultsCount=0;
		total = total + keyComparison;
		counter=counter+1;
		keyComparison=0;
		scanAlternator=1;
		searchRadius(outputFile,radius,xCor,yCor,kdRoot);
	}  

	double average = total/counter;
	printf("%lf",average);
	//Closes the output file now that search is complete 
	fclose(outputFile);
	freeKD(kdRoot);
}