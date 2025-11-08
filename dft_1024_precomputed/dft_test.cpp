#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "dft.h"

// Define the RMSE struct for error checking
struct Rmse {
    int num_sq;
    float sum_sq;
    float error;

    Rmse() { num_sq = 0; sum_sq = 0; error = 0; }

    float add_value(float d_n) {
        num_sq++;
        sum_sq += (d_n * d_n);
        error = sqrtf(sum_sq / num_sq);
        return error;
    }
};

Rmse rmse_R, rmse_I;

int main() {
    int index;
    float gold_R, gold_I;

    // Use the HLS stream types for testing
    hls::stream<axi_cplx_sample> stream_in("stream_in");
    hls::stream<axi_cplx_sample> stream_out("stream_out");

    FILE *fp = fopen("out.gold.dat", "r");
    if (!fp) {
        fprintf(stderr, "ERROR: could not open out.gold.dat\n");
        return 1;
    }

    // --- 1. PREPARE INPUT STREAM ---
    printf("Writing %d samples to input stream...\n", SIZE);
    for (int i = 0; i < SIZE; i++) {
        // Use the converter to pack floats
        cplx_data_converter converter;
        converter.f[0] = (DTYPE)i;   // real = i
        converter.f[1] = 0.0;        // imag = 0.0
        
        // Create the input packet
        axi_cplx_sample pkt_in;
        pkt_in.data = converter.u64;
        
        // Set .last on the final input sample
        pkt_in.last = (i == SIZE - 1);
        
        // Set other signals (good practice)
        pkt_in.keep = -1; // All 8 bytes valid
        pkt_in.strb = -1; // All 8 bytes valid
        
        stream_in.write(pkt_in);
    }

    // --- 2. CALL THE DFT FUNCTION ---
    printf("Calling DFT function...\n");
    dft(stream_in, stream_out);

    // --- 3. CHECK THE OUTPUT STREAM ---
    printf("Reading %d samples from output stream and comparing...\n", SIZE);
    for (int i = 0; i < SIZE; i++) {
        axi_cplx_sample pkt_out = stream_out.read();
        
        // Use the union to unpack the floats
        cplx_data_converter converter;
        converter.u64 = pkt_out.data;
        
        DTYPE out_R = converter.f[0];
        DTYPE out_I = converter.f[1];

        // Check golden output
        fscanf(fp, "%d %f %f", &index, &gold_R, &gold_I);
        rmse_R.add_value((float)out_R - gold_R);
        rmse_I.add_value((float)out_I - gold_I);

        // Check that TLAST is correct
        if (i == SIZE - 1) {
            if (pkt_out.last != 1) {
                fprintf(stderr, "ERROR: TLAST was not set on the last output sample\n");
                return 1; // Fail the test
            }
        } else {
            if (pkt_out.last != 0) {
                fprintf(stderr, "ERROR: TLAST was set prematurely on sample %d\n", i);
                return 1; // Fail the test
            }
        }
    }
    fclose(fp);

    // --- 4. PRINT RESULTS ---
    printf("----------------------------------------------\n");
    printf("   RMSE(R)           RMSE(I)\n");
    printf("%0.15f %0.15f\n", rmse_R.error, rmse_I.error);
    printf("----------------------------------------------\n");

    if (rmse_R.error > 0.1 || rmse_I.error > 0.1) {
        fprintf(stdout, "*******************************************\n");
        fprintf(stdout, "FAIL: Output DOES NOT match the golden output\n");
        fprintf(stdout, "*******************************************\n");
        return 1;
    } else {
        fprintf(stdout, "*******************************************\n");
        fprintf(stdout, "PASS: The output matches the golden output!\n");
        fprintf(stdout, "*******************************************\n");
        return 0;
    }
}
