#include <stdio.h>
#include <time.h>

#define SIDE  10
#define MAX   SIDE*SIDE
// ClockWise from top
#define CLOCKWISEUP	0
// ClockWise from Up Right
#define CLOCKWISEUR	1
// ClockWise from Right
#define CLOCKWISERG	2
// ClockWise from Bottom Right
#define CLOCKWISEBR	3
// ClockWise from Bottom
#define CLOCKWISEBT	4
// ClockWise from Bottom Left
#define CLOCKWISEBL	5
// ClockWise from Left
#define CLOCKWISELF	6
// ClockWise from Up Left
#define CLOCKWISEUL	7


//---------------------- Program Parameters
// Path to use for search move
#define SEARCHPATH CLOCKWISERG

// Starting cell
#define START_X  5
#define START_Y  5
//------------------------------------------
// Results START_X=4, START_Y=3
// SIDE = 10    
// Time in seconds
//                  Non recursive   Recursive
// CLOCKWISEUP ->
// CLOCKWISEUR ->	        0.119       0.155
// CLOCKWISERG ->
// CLOCKWISEBR ->
// CLOCKWISEBT ->
// CLOCKWISEBL ->
// CLOCKWISELF ->
// CLOCKWISEUL ->

// Results START_X=5, START_Y=5
// SIDE = 10
// Time in seconds
//                  Non recursive   Recursive
// CLOCKWISEUP ->          11.425      15.804
// CLOCKWISEUR ->          33.673      46.157
// CLOCKWISERG ->           0.365       0.504 
// CLOCKWISEBR ->
// CLOCKWISEBT ->       <   0.083    <  0.116
// CLOCKWISEBL ->           0.193       0.274
// CLOCKWISELF ->
// CLOCKWISEUL ->          34.208      44.722



// Results START_X=5, START_Y=5
// SIDE = 9
// Time in seconds
//                  Non recursive   Recursive
// CLOCKWISEUP ->       <   0.001   <   0.001 
// CLOCKWISEUR ->			1.906       2.252
// CLOCKWISERG ->           
// CLOCKWISEBR ->
// CLOCKWISEBT ->           0.098       0.096
// CLOCKWISEBL ->
// CLOCKWISELF ->
// CLOCKWISEUL ->

// Results START_X=4, START_Y=4 (center)
// SIDE = 9
// Time in seconds
//                  Non recursive   Recursive
// CLOCKWISEUP ->          18.276      23.513 
// CLOCKWISEUR ->		    0.015   <   0.004
// CLOCKWISERG ->          18.305      22.994 
// CLOCKWISEBR ->           0.016       0.009
// CLOCKWISEBT ->          18.286      23.077
// CLOCKWISEBL ->       <   0.010   <   0.004
// CLOCKWISELF ->       >  18.396   >  23.818
// CLOCKWISEUL ->           0.011       0.005


// --------------------------------------------
int M[SIDE+6][SIDE+6] = {0};

#if SEARCHPATH == CLOCKWISEUP
	int Mx[8] = { 0, 2, 3, 2, 0,-2,-3,-2};
	int My[8] = {-3,-2, 0, 2, 3, 2, 0,-2};
#endif 
#if SEARCHPATH == CLOCKWISEUR
	int Mx[8] = { 2, 3, 2, 0,-2,-3,-2, 0};
	int My[8] = {-2, 0, 2, 3, 2, 0,-2,-3};
#endif 
#if SEARCHPATH == CLOCKWISERG
	int Mx[8] = { 3, 2, 0,-2,-3,-2, 0, 2};
	int My[8] = { 0, 2, 3, 2, 0,-2,-3,-2};
#endif 
#if SEARCHPATH == CLOCKWISEBR
	int Mx[8] = { 2, 0,-2,-3,-2, 0, 2, 3};
	int My[8] = { 2, 3, 2, 0,-2,-3,-2, 0};
#endif 
#if SEARCHPATH == CLOCKWISEBT
	int Mx[8] = { 0,-2,-3,-2, 0, 2, 3, 2};
	int My[8] = { 3, 2, 0,-2,-3,-2, 0, 2};
#endif 
#if SEARCHPATH == CLOCKWISEBL
	int Mx[8] = {-2,-3,-2, 0, 2, 3, 2, 0};
	int My[8] = { 2, 0,-2,-3,-2, 0, 2, 3};
#endif 
#if SEARCHPATH == CLOCKWISELF
	int Mx[8] = { -3,-2, 0, 2, 3, 2, 0,-2};
	int My[8] = {  0,-2,-3,-2, 0, 2, 3, 2};
#endif 
#if SEARCHPATH == CLOCKWISEUL
	int Mx[8] = {-2, 0, 2, 3, 2, 0,-2,-3};
	int My[8] = {-2,-3,-2, 0, 2, 3, 2, 0};
#endif 



int s[MAX][3] = {0};


void CreateBoundary() {
	// Create a big Boundary of 3 rows and 3 columns
	// on every side of matrix
	// to avoid the check of x,y out of bound
	for (int i = 0; i < SIDE + 6; i++) {
		M[0][i]=-1;M[SIDE+5][i]=-1;
		M[i][0]=-1;M[i][SIDE+5]=-1;
		M[1][i]=-1;M[SIDE+4][i]=-1;
		M[i][1]=-1;M[i][SIDE+4]=-1;
		M[2][i]=-1;M[SIDE+3][i]=-1;
		M[i][2]=-1;M[i][SIDE+3]=-1;
	}
}
// Print the Matrix
void PrintMatrix() {
	
	for (int i = 3; i < SIDE+3; ++i) {
		for (int j = 3; j < SIDE+3; ++j) {
			printf("%4d", M[i][j]);
		}
		printf("\n");
	}
	
}

// Recursive algorithm
int PlaceXY(int x, int y, int number) {
	int ok;
	ok = -1;

	if (M[y][x] != 0) return -1;
	M[y][x] = number;
	if (number < MAX){
		// Try all moves
		for (int i = 0; i < 8 && ok != 0; ++i) {
			ok = PlaceXY(x+Mx[i],y+My[i],number+1);
		}		
		if (ok) M[y][x] = 0;
			
	} else {
		// End of game
		ok = 0;
	}
	return ok;
}

// NON Recursive algorithm
int PlaceXYNR(int _x, int _y) {
	int sp=0, x=_x, y=_y;

	// sp is the "stack pointer" and the (current number to place)-1
	while (sp < MAX) {
		if (M[y][x] == 0) {
			// Legal Move
			// Assign the value to the cell
			s[sp][0]=x;
			s[sp][1]=y;
			s[sp][2]=0;
			M[y][x] = sp+1;

		} else {
			// invalid move
			// come back to previous move
			--sp;
			// BackTracking
			// if moves of cell are finished ...
			while(++s[sp][2]==8) {				
				// reset the cell ...
				M[s[sp][1]][s[sp][0]] = 0;
				// and check the previous one
				--sp;
			}
			// Take x, y from last good move
			x=s[sp][0];
			y=s[sp][1];
		}
		x+=Mx[s[sp][2]];
		y+=My[s[sp][2]];
		++sp;
	}
	return 0;
}

// Reset the Matrix
void CleanMatrix() {
	for (int i = 3; i < SIDE+3; ++i) {
		for (int j = 3; j < SIDE+3; ++j) {
			M[i][j]=0;
		}
	}

}
// Check if all moves are valid
int CheckMatrix(int x, int y) {
	int Ok = 1;
	int n = 1;
	for (int i = 0; i < MAX-1 && Ok == 1; ++i) {
		int found = 0;
		for (int j = 0; j < 8 && found == 0; ++j) {
			if (M[y + My[j]][x + Mx[j]] == n + 1) {
				found = 1;
				x += Mx[j];
				y += My[j];
				++n;
			}
		}
		Ok = found;
	}
	return Ok;
}


int main(int argc, char **argv){

	clock_t start,end;
	double tempo;

	int startx=START_X, starty=START_Y;

	CreateBoundary();

	printf("Clocks x Sec: %lu\n", CLOCKS_PER_SEC);
	start=clock();
	PlaceXYNR(startx+3,starty+3);
	end=clock();
	tempo=((double)(end-start))/CLOCKS_PER_SEC;
	PrintMatrix();
	printf("Time NR Sec.: %f (Clocks %lu)\n", tempo, end-start);
	printf("Check       : %d\n", CheckMatrix(startx+3,starty+3));


	CleanMatrix();

	start=clock();
	PlaceXY(startx+3,starty+3,1);
	end=clock();
	tempo=((double)(end-start))/CLOCKS_PER_SEC;
	PrintMatrix();
	printf("Time R Sec.: %f (Clocks %lu)\n", tempo, end-start);
	printf("Check      : %d\n", CheckMatrix(startx+3,starty+3));
	


}

