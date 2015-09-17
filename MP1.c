#include <stdio.h>
#include <math.h>
#include <windows.h>

#define ROW 20
#define COL 40
#define OBS 32
#define SPC 46
#define DNE 35
#define STR 65
#define GOL 90
#define SLP 100
#define INF -1

#define NON 32
#define DOT 46
#define ZRO 48
#define PTH 254

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
void reverseQueue(Queue * Q){
	QueueNode * curr, * alpha;

	curr = Q->front;

	if (curr == NULL) return;				//Exit funciton if no elements
	if (curr->next == NULL) return;			//Exit function if only one element

	while (curr->next->next != NULL){		//Else if two or more elements in queue traverse until the end of the line
		curr = curr->next;
	}

	alpha = curr->next;						//Set initial alpha node

	while (alpha->next != Q->front){

		alpha->next = curr;		//Inverted the link. This node now points to the pointer to this node (the one behind it);

		curr = Q->front;

		while (curr->next->next != alpha){
			curr = curr->next;
		}

		alpha = curr->next;
	}

	Q->front->next = NULL;

	alpha = Q->front;			//Swap the front and rear pointers
	Q->front = Q->rear;
	Q->rear = alpha;
}
void traverseQueue(Queue * Q){
	QueueNode * curr;

	curr = Q->front;

	printf("\n\nSTART -> ");

	while (curr != NULL){
		printf("(%d,", curr->coord[0]);
		printf("%d)  ", curr->coord[1]);
		curr = curr->next;
	}
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

void gridDists(float array[][COL], int x, int y){
	int i, j;

	for (i=0; i<ROW; i++){
		for (j=0; j<COL; j++){
			array[i][j] = distanceFormula(x, y, j, i);
		}
	}
}

int isInsideGrid(int array[][COL], int x, int y){
	if (x >= 0 && x < COL && y >= 0 && y < ROW) return 1;
	return 0;
}

int isPassable(int array[][COL], int x, int y){
	if (array[y][x] != OBS) return 1;
	return 0;
}

void printPath(int array[][COL], Queue * Q){
	QueueNode * curr;
	curr = Q->front;

	//cleanGrid(array);

	while (curr != NULL){
		array[curr->coord[1]][curr->coord[0]] = PTH;
		Sleep(SLP);
		system("cls");
		printGrid(array);
		curr = curr->next;
	}	
}


Queue Neighbors(int array[][COL], int camefrom[][COL], int x, int y){
	Queue Neighbor;
	initQueue(&Neighbor);

	if (isInsideGrid(array, x, y-1) && isPassable(array, x, y-1) && camefrom[y-1][x] == SPC){
		enqueue(&Neighbor, x, y-1);
		camefrom[y-1][x] = NTH;
	}
	if (isInsideGrid(array, x-1, y) && isPassable(array, x-1, y) && camefrom[y][x-1] == SPC){
		enqueue(&Neighbor, x-1, y);
		camefrom[y][x-1] = WST;
	}
	if (isInsideGrid(array, x, y+1) && isPassable(array, x, y+1) && camefrom[y+1][x] == SPC){
		enqueue(&Neighbor, x, y+1);
		camefrom[y+1][x] = STH;
	}
	if (isInsideGrid(array, x+1, y) && isPassable(array, x+1, y) && camefrom[y][x+1] == SPC){
		enqueue(&Neighbor, x+1, y);
		camefrom[y][x+1] = EST;
	}

	if ((x+y) % 2 == 1) reverseQueue(&Neighbor);

	return Neighbor;
}

void BFS(int array[][COL], int camefrom[][COL], int start[], int end[]){
	
	int * dequeued;																		//Initialize all the variables
	int * dequeuedNeighbor;
	int i, j, next;
	int visitedArray[ROW][COL];
	
	Queue Q;
	initQueue(&Q);

	Queue nb;
	Queue pathtrace;
	initQueue(&pathtrace);

	if (!isPassable(array, start[0], start[1])) return;	

	for (i = 0; i < ROW; i++){															//Initialize the array of visitors to 0
		for (j = 0; j < COL; j++){
			visitedArray[i][j] = 0;
		}
	}

	array[start[1]][start[0]] = STR;													//Mark the start coordinate
	array[end[1]][end[0]] = GOL;														//Mark the end coordinate
	camefrom[start[1]][start[0]] = STR;													//Mark the start coordinate
	

	enqueue(&Q, start[0], start[1]);													//Queue in the starting point
	visitedArray[start[1]][start[0]] = 1;												//Mark it as visited
	
	while (!isEmptyQueue(&Q)){															//While the queue is not empty

		dequeued = dequeue(&Q);

		visitedArray[dequeued[1]][dequeued[0]] = 1;				
		array[dequeued[1]][dequeued[0]] = DNE;	
		//Sleep(SLP);
/*		system("cls");
		printGrid(array);
		printf("\n");
		printGrid(camefrom);*/

		nb = Neighbors(array, camefrom, dequeued[0], dequeued[1]);

		if (dequeued[0] == end[0] && dequeued[1] == end[1]){							//If a newly visited node is the goal, break from the loop twice!!!
			array[start[1]][start[0]] = STR;											//Mark the start coordinate
			array[end[1]][end[0]] = GOL;												//Mark the end coordinate	
			break;
		}		
		
		//Expand the neighbors by selecting the closest one. Queue Neighbors first into Neighbor Queue (becomes sorted). Dequeue them to the main Queue one by one so that their neigbhors will be enqueued in the future:

		while (!isEmptyQueue(&nb)){														//Now that we have the sorted nb, dequeue them then visit!
			dequeuedNeighbor = dequeue(&nb);
			if (visitedArray[dequeuedNeighbor[1]][dequeuedNeighbor[0]] == 0){			//Visit nodes if they haven't been visited though. Otherwise, skip
				enqueue(&Q, dequeuedNeighbor[0], dequeuedNeighbor[1]);				
			}
		}
	}
	
	//printGrid(array);
	//printf("\n");

	enqueue(&pathtrace, end[0], end[1]);
	
	while (1){
		if (pathtrace.rear->coord[0] == start[0] && pathtrace.rear->coord[1] == start[1]) break;
		switch(camefrom[pathtrace.rear->coord[1]][pathtrace.rear->coord[0]]){
			case NTH:
				enqueue(&pathtrace, pathtrace.rear->coord[0], pathtrace.rear->coord[1]+1);				
				break;
			case WST:
				enqueue(&pathtrace, pathtrace.rear->coord[0]+1, pathtrace.rear->coord[1]);				
				break;
			case STH:
				enqueue(&pathtrace, pathtrace.rear->coord[0], pathtrace.rear->coord[1]-1);
				break;
			case EST:
				enqueue(&pathtrace, pathtrace.rear->coord[0]-1, pathtrace.rear->coord[1]);
				break;
		}
	}

	reverseQueue(&pathtrace);
	printPath(camefrom, &pathtrace);
}

void DFS(int array[][COL], int camefrom[][COL], int start[], int end[]){
	int temp[2];


	array[start[1]][start[0]] = DNE;
	Sleep(SLP);
	system("cls");
	printGrid(array);

	if (start[0] == end[0] && start[1] == end[1]){							//If a newly visited node is the goal, break from the loop twice!!!
		array[start[1]][start[0]] = STR;											//Mark the start coordinate
		array[end[1]][end[0]] = GOL;												//Mark the end coordinate	
		return;
	}	


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

	if (array[start[1]][start[0]+1] == SPC && (start[0]+1 < COL)){
		temp[0] = start[0]+1;
		temp[1] = start[1];

		camefrom[temp[1]][temp[0]] = EST;
		DFS(array, camefrom, temp, end);
	}

	if (array[start[1]+1][start[0]] == SPC && (start[1]+1 < ROW)){
		temp[0] = start[0];
		temp[1] = start[1]+1;

		camefrom[temp[1]][temp[0]] = STH;
		DFS(array, camefrom, temp, end);
	}


}


int main(){

	system("cls");

	int grid[ROW][COL];
	int startpt[2] = {6, 0};
	int endpt[2] = {20, 19};

	int * temp;

	int gridBFS[ROW][COL];
	int gridDFS[ROW][COL];

	cleanGrid(grid);
	cleanGrid(gridBFS);
	cleanGrid(gridDFS);

	Queue Q;
	initQueue(&Q);

	/*	quadrilateralGenerator(grid, 10, 4, 28, 7);
	quadrilateralGenerator(grid, 27, 4, 30, 14);
	triangleGenerator(grid, 16, 10, 7);
	printGrid(grid);

	Sleep(1000);

	DFS(grid, gridDFS, startpt, endpt);
	printGrid(gridDFS);
	
	Sleep(1000);*/
	
	cleanGrid(grid);
	quadrilateralGenerator(grid, 2, 2, 2, 5);
	quadrilateralGenerator(grid, 2, 8, 2, 12);
	quadrilateralGenerator(grid, 0, 11, 2, 11);
	quadrilateralGenerator(grid, 7, 0, 7, 2);
	quadrilateralGenerator(grid, 2, 2, 7, 2);


	//triangleGenerator(grid, 165, 20, 100);	
	//printGrid(grid);
	BFS(grid, gridBFS, startpt, endpt);
	Sleep(1000);
	system("cls");
	cleanGrid(grid);
	quadrilateralGenerator(grid, 2, 2, 2, 5);
	quadrilateralGenerator(grid, 2, 8, 2, 12);
	quadrilateralGenerator(grid, 0, 11, 2, 11);
	quadrilateralGenerator(grid, 7, 0, 7, 2);
	quadrilateralGenerator(grid, 2, 2, 7, 2);
	DFS(grid, gridDFS, startpt, endpt);

	//printf("\n");
	//printGrid(gridBFS);

	return 0;
}