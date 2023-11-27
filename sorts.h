#include <algorithm>
#include <iostream>
#include <vector>
#include <random>
#include <cmath>


class Sorts {

private:
    // Helper Functions
    static void mergeSortHelper(std::vector<int> &arr, int left, int mid, int right);
    static int quickSortHelper(std::vector<int> &arr, int left, int right);
    static void quickSort(std::vector<int> &arr, int left, int right);

public:
    // Insertion Sort
    static void insertionSort(std::vector<int> &arr);

    // Merge Sort
    static void mergeSort(std::vector<int> &arr);
    static void mergeSort(std::vector<int> &arr, int left, int right);

    // QuickSort
    static void quickSort(std::vector<int> &arr);

    // radix sort methods
    static void radixSort(std::vector<int> &arr);
    static void radixSort(std::vector<int> &arr, int maxDigits);

};
