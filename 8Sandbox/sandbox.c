#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <fcntl.h>
#include "vector.h"

/* Resources:   https://stackoverflow.com/questions/12784766/check-substring-exists-in-a-string-in-c
                https://www.geeksforgeeks.org/strtok-strtok_r-functions-c-examples/
*/

struct RunningProcess {
    pid_t pid;
    char **arguments;
};

// Prints the prompt string for the sandbox
void prompt() { 
    char *username = getenv("USER");
    char currentWorkingDirectory[256];
    getcwd(currentWorkingDirectory, sizeof(currentWorkingDirectory));

    // TODO: replace all $HOME substrings with ~
    char *cwdPtr = strstr(currentWorkingDirectory, getenv("HOME"));
    char *cwdStart = currentWorkingDirectory;
    while (cwdPtr != NULL) {
        cwdPtr += strlen(getenv("HOME")) - 1;
        cwdPtr[0] = '~';
        cwdStart = cwdPtr;
        cwdPtr = strstr(cwdPtr, getenv("HOME"));

    }
    printf("%s@sandbox:%s> ", username, cwdStart);
}

char** parseInput(char *input) {
    
    Vector *argVector = vector_new_with_capacity(10);
    char *saveptr = input;
    char *argument = NULL;
    do {
        argument = strtok_r(saveptr, " \t\r\n\v\f", &saveptr);  // Making sure strtok stops on any from of whitespace
        
        // Checking if the argument needs expanded from an environment variable
        if (argument != NULL) {
            if (argument[0] == '$'){
                argument = getenv(argument + 1);
                if (argument == NULL) {
                    argument = "";
                }
            }
        }
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

    // int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    // fcntl(0, F_SETFL, flags | O_NONBLOCK);
    // printf("among us");
    // fd_set readSet;

    while (true) {
        prompt();

        // Parsing the input
        char *input;
        size_t bytes = 0;
        getline(&input, &bytes, stdin);

        char **argArray = parseInput(input);


        // L O O K   F O R   P O S S I B L E   C O M M A N D S

        // Internal commands
        if (strcmp("exit", argArray[0]) == 0) {
            // Free the memory allocated for each argument
            // for (int i = 0;; i++) {
            //     free(argArray[i]);
            //     if (argArray[i] == NULL) {
            //         break;
            //     }
            // }
            free(input);
            free(argArray);
            return 0;
        }
        else if (strcmp("cd", argArray[0]) == 0) {
            if (argArray[1] == NULL || strcmp(argArray[1], "~") == 0) {
                argArray[1] = getenv("HOME");
            }
            if (chdir(argArray[1]) == -1) {
                perror(argArray[1]);
            }
        }
        else if (strcmp("jobs", argArray[0]) == 0) {
            // TODO: jobs
        }
        else {  // Creating a child process to execute a non-internal command
            pid_t pid = fork();
            if (pid == 0) { // Making the child execute the command
                execvp(argArray[0], argArray);
                perror(argArray[0]);
                free(argArray); // I used argArray to print a message within the child, so COW makes me have to free it in the child process
                return -1;
            }
            // for (int i = 0; argArray[i] != NULL; i++) {
            //     if (strcmp(argArray[i], "&") == 0) {
                    
            //     }
            // }
            int wstatus;
            waitpid(pid, &wstatus, 0);
        } 

        free(argArray);
        free(input);
    }
    
    return 0;
}