function [Word] = CreaminoWaitForData(inCount)

global s;
global CHnum;
global Cal;
global BufferSize;
%inCount is the number of byte that must be read in a single time step of
%the simulation

    BufferByte = inCount*(double(CHnum)*3+2);
    
    timeout = 0;
    
    %Word = zeros(1,inCount);
    Word = zeros(BufferSize,CHnum);
    
    AvailableBytes = s.BytesAvailable;
  
    while (AvailableBytes < BufferByte && timeout < 500)  %//Wait untill bytes are available on the input buffer
        %Wait untill bytes are available on the input buffer
        AvailableBytes = s.BytesAvailable;
        pause(0.01);
        timeout = timeout+1;
    end	

    if (timeout == 500)
        me = MSLException(block.BlockHandle, message('Simulink:blocks:Timeout error during synchronization'));
        throw(me);  
    end
    
    Buffer = fread(s,BufferByte,'uchar');

    for j=0:1:(BufferSize-1) 
       for i=1:1:CHnum
           %if (Buffer(j*(CHnum*2+2) + (i*2)) > 127)
           if (Buffer(j*(CHnum*3+2) + (i*3)-1) > 127)
               %Word(j+1,i) = (2 * 2.4*(-(65536 - (Buffer(j*(CHnum*2+2) + ((i*2)-1))*256 + Buffer(j*(CHnum*2+2) + (i*2)) + 1)) / 65536.0 / 47 / 1e-6));
 			   Word(j+1,i) =  2*2.4*(-(16777216-(Buffer(j*(CHnum*3+2) + ((i*3)-1))*65536+Buffer(j*(CHnum*3+2) + (i*3))*256+Buffer(j*(CHnum*3+2) + (i*3)+1))+1))/16777216/1/1e-6*Cal(i);               %Word(j*CHnum + i) = (2 * 2.4*(-(65536 - (Buffer(j*(CHnum*2+2) + ((i*2)-1))*256 + Buffer(j*(CHnum*2+2) + (i*2)) + 1)) / 65536.0 / 47 / 1));
           else
               %Word(j+1,i) = (2 * 2.4*(Buffer(j*(CHnum*2+2) +((i*2)-1))*256 + Buffer(j*(CHnum*2+2) + i*2)) / 65536.0 / 47 / 1e-6);
               Word(j+1,i) =  2*2.4*(((Buffer(j*(CHnum*3+2) + ((i*3)-1))*65536+Buffer(j*(CHnum*3+2) + (i*3))*256+Buffer(j*(CHnum*3+2) + (i*3)+1))+1))/16777216/1/1e-6*Cal(i);               %Word(j*CHnum + i) = (2 * 2.4*(-(65536 - (Buffer(j*(CHnum*2+2) + ((i*2)-1))*256 + Buffer(j*(CHnum*2+2) + (i*2)) + 1)) / 65536.0 / 47 / 1));
           end
       end

       if(~((Buffer(j*(CHnum * 3 + 2) + (i*3+1)) == 240) && (Buffer(j*(CHnum * 3 + 2) + ((i*3)+2)) == 240)));
           fprintf('\n Entro nella sincronizzazione \n');
           Sync(); 
       end         

    end
                
end

