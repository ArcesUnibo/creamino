function [] = Initialize()



%Connect the device to Simulink
 global COMport
 global s;
 s = serial(COMport);
 set(s, 'DataBits', 8);
 set(s, 'StopBits', 1);
 set(s, 'BaudRate', 256000);
 set(s, 'Parity', 'none');
 set(s, 'InputBufferSize', 1056); % Il parametro deve essere cambiato in base al numero di dati a cui si vuole accedere
 
 %Open the serial port
 fopen(s);

 %Sleep 100ms
 pause(0.1);

 %Reset the device
 ResetCreamino();
 
 %Check the sync Word
 Sync();
 

 
    
 
 
end

