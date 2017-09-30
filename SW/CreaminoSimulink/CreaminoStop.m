function [] = CreaminoStop()
 
global s;

%Disconnect

DisconnectingWord = 'Disconnect';
%fprintf('Disonnect');
fwrite(s,DisconnectingWord,'char','sync'); 
pause(0.1);

%Purge port
flushinput(s);

%close port
fclose(s);
delete(s);
clear s; 

end

