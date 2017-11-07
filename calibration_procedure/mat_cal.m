clear all
clc
 
answer=1; 
arduino=serial('COM3','BaudRate',600);
 
fopen(arduino); % initiate arduino communication
 
while answer
    fprintf(arduino,'%s',char(answer)); % send answer variable content to arduino
    answer=input('Enter 1 to increment the stepper motor\nOr 2 to take a bend sensor reading: '); % ask user to enter value for variable answer
end
 
fclose(arduino); % end communication with arduino