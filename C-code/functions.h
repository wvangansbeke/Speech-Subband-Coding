#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "encoder.h"
//void splitBuffer(short buffer[BUFFERSIZE], struct chunkAnalysis *analysisChunkL, struct chunkAnalysis *analysisChunkR);
//void mergeBuffer(short buffer1[MONOSIZE], short buffer2[MONOSIZE], short out[BUFFERSIZE]);
/*long long divideRound(long long divisor, long long dividend);
long long rescale(long long a, int K);
void conv(long long *result, short *h, long long *buffer, int m);
void conv_unrolled(long long *result, short *h, long long *buffer, int m);
void conv_unrolled10(long long *result, short *h, long long *buffer, int m);
void conv_unrolled5(long long *result, short *h, long long *buffer, int m);
Use the smallest possible data types
*/
long divideRound(long divisor, long dividend);
long rescale(long a, int K);
/*void conv(long *result, short *h, long *buffer, int m);
void conv_unrolled(long *result, short *h, long *buffer, int m);*/
/*void conv_unrolled10(long *result, short *h, long *buffer, int m);
void conv_unrolled5(long *result, short *h, long *buffer, int m);*/
void conv_unrolled1(long *result, short *h, short *buffer);
void conv_unrolled2(long *result, short *h, short *buffer);
void shiftBits(short in1[MONOSIZE], short in2[MONOSIZE], int out[COMPRESSEDSIZE]);
void shiftBitsInv(int in[COMPRESSEDSIZE], short out1[MONOSIZE], short out2[MONOSIZE]);

#endif // FUNCTIONS_H
