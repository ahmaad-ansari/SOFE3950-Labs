#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT 1024

// Internal command functions
void change_directory(char *directory);
void clear_screen();

int main() {
  char input[MAX_INPUT];
  char *args[MAX_INPUT];
  int num_args;

  while (1) {
    printf("myshell> ");
    if (fgets(input, MAX_INPUT, stdin) == NULL) {
      printf("\n");
      break;
    }

    // Split input into arguments
    num_args = 0;
    args[num_args] = strtok(input, " \n");
    while (args[num_args] != NULL) {
      num_args++;
      args[num_args] = strtok(NULL, " \n");
    }

    if (num_args == 0) {
      // Empty command, do nothing
    } else if (strcmp(args[0], "cd") == 0) {
      // Change directory command
      if (num_args == 1) {
        // Report current directory
        printf("%s\n", getcwd(NULL, 0));
      } else {
        // Change directory
        change_directory(args[1]);
      }
    } else if (strcmp(args[0], "clr") == 0) {
      // Clear screen command
      clear_screen();
    } else {
      // External command, not implemented
      printf("Command not implemented.\n");
    }
  }

  return 0;
}

void change_directory(char *directory) {
  if (chdir(directory) != 0) {
    printf("Directory not found.\n");
  } else {
    char *cwd = getcwd(NULL, 0);
    setenv("PWD", cwd, 1);
    free(cwd);
  }
}

void clear_screen() {
  printf("\033[2J\033[1;1H");
}
