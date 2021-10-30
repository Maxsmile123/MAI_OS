#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include "queue.h"

struct send{
  int *a;
  int size;
  int l;
  int r;
};

void compexch(int *a, int *b) {
  if (*b < *a) {
    int t = *a;
    *a = *b;
    *b = t;
  }
}

bool is_int(char *a){
  char *temp = a;
  if(*temp == '-') temp++;
  while(*temp != '\0'){
    if(*temp > '9' || *temp < '0'){
      return false;
    }
    temp++;
  }
  return true;
}

int str_to_int(char *a){
  int sign = 1;
  if(*a == '-'){
    sign = -1;
    a++;
  }
  int res = 0;
  for(char *temp = a; *temp != '\0'; temp++){
    res *= 10;
    res += *temp-'0';
  }
  if(sign == -1){
    res *= -1;
  }
  return res;
}

int maxFromArray(int *a, const int size){
  int max = INT_MIN;
  for(int i = 0; i < size; i++){
    if(a[i] > max) max = a[i];
  }
  return max;
}

void *shuffle(void *arg) {
  struct send *received = (struct send*)arg;

  int half = (int) (received->l + received->r) / 2;
  int tmp[received->size];
  int i;
  int winRight = 0;
  int winLeft = 0;

  for (i = 0; i < received->size; i++)
    tmp[i] = received->a[i];

  while (winRight + winLeft != (received->r - received->l) + 1) {
    if(winLeft == (received->r - received->l + 1) / 2) {
      tmp[received->l + winRight + winLeft] = received->a[half + 1 + winRight];
      winRight++;
      continue;
    }
    if(winRight == (received->r - received->l + 1) / 2) {
      tmp[received->l + winRight + winLeft] = received->a[received->l + winLeft];
      winLeft++;
      continue;
    }
    if(received->a[received->l + winLeft] > received->a[half + 1 + winRight]) {
      tmp[received->l + winRight + winLeft] = received->a[half + 1 + winRight];
      winRight++;
    } else{
      tmp[received->l + winRight + winLeft] = received->a[received->l + winLeft];
      winLeft++;
    }
  }
  for (i = 0; i < received->size; i++)
    received->a[i] = tmp[i];
  free(received);
  return NULL;
}

void unshuffle(int a[], const int size, int l, int r) {
  int half = (int) (l + r) / 2;
  int tmp[size];
  int i, j;
  for (i = 0; i < size; i++)
    tmp[i] = a[i];
  for (i = l, j = 0; i <= r; i += 2, j++) {
    tmp[l + j] = a[i];
    tmp[half + j + 1] = a[i + 1];
  }
  for (i = 0; i < size; i++)
    a[i] = tmp[i];
}

void OddEvenMergeSort(int a[], const int size, int l, int r, const int max_threads, queue *q) {
  if (r == l + 1) compexch(&a[l], &a[r]); // мы дошли до подмассива размера 2 - теперь просто сравним элементы
  if (r < l + 2) return; // дошли до подмассива размера 1 - выходим, такой подмассив априори отсортирован
  unshuffle(a, size, l, r); // делим массив на две части
  int half = (int) (l + r) / 2;
  OddEvenMergeSort(a, size, l, half, max_threads, q);
  OddEvenMergeSort(a, size, half + 1, r, max_threads, q); //вызываемся рекурсивно для половинок
  if(max_threads < 0){ // no limit
    struct send *s = malloc(sizeof(struct send));
    s->a = a;
    s->l = l;
    s->r = r;
    s->size = size;
    pthread_t *thr = malloc(sizeof(pthread_t));
    point1:{
    int check = pthread_create(thr, NULL, shuffle, (void*)s);
    if(check != 0){
      pthread_join(*(q->first->thread), NULL);
      pop(q);
      goto point1;
    }else{
      push(q, thr);
    }
    while(!isEmpty(q)){
      pthread_join(*(q->first->thread), NULL);
      pop(q);
    }
  }
  } else{ // limit
    struct send *s = malloc(sizeof(struct send));
    s->a = a;
    s->l = l;
    s->r = r;
    s->size = size;
    pthread_t *thr = malloc(sizeof(pthread_t));
    point2:{
    if(sizes(q) < max_threads){
      int check = pthread_create(thr, NULL, shuffle, (void*)s);
      if(check != 0){
        pthread_join(*(q->first->thread), NULL);
        pop(q);
        goto point2;
      }else{
        push(q, thr);
      }
    } else{
      pthread_join(*(q->first->thread), NULL);
      pop(q);
      goto point2;
    }
  }
    while(!isEmpty(q)){
      pthread_join(*(q->first->thread), NULL);
      pop(q);
    }

  }
}



int main(int argc, char *argv[]) {

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
  int size;
  int max;

  printf("Enter the size of array : ");
  scanf("%d", &size);

  int sizeOfArray = pow(2,(int)log2(size) + 1);

  if (size == sizeOfArray/2) sizeOfArray/=2;
  int a[sizeOfArray];

  printf("Enter %d numbers of array : ", size);
  for (int i = 0; i < size; i++)
    scanf("%d", &a[i]);

  max = maxFromArray(a, size);
  for(int i = size; i < sizeOfArray; i++)
    a[i] = max;

  queue *q;
  create(&q);

  OddEvenMergeSort(a, sizeOfArray, 0, sizeOfArray - 1, max_threads, q);
  for (int i = 0; i < size; i++)
    printf("%d ", a[i]);
  printf("\n");
  return 0;
}