% Hunter Phillips
% 09/02/2017
% 3D Plot for Uncertainty Analysis and Error - Animated Line

grid on;
h = animatedline('MaximumNumPoints', 100);
h2 = animatedline('MaximumNumPoints', 100);
h3 = animatedline('MaximumNumPoints', 20);
%plot3([a;a]',8*ones(size([a;a]))',[zMaxY;zMinY]')
% Force a 3D view
view(3);
% axis([-100 100 -100 100 -1 100]);
addpoints(h3,2,1,1);
for x = 1:10000
    y = sin(x);
    addpoints(h,x,y,-1);
    addpoints(h2,x,1,y);
    
%     addpoints(h,-x,y,-1); %  2nd Work
%     addpoints(h2,3,x,y);

%     [x5,y5,z5] = getpoints(h)
%     xl = xlim
%     [x2,y2,z2] = getpoints(h3)
%     clearpoints(h3)
%     addpoints(h3,xl(2)*ones(length(x2)),x2,y2);

    drawnow update
    pause(0.1)
end
