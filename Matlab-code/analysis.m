function [x6,x7,x8,x9,x10,x11,x12,x13]=analysis(x, h0,h2,h4,h6,h8,h10,h12)

% Polyphase decomposition
e0=h0(2:2:end);
e1=h0(1:2:end);

% Convolution from even x with e0 and odd x with e1
u0=filter(e0, 1, x(2:2:end));
u1=filter(e1, 1, x(1:2:end));

% Sum
x0=u0+u1;
x1=u0-u1;

% Polyphase decomposition
e2=h2(2:2:end);
e3=h2(1:2:end);

% Convolution from even u0 with e2 and odd u0 with e3
u2=filter(e2, 1, x0(2:2:end));
u3=filter(e3, 1, x0(1:2:end));

% Sum
x2=u2+u3;
x3=u2-u3;

% Polyphase decomposition
e4=h4(2:2:end);
e5=h4(1:2:end);

% Convolution from even u1 with e4 and odd u1 with e5
u4=filter(e4, 1, x1(2:2:end));
u5=filter(e5, 1, x1(1:2:end));

% Sum
x4=u4+u5;
x5=u4-u5;

% Polyphase decomposition
e6=h6(2:2:end);
e7=h6(1:2:end);

% Convolution from even u2 with e6 and odd u2 with e7
u6=filter(e6, 1, x2(2:2:end));
u7=filter(e7, 1, x2(1:2:end));

% Sum
x6=u6+u7;
x7=u6-u7;

% Polyphase decomposition
e8=h8(2:2:end);
e9=h8(1:2:end);

% Convolution from even u3 with e8 and odd u3 with e9
u8=filter(e8, 1, x3(2:2:end));
u9=filter(e9, 1, x3(1:2:end));

% Sum
x8=u8+u9;
x9=u8-u9;


% Polyphase decomposition
e10=h10(2:2:end);
e11=h10(1:2:end);

% Convolution from even u3 with e8 and odd u3 with e9
u10=filter(e10, 1, x4(2:2:end));
u11=filter(e11, 1, x4(1:2:end));

% Sum
x10=u10+u11;
x11=u10-u11;

% Polyphase decomposition
e12=h12(2:2:end);
e13=h12(1:2:end);

% Convolution from even u3 with e8 and odd u3 with e9
u12=filter(e12, 1, x5(2:2:end));
u13=filter(e13, 1, x5(1:2:end));

% Sum
x12=u12+u13;
x13=u12-u13;

%PLOTS
% figure
% plot(20*log10(abs(fft(x)))) %plot spectrum input wave file
% title('Spectrum Input')
% legend('Input Data')
% figure
% plot(20*log10(abs(fft(x0)))) %plot output spectrum after h0
% hold on
% plot(20*log10(abs(fft(x1))), 'red') %plot output spectrum after h1
% legend('x0','x1')
% title('Analysis signals')
% figure
% plot(0:fs/(2*NFFT):fs/2-fs/(2*NFFT),20*log10(abs(fft(u2,NFFT)))) %plot output spectrum after h1
% hold on
% plot(0:fs/(2*NFFT):fs/2-fs/(2*NFFT),20*log10(abs(fft(u3,NFFT))), 'red') %plot output spectrum after h1
% hold on
% plot(0:fs/(2*NFFT):fs/2-fs/(2*NFFT),20*log10(abs(fft(u4,NFFT))), 'green') %plot output spectrum after h1
% hold on
% plot(0:fs/(2*NFFT):fs/2-fs/(2*NFFT),20*log10(abs(fft(u5,NFFT))), 'yellow') %plot output spectrum after h1
% legend('x2','x3','x4','x5')
% title('Analysis signals')
% figure
% plot(0:fs/(4*NFFT):fs/4-fs/(4*NFFT),20*log10(abs(fft(u6,NFFT)))) %plot output spectrum after h1
% hold on
% plot(0:fs/(4*NFFT):fs/4-fs/(4*NFFT),20*log10(abs(fft(u7,NFFT))), 'red') %plot output spectrum after h1
% hold on
% plot(0:fs/(4*NFFT):fs/4-fs/(4*NFFT),20*log10(abs(fft(u8,NFFT))), 'green') %plot output spectrum after h1
% hold on
% plot(0:fs/(4*NFFT):fs/4-fs/(4*NFFT),20*log10(abs(fft(u9,NFFT))), 'yellow') %plot output spectrum after h1
% legend('x6','x7','x8','x9')
% title('Analysis signals')
% figure
% stem(e0) %plot output spectrum after h0
% hold on
% stem(e1, 'red') %plot output spectrum after h1
% figure
% plot(0:fs/(2*NFFT):fs/2-fs/(2*NFFT),20*log10(abs(fft(h2,NFFT)))) %plot output spectrum after h1
% hold on
% plot(0:fs/(2*NFFT):fs/2-fs/(2*NFFT),20*log10(abs(fft(h3,NFFT))), 'red') %plot output spectrum after h1
% hold on
% plot(0:fs/(2*NFFT):fs/2-fs/(2*NFFT),20*log10(abs(fft(h4,NFFT))), 'green') %plot output spectrum after h1
% hold on
% plot(0:fs/(2*NFFT):fs/2-fs/(2*NFFT),20*log10(abs(fft(h5,NFFT))), 'yellow') %plot output spectrum after h1
% legend('h2','h3','h4','h5')
% title('Analysis filters')
% figure
% plot(0:fs/(4*NFFT):fs/4-fs/(4*NFFT),20*log10(abs(fft(h6,NFFT)))) %plot output spectrum after h1
% hold on
% plot(0:fs/(4*NFFT):fs/4-fs/(4*NFFT),20*log10(abs(fft(h7,NFFT))), 'red') %plot output spectrum after h1
% hold on
% plot(0:fs/(4*NFFT):fs/4-fs/(4*NFFT),20*log10(abs(fft(h8,NFFT))), 'green') %plot output spectrum after h1
% hold on
% plot(0:fs/(4*NFFT):fs/4-fs/(4*NFFT),20*log10(abs(fft(h9,NFFT))), 'yellow') %plot output spectrum after h1
% legend('h6','h7','h8','h9')
% title('Analysis filters')
% figure
% stem(u0)
% figure
% stem(u1)
% figure
% stem(u2)
% figure
% stem(u3)
% figure
% stem(u4)
% figure
% stem(u5)
% figure
% stem(u6)
% figure
% stem(u7)
% figure
% stem(u8)
% figure
% stem(u9)