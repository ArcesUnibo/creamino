
#define ArduinoDue
#include "ADS1298.h"

// set up the speed, data order and data mode
//SPISettings settingsADS1298(_CLOCK, _BITORDER, _DATAMODE);

//ADS Initialitation
void ADS1298::initialize(byte SampleRate, byte ADSNum, byte ChipSelect)
{
	
	_NUMBEROFADS = (int)ADSNum;
	ADS_SS = (int)ChipSelect;

	//Initialize the SPI communication
	SPI.begin(ADS_SS);
	SPI.setClockDivider(ADS_SS, 21);
	SPI.setDataMode(ADS_SS, _DATAMODE);
	SPI.setBitOrder(ADS_SS, _BITORDER);

	delay(50);

	//SPI Reset
	SPI.transfer(ADS_SS, _RESET);

	delayMicroseconds(12);    	//must wait 18 tCLK cycles to execute this command

	SPI.transfer(ADS_SS, _SDATAC, SPI_CONTINUE);		//stop read data so registers can be written… 0x11
	delayMicroseconds(3);   	//must wait 4 tCLK cycles after executing this command

	SPI.transfer(ADS_SS, CONFIG1 | _WREG, SPI_CONTINUE);	//Write Regs from REG1 to REG4… 0x41
	SPI.transfer(ADS_SS, 0x02, SPI_CONTINUE);
	SPI.transfer(ADS_SS, ADS_HIGHRESOLUTION | ADS_DAISYCHAIN | ADS_CLK_OUTENABLE | SampleRate, SPI_CONTINUE);
	SPI.transfer(ADS_SS, ADS_WCT_CHOP | ADS_INT_TEST_INT | ADS_TEST_AMP_2 | ADS_TEST_FREQ_1SEC, SPI_CONTINUE);	  //Test signal intern, 0x30 := –1 × (VREFP – VREFN)/2.4mV, 0x34 := –2 × (VREFP – VREFN)/2.4mV
	SPI.transfer(ADS_SS, ADS_PDB_REFBUF_ON | ADS_VREFP_2_4 | ADS_OPAMP_REF_PIN | ADS_PDB_OPAMP_PD);		//Internal reference

	delay(1);

	
}

// to be sure that all the ADCs are configured
void ADS1298::reINITforMoreADC(byte SampleRate, byte ModeADS)
{
	
	SPI.transfer(ADS_SS, CONFIG1 | _WREG, SPI_CONTINUE);	//Write Regs from REG1 to REG4… 0x41
	SPI.transfer(ADS_SS, 0x02, SPI_CONTINUE);
	SPI.transfer(ADS_SS, ADS_HIGHRESOLUTION | ADS_DAISYCHAIN | ADS_CLK_OUTENABLE | SampleRate, SPI_CONTINUE);
	SPI.transfer(ADS_SS, ADS_WCT_CHOP | ADS_INT_TEST_INT | ADS_TEST_AMP_2 | ADS_TEST_FREQ_1SEC, SPI_CONTINUE);	  //Test signal intern, 0x30 := –1 × (VREFP – VREFN)/2.4mV, 0x34 := –2 × (VREFP – VREFN)/2.4mV
	SPI.transfer(ADS_SS, ADS_PDB_REFBUF_ON | ADS_VREFP_2_4 | ADS_OPAMP_REF_PIN | ADS_PDB_OPAMP_PD, SPI_CONTINUE);		//Internal reference

	delay(1);

	SPI.transfer(ADS_SS, CH1SET | _WREG, SPI_CONTINUE);	//Write Regs from REG5 to REG13… 0x45
	SPI.transfer(ADS_SS, 0x07, SPI_CONTINUE);
	SPI.transfer(ADS_SS, ADS_GAIN01 | ModeADS, SPI_CONTINUE);		//CH1 ON, PGA=1, NORMAL MODE
	SPI.transfer(ADS_SS, ADS_GAIN01 | ModeADS, SPI_CONTINUE);		//…
	SPI.transfer(ADS_SS, ADS_GAIN01 | ModeADS, SPI_CONTINUE);
	SPI.transfer(ADS_SS, ADS_GAIN01 | ModeADS, SPI_CONTINUE);
	SPI.transfer(ADS_SS, ADS_GAIN01 | ModeADS, SPI_CONTINUE);
	SPI.transfer(ADS_SS, ADS_GAIN01 | ModeADS, SPI_CONTINUE);
	SPI.transfer(ADS_SS, ADS_GAIN01 | ModeADS, SPI_CONTINUE);
	SPI.transfer(ADS_SS, ADS_GAIN01 | ModeADS, SPI_CONTINUE);

	//test GPIO --This function stop the SPI. Remember to erase SPI_CONTINUE from the previous line if you comment this line
	//GPIOinitialize(GPIO1_OUTPUT_ON + GPIO2_OUTPUT_ON + GPIO3_OUTPUT_ON + GPIO4_OUTPUT_ON);

	SPI.transfer(ADS_SS, GPIO | _WREG, SPI_CONTINUE);	//Write Reg GPIO… GPIO + 0x40
	SPI.transfer(ADS_SS, 0x00, SPI_CONTINUE);
	SPI.transfer(ADS_SS, GPIO1_OUTPUT_ON | GPIO2_OUTPUT_ON | GPIO3_OUTPUT_ON | GPIO4_OUTPUT_ON, SPI_CONTINUE);		//GPIOA-B-C-D as input/output and set HIGH/LOW

	SPI.transfer(ADS_SS, GPIO | _WREG, SPI_CONTINUE);	//Write Reg GPIO… GPIO + 0x40
	SPI.transfer(ADS_SS, 0x00, SPI_CONTINUE);
	SPI.transfer(ADS_SS, GPIO1_OUTPUT_ON | GPIO2_OUTPUT_ON | GPIO3_OUTPUT_ON | GPIO4_OUTPUT_ON, SPI_CONTINUE);		//GPIOA-B-C-D as input/output and set HIGH/LOW

	SPI.transfer(ADS_SS, GPIO | _WREG, SPI_CONTINUE);	//Write Reg GPIO… GPIO + 0x40
	SPI.transfer(ADS_SS, 0x00, SPI_CONTINUE);
	SPI.transfer(ADS_SS, GPIO1_OUTPUT_ON | GPIO2_OUTPUT_ON | GPIO3_OUTPUT_ON | GPIO4_OUTPUT_ON);		//GPIOA-B-C-D as input/output and set HIGH/LOW

}

//reset all the ADS1298's settings.  Call however you'd like.  Stops all data acquisition
void ADS1298::resetADS()
{
	
	SPI.transfer(ADS_SS, _RESET, SPI_CONTINUE);		//send RESET command to default all registers
	delayMicroseconds(12);    	//must wait 18 tCLK cycles to execute this command

	SPI.transfer(ADS_SS, _SDATAC);		//stop read data so registers can be written… 0x11
	delayMicroseconds(3);   	//must wait 4 tCLK cycles after executing this command

	delay(100);
	
};

// Start continuous data acquisition
void ADS1298::startADS(void)
{
	
	RDATAC();
	delay(1);   // enter Read Data Continuous mode
	START();        // start the data acquisition
	delay(1);
	isRunning = true;
	
}

//write as binary each channel's 2 bytes of data
void ADS1298::writeADSchannelData(byte TriggerWord)
{
	//print rawChannelData array
	byte Trigger = 0xF0 | TriggerWord;
	byte ADCbyte = 0xF0;
	SerialUSB.write(ADCbyte);
	SerialUSB.write(Trigger);
	SerialUSB.write(bit24ChannelData, _NUMBEROFADS * _NUMBEROFBYTES);
}

// Stop the continuous data acquisition
void ADS1298::stopADS(void)
{
	STOP();
	delay(1);       // start the data acquisition
	SDATAC();
	delay(1);       // exit Read Data Continuous mode to communicate with ADS
	isRunning = false;
}

//start data conversion
void ADS1298::START()
{
	SPI.transfer(ADS_SS, _START);
}

//stop data conversion
void ADS1298::STOP()
{
	SPI.transfer(ADS_SS, _STOP);
}

//read data continuous mode
void ADS1298::RDATAC()
{
	SPI.transfer(ADS_SS, _RDATAC);
	delayMicroseconds(3);
}

//Single Read data
void ADS1298::RDATA()
{
	
	byte inByte;            			//  to read in one sample of the channels
	int byteCounter = 0;
	byte bits = 0;
	SPI.transfer(ADS_SS, _RDATA, SPI_CONTINUE);  				//  send the RDATA command

	for (int j = 0; j < _NUMBEROFADS; j++)
	{
		for (int i = 0; i < 3; i++){
			stat[i] = SPI.transfer(ADS_SS, 0x00, SPI_CONTINUE);     	//  read status register (1100 + LOFF_STATP + LOFF_STATN + GPIO[7:4])
			if (j>1 && i == 0 && _NUMBEROFADS > 2) {
				for (int k = 0; k < j; k++)
					bits = bits | bitRead(bit24ChannelData[byteCounter], 7 - k) << (k);
				bit24ChannelData[byteCounter - 1] = (bit24ChannelData[byteCounter - 1] << j - 1) | bits;
			}
		}
		for (int i = 0; i < _NUMBEROFBYTES; i++)
		{
			// read new channel data
			if (i == _NUMBEROFBYTES - 1 && _NUMBEROFADS == 1)
				inByte = SPI.transfer(ADS_SS, 0x00);
			else
				inByte = SPI.transfer(ADS_SS, 0x00, SPI_CONTINUE);

			bit24ChannelData[byteCounter] = inByte;  //raw data

			bits = 0;
			if (i>0)
				for (int k = 0; k < j; k++)
					bits = bits | bitRead(bit24ChannelData[byteCounter], 7 - k) << (k);
			if (i>0 && j>0)
				bit24ChannelData[byteCounter - 1] = (bit24ChannelData[byteCounter - 1] << j) | bits;
			byteCounter++;

		}
	}

	if (_NUMBEROFADS > 1){
		inByte = SPI.transfer(ADS_SS, 0x00);
		bit24ChannelData[byteCounter] = inByte;
		bits = 0;
		for (int k = 0; k < _NUMBEROFADS - 1; k++)
			bits = bits | bitRead(bit24ChannelData[byteCounter], 7 - k) << (k);

		bit24ChannelData[byteCounter - 1] = (bit24ChannelData[byteCounter - 1] << _NUMBEROFADS - 1) | bits;
	}
}

//stop read data continuous mode
void ADS1298::SDATAC()
{
	SPI.transfer(ADS_SS, _SDATAC);
	delayMicroseconds(3);   //must wait 4 tCLK cycles after executing this command
}

// Used to get the DeviceID
byte ADS1298::getDeviceID()
{
	// simple hello world com check
	SPI.transfer(ADS_SS, ID | _RREG, SPI_CONTINUE);	//Read ID Reg
	SPI.transfer(ADS_SS, 0x00, SPI_CONTINUE);

	byte data = SPI.transfer(ADS_SS, 0x00);	//Read value of ID Register

	//print value in HEX
	SerialUSB.print(F("Device ID: "));
	printHex(data);
	SerialUSB.print(F(" —> "));
	printBin(data);
	SerialUSB.println();

	return data;
}


// ADS GPIO select as input or output and if output pins set value
void ADS1298::GPIOinitialize(byte _set)
{
	SPI.begin(ADS_SS);
	SPI.setClockDivider(ADS_SS, 42);
	SPI.setDataMode(ADS_SS, SPI_MODE1);
	SPI.setBitOrder(ADS_SS, MSBFIRST);

	SPI.transfer(ADS_SS, GPIO | _WREG, SPI_CONTINUE);	//Write Reg GPIO… GPIO + 0x40
	SPI.transfer(ADS_SS, 0x00, SPI_CONTINUE);
	SPI.transfer(ADS_SS, _set);		//GPIOA-B-C-D as input/output and set HIGH/LOW
}


// Used for printing HEX data
void ADS1298::printHex(byte _data)
{
	SerialUSB.print(F("0x"));
	if (_data < 0x10)
		SerialUSB.print(F("0"));
	SerialUSB.print(_data, HEX);
}

// Used for printing BIN data
void ADS1298::printBin(byte _data)
{
	SerialUSB.print(F("B"));
	SerialUSB.print(_data, BIN);
}

// Used for printing Channel values in HEX format
void ADS1298::printCHvalueHex(word _data)
{
	SerialUSB.print(F("0x"));
	if (_data < 0x0010)
	{
		SerialUSB.print(F("000"));
		SerialUSB.print(_data, HEX);
	}
	else if (_data < 0x0100)
	{
		SerialUSB.print(F("00"));
		SerialUSB.print(_data, HEX);
	}
	else if (_data < 0x1000)
	{
		SerialUSB.print(F("0"));
		SerialUSB.print(_data, HEX);
	}
	else
		SerialUSB.print(_data, HEX);
 }

// ADS Slave Select
void ADS1298::csLow()
{
	digitalWrite(ADS_SS, LOW);
	delayMicroseconds(5);

	//SPI.beginTransaction(settingsADS1198);
}

void ADS1298::csHigh()
{
	//SPI.endTransaction();
	digitalWrite(ADS_SS, HIGH);
	delayMicroseconds(5);
}
