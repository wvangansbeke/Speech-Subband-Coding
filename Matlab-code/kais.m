function h = kais(fc, fs, df, Astop, odd);

% PURPOSE: Returns the impulse response of a Kaiser lowpass filter
%
% CALL:    h = kais(fc, fs, df, Astop, odd)
%
% INPUT: fc       -- cutoff frequency
%        fs       -- sampling frequency
%	 df	  -- transition width
%	 Astop    -- stopband attenuation (dB)
%        odd      -- (default odd = 0) -> even length
%
%		     If odd==1 an odd length filter is designed
%		     If odd > 1, odd specifies the desired length of the filter
%
% OUTPUT: h       -- impulse response of a Kaiser lowpass filter
% 

% Werner Verhelst
% Version 1.2001.5.23

if nargin < 2
   fs=44100;
end;
if nargin < 3
   df=fs/10;
end;
if nargin < 4
   Astop=60;
end;
if nargin < 5
   odd = 0;
end;

FiltLngth = odd;
odd = rem(odd,2);

%Berekenen van de fpass,fstop

fpass=fc-0.5*df;
fstop=fc+0.5*df;

%Berekenen van de genormaliseerde frequenties (digitale frequenties)

wc=2*pi*fc/fs;
wpass=2*pi*fpass/fs;
wstop=2*pi*fstop/fs;
dw=2*pi*df/fs;

A=Astop;

%Berekenen van beta

if A>50, 
	beta=0.1102*(A-8.7);
elseif 21<=A<=50,
	beta=0.5842*(A-21)^0.4+0.07886*(A-21);
else A<21,
	beta=0;
end

if FiltLngth > 1
   M = FiltLngth;
else
	% Berekenen van de filterlengte M, die voorspeld wordt op +-2 na.
	% M is de filterorde in de formule, dus de lengte -1 !
	% M=ceil((A-8)/(2.285*dw));
	M=ceil((A-8)/(2.285*dw))+1;
	% Ik ga om die +/- 2 aan de veilige kant te pakken er 2 bijlappen
	M=M+2;
	if rem(M,2) ~= odd
		M=M+1;
	end;
end;

%Berekenen van de window functie w(n)=Io(beta*sqrt(n*(2M-n))/M)/Io(beta)
%waarbij Io de gemodificierde Besselse functie is van de eerste soort en nulde orde

alfa=(M-1)/2;
n=1:(M+1)/2;
w(n)=besseli(0,beta*sqrt(1-[((n-1)-alfa)/(alfa)].^2))/besseli(0,beta);
w(M+1-n)=w(n);

%Berekenen van de laagdoorlaat imulsrespons hLD(n)=sin(wc(n-(M-1)/2))/(pi(n-(M-1)/2)) n=0:M-1

for k=-(M-1)/2:(M-1)/2,
	if k==0,
		hLD(k+(M-1)/2+1)=wc/pi;
	else  
   	hLD(k+(M-1)/2+1)=sin(wc*k)/(pi*k);
   end
end

%Berekenen van de Kaiser gevensterde laagdoorlaat impulsrespons h(n)=w(n)*hLD(n)
n=1:M;
h(n)=w(n).*hLD(n); 
