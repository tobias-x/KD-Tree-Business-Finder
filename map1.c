/*map1.c
* 
* Created by Edward Tobias Franks (efranks@student.unimelb.edu.au) 
* 17/09/2020
*
* Builds a 2-dimensional 'KD Tree' of locations specified in csv file
* using kdbuilder.c. Prints the details of the location(s) closest to
* the specified coordinates.
* 
* To run the program, compile and then type:
* ./map1 {input}.csv {ouptput}.txt
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
    busList* root = buildKD(argv[1]);    
	
	double total;
	double counter;
	
	//Open output file in order to write to it
	FILE *outputFile = fopen(argv[2], "w");
	safemalloc(outputFile);
    printf("Please search query:\n");

	//Search tree for each set of coordinates and print result(s) to output
	double x, y;
    while(scanf("%lf %lf",&x,&y)==2) {
		resultsCount=0;
		scanAlternator=1;
		keyComparison=0;
		busList* closestNode = (busList*) malloc(sizeof(busList));
		safemalloc(closestNode);
		searchKD(closestNode,x,y,root);
		total=total+keyComparison;
		counter=counter+1;
		keyComparison=0;
		printResults(closestNode,outputFile);
		free(closestNode);
	}  
	
	double average = total/counter;
	printf("%lf",average);
	//Closes the output file now that search is complete and frees memory
	fclose(outputFile);
	freeKD(root);
}