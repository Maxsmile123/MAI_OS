#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void compexch(int *a, int *b)
{
    if (b < a){
        a = a ^ b;
        b = b ^ a;
        a = a ^ b;
    }
}

void shuffle(int a[], const int size, unsigned int l, unsigned int r)
{
    unsigned int half = (unsigned int) (l + r) / 2;
    int b[size];
    unsigned int i, j;
    for (i = l, j = 0; i <= r; i += 2, j++){
        b[i] = a[l + j];
        b[i + 1] = a[half + j + 1];
    }
    for (i = 0; i < size; i++)
        a[i] = b[i];
}


void unshuffle(int a[], const int size, unsigned int l, unsigned int r)
{
    unsigned int half = (unsigned int) (l + r) / 2;
    int b[size];
    unsigned int i, j;
    for (i = l, j = 0; i <= r; i += 2, j++){
        b[l + j] = a[i];
        b[half + j + 1] = a[i + 1];
    }
    for (i = 0; i < size; i++)
        a[i] = b[i];
}



int main(int argc, char *argv[]) {
    srand(time(NULL));
    int max_threads;

    if (argc == 1) {
        printf("Limit of threads is not set. Default value is 10.\n");
        max_threads = 10;
    } else if (argc == 3) {
        if (strcmp(argv[1], "-t") != 0) {
            printf("Error: incorrect key\n");
            printf("Usage: %s [-t MAX_THREADS]\n", argv[0]);
            printf("if MAX_THREADS is negative value, then the number of threads is unlimited\n");
            return 1;
        }
        if (!is_int(argv[2])) {
            printf("Error: incorrect third argument\n");
            printf("Usage: %s [-t MAX_THREADS]\n", argv[0]);
            printf("if MAX_THREADS is negative value, then the number of threads is unlimited\n");
            return 1;
        }
        max_threads = str_to_int(argv[2]);
    } else {
        printf("Usage: %s [-t MAX_THREADS]\n", argv[0]);
        printf("if MAX_THREADS is negative value, then the number of threads is unlimited\n");
        return 1;
    }

}

