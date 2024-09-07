#include <iostream>
#include <vector>
#include <fstream>
#include <random>

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

void createMatrixDatasets() {
    std::vector<std::pair<int, int>> sizes = {{500, 500}, {200,400}, {300,500}, {100,100},{1000,1000}};
    
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
