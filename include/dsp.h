#ifndef DSP_H
#define DSP_H
#include <stdlib.h>


typedef struct{
    float* data;
    size_t len;
} dsp_vector_t;

typedef struct {
    float* taps;
    size_t n_taps;
} dsp_fir_filter_t;

typedef struct {
    float real, imag;
} dsp_complex_numbers;

int dsp_vector_alloc(dsp_vector_t* vector, size_t len);//выделяем память 
void dsp_vector_free(dsp_vector_t* vector);//освобождаем память 
int dsp_vector_copy(const dsp_vector_t* src, dsp_vector_t* dsp);//копируем вектор
int dsp_vector_fill(dsp_vector_t* vector, float value);//заполняем вектор
int dsp_vector_add(const dsp_vector_t* vector_a, const dsp_vector_t* vector_b, dsp_vector_t* out);//поэлементная сумма
int dsp_vector_mul(const dsp_vector_t* vector_a, const dsp_vector_t* vector_b, dsp_vector_t* out); //поэлементное умноженние
float dsp_vector_sum(const dsp_vector_t* vector); //сумма элементов
float dsp_vector_energy(const dsp_vector_t* vector); //сумма квадратов


//FFT 
int dsp_window_hann(dsp_vector_t* vector);
int dsp_window_hamming(dsp_vector_t* vector);
int dsp_window_blackman(dsp_vector_t* vector);
int dsp_dft(const dsp_vector_t* input, dsp_complex_numbers* out, size_t size);
dsp_complex_numbers* dsp_vector_fft(const dsp_complex_numbers* data, size_t size);
dsp_complex_numbers* dsp_ifft(dsp_complex_numbers* input, size_t size);
int dsp_idft(const dsp_vector_t* input, dsp_complex_numbers* out, size_t size);

int dsp_spectrum(const dsp_vector_t* signal, dsp_vector_t* magnitude); // |FFT|
int dsp_spectrogram(const dsp_vector_t* signal, size_t window_size, size_t hop, float** out);


//Z
int dsp_z_transform(dsp_vector_t* vector);


//FIR
int dsp_fir_init(dsp_fir_filter_t* fir, const float* values, size_t n_taps);
int dsp_fir_process(const dsp_fir_filter_t* fir, const dsp_vector_t* vector, dsp_vector_t* out);
void dsp_fir_destroy(dsp_fir_filter_t* fir);

dsp_complex_numbers complex_add(dsp_complex_numbers a, dsp_complex_numbers b);
dsp_complex_numbers complex_sub(dsp_complex_numbers a, dsp_complex_numbers b);
dsp_complex_numbers complex_mul(dsp_complex_numbers a, dsp_complex_numbers b);


#endif


