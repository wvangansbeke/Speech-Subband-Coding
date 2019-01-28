#ifndef ENCODER_H
#define ENCODER_H

#include "globals.h"
#include "codec.h"

// Chunk for the encoder function
struct chunkEncoder {
	/*long long d_prime2[DSIZE2];
	long long d_prime3[DSIZE3];
	long long d_prime4[DSIZE4];
	long long d_prime5[DSIZE5];
	long long s_star2[SSIZE];
	long long s_star3[SSIZE];
	long long s_star4[SSIZE];
	long long s_star5[SSIZE];
	long long stepsize2[SSIZE];
	long long stepsize3[SSIZE];
	long long stepsize4[SSIZE];
	long long stepsize5[SSIZE];*/
	long d_prime2[DSIZE2];
	long d_prime3[DSIZE3];
	long d_prime4[DSIZE4];
//	long d_prime5[DSIZE5];
	long s_star2[SSIZE];
	long s_star3[SSIZE];
	long s_star4[SSIZE];
//	long s_star5[SSIZE];
	long stepsize2[SSIZE];
	long stepsize3[SSIZE];
	long stepsize4[SSIZE];
//	long stepsize5[SSIZE];
};

// Chunk for the analysis function
struct chunkAnalysis {
	/*long long buffer0[BUFFERSIZE0];
    long long buffer1[BUFFERSIZE0];
    long long buffer2[BUFFERSIZE2];
    long long buffer3[BUFFERSIZE2];
    long long buffer4[BUFFERSIZE2];
    long long buffer5[BUFFERSIZE2];*/
    short buffer0[BUFFERSIZE0];
    short buffer1[BUFFERSIZE0];
    short buffer2[BUFFERSIZE2];
    short buffer3[BUFFERSIZE2];
    short buffer4[BUFFERSIZE2];
    short buffer5[BUFFERSIZE2];
};

void analysis(short x[MONOSIZE3BANDS], struct chunkAnalysis *analysisChunk, short e0[ESIZE], short e1[ESIZE], short e2[ESIZE], short e3[ESIZE], short e4[ESIZE], short e5[ESIZE]);
void initializeAnalysis(struct chunkAnalysis *analysisChunkL, struct chunkAnalysis *analysisChunkR, short e0[ESIZE], short e1[ESIZE], short e2[ESIZE], short e3[ESIZE], short e4[ESIZE], short e5[ESIZE]);
void encoder (short s[MONOSIZE3BANDS], struct chunkEncoder *encoderChunk, short z[MONOSIZE3BANDS]);
void initializeEncoder(struct chunkEncoder *encoderChunkL, struct chunkEncoder *encoderChunkR);

#endif // ENCODER_H
