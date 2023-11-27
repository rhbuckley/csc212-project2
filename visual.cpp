#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>
#include <sstream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
int NUM_BARS; // Number of bars to visualize
const int BAR_SPACING = 1; // Spacing between bars

int BAR_WIDTH = (SCREEN_WIDTH - (NUM_BARS - 1) * BAR_SPACING) / NUM_BARS;
int MAX_BAR_HEIGHT = SCREEN_HEIGHT - 10;
int FRAME_DELAY = 50;

void renderBars(SDL_Renderer* renderer, const std::vector<int>& bars) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (size_t i = 0; i < bars.size(); ++i) {
        int barHeight = bars[i] * MAX_BAR_HEIGHT / *std::max_element(bars.begin(), bars.end());
        int x = i * (BAR_WIDTH + BAR_SPACING);
        SDL_Rect rect = { x, SCREEN_HEIGHT - barHeight, BAR_WIDTH, barHeight };
        SDL_SetRenderDrawColor(renderer, 37, 150, 190, 255);
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_RenderPresent(renderer);
}


void radixSort(std::vector<int>& arr, int numDigits, SDL_Renderer* renderer) {
    renderBars(renderer, arr);
    SDL_Delay(FRAME_DELAY);
    for (int i = 0; i < numDigits; i++) {
        // create empty containers that we will organize into
        std::vector<std::vector<int>> containers(10);

        // for num in arr
        for (int v : arr) {
            containers[(v / static_cast<int>(std::pow(10, i))) % 10].push_back(v);
        }
        // clear out arr
        arr.clear();

        // Rinse & Repeat
        for (const std::vector<int>& c : containers) {
            for (int i = 0; i < c.size(); i++) {
                arr.push_back(c[i]);
            }
        }
        renderBars(renderer, arr);
        SDL_Delay(FRAME_DELAY);
    }
}

void mergeSortHelper(std::vector<int>& arr, int left, int mid, int right, SDL_Renderer* renderer) {
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

    renderBars(renderer, arr);
    SDL_Delay(FRAME_DELAY);
}

void mergeSort(std::vector<int>& arr, int left, int right, SDL_Renderer* renderer) {
    if (left < right) {
        // Calculate the mid-point
        int mid = left + (right - left) / 2;

        // Recursively divide the array into two halves
        mergeSort(arr, left, mid, renderer);
        mergeSort(arr, mid + 1, right, renderer);

        // Merge the sorted halves
        mergeSortHelper(arr, left, mid, right, renderer);
    }
}

void insertionSort(std::vector<int>& arr, SDL_Renderer* renderer) {
    // Define temporary variables
    int j, k;

    // Loop through the array
    for (int i = 1 ; i < arr.size() ; i++) {
        // Update Values, Define temp (k)
        j = i, k = arr[i];

        // move element to correct position
        while (j > 0 && k < arr[j - 1]) {
            arr[j] = arr[j - 1];
            j--;
            renderBars(renderer, arr); // Visualize the array after each swap
            SDL_Delay(FRAME_DELAY);
        }

        // restore temp
        arr[j] = k;

        renderBars(renderer, arr); // Visualize the array after each element is moved to the correct position
        SDL_Delay(FRAME_DELAY);
    }
}

int quickSortHelper(std::vector<int>& arr, int left, int right, SDL_Renderer* renderer) {
    int i = left;
    int j = right + 1;

    while (true) {
        // while quick[i] < pivot, increase i
        while (arr[++i] < arr[left])
            if (i == right) break;

        // while quick[j] > pivot, decrease j
        while (arr[left] < arr[--j])
            if (j == left) break;

        // if i and j cross exit the loop
        if (i >= j) break;

        // swap quick[i], quick[j]
        std::swap(arr[i], arr[j]);

        // Visualize the array after each swap
        renderBars(renderer, arr);
        SDL_Delay(FRAME_DELAY);
    }

    // swap the pivot with quick[j]
    std::swap(arr[left], arr[j]);

    // Visualize the array after partitioning step
    renderBars(renderer, arr);
    SDL_Delay(FRAME_DELAY);
    
    // return pivot's position
    return j;
}

void quickSort(std::vector<int>& arr, int left, int right, SDL_Renderer* renderer) {
    if (right <= left)
        return;

    int pivot = quickSortHelper(arr, left, right, renderer);

    quickSort(arr, left, pivot - 1, renderer);
    quickSort(arr, pivot + 1, right, renderer);
}

void quickSort(std::vector<int>& arr, SDL_Renderer* renderer) {
    quickSort(arr, 0, arr.size() - 1, renderer);
}

int main(int argc, char* argv[]) {
    std::string fileName = argv[1];
    std::ifstream inputFile(fileName); 
    std::string line;
    std::getline(inputFile, line);
    std::istringstream data(line);

    std::vector<int> arr;

    int value;
    while (data >> value) {
        arr.push_back(value);
    }

    int mode = std::stoi(argv[2]);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Sort Visualizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    std::vector<int> bars(NUM_BARS);

    NUM_BARS = arr.size();
    BAR_WIDTH = (SCREEN_WIDTH - (NUM_BARS - 1) * BAR_SPACING) / NUM_BARS;
    MAX_BAR_HEIGHT = SCREEN_HEIGHT - 10;

    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Insertion
        if (mode == 0) {
            FRAME_DELAY = 25;
            insertionSort(arr, renderer);
        }
        // Merge
        else if (mode == 1) {
            mergeSort(arr, 0, arr.size() - 1, renderer);
        }
        // Quick
        else if (mode == 2) {
            FRAME_DELAY = 75;
            quickSort(arr, renderer);
        }
        // Radix
        else if (mode == 3) {
            FRAME_DELAY = 350;
            radixSort(arr, static_cast<int>(log10(NUM_BARS) + 1), renderer);
        }

        // Wait for the user to close the window
        while (!quit) {
            while (SDL_PollEvent(&event) != 0) {
                if (event.type == SDL_QUIT) {
                    quit = true;
                }
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
