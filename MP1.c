#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <time.h>

#define ROW 201
#define COL 401
#define OBS 250
#define SPC 32
#define DNE 35
#define EDG 36
#define VTX 33
#define STR 65
#define GOL 90
#define SLP 10
#define INF -1

#define NON 32
#define DOT 250
#define BLK 254
#define ZRO 48
#define PTH 254

#define NTH 25
#define WST 26
#define STH 24
#define EST 27


//Polygon Generation
typedef struct PolyNode
{

    struct PolyNode * next;
    int numVertex;
    float * XCoord;
    float * YCoord;

} PolyNode;


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
	//system("cls");
	//printGrid(array);
	//Sleep(SLP);

}

void plotVertices(int array[][COL], float XCoord[], float YCoord[], int n){

	int i;

	for (i=0; i<n; i++) {
		plot(array, XCoord[i], YCoord[i], 2);
		// printf("%.0f %.0f\n", XCoord[i], YCoord[i]);
		// pressEnter();
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
				// printf("MORE Y (%d > %d): %.0f > %.0f\n", dy, dx, y1, y2);
				// pressEnter();
			}
		}

		else {
			for(i=y1+1; i < y2; i++){
				xPos = ((i- y2)/m) + (x2);
				yPos = i;
				plot(array, xPos, yPos, 3);
				// printf("MORE Y (%d > %d): %.0f <= %.0f\n", dy, dx, y1, y2);
				// pressEnter();
			}
		}
	}

	else{
		if (x1 < x2){
			for(i=x1+1; i<x2; i++){
				xPos = abs(i);
				yPos = abs((m * (i - x2)) + (y2));
				plot(array, xPos, yPos, 3);
				// printf("MORE X (%d > %d): %.0f < %.0f\n", dx, dy, x1, x2);
				// pressEnter();
			}
		}

		else {
			for(i=x2+1; i < x1; i++){
				xPos = abs(i);
				yPos = abs((m * (i - x2)) + (y2));
				plot(array, xPos, yPos, 3);
				// printf("MORE X (%d > %d): %.0f >= %.0f\n", dx, dy, x1, x2);
				// pressEnter();
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

	// printf("GENERATED EDGE BETWEEN (%.0f, %.0f) & (%.0f, %.0f) \n", x1, y1, x2, y2);
	// pressEnter();
}

void generatePolygon(int array[][COL], float XCoord[], float YCoord[], int n){

	plotVertices(array, XCoord, YCoord, n);
	//pressEnter();
	int i, j; 
	for (i = 0; i < n; i++){
		j = i + 1;

		if (j < n) generateEdge(array, XCoord[i], YCoord[i], XCoord[j], YCoord[j]);
		else generateEdge(array, XCoord[i], YCoord[i], XCoord[0], YCoord[0]);
	}
	//pressEnter();
	//fillPolygon(array, XCoord, YCoord, n);
	convertVertices(array);
	//pressEnter();

}

void traversePolyNodes(int array[][COL], PolyNode * start){

	PolyNode * current = start;
	int i, j = 1;
	char x;

	while(current != NULL){
		generatePolygon(array, current->XCoord, current->YCoord, current->numVertex);
		current = current->next;
	}
	printGrid(array);

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

void readPolygonData(PolyNode ** start, PolyNode ** current, int startpt[2], int endpt[2]){

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

	fscanf(fp, "%d %d", &startpt[0], &startpt[1]);
	fscanf(fp, "%d %d", &endpt[0], &endpt[1]);

	while(!feof(fp)){
		if (fscanf(fp, "%d", &n) == 1){
			// printf("Number of  Vertices: %d\n", n);
			createPolyNode(start, current, n);

			i = 0;
			// printf("X Coordinates ");
			while (i < n){
				fscanf(fp, "%d", &x);
				(*current)->XCoord[i] = x;
				i++;
			}
			// printf("\n");

			i = 0;
			// printf("Y Coordinates ");
			while (i < n){
				fscanf(fp, "%d", &y);
				(*current)->YCoord[i] = y;	
				i++;
			}
			// printf("\n\n");
		}
	}

    fclose(fp);

}


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
		free(alpha->coord);
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

	//STACK



typedef struct stacknode{
	int * coordNext;
	struct stacknode * next;
} StackNode;
typedef struct stack{

	StackNode * top;
} Stack;
void initStack(Stack * S){
	S->top = NULL;
}
int isEmptyStack(Stack * S){
	return (S->top == NULL);
}
void stackOverflow(void){
	printf("You have a stack overflow!\n\n");
	exit(1);
}
void stackUnderflow(void){
	printf("\nYou have a stack undeflow!\n\n");
	exit(1);
}
void push(Stack * S, int x, int y){
	StackNode * alpha;
	alpha = (StackNode *)malloc(sizeof(StackNode));
	if (alpha == NULL){
		stackOverflow();
	}

	else {
		alpha->coordNext = (int *)malloc(2*sizeof(int));
		alpha->coordNext[0] = x;
		alpha->coordNext[1] = y;

		alpha->next = S->top;
		S->top = alpha;
	}
}
void * pop(Stack * S){
	StackNode * alpha;

	if (S->top == NULL){
		stackUnderflow();
	}

	else {
		alpha = S->top;
		S->top = S->top->next;
		free(alpha->coordNext);
		free(alpha);		
	}
}
StackNode * peekStack(Stack * S){
	return S->top;
}
void traverse(Stack * S){
	StackNode * curr;
	curr = S->top;

	printf("\n\nTOP → ");

	while (curr != NULL){
		printf("(%d,", curr->coordNext[0]);
		printf("%d)  ", curr->coordNext[1]);
		curr = curr->next;
	}

	printf("NULL\n");

	free(curr);
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

int isInsideGrid(int x, int y){
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

	while (curr != NULL){
		array[curr->coord[1]][curr->coord[0]] = PTH;
		curr = curr->next;
	}
}


Queue Neighbors(int array[][COL], int camefrom[][COL], int x, int y){
	Queue Neighbor;
	initQueue(&Neighbor);

	if (isInsideGrid(x, y-1) && isPassable(array, x, y-1) && camefrom[y-1][x] == SPC){
		enqueue(&Neighbor, x, y-1);
		camefrom[y-1][x] = NTH;
	}
	if (isInsideGrid(x-1, y) && isPassable(array, x-1, y) && camefrom[y][x-1] == SPC){
		enqueue(&Neighbor, x-1, y);
		camefrom[y][x-1] = WST;
	}
	if (isInsideGrid(x, y+1) && isPassable(array, x, y+1) && camefrom[y+1][x] == SPC){
		enqueue(&Neighbor, x, y+1);
		camefrom[y+1][x] = STH;
	}
	if (isInsideGrid(x+1, y) && isPassable(array, x+1, y) && camefrom[y][x+1] == SPC){
		enqueue(&Neighbor, x+1, y);
		camefrom[y][x+1] = EST;
	}

	if ((x+y) % 2 == 1) reverseQueue(&Neighbor);

	return Neighbor;
}

void BFS(int array[][COL], int camefrom[][COL], int start[], int end[]){

	clock_t beginProcess, endProcess;
	double time_spent;

	beginProcess = clock();
	
	int * dequeued;																		//Initialize all the variables
	int * dequeuedNeighbor;
	int i, j, next, found=0, expandedNodes=0, costOfSolution=0;
	
	int ** visitedArray = (int **)malloc(ROW * sizeof(int *));
	for (i=0; i<ROW; i++)
		visitedArray[i] = (int *)malloc(COL * sizeof(int));
	
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

		expandedNodes++;

		dequeued = dequeue(&Q);

		if (dequeued[0] == end[0] && dequeued[1] == end[1]){							//If a newly visited node is the goal, break from the loop twice!!!
			array[start[1]][start[0]] = STR;											//Mark the start coordinate
			array[end[1]][end[0]] = GOL;												//Mark the end coordinate
			found = 1;	
			break;
		}

		visitedArray[dequeued[1]][dequeued[0]] = 1;				
		array[dequeued[1]][dequeued[0]] = DNE;	

		nb = Neighbors(array, camefrom, dequeued[0], dequeued[1]);		
		
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
		costOfSolution++;
		
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
			costOfSolution++;
		}

		reverseQueue(&pathtrace);
		printPath(camefrom, &pathtrace);
	}

	else printf("\nNo path found.\n");
	
	for (i=0; i<ROW; i++)
         free(visitedArray[i]);
	free(visitedArray);

	while (!isEmptyQueue(&Q)){
		dequeue(&Q);
	}

	endProcess = clock();
	time_spent = (double)(endProcess - beginProcess) / CLOCKS_PER_SEC;
	printf("\nNumber of expanded nodes: %d\nCost of solution: %d\nTime Spent: %lf\n", expandedNodes, costOfSolution, time_spent);
}

void DFS(int array[][COL], int camefrom[][COL], int start[], int end[]){

	clock_t beginProcess, endProcess;
	double time_spent;

	beginProcess = clock();	

	Stack S;
	initStack(&S);

	int found=0, expandedNodes=0, costOfSolution=0;

	if (!isPassable(array, start[0], start[1])) return;	

	array[end[1]][end[0]] = GOL;

	array[start[1]][start[0]] = DNE;

	push(&S, start[0], start[1]);

	while (!isEmptyStack(&S)){

		expandedNodes++;

		if (peekStack(&S)->coordNext[0] == end[0] && peekStack(&S)->coordNext[1] == end[1]){							//If a newly visited node is the goal, break from the loop twice!!!
			array[start[1]][start[0]] = STR;											//Mark the start coordinate
			array[end[1]][end[0]] = GOL;												//Mark the end coordinate
			found = 1;
			break;	
		}

		if (isPassable(array, peekStack(&S)->coordNext[0], peekStack(&S)->coordNext[1]-1) && isInsideGrid(peekStack(&S)->coordNext[0], peekStack(&S)->coordNext[1]-1) && camefrom[peekStack(&S)->coordNext[1]-1][peekStack(&S)->coordNext[0]] == SPC){
			camefrom[peekStack(&S)->coordNext[1]-1][peekStack(&S)->coordNext[0]] = NTH;
			array[peekStack(&S)->coordNext[1]-1][peekStack(&S)->coordNext[0]] = DNE;
			push(&S, peekStack(&S)->coordNext[0], peekStack(&S)->coordNext[1]-1);
			continue;
		}

		if (isPassable(array, peekStack(&S)->coordNext[0]-1, peekStack(&S)->coordNext[1]) && isInsideGrid(peekStack(&S)->coordNext[0]-1, peekStack(&S)->coordNext[1]) && camefrom[peekStack(&S)->coordNext[1]][peekStack(&S)->coordNext[0]-1] == SPC){
			camefrom[peekStack(&S)->coordNext[1]][peekStack(&S)->coordNext[0]-1] = WST;
			array[peekStack(&S)->coordNext[1]][peekStack(&S)->coordNext[0]-1] = DNE;
			push(&S, peekStack(&S)->coordNext[0]-1, peekStack(&S)->coordNext[1]);
			continue;
		}

		if (isPassable(array, peekStack(&S)->coordNext[0]+1, peekStack(&S)->coordNext[1]) && isInsideGrid(peekStack(&S)->coordNext[0]+1, peekStack(&S)->coordNext[1]) && camefrom[peekStack(&S)->coordNext[1]][peekStack(&S)->coordNext[0]+1] == SPC){
			camefrom[peekStack(&S)->coordNext[1]][peekStack(&S)->coordNext[0]+1] = EST;
			array[peekStack(&S)->coordNext[1]][peekStack(&S)->coordNext[0]+1] = DNE;
			push(&S, peekStack(&S)->coordNext[0]+1, peekStack(&S)->coordNext[1]);
			continue;
		}

		if (isPassable(array, peekStack(&S)->coordNext[0], peekStack(&S)->coordNext[1]+1) && isInsideGrid(peekStack(&S)->coordNext[0], peekStack(&S)->coordNext[1]+1) && camefrom[peekStack(&S)->coordNext[1]+1][peekStack(&S)->coordNext[0]] == SPC){
			camefrom[peekStack(&S)->coordNext[1]+1][peekStack(&S)->coordNext[0]] = STH;
			array[peekStack(&S)->coordNext[1]+1][peekStack(&S)->coordNext[0]] = DNE;
			push(&S, peekStack(&S)->coordNext[0], peekStack(&S)->coordNext[1]+1);
			continue;
		}

		pop(&S);

	}


	if (found == 1){

		Queue pathtrace;
		initQueue(&pathtrace);

		enqueue(&pathtrace, end[0], end[1]);
		costOfSolution++;

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
			}
			costOfSolution++;
		}

		reverseQueue(&pathtrace);
		printPath(camefrom, &pathtrace);
	}

	else printf("\nNo path found.\n");

	while (!isEmptyStack(&S)){
		pop(&S);
	}

	endProcess = clock();
	time_spent = (double)(endProcess - beginProcess) / CLOCKS_PER_SEC;
	printf("\nNumber of expanded nodes: %d\nCost of solution: %d\nTime Spent: %lf\n", expandedNodes, costOfSolution, time_spent);

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
	if(maze[x - 1][y]  == OBS && maze[x][y - 1] == OBS && maze[x][y + 1] == OBS && maze[x + 1][y] == OBS && isInsideGrid(y, x)) return 1; 
	return 0;
}

void maze_generator(int indeks, int maze[][COL], int backtrack_x[(ROW-1)*(COL-1)], int backtrack_y[(ROW-1)*(COL-1)], int x, int y, int visited){

	while (visited < (ROW/2)*(COL/2)){
		
		int * neighbour_valid = (int *)malloc(sizeof(int));
		
		*neighbour_valid = -1;

		int * neighbour_x = (int *)malloc(4*sizeof(int));
		int * neighbour_y = (int *)malloc(4*sizeof(int));
		int * step = (int *)malloc(4*sizeof(int));
 
		int * x_next = (int *)malloc(sizeof(int));
		int * y_next = (int *)malloc(sizeof(int));

		if(x - 2 > 0 && is_closed(maze, x - 2, y)){										//up
			++*neighbour_valid;
			neighbour_x[*neighbour_valid]=x - 2;
			neighbour_y[*neighbour_valid]=y;
			step[*neighbour_valid]=1;
		}
 
		if(y - 2 > 0 && is_closed(maze, x, y - 2)){										//left
			++*neighbour_valid;
			neighbour_x[*neighbour_valid]=x;
			neighbour_y[*neighbour_valid]=y - 2;
			step[*neighbour_valid]=2;
		}
 
		if(y + 2 < COL + 1 && is_closed(maze, x, y + 2)){								//right
			++*neighbour_valid;
			neighbour_x[*neighbour_valid]=x;
			neighbour_y[*neighbour_valid]=y + 2;
			step[*neighbour_valid]=3;
 
		}
 
		if(x + 2 < ROW + 1 && is_closed(maze, x + 2, y)){								//down
			++*neighbour_valid;
			neighbour_x[*neighbour_valid]=x+2;
			neighbour_y[*neighbour_valid]=y;
			step[*neighbour_valid]=4;
		}
 
		if(*neighbour_valid == -1){														//backtrack if no more neighbors
			*x_next = backtrack_x[indeks];
			*y_next = backtrack_y[indeks];
			--indeks;
		}
 
		if(*neighbour_valid != -1){														//if it has neighbors, randomly pick one to go next, then deepen the search
			int randomization = *neighbour_valid + 1;
			int random = rand()%randomization;
			
			*x_next = neighbour_x[random];
			*y_next = neighbour_y[random];
			
			indeks++;
			
			backtrack_x[indeks] = *x_next;
			backtrack_y[indeks] = *y_next;
 
			int rstep = step[random];
 
			if(rstep == 1) maze[*x_next+1][*y_next] = SPC;
			else if(rstep == 2)	maze[*x_next][*y_next + 1] = SPC;
			else if(rstep == 3)	maze[*x_next][*y_next - 1] = SPC;
			else if(rstep == 4)	maze[*x_next - 1][*y_next] = SPC;
			
			visited++;
		}

		x=*x_next;
		y=*y_next;
		
		free(neighbour_valid);
		free(neighbour_x);
		free(neighbour_y);
		free(step);
		free(x_next);
		free(y_next);
	}
}

//END MAZE GENERATOR CODE

//A*

//manhattan distance
int heuristicFxn(int x, int y, int startx, int starty, int endptx, int endpty){
	return abs(endptx - x) + abs(endpty - y) + abs(startx - x) + abs(starty - y);
}

//modified enqueue for priority queue for A* search
void enqueue2(Queue * Q, int x, int y, int startptx, int startpty, int endptx, int endpty){

	QueueNode * alpha;
	QueueNode * current = Q->front;
	int AlphaDistance, CurrentDistance;
	char buffer;


	alpha = (QueueNode *)malloc(sizeof(QueueNode));
	if (alpha == NULL) queueOverflow();
	else {
		alpha->coord = (int *)malloc(2*sizeof(int));
		alpha->coord[0] = x;
		alpha->coord[1] = y;
		alpha->next = NULL;

		AlphaDistance = heuristicFxn(x, y, startptx, startpty, endptx, endpty);

		if (Q->front == NULL){
			Q->front = alpha;
			Q->rear = alpha;
		}

		else {
			CurrentDistance = heuristicFxn(Q->front->coord[0], Q->front->coord[1], startptx, startpty, endptx, endpty);

			
			if(AlphaDistance < CurrentDistance){
				alpha->next = Q->front;
				Q->front = alpha;
				return;
			}

			else{
				while(current != Q->rear && current != NULL){
					CurrentDistance = heuristicFxn(current->next->coord[0], current->next->coord[1], startptx, startpty, endptx, endpty);
					
					if(AlphaDistance < CurrentDistance){
						alpha->next = current->next;
						current->next = alpha;
						return;						
					}

					current = current->next;
				}
				
				Q->rear->next = alpha;
				Q->rear = alpha;
				return;
			}

			

		}
	}
}

//modified BFS search for A* search
void AStar(int array[][COL], int camefrom[][COL], int start[], int end[]){

	clock_t beginProcess, endProcess;
	double time_spent;

	beginProcess = clock();
	
	int * dequeued;																		//Initialize all the variables
	int * dequeuedNeighbor;
	int i, j, next;
	int found=0, expandedNodes=0, costOfSolution=0;
	int newCost;
	
	Queue Q;
	initQueue(&Q);

	Queue nb;
	Queue pathtrace;
	initQueue(&pathtrace);
	
	int ** visitedArray = (int **)malloc(ROW * sizeof(int *));
	for (i=0; i<ROW; i++)
		visitedArray[i] = (int *)malloc(COL * sizeof(int));


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

		expandedNodes++;

		dequeued = dequeue(&Q);


		if (dequeued[0] == end[0] && dequeued[1] == end[1]){							//If a newly visited node is the goal, break from the loop twice!!!
			array[start[1]][start[0]] = STR;											//Mark the start coordinate
			array[end[1]][end[0]] = GOL;												//Mark the end coordinate	
			found = 1;
			break;
		}		
		
		visitedArray[dequeued[1]][dequeued[0]] = 1;				
		array[dequeued[1]][dequeued[0]] = DNE;
		
		nb = Neighbors(array, camefrom, dequeued[0], dequeued[1]);
		//Expand the neighbors by selecting the closest one. Queue Neighbors first into Neighbor Queue (becomes sorted). Dequeue them to the main Queue one by one so that their neigbhors will be enqueued in the future:

		while (!isEmptyQueue(&nb)){														//Now that we have the sorted nb, dequeue them then visit!
			dequeuedNeighbor = dequeue(&nb);

			if (visitedArray[dequeuedNeighbor[1]][dequeuedNeighbor[0]] == 0){			//Visit nodes if they haven't been visited though. Otherwise, skip
				enqueue2(&Q, dequeuedNeighbor[0], dequeuedNeighbor[1], start[0], start[1], end[0], end[1]);	
				
			}
		}
	}

	if (found == 1){

		enqueue(&pathtrace, end[0], end[1]);
		costOfSolution++;
		
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
			costOfSolution++;
		}

		reverseQueue(&pathtrace);
		printPath(camefrom, &pathtrace);
	}

	else printf("\nNo path found.\n");
	
	for (i=0; i<ROW; i++)
         free(visitedArray[i]);
	free(visitedArray);

	while (!isEmptyQueue(&Q)){
		dequeue(&Q);
	}

	endProcess = clock();
	time_spent = (double)(endProcess - beginProcess) / CLOCKS_PER_SEC;
	printf("\nNumber of expanded nodes: %d\nCost of solution: %d\nTime Spent: %lf\n", expandedNodes, costOfSolution, time_spent);
}


int main(){

	system("cls");

	srand((unsigned)time(NULL));

	int grid[ROW][COL];


	// int startpt[2] = {1, 1};
	// int endpt[2] = {399, 199};

	int startpt[2], endpt[2];

	int gridPaths[ROW][COL];

	int indeks = 0;
	int gridBackup[ROW][COL];
	
	int backtrack_x[(ROW-1)*(COL-1)];
	int backtrack_y[(ROW-1)*(COL-1)];

	
	Queue Q;
	initQueue(&Q);


	cleanGrid(gridPaths);
 
	backtrack_x[indeks] = 1;
	backtrack_y[indeks] = 1;

	PolyNode * start;
	PolyNode * current;
	start = NULL;
	current = NULL;
	
	cleanGrid(grid);
	readPolygonData(&start, &current, startpt, endpt);
	traversePolyNodes(grid, start);	
	

	// init_maze(grid);
	// maze_generator(indeks, grid, backtrack_x, backtrack_y, 1, 1, 1);
	// printGrid(grid);
	// printf("\n");
	
	copyGrid(grid, gridBackup);
	BFS(grid, gridPaths, startpt, endpt);
	printGrid(gridPaths);
	
	printf("\n");

	cleanGrid(gridPaths);
	copyGrid(gridBackup, grid);
	AStar(grid, gridPaths, startpt, endpt);
	printGrid(gridPaths);
	
	printf("\n");
	
	cleanGrid(gridPaths);
	copyGrid(gridBackup, grid);
	DFS(grid, gridPaths, startpt, endpt);
	printGrid(gridPaths);

	printf("\n");

	return 0;
}