/*Matteo Chiesi - University of Bologna*/
//CreaminoLib.h

#pragma once
#include "../SerialLib/SerialLib.h"
#include <Windows.h>
#include <iostream>
#include <vector>
#include <fstream>


using namespace std;

#define CREAMINO_NO_ERROR 0

#define ADS_OUTPUTDATA_8KSPS	0x00
#define ADS_OUTPUTDATA_4KSPS	0x01
#define ADS_OUTPUTDATA_2KSPS	0x02
#define ADS_OUTPUTDATA_1KSPS	0x03
#define ADS_OUTPUTDATA_500SPS	0x04
#define ADS_OUTPUTDATA_250SPS	0x05
#define ADS_OUTPUTDATA_125SPS	0x06

#define CH8		 8
#define CH16	16
#define CH24	24
#define CH32	32
#define CH40	40
#define CH48	48
#define CH56	56
#define CH64	64

#define DISABLE_EVENTS  0
#define MASK_RXCHAR		1
#define MASK_RXFLAG		2
#define MASK_TXEMPTY	4
#define MASK_CTS		8
#define MASK_DSR		16
#define MASK_RLSD		32
#define MASK_BREAK		64
#define MASK_ERR		128
#define MASK_RING		256

namespace CreaminoLibrary
{

	static float INF[7][CH32];
	static float OUTF[7][CH32];
	static float INF_Notch[7][CH32];
	static float OUTF_Notch[7][CH32];

	static float NF[7];
	static float NF_Notch[7];

	static float DF[7];
	static float DF_Notch[7];

	class CreaminoLib
	{

	public:

		/*
		* Class:    CreaminoLibrary
		* Method:   StartADS
		*/

		static bool StartADS(string COMPort, byte ADSSampleRate, int ChannelsNumber);

		/*
		* Class:    CreaminoLibrary
		* Method:   Initialize
		*/
		static bool Initialize(string FileName, string COMPort);

		/*
		* Class:    CreaminoLibrary
		* Method:   CreaminoWaitforData
		*/
		static int CreaminoWaitforData(float* outBuffer, int inCount);

		/*
		* Class:    CreaminoLibrary
		* Method:   ClosePort
		*/
		static int CreaminoStop();

		/*
		* Class:    CreaminoLibrary
		* Method:   CreaminoStart
		*/
		static int CreaminoStart(string COMPort, byte ADSSampleRate, int ChannelsNumber);

		/*
		* Class:    CreaminoLibrary
		* Method:   NotchFilteredWord
		*/
	    static float CreaminoNotchFilteredWord(int cf, float Word);

		/*
		* Class:    CreaminoLibrary
		* Method:   CreaminoIIR6
		*/
		static float CreaminoIIR6(int cf, float Word);

		/*
		* Class:    CreaminoLibrary
		* Method:   SetNotchFilter
		*/
		static void SetNotchFilter();

		/*
		* Class:    CreaminoLibrary
		* Method:   ResetNotchFilter
		*/
		static void ResetNotchFilter();

		/*
		* Class:    CreaminoLibrary
		* Method:   IIR_05_65_250SPS
		*/
		static void IIR_05_65_250SPS();

		/*
		* Class:    CreaminoLibrary
		* Method:   IIR_05_40_250SPS
		*/
		static void IIR_05_40_250SPS();

		/*
		* Class:    CreaminoLibrary
		* Method:   IIR_05_20_250SPS
		*/
		static void IIR_05_20_250SPS();

		/*
		* Class:    CreaminoLibrary
		* Method:   IIR_2_65_250SPS
		*/
		static void IIR_2_65_250SPS();

		/*
		* Class:    CreaminoLibrary
		* Method:   IIR_2_40_250SPS
		*/
		static void IIR_2_40_250SPS();

		/*
		* Class:    CreaminoLibrary
		* Method:   IIR_2_20_250SPS
		*/
		static void IIR_2_20_250SPS();


	private:

		/*
		* Class:    CreaminoLibrary
		* Method:   resetCreamino
		*/
		static bool resetCreamino();

		/*
		* Class:    CreaminoLibrary
		* Method:   Synch
		*/
		static bool Synch();

		/*
		* Class:    CreaminoLibrary
		* Method:   Write
		*/
		static bool Write();

		


	};



}