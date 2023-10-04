#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

void wait();

int main()
{
    int n = 5; // Cambia n al número deseado de subprocesos

    for (int i = 0; i < n; i++)
    {
        pid_t child = fork();

        if (child < 0)
        {
            fprintf(stderr, "Error al crear el subproceso %d\n", i);
            return 1;
        }
        else if (child == 0)
        {
            // Código ejecutado por el subproceso hijo
            printf("Soy el subproceso %d, mi PID es %d, y mi padre es %d\n", i, getpid(), getppid());
            // Realiza tareas específicas para el subproceso hijo
            exit(0); // Importante: asegúrate de que los subprocesos hijos terminen correctamente
        }
    }

    // Código ejecutado por el proceso padre
    printf("Soy el proceso padre, mi PID es %d\n", getpid());

    // El proceso padre puede realizar tareas específicas aquí

    // Esperar a que todos los subprocesos hijos terminen
    for (int i = 0; i < n; i++)
    {
        wait(NULL);
    }

    return 0;
}