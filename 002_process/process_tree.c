#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

void wait();

void crearArbol(int nivel, int maxNiveles) {
    if (nivel == maxNiveles) {
        return;  // Caso base: No se crean más hijos en el nivel dado
    }

    for (int i = 0; i <= nivel; i++) {
        pid_t child = fork();

        if (child < 0) {
            fprintf(stderr, "Error al crear el subproceso en el nivel %d\n", nivel);
            exit(1);
        } else if (child == 0) {
            // Código ejecutado por el subproceso hijo
            printf("Soy el subproceso en el nivel %d, mi PID es %d\n", nivel, getpid());
            // Realiza tareas específicas para el subproceso hijo
            crearArbol(nivel + 1, maxNiveles);  // Llama recursivamente para crear más hijos en el siguiente nivel
            exit(0); // Importante: asegúrate de que los subprocesos hijos terminen correctamente
        }
    }
}

int main() {
    int n = 3; // Cambia n al número deseado de niveles del árbol
    
    printf("Soy el proceso raíz, mi PID es %d\n", getpid());
    
    crearArbol(0, n); // Llama a la función para crear el árbol de procesos

    // El proceso raíz puede realizar tareas específicas aquí
    
    // Esperar a que todos los subprocesos hijos terminen
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    return 0;
}