#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <c6x.h>
#include "wavpcm_io.h"
#include "globals.h"
#include "functions.h"
#include "codec.h"
#include "encoder.h"

// s contains the signal samples from the four subbands
// encoderChunk is the chunk variable containing d_prime, s_star and stepsize for each subband
// z contains the quantized samples for the four subbands
void encoder (short s[MONOSIZE3BANDS], struct chunkEncoder *encoderChunk, short z[MONOSIZE3BANDS]){

	long s_prime, sum, power, b, carry; //,d,step;
	int n1, n2, n3, n5, n6, n7, i, j, k;//, n4, n8, l;

	for (n1=0; n1 < SIZE2; n1++) {
		// Calculate the difference
		//d=s[n1] - encoderChunk->s_star2[n1];

		// Quantize the difference
		//z[n1]=divideRound(encoderChunk->stepsize2[n1], d);
		z[n1]=(s[n1] - encoderChunk->s_star2[n1])/encoderChunk->stepsize2[n1];

		// Clip the quantized signal
		if (z[n1] > z_max2) {
			z[n1]=z_max2;
		}
		else if (z[n1] < z_min2) {
			z[n1]=z_min2;
		}

		// Dequantize the difference
		encoderChunk->d_prime2[n1+windowsize2dec]=encoderChunk->stepsize2[n1]*z[n1];

		// Sum the dequantized difference and the predicted sample
		s_prime=encoderChunk->d_prime2[n1+windowsize2dec] + encoderChunk->s_star2[n1];

		// Predict the sample
		//encoderChunk->s_star2[n1+1]=mu2*s_prime;
		encoderChunk->s_star2[n1+1]=s_prime << mu2pow;

		// Rescale the sample to stay under 40 bits
		//encoderChunk->s_star2[n1+1]=rescale(encoderChunk->s_star2[n1+1], 15);
		b = encoderChunk->s_star2[n1+1] >> 14;
		carry = b & 1;
		b = ((b + carry) >> 1);
		encoderChunk->s_star2[n1+1] = b;

		// Calculate the sum of the absolute values of the dequantized differences
		sum=0;
		//long r[8];
		#pragma MUST_ITERATE(8,windowsize2);
		#pragma UNROLL(8);
		for (i=n1; i < n1+windowsize2; i++) {
		   if (encoderChunk->d_prime2[i] > 0) {
				sum=sum+encoderChunk->d_prime2[i];
			}
			else {
				sum=sum-encoderChunk->d_prime2[i];
			}
			//sum=sum+_labs(encoderChunk->d_prime2[i]);

		    //step = encoderChunk->d_prime2[i];
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
		//encoderChunk->stepsize2[n1+1]=rescale(power, 15);
		b = power >> 14;
		carry = b & 1;
		b = ((b + carry) >> 1);
		encoderChunk->stepsize2[n1+1] = b;

		// Limit the stepsize
		/*if (encoderChunk->stepsize2[n1+1] > max_step2) {	// This is never true
			encoderChunk->stepsize2[n1+1]=max_step2;
		}
		else*/ if (encoderChunk->stepsize2[n1+1] < min_step) {
			encoderChunk->stepsize2[n1+1]=min_step;
		}
	}
 //   #pragma MUST_ITERATE(5,5);
 //   #pragma UNROLL(5)
	for (n2=0; n2 < SIZE2; n2++) {
		// Calculate the difference
		//d=s[n2+SIZE2] - encoderChunk->s_star3[n2];

		// Quantize the difference
		//z[n2+SIZE2]=divideRound(encoderChunk->stepsize3[n2], d);
		z[n2+SIZE2]=(s[n2+SIZE2] - encoderChunk->s_star3[n2])/encoderChunk->stepsize3[n2];

		// Clip the quantized signal
		if (z[n2+SIZE2] > z_max3) {
			z[n2+SIZE2]=z_max3;
		}
		else if (z[n2+SIZE2] < z_min3) {
			z[n2+SIZE2]=z_min3;
		}

		// Dequantize the difference
		encoderChunk->d_prime3[n2+windowsize3dec]=encoderChunk->stepsize3[n2]*z[n2+SIZE2];

		// Sum the dequantized difference and the predicted sample
		s_prime=encoderChunk->d_prime3[n2+windowsize3dec] + encoderChunk->s_star3[n2];

		// Predict the sample
		//encoderChunk->s_star3[n2+1]=mu3*s_prime;
		encoderChunk->s_star3[n2+1]=s_prime << mu3pow;

		// Rescale the sample to stay under 40 bits
		//encoderChunk->s_star3[n2+1]=rescale(encoderChunk->s_star3[n2+1], 15);
		b = encoderChunk->s_star3[n2+1] >> 14;
		carry = b & 1;
		b = ((b + carry) >> 1);
		encoderChunk->s_star3[n2+1] = b;

		// Calculate the sum of the absolute values of the dequantized differences
		sum=0;
		#pragma MUST_ITERATE(16,windowsize3);
		#pragma UNROLL(16);
		for (j=n2; j < n2+windowsize3; j++) {
			if (encoderChunk->d_prime3[j] > 0) {
				sum=sum+encoderChunk->d_prime3[j];
			}
			else {
				sum=sum-encoderChunk->d_prime3[j];
			}

			//sum=sum+_labs(encoderChunk->d_prime3[j]);

			//step = encoderChunk->d_prime3[j];
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
		//encoderChunk->stepsize3[n2+1]=rescale(power, 15);
		b = power >> 14;
		carry = b & 1;
		b = ((b + carry) >> 1);
		encoderChunk->stepsize3[n2+1] = b;

		// Limit the stepsize
		/*if (encoderChunk->stepsize3[n2+1] > max_step3) {	//This is never true
			encoderChunk->stepsize3[n2+1]=max_step3;
		}
		else*/ if (encoderChunk->stepsize3[n2+1] < min_step) {
			encoderChunk->stepsize3[n2+1]=min_step;
		}
	}
    #pragma MUST_ITERATE(5,5);
    #pragma UNROLL(5)
	for (n3=0; n3 < SIZE2; n3++) {
		// Calculate the difference
		//d=s[n3+SIZE2t2] - encoderChunk->s_star4[n3];

		// Quantize the difference
		//z[n3+SIZE2t2]=divideRound(encoderChunk->stepsize4[n3], d);
		z[n3+SIZE2t2]=(s[n3+SIZE2t2] - encoderChunk->s_star4[n3])/encoderChunk->stepsize4[n3];

		// Clip the quantized signal
		if (z[n3+SIZE2t2] > z_max4) {
			z[n3+SIZE2t2]=z_max4;
		}
		else if (z[n3+SIZE2t2] < z_min4) {
			z[n3+SIZE2t2]=z_min4;
		}

		// Dequantize the difference
		encoderChunk->d_prime4[n3+windowsize4dec]=encoderChunk->stepsize4[n3]*z[n3+SIZE2t2];

		// Sum the dequantized difference and the predicted sample
		s_prime=encoderChunk->d_prime4[n3+windowsize4dec] + encoderChunk->s_star4[n3];

		// Predict the sample
		//encoderChunk->s_star4[n3+1]=mu4*s_prime;
		encoderChunk->s_star4[n3+1]=s_prime << mu4pow;

		// Rescale the sample to stay under 40 bits
		//encoderChunk->s_star4[n3+1]=rescale(encoderChunk->s_star4[n3+1], 15);
		b = encoderChunk->s_star4[n3+1] >> 14;
		carry = b & 1;
		b = ((b + carry) >> 1);
		encoderChunk->s_star4[n3+1] = b;


		// Calculate the sum of the absolute values of the dequantized differences
		sum=0;
		#pragma MUST_ITERATE(8,windowsize4, 8);
		#pragma UNROLL(8);
		for (k=n3; k < n3+windowsize4; k++) {
			if (encoderChunk->d_prime4[k] > 0) {
				sum=sum+encoderChunk->d_prime4[k];
			}
			else {
				sum=sum-encoderChunk->d_prime4[k];
			}

			//sum=sum+_labs(encoderChunk->d_prime4[k]);

			//step = encoderChunk->d_prime4[k];
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
		//encoderChunk->stepsize4[n3+1]=rescale(power, 15);
		b = power >> 14;
		carry = b & 1;
		b = ((b + carry) >> 1);
		encoderChunk->stepsize4[n3+1] = b;

		// Limit the stepsize
		/*if (encoderChunk->stepsize4[n3+1] > max_step4) { // This is never true
			encoderChunk->stepsize4[n3+1]=max_step4;
		}
		else*/ if (encoderChunk->stepsize4[n3+1] < min_step) {
			encoderChunk->stepsize4[n3+1]=min_step;
		}
	}

	/*for (n4=0; n4 < SIZE2; n4++) {
		// Calculate the difference
		d=s[n4+SIZE2t3] - encoderChunk->s_star5[n4];

		// Quantize the difference
		z[n4+SIZE2t3]=divideRound(encoderChunk->stepsize5[n4], d);

	// Clip the quantized signal
		if (z[n4+SIZE2t3] > z_max5) {
			z[n4+SIZE2t3]=z_max5;
		}
		else if (z[n4+SIZE2t3] < z_min5) {
			z[n4+SIZE2t3]=z_min5;
		}

		// Dequantize the difference
		encoderChunk->d_prime5[n4+windowsize5dec]=encoderChunk->stepsize5[n4]*z[n4+SIZE2t3];

		// Sum the dequantized difference and the predicted sample
		s_prime=encoderChunk->d_prime5[n4+windowsize5dec] + encoderChunk->s_star5[n4];

		// Predict the sample
		encoderChunk->s_star5[n4+1]=mu5*s_prime;

		// Rescale the sample to stay under 40 bits
		//encoderChunk->s_star5[n4+1]=rescale(encoderChunk->s_star5[n4+1], 15);
		b = encoderChunk->s_star5[n4+1] >> 14;
		carry = b & 1;
		b = ((b + carry) >> 1);
		encoderChunk->s_star5[n4+1] = b;

		// Calculate the sum of the absolute values of the dequantized differences
		sum=0;
		for (l=n4; l < n4+windowsize5; l++) {
			if (encoderChunk->d_prime5[l] < 0) {
				sum=sum-encoderChunk->d_prime5[l];
			}
			else {
				sum=sum+encoderChunk->d_prime5[l];
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
		//encoderChunk->stepsize5[n4+1]=rescale(power, 15);
		b = power >> 14;
		carry = b & 1;
		b = ((b + carry) >> 1);
		encoderChunk->stepsize5[n4+1] = b;

		// Limit the stepsize
		if (encoderChunk->stepsize5[n4+1] > max_step5) {
			encoderChunk->stepsize5[n4+1]=max_step5;
		}
		else if (encoderChunk->stepsize5[n4+1] < min_step) {
			encoderChunk->stepsize5[n4+1]=min_step;
		}
		z[n4+SIZE2t3]=0;
	}*/

	// Write the last windowsize - 1 samples to the beginning of the chunks to use them in the next iteration
    #pragma MUST_ITERATE(8,windowsize2);
	#pragma UNROLL(8);
	for (n5=0; n5 < windowsize2; n5++) {
		encoderChunk->d_prime2[n5]=encoderChunk->d_prime2[n5+SIZE2];
	}
    #pragma MUST_ITERATE(16,16);
    #pragma UNROLL(16);
	for (n6=0; n6 < windowsize3; n6++) {
		encoderChunk->d_prime3[n6]=encoderChunk->d_prime3[n6+SIZE2];
	}
    #pragma MUST_ITERATE(8,windowsize2);
	#pragma UNROLL(8);
	for (n7=0; n7 < windowsize4; n7++) {
		encoderChunk->d_prime4[n7]=encoderChunk->d_prime4[n7+SIZE2];
	}
	/*for (n8=0; n8 < windowsize5; n8++) {
		encoderChunk->d_prime5[n8]=encoderChunk->d_prime5[n8+SIZE2];
	}*/

	// Write the last stepsizes and s_star values in the chunks to use them in the next iteration
	encoderChunk->stepsize2[0]=encoderChunk->stepsize2[SIZE2];
	encoderChunk->stepsize3[0]=encoderChunk->stepsize3[SIZE2];
	encoderChunk->stepsize4[0]=encoderChunk->stepsize4[SIZE2];
	//encoderChunk->stepsize5[0]=encoderChunk->stepsize5[SIZE2];
	encoderChunk->s_star2[0]=encoderChunk->s_star2[SIZE2];
	encoderChunk->s_star3[0]=encoderChunk->s_star3[SIZE2];
	encoderChunk->s_star4[0]=encoderChunk->s_star4[SIZE2];
	//encoderChunk->s_star5[0]=encoderChunk->s_star5[SIZE2];
}

void initializeEncoder(struct chunkEncoder *encoderChunkL, struct chunkEncoder *encoderChunkR) {

	// Encoder chunks
	memset(encoderChunkL->d_prime2, 0, DSIZE2MEM);
	memset(encoderChunkL->d_prime3, 0, DSIZE3MEM);
	memset(encoderChunkL->d_prime4, 0, DSIZE4MEM);
	//	memset(encoderChunkL->d_prime5, 0, DSIZE5MEM);
	memset(encoderChunkL->s_star2, 0, SSIZEMEM);
	memset(encoderChunkL->s_star3, 0, SSIZEMEM);
	memset(encoderChunkL->s_star4, 0, SSIZEMEM);
	//	memset(encoderChunkL->s_star5, 0, SSIZEMEM);
	memset(encoderChunkL->stepsize2, 0, SSIZEMEM);
	memset(encoderChunkL->stepsize3, 0, SSIZEMEM);
	memset(encoderChunkL->stepsize4, 0, SSIZEMEM);
	//	memset(encoderChunkL->stepsize5, 0, SSIZEMEM);

	encoderChunkL->stepsize2[0]=min_step;
	encoderChunkL->stepsize3[0]=min_step;
	encoderChunkL->stepsize4[0]=min_step;
	//	encoderChunkL->stepsize5[0]=min_step;

	memset(encoderChunkR->d_prime2, 0, DSIZE2MEM);
	memset(encoderChunkR->d_prime3, 0, DSIZE3MEM);
	memset(encoderChunkR->d_prime4, 0, DSIZE4MEM);
	//	memset(encoderChunkR->d_prime5, 0, DSIZE5MEM);
	memset(encoderChunkR->s_star2, 0, SSIZEMEM);
	memset(encoderChunkR->s_star3, 0, SSIZEMEM);
	memset(encoderChunkR->s_star4, 0, SSIZEMEM);
	//	memset(encoderChunkR->s_star5, 0, SSIZEMEM);
	memset(encoderChunkR->stepsize2, 0, SSIZEMEM);
	memset(encoderChunkR->stepsize3, 0, SSIZEMEM);
	memset(encoderChunkR->stepsize4, 0, SSIZEMEM);
	//	memset(encoderChunkR->stepsize5, 0, SSIZEMEM);

	encoderChunkR->stepsize2[0]=min_step;
	encoderChunkR->stepsize3[0]=min_step;
	encoderChunkR->stepsize4[0]=min_step;
	//	encoderChunkR->stepsize5[0]=min_step;
}

