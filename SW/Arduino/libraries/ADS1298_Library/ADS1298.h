/*
Copyright(c) 2017 Alma Mater Studiorum - Università di Bologna

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


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
	void writeADSchannelData(byte);
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


