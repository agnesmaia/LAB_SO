#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: xargx command [arg ...]\n");
    exit(1);
  }

  const int maxArgumentLenght = 512; // tamanho máximo de cada argumento
  char buf[maxArgumentLenght]; // criação do buffer
  char *cmd_argv[argc + 1]; // armazena os argumentos e o nome do comando

  //
  for (int i = 1; i < argc; i++) {
    cmd_argv[i - 1] = argv[i];
  }
  cmd_argv[argc - 1] = buf;
  cmd_argv[argc] = NULL;

  //variáveis para leitura dos argumentos do terminal
  int i = 0;
  int ignore = 0;
  int ch;

  //leitura dos caracteres até o fim do arquivo
  while ((ch = getchar()) != EOF) {
    if (ch == '\n') { // se for uma quebra de linha, termina a leitura do argumento atual
      if (ignore) {
        //
        i = 0;
        ignore = 0;
        continue;
      }
      buf[i] = '\0'; // adiciona o '\0' no final do buffer
      i = 0;

      pid_t pid = fork(); // novo processo com os argumentos passados

      if (pid == 0) {
        //child
        execvp(cmd_argv[0], cmd_argv);
        perror("execvp");
        exit(1);
      } else if (pid == -1) {
        perror("fork");
        exit(1);
      } else {
        //parent
        wait(NULL);
      }
    } else {
      if (!ignore && i >= maxArgumentLenght - 1) {
        fprintf(stderr, "xargs: argument too long\n");
        ignore = 1;
      }
      if (!ignore) {
        buf[i++] = ch;
      }
    }
  }
  exit(0);
}
