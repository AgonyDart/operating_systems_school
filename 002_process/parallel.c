#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int *ptr;
int array_size;
int x;

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
        printf("\nSubprocess %d", i);
        printf(", My array elements are: [");
        for (int j = i * chunk_size; j < (i + 1) * chunk_size; j++)
        {
          printf("%d ", ptr[j]);
        }
        printf("]");
        exit(0);
      }
    }

    // avoid the '%' sign in the printf statement
    printf("\n");
  }
  return 0;
}
