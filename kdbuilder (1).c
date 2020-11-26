//This program contains all functions relating to the construction, manipulation, and accessing of the KD Tree

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

//Inserts Node into KD Tree; used to support buildKD
void insertNode(busList* currentNode, busList* rootNode) {
	currentNode->parent=rootNode;
	int alternator=0;
	int isLeaf=0;
	while(isLeaf==0) {
		if(*(currentNode->coor[0])==*(currentNode->parent->coor[0]) && *(currentNode->coor[1])==*(currentNode->parent->coor[1])) {
			busList* tempNode = currentNode->parent;
			while(tempNode->next!=NULL) {
				tempNode=tempNode->next;
			}
			tempNode->next = (busList*)malloc(sizeof(busList));
			safemalloc(tempNode->next);
			*(tempNode->next) = *(currentNode);
			isLeaf=1;
		}
		else {
			if(*(currentNode->coor[alternator]) < *(currentNode->parent->coor[alternator])) {
				if(currentNode->parent->left != NULL) {
					currentNode->parent = currentNode->parent->left;
				}
				else{
					currentNode->parent->left = (busList*)malloc(sizeof(busList));
					safemalloc(currentNode->parent->left);
					*(currentNode->parent->left) = *(currentNode);
					isLeaf=1;
				}
			}
			else {
				if(currentNode->parent->right != NULL) {
					currentNode->parent = currentNode->parent->right;
				}
				else {
					currentNode->parent->right = (busList*)malloc(sizeof(busList));
					safemalloc(currentNode->parent->right);
					*(currentNode->parent->right) = *(currentNode);
					isLeaf=1;
				}
			}
		}
		alternator^=1;
	}
}

//Builds the KDTree using data from user-inputted CSV
busList* buildKD(char* dataFile) {
	double typeDouble;
	char refLine[MAX_LINE_LENGTH];
	FILE *openDataFile = fopen(dataFile, "r");
	busList *rootNode = (busList*)malloc(sizeof(busList));
	safemalloc(rootNode);
    busList *currentNode=(busList*)malloc(sizeof(busList));
	safemalloc(currentNode);

	while(fgets(refLine, MAX_LINE_LENGTH, openDataFile)!=NULL) {

        //Constructs linked-list using cstrtok function
        prevDelim=0;
        currentNode->censusYear = strdup(cstrtok(refLine, ','));
        currentNode->blockID=strdup(cstrtok(refLine, ','));
        currentNode->propertyID=strdup(cstrtok(refLine, ','));
        currentNode->baseProperty=strdup(cstrtok(refLine, ','));
        currentNode->CLUESmallArea=strdup(cstrtok(refLine, ','));
        currentNode->tradingName=strdup(cstrtok(refLine, ','));
        currentNode->industryCode=strdup(cstrtok(refLine, ','));
        currentNode->industryDescription=strdup(cstrtok(refLine, ','));

		currentNode->coor[0]=malloc(sizeof(typeDouble));
		safemalloc(currentNode->coor[0]);
        *(currentNode->coor[0])=atof(cstrtok(refLine, ','));
		currentNode->coor[1]=malloc(sizeof(typeDouble));
		safemalloc(currentNode->coor[1]);
        *(currentNode->coor[1])=atof(cstrtok(refLine, ','));
		
        currentNode->location=strdup(cstrtok(refLine, ','));
		free(cstrtok(refLine,','));
		
		if(*(currentNode->coor[0])!=0) {
			if(rootNode->tradingName==NULL) {
				memcpy(rootNode,currentNode,sizeof(busList));
			}

			else{
				insertNode(currentNode,rootNode);
			}
		}
	}
    fclose(openDataFile);
	free(currentNode);
	return rootNode;
}

int keyComparison=0;
int scanAlternator=1;
int resultsCount=0;
double shortestDistance;
//Searches KD for the closest node to the specified coordinates
busList* searchKD(busList* closestNode, double x, double y,busList* current) {
	scanAlternator^=1;
	double coor[2]= {x,y};
	double currentDistance = calculateDistance(x,y,current);
	double linearDistance=fabs(coor[scanAlternator]-*(current->coor[scanAlternator]));
	if(currentDistance < shortestDistance || resultsCount==0) {
		resultsCount=1;
		shortestDistance=currentDistance; 
		*(closestNode)=*(current);
	}
	if(coor[scanAlternator] < *(current->coor[scanAlternator])) {
		if(current->left!=NULL) {
			searchKD(closestNode,x,y,current->left);
		}
		if(shortestDistance>=linearDistance && current->right!=NULL) {
			searchKD(closestNode,x,y,current->right);
		}
	}
	else {
		if(current->right!=NULL) {
			searchKD(closestNode,x,y,current->right);
		}
		if(shortestDistance>=linearDistance && current->left!=NULL) {
			searchKD(closestNode,x,y,current->left);
		}
	}
	keyComparison++;
}


void printResults(busList* currentNode, FILE* openOutput) {
    //Loops through each node, printing results to the specified file
    while(currentNode->next!=NULL) {
		fprintf(openOutput,"%s --> ",currentNode->tradingName);
		fprintf(openOutput,"Census year: %s || ",currentNode->censusYear); 
		fprintf(openOutput,"Block ID: %s || ",currentNode->blockID); 
		fprintf(openOutput,"Property ID: %s || ",currentNode->propertyID); 
		fprintf(openOutput,"Base property ID: %s || ",currentNode->baseProperty); 
		fprintf(openOutput,"CLUE small area: %s || ",currentNode->CLUESmallArea); 
		fprintf(openOutput,"Industry (ANZSIC4) code: %s || "
		,currentNode->industryCode); 
		fprintf(openOutput,"Industry (ANZSIC4) description: %s || "
		,currentNode->industryDescription); 
		fprintf(openOutput,"x coordinate: %lf || ",*(currentNode->coor[0]));
		fprintf(openOutput,"y coordinate: %lf || ",*(currentNode->coor[1]));
		strtok(currentNode->location,"\r");
		fprintf(openOutput,"Location: %s || \n\n",currentNode->location);
        currentNode=currentNode->next;
    }
}


//Searches with a specified radius and prints results to openOutput
void searchRadius(FILE* openOutput ,double radius, double x, double y,busList* current) {
	scanAlternator^=1;
	double radiusSqr=radius*radius;
	double coor[2]={x,y};
	double currentDistance = calculateDistance(x,y,current);
	double linearDistance=fabs(coor[scanAlternator]-*(current->coor[scanAlternator]));
	if(currentDistance<=radiusSqr) {
		resultsCount++;
		printResults(current,openOutput);
	}
	if(coor[scanAlternator] < *(current->coor[scanAlternator])) {
		if(current->left!=NULL) {
			searchRadius(openOutput,radius,x,y,current->left);
		}
		if(radiusSqr>=linearDistance && current->right!=NULL) {
			searchRadius(openOutput,radius,x,y,current->right);
		}
	}
	else {
		if(current->right!=NULL) {
			searchRadius(openOutput,radius,x,y,current->right);
		}
		if(radiusSqr>=linearDistance && current->left!=NULL) {
			searchRadius(openOutput,radius,x,y,current->left);
		}
	}
	keyComparison++;
}

//Sequentailly traverses KD Tree, freeing memory
void freeKD(busList* current) {
	if(current==NULL) {
		return;
	}
	freeKD(current->left);
	freeKD(current->right);
	freeKD(current->next);
	free(current);
}