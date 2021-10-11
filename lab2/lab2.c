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

    while (c != '\n') {
        s[(*len)++] = c; // заносим в строку новый символ
        // если реальный размер больше размера контейнера, то увеличим его размер
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
    scanf("%s", filename);
    int file1 = (filename, O_RDWR | O_CREAT, 0777);
    scanf("%s", filename);
    int file2 = (filename, O_RDWR | O_CREAT, 0777);

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
    if(!child1){
        pid_t child2 = fork();
        if (child2 == -1) {
            printf("Fork failed\n");
            exit(5);
        }
        if(!child2){ // process2 is good
            while(1){
                int size2;
                read(fd2[0], &size2, sizeof(int));
                char string2[size2];
                read(fd2[0], string2, sizeof(char) * size2);
                printf("%s From child2: ", string2);
                Reverse(string2);
                write(file2, string2, sizeof(char) * size2);
                write(file2, "\n", sizeof(char));
            }
        } else {   // process1 is good
            while(1) {
                int size1;
                read(fd1[0], &size1, sizeof(int));
                char string1[size1];
                read(fd1[0], string1, sizeof(char) * size1);
                printf("%s From child1: ", string1);
                Reverse(string1);
                write(file1, string1, sizeof(char) * size1);
                write(file1, "\n", sizeof(char));
            }
        }

    } else { // Parent's process
        while(1) {
            int len;
            char *string = get_string(&len);
            if(counter % 2){
                write(fd2[1], &len, sizeof(int));
                write(fd2[1], string, sizeof(char)* len)
            } else{
                write(fd1[1], &len, sizeof(int));
                write(fd1[1], string, sizeof(char)* len)
            }
            counter++;
        }
        close(fd1[0]); close(fd2[0]);
        close(fd1[1]); close(fd2[1]);
        close(file1); close(file2);
    }
    return 0;
}
