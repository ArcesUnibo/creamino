//
//  Definitions.h
//
//
//
//
//

#ifndef _Definitions_h	// add _8_
#define _Definitions_h

//SPI
#define SPI_DATA_MODE 		0x04	// CPOL = 0; CPHA = 1
#define SPI_MODE_MASK 		0x0C  	// mask of CPOL and CPHA  on SPCR
#define SPI_CLOCK_MASK 		0x03  	// SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK 	0x01  	// SPI2X = bit 0 on SPSR
#define SPI_CLOCK_DIV_2 	0X04	// 8MHz SPI SCK
#define SPI_CLOCK_DIV_4 	0X00	// 4MHz SPI SCK
#define SPI_CLOCK_DIV_16 	0x01    // 1MHz SPI SCK

#define dummy			0x00

//PIN CONNECTIONS 
//#ifndef ArduinoDue	// add _8_
//#define ADS_SS (10)
//#else
//#define ADS_SS (4)
//#endif


//ADS1198 SPI Initialization
#define _BITORDER 		MSBFIRST  	// MSB or LSB order
#define _DATAMODE 		SPI_MODE1 	// data clock idle when high or low?
#define _CLOCK 			2000000 	// set SLCK SPI
#define _MAXNUMBEROFADS	8			// set the maximum number of ads in daisy chain mode
#define _NUMBEROFBYTES	24			// set number of bytes to be read


//ADS1298 SPI Command Definition Byte Assignments
#define _WAKEUP 	0x02	// Wake-up from standby mode
#define _STANDBY 	0x04	// Enter Standby mode
#define _RESET 		0x06	// Reset the device registers to default
#define _START 		0x08	// Start and restart (synchronize) conversions
#define _STOP 		0x0A	// Stop conversion
#define _RDATAC 	0x10	// Enable Read Data Continuous mode (default mode at power-up)
#define _SDATAC 	0x11	// Stop Read Data Continuous mode
#define _RDATA 		0x12	// Read data by command; supports multiple read back
//#define _OFFSETCAL	0x1A	// Channel offset calibration
#define _RREG		0x20	// Read n nnnn registers starting at address rrrr
#define _WREG		0x40	// Write n nnnn registers starting at address rrrr


//ASD1298 Register Addresses
#define ID      	0x00
#define CONFIG1 	0x01
#define CONFIG2 	0x02
#define CONFIG3 	0x03
#define LOFF 		0x04
#define CH1SET 		0x05
#define CH2SET 		0x06
#define CH3SET 		0x07
#define CH4SET 		0x08
#define CH5SET 		0x09
#define CH6SET 		0x0A
#define CH7SET 		0x0B
#define CH8SET 		0x0C
#define RLD_SENSP 	0x0D
#define RLD_SENSN 	0x0E
#define LOFF_SENSP 	0x0F
#define LOFF_SENSN 	0x10
#define LOFF_FLIP 	0x11
#define LOFF_STATP 	0x12
#define LOFF_STATN 	0x13
#define GPIO 		0x14
#define PACE 		0x15
#define CONFIG4 	0x17


//ADS1198 GPIO SETTINGS
#define GPIO1_INPUT			0x01
#define GPIO2_INPUT			0x02
#define GPIO3_INPUT			0x04
#define GPIO4_INPUT			0x08
#define GPIO1_OUTPUT_ON		0x10
#define GPIO1_OUTPUT_OFF	0x00
#define GPIO2_OUTPUT_ON		0x20
#define GPIO2_OUTPUT_OFF	0x00
#define GPIO3_OUTPUT_ON		0x40
#define GPIO3_OUTPUT_OFF	0x00
#define GPIO4_OUTPUT_ON		0x80
#define GPIO4_OUTPUT_OFF	0x00

// CHANNEL SETTINGS

#define YES      	(0x01)
#define NO      	(0x00)

//CONFIGURATION REG1
#define ADS_HIGHRESOLUTION		0x80
#define ADS_LOWRESOLUTION		0x00
#define ADS_DAISYCHAIN			0x00
#define ADS_MULTIPLEREADBACK	0x40
#define ADS_CLK_OUTDISABLE		0x00
#define ADS_CLK_OUTENABLE		0x20
#define ADS_OUTPUTDATA_32KSPS	0x00
#define ADS_OUTPUTDATA_16KSPS	0x01
#define ADS_OUTPUTDATA_8KSPS	0x02
#define ADS_OUTPUTDATA_4KSPS	0x03
#define ADS_OUTPUTDATA_2KSPS	0x04
#define ADS_OUTPUTDATA_1KSPS	0x05
#define ADS_OUTPUTDATA_500SPS	0x06

//CONFIGURATION REG2
#define ADS_WCT_CHOP		0x20    // CHopping frequency constant at fMOD/16
#define ADS_INT_TEST_EXT	0x00	// Test signals are driven externally
#define ADS_INT_TEST_INT	0x10	// Test signals are generated internally
#define ADS_TEST_AMP_1		0x00	// 1 × –(VREFP – VREFN) / 2400
#define ADS_TEST_AMP_2		0x04	// 2 × –(VREFP – VREFN) / 2400
#define ADS_TEST_FREQ_1SEC	0x00	// Pulsed at fCLK / 2^21
#define ADS_TEST_FREQ_2SEC	0x01	// Pulsed at fCLK / 2^20
#define ADS_TEST_FREQ_DC	0x03	// at DC

//CONFIGURATION REG3
#define ADS_PDB_REFBUF_OFF	0x40	// Power-down internal reference buffer
#define ADS_PDB_REFBUF_ON	0xC0	// Enable internal reference buffer
#define ADS_VREFP_2_4		0x40	// VREFP is set to 2.4 V
#define ADS_VREFP_4			0x60	// VREFP is set to 4 V
#define ADS_OPAMP_REF_PIN	0x40	// Noninverting input connected to the OPAMPP pin
#define ADS_OPAMP_REF_DC	0x4C	// Noninverting input connected to (AVDD + AVSS) / 2
#define ADS_PDB_OPAMP_PD	0x40
#define ADS_PDB_OPAMP_EN	0x44


//gainCode choices
#define ADS_GAIN01	 0x10	//(0b00010000)
#define ADS_GAIN02	 0x20	//(0b00100000)
#define ADS_GAIN03	 0x30	//(0b00110000)
#define ADS_GAIN04	 0x40	//(0b01000000)
#define ADS_GAIN06	 0x00	//(0b00000000)
#define ADS_GAIN08	 0x50	//(0b01010000)
#define ADS_GAIN12	 0x60	//(0b01100000)

//inputType choices
#define ADSINPUT_NORMAL 	 0x00	//(0b00000000)
#define ADSINPUT_SHORTED 	 0x01	//(0b00000001)
#define ADSINPUT_BIAS_MEAS 	 0x02	//(0b00000010)
#define ADSINPUT_MVDD 		 0x03	//(0b00000011)
#define ADSINPUT_TEMP 		 0x04	//(0b00000100)
#define ADSINPUT_TESTSIG 	 0x05	//(0b00000101)
#define ADSINPUT_BIAS_DRP 	 0x06	//(0b00000110)
#define ADSINPUT_BIAS_DRN 	 0x07	//(0b00000111)


#endif