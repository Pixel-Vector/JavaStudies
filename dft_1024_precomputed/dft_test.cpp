/*
This is a testbench for the *streaming* DFT.
It will:
1. Create hls::stream objects.
2. Write SIZE (1024) input samples to the input stream.
3. Call the streaming dft() function.
4. Read SIZE (1024) output samples from the output stream.
5. Compare the results with a golden file.

** WARNING: 'out.gold.dat' MUST be for SIZE=1024 **
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "dft.h" // This must be the streaming version of dft.h
#include "hls_stream.h" // Required for stream objects

struct Rmse
{
	int num_sq;
	float sum_sq;
	float error;

	Rmse(){ num_sq = 0; sum_sq = 0; error = 0; }

	float add_value(float d_n)
	{
		num_sq++;
		sum_sq += (d_n*d_n);
		error = sqrtf(sum_sq / num_sq);
		return error;
	}
};

Rmse rmse_R,  rmse_I;

// We still use arrays to hold the test data before streaming
// and to store the results after streaming.
DTYPE In_R[SIZE], In_I[SIZE], Out_R[SIZE], Out_I[SIZE];

int main()
{
	int index;
	float gold_R, gold_I;

    // --- Create the stream objects ---
    hls::stream<cplx_sample> stream_in("stream_in");
    hls::stream<cplx_sample> stream_out("stream_out");
    cplx_sample temp_sample;

    // --- CRITICAL WARNING ---
    // Make sure your 'out.gold.dat' file is for SIZE=1024.
	FILE * fp = fopen("out.gold.dat","r");
    if (!fp) {
        printf("ERROR: Could not open 'out.gold.dat'.\n");
        printf("Please make sure you have a golden file for SIZE=1024.\n");
        return 1;
    }

	// 1. Getting input data and writing to stream
	printf("Writing %d samples to input stream...\n", SIZE);
	for(int i = 0; i < SIZE; i++)
	{
		In_R[i] = i;
		In_I[i] = 0.0;

        // Pack data into struct
        temp_sample.real = In_R[i];
        temp_sample.imag = In_I[i];

        // Write to the input stream
        stream_in.write(temp_sample);
	}
	
	// 2. Call the streaming DFT function
    printf("Calling DFT function...\n");
	dft(stream_in, stream_out);

    // 3. Read from output stream and compare
    printf("Reading %d samples from output stream and comparing...\n", SIZE);
	for(int i = 0; i < SIZE; i++)
	{
        // Read from the output stream
        temp_sample = stream_out.read();

        // Store results
        Out_R[i] = temp_sample.real;
        Out_I[i] = temp_sample.imag;

        // Compare with golden output
		fscanf(fp, "%d %f %f", &index, &gold_R, &gold_I);
		rmse_R.add_value((float)Out_R[i] - gold_R);
		rmse_I.add_value((float)Out_I[i] - gold_I);
	}
	fclose(fp);

	// printing error results
	printf("----------------------------------------------\n");
	printf("   RMSE(R)           RMSE(I)\n");
	printf("%0.15f %0.15f\n", rmse_R.error, rmse_I.error);
	printf("----------------------------------------------\n");

	if (rmse_R.error > 0.1 || rmse_I.error > 0.1 ) {
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