#include <riscv_vector.h>
#include <dsp.h>


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

        vfloat32m1_t v_out = vfmul_vv_f32m1(ptr_a, ptr_b, vl);

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

        vfloat32m1_t v_out = vfmul_vv_f32m1(ptr_a, ptr_b, vl);

        vse32_v_f32m1(ptr_out, v_out, vl);
    }
    return 0;
}

float dsp_vector_sum_intrisincs(const dsp_vector_t* vector){
    if(!vector || !vector->data){
        perror("Error in dsp_vector_sum_intrisincs");
        return -1,0;
    }

    size_t n = vector->len;
    float* ptr = vector->data;
    float sum = 0.0f;
    vfloat32m1_t v_acc = vfmv_v_f_f32m1(0.0f, vsetvl_e32m1(1));
    for(size_t vl; n > 0; n -= vl, ptr += vl){
        vl = vsetvl_e32m1(n);

        vfloat32m1_t va = vle32_v_f32m1(ptr, vl);

        v_acc = vfredusum_vs_f32m1_f32m1(va, v_acc, vl);
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

        v_acc = vfredusum_vs_f32m1_f32m1(square_v, v_acc, vl);
        sum_square += vfmv_f_s_f32m1_f32(v_acc);
    }
    return sum_square;
}