#include "dft.h"
#include <math.h>

// Define M_PI if it's not already defined by math.h
// M_PI is a common, but not strictly standard, C++ constant
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * @brief Performs a 1024-point Discrete Fourier Transform (DFT).
 * * This function calculates the DFT based on the formula:
 * X[k] = SUM_n=0..N-1 [ x[n] * (cos(2*pi*k*n/N) - i*sin(2*pi*k*n/N)) ]
 * * Where:
 * x[n] = real_sample[n] + i*imag_sample[n]
 * X[k] = real_op[k] + i*imag_op[k]
 * N = SIZE (1024)
 * * Expanding the complex multiplication gives:
 * real_op[k] = SUM [ real_sample[n]*cos(angle) + imag_sample[n]*sin(angle) ]
 * imag_op[k] = SUM [ imag_sample[n]*cos(angle) - real_sample[n]*sin(angle) ]
 * (where angle = 2*pi*k*n / N)
 * * @param real_sample Input array of real components of time-domain signal.
 * @param imag_sample Input array of imaginary components of time-domain signal.
 * @param real_op     Output array for real components of frequency-domain signal.
 * @param imag_op     Output array for imaginary components of frequency-domain signal.
 */
void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE],DTYPE real_op[SIZE], DTYPE imag_op[SIZE])
{
    // Iterate over each output frequency bin (k)
    for (int k = 0; k < SIZE; k++)
    {
        DTYPE sum_real = 0;
        DTYPE sum_imag = 0;

        // Iterate over each input time sample (n)
        for (int n = 0; n < SIZE; n++)
        {
            // Calculate the angle for the twiddle factor
            // angle = (2 * pi * k * n) / N
            DTYPE angle =(DTYPE) (2 * M_PI * k * n) / SIZE;

            // Calculate the cosine and sine components
            double cos_angle = cos(angle);
            double sin_angle = sin(angle);

            // Perform the DFT sum for this (k, n) pair
            // Accumulate the real part
            sum_real += real_sample[n] * cos_angle + imag_sample[n] * sin_angle;

            // Accumulate the imaginary part
            sum_imag += imag_sample[n] * cos_angle - real_sample[n] * sin_angle;
        }

        // Assign the final sum to the output arrays for frequency bin k
        real_op[k] = sum_real;
        imag_op[k] = sum_imag;
    }
}