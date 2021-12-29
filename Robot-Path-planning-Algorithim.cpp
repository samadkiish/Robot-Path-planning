
// Defined in header <chrono> & <bits/stdc++.h>
#include <chrono>
#include <bits/stdc++.h>
// Defined in namespace std & std::chrono
using namespace std;
using namespace std::chrono; 

//The following size is use to mazee / draw maze 2D 12 * 24
#define ROW 12
#define COL 24
//The following numbers is use to represent different types of blocks: 
#define WALL 1
#define SPACE 0
#define INITIAL -1
#define TARGET 9
#define PATH '.'
int DIST_LENGTH = 0; // this varibale is using to store distancen length

// Creating a shortcut for int, int pair type
typedef pair<int, int> Pair;

// Creating a shortcut for pair<int, pair<int, int>> type
typedef pair<double, pair<int, int> > pPair;

// A structure to hold the neccesary parameters
struct cell {
	// Row and Column index of its parent
	// Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
	int parent_i, parent_j;
	// f = g + h
	double f, g, h;
};

// This Function is used to check whether 
// given cell (row, col) is a valid cell or not.
bool is_valid(int row, int col){

	// Returns true if row number and column number
	// is in range
	return (row >= 0) && (row < ROW) && (col >= 0)
		&& (col < COL);
}

// This Function is used to check whether 
// the given cell is wall or not
bool is_wall(int maze[][COL], int row, int col){

	// Returns false if the cell is wall else true
	if (maze[row][col] == WALL)
		return (false);
	else
		return (true);
}


// This  Function is used to check whether 
// target cell has been reached or not
bool is_target(int row, int col, Pair dest){

	if (row == dest.first && col == dest.second)
		return (true);
	else
		return (false);
}

// This  Function is used to calculate the 'h' heuristics.
double calculate_heuris_value(int row, int col, Pair dest){

	// Return using the distance formula
	return ((double)sqrt(
		(row - dest.first) * (row - dest.first)
		+ (col - dest.second) * (col - dest.second)));
}

// This  Function is used to trace the path
// from the source to target
void trace_path(cell cell_detial[][COL], Pair dest){

	cout <<"\nThe Trace Path is: ";
	int row = dest.first;
	int col = dest.second;

	stack<Pair> Path;

	while (!(cell_detial[row][col].parent_i == row
			&& cell_detial[row][col].parent_j == col)) {
		Path.push(make_pair(row, col));
		int temp_row = cell_detial[row][col].parent_i;
		int temp_col = cell_detial[row][col].parent_j;
		row = temp_row;
		col = temp_col;
	}

	Path.push(make_pair(row, col));
	while (!Path.empty()) {
		pair<int, int> p = Path.top();
		Path.pop();
		DIST_LENGTH++;
		cout <<"-> ("<<p.first <<"," <<p.second <<") ";
	}
	cout << "\nThe shortest path is: " << DIST_LENGTH <<endl;
	// draw_maze(cell_detial[p.first][p.second]);
	return;
}

// This Function is used to search the shortest path between 
// the source and the distination With A start Search Algorithm
void a_start(int maze[][COL], Pair src, Pair dest)
{
	// If the source is out of range
	if (is_valid(src.first, src.second) == false) {
		cout <<"Source is invalid"<<endl;
		return;
	}

	// If the target is out of range
	if (is_valid(dest.first, dest.second) == false) {
		cout <<"Target is invalid"<<endl;
		return;
	}

	// Either the source or the target is the wall
	if (is_wall(maze, src.first, src.second) == false || is_wall(maze, dest.first, dest.second)	== false) {
		cout <<"Source or the Target is the wall"<<endl;
		return;
	}

	// If the target cell is the same as source cell
	if (is_target(src.first, src.second, dest) == true) {
		cout <<"Already at the target"<<endl;
		return;
	}

	// Create a closed list and initialise it to false which
	// means that no cell has been included yet This closed
	// list is implemented as a boolean 2D array
	bool close_list[ROW][COL];
	memset(close_list, false, sizeof(close_list));

	// Declare a 2D array of structure to hold the details
	// of that cell
	cell cell_detial[ROW][COL];

	int i, j;

	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COL; j++) {
			cell_detial[i][j].f = FLT_MAX;
			cell_detial[i][j].g = FLT_MAX;
			cell_detial[i][j].h = FLT_MAX;
			cell_detial[i][j].parent_i = -1;
			cell_detial[i][j].parent_j = -1;
		}
	}

	// Initialising the parameters of the starting node
	i = src.first, j = src.second;
	cell_detial[i][j].f = 0.0;
	cell_detial[i][j].g = 0.0;
	cell_detial[i][j].h = 0.0;
	cell_detial[i][j].parent_i = i;
	cell_detial[i][j].parent_j = j;

	/*
	Create an open list having information as-
	<f, <i, j>>
	where f = g + h,
	and i, j are the row and column index of that cell
	Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
	This open list is implenented as a set of pair of
	pair.*/
	set<pPair> open_list;

	// Put the starting cell on the open list and set its
	// 'f' as 0
	open_list.insert(make_pair(0.0, make_pair(i, j)));

	// We set this boolean value as false as initially
	// the target is not reached.
	bool found_dest = false;

	while (!open_list.empty()) {
		pPair p = *open_list.begin();

		// Remove this vertex from the open list
		open_list.erase(open_list.begin());

		// Add this vertex to the closed list
		i = p.second.first;
		j = p.second.second;
		close_list[i][j] = true;

	

		// To store the 'g', 'h' and 'f' of the 8 Alternatives
		double g_new, h_new, f_new;

		//----------- 1st Alternative (Up) ------------

		// Only process this cell if this is a valid one
		if (is_valid(i - 1, j) == true) {
			// If the target cell is the same as the
			// current Alternative
			if (is_target(i - 1, j, dest) == true) {
				// Set the Parent of the target cell
				cell_detial[i - 1][j].parent_i = i;
				cell_detial[i - 1][j].parent_j = j;
				cout <<"Found the cell target"<<endl;
				trace_path(cell_detial, dest);
				found_dest = true;
				return;
			}
			// If the Alternative is already on the closed
			// list or if it is the wall, then ignore it.
			// Else do the following
			else if (close_list[i - 1][j] == false
					&& is_wall(maze, i - 1, j)
							== true) {
				g_new = cell_detial[i][j].g + 1.0;
				h_new = calculate_heuris_value(i - 1, j, dest);
				f_new = g_new + h_new;

				// If it isn’t on the open list, add it to
				// the open list. Make the current square
				// the parent of this square. Record the
				// f, g, and h costs of the square cell
				//			 OR
				// If it is on the open list already, check
				// to see if this path to that square is
				// better, using 'f' cost as the measure.
				if (cell_detial[i - 1][j].f == FLT_MAX
					|| cell_detial[i - 1][j].f > f_new) {
					open_list.insert(make_pair(
						f_new, make_pair(i - 1, j)));

					// Update the details of this cell
					cell_detial[i - 1][j].f = f_new;
					cell_detial[i - 1][j].g = g_new;
					cell_detial[i - 1][j].h = h_new;
					cell_detial[i - 1][j].parent_i = i;
					cell_detial[i - 1][j].parent_j = j;
				}
			}
		}

		//----------- 2nd Alternative (Down) ------------

		// Only process this cell if this is a valid one
		if (is_valid(i + 1, j) == true) {
			// If the target cell is the same as the
			// current Alternative
			if (is_target(i + 1, j, dest) == true) {
				// Set the Parent of the target cell
				cell_detial[i + 1][j].parent_i = i;
				cell_detial[i + 1][j].parent_j = j;
				cout <<"Found the cell target"<<endl;
				trace_path(cell_detial, dest);
				found_dest = true;
				return;
			}
			// If the Alternative is already on the closed
			// list or if it is the wall, then ignore it.
			// Else do the following
			else if (close_list[i + 1][j] == false
					&& is_wall(maze, i + 1, j)
							== true) {
				g_new = cell_detial[i][j].g + 1.0;
				h_new = calculate_heuris_value(i + 1, j, dest);
				f_new = g_new + h_new;

				// If it isn’t on the open list, add it to
				// the open list. Make the current square
				// the parent of this square. Record the
				// f, g, and h costs of the square cell
				//			 OR
				// If it is on the open list already, check
				// to see if this path to that square is
				// better, using 'f' cost as the measure.
				if (cell_detial[i + 1][j].f == FLT_MAX
					|| cell_detial[i + 1][j].f > f_new) {
					open_list.insert(make_pair(
						f_new, make_pair(i + 1, j)));
					// Update the details of this cell
					cell_detial[i + 1][j].f = f_new;
					cell_detial[i + 1][j].g = g_new;
					cell_detial[i + 1][j].h = h_new;
					cell_detial[i + 1][j].parent_i = i;
					cell_detial[i + 1][j].parent_j = j;
				}
			}
		}

		//----------- 3rd Alternative (Right) ------------

		// Only process this cell if this is a valid one
		if (is_valid(i, j + 1) == true) {
			// If the target cell is the same as the
			// current Alternative
			if (is_target(i, j + 1, dest) == true) {
				// Set the Parent of the target cell
				cell_detial[i][j + 1].parent_i = i;
				cell_detial[i][j + 1].parent_j = j;
				cout <<"Found the cell target"<<endl;
				trace_path(cell_detial, dest);
				found_dest = true;
				return;
			}

			// If the Alternative is already on the closed
			// list or if it is the wall, then ignore it.
			// Else do the following
			else if (close_list[i][j + 1] == false
					&& is_wall(maze, i, j + 1)
							== true) {
				g_new = cell_detial[i][j].g + 1.0;
				h_new = calculate_heuris_value(i, j + 1, dest);
				f_new = g_new + h_new;

				// If it isn’t on the open list, add it to
				// the open list. Make the current square
				// the parent of this square. Record the
				// f, g, and h costs of the square cell
				//			 OR
				// If it is on the open list already, check
				// to see if this path to that square is
				// better, using 'f' cost as the measure.
				if (cell_detial[i][j + 1].f == FLT_MAX
					|| cell_detial[i][j + 1].f > f_new) {
					open_list.insert(make_pair(
						f_new, make_pair(i, j + 1)));

					// Update the details of this cell
					cell_detial[i][j + 1].f = f_new;
					cell_detial[i][j + 1].g = g_new;
					cell_detial[i][j + 1].h = h_new;
					cell_detial[i][j + 1].parent_i = i;
					cell_detial[i][j + 1].parent_j = j;
				}
			}
		}

		//----------- 4th Alternative (Left) ------------

		// Only process this cell if this is a valid one
		if (is_valid(i, j - 1) == true) {
			// If the target cell is the same as the
			// current Alternative
			if (is_target(i, j - 1, dest) == true) {
				// Set the Parent of the target cell
				cell_detial[i][j - 1].parent_i = i;
				cell_detial[i][j - 1].parent_j = j;
				cout <<"Found the cell target"<<endl;
				trace_path(cell_detial, dest);
				found_dest = true;
				return;
			}

			// If the Alternative is already on the closed
			// list or if it is the wall, then ignore it.
			// Else do the following
			else if (close_list[i][j - 1] == false
					&& is_wall(maze, i, j - 1)
							== true) {
				g_new = cell_detial[i][j].g + 1.0;
				h_new = calculate_heuris_value(i, j - 1, dest);
				f_new = g_new + h_new;

				// If it isn’t on the open list, add it to
				// the open list. Make the current square
				// the parent of this square. Record the
				// f, g, and h costs of the square cell
				//			 OR
				// If it is on the open list already, check
				// to see if this path to that square is
				// better, using 'f' cost as the measure.
				if (cell_detial[i][j - 1].f == FLT_MAX
					|| cell_detial[i][j - 1].f > f_new) {
					open_list.insert(make_pair(
						f_new, make_pair(i, j - 1)));

					// Update the details of this cell
					cell_detial[i][j - 1].f = f_new;
					cell_detial[i][j - 1].g = g_new;
					cell_detial[i][j - 1].h = h_new;
					cell_detial[i][j - 1].parent_i = i;
					cell_detial[i][j - 1].parent_j = j;
				}
			}
		}

		//----------- 5th Alternative (Up-Right)
		//------------

		// Only process this cell if this is a valid one
		if (is_valid(i - 1, j + 1) == true) {
			// If the target cell is the same as the
			// current Alternative
			if (is_target(i - 1, j + 1, dest) == true) {
				// Set the Parent of the target cell
				cell_detial[i - 1][j + 1].parent_i = i;
				cell_detial[i - 1][j + 1].parent_j = j;
				cout <<"Found the cell target"<<endl;
				trace_path(cell_detial, dest);
				found_dest = true;
				return;
			}

			// If the Alternative is already on the closed
			// list or if it is the wall, then ignore it.
			// Else do the following
			else if (close_list[i - 1][j + 1] == false
					&& is_wall(maze, i - 1, j + 1)
							== true) {
				g_new = cell_detial[i][j].g + 1.414;
				h_new = calculate_heuris_value(i - 1, j + 1, dest);
				f_new = g_new + h_new;

				// If it isn’t on the open list, add it to
				// the open list. Make the current square
				// the parent of this square. Record the
				// f, g, and h costs of the square cell
				//			 OR
				// If it is on the open list already, check
				// to see if this path to that square is
				// better, using 'f' cost as the measure.
				if (cell_detial[i - 1][j + 1].f == FLT_MAX
					|| cell_detial[i - 1][j + 1].f > f_new) {
					open_list.insert(make_pair(
						f_new, make_pair(i - 1, j + 1)));

					// Update the details of this cell
					cell_detial[i - 1][j + 1].f = f_new;
					cell_detial[i - 1][j + 1].g = g_new;
					cell_detial[i - 1][j + 1].h = h_new;
					cell_detial[i - 1][j + 1].parent_i = i;
					cell_detial[i - 1][j + 1].parent_j = j;
				}
			}
		}

		//----------- 6th Alternative (Up-Left)
		//------------

		// Only process this cell if this is a valid one
		if (is_valid(i - 1, j - 1) == true) {
			// If the target cell is the same as the
			// current Alternative
			if (is_target(i - 1, j - 1, dest) == true) {
				// Set the Parent of the target cell
				cell_detial[i - 1][j - 1].parent_i = i;
				cell_detial[i - 1][j - 1].parent_j = j;
				cout <<"Found the cell target"<<endl;
				trace_path(cell_detial, dest);
				found_dest = true;
				return;
			}

			// If the Alternative is already on the closed
			// list or if it is the wall, then ignore it.
			// Else do the following
			else if (close_list[i - 1][j - 1] == false
					&& is_wall(maze, i - 1, j - 1)
							== true) {
				g_new = cell_detial[i][j].g + 1.414;
				h_new = calculate_heuris_value(i - 1, j - 1, dest);
				f_new = g_new + h_new;

				// If it isn’t on the open list, add it to
				// the open list. Make the current square
				// the parent of this square. Record the
				// f, g, and h costs of the square cell
				//			 OR
				// If it is on the open list already, check
				// to see if this path to that square is
				// better, using 'f' cost as the measure.
				if (cell_detial[i - 1][j - 1].f == FLT_MAX
					|| cell_detial[i - 1][j - 1].f > f_new) {
					open_list.insert(make_pair(
						f_new, make_pair(i - 1, j - 1)));
					// Update the details of this cell
					cell_detial[i - 1][j - 1].f = f_new;
					cell_detial[i - 1][j - 1].g = g_new;
					cell_detial[i - 1][j - 1].h = h_new;
					cell_detial[i - 1][j - 1].parent_i = i;
					cell_detial[i - 1][j - 1].parent_j = j;
				}
			}
		}

		//----------- 7th Alternative (Down-Right)
		//------------

		// Only process this cell if this is a valid one
		if (is_valid(i + 1, j + 1) == true) {
			// If the target cell is the same as the
			// current Alternative
			if (is_target(i + 1, j + 1, dest) == true) {
				// Set the Parent of the target cell
				cell_detial[i + 1][j + 1].parent_i = i;
				cell_detial[i + 1][j + 1].parent_j = j;
				cout <<"Found the cell target"<<endl;
				trace_path(cell_detial, dest);
				found_dest = true;
				return;
			}

			// If the Alternative is already on the closed
			// list or if it is the wall, then ignore it.
			// Else do the following
			else if (close_list[i + 1][j + 1] == false
					&& is_wall(maze, i + 1, j + 1)
							== true) {
				g_new = cell_detial[i][j].g + 1.414;
				h_new = calculate_heuris_value(i + 1, j + 1, dest);
				f_new = g_new + h_new;

				// If it isn’t on the open list, add it to
				// the open list. Make the current square
				// the parent of this square. Record the
				// f, g, and h costs of the square cell
				//			 OR
				// If it is on the open list already, check
				// to see if this path to that square is
				// better, using 'f' cost as the measure.
				if (cell_detial[i + 1][j + 1].f == FLT_MAX
					|| cell_detial[i + 1][j + 1].f > f_new) {
					open_list.insert(make_pair(
						f_new, make_pair(i + 1, j + 1)));

					// Update the details of this cell
					cell_detial[i + 1][j + 1].f = f_new;
					cell_detial[i + 1][j + 1].g = g_new;
					cell_detial[i + 1][j + 1].h = h_new;
					cell_detial[i + 1][j + 1].parent_i = i;
					cell_detial[i + 1][j + 1].parent_j = j;
				}
			}
		}

		//----------- 8th Alternative (Down-Left)
		//------------

		// Only process this cell if this is a valid one
		if (is_valid(i + 1, j - 1) == true) {
			// If the target cell is the same as the
			// current Alternative
			if (is_target(i + 1, j - 1, dest) == true) {
				// Set the Parent of the target cell
				cell_detial[i + 1][j - 1].parent_i = i;
				cell_detial[i + 1][j - 1].parent_j = j;
				cout <<"Found the cell target"<<endl;
				trace_path(cell_detial, dest);
				found_dest = true;
				return;
			}

			// If the Alternative is already on the closed
			// list or if it is the wall, then ignore it.
			// Else do the following
			else if (close_list[i + 1][j - 1] == false
					&& is_wall(maze, i + 1, j - 1)
							== true) {
				g_new = cell_detial[i][j].g + 1.414;
				h_new = calculate_heuris_value(i + 1, j - 1, dest);
				f_new = g_new + h_new;

				// If it isn’t on the open list, add it to
				// the open list. Make the current square
				// the parent of this square. Record the
				// f, g, and h costs of the square cell
				//			 OR
				// If it is on the open list already, check
				// to see if this path to that square is
				// better, using 'f' cost as the measure.
				if (cell_detial[i + 1][j - 1].f == FLT_MAX
					|| cell_detial[i + 1][j - 1].f > f_new) {
					open_list.insert(make_pair(
						f_new, make_pair(i + 1, j - 1)));

					// Update the details of this cell
					cell_detial[i + 1][j - 1].f = f_new;
					cell_detial[i + 1][j - 1].g = g_new;
					cell_detial[i + 1][j - 1].h = h_new;
					cell_detial[i + 1][j - 1].parent_i = i;
					cell_detial[i + 1][j - 1].parent_j = j;
				}
			}
		}
	}

	// When the target cell is not found and the open
	// list is empty, then we conclude that we failed to
	// reach the destiantion cell. This may happen when the
	// there is no way to target cell (due to
	// blockages)
	if (found_dest == false)
		cout <<"Failed to find the target Cell"<<endl;

	return;
}

// This Function is used to draw the 2D maze
void draw_maze(int maze[][COL]){
	
	cout<<"\n"<<endl;
	
	for(int i=0; i<ROW; i++){
	
		for(int j=0; j<COL; j++){
			
			switch(maze[i][j]){
				
				case WALL:
					cout<<"|";
					break;
				case SPACE:
					cout<<" ";
					break;
				case INITIAL:
					cout<<"*";
					break;
				case TARGET:
					cout<<"#";
					break;
				case PATH:
					cout<<"*";
				// 	break;
			}
		}
		cout<<endl;
	}
	
	cout<<"\n"<<endl;
}


// The main function
int main()
{
	// declaring variable type time_t 
	time_t current_time;


	/* Description of the maze
	-1:- The cell is the initial
	 0:- The cell is space
	 1:- The cell is wall 
	 9:- The cell is the target
	*/
	// maze 2D (12*24)
	int maze[ROW][COL] = { 
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,-1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,0,1},
            {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,1},
            {1,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,0,0,9,1,0,1},
            {1,0,0,0,0,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0,0,1,0,1},
            {1,0,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1},
            {1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,1},
            {1,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,0,1,0,0,0,1,0,1},
            {1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1},
            {1,0,0,0,1,0,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}, 
    };


	
	// Source is  the second position into the third row
	Pair src = make_pair(2, 2);

	// Distination/Target is the the last fourth position into the five row
	Pair dest = make_pair(4, 20);

	// calling drawing function 
	draw_maze(maze);

	// Get starting timepoint
    auto start = high_resolution_clock::now();

	//calling a start search fucntion
	a_start(maze, src, dest);

	// Get ending timepoint
    auto stop = high_resolution_clock::now();

	// This code (get calclating running time) is third-party  Article Contributed By : GeeksforGeeks
	// Get duration. Substart timepoints to 
	// get durarion. To cast it to proper unit
	// use duration cast method
    auto duration = duration_cast<milliseconds>(stop - start);
  
	// Printing Time taken 
    cout << "\nTime taken by function A* Algorithm: "
         << duration.count() << " milliseconds" << endl;

	return (0);
}
