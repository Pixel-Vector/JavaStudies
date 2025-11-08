#pragma once

#include <ap_axi_sdata.h> // Include this for ap_axis
#include "hls_stream.h"
#include <ap_int.h>

typedef float DTYPE;
#define SIZE 1024
#define ACCUM_FACTOR 8

// Define a union to easily convert between two floats and a 64-bit integer
// This is the cleanest way to map our data to the ap_axis .data field
typedef union {
    uint64_t u64;    // 64-bit integer view
    DTYPE    f[2];   // Array of 2 floats view
} cplx_data_converter;

// Define the AXI-Stream packet type
// It has a 64-bit data payload (for 2 floats)
// It has no TUSER, TSTRB, or TID (all set to 0)
typedef ap_axis<64, 0, 0, 0> axi_cplx_sample;

// The function signature now uses this new AXI stream type
void dft(
    hls::stream<axi_cplx_sample>& stream_in,
    hls::stream<axi_cplx_sample>& stream_out
);
