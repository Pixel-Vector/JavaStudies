#ifndef _DFT_H_
#define _DFT_H_

#include "hls_stream.h"
#include <ap_fixed.h> // Include for ap_fixed if you use it

// ----------------------------------------------------------------
// Config
// ----------------------------------------------------------------

// Make sure SIZE and DTYPE are defined here
// This MUST match the design in dft_streaming.cpp
#define SIZE 1024

// Using float for DTYPE as in your original math.h include
typedef float DTYPE;
// Example for fixed-point:
// typedef ap_fixed<32, 10> DTYPE;

// Number of parallel accumulators (used in .cpp)
#define ACCUM_FACTOR 8

// ----------------------------------------------------------------
// AXI-Stream Data Type Definition
// ----------------------------------------------------------------

// A struct to hold one complex sample (real + imag).
// This will be the data type on our AXI-Stream.
struct cplx_sample {
    DTYPE real;
    DTYPE imag;
};

// ----------------------------------------------------------------
// Function Prototype
// ----------------------------------------------------------------

// The top-level function signature MUST change to use hls::stream
void dft(
    hls::stream<cplx_sample>& stream_in,
    hls::stream<cplx_sample>& stream_out
);

#endif // _DFT_H_