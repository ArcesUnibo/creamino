function [  ] = StartADS()

%%
  %Configure the Serial Port
  %COMport = block.DialogPrm(1).Data;
  global COMport
  global s;
  s = serial(COMport);
  set(s, 'DataBits', 8);
  set(s, 'StopBits', 1);
  set(s, 'BaudRate', 256000);
  set(s, 'Parity', 'none');

  %Open the serial port
  fopen(s);

  %Sleep 100ms
  pause(0.1);
  
  ConnectingWord = 'Connect';
  %fprintf('Connect');
  fwrite(s,ConnectingWord,'char','sync'); 
  AvailableBytes = 0;
  
  while (AvailableBytes == 0)  
    %Wait untill bytes are available on the input buffer
    AvailableBytes = s.BytesAvailable;
	pause(0.01);
  end	
  
  AckWord = fscanf(s,'char',5);
  if(~strcmp(AckWord,'Ready'))
    me = MSLException(block.BlockHandle, message('Simulink:blocks:Error reading the Acknowledge'));
    throw(me);  
  end
   
 %Write the sample rate
  SR = 6;
  ModeADS = 5;
  ADSNum = 1;
  ChipSelect = 4;
  Gain = 16;
  
  fwrite(s,SR,'char','sync'); 
  fwrite(s,ModeADS,'char','sync');
  fwrite(s,ADSNum,'char','sync');
  fwrite(s,ChipSelect,'char','sync');
  fwrite(s,Gain,'char','sync');
  
  pause(0.01);
  fclose(s);
  delete(s);
  
  %%

end

