#include <math.h>
#include "dft.h"
#include "coefficients1024.h"

void dft(
    hls::stream<cplx_sample>& stream_in,
    hls::stream<cplx_sample>& stream_out
)
{
    // --- AXI-Stream Interface Pragmas ---
    // These directives tell HLS to create AXI-Stream ports
    // for our `hls::stream` arguments.
    #pragma HLS INTERFACE axis port=stream_in
    #pragma HLS INTERFACE axis port=stream_out
    
    // Standard pragma to bundle control signals into a separate AXI-Lite interface
    #pragma HLS INTERFACE s_axilite port=return bundle=control

    int k, n;

    // --- 1. BUFFER ---
    // Create local arrays to store the input frame.
    // For SIZE=1024, HLS will synthesize these as BRAMs.
    DTYPE local_real[SIZE];
    DTYPE local_imag[SIZE];

    // --- Optimization: Partitioning ---
    // For SIZE=1024, we CANNOT partition 'complete'.
    // We remove the pragma and let HLS infer two-port BRAMs (RAM_T2P),
    // which is the correct implementation for a buffer this large.
    // #pragma HLS ARRAY_PARTITION variable=local_real complete (REMOVED)
    // #pragma HLS ARRAY_PARTITION variable=local_imag complete (REMOVED)

    // Read the input stream and fill the local buffers
    READ_INPUT_LOOP:
    for (int i = 0; i < SIZE; i++) {
        // This loop will run for SIZE cycles
        #pragma HLS PIPELINE II=1
        
        // Read one complex sample from the input stream
        cplx_sample temp_in = stream_in.read();
        
        // Store in our local buffers (BRAMs)
        local_real[i] = temp_in.real;
        local_imag[i] = temp_in.imag;
    }

    // --- 2. COMPUTE ---
    // This is your original, optimized DFT computation.
    // The *only* change is reading from `local_real`/`local_imag`
    // instead of `real_sample`/`imag_sample`.

    // Loop over each output frequency bin (k)
    DFT_OUTER_LOOP:
    for (k = 0; k < SIZE; k++) {
        // Create 8 independent accumulators to break the dependency chain.
        DTYPE sum_real_local[ACCUM_FACTOR] = {0};
        DTYPE sum_imag_local[ACCUM_FACTOR] = {0};

        //Partition 'complete' to ensure they are 8 separate registers.
        #pragma HLS ARRAY_PARTITION variable=sum_real_local complete
        #pragma HLS ARRAY_PARTITION variable=sum_imag_local complete

        //Use the fast, efficient phase accumulator
        int phase = 0;

        // Loop over each input time-domain sample (n)
        DFT_INNER_LOOP:
        for (n = 0; n < SIZE; n++) {
            #pragma HLS PIPELINE II=1

            // These allocation pragmas are still good
            #pragma HLS ALLOCATION instances=fmul limit=4 operation
            #pragma HLS ALLOCATION instances=fadd limit=2 operation

            int index = phase;

            DTYPE C = cos_coefficients_table[index];
            DTYPE S = sin_coefficients_table[index];
            
            // --- *** KEY CHANGE *** ---
            // Read from our local, partitioned buffers (now BRAMs)
            DTYPE r_in = local_real[n];
            DTYPE i_in = local_imag[n];
            // --- *** END KEY CHANGE *** ---

            //Math corrected for your inverted sine table
            DTYPE r_comp = (r_in * C - i_in * S);
            DTYPE i_comp = (i_in * C + r_in * S);

            //Add to a different local accumulator each cycle
            int accum_idx = n % ACCUM_FACTOR;
            sum_real_local[accum_idx] += r_comp;
            sum_imag_local[accum_idx] += i_comp;

            // Update the efficient phase accumulator
            phase += k;
            if (phase >= SIZE) {
                phase -= SIZE;
            }
        }

        // --- FINAL SUM LOOP ---
        DTYPE final_sum_real = 0;
        DTYPE final_sum_imag = 0;
        SUM_LOOP:
        for (int i = 0; i < ACCUM_FACTOR; i++) {
            #pragma HLS PIPELINE
            final_sum_real += sum_real_local[i];
            final_sum_imag += sum_imag_local[i];
        }

        // --- 3. STREAM OUT ---
        // Instead of writing to an array, create a complex sample
        // and write it to the output stream.
        cplx_sample result_out;
        result_out.real = final_sum_real;
        result_out.imag = final_sum_imag;
        
        stream_out.write(result_out);
    }
}