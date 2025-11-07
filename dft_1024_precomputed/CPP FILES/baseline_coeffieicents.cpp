#include "dft.h"
#include "coefficients1024.h" // Include the pre-computed tables
#include <math.h>


void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE], DTYPE real_op[SIZE], DTYPE imag_op[SIZE])
{
    // Iterate over each output frequency bin (k)
    for (int k = 0; k < SIZE; k++)
    {
        DTYPE sum_real = 0;
        DTYPE sum_imag = 0;

        // Iterate over each input time sample (n)
        for (int n = 0; n < SIZE; n++)
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

            // Accumulate the real part
            sum_real += real_sample[n] * cos_angle + imag_sample[n] * sin_angle;

            // Accumulate the imaginary part
            sum_imag += imag_sample[n] * cos_angle + real_sample[n] * sin_angle;
        }

        // Assign the final sum to the output arrays for frequency bin k
        real_op[k] = sum_real;
        imag_op[k] = sum_imag;
    }
}