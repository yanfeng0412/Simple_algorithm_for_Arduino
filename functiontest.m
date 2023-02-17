
tf = 2;
t= 0:0.1:tf;
tx = tf: 0.1 : tf+2;

thetaf =45;
theta0 = 135;

thetaf1 = 135;
theta01 = 45;


%% 三次样条135到45
a0 = theta0;
a1 = 0;
a2 = 3/(tf^2)*(thetaf - theta0);
a3 = -2/(tf^3)*(thetaf - theta0);
theta0 = a0 + a1.*(t) +a2.*((t).^2) +a3.*((t).^3);
% t0 = a0 + a1.*(t) +a2.*((t).^2) +a3.*((t).^3);
subplot(3,1,1)
plot(t,theta0);

%% 三次样条45 到 135
a01 = theta01; a11 = 0;
a21 = 3/(tf^2)*(thetaf1 - theta01);
a31 = -2/(tf^3)*(thetaf1 - theta01);
theta1 = a01 + a11.*(t) +a21.*((t).^2) +a31.*((t).^3);
subplot(3,1,2)
plot(t,theta1);

%% 五次样条曲线
thi =45 ;thf =135; 
omegai = 15; omegaf = 2;
acci = 0 ; accf =6;
a0_5 = thi;
a1_5 = omegai; 
a2_5 = acci/2;
a3_5 = (20*thf-20*thi-(8*omegaf+12*omegai)*tf-(3*acci-accf)*(tf^2)) / (2*(tf^3));
a4_5 = (30*thi-30*thf+(14*omegaf+16*omegai)*tf+(3*acci-2*accf)*(tf^2)) / (2*tf^4);
a5_5 = (12*thf - 12*thi - (6*omegaf+6*omegai)*tf-(acci-accf)*(tf^2)) /(2*tf^5);

th0_5 = a0_5 +a1_5.*t +a2_5.*t.^2+ a3_5.*t.^3 + a4_5.*t.^4 + a5_5.*t.^5;
figure (2)
subplot(3,1,1)
plot(t,th0_5);

thi =135 ;thf =45; 
omegai = 15; omegaf = 2;
acci = 0 ; accf =6;
a0_51 = thi;
a1_51 = omegai;
a2_51 = acci/2;
a3_51 = (20*thf-20*thi-(8*omegaf+12*omegai)*tf-(3*acci-accf)*(tf^2)) / (2*(tf^3));
a4_51 = (30*thi-30*thf+(14*omegaf+16*omegai)*tf+(3*acci-2*accf)*(tf^2)) / (2*tf^4);
a5_51 = (12*thf - 12*thi - (6*omegaf+6*omegai)*tf-(acci-accf)*(tf^2)) /(2*tf^5);

th0_51 = a0_51 +a1_51.*t +a2_51.*t.^2+ a3_51.*t.^3 + a4_51.*t.^4 + a5_51.*t.^5;
figure (2)
subplot(3,1,2)
plot(t,th0_51);
