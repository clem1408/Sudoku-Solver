#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
using namespace std::chrono;

class Sudoku {
public:
    explicit Sudoku(const std::string& filename);
    void printGrid() const;
private:
    int _grid_size;  
    std::vector<std::vector<int>> _grid;  
    void loadGridFromFile(const std::string& filename);
    inline void convert_grid_to_bit_row(std::vector<std::vector<int>> _grid, int (&bitRows)[SIZE]);
};

Sudoku::Sudoku(const std::string& filename) {
    loadGridFromFile(filename);
}

void Sudoku::loadGridFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erreur : impossible d'ouvrir le fichier " << filename << std::endl;
        return;
    }

    file >> _grid_size;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Ignore row end

    // Redimensionner le vecteur pour la grille
    _grid.resize(_grid_size, std::vector<int>(_grid_size));

    for (int i = 0; i < _grid_size; ++i) {
        std::string line;
        std::getline(file, line);
        for (int j = 0; j < _grid_size; ++j) {
            _grid[i][j] = line[j] - '0';  // Convert the char in int
        }
    }
}

void Sudoku::printGrid() const {
    for (int i = 0; i < _grid_size; ++i) {
        for (int j = 0; j < _grid_size; ++j) {
            std::cout << _grid[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

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
    Sudoku sudoku("../Sudokus/9x9_easy.txt");
    sudoku.printGrid();
    return 0;
}