% Hunter Phillips
% 09/02/2017
% 3D Plot for Uncertainty Analysis and Error - plot redraw -3 lines

% view(3);
% axis([-100 100 -100 100 -1 100]);
view([-55.1 41.2]);
y = [];
x2 = [];
h3 = plot3(1,1,1);
h2 = plot3(1,1,1);
for x = 1:10000
    x2 = [x2, x];
    y = [y, sin(x)];
    delete(h3); delete(h2);
    h1 = plot3(x2,y,-1*ones(length(x2)));
    hold on
    xl = xlim;
    h2 = plot3(xl(2)*ones(length(x2)),x2,y);
    h3 = plot3(x2,xl(2)*ones(length(x2)),y);
    view([-55.1 41.2]);
    grid on;
    
    if length(x2) > 25
        x2(1) = [];
        y(1)  = [];
    end
    pause(0.1)
end
