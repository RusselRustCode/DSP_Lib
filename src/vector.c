#include <dsp.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
int dsp_vector_alloc(dsp_vector_t* vector, size_t len){
    if(vector == NULL){
        return 1;
    }

    vector->data = (float*)malloc(sizeof(float) * len);
    if(vector->data == NULL){
        perror("DSP ERROR: Failed to allocate memory");
        return 1;
    }
    vector->len = len;
    return 0;
}

void dsp_vector_free(dsp_vector_t* ptr){
    if(ptr != NULL && ptr->data != NULL){
        free(ptr->data);
        ptr->data = NULL;
        ptr->len = 0;
    }
}

int dsp_vector_fill(dsp_vector_t* ptr, float value){
    size_t new_cap = (ptr->len == 0) ? 1 : ptr->len * 2;
    float* new_data = realloc(ptr->data, new_cap * sizeof(float));

    if(!new_data){
        perror("DSP ERROR: Failed to allocate memory");
        return 1;
    }

    ptr->data = new_data;
    ptr->len++;

    ptr->data[ptr->len - 1] = value;
    return 0; 
}

int dsp_vector_copy(const dsp_vector_t* src, dsp_vector_t* dsp){
    memcpy(dsp->data, src->data, src->len * sizeof(float)); //Нужно передавать размер в байтах, поэтому просто len не пойдет
    dsp->len = src->len;
    return 0;
}

int dsp_vector_add(const dsp_vector_t* vector_a, const dsp_vector_t* vector_b, dsp_vector_t* out){
    if(vector_a->len != vector_b->len || vector_a->len != out->len){
        perror("Несоответсвие размеров векторов");
        return 1;
    }   
    for(int i=0; i < vector_a->len; i++){
        out->data[i] = vector_a->data[i] + vector_b->data[i];
    } 
    return 0;
}

int dsp_vector_mul(const dsp_vector_t* vector_a, const dsp_vector_t* vector_b, dsp_vector_t* out){
    if(vector_a->len != vector_b->len || vector_a->len != out->len){
        perror("Несоответсвие размеров векторов");
        return 1;
    }   
    for(int i=0; i < vector_a->len; i++){
        out->data[i] = vector_a->data[i] * vector_b->data[i];
    } 
    return 0;    
}

float dsp_vector_sum(const dsp_vector_t* vector){
    float sum = 0.0f;
    for(int i=0; i < vector->len; i++){
        sum += vector->data[i];
    }
    return sum;
}

float dsp_vector_energy(const dsp_vector_t* vector){
    float energy = 0.0f; //Нужно сразу инициализировать, т.к. без init там будет мусор
    for(int i=0; i < vector->len; i++){
        energy += vector->data[i] * vector->data[i]; // x * x быстрее чем pow
    }
    return energy;
}


int dsp_z_transform(dsp_vector_t* vector){
    return 0;
}