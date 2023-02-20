#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#define MAX_COMMAND_INPUT 1024

// Internal command functions
void cd(char *directory);
void clr();
void dir(char *directory);

int main() {
  char input[MAX_COMMAND_INPUT];
  char *args[MAX_COMMAND_INPUT];
  int num_args;

  while (1) {
    printf("myshell> ");
    if (fgets(input, MAX_COMMAND_INPUT, stdin) == NULL) {
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
    }
    else if (strcmp(args[0], "cd") == 0) {
      // Change directory command
      if (num_args == 1) {
        // Report current directory
        printf("%s\n", getcwd(NULL, 0));
      }
      else {
        // Change directory
        cd(args[1]);
      }
    } 
    else if (strcmp(args[0], "clr") == 0) {
      // Clear screen command
      clr();
    }
    else if (strcmp(args[0], "dir") == 0) {
      // List directory command
      if (num_args == 1) {
        // List contents of current directory
        dir(getcwd(NULL, 0));
      }
      else {
        // List contents of specified directory
        dir(args[1]);
      }
    }
    else {
      // External command, not implemented
      printf("Command not implemented.\n");
    }
  }

  return 0;
}

void cd(char *directory) {
  if (chdir(directory) != 0) {
    printf("Directory not found.\n");
  }
  else {
    char *cwd = getcwd(NULL, 0);
    setenv("PWD", cwd, 1);
    free(cwd);
  }
}

void clr() {
  printf("\033[2J\033[1;1H");
}

void dir(char *directory){
  DIR *dir = opendir(directory);
  if(dir == NULL) {
    printf("Directory not found.\n");
  }
  else {
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
      printf("%s\n", entry->d_name);
    }
    closedir(dir);
  }
}

