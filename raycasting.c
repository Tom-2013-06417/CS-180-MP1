#include <stdio.h>
#include <math.h>
#include <windows.h>

#define ROW 15
#define COL 30
#define OBS 254
#define SPC 46
#define DNE 48
#define STR 71
#define GOL 65
#define SLP 100
#define INF 500

#define NTH 25
#define WST 26
#define STH 24
#define EST 27

typedef struct queuenode QueueNode;
struct queuenode{
	int * coord;
	QueueNode * next;
};

void flag(){

	printf("\nPUTANGINAAAAAAA\n");

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


int PIPVerifier(int array[][COL], float vertices[][3], int x, int y, int n){

	int i, j, c = 0;
	for (i = 0, j = n-1; i < n; j = i++) {
		if ( 
			((vertices[0][i] >= y) != (vertices[0][j] >= y)) &&
	 		(x <= ((vertices[1][j]-vertices[1][i]) * (y-vertices[0][i]) / (vertices[0][j]-vertices[0][i]) + vertices[1][i]))
	 	) c = !c;
	}

	return c;

}

void plot(int array[][COL], int x, int y){

	array[y][x] = OBS;
	system("cls");
	printGrid(array);
	Sleep(SLP);

}

void plotVertices(int array[][COL], float vertices[][3], int n){

	int i;

	for (i=0; i<n; i++){
		plot(array, vertices[1][i], vertices[0][i]);
		system("cls");
		printGrid(array);
		Sleep(SLP);
	}

}

void fillPolygon(int array[][COL], float vertices[][3], int n){

	int i, j;

	for (i=0; i<ROW; i++){
		for (j=0; j<COL; j++){
			if (PIPVerifier(array, vertices, j, i, n) == 1){
				plot(array, j, i);
			}
		}
	}

}

void generatePolygon(int array[][COL], float vertices[][3], int n){

	plotVertices(array, vertices, n);
	fillPolygon(array, vertices, n);
	system("cls");
	printGrid(array);

}

float ** readFile(){

	flag();

}

int main(){
	//convention: array[0] = y || array[1] = x
	
	int grid[ROW][COL];

	float shape2[2][3] = {{12, 8, 5}, {6, 8, 5}};
	float shape3[2][3] = {{7, 11, 12}, {8, 9, 2}};

	cleanGrid(grid);
	flag();
	generatePolygon(grid, shape2, 3);
	generatePolygon(grid, shape3, 3);

}