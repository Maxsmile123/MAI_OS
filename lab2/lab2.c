#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

void Reverse(char name[])
{
    char tmp;
    int len = strlen(name);
    for(int i = 0; i < len; i++)
    {
        tmp = name[len - 1];
        name[len - 1] = name[i];
        name[i] = tmp;
        len--;
    }
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

int main(void)
{
    char filename[150];
    int fd1[2];
    int fd2[2];
    int counter = 1; // Четная или нечётная строка
    printf("Enter filename for first process\n");
    scanf("%s", filename);
    int file1 = open(filename, O_RDWR | O_CREAT, 0777);
    printf("Enter filename for second process\n");
    scanf("%s", filename);
    int file2 = open(filename, O_RDWR | O_CREAT, 0777);

    if(file1 < 0 | file2 < 0){
        printf("One of the files opened incorrectly\n");
        exit(1);
    }
    if(pipe(fd1) == -1){
        printf("Pipe failed\n");
        exit(2);
    }
    if(pipe(fd2) == -1){
        printf("Pipe failed\n");
        exit(3);
    }
    pid_t child1 = fork();
    if(child1 == -1){
        printf("Fork failed\n");
        exit(4);
    }
    if(child1 == 0){
        pid_t child2 = fork();
        if (child2 == -1) {
            printf("Fork failed\n");
            exit(5);
        }
        if(child2 == 0){ // process2 is good
            while(1){
                int size2;
                char string2[size2];
                read(fd2[0], &size2, sizeof(int));
                read(fd2[0], string2, sizeof(char) * size2);
                printf("%s -child2\n", string2);
                Reverse(string2);
                write(file2, string2, sizeof(char) * size2);
                write(file2, "\n", sizeof(char));
            }
        } else {   // process1 is good
            while(1) {
                int size1;
                char string1[size1];
                read(fd1[0], &size1, sizeof(int));
                read(fd1[0], string1, sizeof(char) * size1);
                printf("%s -child1\n", string1);
                Reverse(string1);
                write(file1, string1, sizeof(char) * size1);
                write(file1, "\n", sizeof(char));
            }
        }
    } else { // Parent's process
        while(1) {
            int len;
            char *string = get_string(&len);
            if(counter % 2 == 0){
                int len2 = strlen(string);
                write(fd2[1], &len2, sizeof(int));
                write(fd2[1], string, sizeof(char)* len2);
            } else{
                int len1 = strlen(string);
                write(fd1[1], &len1, sizeof(int));
                write(fd1[1], string, sizeof(char)* len1);
            }
            counter++;
        }
        close(fd1[0]); close(fd2[0]);
        close(fd1[1]); close(fd2[1]);
        close(file1); close(file2);
    }
    return 0;
}
