function [] = ResetCreamino()

global s;

%Reset the Serial Port
s.DataTerminalReady = 'on';
pause(0.1);
s.DataTerminalReady = 'off';
pause(0.1);

end

