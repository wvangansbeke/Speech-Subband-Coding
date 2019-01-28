#ifndef GLOBALS_H
#define GLOBALS_H

/* represents buffer array elements: equivalent to BUFFERSIZE/2 temporal sample positions */
#define BUFFERSIZE 40
#define MONOSIZE 20
#define MONOSIZE3BANDS 15
#define COMPRESSEDSIZE 8

#define INPUTWAVFILE  "input.wav"
#define OUTPUTWAVFILE "output.wav"

#define VERBOSE

#define SIZE0 BUFFERSIZE/4
#define SIZE2 BUFFERSIZE/8
#define SIZE2t2 (2*SIZE2)
#define SIZE2t3 (3*SIZE2)
#define SIZE2t4 (4*SIZE2)

//#define nr_subbands 4
#define nr_subbands 3

#define FILTERSIZE 5
#define FILTERSIZEDEC (FILTERSIZE-1)
#define ESIZE (SIZE0+FILTERSIZE-1)

#define BUFFERSIZE0 (SIZE0+FILTERSIZE-1)
#define BUFFERSIZE2 (SIZE2+FILTERSIZE-1)

#define ESIZEMEM (ESIZE*(sizeof(short)))
#define CHUNKMEMSIZE0 (BUFFERSIZE0*(sizeof(short)))
#define CHUNKMEMSIZE2 (BUFFERSIZE2*(sizeof(short)))
#define DSIZE2MEM (DSIZE2*(sizeof(long)))
#define DSIZE3MEM (DSIZE3*(sizeof(long)))
#define DSIZE4MEM (DSIZE4*(sizeof(long)))
#define DSIZE5MEM (DSIZE5*(sizeof(long)))
#define SSIZEMEM (SSIZE*(sizeof(long)))

/*typedef char size8_t;
typedef short size16_t;
typedef int size16_ut;
typedef long size32_t;*/

#endif // GLOBALS_H
