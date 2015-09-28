#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <time.h>

#define ROW 21
#define COL 21
#define OBS 254
#define SPC 32
#define DNE 35
#define STR 65
#define GOL 90
#define SLP 5
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

float distanceFormula(int x1, int y1, int x2, int y2){
	return abs(x1 - x2) + abs(y1 - y2);
}

float heuristicDist(int x, int y, int startptx, int startpty, int endptx, int endpty){
	return distanceFormula(x, y, endptx, endpty);
	//return distanceFormula(x, y, endptx, endpty) + distanceFormula(x, y, startptx, startpty);
}

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
void traverseQueue(Queue * Q, float endptx, float endpty){
	char c;
	QueueNode * curr;

	curr = Q->front;

	printf("\n\nSTART\n");

	while (curr != NULL){
		printf("(%d,", curr->coord[0]);
		printf("%d)  ", curr->coord[1]);
		printf("[%.0f]\n", distanceFormula(endptx, endpty, curr->coord[0], curr->coord[1]));
		curr = curr->next;
	}
	printf("NULL\n");
	scanf("%c", &c);
}



//FUNCTIONS



void copyGrid(int array1[][COL], int array2[][COL]){
	int i, j;

	for (i=0; i<ROW; i++){
		for (j=0; j<COL; j++){
			array2[i][j] = array1[i][j];
		}
	}

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

void gridDists(int array[][COL], int x, int y){
	int i, j;

	for (i=0; i<ROW; i++){
		for (j=0; j<COL; j++){
			if(array[i][j] == SPC) array[i][j] = distanceFormula(x, y, j, i);
			else array[i][j] = -1;
			printf("%d\t", array[i][j]);
		}
		printf("\n");
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
	int i, j, next, found=0;
	int visitedArray[ROW][COL];
	char c;
	
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
		// Sleep(SLP);
		// system("cls");
		// printGrid(array);
		//printf("\n");
		//printGrid(camefrom);

		nb = Neighbors(array, camefrom, dequeued[0], dequeued[1]);

		if (dequeued[0] == end[0] && dequeued[1] == end[1]){							//If a newly visited node is the goal, break from the loop twice!!!
			array[start[1]][start[0]] = STR;											//Mark the start coordinate
			array[end[1]][end[0]] = GOL;												//Mark the end coordinate
			found = 1;	
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

	if (found == 1){

		enqueue(&pathtrace, end[0], end[1]);
		
		while (!(pathtrace.rear->coord[0] == start[0] && pathtrace.rear->coord[1] == start[1])){
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
				default:
					break;
			}
		}

		reverseQueue(&pathtrace);
		printPath(camefrom, &pathtrace);
	}

	else printf("No path found.");
}

void enqueue2(Queue * Q, int x, int y, int startptx, int startpty, int endptx, int endpty){
	// QueueNode * alpha;
	// alpha = (QueueNode *)malloc(sizeof(QueueNode));
	// if (alpha == NULL) queueOverflow();
	// else {
	// 	alpha->coord = (int *)malloc(2*sizeof(int));
	// 	alpha->coord[0] = x;
	// 	alpha->coord[1] = y;
	// 	alpha->next = NULL;

	// 	if (Q->front == NULL){
	// 		Q->front = alpha;
	// 		Q->rear = alpha;
	// 	}

	// 	else {
	// 		Q->rear->next = alpha;
	// 		Q->rear = alpha;
	// 	}
	// }

	QueueNode * alpha;
	QueueNode * current = Q->front;
	float AlphaDistance, CurrentDistance;
	char buffer;

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
			AlphaDistance = heuristicDist(x, y, startptx, startpty, endptx, endpty);
			CurrentDistance = heuristicDist(Q->front->coord[0], Q->front->coord[1], startptx, startpty, endptx, endpty);
			
			if(AlphaDistance <= CurrentDistance){
				alpha->next = Q->front;
				Q->front = alpha;
				return;
			}

			else{
				while(current != Q->rear && current != NULL){
					// printf("Looping\n");
					CurrentDistance = heuristicDist(current->next->coord[0], current->next->coord[1], startptx, startpty, endptx, endpty);
					//scanf("%c", &buffer);

					if(AlphaDistance <= CurrentDistance){
						// printf("ENTERED\n");
						// scanf("%c", &buffer);
						alpha->next = current->next;
						current->next = alpha;
						return;						
					}

					current = current->next;
				}
				
				// printf("%.0f %.0f\n", CurrentDistance, AlphaDistance);
				// printf("REACHED\n");
				// scanf("%c", &buffer);
				Q->rear->next = alpha;
				Q->rear = alpha;
				return;
			}

			

		}
	}
}


Queue Neighbors_AStar(int array[][COL], int camefrom[][COL], int x, int y, int startptx, int startpty, int endx, int endy){
	Queue Neighbor;
	initQueue(&Neighbor);

	if (isInsideGrid(array, x, y-1) && isPassable(array, x, y-1) && camefrom[y-1][x] == SPC){
		enqueue2(&Neighbor, x, y-1, startptx, startpty, endx, endy);
		camefrom[y-1][x] = NTH;
	}
	if (isInsideGrid(array, x-1, y) && isPassable(array, x-1, y) && camefrom[y][x-1] == SPC){
		enqueue2(&Neighbor, x-1, y, startptx, startpty, endx, endy);
		camefrom[y][x-1] = WST;
	}
	if (isInsideGrid(array, x, y+1) && isPassable(array, x, y+1) && camefrom[y+1][x] == SPC){
		enqueue2(&Neighbor, x, y+1, startptx, startpty, endx, endy);
		camefrom[y+1][x] = STH;
	}
	if (isInsideGrid(array, x+1, y) && isPassable(array, x+1, y) && camefrom[y][x+1] == SPC){
		enqueue2(&Neighbor, x+1, y, startptx, startpty, endx, endy);
		camefrom[y][x+1] = EST;
	}

	if ((x+y) % 2 == 1) reverseQueue(&Neighbor);

	return Neighbor;
}

void AStar(int array[][COL], int camefrom[][COL], int start[], int end[]){
	
	int * dequeued;																		//Initialize all the variables
	int * dequeuedNeighbor;
	int i, j, next;
	int visitedArray[ROW][COL];
	char c;
	
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
	

	enqueue2(&Q, start[0], start[1], start[0], start[1], end[0], end[1]);													//Queue in the starting point
	visitedArray[start[1]][start[0]] = 1;												//Mark it as visited
	
	while (!isEmptyQueue(&Q)){															//While the queue is not empty

		
		// traverseQueue(&Q, end[0], end[1]);
		dequeued = dequeue(&Q);
		// printf("DEQUEUED: (%d, %d)\n", dequeued[0], dequeued[1]);
		// scanf("%c", &c);
		

		visitedArray[dequeued[1]][dequeued[0]] = 1;				
		array[dequeued[1]][dequeued[0]] = DNE;	
		//Sleep(SLP);
		system("cls");
		printGrid(array);
		printf("\n");
		printGrid(camefrom);

		nb = Neighbors_AStar(array, camefrom, dequeued[0], dequeued[1], start[0], start[1], end[0], end[1]);

		if (dequeued[0] == end[0] && dequeued[1] == end[1]){							//If a newly visited node is the goal, break from the loop twice!!!
			array[start[1]][start[0]] = STR;											//Mark the start coordinate
			array[end[1]][end[0]] = GOL;												//Mark the end coordinate	
			break;
		}		
		
		//Expand the neighbors by selecting the closest one. Queue Neighbors first into Neighbor Queue (becomes sorted). Dequeue them to the main Queue one by one so that their neigbhors will be enqueued in the future:

		while (!isEmptyQueue(&nb)){														//Now that we have the sorted nb, dequeue them then visit!
			dequeuedNeighbor = dequeue(&nb);
			if (visitedArray[dequeuedNeighbor[1]][dequeuedNeighbor[0]] == 0){			//Visit nodes if they haven't been visited though. Otherwise, skip
				// printf("FOUND: (%d, %d)\n", dequeuedNeighbor[0], dequeuedNeighbor[1]);
				enqueue2(&Q, dequeuedNeighbor[0], dequeuedNeighbor[1], start[0], start[1], end[0], end[1]);	
				// traverseQueue(&Q, end[0], end[1]);			
			}
		}

		// printf("POPPING NEW NODE\n");
	}

	enqueue2(&pathtrace, end[0], end[1], start[0], start[1], end[0], end[1]);
	
	while (!(pathtrace.rear->coord[0] == start[0] && pathtrace.rear->coord[1] == start[1])){
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
			default:
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


//MAZE GENERATOR CODECOPY-PASTED FROM https://azerdark.wordpress.com/2009/03/29/588/

void init_maze(int maze[][COL]){
	int a, b;
	for(a = 0; a < ROW; a++){
		for(b = 0; b < COL; b++){
			if(a % 2 == 0 || b % 2 == 0)
				maze[a][b] = OBS;
			else
				maze[a][b] = SPC;
		}
	}
}

int is_closed(int maze[][COL], int x, int y){
	if(maze[x - 1][y]  == OBS && maze[x][y - 1] == OBS && maze[x][y + 1] == OBS && maze[x + 1][y] == OBS && isInsideGrid(maze, y, x)) return 1; 
	return 0;
}
 
void maze_generator(int indeks, int maze[][COL], int backtrack_x[COL*ROW], int backtrack_y[ROW*COL], int x, int y, int visited){
	
	if(visited < (ROW/2)*(COL/2)){														//If there are still uncarved walls, 
		
		int neighbour_valid = -1;
		int neighbour_x[4];
		int neighbour_y[4];
		int step[4];
 
		int x_next;
		int y_next;

		if(x - 2 > 0 && is_closed(maze, x - 2, y)){										//up
			neighbour_valid++;
			neighbour_x[neighbour_valid]=x - 2;;
			neighbour_y[neighbour_valid]=y;
			step[neighbour_valid]=1;
		}
 
		if(y - 2 > 0 && is_closed(maze, x, y - 2)){										//left
			neighbour_valid++;
			neighbour_x[neighbour_valid]=x;
			neighbour_y[neighbour_valid]=y - 2;
			step[neighbour_valid]=2;
		}
 
		if(y + 2 < COL + 1 && is_closed(maze, x, y + 2)){								//right
			neighbour_valid++;
			neighbour_x[neighbour_valid]=x;
			neighbour_y[neighbour_valid]=y + 2;
			step[neighbour_valid]=3;
 
		}
 
		if(x + 2 < ROW + 1 && is_closed(maze, x + 2, y)){								//down
			neighbour_valid++;
			neighbour_x[neighbour_valid]=x+2;
			neighbour_y[neighbour_valid]=y;
			step[neighbour_valid]=4;
		}
 
		if(neighbour_valid == -1){														//backtrack if no more neighbors
			x_next = backtrack_x[indeks];
			y_next = backtrack_y[indeks];
			indeks--;
		}
 
		if(neighbour_valid != -1){														//if it has neighbors, randomly pick one to go next, then deepen the search
			int randomization = neighbour_valid + 1;
			int random = rand()%randomization;
			
			x_next = neighbour_x[random];
			y_next = neighbour_y[random];
			
			indeks++;
			
			backtrack_x[indeks] = x_next;
			backtrack_y[indeks] = y_next;
 
			int rstep = step[random];
 
			if(rstep == 1) maze[x_next+1][y_next] = SPC;
			else if(rstep == 2)	maze[x_next][y_next + 1] = SPC;
			else if(rstep == 3)	maze[x_next][y_next - 1] = SPC;
			else if(rstep == 4)	maze[x_next - 1][y_next] = SPC;
			
			visited++;
		}		
 
		maze_generator(indeks, maze, backtrack_x, backtrack_y, x_next, y_next, visited);
	}
}

//END MAZE GENERATOR CODE


int main(){

	system("cls");


	srand((unsigned)time(NULL));

	int grid[ROW][COL];
	int grid2[ROW][COL];
	int startpt[2] = {0, 17};
	int endpt[2] = {20, 9};

	int gridBFS[ROW][COL];
	int gridDFS[ROW][COL];

	int indeks = 0;
	int maze[ROW][COL];
	int mazeBackup[ROW][COL];
	
	int backtrack_x[(ROW-1)*(COL-1)];
	int backtrack_y[(ROW-1)*(COL-1)];

	char c;

	
	Queue Q;
	initQueue(&Q);
	init_maze(maze);

	//gridDists()
	// quadrilateralGenerator(grid, 10, 4, 28, 7);
	// quadrilateralGenerator(grid, 27, 4, 30, 14);
	//triangleGenerator(grid, 16, 10, 7);
	//printGrid(grid);

	//Sleep(1000);

	//DFS(grid, gridDFS, startpt, endpt);
	//printGrid(gridDFS);
	
	//Sleep(1000);
	
	// cleanGrid(grid);
	// quadrilateralGenerator(grid, 2, 2, 2, 5);
	// quadrilateralGenerator(grid, 2, 8, 2, 12);
	// quadrilateralGenerator(grid, 0, 11, 2, 11);
	// quadrilateralGenerator(grid, 7, 0, 7, 2);
	// quadrilateralGenerator(grid, 2, 2, 7, 2);


	//triangleGenerator(grid, 165, 20, 100);	
	//printGrid(grid);
	//BFS(grid, gridBFS, startpt, endpt);
	//printGrid(gridBFS);
	//Sleep(1000);
	//system("pause");
	//cleanGrid(grid); 

 
	backtrack_x[indeks] = 1;
	backtrack_y[indeks] = 1;
 
	//maze_generator(indeks, maze, backtrack_x, backtrack_y, 1, 1, 1);
	

	//copyGrid(maze, mazeBackup);
	// printGrid(maze);


	// system("pause");

	//copyGrid(mazeBackup, maze);
	//printGrid(maze);
	//BFS(maze, gridBFS, startpt, endpt);
	//gridDists(grid2, endpt[0], endpt[1]);
	//scanf("%c", &c);
	cleanGrid(grid);
	cleanGrid(gridBFS);
	cleanGrid(gridDFS);

	quadrilateralGenerator(grid, 1, 17, 17, 17);
	quadrilateralGenerator(grid, 17, 3, 17, 17);
	quadrilateralGenerator(grid, 17, 3, 17, 17);
	AStar(grid, gridBFS, startpt, endpt);

	
	//system("pause");

	// copyGrid(mazeBackup, maze);
	// printGrid(maze);
	// DFS(maze, gridDFS, startpt, endpt);

	// quadrilateralGenerator(grid, 2, 2, 2, 5);
	// quadrilateralGenerator(grid, 2, 8, 2, 12);
	// quadrilateralGenerator(grid, 0, 11, 2, 11);
	// quadrilateralGenerator(grid, 7, 0, 7, 2);
	// quadrilateralGenerator(grid, 2, 2, 7, 2);
	// DFS(grid, gridDFS, startpt, endpt);
	//printGrid(gridDFS);

	//printf("\n");
	//printGrid(gridBFS);

	return 0;
}