#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

// Function to swap two elements
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Partition function to select a pivot and partition the array
int partition(int arr[], int low, int high) {
    int pivot = arr[high]; // Choose the pivot as the last element
    int i = (low - 1); // Initialize the index of the smaller element

    #pragma omp parallel for
    for (int j = low; j <= high - 1; j++) {
        // If the current element is smaller than or equal to the pivot
        if (arr[j] <= pivot) {
            #pragma omp critical
            {
                i++; // Increment index of the smaller element
                swap(&arr[i], &arr[j]);
            }
        }
    }

    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// Parallel quicksort function using OpenMP
void parallelQuickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        #pragma omp task
        parallelQuickSort(arr, low, pi - 1);

        #pragma omp task
        parallelQuickSort(arr, pi + 1, high);
    }
}

int main() {
    int n = 1000000; // Adjust the array size as needed
    int* arr_serial = (int*)malloc(n * sizeof(int));
    int* arr_parallel = (int*)malloc(n * sizeof(int));

    // Initialize arrays with random values
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        arr_serial[i] = arr_parallel[i] = rand() % 1000;
    }

    printf("Array size: %d\n", n);

    // Serial quicksort
    clock_t start_time = clock();
    parallelQuickSort(arr_serial, 0, n - 1);
    clock_t end_time = clock();
    double serial_execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Serial quicksort execution time: %f seconds\n", serial_execution_time);

    // Parallel quicksort using OpenMP
    start_time = clock();
    #pragma omp parallel
    {
        #pragma omp single
        parallelQuickSort(arr_parallel, 0, n - 1);
    }
    end_time = clock();
    double parallel_execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Parallel quicksort execution time: %f seconds\n", parallel_execution_time);

    // Check if the arrays are sorted correctly
    for (int i = 0; i < n - 1; i++) {
        if (arr_serial[i] > arr_serial[i + 1] || arr_parallel[i] > arr_parallel[i + 1]) {
            printf("Sorting Successful!");
            break;
        }
    }

    free(arr_serial);
    free(arr_parallel);
    return 0;
}
