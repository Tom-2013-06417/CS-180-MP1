#include <stdio.h>
#include <math.h>
#include <windows.h>

#define ROW 15
#define COL 30
#define OBS 254
#define SPC 32
#define DNE 48
#define STR 71
#define GOL 65
#define SLP 500
#define INF 500

#define NTH 25
#define WST 26
#define STH 24
#define EST 27


//DATA STRUCTURES

	//QUEUE

typedef struct queuenode QueueNode;
struct queuenode{
	int * coord;
	QueueNode * next;
};
typedef struct queue{
	QueueNode * front;
	QueueNode * rear;
} Queue;
void initQueue(Queue * Q){
	Q->front = NULL;	
}
int isEmptyQueue(Queue * Q){
	return (Q->front == NULL);
}
void queueOverflow(){
	printf("Queue overflow!\n\n");
	exit(1);
}
void queueUnderflow(){
	printf("Queue underflow!\n\n");
	exit(1);
}
void enqueue(Queue * Q, int x, int y){
	QueueNode * alpha;
	alpha = (QueueNode *)malloc(sizeof(QueueNode));
	if (alpha == NULL) queueOverflow();
	else {
		alpha->coord = (int *)malloc(2*sizeof(int));
		alpha->coord[0] = x;
		alpha->coord[1] = y;
		alpha->next = NULL;

		if (Q->front == NULL){
			Q->front = alpha;
			Q->rear = alpha;
		}

		else {
			Q->rear->next = alpha;
			Q->rear = alpha;
		}
	}
}
int * dequeue(Queue * Q){
	QueueNode * alpha;
	static int * returnthis;
	if (Q->front == NULL) queueUnderflow();
	else {
		returnthis = Q->front->coord;
		alpha = Q->front;
		Q->front = Q->front->next;
		free(alpha);
	}
	return returnthis;
}
int * peek(Queue * Q){
	static int * returnthis;
	returnthis = Q->rear->coord;
	return returnthis;
}
void traverse(Queue * Q){
	QueueNode * curr;

	curr = Q->front;

	printf("\n\nSTART -> ");

	while (curr != NULL){
		printf("(%d,", curr->coord[0]);
		printf("%d)  ", curr->coord[1]);
		curr = curr->next;
	}
	free(curr);
	printf("NULL\n");
}



//FUNCTIONS


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

void quadrilateralGenerator(int array[][COL], int anchorx, int anchory, int endptx, int endpty){
	int i, j;

	for (i=anchory; i<=endpty; i++){
		for (j=anchorx; j<=endptx; j++){
			array[i][j] = OBS;
		}
	}
}

void triangleGenerator(int array[][COL], int apexx, int apexy, int level){
	int i, j;

	for (i=0; i<level; i++){
		for (j=-i; j<=i; j++){
			array[apexy+i][apexx+j] = OBS;
		}
	}
}

float distanceFormula(int x1, int y1, int x2, int y2){
	return sqrt(pow((float)x2-(float)x1, 2)+pow((float)y2-(float)y1, 2));
}

int * rank(float n, float w, float s, float e){
	float original[4];
	static int rank[4];
	int i, j, foo, temp;

	original[0] = n;
	original[1] = w;
	original[2] = s;
	original[3] = e;

	for (i=0; i<4; i++){
		rank[i] = 0;
	}

	for (i=0; i<4; i++){
		printf("%f ", original[i]);
	}

	for (i=1; i<4; i++){
		for (j=0; j<i; j++){
			if (original[j] <= original[i]) rank[i]++;
			else rank[j]++;
		}
	}

	return rank;
}

void gridDists(float array[][COL], int x, int y){
	int i, j;

	for (i=0; i<ROW; i++){
		for (j=0; j<COL; j++){
			array[i][j] = distanceFormula(x, y, j, i);
		}
	}
}

void BFS(int array[][COL], int camefrom[][COL], int start[], int end[]){
	
	int * dequeued;
	int * orderQueue;
	int counter, next;

	float distNorth, distWest, distSouth, distEast;

	int visitedArray[ROW][COL];
	float distanceArray[ROW][COL];
	
	Queue Q;
	initQueue(&Q);

	array[start[1]][start[0]] = STR;	
	array[end[1]][end[0]] = GOL;
	camefrom[end[1]][end[0]] = GOL;

	for (int i = 0; i < ROW; i++){
		for (int j = 0; j < COL; j++){
			visitedArray[i][j] = 0;
		}
	}

	gridDists(distanceArray, start[0], start[1]);

	enqueue(&Q, start[0], start[1]);

	system("cls");
	printGrid(array);
	printf("\n");
	printGrid(camefrom);

	
	while (!isEmptyQueue(&Q)){
		
		dequeued = dequeue(&Q);
		visitedArray[dequeued[0]][dequeued[1]] = 1;										//Mark it as visited


		//Expand the neighbors by selecting the closest one. Queue Neighbors first. Dequeue them one by one while enqueueuing their respective neighbors too:

		/*distNorth = distanceArray[dequeued[1]-1][dequeued[0]];
		distWest = distanceArray[dequeued[1]][dequeued[0]-1];
		distSouth = distanceArray[dequeued[1]+1][dequeued[0]];
		distEast = distanceArray[dequeued[1]][dequeued[0]+1];*/


/*		if (dequeued[1] == 0){
			distNorth = INF;
			if (dequeued[0] == 0){
				distWest = INF;
				distEast = distanceArray[dequeued[1]][dequeued[0]+1];
			}
			else if (dequeued[0] == COL-1){
				distEast = INF;
				distWest = distanceArray[dequeued[1]][dequeued[0]-1];
			}
			distSouth = distanceArray[dequeued[1]+1][dequeued[0]];
		}

		else if (dequeued[1] == ROW-1){
			distSouth = INF;
			if (dequeued[0] == 0){
				distWest = INF;
				distEast = distanceArray[dequeued[1]][dequeued[0]+1];
			}
			else if (dequeued[0] == COL-1){
				distEast = INF;
				distWest = distanceArray[dequeued[1]][dequeued[0]-1];
			}
			distNorth = distanceArray[dequeued[1]-1][dequeued[0]];
		}

		else if (dequeued[0] == 0){
			distWest = INF;
			if (dequeued[1] == 0){
				distNorth = INF;
				distSouth = distanceArray[dequeued[1]+1][dequeued[0]];
			}
			else if (dequeued[1] == ROW-1){
				distSouth = INF;
				distNorth = distanceArray[dequeued[1]-1][dequeued[0]];
			}
			distEast = distanceArray[dequeued[1]][dequeued[0]+1];
		}

		else if (dequeued[0] == COL-1){
			distEast = INF;
			if (dequeued[1] == 0){
				distNorth = INF;
				distSouth = distanceArray[dequeued[1]+1][dequeued[0]];
			}
			else if (dequeued[1] == ROW-1){
				distSouth = INF;
				distNorth = distanceArray[dequeued[1]-1][dequeued[0]];
			}
			distWest = distanceArray[dequeued[1]][dequeued[0]-1];
		}

		else {*/
			distNorth = distanceArray[dequeued[1]-1][dequeued[0]];
			distWest = distanceArray[dequeued[1]][dequeued[0]-1];
			distSouth = distanceArray[dequeued[1]+1][dequeued[0]];
			distEast = distanceArray[dequeued[1]][dequeued[0]+1];
		//}

		orderQueue = rank(distNorth, distWest, distSouth, distEast);

		

		for (counter=0; counter<4; counter++){												//Determine first to explore
			if (orderQueue[counter] == 0){
				next = counter;
				break;
			}
		}

		if (next == 0) {
			if (array[dequeued[1]-1][dequeued[0]] == SPC && (dequeued[1]-1 > -1)){			//North
				enqueue(&Q, dequeued[0], dequeued[1]-1);				
				array[dequeued[1]-1][dequeued[0]] = DNE;
				camefrom[dequeued[1]-1][dequeued[0]] = NTH;
				Sleep(SLP);
				system("cls");
				printGrid(array);
				printf("\n");
				printGrid(camefrom);
			}
			if (dequeued[0] == end[0] && dequeued[1]-1 == end[1]){
				camefrom[dequeued[1]-1][dequeued[0]] = NTH;
				break;
			}
		}
		else if (next == 1){
			if (array[dequeued[1]][dequeued[0]-1] == SPC && (dequeued[0]-1 > -1)){			//West
				enqueue(&Q, dequeued[0]-1, dequeued[1]);				
				array[dequeued[1]][dequeued[0]-1] = DNE;
				camefrom[dequeued[1]][dequeued[0]-1] = WST;
				Sleep(SLP);
				system("cls");
				printGrid(array);
				printf("\n");
				printGrid(camefrom);
			}
			if (dequeued[0]-1 == end[0] && dequeued[1] == end[1]){
				camefrom[dequeued[1]-1][dequeued[0]] = WST;
				break;
			}	
		}
		else if (next == 2){
			if (array[dequeued[1]+1][dequeued[0]] == SPC && (dequeued[1]+1 < ROW)){			//South	
				enqueue(&Q, dequeued[0], dequeued[1]+1);				
				array[dequeued[1]+1][dequeued[0]] = DNE;
				camefrom[dequeued[1]+1][dequeued[0]] = STH;
				Sleep(SLP);
				system("cls");
				printGrid(array);
				printf("\n");
				printGrid(camefrom);
			}
			if (dequeued[0] == end[0] && dequeued[1]+1 == end[1]){
				camefrom[dequeued[1]-1][dequeued[0]] = STH;
				break;
			}
		}		
		else {
			
			if (array[dequeued[1]][dequeued[0]+1] == SPC && (dequeued[0]+1 < COL)){			//East
				enqueue(&Q, dequeued[0]+1, dequeued[1]);				
				array[dequeued[1]][dequeued[0]+1] = DNE;
				camefrom[dequeued[1]][dequeued[0]+1] = EST;
				Sleep(SLP);
				system("cls");
				printGrid(array);
				printf("\n");
				printGrid(camefrom);
			}
			if (dequeued[0]+1 == end[0] && dequeued[1] == end[1]){
				camefrom[dequeued[1]-1][dequeued[0]] = EST;
				break;
			}
		}


		for (counter=0; counter<4; counter++){												//Determine second to explore
			if (orderQueue[counter] == 1){
				next = counter;
				break;
			}
		}

		if (next == 0) {
			if (array[dequeued[1]-1][dequeued[0]] == SPC && (dequeued[1]-1 > -1)){			//North
				enqueue(&Q, dequeued[0], dequeued[1]-1);				
				array[dequeued[1]-1][dequeued[0]] = DNE;
				camefrom[dequeued[1]-1][dequeued[0]] = NTH;
				Sleep(SLP);
				system("cls");
				printGrid(array);
				printf("\n");
				printGrid(camefrom);
			}
			if (dequeued[0] == end[0] && dequeued[1]-1 == end[1]){
				camefrom[dequeued[1]-1][dequeued[0]] = NTH;
				break;
			}
		}
		else if (next == 1){
			if (array[dequeued[1]][dequeued[0]-1] == SPC && (dequeued[0]-1 > -1)){			//West
				enqueue(&Q, dequeued[0]-1, dequeued[1]);				
				array[dequeued[1]][dequeued[0]-1] = DNE;
				camefrom[dequeued[1]][dequeued[0]-1] = WST;
				Sleep(SLP);
				system("cls");
				printGrid(array);
				printf("\n");
				printGrid(camefrom);
			}
			if (dequeued[0]-1 == end[0] && dequeued[1] == end[1]){
				camefrom[dequeued[1]-1][dequeued[0]] = WST;
				break;
			}	
		}
		else if (next == 2){
			if (array[dequeued[1]+1][dequeued[0]] == SPC && (dequeued[1]+1 < ROW)){			//South	
				enqueue(&Q, dequeued[0], dequeued[1]+1);				
				array[dequeued[1]+1][dequeued[0]] = DNE;
				camefrom[dequeued[1]+1][dequeued[0]] = STH;
				Sleep(SLP);
				system("cls");
				printGrid(array);
				printf("\n");
				printGrid(camefrom);
			}
			if (dequeued[0] == end[0] && dequeued[1]+1 == end[1]){
				camefrom[dequeued[1]-1][dequeued[0]] = STH;
				break;
			}
		}		
		else {
			
			if (array[dequeued[1]][dequeued[0]+1] == SPC && (dequeued[0]+1 < COL)){			//East
				enqueue(&Q, dequeued[0]+1, dequeued[1]);				
				array[dequeued[1]][dequeued[0]+1] = DNE;
				camefrom[dequeued[1]][dequeued[0]+1] = EST;
				Sleep(SLP);
				system("cls");
				printGrid(array);
				printf("\n");
				printGrid(camefrom);
			}
			if (dequeued[0]+1 == end[0] && dequeued[1] == end[1]){
				camefrom[dequeued[1]-1][dequeued[0]] = EST;
				break;
			}
		}



		for (counter=0; counter<4; counter++){												//Determine third to explore
			if (orderQueue[counter] == 2){
				next = counter;
				break;
			}
		}

		if (next == 0) {
			if (array[dequeued[1]-1][dequeued[0]] == SPC && (dequeued[1]-1 > -1)){			//North
				enqueue(&Q, dequeued[0], dequeued[1]-1);				
				array[dequeued[1]-1][dequeued[0]] = DNE;
				camefrom[dequeued[1]-1][dequeued[0]] = NTH;
				Sleep(SLP);
				system("cls");
				printGrid(array);
				printf("\n");
				printGrid(camefrom);
			}
			if (dequeued[0] == end[0] && dequeued[1]-1 == end[1]){
				camefrom[dequeued[1]-1][dequeued[0]] = NTH;
				break;
			}
		}
		else if (next == 1){
			if (array[dequeued[1]][dequeued[0]-1] == SPC && (dequeued[0]-1 > -1)){			//West
				enqueue(&Q, dequeued[0]-1, dequeued[1]);				
				array[dequeued[1]][dequeued[0]-1] = DNE;
				camefrom[dequeued[1]][dequeued[0]-1] = WST;
				Sleep(SLP);
				system("cls");
				printGrid(array);
				printf("\n");
				printGrid(camefrom);
			}
			if (dequeued[0]-1 == end[0] && dequeued[1] == end[1]){
				camefrom[dequeued[1]-1][dequeued[0]] = WST;
				break;
			}	
		}
		else if (next == 2){
			if (array[dequeued[1]+1][dequeued[0]] == SPC && (dequeued[1]+1 < ROW)){			//South	
				enqueue(&Q, dequeued[0], dequeued[1]+1);				
				array[dequeued[1]+1][dequeued[0]] = DNE;
				camefrom[dequeued[1]+1][dequeued[0]] = STH;
				Sleep(SLP);
				system("cls");
				printGrid(array);
				printf("\n");
				printGrid(camefrom);
			}
			if (dequeued[0] == end[0] && dequeued[1]+1 == end[1]){
				camefrom[dequeued[1]-1][dequeued[0]] = STH;
				break;
			}
		}		
		else {
			
			if (array[dequeued[1]][dequeued[0]+1] == SPC && (dequeued[0]+1 < COL)){			//East
				enqueue(&Q, dequeued[0]+1, dequeued[1]);				
				array[dequeued[1]][dequeued[0]+1] = DNE;
				camefrom[dequeued[1]][dequeued[0]+1] = EST;
				Sleep(SLP);
				system("cls");
				printGrid(array);
				printf("\n");
				printGrid(camefrom);
			}
			if (dequeued[0]+1 == end[0] && dequeued[1] == end[1]){
				camefrom[dequeued[1]-1][dequeued[0]] = EST;
				break;
			}
		}



		for (counter=0; counter<4; counter++){												//Determine fourth to explore
			if (orderQueue[counter] == 3){
				next = counter;
				break;
			}
		}

		if (next == 0) {
			if (array[dequeued[1]-1][dequeued[0]] == SPC && (dequeued[1]-1 > -1)){			//North
				enqueue(&Q, dequeued[0], dequeued[1]-1);				
				array[dequeued[1]-1][dequeued[0]] = DNE;
				camefrom[dequeued[1]-1][dequeued[0]] = NTH;
				Sleep(SLP);
				system("cls");
				printGrid(array);
				printf("\n");
				printGrid(camefrom);
			}
			if (dequeued[0] == end[0] && dequeued[1]-1 == end[1]){
				camefrom[dequeued[1]-1][dequeued[0]] = NTH;
				break;
			}
		}
		else if (next == 1){
			if (array[dequeued[1]][dequeued[0]-1] == SPC && (dequeued[0]-1 > -1)){			//West
				enqueue(&Q, dequeued[0]-1, dequeued[1]);				
				array[dequeued[1]][dequeued[0]-1] = DNE;
				camefrom[dequeued[1]][dequeued[0]-1] = WST;
				Sleep(SLP);
				system("cls");
				printGrid(array);
				printf("\n");
				printGrid(camefrom);
			}
			if (dequeued[0]-1 == end[0] && dequeued[1] == end[1]){
				camefrom[dequeued[1]-1][dequeued[0]] = WST;
				break;
			}	
		}
		else if (next == 2){
			if (array[dequeued[1]+1][dequeued[0]] == SPC && (dequeued[1]+1 < ROW)){			//South	
				enqueue(&Q, dequeued[0], dequeued[1]+1);				
				array[dequeued[1]+1][dequeued[0]] = DNE;
				camefrom[dequeued[1]+1][dequeued[0]] = STH;
				Sleep(SLP);
				system("cls");
				printGrid(array);
				printf("\n");
				printGrid(camefrom);
			}
			if (dequeued[0] == end[0] && dequeued[1]+1 == end[1]){
				camefrom[dequeued[1]-1][dequeued[0]] = STH;
				break;
			}
		}		
		else {
			
			if (array[dequeued[1]][dequeued[0]+1] == SPC && (dequeued[0]+1 < COL)){			//East
				enqueue(&Q, dequeued[0]+1, dequeued[1]);				
				array[dequeued[1]][dequeued[0]+1] = DNE;
				camefrom[dequeued[1]][dequeued[0]+1] = EST;
				Sleep(SLP);
				system("cls");
				printGrid(array);
				printf("\n");
				printGrid(camefrom);
			}
			if (dequeued[0]+1 == end[0] && dequeued[1] == end[1]){
				camefrom[dequeued[1]-1][dequeued[0]] = EST;
				break;
			}
		}
	}
}

void DFS(int array[][COL], int camefrom[][COL], int start[], int end[]){
	int temp[2];


	array[start[1]][start[0]] = DNE;
	Sleep(SLP);
	system("cls");
	printGrid(array);

	if (array[start[1]-1][start[0]] == SPC && (start[1]-1 > -1)){
		temp[0] = start[0];
		temp[1] = start[1]-1;

		camefrom[temp[1]][temp[0]] = NTH;
		DFS(array, camefrom, temp, end);
	}

	if (array[start[1]][start[0]-1] == SPC && (start[0]-1 > -1)){
		temp[0] = start[0]-1;
		temp[1] = start[1];
		
		camefrom[temp[1]][temp[0]] = WST;
		DFS(array, camefrom, temp, end);
	}

	if (array[start[1]+1][start[0]] == SPC && (start[1]+1 < ROW)){
		temp[0] = start[0];
		temp[1] = start[1]+1;

		camefrom[temp[1]][temp[0]] = STH;
		DFS(array, camefrom, temp, end);
	}

	if (array[start[1]][start[0]+1] == SPC && (start[0]+1 < COL)){
		temp[0] = start[0]+1;
		temp[1] = start[1];

		camefrom[temp[1]][temp[0]] = EST;
		DFS(array, camefrom, temp, end);
	}
}


int main(){

	system("cls");

	int grid[ROW][COL];
	int startpt[2] = {8, 7};
	int endpt[2] = {19, 10};

	int * temp;

	int gridBFS[ROW][COL];
	int gridDFS[ROW][COL];

	cleanGrid(grid);
	cleanGrid(gridBFS);
	cleanGrid(gridDFS);

/*	quadrilateralGenerator(grid, 10, 4, 28, 7);
	quadrilateralGenerator(grid, 27, 4, 30, 14);
	triangleGenerator(grid, 16, 10, 7);
	printGrid(grid);

	Sleep(1000);

	DFS(grid, gridDFS, startpt, endpt);
	printGrid(gridDFS);
	
	Sleep(1000);*/
	
	cleanGrid(grid);
	quadrilateralGenerator(grid, 3, 3, 4, 11);
	quadrilateralGenerator(grid, 13, 4, 14, 14);
	BFS(grid, gridBFS, startpt, endpt);
	//printf("\n");
	//printGrid(gridBFS);


	
	return 0;
}