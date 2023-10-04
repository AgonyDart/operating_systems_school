#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int n = 0;

int main()
{
  scanf("Enter the length of the array: %d", &n);
  printf("The length of the array is: %d\n", n);
  return 0;
}
