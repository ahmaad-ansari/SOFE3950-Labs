#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>

extern char **environ; // Declare the environment variable

#define MAX_COMMAND_INPUT 1024

// Internal command functions
void execute_commands(char *command, char **args, int num_args);
void cd(char *directory);
void clr();
void dir(char *directory);
void env();
void echo(char *comment);
void manual();
void pause_shell();
void quit_shell();

int main(int argc, char **argv, char **envp) {
  // Set the shell environment variable to the full path of the myshell executable
  char *shell_path = realpath(argv[0], NULL);
  char *shell_var = malloc(strlen(shell_path) + 7);
  sprintf(shell_var, "shell=%s", shell_path);
  putenv(shell_var);

  // If a command line argument is present, read commands from the file
  if (argc > 1) {
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
      printf("Unable to open file: %s\n", argv[1]);
      exit(EXIT_FAILURE);
    }

    char *line = NULL;
    size_t line_size = 0;
    ssize_t line_len;

    while ((line_len = getline(&line, &line_size, file)) != -1) {
      // Remove trailing newline character from the line
      if (line[line_len - 1] == '\n') {
        line[line_len - 1] = '\0';
      }

      //Parse and execute the command
      char *command = strtok(line, " ");
      if (command != NULL) {
        char *args[MAX_COMMAND_INPUT];
        int arg_count = 0;

        while (arg_count < MAX_COMMAND_INPUT - 1) {
          args[arg_count] = strtok(NULL, " ");
          if (args[arg_count] == NULL) {
            break;
          }
          arg_count++;
        }
        args[arg_count] = NULL;

        execute_commands(command, args, arg_count);
      }
    }

  }
  else {
    // Prompt the user for input and execute commands
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

      execute_commands(args[0], args, num_args);

    }
  }

  

  // Free dynamically allocated memory
  free(shell_var);

  return 0;
}

void execute_commands(char *command, char **args, int num_args) {
  if (num_args == 0) {
    // Empty command, do nothing
  }
  else if (strcmp(command, "cd") == 0) {
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
  else if (strcmp(command, "clr") == 0) {
    // Clear screen command
    clr();
  }
  else if (strcmp(command, "dir") == 0) {
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
  else if (strcmp(command, "environ") == 0) {
    // List environment variables
    env();
  }
  else if (strcmp(command, "echo") == 0) {
    // Echo comment command
    if (num_args > 1) {
      for (int i = 1; i < num_args; i++) {
        printf("%s ", args[i]);
      }
      printf("\n");
    }
  }
  else if (strcmp(command, "help") == 0) {
    // Help command
    manual();
  }
  else if (strcmp(command, "pause") == 0) {
    // Run Pause command
    pause_shell();
  }
  else if (strcmp(command, "quit") == 0) {
    // Run Quit command
    quit_shell();
  }
  else {
    // External command: fork and execute
    int child_pid = fork();
    if (child_pid == 0) {
      // Set the parent environment variable to the shell path
      char *parent_var = malloc(strlen("parent=") + strlen(getenv("shell")) + 1);
      sprintf(parent_var, "parent=%s", getenv("shell"));
      putenv(parent_var);

      // Execute the program
      execvp(args[0], args);

      // Should not reach this line if execvp succeeds
      printf("Unknown command: %s\n", args[0]);
      exit(EXIT_FAILURE);
    }
    else if (child_pid > 0) {
      waitpid(child_pid, NULL, 0);
    }
    else {
      printf("Unable to fork child process.\n");
    }
  }
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
  system("clear");
}

void dir(char *directory) {
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

void env() {
  int i = 0;
  while (environ[i] != NULL) {
    printf("%s\n", environ[i]);
    i++;
  }
}

void manual() {
  FILE *fp = fopen("readme", "r");
  if (fp == NULL) {
    printf("Unable to open manual file.\n");
    return;
  }

  // Use the more filter to display the contents of the manual file
  char command[MAX_COMMAND_INPUT];
  sprintf(command, "more %s", "readme");
  FILE *pipe = popen(command, "r");
  if(pipe == NULL) {
    printf("Unable to start more filter.\n");
    return;
  }

  // Print the contents of the manual file
  char buffer[MAX_COMMAND_INPUT];
  while (fgets(buffer, MAX_COMMAND_INPUT, pipe) != NULL) {
    printf("%s", buffer);
  }

  pclose(pipe);
  fclose(fp);
}

void pause_shell() {
  printf("Press Enter to continue...");
  while (getchar() != '\n');
}

void quit_shell() {
  printf("Goodbye!\n");
  exit(EXIT_SUCCESS);
}
