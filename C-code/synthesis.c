#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wavpcm_io.h"
#include "globals.h"
#include "functions.h"
#include "decoder.h"

void synthesis (short y[MONOSIZE], short v[MONOSIZE3BANDS], struct chunkSynthesis *synthesisChunck, short d0[ESIZE], short d1[ESIZE], short d2[ESIZE], short d3[ESIZE], short d4[ESIZE], short d5[ESIZE]) {

	/*long long w0[SIZE0];
	long long w1[SIZE0];
	long long w2[SIZE2];
	long long w3[SIZE2];
	long long w4[SIZE2];
	long long w5[SIZE2];*/
	long w0[SIZE0];
	long w1[SIZE0];
	long w2[SIZE2];
	long w3[SIZE2];
	long w4[SIZE2];
	//long w5[SIZE2];


	/*long long my0[SIZE0];
	long long my1[SIZE0];*/
	short my0[SIZE0];
	short my1[SIZE0];

	long carry, b;

	int i, j, k, l;

	// Sum and rescale the signals
	for (i=0; i < SIZE2; i++) {
		synthesisChunck->buffer2[i+FILTERSIZEDEC]=v[i]+v[i+SIZE2];
		synthesisChunck->buffer3[i+FILTERSIZEDEC]=v[i]-v[i+SIZE2];
		synthesisChunck->buffer4[i+FILTERSIZEDEC]=v[i+SIZE2t2];//+v[i+SIZE2t3];
		//synthesisChunck->buffer5[i+FILTERSIZEDEC]=v[i+SIZE2t2];//-v[i+SIZE2t3];
	}

	// Convolve the even samples with the even polyphase components and the odd with the odd polyphase components
	/*conv_unrolled5(w2, d3, synthesisChunck->buffer2, SIZE2);
	conv_unrolled5(w3, d2, synthesisChunck->buffer3, SIZE2);
	conv_unrolled5(w4, d5, synthesisChunck->buffer4, SIZE2);
	//conv_unrolled5(w5, d4, synthesisChunck->buffer5, SIZE2);*/
	conv_unrolled2(w2, d3, synthesisChunck->buffer2);
	conv_unrolled2(w3, d2, synthesisChunck->buffer3);
	conv_unrolled2(w4, d5, synthesisChunck->buffer4);

	// Interleave the signals
	for (j=0; j < SIZE2; j++) {
		/*my0[2*j+1]=(short)rescale(w2[j], 15);
		my0[2*j]=(short)rescale(w3[j], 15);
		my1[2*j+1]=(short)rescale(w4[j], 15);
		my1[2*j]=(short)rescale(w5[j], 15);*/
		b = w2[j] >> 14;
		carry = b & 1;
		b = ((b + carry) >> 1);
		my0[2*j+1] = b;
		b = w3[j] >> 14;
		carry = b & 1;
		b = ((b + carry) >> 1);
		my0[2*j] = b;
		b = w4[j] >> 14;
		carry = b & 1;
		b = ((b + carry) >> 1);
		my1[2*j+1] = b;
		/*b = w5[j] >> 14;
		carry = b & 1;
		b = ((b + carry) >> 1);*/
		my1[2*j] = 0;
	}

	//	// Delay the least split up branch -> This is only as an illustration and only necessary when using an asymmetrical filterbank
	//  // It is added since the Matlab part also used a 6 subband filterbank.
	//	delay0=2*((2*FILTERSIZE - 1) + 2*(FILTERSIZE - 1));
	//	delay1=2*(2*FILTERSIZE - 1);
	//	delay=delay0-delay1;
	//	for (i=delay; i < 2*FILTERSIZE; i++) {
	//		y1_delayed[i]=y1[i+delay];
	//	}

	// Sum the signals
	for (k=0; k < SIZE0; k++) {
		synthesisChunck->buffer0[k+FILTERSIZEDEC]=my0[k]+my1[k];
		synthesisChunck->buffer1[k+FILTERSIZEDEC]=my0[k]-my1[k];
	}

	// Convolve the even samples with the even polyphase components and the odd with the odd polyphase components
	/*conv_unrolled10(w0, d1, synthesisChunck->buffer0, SIZE0);
	conv_unrolled10(w1, d0, synthesisChunck->buffer1, SIZE0);*/
	conv_unrolled1(w0, d1, synthesisChunck->buffer0);
	conv_unrolled1(w1, d0, synthesisChunck->buffer1);

	// Interleave the signals
	for (l=0; l < SIZE0; l++) {
		/*y[2*l+1]=(short)rescale(w0[l], 15);
		y[2*l]=(short)rescale(w1[l], 15);*/
		b = w0[l] >> 14;
		carry = b & 1;
		b = ((b + carry) >> 1);
		y[2*l+1] = b;
		b = w1[l] >> 14;
		carry = b & 1;
		b = ((b + carry) >> 1);
		y[2*l] = b;
	}
}

void initializeSynthesis(struct chunkSynthesis *synthesisChunkL, struct chunkSynthesis *synthesisChunkR, short d0[ESIZE], short d1[ESIZE], short d2[ESIZE], short d3[ESIZE], short d4[ESIZE], short d5[ESIZE]) {
	// Filters
	memset(d0, 0, ESIZEMEM);
	memset(d1, 0, ESIZEMEM);
	memset(d2, 0, ESIZEMEM);
	memset(d3, 0, ESIZEMEM);
//	memset(d4, 0, ESIZEMEM);
	memset(d5, 0, ESIZEMEM);

	d0[0]=214;
	d0[1]=3538;
	d0[2]=32382;
	d0[3]=-3543;
	d0[4]=173;

	/*    d1[4]=d0[0];
    d1[3]=d0[1];
    d1[2]=d0[2];
    d1[1]=d0[3];
    d1[0]=d0[4];
    Re-reading avoided
	 */

	d1[4]=214;
	d1[3]=3538;
	d1[2]=32382;
	d1[1]=-3543;
	d1[0]=173;

	d2[0]=214;
	d2[1]=3475;
	d2[2]=32396;
	d2[3]=-3481;
	d2[4]=159;

	/*    d3[4]=d2[0];
    d3[3]=d2[1];
    d3[2]=d2[2];
    d3[1]=d2[3];
    d3[0]=d2[4];
    Re-reading avoided
	 */

	d3[4]=214;
	d3[3]=3475;
	d3[2]=32396;
	d3[1]=-3481;
	d3[0]=159;

	/*    d4[0]=d2[0];
    d4[1]=d2[1];
    d4[2]=d2[2];
    d4[3]=d2[3];
    d4[4]=d2[4];
    Re-reading avoided
	 */

/*	d4[0]=214;
	d4[1]=3475;
	d4[2]=32396;
	d4[3]=-3481;
	d4[4]=159;*/

	/*    d5[4]=d2[0];
    d5[3]=d2[1];
    d5[2]=d2[2];
    d5[1]=d2[3];
    d5[0]=d2[4];
    Re-reading avoided
	 */

	d5[4]=214;
	d5[3]=3475;
	d5[2]=32396;
	d5[1]=-3481;
	d5[0]=159;
	d5[0]=159;

	// Synthesis chunks
	memset(synthesisChunkL->buffer0, 0, CHUNKMEMSIZE0);
	memset(synthesisChunkL->buffer1, 0, CHUNKMEMSIZE0);
	memset(synthesisChunkL->buffer2, 0, CHUNKMEMSIZE2);
	memset(synthesisChunkL->buffer3, 0, CHUNKMEMSIZE2);
	memset(synthesisChunkL->buffer4, 0, CHUNKMEMSIZE2);
	//memset(synthesisChunkL->buffer5, 0, CHUNKMEMSIZE2);

	memset(synthesisChunkR->buffer0, 0, CHUNKMEMSIZE0);
	memset(synthesisChunkR->buffer1, 0, CHUNKMEMSIZE0);
	memset(synthesisChunkR->buffer2, 0, CHUNKMEMSIZE2);
	memset(synthesisChunkR->buffer3, 0, CHUNKMEMSIZE2);
	memset(synthesisChunkR->buffer4, 0, CHUNKMEMSIZE2);
	//memset(synthesisChunkR->buffer5, 0, CHUNKMEMSIZE2);
}

