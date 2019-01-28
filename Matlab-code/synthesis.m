function y=synthesis(y6,y7,y8,y9,y10,y11,y12,y13,f0,f2,f4,f6,f8,f10,f12)

% Sum
v6=y6+y7;
v7=y6-y7;

% Polyphase decomposition
e6=f6(2:2:end);
e7=f6(1:2:end);

% Interleaving
y2=zeros(2*length(v6), 1);
y2(1:2:end)=filter(e6, 1, v7);
y2(2:2:end)=filter(e7, 1, v6);

% Sum
v8=y8+y9;
v9=y8-y9;

% Polyphase decomposition
e8=f8(2:2:end);
e9=f8(1:2:end);

% Interleaving
y3=zeros(2*length(v8), 1);
y3(1:2:end)=filter(e8, 1, v9);
y3(2:2:end)=filter(e9, 1, v8);


% Sum
v10=y10+y11;
v11=y10-y11;

% Polyphase decomposition
e10=f10(2:2:end);
e11=f10(1:2:end);

% Interleaving
y4=zeros(2*length(v10), 1);
y4(1:2:end)=filter(e10, 1, v11);
y4(2:2:end)=filter(e11, 1, v10);

% Sum
v12=y12+y13;
v13=y12-y13;

% Polyphase decomposition
e12=f12(2:2:end);
e13=f12(1:2:end);

% Interleaving
y5=zeros(2*length(v12), 1);
y5(1:2:end)=filter(e12, 1, v13);
y5(2:2:end)=filter(e13, 1, v12);




% Sum
v4=y4+y5;
v5=y4-y5;

% Polyphase decomposition
e4=f4(2:2:end);
e5=f4(1:2:end);

% Interleaving
y1=zeros(2*length(v4), 1);
y1(1:2:end)=filter(e4, 1, v5);
y1(2:2:end)=filter(e5, 1, v4);


% Sum
v2=y2+y3;
v3=y2-y3;

% Polyphase decomposition
e2=f2(2:2:end);
e3=f2(1:2:end);

% Interleaving
y0=zeros(2*length(v2), 1);
y0(1:2:end)=filter(e2, 1, v3);
y0(2:2:end)=filter(e3, 1, v2);


% Sum
v0=y0+y1;
v1=y0-y1;

% Polyphase decomposition
e0=f0(2:2:end);
e1=f0(1:2:end);

% Sum of subbands
y=zeros(2*length(v0), 1);
y(1:2:end)=filter(e0, 1, v1);
y(2:2:end)=filter(e1, 1, v0);
end


