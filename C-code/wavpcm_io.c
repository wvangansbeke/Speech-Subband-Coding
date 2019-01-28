/* ************************************************************************
*  based on AMaMP, pruned and simplified in 2007
*  Copyright Jonathan Worthington 2003, 2004
*  ************************************************************************
*  This file is part of AMaMP.
*
*  AMaMP is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*  ************************************************************************ */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "wavpcm_io.h"
#include "globals.h"

/* ************************ */
/* INPUT RELATED STRUCTURES */
/* ************************ */
struct PCM_WAVE_HEADER
{
	char riff[4];		/* Contains the text string "RIFF"             : 4 bytes */
	int fileSize;		/* Contains the file size - 8                  : 4 bytes */
	char wavefmt[8];	/* Contains the text string "WAVEfmt "         : 8 bytes */
	int headerSize;		/* Contains the size of the description header : 4 bytes */
	short type;		/* Type of wave file. PCM = 1                  : 2 bytes */
	short channels;		/* Number of channels                          : 2 bytes */
	int samplingRate;	/* Sampling rate                               : 4 bytes */
	int bytesPerSecond;	/* Number of bytes per second                  : 4 bytes */
	short blockAlign;	/* Channels * Bitdepth / 8                     : 2 bytes */
	short bitDepth;		/* The bitdepth (bits per sample)              : 2 bytes */
};

struct PCM_WAVE_CHUNK_HEADER
{
	char name[4];		/* Name of chunk                                 : 4 bytes */
	int size;		/* Size of chunk that follows (excluding header) : 4 bytes */
};

/**************************************************************************
********************** THIS SUB OPENS AN INPUT SOURCE *********************
***************************************************************************/
void wavpcm_input_open (struct wavpcm_input *input) {

  /* We need a few temporary variables. */
  int readSize;
  struct PCM_WAVE_HEADER       waveHeader;
  struct PCM_WAVE_CHUNK_HEADER waveChunkHeader;
  memset(&waveHeader, 0, sizeof(struct PCM_WAVE_HEADER));
  memset(&waveChunkHeader, 0, sizeof(struct PCM_WAVE_CHUNK_HEADER));

  /* It's a PCM Wave File. Open file. */
  input->fileHandle = fopen(input->resource, "rb");
  if (input->fileHandle == NULL) {
    printf("Error: Cannot open file %s\n", input->resource);
    exit(1);
  }

  /* Now try to read in wave header. Check it's kinda what we expect. */
  readSize = fread((char *) &waveHeader , 1, sizeof(struct PCM_WAVE_HEADER), input->fileHandle);
  if (readSize != sizeof(struct PCM_WAVE_HEADER)) {
    printf("Error: %s is not a recognised PCM WAVE file.\n", input->resource);
    exit(1);
  }

  if (waveHeader.type != 1 || waveHeader.riff[0] != 'R' || waveHeader.wavefmt[0] != 'W'){
    printf("Error: %s is not a recognised PCM WAVE file.\n", input->resource);
    exit(1);
  }

  /* Copy data from header. */
  input->bitDepth = waveHeader.bitDepth;
  input->channels = waveHeader.channels;
  input->samplingRate = waveHeader.samplingRate;

#ifdef VERBOSE
  printf("PCM WAVE HEADER:\n  type:       %c%c%c%c\n  filesize:   %d\n  wavefmt:    %c%c%c%c%c%c%c%c\n  headersize: %d\n  type:       %d\n  channels:   %d\n  fs:         %d\n  byterate:   %d\n  blockalign: %d\n  bitdepth:   %d\n",waveHeader.riff[0],waveHeader.riff[1],waveHeader.riff[2],waveHeader.riff[3], waveHeader.fileSize, waveHeader.wavefmt[0],waveHeader.wavefmt[1],waveHeader.wavefmt[2],waveHeader.wavefmt[3],waveHeader.wavefmt[4],waveHeader.wavefmt[5],waveHeader.wavefmt[6],waveHeader.wavefmt[7],waveHeader.headerSize,waveHeader.type,waveHeader.channels,waveHeader.samplingRate,waveHeader.bytesPerSecond,waveHeader.blockAlign,waveHeader.bitDepth);
#endif

  /* Find data chunk and the start of the data. */
  while (1) {

    readSize = fread((char *) &waveChunkHeader, 1, sizeof(struct PCM_WAVE_CHUNK_HEADER), input->fileHandle);
    if (readSize != sizeof(struct PCM_WAVE_CHUNK_HEADER)){
      printf("Error: %s is not a recognised PCM WAVE file.\n", input->resource);
      exit(1);
    }

    if (waveChunkHeader.name[0] == 'd' &&  waveChunkHeader.name[1] == 'a' &&
	waveChunkHeader.name[2] == 't' && waveChunkHeader.name[3] == 'a')
      break;
    /*Data chunk found*/

    /*previous chunk was not a data chunk, but an optional chunk (e.g. cue points,
      instrument parameters, application specific data,...) -> find next chunk   */
    readSize = fseek(input->fileHandle, (long) waveChunkHeader.size, SEEK_CUR);
    if (readSize != 0) {
      printf("Error: %s is not a recognised PCM WAVE file.\n", input->resource);
      exit(1);
    }

  }

#ifdef VERBOSE
  /* Check consistency of waveChunkHeader.size and waveHeader.fileSize */
  if ( waveHeader.fileSize != (waveChunkHeader.size+36) ) printf("According to some random website I consulted, the fileSize field in the WaveHeader should be the size field in the waveChunkHeader +36,\nso that's how i will dump it, even if that's not what I'm reading in now.\n");
#endif

  /* Store important data. */
  input->dataStartPos = ftell(input->fileHandle);
  input->currentPosition = input->dataStartPos;
  input->samplesAvailable = (int) ((waveChunkHeader.size / (input->bitDepth / 8)) / input->channels);


#ifdef VERBOSE
  printf("PCM_WAVE_CHUNK_HEADER:\n  name:       %c%c%c%c\n  size:       %d\n",waveChunkHeader.name[0],waveChunkHeader.name[1],waveChunkHeader.name[2],waveChunkHeader.name[3],waveChunkHeader.size);
#endif

  if ( !((input->bitDepth == 8) || (input->bitDepth == 16)) ) {
    printf("Error: Unsupported bit depth %d cannot be translated.\n", input->bitDepth);
    exit(1);
  }
  if (input->channels > 2) {
    printf("Error: Unsupported number of channels %d  cannot be handled.\n", input->channels);
    exit(1);
  }


}

/**************************************************************************
****************** THIS SUB READS DATA FROM AN INPUT SOURCE ***************
***************************************************************************/
int wavpcm_input_read (struct wavpcm_input *input, short destBuffer[BUFFERSIZE]) {
  /* PCM Wave File. We always declare a buffer of constant size BUFFERSIZE
   * with all data expanded to 16-bit resolution, and potential mono inputs
   * duplicated to stereo */
  int tempBufferSize = BUFFERSIZE / 2;     /* how many temporal samples per buffer, regardless of channels */
  int blockAlign = (input->bitDepth / 8) * input->channels; /* How many bytes per temporal sample in input */
  int bytesToRead = tempBufferSize * blockAlign;               /* How many bytes to read to fill tempBuffer*/

  int readSize; /* bytes actually read */
  int destPos, copyPos;

  if (fseek(input->fileHandle, input->currentPosition, SEEK_SET)) {
    printf("Read error: unexpected end of file.\n");
    exit(1);
  }

  /* Copy data. */
  copyPos = 0;
  if (input->bitDepth == 8) {
    /* We then need to read the data into that buffer. We don't check readSize here.
     * The reason is that we may have really found the end of the file, which is OK.  */
    char tempBuffer[BUFFERSIZE];
    readSize = fread(tempBuffer, 1, bytesToRead, input->fileHandle);

    for (destPos = 0; destPos<BUFFERSIZE; destPos+=2) {
      destBuffer[destPos] = 256 * (short) tempBuffer[copyPos]; /* expand 8 bit to 16 bit */
      if (input->channels == 2)
	destBuffer[destPos + 1] = 256 * (short) tempBuffer[copyPos + 1];
      else
	destBuffer[destPos + 1] = destBuffer[destPos];
      copyPos += input->channels;
    }
  }
  else if (input->bitDepth == 16) {
    /* We then need to read the data into that buffer. We don't check readSize here.
     * The reason is that we may have really found the end of the file, which is OK.  */
    short tempBuffer[BUFFERSIZE];
    readSize = fread(tempBuffer, 1, bytesToRead, input->fileHandle);

    for (destPos = 0; destPos<BUFFERSIZE; destPos+=2) {
      destBuffer[destPos] = tempBuffer[copyPos]; /* not expanded */
      if (input->channels == 2)
	destBuffer[destPos + 1] = tempBuffer[copyPos + 1];
      else
	destBuffer[destPos + 1] = destBuffer[destPos];
      copyPos += input->channels;
    }
  }

  /* Update next read positions. */
  input->currentPosition += readSize;

  return readSize;
}



/**************************************************************************
*********** THIS SUB SEEKS TO A CERTAIN SAMPLE IN AN INPUT SOURCE *********
***************************************************************************/
void wavpcm_input_seek(struct wavpcm_input *input, int startSample) {
  int position;

  /* We need to calculate the real position in the file to read from. */
  position = input->dataStartPos;
  position += startSample * input->channels * (input->bitDepth / 8);
  input->currentPosition = position;
}



/**************************************************************************
********************** THIS SUB CLOSES AN INPUT SOURCE ********************
***************************************************************************/
void wavpcm_input_close (struct wavpcm_input *input)
{
  /* Close the file */
  fclose(input->fileHandle);
}


/**************************************************************************
************************* THIS SUB OPENS AN OUTPUT ************************
***************************************************************************/
void wavpcm_output_open (struct wavpcm_output *output)
{
  /* It's a file, so we need a handle. */
  output->fileHandle = fopen(output->resource, "wb");
  if (output->fileHandle == 0) {
    printf("Error: Cannot open output file %s\n", output->resource);
    exit(1);
  }

  /* We'll write the header in finalization; simply set the output pointer
     to the end of where the header would be. */
  fseek(output->fileHandle, 44, 0);
}

/******the bitDepth, samplingRate and channels characteristics for an already opened input are set for the output******/
void wavpcm_output_copy_settings (struct wavpcm_input *input, struct wavpcm_output *output)
{

  /* Copy input parameters for bit depth and sampling rate. */
  if (input->bitDepth != 0)
    output->bitDepth = input->bitDepth;
  else {
    printf("Write error: invalid input bitDepth reference.\n");
    exit(1);
  }
  if (input->samplingRate != 0)
    output->samplingRate = input->samplingRate;
  else {
    printf("Write error: invalid input samplingRate reference.\n");
    exit(1);
  }
  if ( (input->channels == 1) || (input->channels == 2) ) {
    output->channels = input->channels;
  }
  else {
    printf("Write error: invalid input channels reference.\n");
    exit(1);
  }

}

/**************************************************************************
*********************** THIS SUB WRITES TO AN OUTPUT **********************
***************************************************************************/
void wavpcm_output_write (struct wavpcm_output *output, short inputBuffer[BUFFERSIZE], int bytesToWrite)
{
  /* Declare variables. */
  int curPos = 0;
  int channels = output->channels;
  int bitDepth = output->bitDepth;

  /* (max) bytesToRead was: BUFFERSIZE/2 * bitDepth/8 * channels */
  /* -> maximal buffer index is: (2*bytesToWrite)/(bitDepth/8 * channels) */
  int bufferEndIndex = (2*bytesToWrite)/(bitDepth/8 * channels);

  /* Copy input buffer into tempBuffer, doing any conversions along the way. */
  if (bitDepth == 8) {
    /* written sample per sample */
    char toWrite;
    for (curPos = 0; curPos < bufferEndIndex; curPos += 2) {/* buffer aways in stereo: curPos +=2 */
      toWrite = (char) (inputBuffer[curPos] / 256);
      fwrite(&toWrite, 1, 1, output->fileHandle);
      if (channels == 2) {
	toWrite = (char) (inputBuffer[curPos + 1] / 256);
	fwrite(&toWrite, 1, 1, output->fileHandle);
      }
    }
  }
  else if (bitDepth == 16) {
    /* written sample per sample */
    short toWrite;
    for (curPos = 0; curPos < bufferEndIndex; curPos += 2) {
      toWrite = inputBuffer[curPos];
      fwrite(&toWrite, 2, 1, output->fileHandle);
      if (channels == 2){
	toWrite = inputBuffer[curPos+1];
	fwrite(&toWrite, 2, 1, output->fileHandle);
      }
    }
  }
  else {
    printf("Error: Unsupported bit depth %d cannot be dumped.\n", bitDepth);
    exit(1);
  }

  /* Store size of the written buffer. */
  output->dataLength += bytesToWrite;
}


/**************************************************************************
************************ THIS SUB CLOSES AN OUTPUT ************************
***************************************************************************/
void wavpcm_output_close (struct wavpcm_output *output)
{

  /* Generate headers. */
  struct PCM_WAVE_HEADER       waveHeader;
  struct PCM_WAVE_CHUNK_HEADER waveChunkHeader;
  memset(&waveHeader, 0, sizeof(struct PCM_WAVE_HEADER));
  memset(&waveChunkHeader, 0, sizeof(struct PCM_WAVE_CHUNK_HEADER));

  strcpy(waveHeader.riff, "RIFF");
  strcpy(waveHeader.wavefmt, "WAVEfmt ");
  waveHeader.bitDepth = output->bitDepth;
  waveHeader.blockAlign = output->channels * (output->bitDepth / 8);
  waveHeader.bytesPerSecond = waveHeader.blockAlign * output->samplingRate;
  waveHeader.channels = output->channels;
  waveHeader.fileSize = sizeof(struct PCM_WAVE_HEADER) + output->dataLength;
  waveHeader.headerSize = 16;
  waveHeader.type = 1;
  waveHeader.samplingRate = output->samplingRate;
  strcpy(waveChunkHeader.name, "data");
  waveChunkHeader.size = output->dataLength;

  /* We need to seek back to the start of the file and write the header. */
  fseek(output->fileHandle, 0, SEEK_SET);
  fwrite(&waveHeader, 1, sizeof(struct PCM_WAVE_HEADER), output->fileHandle);
  fwrite(&waveChunkHeader, 1, sizeof(struct PCM_WAVE_CHUNK_HEADER), output->fileHandle);

  /* Close the file. */
  fclose(output->fileHandle);
}
