% Fast animated line reading from sensor 

clear all
clc

trip = 2;
w=0;
data2 = [];
value = [];
figure('units','normalized','outerposition',[0 0 1 1])
comp = compass(1,1);
x=1:100000;
arduino=serial('COM4','BaudRate',1200);
fopen(arduino);

while(trip~=102)
    trip=fread(arduino,1);
end

data = 102;
data = [data; fread(arduino,11,'char')];

while(1)
    delete(comp)
    w=w+1;
    data2 = fread(arduino,12,'char');
    data_char = char(data2);
    data_converted = [data_char(9) data_char(10) data_char(7) data_char(8) data_char(5) data_char(6) data_char(3) data_char(4)];
    value = [value; hexsingle2num(data_converted)];
    flexR = v2r(value(w));
    map_value = map(flexR,13213.39,47393.18,0.0,90.0)
    [x_c,y_c] = pol2cart(map_value*(pi/180),1);
    comp = compass(x_c,y_c);
    ylim([0 1])
    drawnow update
end

