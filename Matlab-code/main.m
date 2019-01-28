clear all
close all

%% Initialisation
fs=32000;
df=fs/10;
Astop=60;
L=64;
NFFT=1024;
iter=100;

%% Filter design
% Determine h0, f0 %lengtes belangrijk te kiezen om zelfde delay te hebben
% als ge zaken optelt. 
[h0, ~, f0, ~]=QMF_design(fs, df, Astop, 10, iter, L);
% Determine h2, f2
[h2, ~, f2, ~]=QMF_design(fs/2, df, Astop, 10, iter, L/2);
% Determine h4, f4
[h4, ~, f4, ~]=QMF_design(fs/2, df, Astop, 10, iter, L/2);
% Determine h6, f6
[h6, ~, f6, ~]=QMF_design(fs/4, df, Astop, 10, iter, L/4);
% Determine h8
[h8, ~, f8, ~]=QMF_design(fs/4, df, Astop, 10, iter, L/4);
% Determine h9
[h10, ~, f10, ~]=QMF_design(fs/4, df, Astop, 10, iter, L/4);
% Determine h8
[h12, ~, f12, ~]=QMF_design(fs/4, df, Astop, 10, iter, L/4);


%% Signal analysis
% Load the signal
[x, fs, nbits, nr_channels] = LoadWav_new('belasting.wav');

% Analysis
[x6,x7,x8,x9,x10,x11,x12,x13]=analysis(x,h0,h2,h4,h6,h8,h10,h12);

%% Encoding subbands
% N4=4;
% stepsize4=(max(x4)-min(x4))/(2^N4);
% mu4=0.65;
% [z4, s_star4]=encoder(x4, stepsize4, mu4);
% 
% N5=4;
% stepsize5=(max(x5)-min(x5))/(2^N5);
% mu5=0.75;
% [z5, s_star5]=encoder(x5, stepsize5, mu5);
% 
% N6=7;
% stepsize6=(max(x6)-min(x6))/(2^N6);
% mu6=0.80;
% [z6, s_star6]=encoder(x6, stepsize6, mu6);
% 
% N7=7;
% stepsize7=(max(x7)-min(x7))/(2^N7);
% mu7=0.30;
% [z7, s_star7]=encoder(x7, stepsize7, mu7);
% 
% N8=7;
% stepsize8=(max(x8)-min(x8))/(2^N8);
% mu8=0.15;
% [z8, s_star8]=encoder(x8, stepsize8, mu8);
% 
% N9=7;
% stepsize9=(max(x9)-min(x9))/(2^N9);
% mu9=0.60;
% [z9, s_star9]=encoder(x9, stepsize9, mu9);
% 
% %% Decoding subbands
% y4=decoder(z4, s_star4, stepsize4);
% y5=decoder(z5, s_star5, stepsize5);
% y6=decoder(z6, s_star6, stepsize6);
% y7=decoder(z7, s_star7, stepsize7);
% y8=decoder(z8, s_star8, stepsize8);
% y9=decoder(z9, s_star9, stepsize9);
% 
% %% SNR
% SNR4=snrseg(y4, x4, fs/2);
% disp(['SNR4: ',num2str(SNR4)]);
% SNR5=snrseg(y5, x5, fs/2);
% disp(['SNR5: ',num2str(SNR5)]);
% SNR6=snrseg(y6, x6, fs/4);
% disp(['SNR6: ',num2str(SNR6)]);
% SNR7=snrseg(y7, x7, fs/4);
% disp(['SNR7: ',num2str(SNR7)]);
% SNR8=snrseg(y8, x8, fs/4);
% disp(['SNR8: ',num2str(SNR8)]);
% SNR9=snrseg(y9, x9, fs/4);
% disp(['SNR9: ',num2str(SNR9)]);
% 
% %% Signal synthesis
[y]=synthesis(x6,x7,x8,x9,x10,x11,x12,x13,f0,f2,f4,f6,f8,f10,f12);

% 
%%
[acor,lag] = xcorr(y,x);
[~,I] = max(abs(acor));
lagDiff = lag(I);
timeDiff = lagDiff/fs;

out=zeros(length(y),1);
out(1:end-lagDiff)=y(lagDiff+1:end);

%% Sound
%sound(y,2*fs)

%% Plots
figure
plot(20*log10(abs(fft(x))),'red')
hold on
plot(20*log10(abs(fft(y))),'blue')
% figure
% subplot(2,1,1)
% stem(x)
% subplot(2,1,2)
% stem(y, 'red')
figure
stem(x-out)