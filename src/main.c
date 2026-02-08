#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <dsp.h>

int main() {
    printf("Starting FFT test...\n");

    float input_data[] = {1.0f, 0.0f, 0.0f, 0.0f};
    dsp_complex_numbers fft_input[4];
    for (int i = 0; i < 4; i++) {
        fft_input[i].real = input_data[i];
        fft_input[i].imag = 0.0f;
    }

    dsp_complex_numbers* output = dsp_vector_fft(fft_input, 4);
    if (!output) {
        fprintf(stderr, "FFT failed!\n");
        return 1;
    }

    printf("Results:\n");
    for (int k = 0; k < 4; k++) {
        printf("X[%d] = %+.6f %+.6fj\n", k, output[k].real, output[k].imag);
    }

    free(output);
    printf("Done.\n");
    return 0;
}





