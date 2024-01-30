#include "types.h"
#include "user.h"
#include "x86.h"

#define N 3
#define DELAY_MS 100
lock_t lock;

int matrix_a[N][N] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
};

int matrix_b[N][N] = {
    {9, 8, 7},
    {6, 5, 4},
    {3, 2, 1}
};

int result[N][N];

void multiply_row(void *arg1, void *arg2) {
    int row = *(int*)arg1;
    int i, j;
    lock_acquire(&lock);
    printf(1, "Thread for row %d started.\n", row );
    lock_release(&lock);

    for (i = 0; i < N; i++) {
        result[row][i] = 0;
        for (j = 0; j < N; j++) {
            result[row][i] += matrix_a[row][j] * matrix_b[j][i];
        }
    }
    lock_acquire(&lock);
    printf(1, "Thread for row %d finished.\n",row);
    lock_release(&lock);
    exit();
}

int main() {
    lock_init(&lock);
    int *row;
    for (int i = 0; i < N; i++) {
        row = malloc(sizeof(int)); // Allocate memory for row
        *row = i; // Set the value of row
        thread_create(multiply_row, row, 0); // Pass the address of row
    }
    for (int i = 0; i < N; i++) {
        thread_join();
    }

    // Print the result
    printf(1, "Matrix multiplication result:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf(1, "%d ", result[i][j]);
        }
        printf(1, "\n");
    }

    exit();
}