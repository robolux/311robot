function flexR = v2r( volt_V )
% Hunter Phillips
% Converts voltage to Resistance for Flex Sensor
% 9/9/2017

R_DIV = 46880.0;    % Measured resistance of 47k resistor
VCC = 4.096;        % Measured voltage of 5V line

flexR = R_DIV * (VCC / volt_V - 1.0);

end

