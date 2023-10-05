#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

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
    }
  }
}

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
    }
  }
  return 0;
}