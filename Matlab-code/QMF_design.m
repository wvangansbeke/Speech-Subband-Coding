function [h0, h1, f0, f1] = QMF_design(fs, df, Astop, fstep, Niter, FLength);

% PURPOSE: Design of the LowPass (0) and HighPass (1) linear phase impulse
%	   responses of the analysis (h) and synthesis (f) filters for a
%	   near-perfect reconstruction QMF filterbank (perfect alias
%	   cancellation).
% 	   The design method is as described in Bobrek & Koch, "Music signal
%	   segmentation using Tree-Structured Filter Banks", JAES 46, nr 5,
%	   pp. 412-427, May 1998.
%
% CALL:    [h0, h1, f0, f1] = QMF_design(fs, df, Astop, fstep, Niter, Flength);
%
% INPUT: fs       -- sampling freq. in Hz (default: 44100)
%        df       -- total transition width of prototype in Hz (default: fs/10)
%        Astop    -- min. stopband attenuation of prototype in dB (default: 60)
% 	 fstep    -- freq step for finding optimal fc in Hz(default: fs/4410)   
%	 Niter	  -- max number of iterations (default: 100)
%	 FLength  -- the desired filter length (nr of taps); this must be even
%		     (if specified, the resulting filter will not necessarily
%		     obey the specs; if unspecified, the required order is
%		     computed from to the specifications df and Astop)
%
% OUTPUT: h0      -- LowPass  linear phase impulse response (analysis filter)
%         h1	  -- HighPass linear phase impulse response (analysis filter)
%	  f0      -- LowPass  linear phase impulse response (synthesis filter)
%	  f1      -- HighPass linear phase impulse response (synthesis filter)
%

% Author: Werner Verhelst
% Version: 1.2001.5.30

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Stap 1: Ontwerp een Kaiser gevensterd LDF 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

if nargin < 1
   fs=44100;
end;
if nargin < 2
   df=fs/10;
end;
if nargin < 3
   Astop=60;
end;
if nargin < 4
   fstep=fs/4410;
end;
if nargin < 5
   Niter=100;
end;
if nargin < 6
   FLength=0; % this ensures that kaiser designs a filter with an even nr of taps
end;

F=fs/4;					%Cut_off frequentie (digitaal=pi/2)  
fc=F;

hLDF=kais(fc, fs, df, Astop, FLength);	%Ontwerp van de gevensterde lp en hp filters,lengte IR even,
NFFT=max(1024,10*length(hLDF));
H_LDF=fft(hLDF,NFFT);      %De FFT_orde moet veel groter zijn dan de filter orde   
H_HDF=fftshift(H_LDF);

%H0test=H_LDF;
%H1test=H_HDF;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Stap 2: Bereken de vermogen complementaire fout epsi 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

epsi(1)=max(abs(10*log10(abs(H_LDF).^2 + abs(H_HDF).^2)));
disp(['epsi old cutoff: ',num2str(epsi(1)), '    Processing ... please wait ...']); %geef kleinste fout 

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Stap 3: fc bijstellen om de fout te minimaliseren
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

k=1;                %k is een teller om na te gaan bij welke fc epsi minimaal is
fc=F+fs/20;
while fc > F-fs/20  %cuttoff frequentie varieren,filter en fout herberekenen.    
   fc=fc-fstep;
  
  	%Herberekenen van laag-en hoogdoorlaatfilter 
	hLDF=kais(fc, fs, df, Astop, FLength);
  	H_LDF=fft(hLDF,NFFT);
   H_HDF=fftshift(H_LDF);
  
	%nieuwe fout berekenen
   epsin=max(abs(10*log10(abs(H_LDF).^2+abs(H_HDF).^2))); 
  
  if epsin<epsi,			%huidige fout kleiner dan vorige
     epsi(2)=epsin;		%fout = huidige
     ki=k;
  end
  k=k+1;
end

disp(['epsi new cutoff: ',num2str(epsi(2))]);	%geef kleinste fout 
fc=F-fstep*ki+fs/20										%fc waarbij de fout het kleinst is 

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Stap 4: Herbereken laag-en hoogdoorlaatfilter bij deze fc waarbij fout
%	  minimaal is
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

hLDF=kais(fc, fs, df, Astop, FLength);
H_LDF=fft(hLDF,NFFT);
H_HDF=fftshift(H_LDF);
figure(1);
clf;
plot(0:fs/NFFT:fs-fs/NFFT,20*log10(abs(H_LDF)));title('HLDF & HHDF after fc optimization');
hold on
plot(0:fs/NFFT:fs-fs/NFFT,20*log10(abs(H_HDF)));grid
zoom on

%einde zoektocht naar fc waarbij vermogen complementaire fout minimaal is

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Stap 5 : Bereken een nieuwe frequentierespons HIIR
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

for w=1:length(H_LDF)
	HIIR(w)=H_LDF(w)/sqrt(abs(H_LDF(w))^2 + abs(H_HDF(w))^2);;
end;
% kan sneller: HIIR = H_LDF./sqrt(abs(H_LDF).^2 + abs(H_HDF).^2);

h_iir=ifft(HIIR);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Stap 6: vermenigvuldig h_iir met een rechthoekig venster zodat alleen de
%	  lengte van hFIR (v/d M punten) waarden verkrijgen <> 0. 
%	  hFIR = hIIR n<N
%                 0   n>=N 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

hfir=h_iir(1:length(hLDF));
hLDF=hfir;

%plotten van HFIR

figure(2);
clf;
HFIR=fft(hfir,NFFT);
plot(0:fs/NFFT:fs-fs/NFFT,20*log10(abs(HFIR)));title('HFIR after 1 iteration');
grid
zoom on

%***********************************************************************
%Tot hiertoe heb ik voor de 1e maal een nieuwe frequentierespons :HFIR
%***********************************************************************

%De volgende stap is het ontwerp van HIIR iteratief herhalen tot de vermogen 
%complementaire fout aanvaardbaar klein is

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Stap 7 : Bereken de vermogen complementaire fout voor hfir; stop als ok 
%          of verhoog de index en ga terug naar Stap 5 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

H_LDF=fft(hLDF,NFFT);
H_HDF=fftshift(H_LDF);
figure(2);
hold on
plot(0:fs/NFFT:fs-fs/NFFT,20*log10(abs(H_HDF)));
zoom on
p=1;
epsin(p)=max(abs(10*log10(abs(H_LDF).^2+abs(H_HDF).^2))); 

keepgoing = 1;
while ((p < Niter) & (keepgoing == 1))
%Herhalen van Stap 5
%********************
	temp= sqrt(abs(H_LDF).^2 + abs(H_HDF).^2);
   HIIR=H_LDF./temp;
   h_iir=ifft(HIIR); 

	%Herhalen van Stap 6
	%********************
	hfir=h_iir(1:length(hLDF));
	hLDF=hfir;

	%berekenen van het hoogdoorlaat filter
	H_LDF=fft(hLDF,NFFT);
	H_HDF=fftshift(H_LDF);
	p=p+1;
	epsin(p)=max(abs(10*log10(abs(H_LDF).^2+abs(H_HDF).^2)));
	%if (epsin(p) > epsin(p-1))
   %   keepgoing=0; % stop !
   % end;
   % Often epsin increases a little after a few iterations, but after that it
   % usually drops off monotonically
end

disp(['epsi final: ',num2str(epsin(p))]);		%geef kleinste fout 
figure(3);
plot(epsin);

%plotten van HFIR wanneer de  vermogen complementaire fout voldoende klein is
hfir=real(hfir);
hhp=(-1).^(0:length(hfir)-1).*hfir;
figure(4);
clf;
subplot(2,2,1), stem(hfir);
subplot(2,2,2), stem(hhp);
HFIR=fft(hfir,NFFT);
H_HDF=fft(hhp,NFFT);
subplot(2,1,2);
plot(0:fs/NFFT:fs-fs/NFFT,20*log10(abs(HFIR)),'red');grid;
title('H0,H1');
hold on
plot(0:fs/NFFT:fs-fs/NFFT,20*log10(abs(H_HDF)));
zoom on
 
figure(5);
clf;
subplot(2,1,1);
plot(0:fs/NFFT:fs-fs/NFFT,20*log10(abs(H_LDF.^2 - H_HDF.^2))); 
title('QMF reconstruction (dB)');
subplot(2,1,2);
plot(real(ifft(H_LDF.^2 - H_HDF.^2)));
title('QMF reconstruction - impulse response');
 
h0=hfir;
h1=hhp;
f0=2*h0;
f1=-2*h1; 
