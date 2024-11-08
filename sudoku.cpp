#include <chrono>
#include <cmath>
#include <iostream>

#define N 3
#define SIZE N *N

using namespace std;
using namespace std::chrono;

void convert_grid_to_bit_row(int grid[SIZE][SIZE], int bit[SIZE]) {
  for (int row = 0; row < SIZE; row++) {
    int bitmask = 0;
    for (int col = 0; col < SIZE; col++) {
      int num = grid[row][col];
      if (num > 0) {
        bitmask |= (1 << (num - 1));
      }
    }
    bit[row] = bitmask;
  }
}

void convert_grid_to_bit_column(int grid[SIZE][SIZE], int bit[SIZE]) {
  for (int col = 0; col < SIZE; col++) {
    int bitmask = 0;
    for (int row = 0; row < SIZE; row++) {
      int num = grid[row][col];
      if (num > 0) {
        bitmask |= (1 << (num - 1));
      }
    }
    bit[col] = bitmask;
  }
}

void convert_grid_to_bit_box(int grid[SIZE][SIZE], int bit[SIZE]) {
  for (int i = 0; i < SIZE; i++) {
    int bitmask = 0;

    int startRow = (i / N) * N;
    int startCol = (i % N) * N;

    for (int row = 0; row < N; row++) {
      for (int col = 0; col < N; col++) {
        int num = grid[startRow + row][startCol + col];
        if (num > 0) {
          bitmask |= (1 << (num - 1));
        }
      }
    }

    bit[i] = bitmask;
  }
}

void print_grid(int grid[SIZE][SIZE]) {
  for (int row = 0; row < SIZE; row++) {
    for (int col = 0; col < SIZE; col++) {
      cout << grid[row][col] << " ";
    }
    cout << endl;
  }
}

void print_bit_grid(int bit[SIZE]) {
  for (int i = 0; i < SIZE; i++) {
    cout << bit[i] << " ";
  }
  cout << endl;
}

int convert_number_to_bit(int number) { return pow(2, number - 1); }

inline int place_number(int number, int row, int column, int box,
                        int bitRow[SIZE], int bitColumn[SIZE],
                        int bitBox[SIZE]) {
  number = convert_number_to_bit(number);

  // Row and column placing verification
  if (((number & bitRow[row]) != 0) || ((number & bitColumn[column]) != 0) ||
      ((number & bitBox[box]) != 0)) {
    // Placing is impossible
    return 0;
  } else {
    // Placing is possible
    return 1;
  }
}

int backtraking_solver() { return 1; }

int main() {
  int grid[SIZE][SIZE] = {
      {8, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 3, 6, 0, 0, 0, 0, 0},
      {0, 7, 0, 0, 9, 0, 2, 0, 0}, {0, 5, 0, 0, 0, 7, 0, 0, 0},
      {0, 0, 0, 0, 4, 5, 7, 0, 0}, {0, 0, 0, 1, 0, 0, 0, 3, 0},
      {0, 0, 1, 0, 0, 0, 0, 6, 8}, {0, 0, 8, 5, 0, 0, 0, 1, 0},
      {0, 9, 0, 0, 0, 0, 4, 0, 0}};

  int bitRow[SIZE];

  int bitColumn[SIZE];

  int bitBox[SIZE];

  convert_grid_to_bit_row(grid, bitRow);

  convert_grid_to_bit_column(grid, bitColumn);

  convert_grid_to_bit_box(grid, bitBox);

  print_grid(grid);

  // Commencer la mesure du temps
  auto start = high_resolution_clock::now();

  if (place_number(5, 1, 1, 0, bitRow, bitColumn, bitBox)) {
    cout << "Succes" << endl;
  } else {
    cout << "Fail" << endl;
  }

  // Fin de la mesure du temps
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(stop - start);

  cout << "Temps d'exécution pour le brut de force: " << duration.count()
       << " ms" << endl;

  return 0;
}
