#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int *ptr;
int array_size;
int x;

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
  printf("Enter the length of the array: ");
  scanf("%d", &array_size);

  ptr = (int *)malloc(array_size * sizeof(int));
  if (ptr == NULL)
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
      int random_int = rand() % 1024 + 1;
      ptr[i] = random_int;
    }
    printf("Array elements are: [");
    for (int i = 0; i < array_size; i++)
    {
      printf("%d ", ptr[i]);
    }

    printf("]\nEnter how many subprocesses you want to create: ");
    scanf("%d", &x);

    int chunk_size = array_size / x;
    // printf("Chunk size: %d\n", chunk_size);
    for (int i = 0; i < x; i++)
    {
      pid_t pid = fork();
      if (pid < 0)
      {
        printf("Error creating subprocess\n");
        exit(1);
      }
      else if (pid == 0)
      {
        // Child process
        int *ptr_temp = (int *)malloc(chunk_size * sizeof(int));
        int k = 0;

        printf("\nSubprocess %d", i);
        printf(", My array elements are: [");
        for (int j = i * chunk_size; j < (i + 1) * chunk_size; j++)
        {
          ptr_temp[k] = ptr[j];
          printf("%d ", ptr_temp[k]);
          k++;
        }
        int aux = 0;
        int *p = &aux;
        int ptr_temp_size = sizeof(ptr_temp_size) / sizeof(ptr_temp_size[p]);
        order_numbers(ptr_temp, ptr_temp_size);
        printf("]\nMy ordered array elements are: [");
        for (int j = 0; j < chunk_size; j++)
        {
          printf("%d ", ptr_temp[j]);
        }

        printf("]\n");
        exit(0);
      }
    }
  }
  return 0;
}