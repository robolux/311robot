% Project Demo for Dr. Armentrout
% Group 1
% 11/08/2017

clear all
clc

% Setup Excel X Server 
Excel = actxserver ('Excel.Application'); 
File='C:\Users\robolux\Desktop\311robot\demo\test3.xls'; 
if ~exist(File,'file') 
ExcelWorkbook = Excel.workbooks.Add; 
ExcelWorkbook.SaveAs(File,1); 
ExcelWorkbook.Close(false); 
end 
invoke(Excel.Workbooks,'Open',File); % open the invoked Excel workbook

mapc = @(x) 4.2619958863*(10^(-10))*x^3 - 2.1440571377*(10^(-5))*x^2 + 3.3771916419*(10^(-1))*x - 1.5811722057*(10^(3));

mean_angle=[];  % init mean_angle
trip = 2;       % dummy var
w=0;            % need w for assigning positions in arrays during while loop
data2 = [];     % init data 2 to null matrix
value = [];     % initi value to null matrix
figure('units','normalized','outerposition',[0 0 1 1])  % setup the figure
comp = compass(1,1);                        % compass graph
arduino=serial('COM4','BaudRate',1200);     % open up the com port for the arduino
fopen(arduino);                             % at a baud rate of 1200

while(trip~=102)            % the first bytes before 102 of information need
    trip=fread(arduino,1);  % to be read and assigned to a dummy variable
end                         % they contain unreadable characters / symbols

data = 102;                                 % Read one more byte chain 
data = [data; fread(arduino,11,'char')];    % and dump the data into a dummy var
                                            % it should be valid, however ensures that proper flushing occurs 
                                        
while(1)            % infinite loop
    delete(comp)    % every iteration need to delete the old arrow from compass plot -> for speed
    w=w+1;          % iterate for array assignment advancement
    data2 = fread(arduino,12,'char');   % get data from serial com
    data_char = char(data2);            % ASCII is converted from raw data chain
    data_converted = [data_char(9) data_char(10) data_char(7) data_char(8) data_char(5) data_char(6) data_char(3) data_char(4)];
    value = hexsingle2num(data_converted); % data comes in backwards and needs to be rearranged and converted from single precision IEEE hexadecimal string to number
    flexR = v2r(value);                    % value is in form of voltage, need to convert it to a resistance
    map_value = mapc(flexR)
    
    data_point = sprintf('A%s',num2str(w));
    v_col = sprintf('B%s',num2str(w));
    r_col = sprintf('C%s',num2str(w));
    a_col = sprintf('D%s',num2str(w));
    xlswrite1('test3.xls',w,1,data_point)       % record data point number
    xlswrite1('test3.xls',value,1,v_col)        % record voltage
    xlswrite1('test3.xls',flexR,1,r_col)        % record resistance
    xlswrite1('test3.xls',map_value,1,a_col)    % record angle
    
    mean_angle = [mean_angle, map_value];                                           % get the mean angle values stored from the measurements
    [x_c,y_c] = pol2cart(map_value*(pi/180),1);                                     % convert angle value from polar to cartesian cord. 
    comp = compass(x_c,y_c);                                                        % create compass polt with angle
    ylim([0 1])                                                                     % only show top half of plot
    if mod(w,250)==0                                                                % every ~250 measurements ask user for input
        fprintf('Mean Angle Measurement : %.2f\n\n',mean(mean_angle))               % Show user the mean of the ~250 measurements
        mean_angle = [];
        user_dec=input('Enter 1 to take data points at position\nEnter 2 to end testing : '); % ask user to enter value for choice
        if user_dec == 2 % if they pick 3
            break        % exit loop and shutdown processes
        end
        fclose(arduino);
        fopen(arduino);                       
    end
    drawnow update % force the figure to update even if OPENGL denies it due to timing
end

fclose(arduino); % close arduino

% close Excel and save work
invoke(Excel.ActiveWorkbook,'Save'); 
Excel.Quit 
Excel.delete 
clear Excel

