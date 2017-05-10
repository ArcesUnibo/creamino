#ifndef ____ADS1298__
#define ____ADS1298__


#include <Arduino.h>
#include <avr/pgmspace.h>
#include <SPI.h>
#include "pins_arduino.h"
#include "Definitions.h"

class ADS1298 {
public:

	//variables
	byte stat[3];			// used to hold the status register
	word channelData[8];		// array used when reading channel data as ints
	byte bit24ChannelData[_MAXNUMBEROFADS * _NUMBEROFBYTES + 1];      // array to hold raw data channel //controllare se va bene (_NUMBEROFADS - 1)
	int _NUMBEROFADS = 0;
	int count = 0;
	int ADS_SS = 4;

	void initialize(byte, byte, byte);
	void reINITforMoreADC(byte, byte);

	void resetADS();
	void startADS();
	void writeADSchannelData(byte, byte);
	void stopADS();

	byte getDeviceID();

	//ADS1298 SPI Command Definitions
	//System Commands
	void START();
	void STOP();

	//Data read Commands
	void RDATAC();
	void RDATA();
	void SDATAC();

	void getChannelData(word*);
	void getBit24ChannelData(byte*);

	void GPIOinitialize(byte);

	//Print functions
	void printHex(byte);
	void printBin(byte);
	void printCHvalueHex(word);

private:

	boolean isRunning;
	void csLow();
	void csHigh();

};

#endif


