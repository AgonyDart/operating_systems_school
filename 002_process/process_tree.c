#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

<<<<<<< HEAD
// funcion que crea n subprocesos
void create_children(int n) {
  pid_t pid;
  for (int i = 0; i < n; i++) {
    pid = fork();
    if (pid == 0) {
      // subproceso
      printf("Soy el subproceso %d y mi PID es %d , y mi padre tiene el PID %d\n", i + 1, getpid(), getppid());
      exit(0);
    } else if (pid > 0) {
      // padre
      wait(NULL);
    } else {
      // Fork fail
      perror("fork");
      exit(1);
=======
void wait();

void crearArbol(int nivel, int maxNiveles)
{
    if (nivel == maxNiveles)
    {
        return; // Caso base: No se crean más hijos en el nivel dado
    }

    for (int i = 0; i <= nivel; i++)
    {
        pid_t child = fork();

        if (child < 0)
        {
            fprintf(stderr, "Error al crear el subproceso en el nivel %d\n", nivel);
            exit(1);
        }
        else if (child == 0)
        {
            // Código ejecutado por el subproceso hijo
            printf("Soy el subproceso en el nivel %d, mi PID es %d\n", nivel, getpid());
            // Realiza tareas específicas para el subproceso hijo
            crearArbol(nivel + 1, maxNiveles); // Llama recursivamente para crear más hijos en el siguiente nivel
            exit(0);                           // Importante: asegúrate de que los subprocesos hijos terminen correctamente
        }
>>>>>>> c0736816ef18ba5a6519958616efaa32cbfcca28
    }
  }
}

<<<<<<< HEAD
int main() {
  pid_t pid;
  int n = 3; // Numero de hijos del proceso root
  printf("proceso ROOT con PID  %d\n", getpid());
  for (int i = 0; i < n; i++) {
    pid = fork();
    if (pid == 0) {
      printf("Soy el subproceso %d y mi PID es %d , y mi padre tiene el PID %d, [ROOT]\n", i + 1, getpid(), getppid());
      create_children(i + 1); // Crea i + 1 subprocesos basados en el index
      exit(0);
    } else if (pid > 0) {
      wait(NULL); 
    } else {
      perror("fork");
      exit(1);
=======
int main()
{
    int n = 3; // Cambia n al número deseado de niveles del árbol

    printf("Soy el proceso raíz, mi PID es %d\n", getpid());

    crearArbol(0, n); // Llama a la función para crear el árbol de procesos

    // El proceso raíz puede realizar tareas específicas aquí

    // Esperar a que todos los subprocesos hijos terminen
    for (int i = 0; i < n; i++)
    {
        wait(NULL);
>>>>>>> c0736816ef18ba5a6519958616efaa32cbfcca28
    }
  }
  return 0;
}