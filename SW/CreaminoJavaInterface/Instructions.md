1) How to use the Java Interface

In order to use the Java Interface you go on creamino\SW\CreaminoJavaInterface\dist 
Then double click on CreaminoJavaInterface.jar file. The interface appears.
Connect the Arduino Native port to a PC USB then click on the search button on the left under COM ports.
This perform a check on the available COM port. Chose the right one. 
(For further information on Arduino COM ports do on: https://www.arduino.cc/en/Guide/ArduinoDue) 
Once this has been done configure your EEG system. The ADS model (in our case ADS1298) the chip select (4),
the sample rate (500 SPS) the number of ADC Channels, the scale, the filter type and the gain for each channel.
In addition you can chose between the ADS mode. This allows you to configure the board in normal mode to acquire
EEG signals or to use the autogenerate test signals from the ADS1298.
For further information on how to configure ADS1298 please read http://www.ti.com/product/ADS1298 


2) How to modify the Java Interface as you want

In order to modify the interface you have to download Netbeans from here:
https://netbeans.org/downloads/

a) Download and Install Netbeans IDE from here : https://netbeans.org/downloads/
 
b) Open the Netbeans IDE 
Click on File-> Open project and then select CreaminoJavaInterface located in the SW folder of the repository.
If you chose to put the repository in the directory C:\WORK is simple

Otherwise you get two errors that you need to solve.
In order to resolve errors you need to download and import two libraries.
The first one is jssc available at https://github.com/scream3r/java-simple-serial-connector/releases.
Download the jSSC-2.8.0-Release.zip and unpacket the archieve using winzip or 7-zip
Then on Netbeans select Tools->Libraries
Add the jssc-2.8.0 libraries by including the Classpath the Sources and the Javadoc that you found in the jSSC-2.8.0-Release.zip
The same must be done with the aqua libraries available here: http://aquafx-project.com/downloads.html

c) Modify the interface

From the Netbeans IDE knowing a bit of Java you can modify the interface as you want.