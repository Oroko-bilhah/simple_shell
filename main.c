#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

char **split_line(char *line) {
    // Implement this function to split the line into tokens
    // Use strtok or similar functions
}

int main(void) {
    char *line;
    char **args;
    size_t len = 0;
    pid_t child_pid;
    int status;

    while (1) {
        printf("($) ");
        getline(&line, &len, stdin);
        args = split_line(line);

        child_pid = fork();
        if (child_pid == 0) {
            // Child process
            execvp(args[0], args);
            perror("Error");
            exit(EXIT_FAILURE);
        } else if (child_pid < 0) {
            perror("Fork error");
        } else {
            // Parent process
            wait(&status);
        }

        free(line);
        free(args);
    }

    return 0;
}

