#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 11// 9 threads for each column, 9 threads for each row, and 9 threads for each subgrid
#define SIZE 9 // Size of sudoku grid

// Sample correct sudoku puzzle
int sudoku[SIZE][SIZE] = {
 {6, 3, 9, 5, 7, 4, 1, 8, 2},
 {5, 4, 1, 8, 2, 9, 3, 7, 6},
 {7, 8, 2, 6, 1, 3, 9, 5, 4},
 {1, 9, 8, 4, 6, 7, 5, 2, 3},
 {3, 6, 5, 9, 8, 2, 4, 1, 7},
 {4, 2, 7, 1, 3, 5, 8, 6, 9},
 {9, 5, 6, 7, 4, 8, 2, 3, 1},
 {8, 1, 3, 2, 9, 6, 7, 4, 5},
 {2, 7, 4, 3, 5, 1, 6, 9, 8}}
;

int valid[NUM_THREADS] = {0}; // Array to hold the validity of each check

// structure for passing data to threads
typedef struct {
    int row;
    int column;
} parameters;

// Method to check if each subbgrid is valid
void *checkSubGrid(void *params) {

    parameters *data = (parameters *)params;

    int row = data->row;
    int column = data->column;
    int nums[SIZE] = {0};

    for (int i = row; i < row + 3; i++) {
        for (int j = column; j < column + 3; j++) {
            int num = sudoku[i][j];
            if (num == 0 || nums[num - 1] == 1) {
                valid[NUM_THREADS - 1] = 0;
                pthread_exit(NULL);
            }
            nums[num - 1] = 1;
        }
    }

    valid[NUM_THREADS - 1] = 1;
    pthread_exit(NULL);
}

// Method to check if row is valid
void *checkRow(void *params) {

    parameters *data = (parameters *)params;
    int row = data->row;
    int nums[SIZE] = {0};

    for (int j = 0; j < SIZE; j++) {
        int num = sudoku[row][j];
        if (num == 0 || nums[num - 1] == 1) {
            valid[NUM_THREADS - 2 - row] = 0;
            pthread_exit(NULL);
        }
        nums[num - 1] = 1;
    }

    valid[NUM_THREADS - 2 - row] = 1;
    pthread_exit(NULL);
}

// Method to check if column is valid
void *checkColumn(void *params) {

    parameters *data = (parameters *)params;

    int column = data->column;
    int nums[SIZE] = {0};

    for (int i = 0; i < SIZE; i++) {

        int num = sudoku[i][column];

        if (num == 0 || nums[num - 1] == 1) {

            valid[column] = 0;
            pthread_exit(NULL);

        }
        nums[num - 1] = 1;
    }

    valid[column] = 1;
    pthread_exit(NULL);
}

int main() {

    pthread_t threads[27];
    int createThread;

    // Will create threads for each column
    for (int i = 0; i < SIZE; i++) {

        parameters *data = (parameters *)malloc(sizeof(parameters));

        data->row = 0;
        data->column = i;

        createThread = pthread_create(&threads[i], NULL, checkColumn, (void *)data);

        if (createThread) {

            printf("Error: Thread cannot be created,%d\n", createThread);
            exit(-1);

        }
    }

    /// Will create threads for each row
    for (int i = 0; i < SIZE; i++) {

        parameters *data = (parameters *)malloc(sizeof(parameters));

        data->row = i;
        data->column = 0;
        createThread = pthread_create(&threads[SIZE+i], NULL, checkRow, (void *)data);

        if (createThread) {

            printf("Error: Thread cannot be created,%d\n", createThread);
            exit(-1);

        }
    }

    // Will create the threads for each sub grid
    int thread_index = SIZE*2;

    for (int i = 0; i < SIZE; i += 3) {

        for (int j = 0; j < SIZE; j += 3) {

            parameters *data = (parameters *)malloc(sizeof(parameters));

            data->row = i;
            data->column = j;
            createThread = pthread_create(&threads[thread_index++], NULL, checkSubGrid, (void *)data);

            if (createThread) {

                printf("Error: Thread cannot be created,%d\n", createThread);
                exit(-1);

            }
        }
    }

    // Wait for all the threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {

        pthread_join(threads[i], NULL);

    }

    // Will check if the sudoku is valid 
    for (int i = 0; i < NUM_THREADS; i++) {

        if (!valid[i]) {

            printf("The Sudoku is INVALID.\n");
            return 0;

        }

    }

    printf("The Sudoku is VALID\n");

    return 0;
}
