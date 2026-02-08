#include <dsp.h>


int dsp_fir_init(dsp_fir_filter_t* fir, const float* values, size_t n_taps){
    if(fir == NULL){
        return 1;
    }
    fir->taps = malloc(sizeof(float) * n_taps);
    if(fir->taps == NULL){
        perror("Ошибка выделения памяти для FIR");
        return -1;
    }
    for(int i=0; i < n_taps; i++){
        fir->taps[i] = values[i];
    }
    fir->n_taps = n_taps;
    return 0;
}

int dsp_fir_process(const dsp_fir_filter_t* fir, const dsp_vector_t* vector, dsp_vector_t* out){
    float counter = 0.0f;
    if(fir == NULL || vector == NULL || out == NULL){
        perror("Не инициализированы вектора");
        return -1;
    }
    for(int i=0; i < vector->len; i++){
        if(i < fir->n_taps){
            continue; // пока что пропускаем, дальше подумаем че делать
        }
        for(int j=0; j < fir->n_taps; j++){
            counter += vector->data[i - j] * fir->taps[j];        
        }
        out->data[i] = counter;
    }
    return 0;
}

void dsp_fir_destroy(dsp_fir_filter_t* fir){
    if(fir != NULL && fir->taps != NULL){
        free(fir->taps);
        fir->taps = NULL;
        fir->n_taps = 0;
    }
}