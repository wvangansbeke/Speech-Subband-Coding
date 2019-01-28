#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <c6x.h>
#include "globals.h"
#include "encoder.h"

/* This function splits the input buffer of size BUFFERSIZE into four separate buffers of size SIZE0, two for each channel (Left
    and Right), and one for even and one for odd samples */
/*void splitBuffer(short buffer[BUFFERSIZE], struct chunkAnalysis *analysisChunkL, struct chunkAnalysis *analysisChunkR) {

    for (int i=0; i < SIZE0; i++) {
        // Write the input values in the left and right even and odd chunk buffers
        analysisChunkL->buffer0[i+FILTERSIZE-1]=buffer[4*i];
        analysisChunkR->buffer0[i+FILTERSIZE-1]=buffer[4*i + 1];
        analysisChunkL->buffer1[i+FILTERSIZE-1]=buffer[4*i + 2];
        analysisChunkR->buffer1[i+FILTERSIZE-1]=buffer[4*i + 3];
    }
}*/

/* This function merges two input buffers of size MONOSIZE, one for each channel (Left and Right) into one buffer of BUFFERSIZE
    samples*/
/*void mergeBuffer(short buffer1[MONOSIZE], short buffer2[MONOSIZE], short out[BUFFERSIZE]) {

    for (int i=0; i < MONOSIZE; i++) {
        // Write the input values in the output buffer in an interleaved fashion
        out[2*i]=buffer1[i];
        out[2*i+1]=buffer2[i];
    }
}*/

/* This function performs a division and rounds the quotient */
/*long long divideRound(long long divisor, long long dividend) {
Use the smallest possible data types
*/
long divideRound(long divisor, long dividend) {
		long quotient, rem;
	    quotient=dividend/divisor;
	    rem=dividend%divisor;
	    /*if (divisor < 0) {	// This is never true
	        divisor=-divisor;
	        if (rem > (divisor >> 1)) {
	            quotient=quotient-1;
	        }
			//quotient=quotient - (!((rem - divisor) >> 31));
	    }
	    else {
	        if (rem > (divisor >> 1)) {
	            quotient=quotient+1;
	        }
			//quotient=quotient + (!((rem - divisor) >> 31));
	    }*/
	    quotient=quotient + !((rem - (divisor >> 1) - 1) >> 31);
		return quotient;
}

/* This function divides a by 2^K and rounds the outcome */
/*long long rescale(long long a, int K){
Use the smallest possible data types
*/
/*long rescale(long a, int K){
	 long carry, b;
     b = a >> (K-1);
     carry = b & 1;
     b = ((b + carry) >> 1);
     return b;
}*/

/* This function performs a convolution of h and buffer and puts the output in result */
/*void conv(long long *result, short *h, long long *buffer, int m){
Use the smallest possible data types
*/
/*void conv(long *result, short *h, long *buffer, int m){
	int i, j;
	for(i = 0; i < m; i++){
		result[i]=0;
		for(j = 0; j<m+FILTERSIZE-1; j++){
			if ((i-j+FILTERSIZE-1)>=0){
				result[i] = result[i]+buffer[j]*h[i-j+FILTERSIZE-1];
			}
			else
			    break;
		}
    }

	buffer[0]=buffer[m];
	buffer[1]=buffer[m+1];
	buffer[2]=buffer[m+2];
    buffer[3]=buffer[m+3];
}*/

/* This function convolves the input with the filtervector h and outputs the result. */
/*void conv_unrolled(long long *result, short *h, long long *buffer, int m){
Use the smallest possible data types
*/
/*void conv_unrolled(long *result, short *h, long *buffer, int m){
	int i;
    for (i=0;i<m;i++){
        result[i]=buffer[i]*h[i+4]+buffer[i+1]*h[i+3]+buffer[i+2]*h[i+2]+buffer[i+3]*h[i+1]+buffer[i+4]*h[i];
    }
    buffer[0]=buffer[m];
	buffer[1]=buffer[m+1];
	buffer[2]=buffer[m+2];
    buffer[3]=buffer[m+3];
}*/

/*void conv_unrolled10(long long *result, short *h, long long *buffer, int m){
Use the smallest possible data types
*/
/*void conv_unrolled10(long *result, short *h, long *buffer, int m){
	result[0]=buffer[0]*h[4]+buffer[1]*h[3]+buffer[2]*h[2]+buffer[3]*h[1]+buffer[4]*h[0];
	result[1]=buffer[1]*h[4]+buffer[2]*h[3]+buffer[3]*h[2]+buffer[4]*h[1]+buffer[5]*h[0];
	result[2]=buffer[2]*h[4]+buffer[3]*h[3]+buffer[4]*h[2]+buffer[5]*h[1]+buffer[6]*h[0];
	result[3]=buffer[3]*h[4]+buffer[4]*h[3]+buffer[5]*h[2]+buffer[6]*h[1]+buffer[7]*h[0];
	result[4]=buffer[4]*h[4]+buffer[5]*h[3]+buffer[6]*h[2]+buffer[7]*h[1]+buffer[8]*h[0];
	result[5]=buffer[5]*h[4]+buffer[6]*h[3]+buffer[7]*h[2]+buffer[8]*h[1]+buffer[9]*h[0];
	result[6]=buffer[6]*h[4]+buffer[7]*h[3]+buffer[8]*h[2]+buffer[9]*h[1]+buffer[10]*h[0];
	result[7]=buffer[7]*h[4]+buffer[8]*h[3]+buffer[9]*h[2]+buffer[10]*h[1]+buffer[11]*h[0];
	result[8]=buffer[8]*h[4]+buffer[9]*h[3]+buffer[10]*h[2]+buffer[11]*h[1]+buffer[12]*h[0];
	result[9]=buffer[9]*h[4]+buffer[10]*h[3]+buffer[11]*h[2]+buffer[12]*h[1]+buffer[13]*h[0];
	buffer[0]=buffer[10];
    buffer[1]=buffer[11];
	buffer[2]=buffer[12];
	buffer[3]=buffer[13];
}*/

/*void conv_unrolled5(long long *result, short *h, long long *buffer, int m){
Use the smallest possible data types
*/
/*void conv_unrolled5(long *result, short *h, long *buffer, int m){
    result[0]=buffer[0]*h[4]+buffer[1]*h[3]+buffer[2]*h[2]+buffer[3]*h[1]+buffer[4]*h[0];
	result[1]=buffer[1]*h[4]+buffer[2]*h[3]+buffer[3]*h[2]+buffer[4]*h[1]+buffer[5]*h[0];
	result[2]=buffer[2]*h[4]+buffer[3]*h[3]+buffer[4]*h[2]+buffer[5]*h[1]+buffer[6]*h[0];
	result[3]=buffer[3]*h[4]+buffer[4]*h[3]+buffer[5]*h[2]+buffer[6]*h[1]+buffer[7]*h[0];
	result[4]=buffer[4]*h[4]+buffer[5]*h[3]+buffer[6]*h[2]+buffer[7]*h[1]+buffer[8]*h[0];
	buffer[0]=buffer[5];
    buffer[1]=buffer[6];
	buffer[2]=buffer[7];
	buffer[3]=buffer[8];
}*/

/*void conv_unrolled(long *result, short *h, long *buffer, int m){
	int i;
    #pragma MUST_ITERATE(5,10,5);
    for (i=0;i<m;i++){
        result[i]=buffer[i]*h[i+4]+buffer[i+1]*h[i+3]+buffer[i+2]*h[i+2]+buffer[i+3]*h[i+1]+buffer[i+4]*h[i];
    }
    buffer[0]=buffer[m];
	buffer[1]=buffer[m+1];
	buffer[2]=buffer[m+2];
    buffer[3]=buffer[m+3];
}*/
void conv_unrolled1(long *restrict result, short *restrict h, short *restrict buffer){
	int i;
/*	short h4=h[4];
    short h3=h[3];
	short h2=h[2];
	short h1=h[1];
	short h0=h[0];*/
	//signed int r[5*SIZE0];
	//_nassert((long)buffer % 8 == 0);
	//_nassert((long)result % 8 == 0);
	//_nassert((short)h % 8 == 0);
	#pragma MUST_ITERATE(10,10);
    #pragma UNROLL(10);
    for (i=0;i<10;i++){
      result[i]=buffer[i]*h[4]+buffer[i+1]*h[3]+buffer[i+2]*h[2]+buffer[i+3]*h[1]+buffer[i+4]*h[0];
    }

  /*  #pragma MUST_ITERATE(SIZE0,SIZE0);
    #pragma UNROLL(SIZE0);
    for (i=0;i<SIZE0;i++){
           r[i]=buffer[i]*h[4];
          if (buffer[i]>8000){
              printf("%d\n",buffer[i]);
              }
           r[i+SIZE0]=buffer[i+1]*h[3];
           r[i+2*SIZE0]=buffer[i+2]*h[2];
           r[i+3*SIZE0]=buffer[i+3]*h[1];
           r[i+4*SIZE0]=buffer[i+4]*h[0];
           }

     #pragma MUST_ITERATE(SIZE0,SIZE0);
     #pragma UNROLL(SIZE0);
     for (j=0;j<SIZE0;j++){
           result[j]=r[j]+r[j+SIZE0]+r[j+2*SIZE0]+r[j+3*SIZE0]+r[j+4*SIZE0];
         }*/

    buffer[0]=buffer[10];
	buffer[1]=buffer[11];
	buffer[2]=buffer[12];
    buffer[3]=buffer[13];
}
void conv_unrolled2(long *restrict result, short *restrict h, short *restrict buffer){
	int i;
	short h4=h[4];
	short h3=h[3];
	short h2=h[2];
	short h1=h[1];
	short h0=h[0];
    #pragma MUST_ITERATE(5,5);
    #pragma UNROLL(5);
    for (i=0;i<SIZE2;i++){
        result[i]=buffer[i]*h4+buffer[i+1]*h3+buffer[i+2]*h2+buffer[i+3]*h1+buffer[i+4]*h0;
    }
	/*signed int r[5*SIZE2];
    #pragma MUST_ITERATE(SIZE2,SIZE2);
    #pragma UNROLL(SIZE2);
    for (i=0;i<SIZE2;i++){
            r[i]=buffer[i]*h[4];
            //if (r[i]>32000){
            //printf("%d\n",r[i]);
            //}
            r[i+SIZE2]=buffer[i+1]*h[3];
            r[i+2*SIZE2]=buffer[i+2]*h[2];
            r[i+3*SIZE2]=buffer[i+3]*h[1];
            r[i+4*SIZE2]=buffer[i+4]*h[0];
         }

    #pragma MUST_ITERATE(SIZE2,SIZE2);
    #pragma UNROLL(SIZE2);
    for (j=0;j<SIZE2;j++){
            result[j]=r[j]+r[j+SIZE2]+r[j+2*SIZE2]+r[j+3*SIZE2]+r[j+4*SIZE2];
        }*/
    buffer[0]=buffer[5];
    buffer[1]=buffer[6];
    buffer[2]=buffer[7];
    buffer[3]=buffer[8];
}

/* This function compresses the input samples of two MONOSIZE buffers into one buffer of size 8. This is done by shifting the
    samples according to the number of bits they can be represented in */
void shiftBits(short in1[MONOSIZE3BANDS], short in2[MONOSIZE3BANDS], int out[COMPRESSEDSIZE]) {
    memset(out, 0, COMPRESSEDSIZE*(sizeof(int)));

    short N[nr_subbands];
    short z_min[nr_subbands];

    N[0]=5;
    N[1]=5;
    N[2]=2;
    //N[3]=0;

    z_min[0]=-16;
    z_min[1]=-16;
    z_min[2]=-2;
    //z_min[3]=0;

    int in = 0, res = 0, length = 0;
    int i, j, k=0, l, m;
	#pragma MUST_ITERATE(SIZE2,SIZE2);
	#pragma UNROLL(SIZE2);
    for (j=0; j < SIZE2; j++) {
		#pragma MUST_ITERATE(nr_subbands,nr_subbands);
		#pragma UNROLL(nr_subbands);
        for (i=0; i < nr_subbands; i++) {
            in=(int)in1[j+i*SIZE2] - (int)z_min[i]; // Make the input unsigned by adding an offset
            res=out[k]+(int)(in << length);  // Write the input on the right place (bits) in the output
            length=length+N[i]; // Increase the length  (number of bits of the output number that are already filled)
            if (length >= 16) {
                res=in - (int)((int)(in >> (16-(length-N[i]))) << (16-(length-N[i])));   // Select the least significant bits from the input
                out[k]=out[k]+(int)(res << (length-N[i]));   // Write the least significant bits on the right place in the output
                length=length-16;   // Decrease the length
                k=k+1;  // Increment k to write in the next output number
                out[k]=out[k]+(int)(in >> (N[i]-length));   // Write the most significant input bits in the next output number
            }
            else {
                out[k]=res;
            }
        }
    }
    for (l=0; l < SIZE2; l++) {
		#pragma MUST_ITERATE(nr_subbands,nr_subbands);
		#pragma UNROLL(nr_subbands);
        for (m=0; m < nr_subbands; m++) {
            in=(int)in2[l+m*SIZE2] - (int)z_min[m]; // Make the input unsigned by adding an offset
            res=out[k]+(int)(in << length);  // Write the input on the right place (bits) in the output
            length=length+N[m]; // Increase the length
            if (length >= 16) {
                res=in - (int)((int)(in >> (16-(length-N[m]))) << (16-(length-N[m])));   // Select the least significant bits from the input
                out[k]=out[k]+(int)(res << (length-N[m]));   // Write the least significant bits on the right place in the output
                length=length-16;   // Decrease the length
                k=k+1;  // Increment k to write in the next output number
                out[k]=out[k]+(int)(in >> (N[m]-length));   // Write the most significant input bits in the next output number
            }
            else {
                out[k]=res;
            }
        }
    }
}

/* This function shifts the compressed number back to their original value */
void shiftBitsInv(int in[COMPRESSEDSIZE], short out1[MONOSIZE3BANDS], short out2[MONOSIZE3BANDS]) {

	short N[nr_subbands];
	short z_min[nr_subbands];

	N[0]=5;
	N[1]=5;
	N[2]=2;
	//N[3]=0;

	z_min[0]=-16;
	z_min[1]=-16;
	z_min[2]=-2;
	//z_min[3]=0;

    int i, j, k=0, l, m;
    int length = 0, rest = 0, LSB = 0, MSB = 0, out = 0;
	#pragma MUST_ITERATE(SIZE2,SIZE2,SIZE2);
	#pragma UNROLL(SIZE2);
    for (j=0; j < SIZE2; j++) {
		#pragma MUST_ITERATE(nr_subbands,nr_subbands);
		#pragma UNROLL(nr_subbands);
        for (i=0; i < nr_subbands; i++) {
            length=length+N[i]; // Increase the length (number of bits of the output number that are already filled)
            if (length <= 16) {
                out=(int)in[k] - (int)((int)(in[k] >> length) << length) - rest;    // Select the right part (bits) of the input
                rest=rest+out;  // Increase the rest (portion of the input number to select the right part of input)
                out=(int)(out >> (length-N[i]));    // Shift the result to make it an N-bit number
                out1[j+i*SIZE2]=(short)out + z_min[i];  // Make the result signed again and write it to the output
            }
            else {
                LSB=in[k] - rest;   // Select the least significant bits of the input
                LSB=LSB >> (length-N[i]);   // Shift the LSB to make it a length-N bit number
                length=length-16;   // Decrease the length
                MSB=in[k+1] - (int)((int)(in[k+1] >> length) << length);    // Select the right part (bits) of the input
                out=LSB + (int)((int)MSB << (N[i]-length)); // Shift the MSB to make it an N-bit number
                out1[j+i*SIZE2]=(short)out + z_min[i];  // Make the result signed again and write it to the output
                rest=0;
                k=k+1; // Increment k to use the next input number
            }
        }
    }
    for (l=0; l < SIZE2; l++) {
		#pragma MUST_ITERATE(nr_subbands,nr_subbands);
		#pragma UNROLL(nr_subbands);
        for (m=0; m < nr_subbands; m++) {
            length=length+N[m]; // Increase the length
            if (length <= 16) {
                out=(int)in[k] - (int)((int)(in[k] >> length) << length) - rest;    // Select the right part (bits) of the input
                rest=rest+out;  // Increase the rest
                out=(int)(out >> (length-N[m]));    // Shift the result to make it an N-bit number
                out2[l+m*SIZE2]=(short)out + z_min[m];    // Make the result signed again and write it to the output
            }
            else {
                LSB=in[k] - rest;   // Select the least significant bits of the input
                LSB=LSB >> (length-N[m]);   // Shift the LSB to make it a length-N bit number
                length=length-16;   // Decrease the length
                MSB=in[k+1] - (int)((int)(in[k+1] >> length) << length);    // Select the right part (bits) of the input
                out=LSB + (int)((int)MSB << (N[m]-length)); // Shift the MSB to make it an N-bit number
                out2[l+m*SIZE2]=(short)out + z_min[m];  // Make the result signed again and write it to the output
                rest=0;
                k=k+1; // Increment k to use the next input number
            }
        }
    }
}
