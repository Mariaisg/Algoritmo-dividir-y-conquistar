#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <dirent.h>
#include <sstream>  
#include <regex>  

typedef std::vector<std::vector<int>> matriz;



matriz multiplicarDirecta(const matriz& A, const matriz& B) {
    int n = A.size();
    int m = B[0].size();
    int p = B.size();
    matriz C(n, std::vector<int>(m, 0));
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            for (int k = 0; k < p; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

matriz multiplicarTranspuesta(const matriz& A, const matriz& B) {
    int n = A.size();
    int m = B[0].size();
    int p = B.size();
    matriz C(n, std::vector<int>(m, 0));
    matriz B_T(m, std::vector<int>(p, 0));

    for (int i = 0; i < p; ++i) {
        for (int j = 0; j < m; ++j) {
            B_T[j][i] = B[i][j];
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            for (int k = 0; k < p; ++k) {
                C[i][j] += A[i][k] * B_T[j][k];
            }
        }
    }
    return C;
}

matriz sumar(const matriz& A, const matriz& B) {
    int n = A.size();
    matriz C(n, std::vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}

matriz restar(const matriz& A, const matriz& B) {
    int n = A.size();
    matriz C(n, std::vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    return C;
}

matriz strassen(const matriz& A, const matriz& B) {
    int n = A.size();
    if (n == 1) {
        return {{A[0][0] * B[0][0]}};
    }

    int newSize = n / 2;
    matriz A11(newSize, std::vector<int>(newSize, 0));
    matriz A12(newSize, std::vector<int>(newSize, 0));
    matriz A21(newSize, std::vector<int>(newSize, 0));
    matriz A22(newSize, std::vector<int>(newSize, 0));

    matriz B11(newSize, std::vector<int>(newSize, 0));
    matriz B12(newSize, std::vector<int>(newSize, 0));
    matriz B21(newSize, std::vector<int>(newSize, 0));
    matriz B22(newSize, std::vector<int>(newSize, 0));

    for (int i = 0; i < newSize; ++i) {
        for (int j = 0; j < newSize; ++j) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + newSize];
            A21[i][j] = A[i + newSize][j];
            A22[i][j] = A[i + newSize][j + newSize];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + newSize];
            B21[i][j] = B[i + newSize][j];
            B22[i][j] = B[i + newSize][j + newSize];
        }
    }

    matriz M1 = strassen(sumar(A11, A22), sumar(B11, B22));
    matriz M2 = strassen(sumar(A21, A22), B11);
    matriz M3 = strassen(A11, restar(B12, B22));
    matriz M4 = strassen(A22, restar(B21, B11));
    matriz M5 = strassen(sumar(A11, A12), B22);
    matriz M6 = strassen(restar(A21, A11), sumar(B11, B12));
    matriz M7 = strassen(restar(A12, A22), sumar(B21, B22));

    matriz C11 = sumar(restar(sumar(M1, M4), M5), M7);
    matriz C12 = sumar(M3, M5);
    matriz C21 = sumar(M2, M4);
    matriz C22 = sumar(restar(sumar(M1, M3), M2), M6);

    matriz C(n, std::vector<int>(n, 0));
    for (int i = 0; i < newSize; ++i) {
        for (int j = 0; j < newSize; ++j) {
            C[i][j] = C11[i][j];
            C[i][j + newSize] = C12[i][j];
            C[i + newSize][j] = C21[i][j];
            C[i + newSize][j + newSize] = C22[i][j];
        }
    }
    return C;
}

matriz padding(const matriz& matrix, int newSize) {
    int originalRows = matrix.size();
    int originalCols = matrix[0].size();
    matriz padded(newSize, std::vector<int>(newSize, 0));
    for (int i = 0; i < originalRows; ++i) {
        for (int j = 0; j < originalCols; ++j) {
            padded[i][j] = matrix[i][j];
        }
    }
    return padded;
}

matriz strassenConPadding(const matriz& A, const matriz& B) {
    int maxDim = std::max({A.size(), A[0].size(), B.size(), B[0].size()});
    int newSize = 1;
    while (newSize < maxDim) {
        newSize <<= 1;
    }

    matriz paddedA = padding(A, newSize);
    matriz paddedB = padding(B, newSize);
    matriz paddedC = strassen(paddedA, paddedB);

    matriz C(A.size(), std::vector<int>(B[0].size(), 0));
    for (int i = 0; i < C.size(); ++i) {
        for (int j = 0; j < C[0].size(); ++j) {
            C[i][j] = paddedC[i][j];
        }
    }
    return C;
}

void cronometrar(matriz matrix) {
    int m = matrix[0].size();
    int p = matrix.size();
    matriz matrix_T(m, std::vector<int>(p, 0));

    for (int i = 0; i < p; ++i) {
        for (int j = 0; j < m; ++j) {
            matrix_T[j][i] = matrix[i][j];
        }
    }

    auto start = std::chrono::high_resolution_clock::now();
    auto C1 = multiplicarDirecta(matrix, matrix_T);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_iterative = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Iterative Matrix Multiplication took " << duration_iterative.count() << " milliseconds." << std::endl;

    start = std::chrono::high_resolution_clock::now();
    auto C2 = multiplicarTranspuesta(matrix, matrix_T);
    end = std::chrono::high_resolution_clock::now();
    auto duration_iterative_transpose = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Iterative Matrix Multiplication with Transpose took " << duration_iterative_transpose.count() << " milliseconds." << std::endl;

    start = std::chrono::high_resolution_clock::now();
    auto C3 = strassenConPadding(matrix, matrix_T);
    end = std::chrono::high_resolution_clock::now();
    auto duration_strassen = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Strassen's Matrix Multiplication took " << duration_strassen.count() << " milliseconds." << std::endl;
}

int main() {
    std::vector<std::string> fileNames; 
    std::vector<matriz> matrices; 

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

            matriz matrix(rows, std::vector<int>(cols)); 
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
        cronometrar(matrices[i]);
        std::cout << std::endl;
    }

    return 0;
}
