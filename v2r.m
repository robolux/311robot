function flexR = v2r( volt_V )
% Hunter Phillips
% Converts voltage to Resistance for Flex Sensor
% 9/9/2017

R_DIV = 46580.0;    % Measured resistance of 47k resistor
VCC = 5.042;        % Measured voltage of 5V line

flexR = R_DIV * (VCC / volt_V - 1.0);

end

