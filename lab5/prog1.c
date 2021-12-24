#include "lib1.h"
#include "lib2.h"
#include <stdio.h>

void help(){
    printf("USAGE: K arg1 arg2 ... arg N\n");
    printf("K = 1 - if you want to take the derivative. arg1 = point A, arg2 = increment\n");
    printf("K = 2 - if you want to calculate the number pi. arg1 = accuracy\n");
}

int main(void){
    int arg;
    help();
    while(scanf("%d ", &arg) != EOF){
        if(arg == 1){
            float A, deltaX;
            scanf("%f %f", &A, &deltaX);
            printf("%f\n", Derivative(A, deltaX));
        } else if (arg == 2){
            int k;
            scanf("%d", &k);
            printf("%f\n", Pi(k));
        } else{
            help();
            return -1;
        }

    }
    return 0;
}


