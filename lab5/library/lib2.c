#include "lib2.h"

// f'(x) = (f(A + deltaX) – f(A-deltaX))/(2*deltaX)
float Derivative(float A, float deltaX){
    return (cos(A + deltaX) - cos(A - deltaX))/(2 * deltaX);
}
// Используется улучшенная версия формулы Валлиса с увеличенной скоростью сходимости
float Pi(int K){
    float res = 0.0;
    float i;
    for (i = 1.0; i < K; i++){
        res *= 2*i*2*i / ((2 * i - 1) * (2 * i + 1));
    }
    res *= 2*i/(2*i - 1) * (2*i/(2*i + 1) * 1/4 + 1) + 3/4;
    return res;
}

