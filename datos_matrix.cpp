#include <iostream>
#include <vector>
#include <fstream>
#include <random>

// Generar una matriz con valores aleatorios
std::vector<std::vector<int>> generateRandomMatrix(int rows, int cols) {
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, 99);
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = distribution(generator);
        }
    }
    return matrix;
}

// Guardar matriz en un archivo
void saveMatrixToFile(const std::vector<std::vector<int>>& matrix, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& row : matrix) {
            for (int value : row) {
                file << value << " ";
            }
            file << std::endl;
        }
    }
}

// Crear datasets para multiplicación de matrices
void createMatrixDatasets() {
    std::vector<std::pair<int, int>> sizes = {{1000, 1000}, {3000, 3000}, {500, 500}, {3000, 5000}};
    
    for (const auto& size : sizes) {
        int rows = size.first;
        int cols = size.second;
        std::vector<std::vector<int>> matrix = generateRandomMatrix(rows, cols);

        saveMatrixToFile(matrix, "matrix_" + std::to_string(rows) + "x" + std::to_string(cols) + ".txt");
    }
}

int main() {
    createMatrixDatasets();
    return 0;
}
