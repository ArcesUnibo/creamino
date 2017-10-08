function [] = Sync()

 global s;
 start = 'false';
 
 while(strcmp(start,'false'))
 
    timeout = 0;
 
    AvailableBytes = s.BytesAvailable;
  
    while (AvailableBytes == 0 && timeout < 500)  %//Wait untill bytes are available on the input buffer
        %Wait untill bytes are available on the input buffer
        AvailableBytes = s.BytesAvailable;
        pause(0.01);
        timeout = timeout+1;
    end	

    if (timeout == 500)
        me = MSLException(block.BlockHandle, message('Simulink:blocks:Timeout error during synchronization'));
        throw(me);  
    end
  
    DataBuffer = fread(s,1,'uchar');
    if (DataBuffer == 240) 
         DataBuffer = fread(s,1,'uchar');
            if (DataBuffer == 240)
                start = 'true';
            end
    end
            
 end
 
 
end

