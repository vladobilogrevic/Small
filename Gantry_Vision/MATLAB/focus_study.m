pkg load optim
Data = ...
  [0.0000    5.8955
   0.1000    3.5639
   0.2000    2.5173
   0.3000    1.9790
   0.4000    1.8990
   0.5000    1.3938
   0.6000    1.1359
   0.7000    1.0096
   0.8000    1.0343
   0.9000    0.8435
   1.0000    0.6856
   1.1000    0.6100
   1.2000    0.5392
   1.3000    0.3946
   1.4000    0.3903
   1.5000    0.5474
   1.6000    0.3459
   1.7000    0.1370
   1.8000    0.2211
   1.9000    0.1704
   2.0000    0.2636];

t = Data(:,1);
y = Data(:,2);
% axis([0 2 -0.5 6])
% hold on
plot(t,y,'ro')
title('Data points')
% hold off


F = @(x,xdata)x(1)*exp(-x(2)*xdata) + x(3)*exp(-x(4)*xdata);
x0 = [1 1 1 0];
[x,resnorm,~,exitflag,output] = lsqcurvefit(F,x0,t,y)

hold on
plot(t,F(x,t))
hold off

input('')
