#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>

// Generar un vector con valores aleatorios
std::vector<int> generateRandomVector(int size) {
    std::vector<int> vec(size);
    std::generate(vec.begin(), vec.end(), rand);
    return vec;
}

// Crear datasets
void createSortDatasets() {
    std::vector<int> sizes = {5000, 10000, 25000, 50000};
    
    for (int size : sizes) {
        // Desordenado
        std::vector<int> unordered = generateRandomVector(size);
        std::ofstream file("desordenado_" + std::to_string(size) + ".txt");
        for (int num : unordered) file << num << " ";
        file.close();
        
        // Parcialmente ordenado
        std::vector<int> partiallyOrdered = generateRandomVector(size);
        std::partial_sort(partiallyOrdered.begin(), partiallyOrdered.begin() + size / 2, partiallyOrdered.end());
        file.open("parcialmente_ordenado_" + std::to_string(size) + ".txt");
        for (int num : partiallyOrdered) file << num << " ";
        file.close();
        
        // Ordenado
        std::vector<int> ordered(size);
        std::iota(ordered.begin(), ordered.end(), 0);
        file.open("ordenado_" + std::to_string(size) + ".txt");
        for (int num : ordered) file << num << " ";
        file.close();
        
        // Peor caso
        std::vector<int> worstCase(size);
        std::iota(worstCase.rbegin(), worstCase.rend(), 1);
        file.open("Peor_caso_" + std::to_string(size) + ".txt");
        for (int num : worstCase) file << num << " ";
        file.close();
    }
}

int main() {
    createSortDatasets();
    return 0;
}
