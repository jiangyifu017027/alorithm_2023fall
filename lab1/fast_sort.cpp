#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>

using namespace std;

// 插入排序
void insertionSort(std::vector<int>& arr, int low, int high) {
    for (int i = low + 1; i <= high; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= low && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

// 快速排序
void quickSort(std::vector<int>& arr, int low, int high, int threshold) {
    if (low < high) {
        if (high - low + 1 <= threshold) {
            insertionSort(arr, low, high);
        } else {
            int pivot = arr[low]; // 使用第一个元素作为基准
            int i = low + 1;
            int j = high;

            while (i <= j) {
                if (arr[i] <= pivot) {
                    i++;
                } else if (arr[j] > pivot) {
                    j--;
                } else {
                    std::swap(arr[i], arr[j]);
                }
            }

            swap(arr[low], arr[j]);

            quickSort(arr, low, j - 1, threshold);
            quickSort(arr, j + 1, high, threshold);
        }
    }
}

// 以固定基准的方式实现快速排序
void quickSortFixedPivot(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pivot = arr[low]; // 使用第一个元素作为基准
        int i = low + 1;
        int j = high;

        while (i <= j) {
            if (arr[i] <= pivot) {
                i++;
            } else if (arr[j] > pivot) {
                j--;
            } else {
                std::swap(arr[i], arr[j]);
            }
        }

        std::swap(arr[low], arr[j]);

        quickSortFixedPivot(arr, low, j - 1);
        quickSortFixedPivot(arr, j + 1, high);
    }
}

// 以随机基准的方式实现快速排序
void quickSortRandomPivot(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(low, high);
        int pivotIndex = dis(gen);
        
        std::swap(arr[low], arr[pivotIndex]);
        int pivot = arr[low];
        int i = low + 1;
        int j = high;

        while (i <= j) {
            if (arr[i] <= pivot) {
                i++;
            } else if (arr[j] > pivot) {
                j--;
            } else {
                std::swap(arr[i], arr[j]);
            }
        }

        std::swap(arr[low], arr[j]);

        quickSortRandomPivot(arr, low, j - 1);
        quickSortRandomPivot(arr, j + 1, high);
    }
}

// 以三数取中的方式实现快速排序
void quickSortMedianOfThree(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int mid = low + (high - low) / 2;
        if (arr[low] > arr[mid]) {
            std::swap(arr[low], arr[mid]);
        }
        if (arr[low] > arr[high]) {
            std::swap(arr[low], arr[high]);
        }
        if (arr[mid] > arr[high]) {
            std::swap(arr[mid], arr[high]);
        }
        int pivot = arr[mid]; // 中间值作为基准

        int i = low + 1;
        int j = high;

        while (i <= j) {
            if (arr[i] <= pivot) {
                i++;
            } else if (arr[j] > pivot) {
                j--;
            } else {
                std::swap(arr[i], arr[j]);
            }
        }

        std::swap(arr[mid], arr[j]);

        quickSortMedianOfThree(arr, low, j - 1);
        quickSortMedianOfThree(arr, j + 1, high);
    }
}


int main() {
    // 获取开始时间点
    auto start = std::chrono::high_resolution_clock::now();

    ifstream inputFile("data.txt");
    if (!inputFile) {
        cout << "Failed to open the file." << endl;
        return 0;
    }

    int numCount;
    inputFile >> numCount;

    vector<int> nums(numCount);
    for (int i = 0; i < numCount; ++i) {
        inputFile >> nums[i];
    }

    inputFile.close();

    // 以固定基准的方式实现快速排序
    // quickSortFixedPivot(nums, 0, numCount - 1);
    // 以随机基准的方式实现快速排序
    // quickSortRandomPivot(nums, 0, numCount - 1);
    // 以三数取中的方式实现快速排序
    // quickSortMedianOfThree(nums, 0, numCount - 1);
    // 采用插入排序优化后的快速排序
    // int threshold = sqrt(nums.size());
    // quickSort(nums, 0, nums.size() - 1, threshold);
    //标准排序
    sort(nums.begin(), nums.end());

    ofstream outputFile("sorted.txt");
    if (!outputFile) {
        cout << "Failed to create the file." << endl;
        return 0;
    }

    // 将排序结果写入文件
    for (int i = 0; i < numCount; ++i) {
        outputFile << nums[i] << " ";
    }
    outputFile.close();

    cout << "Sorting completed. The sorted numbers are written to 'sorted.txt'." << endl;

    // 获取结束时间点
    auto end = std::chrono::high_resolution_clock::now();

    // 计算执行时间
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // 输出执行时间（以毫秒为单位）
    std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;
    
    return 0;
}