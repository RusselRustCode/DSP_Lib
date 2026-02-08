#define _USE_MATH_DEFINES
#include <dsp.h>
#include <math.h>
#include <stdio.h>
int dsp_dft(const dsp_vector_t* input, dsp_complex_numbers* out, size_t n){
    if(input == NULL || out == NULL){
        return -1;
    }

    for(int k = 0; k < input->len; k++){
        out[k].imag = 0.0f;
        out[k].real = 0.0f;
        for(int i = 0; i < input->len; i++){
            out[k].real += input->data[i] * cosf(2 * M_PI * (float)(k * i) / (float)input->len);
            out[k].imag -= input->data[i] * sinf(2 * M_PI * (float)(k * i) / (float)input->len);
        }
    }
    return 0;
}


dsp_complex_numbers* dsp_vector_fft(const dsp_complex_numbers* data, size_t size){
    if(size == 0 || (size & (size - 1)) != 0){
        perror("Ошибка: Не является степенью двойки!");
        return NULL;
    }
    if (size == 1) {
        dsp_complex_numbers* out = malloc(sizeof(dsp_complex_numbers));
        if (out) {
            out[0] = data[0];
        }
        return out; // может быть NULL, если malloc failed
    }
    size_t half_size = size / 2;
    dsp_complex_numbers* even_orig = malloc(half_size * sizeof(dsp_complex_numbers));
    dsp_complex_numbers* odd_orig = malloc(half_size * sizeof(dsp_complex_numbers));

    if(!even_orig || !odd_orig){
        free(even_orig);
        free(odd_orig);
        return NULL;
    }


    for (int i=0; i < half_size; i++){
        even_orig[i] = data[2 * i];
        odd_orig[i] = data[2 * i + 1];
    }

    dsp_complex_numbers* even_fft = dsp_vector_fft(even_orig, half_size);
    dsp_complex_numbers* odd_fft = dsp_vector_fft(odd_orig, half_size); 

    free(even_orig);
    free(odd_orig);

    dsp_complex_numbers* output = malloc(size * sizeof(dsp_complex_numbers));
    for(int k = 0; k < half_size; k++){
        dsp_complex_numbers twiddleFactor = {
            cosf(2 * M_PI * k / size),
            sinf(2 * M_PI * k / size)
        };
        dsp_complex_numbers twiddleOdd = complex_mul(odd_fft[k], twiddleFactor);
        output[k] = complex_add(even_fft[k], twiddleOdd);
        output[k + half_size] = complex_sub(even_fft[k], twiddleOdd);
    }
    free(even_fft);
    free(odd_fft);
    return output;

}

int dsp_ifft(dsp_complex_numbers* input, dsp_complex_numbers* out, size_t n){
    return 0;
}

int dsp_idft(const dsp_vector_t* input, dsp_complex_numbers* out, size_t n){
    return 0;
}

dsp_complex_numbers complex_sub(dsp_complex_numbers a, dsp_complex_numbers b){
    dsp_complex_numbers out = {
        a.real - b.real, 
        a.imag - b.imag
    };
    return out;
}

dsp_complex_numbers complex_add(dsp_complex_numbers a, dsp_complex_numbers b){
    dsp_complex_numbers out = {
        a.real + b.real, 
        a.imag + b.imag
    };
    return out;
}

dsp_complex_numbers complex_mul(dsp_complex_numbers a, dsp_complex_numbers b){
    dsp_complex_numbers out = {
        a.real * b.real - a.imag * b.imag,
        a.real * b.imag + a.imag * b.real
    };    

    return out;
}

