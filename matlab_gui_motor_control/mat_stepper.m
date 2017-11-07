clear all
clc
 
answer=1; 
arduino=serial('COM3','BaudRate',600);
fopen(arduino); % initiate arduino communication

S.fh = figure('units','pixels',...
              'position',[300 300 200 100],...
              'menubar','none',...
              'name','GUI_9',...
              'numbertitle','off',...
              'resize','off');
S.pb = uicontrol('style','push',...
                 'unit','pix',...
                 'position',[20 20 160 60],...
                 'string','Push Me',...
                 'callback',{@pb_call},...
                 'backgroundc',[0.94 .94 .94],...
                 'busyaction','cancel',...% So multiple pushes don't stack.
                 'interrupt','off');
tog=0;             
while answer
    if tog==1
        disp('hi')
        fprintf(arduino,'%s',char(answer)); % send answer variable content to arduino
        answer=input('Enter led value 1,2, or 3:   '); % ask user to enter value for variable answer
        tog=0
    end
end
 
fclose(arduino); % end communication with ardu