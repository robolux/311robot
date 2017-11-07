clear all
clc
 
answer=1; 
arduino=serial('COM4','BaudRate',600);
 
fopen(arduino); % initiate arduino communication
 
while answer
    fprintf(arduino,'%s',char(answer)); % send answer variable content to arduino
    answer=input('Enter led value 1,2, or 3:   '); % ask user to enter value for variable answer
end
 
fclose(arduino); % end communication with ardu