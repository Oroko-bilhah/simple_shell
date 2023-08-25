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

char *get_path(void) {
    // Implement this function
    // ...
}

int main(void) {
    char *line;
    char **args;
    size_t len = 0;
    pid_t child_pid;
    int status;
    char *path;

    while (1) {
        printf("($) ");
        getline(&line, &len, stdin);
        args = split_line(line);

        if (args[0] == NULL) {
            // Handle empty command
            continue;
        }

        path = get_path();
        child_pid = fork();
        if (child_pid == 0) {
            // Child process
            char *full_path;
            char *directories = strtok(path, ":");
            while (directories != NULL) {
                full_path = malloc(strlen(directories) + strlen(args[0]) + 2);
                if (!full_path) {
                    perror("Allocation error");
                    exit(EXIT_FAILURE);
                }
                sprintf(full_path, "%s/%s", directories, args[0]);
                execve(full_path, args, NULL);
                free(full_path);
                directories = strtok(NULL, ":");
            }
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
        free(path);
    }

    return 0;
}

