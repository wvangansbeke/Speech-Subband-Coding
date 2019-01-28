function s_prime=decoder(z, s_star, stepsize)

% Initialisation
d_prime=zeros(length(z), 1);
s_prime=zeros(length(s_star), 1);

% Dequantisation first sample
d_prime(1)=stepsize*z(1);

% Recombining signals first sample
s_prime(1)=d_prime(1);

for n=2:length(z)
    
    % Dequantisation
    d_prime(n)=stepsize*z(n);
    
    % Recombining signals
    s_prime(n)=d_prime(n)+s_star(n-1);
    
end