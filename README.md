# Speech-Subband-Coding
In this project a subband filterbank is designed with adaptive quantisation for the compression of stereo
audio signals with fixed point numbers. This report regards the implementation of a system with the
following specifications: stereo sound, 16khz fs, 24kbits/s and 150ms max delay.

## Filterbank
The transition band is completely determined by a speciffc attenuation in the stopband and the filter or-
der. If a higher stopband attenuation or steeper transition band is required, a higher filter order needs to
used, introducing more latency. Furthermore, good performance in the subbands depends on frequency
selectivity. At higher frequencies the masking effect is spread over a wider range, resulting in a reduced
number of quantisation bits needed. Eventually, by using lower filter orders, a higher amount of overlap
and less frequency selectivity is introduced causing lower PESQ/SNR scores. The same information is
after all encoded with a different amount of bits to exploit the masking effect. On the other hand, a
higher filter length creates higher complexity and latency. The latter one is actually a soft constraint,
i.e. 150ms, and can easily be obtained. It can be divided into two parts. On the one hand the delay
introduced by the buffer communication and on the other hand the delay due to the convolution (roughly
the filter length divided by a factor of 2). By iterating over multiple filter lengts, the amount of filter taps 
was determined.

## ADPCM
The quantisation algorithm used in this application is called ADPCM or Adaptive Differential Pulse
Code Modulation. This relies on the use of PCM, which is in fact a discretization of a continuous
(speech) signal. It is done by quantizing the continuous signal with a given number of bits N.
The differential in the name refers to quantizing the difference between succeeding signal samples instead
of the signal samples themselves. What is actually quantized in this use of the algorithm is the difference
between the signal sample s_n and its previous estimated sample s*\_n. This leads to smaller signals that
need to be quantized, hence smaller differences between the different samples and thus also a more precise
quantisation with a small number of bits.
The final part of the algorithm is the adaptive one. This means something changes every iteration based
on the previous outcome, which could be the step size or the number of bits to quantize the signal
with. In this specific application there is chosen to make the number of bits constant for a subband and
adapt the step size throughout the signal. This is done in an AQB sense or Adaptive Quantisation with
Backwards estimation. Consequently, the quantized levels are adapted automatically to the range of the
signal in a certain window.

# Usage
The implementation in both Matlab and C can be found in their respective folders.
