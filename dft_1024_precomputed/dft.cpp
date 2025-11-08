#include <math.h>
#include "dft.h"
#include "coefficients1024.h"

void dft(
    hls::stream<axi_cplx_sample>& stream_in,
    hls::stream<axi_cplx_sample>& stream_out
)
{
    // --- AXI-Stream Interface Pragmas ---
    #pragma HLS INTERFACE axis port=stream_in
    #pragma HLS INTERFACE axis port=stream_out
    #pragma HLS INTERFACE s_axilite port=return bundle=control

    int k, n;

    // --- 1. BUFFER ---
    // These BRAMs store the 1024 input samples
    DTYPE local_real[SIZE];
    DTYPE local_imag[SIZE];

    // Read the input stream and fill the local buffers
    READ_INPUT_LOOP:
    for (int i = 0; i < SIZE; i++) {
        #pragma HLS PIPELINE II=1
        
        // Read the full AXI packet
        axi_cplx_sample temp_in_pkt = stream_in.read();
        
        // Use the union to convert the 64-bit data
        cplx_data_converter converter;
        converter.u64 = temp_in_pkt.data;
        
        // Unpack the two floats
        local_real[i] = converter.f[0];
        local_imag[i] = converter.f[1];
        
        // We don't need to check temp_in_pkt.last,
        // as our design assumes a fixed-size frame of SIZE
    }

    // --- 2. COMPUTE ---
    DFT_OUTER_LOOP:
    for (k = 0; k < SIZE; k++) {
        // Create 8 parallel accumulators
        DTYPE sum_real_local[ACCUM_FACTOR] = {0};
        DTYPE sum_imag_local[ACCUM_FACTOR] = {0};

        // Partition accumulators into individual registers
        #pragma HLS ARRAY_PARTITION variable=sum_real_local complete
        #pragma HLS ARRAY_PARTITION variable=sum_imag_local complete

        int phase = 0;

        DFT_INNER_LOOP:
        for (n = 0; n < SIZE; n++) {
            #pragma HLS PIPELINE II=1

            // Allocate hardware for parallel operations
            #pragma HLS ALLOCATION operation instances=fmul limit=4
            #pragma HLS ALLOCATION operation instances=fadd limit=2

            int index = phase;

            DTYPE C = cos_coefficients_table[index];
            DTYPE S = sin_coefficients_table[index];
            
            DTYPE r_in = local_real[n];
            DTYPE i_in = local_imag[n];

            // Perform the complex multiply-accumulate
            DTYPE r_comp = (r_in * C - i_in * S);
            DTYPE i_comp = (i_in * C + r_in * S);

            int accum_idx = n % ACCUM_FACTOR;
            sum_real_local[accum_idx] += r_comp;
            sum_imag_local[accum_idx] += i_comp;

            phase += k;
            if (phase >= SIZE) {
                phase -= SIZE;
            }
        }

        // --- FINAL SUM LOOP ---
        // Sum the 8 parallel accumulators
        DTYPE final_sum_real = 0;
        DTYPE final_sum_imag = 0;
        SUM_LOOP:
        for (int i = 0; i < ACCUM_FACTOR; i++) {
            #pragma HLS PIPELINE
            final_sum_real += sum_real_local[i];
            final_sum_imag += sum_imag_local[i];
        }

        // --- 3. STREAM OUT ---
        
        // Use the union to pack the two floats into 64 bits
        cplx_data_converter converter;
        converter.f[0] = final_sum_real;
        converter.f[1] = final_sum_imag;
        
        // Create the output AXI packet
        axi_cplx_sample result_out_pkt;
        result_out_pkt.data = converter.u64;
        
        // --- THIS IS THE TLAST FIX ---
        // Explicitly set the .last signal
        // It is 1 (true) if this is the last sample (k=1023)
        // Otherwise, it is 0 (false)
        result_out_pkt.last = (k == SIZE - 1);
        // -----------------------------
        
        // Set other AXI signals to 0 (default)
        result_out_pkt.keep = -1; // -1 means all 8 bytes are valid
        result_out_pkt.strb = -1; // -1 means all 8 bytes are valid
        result_out_pkt.user = 0;
        result_out_pkt.id   = 0;
        result_out_pkt.dest = 0;
        
        stream_out.write(result_out_pkt);
    }
}
