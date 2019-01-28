#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wavpcm_io.h"
#include "globals.h"
#include "functions.h"
#include "encoder.h"
#include "decoder.h"

/* This is the function that is called when the program starts. */
int main (int argc, char *argv[])
{
  /* Variable declarations. */
  struct wavpcm_input input;
  struct wavpcm_output output;
  struct chunkAnalysis analysisChunkL;
  struct chunkAnalysis analysisChunkR;
  struct chunkEncoder encoderChunkL;
  struct chunkEncoder encoderChunkR;
  struct chunkDecoder decoderChunkL;
  struct chunkDecoder decoderChunkR;
  struct chunkSynthesis synthesisChunkL;
  struct chunkSynthesis synthesisChunkR;
  short buffer[BUFFERSIZE];
  short reconstructedBuffer[BUFFERSIZE];
  int bufPos, read;

  short e0[ESIZE];
  short e1[ESIZE];
  short e2[ESIZE];
  short e3[ESIZE];
  short e4[ESIZE];
  short e5[ESIZE];

  short d0[ESIZE];
  short d1[ESIZE];
  short d2[ESIZE];
  short d3[ESIZE];
  short d4[ESIZE];
  short d5[ESIZE];

  short xLE[MONOSIZE3BANDS];
  short xRE[MONOSIZE3BANDS];
  short zLE[MONOSIZE3BANDS];
  short zRE[MONOSIZE3BANDS];
  short xLD[MONOSIZE3BANDS];
  short xRD[MONOSIZE3BANDS];
  short zLD[MONOSIZE3BANDS];
  short zRD[MONOSIZE3BANDS];
  short yL[MONOSIZE];
  short yR[MONOSIZE];
  int zOUT[COMPRESSEDSIZE];

  int i, j;

  memset(&input, 0, sizeof(struct wavpcm_input));
  input.resource=INPUTWAVFILE;
  memset(&output, 0, sizeof(struct wavpcm_output));
  output.resource=OUTPUTWAVFILE;

  initializeAnalysis(&analysisChunkL, &analysisChunkR, e0, e1, e2, e3, e4, e5);
  initializeEncoder(&encoderChunkL, &encoderChunkR);
  initializeDecoder(&decoderChunkL, &decoderChunkR);
  initializeSynthesis(&synthesisChunkL, &synthesisChunkR, d0, d1, d2, d3, d4, d5);

  /* First open input file and parse header, */
  wavpcm_input_open (&input);
  /* and then use this same header configuration for the output file */
  wavpcm_output_copy_settings(&input, &output);
  wavpcm_output_open(&output);

  /*bufPos expressed in temporal samples*/
 //for (bufPos=50000; bufPos<52000; bufPos+=(BUFFERSIZE/2)) {
 for (bufPos=0; bufPos<input.samplesAvailable; bufPos+=(BUFFERSIZE/2)) {
    /* Try to read BUFFERSIZE samples (16 bits, pairwise identical if input is mono, interleaved if input is stereo)
    into buffer, with read the actual amount read (expressed in bytes! =  (2*read)/(channels * bitDepth/8) array elements)*/

    read = wavpcm_input_read (&input, buffer);

     /* transform buffer */
    // Split the buffer in right and left channel
    // splitBuffer(buffer, &analysisChunkL, &analysisChunkR); Inline function
    for (i=0; i < SIZE0; i++) {
        // Write the input values in the left and right even and odd chunk buffers
        analysisChunkL.buffer0[i+FILTERSIZEDEC]=buffer[4*i];
        analysisChunkR.buffer0[i+FILTERSIZEDEC]=buffer[4*i + 1];
        analysisChunkL.buffer1[i+FILTERSIZEDEC]=buffer[4*i + 2];
        analysisChunkR.buffer1[i+FILTERSIZEDEC]=buffer[4*i + 3];
    }

    // Analysis
    analysis(xLE, &analysisChunkL, e0, e1, e2, e3, e4, e5);
    analysis(xRE, &analysisChunkR, e0, e1, e2, e3, e4, e5);

    // Encoding
    encoder(xLE, &encoderChunkL, zLE);
    encoder(xRE, &encoderChunkR, zRE);

    // Bit shifting
    shiftBits(zLE, zRE, zOUT);

    /* if required, dump compressed output */

    /* inverse transform buffer */
    // Inverse bit shifting
    shiftBitsInv(zOUT, zLD, zRD);

    // Decoding
    decoder(zLD, &decoderChunkL, xLD);
    decoder(zRD, &decoderChunkR, xRD);

    // Synthesis
    synthesis(yL, xLD, &synthesisChunkL, d0, d1, d2, d3, d4, d5);
    synthesis(yR, xRD, &synthesisChunkR, d0, d1, d2, d3, d4, d5);

    // Merge buffer
    // mergeBuffer(yL, yR, reconstructedBuffer);
    for (j=0; j < MONOSIZE; j++) {
        // Write the input values in the output buffer in an interleaved fashion
        reconstructedBuffer[2*j]=yL[j];
        reconstructedBuffer[2*j+1]=yR[j];
    }

    /* dump reconstructed output */
    wavpcm_output_write (&output, reconstructedBuffer, read);
  }

  /* finalize output (write header) and close */
  wavpcm_output_close (&output);

  /* Return successful exit code. */
  return 0;
}
