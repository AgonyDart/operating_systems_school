#include <stdio.h>
#include <sys/types.h>

int getpid();
int getppid();
pid_t fork();

int main()
{
    pid_t pid;
    pid = fork(); // crea el sub proceso
    switch (pid)
    {
    case -1:
        perror("fork");
        break;
    case 0:
        printf("soy el hijo %d\n\n", getpid());
        break;
    default:
        printf("soy el padre %d\n\n", getpid());
        break;
    }
    printf("FIN\n");
    return 0;
}