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
#define SLP 0
#define INF -1

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

	/*	for (i=0; i<4; i++){
		printf("%f ", original[i]);
	}*/

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

void queueTile(int array[][COL], int camefrom[][COL], int visitArray[][COL], Queue * Q, int x, int y, int direction){
	if (direction == NTH){
		if (array[y][x] == SPC && (y > -1)){
			if (visitArray[y][x] == 0){
				enqueue(Q, x, y);
				visitArray[y][x] = 1;
			}
			array[y][x] = DNE;
			camefrom[y][x] = NTH;
			Sleep(SLP);
			system("cls");
			printGrid(array);
			printf("\n");
			printGrid(camefrom);
		}
	}

	else if (direction == WST){
		if (array[y][x] == SPC && (x > -1)){
			if (visitArray[y][x] == 0){
				enqueue(Q, x, y);
				visitArray[y][x] = 1;
			}				
			array[y][x] = DNE;
			camefrom[y][x] = WST;
			Sleep(SLP);
			system("cls");
			printGrid(array);
			printf("\n");
			printGrid(camefrom);
		}		
	}

	else if (direction == STH){
		if (array[y][x] == SPC && (y < ROW)){
			if (visitArray[y][x] == 0){
				enqueue(Q, x, y);
				visitArray[y][x] = 1;
			}			
			array[y][x] = DNE;
			camefrom[y][x] = STH;
			Sleep(SLP);
			system("cls");
			printGrid(array);
			printf("\n");
			printGrid(camefrom);
		}
	}

	else {
		if (array[y][x] == SPC && (x < COL)){
			if (visitArray[y][x] == 0){
				enqueue(Q, x, y);
				visitArray[y][x] = 1;
			}			
			array[y][x] = DNE;
			camefrom[y][x] = EST;
			Sleep(SLP);
			system("cls");
			printGrid(array);
			printf("\n");
			printGrid(camefrom);
		}
	}
}

void BFS(int array[][COL], int camefrom[][COL], int start[], int end[]){
	
	int * dequeued;																		//Initialize all the variables
	int * orderQueue;
	int i, j, counter, next;
	float distNorth, distWest, distSouth, distEast;
	int visitedArray[ROW][COL];
	float distanceArray[ROW][COL];	
	Queue Q;
	initQueue(&Q);


	array[start[1]][start[0]] = STR;													//Mark the start coordinate
	array[end[1]][end[0]] = GOL;														//Mark the end coordinate
	camefrom[end[1]][end[0]] = GOL;														//For viewing purposes, mark the end point in the vector field

	for (i = 0; i < ROW; i++){														//Initialize the array of visitors to 0
		for (j = 0; j < COL; j++){
			visitedArray[i][j] = 0;
		}
	}

	gridDists(distanceArray, start[0], start[1]);										//Initialize the distances of each tile to the starting point

	enqueue(&Q, start[0], start[1]);													//Queue in the starting point
	visitedArray[start[1]][start[0]] = 1;										//Mark it as visited

	system("cls");																		//Print both the map and the vector field
	printGrid(array);
	printf("\n");
	printGrid(camefrom);

	
	while (!isEmptyQueue(&Q)){															//While the queue is not empty

		traverse(&Q);
		
		dequeued = dequeue(&Q);															//Dequeue what's on the queue.	

		if (dequeued[0] == end[0] && dequeued[1] == end[1]){
			break;
		}


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
			queueTile(array, camefrom, visitedArray, &Q, dequeued[0], dequeued[1]-1, NTH);
		}
		else if (next == 1){
			queueTile(array, camefrom, visitedArray, &Q, dequeued[0]-1, dequeued[1], WST);	
		}
		else if (next == 2){
			queueTile(array, camefrom, visitedArray, &Q, dequeued[0], dequeued[1]+1, STH);
		}		
		else {
			queueTile(array, camefrom, visitedArray, &Q, dequeued[0]+1, dequeued[1], EST);

		}


		for (counter=0; counter<4; counter++){												//Determine second to explore
			if (orderQueue[counter] == 1){
				next = counter;
				break;
			}
		}

		if (next == 0) {
			queueTile(array, camefrom, visitedArray, &Q, dequeued[0], dequeued[1]-1, NTH);
		}
		else if (next == 1){
			queueTile(array, camefrom, visitedArray, &Q, dequeued[0]-1, dequeued[1], WST);	
		}
		else if (next == 2){
			queueTile(array, camefrom, visitedArray, &Q, dequeued[0], dequeued[1]+1, STH);
		}		
		else {
			queueTile(array, camefrom, visitedArray, &Q, dequeued[0]+1, dequeued[1], EST);

		}



		for (counter=0; counter<4; counter++){												//Determine third to explore
			if (orderQueue[counter] == 2){
				next = counter;
				break;
			}
		}

		if (next == 0) {
			queueTile(array, camefrom, visitedArray, &Q, dequeued[0], dequeued[1]-1, NTH);
		}
		else if (next == 1){
			queueTile(array, camefrom, visitedArray, &Q, dequeued[0]-1, dequeued[1], WST);	
		}
		else if (next == 2){
			queueTile(array, camefrom, visitedArray, &Q, dequeued[0], dequeued[1]+1, STH);
		}		
		else {
			queueTile(array, camefrom, visitedArray, &Q, dequeued[0]+1, dequeued[1], EST);
		}



		for (counter=0; counter<4; counter++){												//Determine fourth to explore
			if (orderQueue[counter] == 3){
				next = counter;
				break;
			}
		}

		if (next == 0) {
			queueTile(array, camefrom, visitedArray, &Q, dequeued[0], dequeued[1]-1, NTH);
		}
		else if (next == 1){
			queueTile(array, camefrom, visitedArray, &Q, dequeued[0]-1, dequeued[1], WST);	
		}
		else if (next == 2){
			queueTile(array, camefrom, visitedArray, &Q, dequeued[0], dequeued[1]+1, STH);
		}		
		else {
			queueTile(array, camefrom, visitedArray, &Q, dequeued[0]+1, dequeued[1], EST);
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
	int startpt[2] = {1, 1};
	int endpt[2] = {29, 14};

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