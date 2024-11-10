/**
 * Sudoku Algorithms
 * @author Clement Jourd'heuil
 * @version 1.0
 */

#include <chrono>
#include <iostream>

#define N 3
#define SIZE N * N

using namespace std;
using namespace std::chrono;

/**
 * Fill bit rows array from the grid
 * @param grid Array representing the grid
 * @param bitRows Array representing rows of the grid in a "bit" version
 */
inline void convert_grid_to_bit_row(int (&grid)[SIZE][SIZE], int (&bitRows)[SIZE]) {
    for (int row = 0; row < SIZE; ++row) {
        int bitmask = 0;
        for (int col = 0; col < SIZE; ++col) {
            int num = grid[row][col];
            if (num > 0) bitmask |= (1 << (num - 1));
        }
        bitRows[row] = bitmask;
    }
}

/**
 * Fill bit columns array from the grid
 * @param grid Array representing the grid
 * @param bitColumns Array representing colums of the grid in a "bit" version
 */
inline void convert_grid_to_bit_column(int (&grid)[SIZE][SIZE], int (&bitColumns)[SIZE]) {
    for (int col = 0; col < SIZE; ++col) {
        int bitmask = 0;
        for (int row = 0; row < SIZE; ++row) {
            int num = grid[row][col];
            if (num > 0) bitmask |= (1 << (num - 1));
        }
        bitColumns[col] = bitmask;
    }
}

/**
 * Fill bit cubes array from the grid
 * @param grid Array representing the grid
 * @param bitBoxes Array representing cubes of the grid in a "bit" version
 */
inline void convert_grid_to_bit_box(int (&grid)[SIZE][SIZE], int (&bitBoxes)[SIZE]) {
    for (int i = 0; i < SIZE; ++i) {
        int bitmask = 0;
        int startRow = (i / N) * N;
        int startCol = (i % N) * N;
        for (int row = 0; row < N; ++row) {
            for (int col = 0; col < N; ++col) {
                int num = grid[startRow + row][startCol + col];
                if (num > 0) bitmask |= (1 << (num - 1));
            }
        }
        bitBoxes[i] = bitmask;
    }
}

/**
 * Display the grid 
 * @param grid Array representing the grid
 */
inline void print_grid(int grid[SIZE][SIZE]) {
  for (int row = 0; row < SIZE; row++) {
    for (int col = 0; col < SIZE; col++) {
      cout << grid[row][col] << " ";
    }
    cout << endl;
  }
}


/**
 * Check if a number is placable in the current row and column and box
 * @param number The number we want to place
 * @param row The index of the row
 * @param column The index of the column
 * @param box The index of the box
 * @param bitRows Array representing rows of the grid in a "bit" version
 * @param bitColumns Array representing columns of the grid in a "bit" version
 * @param bitBoxes Array representing cubes of the grid in a "bit" version
 * @return True if the number is placable in the current row and column and box
 */
inline bool place_number(int number, int row, int column, int box,
                         const int (&bitRows)[SIZE], const int (&bitColumns)[SIZE],
                         const int (&bitBoxes)[SIZE]) {
    int bit = (1 << (number - 1));
    return ((bit & bitRows[row]) == 0) &&
           ((bit & bitColumns[column]) == 0) &&
           ((bit & bitBoxes[box]) == 0);
}

/**
 * Backtracking sudoku solver algorithm
 * @param grid Array representing the grid
 * @param bitRows Array representing rows of the grid in a "bit" version
 * @param bitColumns Array representing columns of the grid in a "bit" version
 * @param bitBoxes Array representing cubes of the grid in a "bit" version
 * @return True if the number is placable in the current row and column and box
 */
bool backtraking_solver(int (&grid)[SIZE][SIZE], int (&bitRows)[SIZE], int (&bitColumns)[SIZE],
                        int (&bitBoxes)[SIZE]) {
    int row = -1, col = -1;
    bool isEmpty = false;

    // Find an empty cell
    for (int i = 0; i < SIZE && !isEmpty; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (grid[i][j] == 0) {
                row = i;
                col = j;
                isEmpty = true;
                break;
            }
        }
    }

    // No empty cells, the Sudoku is solved
    if (!isEmpty) return true;

    int box = (row / N) * N + (col / N);  // Precompute the box index
    for (int num = 1; num <= SIZE; ++num) {
        if (place_number(num, row, col, box, bitRows, bitColumns, bitBoxes)) {
            int bit = 1 << (num - 1);
            grid[row][col] = num;
            bitRows[row] |= bit;
            bitColumns[col] |= bit;
            bitBoxes[box] |= bit;

            // Recursive call
            if (backtraking_solver(grid, bitRows, bitColumns, bitBoxes)) return true;

            // Backtrack
            grid[row][col] = 0;
            bitRows[row] &= ~bit;
            bitColumns[col] &= ~bit;
            bitBoxes[box] &= ~bit;
        }
    }
  return false;
}

int main() {
  // Easy one
  int grid[SIZE][SIZE] = {
      {0, 0, 0, 0, 0, 0, 6, 8, 0}, {0, 0, 0, 0, 7, 3, 0, 0, 9},
      {3, 0, 9, 0, 0, 0, 0, 4, 5}, {4, 9, 0, 0, 0, 0, 0, 0, 0},
      {8, 0, 3, 0, 5, 0, 9, 0, 2}, {0, 0, 0, 0, 0, 0, 0, 3, 6},
      {9, 6, 0, 0, 0, 0, 3, 0, 8}, {7, 0, 0, 6, 8, 0, 0, 0, 0},
      {0, 2, 8, 0, 0, 0, 0, 0, 0}};

  // Very hard one
  int grid2[SIZE][SIZE] = {
      {0, 0, 5, 3, 0, 0, 0, 0, 0}, {8, 0, 0, 0, 0, 0, 0, 2, 0},
      {0, 7, 0, 0, 1, 0, 5, 0, 0}, {4, 0, 0, 0, 0, 5, 3, 0, 0},
      {0, 1, 0, 0, 7, 0, 0, 0, 6}, {0, 0, 3, 2, 0, 0, 0, 8, 0},
      {0, 6, 0, 5, 0, 0, 0, 0, 9}, {0, 0, 4, 0, 0, 0, 0, 3, 0},
      {0, 0, 0, 0, 0, 9, 7, 0, 0}};

  // Demon one
  int grid3[SIZE][SIZE] = {
      {8, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 3, 6, 0, 0, 0, 0, 0},
      {0, 7, 0, 0, 9, 0, 2, 0, 0}, {0, 5, 0, 0, 0, 7, 0, 0, 0},
      {0, 0, 0, 0, 4, 5, 7, 0, 0}, {0, 0, 0, 1, 0, 0, 0, 3, 0},
      {0, 0, 1, 0, 0, 0, 0, 6, 8}, {0, 0, 8, 5, 0, 0, 0, 1, 0},
      {0, 9, 0, 0, 0, 0, 4, 0, 0}};

  // Variables for grid one

  int bitRows[SIZE];

  int bitColumns[SIZE];

  int bitBoxes[SIZE];

  convert_grid_to_bit_row(grid, bitRows);

  convert_grid_to_bit_column(grid, bitColumns);

  convert_grid_to_bit_box(grid, bitBoxes);

  // Variables for grid two

  int bitRows2[SIZE];

  int bitColumns2[SIZE];

  int bitBoxes2[SIZE];

  convert_grid_to_bit_row(grid2, bitRows2);

  convert_grid_to_bit_column(grid2, bitColumns2);

  convert_grid_to_bit_box(grid2, bitBoxes2);

  // Variables for grid three

  int bitRows3[SIZE];

  int bitColumns3[SIZE];

  int bitBoxes3[SIZE];

  convert_grid_to_bit_row(grid3, bitRows3);

  convert_grid_to_bit_column(grid3, bitColumns3);

  convert_grid_to_bit_box(grid3, bitBoxes3);

  // Solving grid one

  print_grid(grid);

  // Start of time mesure
  auto start = high_resolution_clock::now();

  if (backtraking_solver(grid, bitRows, bitColumns, bitBoxes)) {
    cout << "Solution for grid one found :" << endl;
    print_grid(grid);
  } else {
    cout << "No solution for grid one !" << endl;
  }

  // End of time mesure
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<nanoseconds>(stop - start);

  cout << "Execution time for backtracking algorithm : " << duration.count() 
    << " ns" << endl << endl;

  // Solving grid two

  print_grid(grid2);

  // Start of time mesure
  start = high_resolution_clock::now();

  if (backtraking_solver(grid2, bitRows2, bitColumns2, bitBoxes2)) {
    cout << "Solution for grid two found :" << endl;
    print_grid(grid2);
  } else {
    cout << "No solution for grid two !" << endl;
  }

  // End of time mesure
  stop = high_resolution_clock::now();
  duration = duration_cast<nanoseconds>(stop - start);

  cout << "Execution time for backtracking algorithm : " << duration.count() 
    << " ns" << endl << endl;

  // Solving grid three

  print_grid(grid3);

  // Start of time mesure
  start = high_resolution_clock::now();

  if (backtraking_solver(grid3, bitRows3, bitColumns3, bitBoxes3)) {
    cout << "Solution for grid three found :" << endl;
    print_grid(grid3);
  } else {
    cout << "No solution for three two !" << endl;
  }

  // End of time mesure
  stop = high_resolution_clock::now();
  duration = duration_cast<nanoseconds>(stop - start);

  cout << "Execution time for backtracking algorithm : " << duration.count() 
    << " ns" << endl;

  return 0;
}
