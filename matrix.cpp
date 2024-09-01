#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <dirent.h>
#include <sstream>  
#include <regex>  

std::vector<std::vector<int>> createMatrix(int n) {
    return std::vector<std::vector<int>>(n, std::vector<int>(n, 0));
}

std::vector<std::vector<int>> iterativeMatrixMultiplication(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {
    int n = A.size();
    std::vector<std::vector<int>> C = createMatrix(n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

std::vector<std::vector<int>> iterativeMatrixMultiplicationWithTranspose(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {
    int n = A.size();
    std::vector<std::vector<int>> C = createMatrix(n);
    std::vector<std::vector<int>> B_T = createMatrix(n);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            B_T[j][i] = B[i][j];
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                C[i][j] += A[i][k] * B_T[j][k];
            }
        }
    }

    return C;
}


std::vector<std::vector<int>> strassenMatrixMultiplication(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {
    int n = A.size();
    if (n == 1) {
        return {{A[0][0] * B[0][0]}};
    }

    int mid = n / 2;
    std::vector<std::vector<int>> A11(mid, std::vector<int>(mid)), A12(mid, std::vector<int>(mid)), A21(mid, std::vector<int>(mid)), A22(mid, std::vector<int>(mid));
    std::vector<std::vector<int>> B11(mid, std::vector<int>(mid)), B12(mid, std::vector<int>(mid)), B21(mid, std::vector<int>(mid)), B22(mid, std::vector<int>(mid));

    for (int i = 0; i < mid; ++i) {
        for (int j = 0; j < mid; ++j) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + mid];
            A21[i][j] = A[i + mid][j];
            A22[i][j] = A[i + mid][j + mid];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + mid];
            B21[i][j] = B[i + mid][j];
            B22[i][j] = B[i + mid][j + mid];
        }
    }

    std::vector<std::vector<int>> M1 = strassenMatrixMultiplication(A11, B11);
    std::vector<std::vector<int>> M2 = strassenMatrixMultiplication(A12, B21);
    std::vector<std::vector<int>> M3 = strassenMatrixMultiplication(A21, B12);
    std::vector<std::vector<int>> M4 = strassenMatrixMultiplication(A22, B22);

    std::vector<std::vector<int>> C = createMatrix(n);
    for (int i = 0; i < mid; ++i) {
        for (int j = 0; j < mid; ++j) {
            C[i][j] = M1[i][j] + M2[i][j];
            C[i][j + mid] = M3[i][j];
            C[i + mid][j] = M4[i][j];
        }
    }
    return C;
}

void timeMatrixMultiplication(std::vector<std::vector<int>> matrix) {

    auto start = std::chrono::high_resolution_clock::now();
    auto C1 = iterativeMatrixMultiplication(matrix, matrix);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_iterative = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Iterative Matrix Multiplication took " << duration_iterative.count() << " milliseconds." << std::endl;

    start = std::chrono::high_resolution_clock::now();
    auto C2 = iterativeMatrixMultiplicationWithTranspose(matrix, matrix);
    end = std::chrono::high_resolution_clock::now();
    auto duration_iterative_transpose = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Iterative Matrix Multiplication with Transpose took " << duration_iterative_transpose.count() << " milliseconds." << std::endl;

    start = std::chrono::high_resolution_clock::now();
    auto C3 = strassenMatrixMultiplication(matrix, matrix);
    end = std::chrono::high_resolution_clock::now();
    auto duration_strassen = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Strassen's Matrix Multiplication took " << duration_strassen.count() << " milliseconds." << std::endl;
}

int main() {
    std::vector<std::string> fileNames;                        
    std::vector<std::vector<std::vector<int>>> matrices;      

    DIR *dir;
    struct dirent *entry;

    dir = opendir(".");
    if (dir == nullptr) {
        std::cerr << "Could not open the directory!" << std::endl;
        return 1;
    }

    std::regex matrixFilePattern("matrix_(\\d+)x(\\d+)\\.txt");

    while ((entry = readdir(dir)) != nullptr) {
        std::string fileName = entry->d_name;
        std::smatch matches;

        if (std::regex_match(fileName, matches, matrixFilePattern)) {
            fileNames.push_back(fileName); 

            int rows = std::stoi(matches[1]);  
            int cols = std::stoi(matches[2]); 

            std::ifstream file(fileName);
            if (!file) {
                std::cerr << "Error opening file: " << fileName << std::endl;
                continue;
            }

            std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));  

            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    if (!(file >> matrix[i][j])) {
                        std::cerr << "Error reading matrix value in file: " << fileName << std::endl;
                        file.close();
                        return 1;
                    }
                }
            }

            matrices.push_back(matrix); 
            file.close();
        }
    }

    closedir(dir); 

    for (size_t i = 0; i < fileNames.size(); ++i) {
        std::cout << "ARCHIVO: " << fileNames[i] << std::endl;
        timeMatrixMultiplication(matrices[i]);
        std::cout << std::endl;
    }

    return 0;
}
