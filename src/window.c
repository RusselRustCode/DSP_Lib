#define _GNU_SOURCE // Нужно чтобы использовать константы math.h
#include <dsp.h>
#include <math.h>
int dsp_window_hann(dsp_vector_t* vector){
    if(vector == NULL || vector->len < 2){
        perror("Ошибка, vector равен NULL или длина меньше 2");
        return -1;
    }
    for(int i=0; i < vector->len; i++){
        //w(n) = 0.5 - 0.5 * cos(2 * pi * n / N - 1)
        //cosf - работает быстрее и специально предназначена для float
        vector->data[i] *= (0.5 - 0.5 * cosf((2 * M_PI * i)/ (vector->len - 1)));
    }
    return 0;
}

int dsp_window_hamming(dsp_vector_t* vector){
    if(vector == NULL || vector->len < 2){
        perror("Ошибка, vector равен NULL или длина меньше 2");
        return -1;
    }
    for(int i=0; i < vector->len; i++){
        //w(n) = 0.54 - 0.46 * cos(2 * pi * n / N - 1)
        vector->data[i] *= (0.54 - 0.46 * cosf((2 * M_PI * i) / (vector->len - 1)));
    }
    return 0;
}

int dsp_window_blackmann(dsp_vector_t* vector){
    if(vector == NULL || vector->len < 2){
        perror("Ошибка, vector равен NULL или длина меньше 2");
        return -1;
    }
    float a0 = 0.42f;
    float a1 = 0.5f;
    float a2 = 0.08f;
    for(int i = 0; i < vector->len; i++){
        //w(n) = a0 - a1 * cos(2 * pi * n / N - 1) + a2 * cos(4 * pi * n / N - 1)
        vector->data[i] *= (a0 - a1 * cosf((2 * M_PI * i) / (vector->len - 1)) + a2 * cosf((4 * M_PI * i) / (vector->len - 1)));
    }
    return 0;
}