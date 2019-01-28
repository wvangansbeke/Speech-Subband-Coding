% Put the audio samples of a *.wav-file in a buffer with alternating L and R samples.
% Data is pairwise identical if mono input, interleaved if input is stereo.

% output buffer 'x' :
% ~~~~~~~~~~~~~~~~~~~
% Stereo : L1/R1/L2/R2/L3/R3/L4/R4/L5/R5/...
% Mono -> duplicate the channel => L1/L1/L2/L2/L3/L3/L4/L4/L5/L5/...

% --------------------
% For the Matlab version after 2015, the function "wavread" is not supported anymore.
% Instead, the function "audioread/audioinfo" can be used for the same purpose.
% --------------------
function [x fs nbits nr_channels] = LoadWav(file_path)


%[in,fs,nbits] = wavread(file_path);
[in,fs] = audioread(file_path);
wavinfo = audioinfo(file_path);
nbits = wavinfo.BitsPerSample;

nr_channels = size(in,2);

if (nr_channels == 2)
	temp = in';
	x = temp(:);
	y =1;
else
	in(:,2) = in;
	in = in';
	x = in(:);
	y = 2;
end
