#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void compexch(int *a, int *b)
{
    if (b < a){
        int t = *a;
        *a = *b;
        *b = t;
    }
}

void shuffle(int a[], const int size, int l, int r)
{
    int half = (int) (l + r) / 2;
    int b[size];
    int i, j;
    for (i = l, j = 0; i <= r; i += 2, j++){
        b[i] = a[l + j];
        b[i + 1] = a[half + j + 1];
    }
    for (i = 0; i < size; i++)
        a[i] = b[i];
}


void unshuffle(int a[], const int size, int l, int r)
{
    int half = (int) (l + r) / 2;
    int b[size];
    int i, j;
    for (i = l, j = 0; i <= r; i += 2, j++){
        b[l + j] = a[i];
        b[half + j + 1] = a[i + 1];
    }
    for (i = 0; i < size; i++)
        a[i] = b[i];
}

void OddEvenMergeSort(int a[], const int size,  int l, int r)
{
    if (r == l + 1) compexch(&a[l], &a[r]); // мы дошли до подмассива размера 2 - теперь просто сравним элементы
    if (r < l + 2) return; //дошли до подмассива размера 1 - выходим, такой подмассив априори отсортирован
    unshuffle(a, size, l, r); //делим подмассив на две части
    int half = (int) (l + r) / 2;
    OddEvenMergeSort(a, size,  l, half);
    OddEvenMergeSort(a, size, half + 1, r); //вызываемся рекурсивно для половинок
    shuffle(a, size, l, r); //сливаем части
    for (int i = l + 1; i < r; i += 2)
        compexch(&a[i], &a[i + 1]);
    int halfSize = (r - l + 1) / 2 - 1;
    for (int i = l + 1; i + halfSize < r; i++)
        compexch(&a[i], &a[i + halfSize]);
}



int main(int argc, char *argv[]) {
    /* srand(time(NULL));
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
*/
    int a[] = {5, 4, 9, 15, 16, 18};
    int size = 6;
    for(int i = 0; i < size; i++)
        printf("%d ", a[i]);
    printf("\n");
    printf("\n");
    OddEvenMergeSort(a, size, 0, size - 1);
    for(int i = 0; i < size; i++)
        printf("%d ", a[i]);
    printf("\n");
    return 0;
}
