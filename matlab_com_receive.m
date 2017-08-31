clear all
clc
 
arduino=serial('COM3','BaudRate',9600);
 
fopen(arduino);

x=1:10000000;
x2=fscanf(arduino,'%i');
for i=1:length(x)
    y(i)=fscanf(arduino,'%d');
    ylim([0 100])
    plot(x(1:length(y)),y(1:length(y)),'-')
    pause(.05)
end
