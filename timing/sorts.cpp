//
// Created by Richard Buckley on 7/23/23.
//

#include "sorts.h"

void Sorts::insertionSort(std::vector<int> &arr) {
    // Define temporary variables
    int j, k;

    // Loop through the array
    for (int i = 1 ; i < arr.size() ; i++) {
        // Update Values, Define temp (k)
        j = i, k = arr[i];

        // move element to correct position
        while (j > 0 and k < arr[j - 1]) {
            arr[j] = arr[j - 1];
            j--;
        }

        // restore temp
        arr[j] = k;
    }
}

void Sorts::mergeSortHelper(std::vector<int> &arr, int left, int mid, int right) {
    int i = left;       // Index for left subarray
    int j = mid + 1;    // Index for right subarray
    int k = 0;          // Index for temporary array

    // Define a temporary array
    std::vector<int> temp(right - left + 1);

    // Merge the two sub-arrays into the temporary array
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j])
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }

    // Copy the remaining elements of the left subarray, if any
    while (i <= mid)
        temp[k++] = arr[i++];

    // Copy the remaining elements of the right subarray, if any
    while (j <= right)
        temp[k++] = arr[j++];

    // Copy the merged elements back to the original array
    for (i = left, k = 0; i <= right; i++, k++)
        arr[i] = temp[k];
}

void Sorts::mergeSort(std::vector<int> &arr) {
    return mergeSort(arr, 0, arr.size() - 1);
}

void Sorts::mergeSort(std::vector<int> &arr, int left, int right) {
    if (left < right) {
        // Calculate the mid-point
        int mid = left + (right - left) / 2;

        // Recursively divide the array into two halves
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        // Merge the sorted halves
        mergeSortHelper(arr, left, mid, right);
    }
}


void Sorts::radixSort(std::vector<int> &arr, int maxDigits) {

    for (int i = 0 ; i < maxDigits ; i++) {

        // create empty containers that we will organize into
        std::vector< std::vector<int> > containers(10);

        // for num in arr
        for (int j = 0 ; i < arr.size() ; i++)
            containers[(arr[j] / (int)std::pow(10, i)) % 10].push_back(arr[j]);

        // clear out arr
        arr.clear();

        // Rinse & Repeat
        for (int j = 0 ; j < containers.size() ; j++)
            arr.insert(arr.end(), containers[j].begin(), containers[j].end());
    }

}

void Sorts::radixSort(std::vector<int> &arr) {

    // Iterate through array and find maximum value
    int maxVal = arr[0];
    for (int i = 1 ; i < arr.size() ; i++) {
        if (arr[i] > maxVal)
            maxVal = arr[i];
    }

    // go to radix-sort implementation with a known maximum value of digits
    return radixSort(arr, ceil(log10(maxVal)));
}

void Sorts::quickSort(std::vector<int> &arr) {
    std::random_device rd;
    std::mt19937 random(rd());

    std::shuffle(arr.begin(), arr.end(), random);
    return quickSort(arr, 0, arr.size() - 1);
}

void Sorts::quickSort(std::vector<int> &arr, int left, int right) {
    if (right <= left)
        return;

    int pivot = quickSortHelper(arr, left, right);

    quickSort(arr, left, pivot - 1);
    quickSort(arr, pivot + 1, right);
}

int Sorts::quickSortHelper(std::vector<int> &arr, int left, int right) {
    int i = left;
    int j = right + 1;

    while (true) {

        // while quick[i] < pivot, increase i
        while (arr[++i] < arr[left])
            if (i == right) break;

        // while quick[i] > pivot, decrease j
        while (arr[left] < arr[--j])
            if (j == left) break;

        // if i and j cross exit the loop
        if(i >= j) break;

        // swap quick[i], quick[j]
        std::swap(arr[i], arr[j]);

    }

    // swap the pivot with quick[j]
    std::swap(arr[left], arr[j]);

    // return pivot's position
    return j;
}
