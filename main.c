#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

char **split_line(char *line) {
    int bufsize = BUFFER_SIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens) {
        fprintf(stderr, "Allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, " \t\n");
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += BUFFER_SIZE;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) {
                fprintf(stderr, "Allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, " \t\n");
    }
    tokens[position] = NULL;
    return tokens;
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

        if (args[0] == NULL) {
            // Handle empty command
            continue;
        }

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

