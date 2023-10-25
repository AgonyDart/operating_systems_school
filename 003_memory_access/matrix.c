#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void printMatrix(int **matrix, int size)
{
    int rows = size;
    int cols = size;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("[%4d]", matrix[i][j]);
        }
        printf("\n");
    }
}

int main()
{
    int MATRIX_SIZE = 0;
    printf("Enter the length of the matrix: ");
    scanf("%d", &MATRIX_SIZE);

    int **matrix1 = (int **)malloc(MATRIX_SIZE * sizeof(int *));
    int **matrix2 = (int **)malloc(MATRIX_SIZE * sizeof(int *));
    int **result = (int **)malloc(MATRIX_SIZE * sizeof(int *));

    double start, end;
    start = omp_get_wtime();

    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        matrix1[i] = (int *)malloc(MATRIX_SIZE * sizeof(int));
        matrix2[i] = (int *)malloc(MATRIX_SIZE * sizeof(int));
        result[i] = (int *)malloc(MATRIX_SIZE * sizeof(int));
    }

    int NUM_THREADS = 0;
    printf("Enter the number of threads: ");
    scanf("%d", &NUM_THREADS);
    omp_set_num_threads(NUM_THREADS);

    srand(omp_get_thread_num());
#pragma omp parallel for
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            matrix1[i][j] = (rand() % 201) - 100;
            matrix2[i][j] = (rand() % 201) - 100;
        }
    }

    int BLOCK_SIZE = MATRIX_SIZE / NUM_THREADS;

#pragma omp parallel for
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            int thread_num = omp_get_thread_num();
            printf("\nThread number: %d || working on row [%4d], col [%4d]", thread_num, i, j);
            result[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }

    end = omp_get_wtime();

    // printf("\nMatrix 1:\n");
    // printMatrix(matrix1, MATRIX_SIZE);

    // printf("\nMatrix 2:\n");
    // printMatrix(matrix2, MATRIX_SIZE);

    // printf("\nResult Matrix:\n");
    // printMatrix(result, MATRIX_SIZE);

    printf("\nTime taken: %f seconds\n", end - start);

#pragma omp parallel for
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        free(matrix1[i]);
        free(matrix2[i]);
        free(result[i]);
    }

    free(matrix1);
    free(matrix2);
    free(result);

    return 0;
}
