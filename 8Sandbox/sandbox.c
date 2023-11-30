#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "vector.h"

// Prints the prompt string for the sandbox
void prompt() { 
    char *username = getenv("USER");
    char currentWorkingDirectory[256];
    getcwd(currentWorkingDirectory, sizeof(currentWorkingDirectory));

    // TODO: replace all $HOME substrings with ~
    printf("%s@sandbox:%s> ", username, currentWorkingDirectory);
}

char** parseInput(char *input) {
    
    Vector *argVector = vector_new_with_capacity(10);
    char *saveptr = input;
    char *argument = NULL;
    do {
        argument = strtok_r(saveptr, " \t\r\n\v\f", &saveptr);  // Making sure strtok stops on any from of whitespace
        vector_push_ptr(argVector, argument);
    } while(argument != NULL);

    char **argArray = calloc(vector_size(argVector), 256);
    for (int i = 0; i < vector_size(argVector); i++) {
        vector_get(argVector, i, (int64_t *)argArray + i);
        // printf("%s ", argArray[i]);
    }
    vector_free(argVector);
    return argArray;
}

int main() {


    while (true) {
        prompt();

        // Parsing the input
        char *input = NULL;
        size_t length = 0;
        getline(&input, &length, stdin);
        
        char **argArray = parseInput(input);


        // L O O K   F O R   P O S S I B L E   C O M M A N D S

        // Internal commands
        if (strcmp("exit", argArray[0]) == 0) {
            
            free(argArray);
            free(input);
            return 0;
        }
        else if (strcmp("cd", argArray[0]) == 0) {
            if (chdir(argArray[1]) == -1) {
                perror(argArray[1]);
            }
        }
        else if (strcmp("jobs", argArray[0])) {
            // TODO: jobs
        }
        else {  // Creating a child process to execute a non-internal command
            pid_t pid = fork();
            if (pid == 0) { // Making the child execute the command
                execvp(argArray[0], argArray);
                perror(argArray[0]);
                return -1;
            }
            int wstatus;
            waitpid(pid, &wstatus, 0);
        } 

        
    }
    
    return 0;
}