% Hunter Phillips
% 09/02/2017
% 3D Plot for Uncertainty Analysis and Error - plot redraw -3 lines -
% ultrasonic sensor

clear all
clc

Excel = actxserver ('Excel.Application'); 
File='C:\Users\robolux\Desktop\311robot\test.xls'; 
if ~exist(File,'file') 
ExcelWorkbook = Excel.workbooks.Add; 
ExcelWorkbook.SaveAs(File,1); 
ExcelWorkbook.Close(false); 
end 
invoke(Excel.Workbooks,'Open',File);

trip = 2;
w=0;
data2 = [];
value = [];
x=1:100000;
arduino=serial('COM3','BaudRate',600);

view([-55.1 41.2]);
y = [];
x2 = [];
h3 = plot3(1,1,1);
h2 = plot3(1,1,1);

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
    
    my_cell = sprintf( 'A%s',num2str(w) );
    xlswrite1('test.xls',value(w),1,my_cell)
    
    if length(x2) > 25
        x2(1) = [];
        value(1) = [];
    end
    pause(0.001)
end

invoke(Excel.ActiveWorkbook,'Save'); 
Excel.Quit 
Excel.delete 
clear Excel