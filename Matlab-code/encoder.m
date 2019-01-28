function [z, s_star]=encoder(s, stepsize, mu)

% Initialisation
z=zeros(length(s), 1);
d_prime=zeros(length(s), 1);
s_prime=zeros(length(s), 1);
s_star=zeros(length(s), 1);

% Calculate the mean s, s_prev and difference
s_prev=[0; s(1:end-1)]; 
d=s-s_prev;

% Quantisation first sample
z(1)=round(d(1)/stepsize);

% Dequantisation first sample
d_prime(1)=stepsize*z(1);

% Recombining signals first sample
s_prime(1)=d_prime(1);    

% Predictor first sample
s_star(1)=mu*s_prime(1);

for n=2:length(s)
    
    % Difference
    d(n)=s(n)-s_star(n-1);
    
    % Quantisation
    z(n)=round(d(n)/stepsize);
    
    % Dequantisation
    d_prime(n)=stepsize*z(n);
    
    % Recombining signals
    s_prime(n)=d_prime(n)+s_star(n-1);
    
    % Predictor
    s_star(n)=mu*s_prime(n);
    
end