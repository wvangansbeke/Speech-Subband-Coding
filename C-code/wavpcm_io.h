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
#ifndef WAVPCM_IO_H
#define WAVPCM_IO_H

#include "globals.h"


/*Note: we are assuming short is 16 bit, char is 8 bit etc: not so for DSP */

struct wavpcm_input
{
  /* The following contain essential data about the input. */
  char *resource;       /* The file, device or module name as applicable. */
  int bitDepth;         /* Bit depth of wave file. */
  int samplingRate;     /* Sampling rate of wave file. */
  FILE *fileHandle;     /* File handle of open wave file. */
  int dataStartPos;     /* Position in file were waveform data begins (input only). */
  int currentPosition;  /* Current processing position in file */
  int channels;         /* Used by the engine, so it knows channels available. */
  int samplesAvailable; /* Number of samples available, independent of channels or bitdepth. */
};

struct wavpcm_output
{
  /* The following contain essential data about the input. */
  char *resource;       /* The file, device or module name as applicable. */
  int bitDepth;         /* Bit depth of wave file. */
  int samplingRate;     /* Sampling rate of wave file. */
  FILE *fileHandle;     /* File handle of open wave file. */
  int dataStartPos;     /* Position in file were waveform data begins (input only). */
  int dataLength;       /* Length of data written (output only). */
  int currentPosition;  /* Current processing position in file */
  int channels;         /* Used by the engine, so it knows channels available. */
  int samplesAvailable; /* Number of samples available, independent of channels or bitdepth. */
};

void wavpcm_input_open (struct wavpcm_input *input);
int  wavpcm_input_read (struct wavpcm_input *input, short buffer[BUFFERSIZE]);
void wavpcm_output_open (struct wavpcm_output *output);
void wavpcm_output_copy_settings (struct wavpcm_input *input, struct wavpcm_output *output);
void wavpcm_output_write (struct wavpcm_output *output, short inputBuffer[BUFFERSIZE], int bufferBytes);
void wavpcm_output_close (struct wavpcm_output *output);

#endif // WAVPCM_IO_H
