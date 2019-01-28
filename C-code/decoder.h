#ifndef DECODER_H
#define DECODER_H

#include "globals.h"
#include "codec.h"

// Chunk for the decoder function
struct chunkDecoder {
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

// Chunk for the synthesis function
struct chunkSynthesis {
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

void decoder(short z[MONOSIZE3BANDS], struct chunkDecoder *decoderChunk, short s[MONOSIZE3BANDS]);
void initializeDecoder(struct chunkDecoder *decoderChunkL, struct chunkDecoder *decoderChunkR);
void synthesis(short y[MONOSIZE], short v[MONOSIZE3BANDS], struct chunkSynthesis *synthesisChunck, short d0[ESIZE], short d1[ESIZE], short d2[ESIZE], short d3[ESIZE], short d4[ESIZE], short d5[ESIZE]);
void initializeSynthesis(struct chunkSynthesis *synthesisChunkL, struct chunkSynthesis *synthesisChunkR, short d0[ESIZE], short d1[ESIZE], short d2[ESIZE], short d3[ESIZE], short d4[ESIZE], short d5[ESIZE]);

#endif // DECODER_H
