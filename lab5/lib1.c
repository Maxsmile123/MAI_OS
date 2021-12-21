#include "lib1.h"
#include <math.h>


// f'(x) = (f(A + deltaX) – f(A))/deltaX
float Derivative(float A, float deltaX){
    return (cos(A + deltaX) - cos(A))/deltaX;
}
// Используется ряд Лейбница
float Pi(int K){
    float res = 0.0;
    float iter = 1.0;
    for(int i = 0; i < K; i++){
        res += pow(-1, i) * (4.0 / iter);
        iter+=2.0;
    }
    return res;
}
