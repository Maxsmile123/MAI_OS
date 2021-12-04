#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <stdint.h>

pid_t create_child(char *filename, int type)
{
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("Fork err");
        exit(1);
    }
    if (pid == 0)
    {
        int file_out = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);
        if (file_out < 0)
        {
            perror("File err");
            exit(1);
        }

        int new_out = dup2(file_out, STDOUT_FILENO);
        close(file_out);
        if (new_out < 0)
        {
            perror("Duping child stdout err");
            exit(1);
        }

        if (execlp("./child", "child", type ? "1" : "0", NULL) < 0)
        {
            perror("Child execl err");
            exit(1);
        }
    }
    return pid;
}

char* get_string(int *len) {
    *len = 0;
    int capacity = 1; // 1, так как точно будет '\0'
    char *s = (char*) malloc(sizeof(char));

    char c = getchar();
    if(c == '\n') c = getchar();

    while (c != '\n') {
        s[(*len)++] = c; // заносим в строку новый символ

        if (*len >= capacity) {
            capacity *= 2;
            s = (char*) realloc(s, capacity * sizeof(char));
        }

        c = getchar();
    }
    s[*len] = '\0';
    return s;
}

int main(int argc, char *argv[])
{
    int len;
    printf("Enter filename for first process\n");
    char *filename1 = get_string(&len);
    printf("Enter filename for second process\n");
    char *filename2 = get_string(&len);

    int input = open("./input.txt", O_RDWR | O_CREAT | O_TRUNC, 0777);
    if (input < 0) {
        perror("Couldn't open input file\n");
        exit(1);
    }

    char *msg;
    printf("Enter strings to process: \n");
    while ((msg = get_string(&len)) && msg[0] != '\0')
    {
        msg[len] = '\n';
        write(input, msg, sizeof(char) * len);
    }

    pid_t pid1, pid2;
    pid1 = create_child(filename1, 0);
    pid2 = create_child(filename2, 1);

    close(input);

    int statusChild1, statusChild2;
    waitpid(pid1, &statusChild1, 0);
    if (WIFEXITED(statusChild1))
    {
        printf("Child 1 exited, returned  %d\n", WEXITSTATUS(statusChild1));
    }
    else
    {
        fprintf(stderr, "Something is wrong with 1st child process\n");
    }
    waitpid(pid2, &statusChild2, 0);
    if (WIFEXITED(statusChild1))
    {
        printf("Child 2 exited, returned  %d\n", WEXITSTATUS(statusChild2));
    }
    else
    {
        fprintf(stderr, "Something is wrong with 2nd child process\n");
    }

    free(msg);

    return 0;
}