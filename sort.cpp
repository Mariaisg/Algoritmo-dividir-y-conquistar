#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <dirent.h>
#include <sstream> 

void bubbleSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    std::vector<int> L(n1);
    std::vector<int> R(n2);
    
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int i = 0; i < n2; i++)
        R[i] = arr[mid + 1 + i];
    
    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    
    while (i < n1) {
        arr[k++] = L[i++];
    }
    
    while (j < n2) {
        arr[k++] = R[j++];
    }
}

void mergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

int test(std::vector<int> arr){
    std::vector<int> arr1;
    std::vector<int> arr2;
    std::vector<int> arr3;
    std::vector<int> arr4;

    arr1 = arr;
    arr2 = arr; 
    arr3 = arr; 
    arr4 = arr; 

    auto start = std::chrono::high_resolution_clock::now();
    bubbleSort(arr1);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_bubble = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "BubbleSort took " << duration_bubble.count() << " milliseconds" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    quickSort(arr2, 0, arr2.size() - 1);
    end = std::chrono::high_resolution_clock::now();
    auto duration_quick = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "QuickSort took " << duration_quick.count() << " milliseconds" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    mergeSort(arr3, 0, arr3.size() - 1);
    end = std::chrono::high_resolution_clock::now();
    auto duration_merge = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "MergeSort took " << duration_merge.count() << " milliseconds" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    std::sort(arr4.begin(), arr4.end());
    end = std::chrono::high_resolution_clock::now();
    auto duration_std_sort = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "std::sort took " << duration_std_sort.count() << " milliseconds" << std::endl;

    return 0;

}

int main() {
    std::vector<std::string> fileNames;     
    std::vector<std::vector<int>> fileContents;    

    DIR *dir;
    struct dirent *entry;


    dir = opendir(".");
    if (dir == nullptr) {
        std::cerr << "Could not open the directory!" << std::endl;
        return 1;
    }

    while ((entry = readdir(dir)) != nullptr) {
        std::string fileName = entry->d_name;

        if (fileName.size() >= 4 && fileName.substr(fileName.size() - 4) == ".txt") {
            fileNames.push_back(fileName);  

            std::ifstream file(fileName);
            if (!file) {
                std::cerr << "Error opening file: " << fileName << std::endl;
                continue;
            }

            std::vector<int> numbers;
            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                int num;
                while (iss >> num) {
                    numbers.push_back(num);  
                }
            }
            fileContents.push_back(numbers);  
            file.close();
        }
    }

    closedir(dir); 

    for (size_t i = 0; i < fileNames.size(); ++i) {
        std::cout << "ARCHIVO: " << fileNames[i] << std::endl;
        test(fileContents[i]);
        std::cout << "\n\n";
    }

    return 0;
}