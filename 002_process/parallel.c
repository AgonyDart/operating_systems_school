#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int array_size;
void wait();
int subprocesses;

// A function to compare two integers for qsort
int compare(const void *a, const void *b)
{
  int x = *(int *)a;
  int y = *(int *)b;

  // If both are even, compare in descending order
  if (x % 2 == 0 && y % 2 == 0)
  {
    return y - x;
  }

  // If both are odd, compare in ascending order
  if (x % 2 == 1 && y % 2 == 1)
  {
    return x - y;
  }

  // If x is even and y is odd, x comes first
  if (x % 2 == 0 && y % 2 == 1)
  {
    return -1;
  }

  // If x is odd and y is even, y comes first
  if (x % 2 == 1 && y % 2 == 0)
  {
    return 1;
  }

  // This should never happen
  return 0;
}

void order_numbers(int *arr, int n)
{
  qsort(arr, n, sizeof(int), compare);
}

int main()
{
  clock_t start, end;
  double execution_time;
  start = clock();
  printf("Enter the length of the array: ");
  scanf("%d", &array_size);

  int *p_original_array = (int *)malloc(array_size * sizeof(int));
  if (p_original_array == NULL)
  {
    printf("Memory allocation failed\n");
    exit(1);
  }
  else
  {
    printf("-- Memory allocation successful --\n");

    // Fill the array with ultra mega random numbers
    srand(time(0));
    for (int i = 0; i < array_size; i++)
    {
      p_original_array[i] = rand() % 1048576 + 1;
    }
    printf("Array elements are: [");
    for (int i = 0; i < array_size; i++)
    {
      printf("%d ", p_original_array[i]);
    }

    printf("]\nEnter how many subprocesses you want to create: ");
    scanf("%d", &subprocesses);

    int chunk_size = array_size / subprocesses;
    // printf("Chunk size: %d\n", chunk_size);
    int remainder = array_size % subprocesses;
    int pipes[subprocesses][2];

    // Create a pipe for each subprocess
    for (int i = 0; i < subprocesses; i++)
    {
      if (pipe(pipes[i]) == -1)
      {
        perror("Pipe failed");
      }
    }

    for (int i = 0; i < subprocesses; i++)
    {

      pid_t child = fork();
      if (child < 0)
      {
        printf("Error creating subprocess\n");
        exit(1);
      }
      else if (child == 0)
      {
        // Child process
        if (i == subprocesses - 1 && remainder > 0)
        {
          chunk_size += remainder;
        }
        // Close the read end of the pipe
        close(pipes[i][0]);
        int *p_chunk_array = (int *)malloc(chunk_size * sizeof(int));
        if (p_chunk_array == NULL)
        {
          printf("Memory allocation failed\n");
          exit(1);
        }
        else
        {
          int k = 0;

          printf(" -- -- -- -- --\nSubprocess %d", i);
          printf(", My array elements are: [");

          for (int j = i * chunk_size; j < (i + 1) * chunk_size; j++)
          {
            p_chunk_array[k] = p_original_array[j];
            printf("%d ", p_chunk_array[k]);
            k++;
          }

          // int p_chunk_array_size = sizeof(p_chunk_array) / sizeof(chunk_size);
          order_numbers(p_chunk_array, chunk_size);
          printf("]\nMy ordered array elements are: [");

          for (int j = 0; j < chunk_size; j++)
          {
            printf("%d ", p_chunk_array[j]);
          }

          printf("]\n");
          write(pipes[i][1], p_chunk_array, chunk_size * sizeof(int));
          // Close the write end of the pipe
          close(pipes[i][1]);
          exit(0);
        }
      }
    }
    // Parent process
    int *p_merged_array = (int *)malloc(array_size * sizeof(int));
    if (p_merged_array == NULL)
    {
      perror("Memory allocation failed");
    }
    else
    {
      int merge_index = 0;
      for (int i = 0; i < subprocesses; i++)
      {
        wait(0);
        close(pipes[i][1]);
        int *p_chunk_array = (int *)malloc(chunk_size * sizeof(int));
        if (p_chunk_array == NULL)
        {
          perror("Memory allocation failed");
        }
        else
        {
          read(pipes[i][0], p_chunk_array, chunk_size * sizeof(int));
          close(pipes[i][0]);
          for (int j = 0; j < chunk_size; j++)
          {
            p_merged_array[merge_index] = p_chunk_array[j];
            merge_index++;
          }
          free(p_chunk_array);
        }
      }
      order_numbers(p_merged_array, array_size);
      end = clock();
      printf("\nMerged ordered array elements are: [");
      for (int i = 0; i < array_size; i++)
      {
        printf("%d ", p_merged_array[i]);
      }
      printf("]\n");
      free(p_merged_array);
      execution_time = (double)(end - start) / CLOCKS_PER_SEC;
      printf("===========================================\nThe program took %f seconds to execute\n", execution_time);
    }
  }
  return 0;
}