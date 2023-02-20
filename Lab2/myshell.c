#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// Function to handle the "cd" command
void cd(char* directory){
    char cwd[1024];

    // If no directory is provided, print the current working directory
    if(directory == NULL){
        if(getced(cwd, sizeof(cwd)) != NULL){
            printf("Current directory: %s\n", cwd);
        }
        else{
            perror("Error getting current directory");
        }
    }
    // Otherwise, try to change to the specified directory
    else{
        // Use the chdir function to change to the specified directory
        if(chdir(directory) != 0){
            perror("Error changineg directory")
        }
        else{
            // Use the setenv function to update the PWD environment variable
            setenv("PWD", directory, 1);
        }
    }
}


int main(){
    char input[1024];
    char *command, *arg;

    // Loop forever, until the user types "exit"
    while(1){
        // Print the shell prompt
        printf("myshell> ");
        // Read a line of input from the user using fgets
        if(fgets(input, 1024, stdin) == NULL){
            break;
        }
        // Remove the newline character from the input
        input[strlen(input - 1)] = '\0';

        // Split the input into the command and argument parts using strtok
        command = strtok(input, " ");
        arg = strtok(NULL, " ");

        // Check if the command is "cd"
        if(strcmp(command, "cd") == 0){
            // Call the cd function with the argument as input
            cd(arg);
        }
        // Check if the command is "exit"
        else if(strcmp(command, "exit") == 0){
            // Exit the shell
            break;
        }
        // If the command is not supported, print an error message
        else{
            printf("Command not supported\n");
        }
    }
    // Exit the program
    return 0;
}
