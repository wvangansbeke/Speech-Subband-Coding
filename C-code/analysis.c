#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wavpcm_io.h"
#include "globals.h"
#include "functions.h"
#include "encoder.h"

void analysis(short x[MONOSIZE3BANDS], struct chunkAnalysis *analysisChunk, short e0[ESIZE], short e1[ESIZE], short e2[ESIZE], short e3[ESIZE], short e4[ESIZE], short e5[ESIZE]){

	/* Analysis variables */
	short x0[SIZE0];
	short x1[SIZE0];

	/*long long u0[SIZE0];
	long long u1[SIZE0];
	long long u2[SIZE2];
	long long u3[SIZE2];
	long long u4[SIZE2];
	long long u5[SIZE2];*/
	long u0[SIZE0];
	long u1[SIZE0];
	long u2[SIZE2];
	long u3[SIZE2];
	long u4[SIZE2];
	long u5[SIZE2];

	long carry, b;

	int i, j, k;

	// Convolve the even samples with the even polyphase components and the odd with the odd polyphase components

	conv_unrolled1(u0, e0, analysisChunk->buffer0);
	conv_unrolled1(u1, e1, analysisChunk->buffer1);

	// Sum and rescale the signals
	for (i=0; i < SIZE0; i++) {
		/*x0[i]=(short)rescale((u0[i]+u1[i]), 15);
		x1[i]=(short)rescale((u0[i]-u1[i]), 15);*/
		b = (u0[i]+u1[i]) >> 14;
		carry = b & 1;
		b = ((b + carry) >> 1);
		x0[i] = b;
		b = (u0[i]-u1[i]) >> 14;
		carry = b & 1;
		b = ((b + carry) >> 1);
		x1[i] = b;
	}

	// Split the signals into even and odd samples and put them into the chunks
	//#pragma MUST_ITERATE(SIZE2,SIZE2);
	//#pragma UNROLL(SIZE2)
	for (j=0; j < SIZE2; j++) {
		analysisChunk->buffer2[j+FILTERSIZEDEC]=x0[2*j+1];
		analysisChunk->buffer4[j+FILTERSIZEDEC]=x1[2*j+1];
		analysisChunk->buffer3[j+FILTERSIZEDEC]=x0[2*j];
		analysisChunk->buffer5[j+FILTERSIZEDEC]=x1[2*j];
	}

	// Convolve the even samples with the even polyphase components and the odd with the odd polyphase components
	/*conv_unrolled5(u2, e2, analysisChunk->buffer2, SIZE2);
	conv_unrolled5(u3, e3, analysisChunk->buffer3, SIZE2);
	conv_unrolled5(u4, e4, analysisChunk->buffer4, SIZE2);
	conv_unrolled5(u5, e5, analysisChunk->buffer5, SIZE2);*/
	conv_unrolled2(u2, e2, analysisChunk->buffer2);
	conv_unrolled2(u3, e3, analysisChunk->buffer3);
	conv_unrolled2(u4, e4, analysisChunk->buffer4);
	conv_unrolled2(u5, e5, analysisChunk->buffer5);

	// Sum and rescale the signals
	//#pragma MUST_ITERATE(SIZE2,SIZE2);
	//#pragma UNROLL(SIZE2)
	for (k=0; k < SIZE2; k++) {
		/*x[k]=(short)rescale((u2[k]+u3[k]), 15);
		x[k+SIZE2]=(short)rescale((u2[k]-u3[k]), 15);
		x[k+SIZE2t2]=(short)rescale((u4[k]+u5[k]), 15);
		x[k+SIZE2t3]=(short)rescale((u4[k]-u5[k]), 15);*/
		b = (u2[k]+u3[k]) >> 14;
		carry = b & 1;
		b = ((b + carry) >> 1);
		x[k] = b;
		b = (u2[k]-u3[k]) >> 14;
		carry = b & 1;
		b = ((b + carry) >> 1);
		x[k+SIZE2] = b;
		b = (u4[k]+u5[k]) >> 14;
		carry = b & 1;
		b = ((b + carry) >> 1);
		x[k+SIZE2t2] = b;
		/*b = (u4[k]-u5[k]) >> 14;
		carry = b & 1;
		b = ((b + carry) >> 1);
		x[k+SIZE2t3] = 0;*/
	}
}

void initializeAnalysis(struct chunkAnalysis *analysisChunkL, struct chunkAnalysis *analysisChunkR, short e0[ESIZE], short e1[ESIZE], short e2[ESIZE], short e3[ESIZE], short e4[ESIZE], short e5[ESIZE]) {
	// Filters
	memset(e0, 0, ESIZEMEM);
	memset(e1, 0, ESIZEMEM);
	memset(e2, 0, ESIZEMEM);
	memset(e3, 0, ESIZEMEM);
	memset(e4, 0, ESIZEMEM);
	memset(e5, 0, ESIZEMEM);

	e0[0]=107;
	e0[1]=1769;
	e0[2]=16191;
	e0[3]=-1771;
	e0[4]=87;

	/*    e1[4]=e0[0];
    e1[3]=e0[1];
    e1[2]=e0[2];
    e1[1]=e0[3];
    e1[0]=e0[4];
    Re-reading avoided
	 */

	e1[4]=107;
	e1[3]=1769;
	e1[2]=16191;
	e1[1]=-1771;
	e1[0]=87;

	e2[0]=107;
	e2[1]=1737;
	e2[2]=16198;
	e2[3]=-1740;
	e2[4]=80;

	/*    e3[4]=e2[0];
    e3[3]=e2[1];
    e3[2]=e2[2];
    e3[1]=e2[3];
    e3[0]=e2[4];
    Re-reading avoided
	 */

	e3[4]=107;
	e3[3]=1737;
	e3[2]=16198;
	e3[1]=-1740;
	e3[0]=80;

	/*    e4[0]=e2[0];
    e4[1]=e2[1];
    e4[2]=e2[2];
    e4[3]=e2[3];
    e4[4]=e2[4];
    Re-reading avoided
	 */

	e4[0]=107;
	e4[1]=1737;
	e4[2]=16198;
	e4[3]=-1740;
	e4[4]=80;

	/*    e5[4]=e2[0];
    e5[3]=e2[1];
    e5[2]=e2[2];
    e5[1]=e2[3];
    e5[0]=e2[4];
    Re-reading avoided
	 */

	e5[4]=107;
	e5[3]=1737;
	e5[2]=16198;
	e5[1]=-1740;
	e5[0]=80;

	// Analysis chunks
	memset(analysisChunkL->buffer0, 0, CHUNKMEMSIZE0);
	memset(analysisChunkL->buffer1, 0, CHUNKMEMSIZE0);
	memset(analysisChunkL->buffer2, 0, CHUNKMEMSIZE2);
	memset(analysisChunkL->buffer3, 0, CHUNKMEMSIZE2);
	memset(analysisChunkL->buffer4, 0, CHUNKMEMSIZE2);
	memset(analysisChunkL->buffer5, 0, CHUNKMEMSIZE2);

	memset(analysisChunkR->buffer0, 0, CHUNKMEMSIZE0);
	memset(analysisChunkR->buffer1, 0, CHUNKMEMSIZE0);
	memset(analysisChunkR->buffer2, 0, CHUNKMEMSIZE2);
	memset(analysisChunkR->buffer3, 0, CHUNKMEMSIZE2);
	memset(analysisChunkR->buffer4, 0, CHUNKMEMSIZE2);
	memset(analysisChunkR->buffer5, 0, CHUNKMEMSIZE2);
}

