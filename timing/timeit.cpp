//
// Created by Richard Buckley on 7/22/23.
//
// Although the code being used here regarding chrono is relatively straightforward
// we found it on stack overflow.
// https://stackoverflow.com/questions/22387586/measuring-execution-time-of-a-function-in-c
//
// The problem isn't that we couldn't figure out how to do it -- we just wanted to know
// the most accurate, the best way of doing it. We figured that we should look to those
// more knowledgeable than ourselves.
//

#include <chrono>
#include <iostream>
#include <fstream>
#include "sorts.h"

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::steady_clock;
using std::chrono::time_point;
using std::chrono::duration;

#define TIME_UNIT std::chrono::milliseconds

/**
 * This function will get an unsorted array from a temporary file. This result
 * will then a vector of integers that were fetched from the file
 * @param file_name
 * @return
 */
std::vector<int> getUnsortedArr(const std::string &file_name) {
    // Open the file
    std::ifstream file(file_name);

    // Check if the file exists
    if (!file.is_open()) {
        std::cout << "File could not be found, exiting..." << std::endl;
        std::exit(1);
    }

    // Read the File
    std::vector<int> array;

    // Temporary variable to get input
    int temp;

    // Parse input from file
    while (file >> temp)
        array.push_back(temp);

    // Return the input
    return array;
}


/**
 * This function will output the results of the sorting algorithm being ran to an output file, provided
 * as a parameter. The output file will be in the following format
 *
 * --- BEGIN FILE --−
 * {...previous text}
 * {SORTED_ARRAY}
 * {...times for sort}
 * --- END FILE ---
 *
 * @param array
 * @param times
 * @param outputFileName
 */
void outputResults(std::vector<int> array, std::vector<int> times, std::string outputFileName) {
    // open the output file
    std::ofstream file(outputFileName, std::ios::app);

    // throw array into second line
    for (int i = 0 ; i < array.size() ; i++)
        file << array[i] << " ";
    file << std::endl;

    // now the results of the algorithm
    for (int i = 0 ; i < times.size() ; i++)
        file << times[i] << std::endl;

}

/**
 * This function will time how long it will take for an array to be sorted. This function
 * will return the resulting time in ms.
 * @param arr
 * @param sortFunc
 * @return
 */
int timeFn(std::vector<int> arr, void (*sortFunc)(std::vector<int>&)) {
    // start time
    time_point<steady_clock> t1 = std::chrono::high_resolution_clock::now();

    // run the function
    sortFunc(arr);

    // end time & return ms difference
    time_point<steady_clock> t2 = std::chrono::high_resolution_clock::now();

    return (t2 - t1).count();
}

int main(int argc, char** argv) {
    // get out output file
    std::string file = argv[1];

    // Initialize vectors
    std::vector<int> base_array = getUnsortedArr(file);
    std::vector<int> times;

    /**
     * Run Sorts in the following order
     * 1. Insertion
     * 2. Merge
     * 3. Quick
     * 4. Radix
     */
    times.push_back(timeFn(base_array, Sorts::insertionSort));
    times.push_back(timeFn(base_array, Sorts::mergeSort));
    times.push_back(timeFn(base_array, Sorts::quickSort));
    times.push_back(timeFn(base_array, Sorts::radixSort));

    // we will actually sort it now
    Sorts::mergeSort(base_array);

    outputResults(base_array, times, file);
}