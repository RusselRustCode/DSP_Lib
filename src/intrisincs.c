#define _GNU_SOURCE
#include <riscv_vector.h>
#include <dsp.h>
#include <stdio.h>
#include <math.h>
size_t get_VLEN(){
    size_t vl = vsetvlmax_e32m1();
    return vl;
}

int dsp_vector_add_intrisincs(const dsp_vector_t* vector_a, const dsp_vector_t* vector_b, dsp_vector_t* out)
{
    if(vector_a->len != vector_b->len || vector_a->len != out->len){
        perror("Error in dsp_vector_add_intrinsics: Dimensions not same");
        return -1;
    }

    if(!vector_a->data || !vector_b->data){
        perror("Error in dsp_vector_add_intrisincs: vector data is null");
        return -2;
    }

    size_t n = vector_a->len;
    float* ptr_a = vector_a->data;
    float* ptr_b = vector_b->data;
    float* ptr_out = out->data;

    for(size_t vl; n > 0; n -= vl, ptr_a += vl, ptr_b += vl, ptr_out += vl){
        vl = vsetvl_e32m1(n);
        
        vfloat32m1_t va = vle32_v_f32m1(ptr_a, vl);
        vfloat32m1_t vb = vle32_v_f32m1(ptr_b, vl);

        vfloat32m1_t v_out = vfmul_vv_f32m1(va, vb, vl);

        vse32_v_f32m1(ptr_out, v_out, vl);
    
    }
    return 0;
}


int dsp_vector_mul_intrisincs(const dsp_vector_t* vector_a, const dsp_vector_t* vector_b, dsp_vector_t* out){
    if (vector_a->len != vector_b->len || vector_a->len != out->len){
        perror("Error: dsp_vector_mul_intrisincs shape doesnt fit");
        return -1;
    }

    if(!vector_a->data || !vector_b->data){
        perror("Erorr in dsp_vector_mul_intrisinsc: vector data is null");
        return -2;
    }
    size_t n = vector_a->len;
    float* ptr_a = vector_a->data;
    float* ptr_b = vector_b->data;
    float* ptr_out = out->data;

    for(size_t vl; n > 0; n -= vl, ptr_a += vl, ptr_b += vl, ptr_out += vl){
        vl = vsetvl_e32m1(n);

        vfloat32m1_t va = vle32_v_f32m1(ptr_a, vl);
        vfloat32m1_t vb = vle32_v_f32m1(ptr_b, vl);

        vfloat32m1_t v_out = vfmul_vv_f32m1(va, vb, vl);

        vse32_v_f32m1(ptr_out, v_out, vl);
    }
    return 0;
}

float dsp_vector_sum_intrisincs(const dsp_vector_t* vector){
    if(!vector || !vector->data){
        perror("Error in dsp_vector_sum_intrisincs");
        return -1.0;
    }

    size_t n = vector->len;
    float* ptr = vector->data;
    float sum = 0.0f;
    vfloat32m1_t v_acc = vfmv_v_f_f32m1(0.0f, vsetvl_e32m1(1));
    for(size_t vl; n > 0; n -= vl, ptr += vl){
        vl = vsetvl_e32m1(n);

        vfloat32m1_t va = vle32_v_f32m1(ptr, vl);

        v_acc = vfredusum_vs_f32m1_f32m1(v_acc, va, v_acc, vl);
        sum += vfmv_f_s_f32m1_f32(v_acc);
    }
    return sum;
}

float dsp_vector_energy_intrisincs(const dsp_vector_t* vector){
    if(!vector || !vector->data){
        perror("Error in dsp_vector_energy_intrisincs");
        return -1.0;
    }
    vfloat32m1_t v_acc = vfmv_v_f_f32m1(0.0f, vsetvl_e32m1(1));
    size_t n = vector->len;
    float* ptr = vector->data;
    float sum_square = 0.0f;
    for(size_t vl; n > 0; n -= vl, ptr += vl){
        vl = vsetvl_e32m1(n);

        vfloat32m1_t vector = vle32_v_f32m1(ptr, vl);
        vfloat32m1_t square_v = vfmul_vv_f32m1(vector, vector, vl);

        v_acc = vfredusum_vs_f32m1_f32m1(v_acc, square_v, v_acc, vl);
        sum_square += vfmv_f_s_f32m1_f32(v_acc);
    }
    return sum_square;
}

static float* cos_table = NULL;
static float* sin_table = NULL;
static float table_size = 0;

void dsp_dft_init(size_t n){
    if(n == table_size)
        return;
    free(cos_table); free(sin_table);

    cos_table = malloc(sizeof(float) * n * n);
    sin_table = malloc(sizeof(float) * n * n);
    table_size = n;
    for(int k = 0; k < n; k++){
        for(int i = 0; i < n; i++){
            cos_table[k * n + i] = cosf(2 * M_PI * k * i / n);
            sin_table[k * n + 1] = sinf(2 * M_PI * k * i / n); 
        }
    }
}

int dsp_dft_intrisincs(const dsp_vector_t* input, dsp_complex_numbers* output, size_t size){
    if(!input || !input->data){
        perror("Error in dsp_dft_intrisincs");
        return -1;
    }

    dsp_dft_init(size);
    float* ptr = input->data;
    for(int k = 0; k < size; k++){
        float real = 0.0f;
        float imag = 0.0f;
        size_t n = size;
        for(size_t vl; n > 0; n -= vl, ptr += vl){
            size_t vl = vsetvl_e32m1(size);

            vfloat32m1_t v_input = vle32_v_f32m1(ptr, vl);
            vfloat32m1_t v_cos = vle32_v_f32m1(&cos_table[k * size + (ptr - input->data)], vl);
            vfloat32m1_t v_sin = vle32_v_f32m1(&sin_table[k * size + (ptr - input->data)], vl);

            vfloat32m1_t v_real = vfmul_vv_f32m1(v_input, v_cos, vl);
            vfloat32m1_t v_imag = vfmul_vv_f32m1(v_input, v_sin, vl);

            real += vfredusum_vs_f32m1_f32(v_real, 0.0f, vl);
            imag += vfredusum_vs_f32m1_f32(v_imag, 0.0f, vl);
            
        }

        output[k].real = real;
        output[k].imag = imag;
    }
}