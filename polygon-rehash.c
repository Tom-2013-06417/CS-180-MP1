#include <stdio.h>
#include <math.h>
#include <windows.h>

#define ROW 20
#define COL 30
#define OBS 254
#define EDG 35
#define VTX 33
#define SPC 32
#define DNE 48
#define STR 71
#define GOL 65
#define SLP 0
#define INF 500

#define NTH 25
#define WST 26
#define STH 24
#define EST 27

typedef struct PolyNode
{

    struct PolyNode * next;
    int numVertex;
    float * XCoord;
    float * YCoord;

} PolyNode;

void flag(){

	printf("\nPUTANGINAAAAAAA\n");

}

void pressEnter(){

	char c;
	scanf("%c", &c);

}


void cleanGrid(int array[][COL]){

	int i, j;

	for (i=0; i<ROW; i++){
		for (j=0; j<COL; j++){
			array[i][j] = SPC;
		}
	}

}

void printGrid(int array[][COL]){

	int i, j;

	for (i=0; i<ROW; i++){
		for (j=0; j<COL; j++){
			printf("%c ", array[i][j]);
		}
		printf("\n");
	}

}

void plot(int array[][COL], int x, int y, int mode){

	if (mode == 1) array[y][x] = OBS;
	else if (array[y][x] != VTX && array[y][x] != EDG){ 
		if (mode == 2) array[y][x] = EDG;
		else array[y][x] = EDG;
	}
	system("cls");
	printGrid(array);
	Sleep(SLP);

}

void plotVertices(int array[][COL], float XCoord[], float YCoord[], int n){

	int i;

	for (i=0; i<n; i++) {
		plot(array, XCoord[i], YCoord[i], 2);
		printf("%.0f %.0f\n", XCoord[i], YCoord[i]);
		pressEnter();
	}
}

void convertVertices(int array[][COL]){

	int i, j;
	for (i=0; i<COL; i++){
		for (j=0; j<ROW; j++){
			if (array[j][i] == EDG) plot(array, i, j, 1);
		}
	}

}

void generateStraightEdge(int array[][COL], float x1, float y1, float x2, float y2){

	int i, start, end;

	if (x1 == x2){
		if (y1 < y2){
			start = y1;
			end = y2;
		}

		else {
			start = y2;
			end = y1;
		}

		for(i = start; i <= end; i++) plot(array, x1, i, 3);
		return;
	}

	if (y1 == y2){
		if (x1 < x2){
			start = x1;
			end = x2;
		}
		else {
			start = x2;
			end = x1;
		}

		for(i = start; i <= end; i++) plot(array, i, y1, 3);
		return;
	}

}

void generateDiagonalEdge(int array[][COL], float x1, float y1, float x2, float y2){

	int i, j;
	int dx = abs(x1 - x2), dy = abs(y1 - y2);
	int xPos = x1, yPos = y1;
	int subedge, horStep, verStep;
	int horSum=0, verSum=0;
	int distribute;

	float m = (y2 - y1)/(x2 - x1);

	if (dy >= dx){
		if (y1 > y2){
			for(i=y2+1; i<y1; i++){
				xPos = ((i - y1)/m) + (x1);
				yPos = i;
				plot(array, xPos, yPos, 3);
				printf("MORE Y (%d > %d): %.0f > %.0f\n", dy, dx, y1, y2);
				pressEnter();
			}
		}

		else {
			for(i=y1+1; i < y2; i++){
				xPos = ((i- y2)/m) + (x2);
				yPos = i;
				plot(array, xPos, yPos, 3);
				printf("MORE Y (%d > %d): %.0f <= %.0f\n", dy, dx, y1, y2);
				pressEnter();
			}
		}
	}

	else{
		if (x1 < x2){
			for(i=x1+1; i<x2; i++){
				xPos = abs(i);
				yPos = abs((m * (i - x2)) + (y2));
				plot(array, xPos, yPos, 3);
				printf("MORE X (%d > %d): %.0f < %.0f\n", dx, dy, x1, x2);
				pressEnter();
			}
		}

		else {
			for(i=x2+1; i < x1; i++){
				xPos = abs(i);
				yPos = abs((m * (i - x2)) + (y2));
				plot(array, xPos, yPos, 3);
				printf("MORE X (%d > %d): %.0f >= %.0f\n", dx, dy, x1, x2);
				pressEnter();
			}
		}	
	}

}

void generateEdge(int array[][COL], float x1, float y1, float x2, float y2){

	int i, j, start, end;
	float dx = abs(x1 - x2), dy = abs(y1 - y2);
	int xPos, yPos;
	int subedge, horStep, verStep;
	int horSum=0, verSum=0;
	char x;

	if(x1 == x2 || y1 == y2) generateStraightEdge(array, x1, y1, x2, y2);
	else generateDiagonalEdge(array, x1, y1, x2, y2);

	printf("GENERATED EDGE BETWEEN (%.0f, %.0f) & (%.0f, %.0f) \n", x1, y1, x2, y2);
	pressEnter();
}

void generatePolygon(int array[][COL], float XCoord[], float YCoord[], int n){

	plotVertices(array, XCoord, YCoord, n);
	pressEnter();
	int i, j; 
	for (i = 0; i < n; i++){
		j = i + 1;

		if (j < n) generateEdge(array, XCoord[i], YCoord[i], XCoord[j], YCoord[j]);
		else generateEdge(array, XCoord[i], YCoord[i], XCoord[0], YCoord[0]);
	}
	pressEnter();
	//fillPolygon(array, XCoord, YCoord, n);
	convertVertices(array);
	pressEnter();

}

void traversePolyNodes(int array[][COL], PolyNode * start){

	PolyNode * current = start;
	int i, j = 1;
	char x;

	while(current != NULL){
		generatePolygon(array, current->XCoord, current->YCoord, current->numVertex);
		current = current->next;
	}

}

void createPolyNode(PolyNode ** start, PolyNode ** current, int n){

	PolyNode * new = malloc(sizeof(PolyNode));

	new->numVertex = n;
	new->XCoord = malloc(n*sizeof(float));
	new->YCoord = malloc(n*sizeof(float));
	new->next = NULL;

	if(*start == NULL){
		*start = new;
		*current = new;
	}

	else{
		(*current)->next = new;
		*current = (*current)->next;
	}

}

void readPolygonData(PolyNode ** start, PolyNode ** current){

	FILE * fp = fopen("input.txt", "r");
	int i, n, x, y;

	if (fp == NULL) {
		printf("Input File Not Found\n");
		exit(1);
	}

	fseek(fp, 0, SEEK_END);
	if ( ftell(fp) == 0 )
	{
		printf("Empty File\n");
		fclose(fp);
		exit(1);
	}

	fclose(fp);


	fp = fopen("input.txt", "r");

	while(!feof(fp)){
		if (fscanf(fp, "%d", &n) == 1){
			printf("Number of  Vertices: %d\n", n);
			createPolyNode(start, current, n);

			i = 0;
			printf("X Coordinates ");
			while (i < n){
				fscanf(fp, "%d", &x);
				(*current)->XCoord[i] = x;
				i++;
			}
			printf("\n");

			i = 0;
			printf("Y Coordinates ");
			while (i < n){
				fscanf(fp, "%d", &y);
				(*current)->YCoord[i] = y;	
				i++;
			}
			printf("\n\n");
		}
	}

    fclose(fp);

}

int main(){
	
	int grid[ROW][COL];
	PolyNode * start;
	PolyNode * current;

	start = NULL;
	current = NULL;

	cleanGrid(grid);
	readPolygonData(&start, &current);
	traversePolyNodes(grid, start);	

}