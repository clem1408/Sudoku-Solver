#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <limits>
#include <cmath>

using namespace std;
using namespace std::chrono;

class Sudoku {
public:
    explicit Sudoku(const std::string& filename);
    void printGrid() const;
    bool solve();

private:
    int _grid_size;
    int _box_size;
    std::vector<std::vector<int>> _grid;
    std::vector<int> _bitRows, _bitColumns, _bitBoxes;

    void loadGridFromFile(const std::string& filename);
    void convertGridToBitRepresentation();
    bool placeNumber(int number, int row, int column) const;
    bool backtrackingSolver();
};

Sudoku::Sudoku(const std::string& filename) {
    loadGridFromFile(filename);
    _box_size = static_cast<int>(sqrt(_grid_size));
    _bitRows.assign(_grid_size, 0);
    _bitColumns.assign(_grid_size, 0);
    _bitBoxes.assign(_grid_size, 0);
    convertGridToBitRepresentation();
}

void Sudoku::loadGridFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erreur : impossible d'ouvrir le fichier " << filename << std::endl;
        return;
    }

    file >> _grid_size;
    file.ignore(numeric_limits<streamsize>::max(), '\n');

    _grid.assign(_grid_size, std::vector<int>(_grid_size));
    for (int i = 0; i < _grid_size; ++i) {
        std::string line;
        std::getline(file, line);  
        std::istringstream line_stream(line); 
        for (int j = 0; j < _grid_size; ++j) {
            line_stream >> _grid[i][j];
        }
    }
}

void Sudoku::printGrid() const {
    for (int i = 0; i < _grid_size; ++i) {
        if (i > 0 && i % _box_size == 0) {
            cout << string((_grid_size * 2) + (_box_size - 1), '-') << endl;
        }
        for (int j = 0; j < _grid_size; ++j) {
            if (j > 0 && j % _box_size == 0) {
                cout << "| ";
            }
            cout << _grid[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void Sudoku::convertGridToBitRepresentation() {
    for (int row = 0; row < _grid_size; ++row) {
        for (int col = 0; col < _grid_size; ++col) {
            int num = _grid[row][col];
            if (num > 0) {
                int bit = 1 << (num - 1);
                _bitRows[row] |= bit;
                _bitColumns[col] |= bit;
                _bitBoxes[(row / _box_size) * _box_size + (col / _box_size)] |= bit;
            }
        }
    }
}

inline bool Sudoku::placeNumber(int number, int row, int column) const {
    int bit = 1 << (number - 1);
    int box = (row / _box_size) * _box_size + (column / _box_size);
    return ((_bitRows[row] & bit) == 0) &&
           ((_bitColumns[column] & bit) == 0) &&
           ((_bitBoxes[box] & bit) == 0);
}

bool Sudoku::backtrackingSolver() {
    int row = -1, col = -1;
    bool isEmpty = false;

    // Trouver une cellule vide en suivant une heuristique simple
    for (int i = 0; i < _grid_size && !isEmpty; ++i) {
        for (int j = 0; j < _grid_size; ++j) {
            if (_grid[i][j] == 0) {
                row = i;
                col = j;
                isEmpty = true;
                break;
            }
        }
    }

    if (!isEmpty) return true;

    int box = (row / _box_size) * _box_size + (col / _box_size);
    for (int num = 1; num <= _grid_size; ++num) {
        if (placeNumber(num, row, col)) {
            int bit = 1 << (num - 1);
            _grid[row][col] = num;
            _bitRows[row] |= bit;
            _bitColumns[col] |= bit;
            _bitBoxes[box] |= bit;

            if (backtrackingSolver()) return true;

            _grid[row][col] = 0;
            _bitRows[row] &= ~bit;
            _bitColumns[col] &= ~bit;
            _bitBoxes[box] &= ~bit;
        }
    }
    return false;
}

bool Sudoku::solve() {
    auto start = high_resolution_clock::now();
    bool solved = backtrackingSolver();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);

    cout << "Execution time for backtracking algorithm : " << duration.count() << " ns" << endl << endl;
    return solved;
}

int main() {
    vector<string> files = {
        "../Sudokus/9x9_easy.txt",
        "../Sudokus/9x9_hard.txt",
        "../Sudokus/9x9_demon.txt"
    };
    
    for (const auto& file : files) {
        Sudoku sudoku(file);
        sudoku.printGrid();
        if (!sudoku.solve()) 
            cout << "Aucune solution n'existe." << endl;
        else
            sudoku.printGrid();
    }
    return 0;
}
