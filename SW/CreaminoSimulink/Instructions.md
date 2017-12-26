1) How to use Creamino in Simulink

In order to import signals coming from Creamino directly into Simulink you need a Simulink licence.
If you have already installed Simulink on your PC double click on CreaminoSimulinkModel in the folder C:\WORK\creamino\SW\CreaminoSimulink 
This automatically open Matlab and Simulink 
Double click on the Creamino block to set the correct parameters. Modify the field parameters.
You have to set:
The COM port
The ADS type
The sample rate
The gain of the ADS channels
Test or Normal mode
The number of channels
The input buffer size.

Example:
'COM17','ADS1298','500SPS',1,'Test',8,8

Connect the Arduino Native port to a USB.
Then run the Simulink Model. You can add all the Simulink blocks needed to perform your signal process.
For further information on how to Use Creamino Inside Simulink watch the video available in this folder. 