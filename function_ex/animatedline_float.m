% Float input

% Fast animated line reading from sensor 

clear all
clc

trip = 2;
w=0;
data2 = [];
value = [];
x=1:100000;
arduino=serial('COM3','BaudRate',9600);
h = animatedline('MaximumNumPoints',1000,'Clipping','on'); 
% ax = axes('Position',([0,1000,0,100]));
% set(ax,'ClippingStyle','rectangle')

fopen(arduino);
while(trip~=102)
    trip=fread(arduino,1);
end
data = 102;
data = [data; fread(arduino,11,'char')];
while(1)
    w=w+1;
    data2 = fread(arduino,12,'char');
    data_char = char(data2);
    data_converted = [data_char(9) data_char(10) data_char(7) data_char(8) data_char(5) data_char(6) data_char(3) data_char(4)];
    value = [value; hexsingle2num(data_converted)];
    addpoints(h,x(w),value(w));
    drawnow   
end

% What's next for me reading add compass livetime functionality
