#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <c6x.h>
#include "wavpcm_io.h"
#include "globals.h"
#include "functions.h"
#include "codec.h"
#include "decoder.h"

// z contains the quantized samples for the four subbands
// decoderChunk is the chunk variable containing d_prime, s_star and stepsize for each subband
// s_prime contains the dequantized samples for the four subbands
void decoder (short z[MONOSIZE3BANDS], struct chunkDecoder *decoderChunk, short s_prime[MONOSIZE3BANDS]){

	long sum, power, carry, b;
	int n1, n2, n3, n5, n6, n7, i, j, k;//, n4, n8, l;
    #pragma MUST_ITERATE(5,5);
    #pragma UNROLL(5)
	for (n1=0; n1 < SIZE2; n1++) {
		// Dequantize the difference
		decoderChunk->d_prime2[n1+windowsize2dec]=decoderChunk->stepsize2[n1]*z[n1];

		// Sum the dequantized difference and the predicted sample
		s_prime[n1]=decoderChunk->d_prime2[n1+windowsize2dec] + decoderChunk->s_star2[n1];

		// Predict the sample
		//decoderChunk->s_star2[n1+1]=mu2*s_prime[n1];
		decoderChunk->s_star2[n1+1]=s_prime[n1] << mu2pow;

		// Rescale the sample to stay under 40 bits
		//decoderChunk->s_star2[n1+1]=rescale(decoderChunk->s_star2[n1+1], 15);
		b = decoderChunk->s_star2[n1+1] >> 14;
		carry = b & 1;
		b = ((b + carry) >> 1);
		decoderChunk->s_star2[n1+1] = b;

		// Calculate the sum of the absolute values of the dequantized differences
		sum=0;
		#pragma MUST_ITERATE(8,windowsize2);
		#pragma UNROLL(8)
		for (i=n1; i < n1+windowsize2; i++) {
			if (decoderChunk->d_prime2[i] > 0) {
				sum=sum+decoderChunk->d_prime2[i];
			}
			else {
				sum=sum-decoderChunk->d_prime2[i];
			}
			//sum=sum+_labs(decoderChunk->d_prime2[i]);

			//step = decoderChunk->d_prime2[i];
			//sum=sum + (step >> 31)*step + (!(step >> 31))*step;
		}

		// Divide the sum by the number of samples in the sum to get the mean
		//power=phi2*sum;
		power=sum << phi2pow;
		//power=divideRound(windowsize2, power);
		/*b = power >> windowsize2pow;
		carry = b & 1;
		b = ((b + carry) >> 1);
		power = b;*/
		power = power/windowsize2;

		// Calculate the stepsize and rescale it to stay under 40 bits
		//decoderChunk->stepsize2[n1+1]=rescale(power, 15);
		b = power >> 14;
		carry = b & 1;
		b = ((b + carry) >> 1);
		decoderChunk->stepsize2[n1+1] = b;

		// Limit the stepsize
		/*if (decoderChunk->stepsize2[n1+1] > max_step2) {	//This is never true
			decoderChunk->stepsize2[n1+1]=max_step2;
		}
		else*/ if (decoderChunk->stepsize2[n1+1] < min_step) {
			decoderChunk->stepsize2[n1+1]=min_step;
		}
	}

	for (n2=0; n2 < SIZE2; n2++) {
		// Dequantize the difference
		decoderChunk->d_prime3[n2+windowsize3dec]=decoderChunk->stepsize3[n2]*z[n2+SIZE2];

		// Sum the dequantized difference and the predicted sample
		s_prime[n2+SIZE2]=decoderChunk->d_prime3[n2+windowsize3dec] + decoderChunk->s_star3[n2];

		// Predict the sample
		//decoderChunk->s_star3[n2+1]=mu3*s_prime[n2+SIZE2];
		decoderChunk->s_star3[n2+1]=s_prime[n2+SIZE2] << mu3pow;

		// Rescale the sample to stay under 40 bits
		//decoderChunk->s_star3[n2+1]=rescale(decoderChunk->s_star3[n2+1], 15);
		b = decoderChunk->s_star3[n2+1] >> 14;
		carry = b & 1;
		b = ((b + carry) >> 1);
		decoderChunk->s_star3[n2+1] = b;

		// Calculate the sum of the absolute values of the dequantized differences
		sum=0;
		#pragma MUST_ITERATE(16,windowsize3);
		#pragma UNROLL(16);
		for (j=n2; j < n2+windowsize3; j++) {
		   	if (decoderChunk->d_prime3[j] < 0) {
				sum=sum-decoderChunk->d_prime3[j];
			}
			else {
				sum=sum+decoderChunk->d_prime3[j];
			}

			//sum=sum+_labs(decoderChunk->d_prime3[j]);
			//step = decoderChunk->d_prime3[j];
			//sum=sum + (step >> 31)*step + (!(step >> 31))*step;
		}

		// Divide the sum by the number of samples in the sum to get the mean
		//power=phi3*sum;
		power=sum << phi3pow;
		//power=divideRound(windowsize3, power);
		/*b = power >> windowsize3pow;
		carry = b & 1;
		b = ((b + carry) >> 1);
		power = b;*/
		power = power/windowsize3;

		// Calculate the stepsize and rescale it to stay under 40 bits
		//decoderChunk->stepsize3[n2+1]=rescale(power, 15);
		b = power >> 14;
		carry = b & 1;
		b = ((b + carry) >> 1);
		decoderChunk->stepsize3[n2+1] = b;

		// Limit the stepsize
		/*if (decoderChunk->stepsize3[n2+1] > max_step3) {	//This is never true
			decoderChunk->stepsize3[n2+1]=max_step3;
		}
		else*/ if (decoderChunk->stepsize3[n2+1] < min_step) {
			decoderChunk->stepsize3[n2+1]=min_step;
		}
	}

	for (n3=0; n3 < SIZE2; n3++) {
		// Dequantize the difference
		decoderChunk->d_prime4[n3+windowsize4dec]=decoderChunk->stepsize4[n3]*z[n3+SIZE2t2];

		// Sum the dequantized difference and the predicted sample
		s_prime[n3+SIZE2t2]=decoderChunk->d_prime4[n3+windowsize4dec] + decoderChunk->s_star4[n3];

		// Predict the sample
		//decoderChunk->s_star4[n3+1]=mu4*s_prime[n3+SIZE2t2];
		decoderChunk->s_star4[n3+1]=s_prime[n3+SIZE2t2] << mu4pow;

		// Rescale the sample to stay under 40 bits
		//decoderChunk->s_star4[n3+1]=rescale(decoderChunk->s_star4[n3+1], 15);
		b = decoderChunk->s_star4[n3+1] >> 14;
		carry = b & 1;
		b = ((b + carry) >> 1);
		decoderChunk->s_star4[n3+1] = b;

		// Calculate the sum of the absolute values of the dequantized differences
		sum=0;
		#pragma MUST_ITERATE(8,windowsize4);
		#pragma UNROLL(8);
		for (k=n3; k < n3+windowsize4; k++) {
			if (decoderChunk->d_prime4[k] < 0) {
				sum=sum-decoderChunk->d_prime4[k];
			}
			else {
				sum=sum+decoderChunk->d_prime4[k];
			}
			//sum=sum+_labs(decoderChunk->d_prime4[k]);

			//step = decoderChunk->d_prime4[k];
			//sum=sum + (step >> 31)*step + (!(step >> 31))*step;
		}

		// Divide the sum by the number of samples in the sum to get the mean
		//power=phi4*sum;
		power=sum << phi4pow;
		//power=divideRound(windowsize4, power);
		/*b = power >> windowsize4pow;
		carry = b & 1;
		b = ((b + carry) >> 1);
		power = b;*/
		power = power/windowsize4;

		// Calculate the stepsize and rescale it to stay under 40 bits
		//decoderChunk->stepsize4[n3+1]=rescale(power, 15);
		b = power >> 14;
		carry = b & 1;
		b = ((b + carry) >> 1);
		decoderChunk->stepsize4[n3+1] = b;

		// Limit the stepsize
		/*if (decoderChunk->stepsize4[n3+1] > max_step4) {	//This is never true
			decoderChunk->stepsize4[n3+1]=max_step4;
		}
		else*/ if (decoderChunk->stepsize4[n3+1] < min_step) {
			decoderChunk->stepsize4[n3+1]=min_step;
		}
	}

	/*for (n4=0; n4 < SIZE2; n4++) {
		// Dequantize the difference
		decoderChunk->d_prime5[n4+windowsize5dec]=decoderChunk->stepsize5[n4]*z[n4+SIZE2t3];

		// Sum the dequantized difference and the predicted sample
		s_prime[n4+SIZE2t3]=decoderChunk->d_prime5[n4+windowsize5dec] + decoderChunk->s_star5[n4];

		// Predict the sample
		decoderChunk->s_star5[n4+1]=mu5*s_prime[n4+SIZE2t3];

		// Rescale the sample to stay under 40 bits
		//decoderChunk->s_star5[n4+1]=rescale(decoderChunk->s_star5[n4+1], 15);
		b = decoderChunk->s_star5[n4+1] >> 14;
		carry = b & 1;
		b = ((b + carry) >> 1);
		decoderChunk->s_star5[n4+1] = b;

		// Calculate the sum of the absolute values of the dequantized differences
		sum=0;
		for (l=n4; l < n4+windowsize5; l++) {
			if (decoderChunk->d_prime5[l] < 0) {
				sum=sum-decoderChunk->d_prime5[l];
			}
			else {
				sum=sum+decoderChunk->d_prime5[l];
			}
		}

		// Divide the sum by the number of samples in the sum to get the mean
		power=phi5*sum;
		//power=divideRound(windowsize5, power);
		b = power >> windowsize5pow;
		carry = b & 1;
		b = ((b + carry) >> 1);
		power = b;

		// Calculate the stepsize and rescale it to stay under 40 bits
		//decoderChunk->stepsize5[n4+1]=rescale(power, 15);
		b = power >> 14;
		carry = b & 1;
		b = ((b + carry) >> 1);
		decoderChunk->stepsize5[n4+1] = b;

		// Limit the stepsize
		if (decoderChunk->stepsize5[n4+1] > max_step5) {
			decoderChunk->stepsize5[n4+1]=max_step5;
		}
		else if (decoderChunk->stepsize5[n4+1] < min_step) {
			decoderChunk->stepsize5[n4+1]=min_step;
		}
		s_prime[n4+SIZE2t3]=0;
	}*/

	// Write the last windowsize - 1 samples to the beginning of the chunks to use them in the next iteration
    #pragma MUST_ITERATE(8,windowsize2);
	#pragma UNROLL(4);
	for (n5=0; n5 < windowsize2; n5++) {
		decoderChunk->d_prime2[n5]=decoderChunk->d_prime2[n5+SIZE2];
	}
    //#pragma MUST_ITERATE(16,16);
    //#pragma UNROLL(2);
	for (n6=0; n6 < windowsize3; n6++) {
		decoderChunk->d_prime3[n6]=decoderChunk->d_prime3[n6+SIZE2];
	}
    #pragma MUST_ITERATE(8,windowsize2);
	#pragma UNROLL(4);
	for (n7=0; n7 < windowsize4; n7++) {
		decoderChunk->d_prime4[n7]=decoderChunk->d_prime4[n7+SIZE2];
	}
	/*for (n8=0; n8 < windowsize5; n8++) {
		decoderChunk->d_prime5[n8]=decoderChunk->d_prime5[n8+SIZE2];
	}*/

	// Write the last stepsizes and s_star values in the chunks to use them in the next iteration
	decoderChunk->stepsize2[0]=decoderChunk->stepsize2[SIZE2];
	decoderChunk->stepsize3[0]=decoderChunk->stepsize3[SIZE2];
	decoderChunk->stepsize4[0]=decoderChunk->stepsize4[SIZE2];
	//decoderChunk->stepsize5[0]=decoderChunk->stepsize5[SIZE2];
	decoderChunk->s_star2[0]=decoderChunk->s_star2[SIZE2];
	decoderChunk->s_star3[0]=decoderChunk->s_star3[SIZE2];
	decoderChunk->s_star4[0]=decoderChunk->s_star4[SIZE2];
	//decoderChunk->s_star5[0]=decoderChunk->s_star5[SIZE2];
}

void initializeDecoder(struct chunkDecoder *decoderChunkL, struct chunkDecoder *decoderChunkR) {

	// Decoder chunks
	memset(decoderChunkL->d_prime2, 0, DSIZE2MEM);
	memset(decoderChunkL->d_prime3, 0, DSIZE3MEM);
	memset(decoderChunkL->d_prime4, 0, DSIZE4MEM);
	//memset(decoderChunkL->d_prime5, 0, DSIZE5MEM);
	memset(decoderChunkL->s_star2, 0, SSIZEMEM);
	memset(decoderChunkL->s_star3, 0, SSIZEMEM);
	memset(decoderChunkL->s_star4, 0, SSIZEMEM);
	//memset(decoderChunkL->s_star5, 0, SSIZEMEM);
	memset(decoderChunkL->stepsize2, 0, SSIZEMEM);
	memset(decoderChunkL->stepsize3, 0, SSIZEMEM);
	memset(decoderChunkL->stepsize4, 0, SSIZEMEM);
	//memset(decoderChunkL->stepsize5, 0, SSIZEMEM);

	decoderChunkL->stepsize2[0]=min_step;
	decoderChunkL->stepsize3[0]=min_step;
	decoderChunkL->stepsize4[0]=min_step;
	//decoderChunkL->stepsize5[0]=min_step;

	memset(decoderChunkR->d_prime2, 0, DSIZE2MEM);
	memset(decoderChunkR->d_prime3, 0, DSIZE3MEM);
	memset(decoderChunkR->d_prime4, 0, DSIZE4MEM);
	//memset(decoderChunkR->d_prime5, 0, DSIZE5MEM);
	memset(decoderChunkR->s_star2, 0, SSIZEMEM);
	memset(decoderChunkR->s_star3, 0, SSIZEMEM);
	memset(decoderChunkR->s_star4, 0, SSIZEMEM);
	//memset(decoderChunkR->s_star5, 0, SSIZEMEM);
	memset(decoderChunkR->stepsize2, 0, SSIZEMEM);
	memset(decoderChunkR->stepsize3, 0, SSIZEMEM);
	memset(decoderChunkR->stepsize4, 0, SSIZEMEM);
	//memset(decoderChunkR->stepsize5, 0, SSIZEMEM);

	decoderChunkR->stepsize2[0]=min_step;
	decoderChunkR->stepsize3[0]=min_step;
	decoderChunkR->stepsize4[0]=min_step;
	//decoderChunkR->stepsize5[0]=min_step;
}

