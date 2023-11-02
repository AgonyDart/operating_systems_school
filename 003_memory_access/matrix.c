#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_THREADS 5000

int n, m;
int num_threads;
int **matriz1;
int **matriz2;
int **resultado;
clock_t start, end;
double execution_time;

struct ThreadData
{
    int thread_id;
    int start_row;
    int end_row;
};

void *suma_bloques(void *arg)
{
    struct ThreadData *data = (struct ThreadData *)arg;
    int start_row = data->start_row;
    int end_row = data->end_row;

    for (int i = start_row; i < end_row; i++)
    {
        for (int j = 0; j < m; j++)
        {
            resultado[i][j] = matriz1[i][j] + matriz2[i][j];
        }
    }

    pthread_exit(NULL);
}

void print_matrix(int **matrix, int n, int m)
{
    printf("\n");
    for (int i = 0; i < n; i++)
    {
        printf("|");
        for (int j = 0; j < m; j++)
        {
            printf("[%4d] ", matrix[i][j]);
        }
        printf("|\n");
    }
}

int main()
{
    srand(time(NULL));

    printf("Ingrese el valor de n: ");
    scanf("%d", &n);
    printf("Ingrese el valor de m: ");
    scanf("%d", &m);

    matriz1 = (int **)malloc(n * sizeof(int *));
    matriz2 = (int **)malloc(n * sizeof(int *));
    resultado = (int **)malloc(n * sizeof(int *));

    start = clock();
    printf("Creando matrices...\n");
    for (int i = 0; i < n; i++)
    {
        matriz1[i] = (int *)malloc(m * sizeof(int));
        matriz2[i] = (int *)malloc(m * sizeof(int));
        resultado[i] = (int *)malloc(m * sizeof(int));
        for (int j = 0; j < m; j++)
        {
            matriz1[i][j] = (rand() % 200) - 100;
            matriz2[i][j] = (rand() % 200) - 100;
        }
    }

    printf("Ingrese el número de hilos a crear: ");
    scanf("%d", &num_threads);
    if (num_threads > MAX_THREADS)
    {
        printf("El número de hilos no puede ser mayor que %d\n", MAX_THREADS);
        return 1;
    }

    pthread_t threads[MAX_THREADS];
    struct ThreadData thread_data[MAX_THREADS];
    int rows_per_thread = n / num_threads;
    int remaining_rows = n % num_threads;
    printf("Remain: %d\n", remaining_rows);
    printf("rows: %d\n", rows_per_thread);
    int current_row = 0;

    for (int i = 0; i < num_threads; i++)
    {
        int rows = rows_per_thread + (i < remaining_rows ? 1 : 0);
        thread_data[i].thread_id = i;
        thread_data[i].start_row = current_row;
        thread_data[i].end_row = current_row + rows;
        current_row += rows;

        printf("Soy el hilo %d y voy a sumar de la fila %d a la fila %d\n", i, thread_data[i].start_row, thread_data[i].end_row);

        if (pthread_create(&threads[i], NULL, suma_bloques, &thread_data[i]))
        {
            fprintf(stderr, "Error al crear el hilo %d\n", i);
            return 1;
        }
    }

    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    end = clock();
    execution_time = (double)(end - start) / CLOCKS_PER_SEC;

    // printf("Matriz 1:\n");
    // print_matrix(matriz1, n, m);

    // printf("\nMatriz 2:\n");
    // print_matrix(matriz2, n, m);

    // printf("\nResultado:\n");
    // print_matrix(resultado, n, m);

    printf("\nTiempo de ejecucion: %4f\n", execution_time);

    for (int i = 0; i < n; i++)
    {
        free(matriz1[i]);
        free(matriz2[i]);
        free(resultado[i]);
    }
    free(matriz1);
    free(matriz2);
    free(resultado);

    return 0;
}