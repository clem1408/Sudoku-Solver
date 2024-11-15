#include <chrono>
#include <iostream>
#include <vector>

#define N 3
#define SIZE N * N

using namespace std;
using namespace std::chrono;


/**
 * Structure pour contenir les masques de bits pour les lignes, colonnes et boîtes de la grille
 */
struct BitMasks {
    int rows[SIZE]{};
    int columns[SIZE]{};
    int boxes[SIZE]{};
};

/**
 * Convertit une grille Sudoku en représentations "bitmasks" pour les lignes, colonnes et boîtes.
 * Chaque cellule occupée par un chiffre est marquée par un bit correspondant dans les masques.
 *
 * @param grid Grille Sudoku représentée par une matrice 2D d'entiers.
 * @param masks Structure BitMasks pour stocker les masques de bits des lignes, colonnes et boîtes.
 */
inline void convert_grid_to_bits(const int grid[SIZE][SIZE], BitMasks& masks) {
    for (int row = 0; row < SIZE; ++row) {
        for (int col = 0; col < SIZE; ++col) {
            int num = grid[row][col];
            if (num > 0) {
                int bit = 1 << (num - 1);
                masks.rows[row] |= bit;
                masks.columns[col] |= bit;
                masks.boxes[(row / N) * N + (col / N)] |= bit;
            }
        }
    }
}

/**
 * Affiche la grille Sudoku dans la console.
 *
 * @param grid Grille Sudoku représentée par une matrice 2D d'entiers.
 */
inline void print_grid(const int grid[SIZE][SIZE]) {
    for (int row = 0; row < SIZE; ++row) {
        for (int col = 0; col < SIZE; ++col) {
            cout << grid[row][col] << " ";
        }
        cout << endl;
    }
}

/**
 * Vérifie si un numéro peut être placé dans une position spécifique (ligne, colonne, boîte) 
 * sans enfreindre les règles de Sudoku.
 *
 * @param num Le chiffre à placer dans la grille (de 1 à SIZE).
 * @param row L'index de la ligne.
 * @param col L'index de la colonne.
 * @param box L'index de la boîte (bloc 3x3).
 * @param masks Structure BitMasks contenant les masques de bits pour les lignes, colonnes et boîtes.
 * @return True si le chiffre peut être placé sans conflit, False sinon.
 */
inline bool is_placeable(int num, int row, int col, int box, const BitMasks& masks) {
    int bit = 1 << (num - 1);
    return !(masks.rows[row] & bit) && !(masks.columns[col] & bit) && !(masks.boxes[box] & bit);
}

/**
 * Algorithme récursif de résolution du Sudoku en utilisant le backtracking.
 * Il tente de placer chaque chiffre de manière valide dans les cellules vides et
 * retourne true si une solution est trouvée.
 *
 * @param grid Grille Sudoku représentée par une matrice 2D d'entiers.
 * @param masks Structure BitMasks contenant les masques de bits pour les lignes, colonnes et boîtes.
 * @param emptyCells Vecteur de paires d'indices (ligne, colonne) représentant les cellules vides dans la grille.
 * @param idx Index de la cellule vide actuellement en cours de traitement dans le vecteur emptyCells.
 * @return True si la grille est résolue avec succès, False sinon.
 */
bool backtracking_solver(int grid[SIZE][SIZE], BitMasks& masks, vector<pair<int, int>>& emptyCells, int idx = 0) {
    if (idx == (int)emptyCells.size()) return true;

    int row = emptyCells[idx].first;
    int col = emptyCells[idx].second;
    int box = (row / N) * N + (col / N);

    for (int num = 1; num <= SIZE; ++num) {
        if (is_placeable(num, row, col, box, masks)) {
            int bit = 1 << (num - 1);
            grid[row][col] = num;
            masks.rows[row] |= bit;
            masks.columns[col] |= bit;
            masks.boxes[box] |= bit;

            if (backtracking_solver(grid, masks, emptyCells, idx + 1)) return true;

            // Backtrack
            grid[row][col] = 0;
            masks.rows[row] &= ~bit;
            masks.columns[col] &= ~bit;
            masks.boxes[box] &= ~bit;
        }
    }
    return false;
}

/**
 * Fonction pour initialiser les masques de bits et résoudre le Sudoku en affichant la solution et le temps d'exécution.
 *
 * @param grid Grille Sudoku représentée par une matrice 2D d'entiers.
 */
void solve_sudoku(int grid[SIZE][SIZE]) {
    BitMasks masks;
    convert_grid_to_bits(grid, masks);

    vector<pair<int, int>> emptyCells;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (grid[i][j] == 0) emptyCells.emplace_back(i, j);
        }
    }

    // Measure time
    auto start = high_resolution_clock::now();

    if (backtracking_solver(grid, masks, emptyCells)) {
        cout << "Solution found:" << endl;
        print_grid(grid);
    } else {
        cout << "No solution exists!" << endl;
    }

    // End of time measure
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    cout << "Execution time: " << duration.count() << " ns" << endl << endl;
}

int main() {
    int grid1[SIZE][SIZE] = {
        {0, 0, 0, 0, 0, 0, 6, 8, 0}, {0, 0, 0, 0, 7, 3, 0, 0, 9},
        {3, 0, 9, 0, 0, 0, 0, 4, 5}, {4, 9, 0, 0, 0, 0, 0, 0, 0},
        {8, 0, 3, 0, 5, 0, 9, 0, 2}, {0, 0, 0, 0, 0, 0, 0, 3, 6},
        {9, 6, 0, 0, 0, 0, 3, 0, 8}, {7, 0, 0, 6, 8, 0, 0, 0, 0},
        {0, 2, 8, 0, 0, 0, 0, 0, 0}
    };

    int grid2[SIZE][SIZE] = {
        {0, 0, 5, 3, 0, 0, 0, 0, 0}, {8, 0, 0, 0, 0, 0, 0, 2, 0},
        {0, 7, 0, 0, 1, 0, 5, 0, 0}, {4, 0, 0, 0, 0, 5, 3, 0, 0},
        {0, 1, 0, 0, 7, 0, 0, 0, 6}, {0, 0, 3, 2, 0, 0, 0, 8, 0},
        {0, 6, 0, 5, 0, 0, 0, 0, 9}, {0, 0, 4, 0, 0, 0, 0, 3, 0},
        {0, 0, 0, 0, 0, 9, 7, 0, 0}
    };

    int grid3[SIZE][SIZE] = {
        {8, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 3, 6, 0, 0, 0, 0, 0},
        {0, 7, 0, 0, 9, 0, 2, 0, 0}, {0, 5, 0, 0, 0, 7, 0, 0, 0},
        {0, 0, 0, 0, 4, 5, 7, 0, 0}, {0, 0, 0, 1, 0, 0, 0, 3, 0},
        {0, 0, 1, 0, 0, 0, 0, 6, 8}, {0, 0, 8, 5, 0, 0, 0, 1, 0},
        {0, 9, 0, 0, 0, 0, 4, 0, 0}
    };

    cout << "Solving grid 1..." << endl;
    solve_sudoku(grid1);

    cout << "Solving grid 2..." << endl;
    solve_sudoku(grid2);

    cout << "Solving grid 3..." << endl;
    solve_sudoku(grid3);

    /*
    // 16x16
    int grid4[SIZE][SIZE] = {{12, 14, 4, 5, 0, 0, 1, 0, 0, 0, 10, 2, 0, 3, 11, 0},
      {0, 0, 10, 1, 2, 0, 5, 14, 0, 0, 0, 9, 0, 7, 4, 16},
      {0, 0, 0, 0, 0, 0, 15, 0, 11, 14, 0, 0, 2, 0, 0, 0},
      {0, 0, 11, 13, 8, 16, 0, 0, 6, 4, 0, 0, 9, 0, 5, 14},

      {0, 16, 0, 0, 0, 13, 0, 0, 0, 0, 7, 0, 14, 0, 0, 9},
      {13, 0, 0, 0, 0, 0, 2, 0, 0, 0, 14, 0, 15, 0, 0, 0},
      {8, 5, 14, 0, 0, 9, 0, 12, 0, 13, 0, 15, 0, 10, 1, 0},
      {0, 0, 0, 15, 16, 0, 7, 3, 12, 9, 0, 6, 11, 0, 13, 0},

      {0, 13, 0, 16, 7, 0, 6, 2, 4, 12, 0, 8, 1, 0, 0, 0},
      {0, 1, 9, 0, 11, 0, 16, 0, 13, 0, 2, 0, 0, 14, 10, 4},
      {0, 0, 0, 6, 0, 4, 0, 0, 0, 15, 0, 0, 0, 0, 0, 7},
      {10, 0, 0, 4, 0, 3, 0, 0, 0, 0, 11, 0, 0, 0, 15, 0},

      {3, 15, 0, 10, 0, 0, 13, 16, 0, 0, 9, 11, 6, 8, 0, 0},
      {0, 0, 0, 2, 0, 0, 11, 7, 0, 1, 0, 0, 0, 0, 0, 0},
      {6, 9, 13, 0, 3, 0, 0, 0, 15, 8, 0, 10, 4, 11, 0, 0},
      {0, 12, 1, 0, 15, 8, 0, 0, 0, 16, 0, 0, 5, 9, 3, 0}};

    cout << "Solving grid 4..." << endl;
    solve_sudoku(grid4);

    int grid5[SIZE][SIZE] = {
      {21,  0,  0,  0,  0,  0, 16,  0,  0,  0,  0,  0,  3, 13,  0,  0,  0,  0,  8,  1, 11, 14,  0,  9,  0},
      { 2,  0,  7,  9,  0,  0,  0, 23,  0,  0,  0,  4, 21,  1, 10, 25,  6,  0,  0,  0,  0,  0,  0,  0, 13},
      { 0,  0,  0, 16, 11, 17,  0,  1,  0,  4, 23,  0,  0,  0,  0,  0,  0, 24, 12,  8,  0,  2,  0,  0,  0},
      { 0,  0,  0,  0,  0,  0, 11,  5,  0,  0,  0,  9,  0,  7,  2,  0,  0,  0,  3, 20, 15,  1,  0, 25,  0},
      { 0,  0, 19, 14,  0,  8, 13,  0,  7,  0, 24,  0, 25,  0, 22, 10,  0, 23,  0,  0,  4,  5,  0,  6, 16},
      { 0,  9,  6,  0, 25,  0, 17,  0, 11, 10,  0,  0,  2,  0,  0,  0,  0,  0, 16,  0, 23,  0,  0,  0,  5},
      { 0,  0, 14,  0, 12,  2,  0,  0,  0, 23,  0, 11,  0,  0,  1,  0,  0,  0,  0, 10, 19, 20,  0,  0,  0},
      {22, 19, 23, 13, 24,  0,  0,  0,  9,  0,  0,  0,  0,  0, 21, 15, 18,  0,  0,  0,  0,  0,  0,  0,  8},
      { 0, 18,  0,  0, 20, 21,  0,  0,  6, 25,  9,  0, 22,  0,  0,  0,  0, 11,  2,  0,  0,  0,  0,  0,  0},
      {10,  0, 15,  2,  0,  0,  0, 19, 24, 12, 17,  6,  0,  0,  0,  0, 22,  0,  0,  0, 18,  0, 14,  0,  0},
      {11,  0,  0,  0, 15, 12, 24, 10, 18,  0, 25,  3,  0,  0,  0,  4,  0, 19,  0,  2,  0,  0, 23,  0,  0},
      { 0, 21, 12,  1,  3,  6,  0,  9,  0,  0,  0, 15,  0,  2,  0,  0,  0, 22, 11,  0,  0,  0,  0,  0,  0},
      { 4, 14,  0,  0,  0,  0,  0,  0,  0,  0, 19,  0,  0,  0,  0,  8,  7, 17, 13,  0, 20,  0,  0, 24, 22},
      { 0,  0, 10,  0,  0, 19,  8, 15, 13, 16,  0,  7,  0,  0,  0,  0, 12, 18,  0, 25,  5,  0,  0,  2,  1},
      {23,  7,  9,  0, 19,  0,  0,  0,  0, 14, 13,  0,  0,  0, 11, 20,  0,  1,  0,  0,  0, 18,  0,  0, 17},
      { 0,  0,  0, 17, 18,  0,  0,  0, 25,  0,  0,  0, 23,  0,  0, 24,  5, 13, 22,  9,  0,  0,  0,  0,  6},
      { 0,  0,  4,  0,  0, 24,  0,  0,  0,  0,  0, 22,  0,  0,  0,  3,  0,  8, 17, 18,  1,  0,  7, 11, 12},
      { 0,  0,  0,  0,  1,  0,  0,  0, 14,  0, 20,  0,  0,  0, 25,  0,  0, 12,  0,  0, 16, 13,  0,  5,  0},
      {24,  0, 11,  0,  6,  0, 12,  0,  0,  0,  0,  0,  4, 10, 13,  0,  0,  0,  0,  0,  0,  2,  0, 15,  9},
      { 0,  0, 13, 20, 21, 10,  0,  0,  4,  9, 14,  5,  7,  0,  0,  1,  0, 16,  0,  6,  3,  0, 25,  0, 18},
      {15, 16,  0,  0,  0,  0,  0,  0,  0,  2,  0, 13,  0,  0,  5, 17, 11,  0,  6,  8,  7,  0,  0,  0,  4},
      { 0,  0,  0,  5,  0, 14,  0, 21, 19, 15,  0,  0, 24,  4,  0,  0,  0,  3,  1,  0,  0, 22,  0, 16,  0},
      { 0, 22,  1,  6,  0,  0,  3, 13,  0, 24, 10, 23,  0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  9, 17,  0},
      { 0,  2,  0,  0,  7, 22,  4, 20,  0, 11,  0,  0,  0,  9, 18,  0,  0, 25,  0,  0, 14,  0,  3,  8,  0},
      { 8,  0,  0,  0,  0,  0,  0,  7,  1,  0,  0,  0,  0,  0, 12,  0, 23,  0, 14,  0, 24, 10,  0,  0, 20}
    };

    cout << "Solving grid 5..." << endl;
    solve_sudoku(grid5);*/

    int grid6[SIZE][SIZE] = {
        {0, 4, 8, 0, 0, 6, 0, 0, 2}, {0, 0, 2, 9, 0, 0, 4, 1, 7},
        {0, 0, 0, 0, 0, 1, 0, 0, 0}, {0, 0, 0, 3, 0, 7, 9, 4, 0},
        {6, 0, 0, 0, 9, 0, 0, 0, 5}, {4, 0, 9, 0, 6, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 2, 0}, {0, 0, 0, 8, 0, 3, 1, 0, 0},
        {0, 7, 0, 0, 0, 0, 5, 8, 0}
    };

    int grid7[SIZE][SIZE] = {
        {0, 0, 0, 9, 0, 8, 0, 0, 0}, {0, 0, 3, 0, 0, 0, 0, 0, 0},
        {0, 6, 0, 0, 0, 0, 5, 1, 0}, {0, 4, 0, 0, 0, 1, 0, 9, 0},
        {0, 0, 5, 3, 9, 0, 0, 7, 0}, {0, 2, 0, 0, 0, 4, 0, 6, 0},
        {0, 1, 0, 0, 0, 0, 8, 3, 0}, {0, 0, 9, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 2, 0, 5, 0, 0, 0}
    };

    int grid8[SIZE][SIZE] = {
        {8, 0, 0, 0, 0, 0, 1, 3, 0}, {0, 0, 6, 2, 3, 0, 0, 0, 0},
        {0, 2, 0, 0, 0, 1, 0, 0, 0}, {0, 8, 0, 7, 0, 0, 0, 0, 6},
        {0, 0, 4, 0, 9, 0, 8, 0, 0}, {9, 0, 0, 0, 0, 5, 0, 4, 0},
        {0, 0, 0, 4, 0, 0, 0, 9, 0}, {0, 0, 0, 0, 6, 8, 7, 0, 0},
        {0, 3, 5, 0, 0, 0, 0, 0, 8}
    };

    cout << "Solving grid 6..." << endl;
    solve_sudoku(grid6);

    cout << "Solving grid 7..." << endl;
    solve_sudoku(grid7);

    cout << "Solving grid 8..." << endl;
    solve_sudoku(grid8);

    return 0;
}
