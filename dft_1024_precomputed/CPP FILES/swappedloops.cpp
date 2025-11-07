#include "dft.h"
#include "coefficients1024.h" // Include the pre-computed tables
#include <math.h>


void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE], DTYPE real_op[SIZE], DTYPE imag_op[SIZE])
{
    // 1. Initialize output buffers to zero before accumulation
    // This is necessary because we now accumulate in the inner loop.
    for (int i = 0; i < SIZE; i++)
    {
        real_op[i] = 0;
        imag_op[i] = 0;
    }

    // 2. Iterate over each input time sample (n) - This is now the OUTER loop
    for (int n = 0; n < SIZE; n++)
    {
        // Fetch input samples once per outer loop
        DTYPE in_real = real_sample[n];
        DTYPE in_imag = imag_sample[n];

        // 3. Iterate over each output frequency bin (k) - This is now the INNER loop
        for (int k = 0; k < SIZE; k++)
        {
            // Calculate the index for the SIN/COS lookup tables
            // This corresponds to (k * n) in the angle calculation
            int index = (k * n) % SIZE;

            // Get the pre-computed twiddle factor components
            DTYPE cos_angle = cos_coefficients_table[index];
            DTYPE sin_angle = sin_coefficients_table[index];

            // Perform the DFT sum using the standard (-i) sign convention
            // This matches the formula:
            // (real + i*imag) * (cos - i*sin) = (real*cos + imag*sin) + i*(imag*cos - real*sin)

            // Calculate this 'n' sample's contribution to the 'k' bin
            DTYPE partial_real = in_real * cos_angle + in_imag * sin_angle;
            DTYPE partial_imag = in_imag * cos_angle + in_real * sin_angle;

            // Accumulate the partial sums into the output arrays
            // This reads and writes from real_op[k] and imag_op[k] in the inner loop
            real_op[k] += partial_real;
            imag_op[k] += partial_imag;
        }
    }
}