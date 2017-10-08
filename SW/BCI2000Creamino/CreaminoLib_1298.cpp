/*Matteo Chiesi - University of Bologna*/

//CreaminoLib.cpp
#include "CreaminoLib.h"
#include <fstream>
#include "BCIError.h"
#include "ThreadUtils.h" // for SleepFor()

using namespace std;

namespace CreaminoLibrary {

	static std::vector<byte> DataBuffer(100000);
	static bool start = false;
	ofstream outputStream;
	static HANDLE Port;
	static int ChNum;
	static PrecisionTime mTimeBefore;
	static PrecisionTime mTimeAfter;

	bool CreaminoLib::StartADS(string COMPort, byte ADSSampleRate, int ChannelsNumber)
	{

		std::vector<std::string>  PortName;
		ChNum = ChannelsNumber;
		PortName = SerialLibrary::SerialLib::getSerialPortNames();
		int msgboxID;

		if (PortName.size() == 0) {
			msgboxID = MessageBox(NULL, "Port not found", "Error Message", MB_ICONWARNING | MB_OK | MB_DEFBUTTON2);
			//msgboxID = MessageBox(NULL, (LPCSTR)L"Port not found", (LPCSTR)L"Error Message", MB_ICONWARNING | MB_OK | MB_DEFBUTTON2);
			
			bciout << "Port not found" << '\n';
			return false;
		}
		else {
			for (int i = 0; i < (int)PortName.size(); i++)
				if (COMPort.compare(PortName[i]) == 0) {
					Port = SerialLibrary::SerialLib::openPort(COMPort, true);
					//bool ConfigurazioneRiuscita = SerialLibrary::SerialLib::setParams(Port, 256000, 8, 0, 0, true, true, 0);
					if (!SerialLibrary::SerialLib::setParams(Port, 256000, 8, 0, 0, true, true, 0)) {
						msgboxID = MessageBox(NULL, "Error during configuration", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
						//msgboxID = MessageBox(NULL, (LPCSTR)L"Error during configuration", (LPCSTR)L"Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
						bciout << "Error during configuration" << '\n';
						return false;
					}

					Sleep(100);

					if (!SerialLibrary::SerialLib::purgePort(Port, PURGE_RXCLEAR | PURGE_TXCLEAR)) {
						msgboxID = MessageBox(NULL, "Error while purging the port", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
						//msgboxID = MessageBox(NULL, (LPCSTR)L"Error while purging the port", (LPCSTR)L"Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
						bciout << "Error while purging the port" << '\n';
						return false;
					}

					Sleep(100);

					//Connecting Word
					std::vector<byte> ConnectingWord(7);
					ConnectingWord.reserve(7);
					ConnectingWord[0] = 'C';	ConnectingWord[1] = 'o';	ConnectingWord[2] = 'n';	ConnectingWord[3] = 'n';	ConnectingWord[4] = 'e';	ConnectingWord[5] = 'c';	ConnectingWord[6] = 't';
					if (!SerialLibrary::SerialLib::writeBytes(Port, ConnectingWord, (int)ConnectingWord.size())) {
						msgboxID = MessageBox(NULL, "Error writing connection word", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
						//msgboxID = MessageBox(NULL, (LPCSTR)L"Error writing connection word", (LPCSTR)L"Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
						bciout << "Error Writing connection word" << '\n';
						return false;
					}

					//Acnowledge
					std::vector<int> ByteDisponibili(2);
					while (ByteDisponibili[0] == 0) { //Wait untill bytes are available on the input buffer
						ByteDisponibili = SerialLibrary::SerialLib::getBuffersBytesCount(Port);
						Sleep(10);
					}

					//cout << "ByteIn\t" << ByteDisponibili[0] << " ByteOut\t" << ByteDisponibili[1] << "\n";
					std::vector<byte> Ack(ByteDisponibili[0]);
					Ack = SerialLibrary::SerialLib::readBytes(Port, ByteDisponibili[0]);
					string AckWord(Ack.begin(), Ack.end());
					if (AckWord.compare("Ready"))
						cout << Ack[0] << Ack[1] << Ack[2] << Ack[3] << Ack[4] << Ack[5] << Ack[6] << "\n";
					else {
						msgboxID = MessageBox(NULL, "Error reading the Acknowledge", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
						//msgboxID = MessageBox(NULL, (LPCSTR)L"Error reading the Acknowledge", (LPCSTR)L"Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
						bciout << "Error reading the Acknowledge" << '\n';
						return false;
					}

					//Init Parameters
					std::vector<byte> InitParams(5);
					//std::vector<byte> SampleRate(1);
					InitParams.reserve(5);
					//SampleRate.reserve(1);
					InitParams[0] = ADSSampleRate;
					InitParams[1] = 0x00;
					InitParams[2] = 0x01;
					InitParams[3] = 0x04;
					InitParams[34 = 0x10;

					//SampleRate[0] = ADSSampleRate;
					if (!SerialLibrary::SerialLib::writeBytes(Port, InitParams, (int)InitParams.size())) {
						msgboxID = MessageBox(NULL, "Error writing the sample rate", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
						//msgboxID = MessageBox(NULL, (LPCSTR)L"Error writing the sample rate", (LPCSTR)L"Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
						bciout << "Error writing the sample rate" << '\n';
						return false;
					}


					if (!SerialLibrary::SerialLib::closePort(Port)) {
						msgboxID = MessageBox(NULL, "Error closing the port", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
						//msgboxID = MessageBox(NULL, (LPCSTR)L"Error closing the port", (LPCSTR)L"Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
						bciout << "Error closing the port" << '\n';
						return false;
					}

					ConnectingWord.clear();
					Ack.clear();

				}
		}

		return true;

	}


	bool CreaminoLib::Initialize(string FileName, string COMPort) {

		int msgboxID;

		outputStream.open("temp.bin", ios::out | ios::binary);

		Port = SerialLibrary::SerialLib::openPort(COMPort, true);
		if (!SerialLibrary::SerialLib::setParams(Port, 256000, 8, 0, 0, true, true, 0)) {
			msgboxID = MessageBox(NULL, "Error during configuration", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
			//msgboxID = MessageBox(NULL, (LPCSTR)L"Error during configuration", (LPCSTR)L"Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
			bciout << "Error during configuration" << '\n';
			return false;
		}
		Sleep(100);

		if (!SerialLibrary::SerialLib::setEventsMask(Port, MASK_RXCHAR)) {
			msgboxID = MessageBox(NULL, "Error setting the mask", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
			//msgboxID = MessageBox(NULL, (LPCSTR)L"Error setting the mask", (LPCSTR)L"Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
			bciout << "Error setting the mask" << '\n';
			return false;
		}

		if (!resetCreamino()) {
			msgboxID = MessageBox(NULL, "Error resetting creamino", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
			//msgboxID = MessageBox(NULL, (LPCSTR)L"Error resetting creamino", (LPCSTR)L"Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
			bciout << "Error resetting creamino" << '\n';
			return false;
		}

		Synch();

		return true;

	}


	bool CreaminoLib::resetCreamino() {

		int msgboxID;

		if (!SerialLibrary::SerialLib::setDTR(Port, true)) {
			msgboxID = MessageBox(NULL, "Error setting DTR", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
			//msgboxID = MessageBox(NULL, (LPCSTR)L"Error setting DTR", (LPCSTR)L"Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
			bciout << "Error setting DTR" << '\n';
			return false;
		}

		Sleep(100);

		if (!SerialLibrary::SerialLib::setDTR(Port, false)) {
			msgboxID = MessageBox(NULL, "Error resetting DTR", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
			//msgboxID = MessageBox(NULL, (LPCSTR)L"Error resetting DTR", (LPCSTR)L"Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
			bciout << "Error resetting DTR" << '\n';
			return false;
		}

		Sleep(100);

		return true;
	}

	bool CreaminoLib::Synch() {

		bciout << "syncronization neded" << '\n';

		int msgboxID;
		std::vector<int> ByteDisponibili(2);
		start = false;
		
		do {
			int timeout = 0;
			
			//mTimeBefore = PrecisionTime::Now();

			while (ByteDisponibili[0] == 0 && timeout < 500) { //Wait untill bytes are available on the input buffer
				ByteDisponibili = SerialLibrary::SerialLib::getBuffersBytesCount(Port);
				Sleep(10);
				++timeout;
			}

			//Error message in case of timeout
			if (timeout == 500) {
				msgboxID = MessageBox(NULL, "Timeout error during synchronization", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
				//msgboxID = MessageBox(NULL, (LPCSTR)L"Timeout error during synchronization", (LPCSTR)L"Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
				bciout << "Timeout error during synchronization" << '\n';
				return false;
			}

			//mTimeAfter = PrecisionTime::Now();
			//bciout << "I byte ci sono dopo = " << PrecisionTime::UnsignedDiff(mTimeAfter, mTimeBefore) << '\t';

			DataBuffer = SerialLibrary::SerialLib::readBytes(Port, 1);
			cout << "Byte " << hex << int(DataBuffer[0]) << "\n";
			if (DataBuffer[0] == (byte)0xF0) {
				DataBuffer = SerialLibrary::SerialLib::readBytes(Port, 1);
				cout << "Byte " << hex << int(DataBuffer[0]) << "\n";
				if (DataBuffer[0] == (byte)0xF0) {
					start = true;
				}
			}

		} while (!start);

		return start;
	}

	int CreaminoLib::CreaminoWaitforData(float* Word, int inCount) {

		int msgboxID, i, j = 0;
		int BufferByte = inCount*(ChNum*3+2); //Three bytes for each channel + 2 synchronization words
		std::vector<int> ByteDisponibili(2);
		std::vector<byte> Buffer(BufferByte);
		int timeout = 0;

		
		while (ByteDisponibili[0] <BufferByte && timeout < 500) { //Wait untill bytes are available on the input buffer
			ByteDisponibili = SerialLibrary::SerialLib::getBuffersBytesCount(Port);
			Sleep(10);
			++timeout;}

		//Error message in case of timeout
		if (timeout == 500) {
			msgboxID = MessageBox(NULL, "Timeout error during synchronization", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
			bciout << "Timeout error during synchronization" << '\n';
			return 1;}

		Buffer = SerialLibrary::SerialLib::readBytes(Port, BufferByte);
		
		//while (inCount > j) {
		for (j = 0; j < inCount; j ++){
			for (i = 0; i < ChNum; i++) {
				if ((0xFF & Buffer[j*(ChNum * 3 + 2) + (i * 3)]) > 127) { 
					Word[j*ChNum + i] = (float)(2 * 2.4*(-(16777216 - ((0xFF & Buffer[((j*ChNum + i)*3)]) * 65536 + (0xFF & Buffer[((j*ChNum + i)*3) + 1]) * 256 + (0xFF & Buffer[((j*ChNum + i)*3) + 2]) + 1)) / 16777216.0 / 1 / 1));
					//Word[j*ChNum + i] = (float)(2 * 2.4*(-(65536 - ((0xFF & Buffer[j*(ChNum * 2 + 2) + (i * 2)]) * 256 + (0xFF & Buffer[j*(ChNum * 2 + 2) + (i * 2) + 1]) + 1)) / 65536.0 / 47 / 1));
					}
				else {
					//Word[j*ChNum + i] = (float)(2 * 2.4*((0xFF & Buffer[(j*(ChNum * 2 + 2) + i * 2)]) * 256 + (0xFF & Buffer[j*(ChNum * 2 + 2) + i * 2 + 1])) / 65536.0 / 47 / 1);
					Word[j*ChNum + i] = (float)(2 * 2.4*((((0xFF & Buffer[((j*ChNum + i) * 3)]) * 65536 + (0xFF & Buffer[((j*ChNum + i) * 3) + 1]) * 256 + (0xFF & Buffer[((j*ChNum + i) * 3) + 2]) + 1)) / 16777216.0 / 1 / 1));
				}
			}

				//if (!(Buffer[j*(ChNum * 3 + 2) + (i * 2)] == (byte)0xF0) && (Buffer[j*(ChNum * 2 + 2) + i * 2 + 1] == (byte)0xF0)) {
				if (!(Buffer[j*(ChNum * 3 + 2) + (i * 3)] == (byte)0xF0) && (Buffer[j*(ChNum * 3 + 2) + i * 3 + 1] == (byte)0xF0)){
					bciout << "Buffer [65] = " << hex << Buffer[j*(ChNum * 3 + 2) + (i * 3)] << '\t'; bciout << "Buffer [66] = " << hex << Buffer[j*(ChNum * 3 + 2) + i * 3 + 1] << '\n';
					if (!Synch()) 
						msgboxID = MessageBox(NULL, "Synchronization failed", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
						bciout << "Synchronization failed" << '\n';
						return 1;
				}
			
			outputStream.write(reinterpret_cast<const char*>(&Buffer[j*(ChNum * 3 + 2)]), ChNum * 3);

		}


		bciout << '\n';

		return 0;
	}


	//Close the native USB port
	int CreaminoLib::CreaminoStop() {

		int msgboxID;
		//serialPort.removeEventListener();
		
		/*
		if (!SerialLibrary::SerialLib::setEventsMask(Port, DISABLE_EVENTS)) {
			msgboxID = MessageBox(NULL, (LPCSTR)L"Error resetting the mask", (LPCSTR)L"Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
			bciout << "Error resetting the mask" << '\n';
			return 1;
		}
		*/

		Sleep(100);

		std::vector<byte> DisconnectingWord(10);
		DisconnectingWord.reserve(10);
		DisconnectingWord[0] = 'D';	DisconnectingWord[1] = 'i';	DisconnectingWord[2] = 's';	DisconnectingWord[3] = 'c';	DisconnectingWord[4] = 'o';	DisconnectingWord[5] = 'n';	DisconnectingWord[6] = 'n';	DisconnectingWord[7] = 'e';	DisconnectingWord[8] = 'c';	DisconnectingWord[9] = 't';
		cout << DisconnectingWord[0] << DisconnectingWord[1] << DisconnectingWord[2] << DisconnectingWord[3] << DisconnectingWord[4] << DisconnectingWord[5] << DisconnectingWord[6] << DisconnectingWord[7] << DisconnectingWord[8] << DisconnectingWord[9] << "\n";

		if (!SerialLibrary::SerialLib::writeBytes(Port, DisconnectingWord, (int)DisconnectingWord.size())) {
			msgboxID = MessageBox(NULL, "Error disconnecting the device", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
			//msgboxID = MessageBox(NULL, (LPCSTR)L"Error disconnecting the device", (LPCSTR)L"Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
			bciout << "Error disconnectting the device" << '\n';
			return 1;
		}

		Sleep(100);

		if (!SerialLibrary::SerialLib::purgePort(Port, PURGE_RXCLEAR | PURGE_TXCLEAR)) {
			msgboxID = MessageBox(NULL, "Error while purging the port", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
			//msgboxID = MessageBox(NULL, (LPCSTR)L"Error while purging the port", (LPCSTR)L"Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
			bciout << "Error while purging the port" << '\n';
			return 1;
		}


		if (!SerialLibrary::SerialLib::closePort(Port)) {
			msgboxID = MessageBox(NULL, "Error closing the port", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
			//msgboxID = MessageBox(NULL, (LPCSTR)L"Error closing the port", (LPCSTR)L"Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
			bciout << "Error closing the port" << '\n';
			return 1;
		}

		Write();

		return 0;

	}


	bool CreaminoLib::Write() {

		Sleep(100);
		outputStream.flush();
		outputStream.close();

		return true;
	}


	int CreaminoLib::CreaminoStart(string COMPort, byte ADSSampleRate, int ChannelsNumber) {

		bool config = StartADS(COMPort, ADSSampleRate, ChannelsNumber);
		string FileName = "temp.bin";
		bool init = Initialize(FileName, COMPort);

		if (config == true && init == true)
			return 0;
		else
			return 1;

	}

	float CreaminoLib::CreaminoNotchFilteredWord(int cf, float Word) {
		
		for (int cf2 = 6; cf2 > 0; cf2--)
		{
			INF_Notch[cf2][cf] = INF_Notch[cf2 - 1][cf];
			OUTF_Notch[cf2][cf] = OUTF_Notch[cf2 - 1][cf];
		}
		INF_Notch[0][cf] = Word;
		OUTF_Notch[0][cf] = NF_Notch[0] * INF_Notch[0][cf] + NF_Notch[1] * INF_Notch[1][cf] + NF_Notch[2] * INF_Notch[2][cf] + NF_Notch[3] * INF_Notch[3][cf] + NF_Notch[4] * INF_Notch[4][cf] + NF_Notch[5] * INF_Notch[5][cf] + NF_Notch[6] * INF_Notch[6][cf];
		return OUTF_Notch[0][cf] = OUTF_Notch[0][cf] - DF_Notch[1] * OUTF_Notch[1][cf] - DF_Notch[2] * OUTF_Notch[2][cf] - DF_Notch[3] * OUTF_Notch[3][cf] - DF_Notch[4] * OUTF_Notch[4][cf] - DF_Notch[5] * OUTF_Notch[5][cf] - DF_Notch[6] * OUTF_Notch[6][cf];
		
	}


	float CreaminoLib::CreaminoIIR6(int cf, float Word)
	{
		for (int cf2 = 6; cf2 > 0; cf2--)
		{
			INF[cf2][cf] = INF[cf2 - 1][cf];
			OUTF[cf2][cf] = OUTF[cf2 - 1][cf];
		}
		INF[0][cf] = Word;
		OUTF[0][cf] = NF[0] * INF[0][cf] + NF[1] * INF[1][cf] + NF[2] * INF[2][cf] + NF[3] * INF[3][cf] + NF[4] * INF[4][cf] + NF[5] * INF[5][cf] + NF[6] * INF[6][cf];
		return OUTF[0][cf] = OUTF[0][cf] - DF[1] * OUTF[1][cf] - DF[2] * OUTF[2][cf] - DF[3] * OUTF[3][cf] - DF[4] * OUTF[4][cf] - DF[5] * OUTF[5][cf] - DF[6] * OUTF[6][cf];


	}

	void CreaminoLib::SetNotchFilter() {
		
		NF_Notch[0] = (float)0.9043187344848;
		NF_Notch[1] = (float)-1.678819565687;
		NF_Notch[2] = (float)3.751835906173;
		NF_Notch[3] = (float)-3.571930779859;
		NF_Notch[4] = (float)3.751835906173;
		NF_Notch[5] = (float)-1.678819565687;
		NF_Notch[6] = (float)0.9043187344848;

		DF_Notch[0] = (float)1;
		DF_Notch[1] = (float)-1.794262584793;
		DF_Notch[2] = (float)3.872744259924;
		DF_Notch[3] = (float)-3.566265590484;
		DF_Notch[4] = (float)3.621772661108;
		DF_Notch[5] = (float)-1.569041735955;
		DF_Notch[6] = (float)0.8177923602828;
		
	}

	void CreaminoLib::ResetNotchFilter() {
	
		NF_Notch[0] = 1;
		NF_Notch[1] = 0;
		NF_Notch[2] = 0;
		NF_Notch[3] = 0;
		NF_Notch[4] = 0;
		NF_Notch[5] = 0;
		NF_Notch[6] = 0;

		DF_Notch[0] = 1;
		DF_Notch[1] = 0;
		DF_Notch[2] = 0;
		DF_Notch[3] = 0;
		DF_Notch[4] = 0;
		DF_Notch[5] = 0;
		DF_Notch[6] = 0;
		
	}

	 void CreaminoLib::IIR_05_65_500SPS() {

		 NF[0] = 0.03425508502224;
		 NF[1] = 0;
		 NF[2] = -0.1027652550667;
		 NF[3] = 0;
		 NF[4] = 0.1027652550667;
		 NF[5] = 0;
		 NF[6] = -0.03425508502224;

		 DF[0] = 1;
		 DF[1] = -4.399260782223;
		 DF[2] = 8.069715648566;
		 DF[3] = -7.997069830767;
		 DF[4] = 4.569085792934;
		 DF[5] = -1.429498613132;
		 DF[6] = 0.1870278540999;


	}

	  void CreaminoLib::IIR_05_40_500SPS() {

		  NF[0] = 0.009853425385308;
		  NF[1] = 0;
		  NF[2] = -0.02956027615592;
		  NF[3] = 0;
		  NF[4] = 0.02956027615592;
		  NF[5] = 0;
		  NF[6] = -0.009853425385308;

		  DF[0] = 1;
		  DF[1] = -5.007967649796;
		  DF[2] = 10.48221355455;
		  DF[3] = -11.76433667406;
		  DF[4] = 7.480442929736;
		  DF[5] = -2.556885366478;
		  DF[6] = 0.3665332263403;

	 }


	   void CreaminoLib::IIR_05_20_500SPS() {

		   NF[0] = 0.001460477432912;
		   NF[1] = 0;
		   NF[2] = -0.004381432298736;
		   NF[3] = 0;
		   NF[4] = 0.004381432298736;
		   NF[5] = 0;
		   NF[6] = -0.001460477432912;

		   DF[0] = 1;
		   DF[1] = -5.506711995898;
		   DF[2] = 12.65172137807;
		   DF[3] = -15.52654408635;
		   DF[4] = 10.73658286157;
		   DF[5] = -3.966858017298;
		   DF[6] = 0.6118098630404;

	  }

	    void CreaminoLib::IIR_2_65_500SPS() {

			NF[0] = 0.03232054543789;
			NF[1] = 0;
			NF[2] = -0.09696163631367;
			NF[3] = 0;
			NF[4] = 0.09696163631367;
			NF[5] = 0;
			NF[6] = -0.03232054543789;

			DF[0] = 1;
			DF[1] = -4.398965204498;
			DF[2] = 8.095717822694;
			DF[3] = -8.073908635408;
			DF[4] = 4.65195893902;
			DF[5] = -1.469877270362;
			DF[6] = 0.1950788334642;

	   }

		 void CreaminoLib::IIR_2_40_500SPS() {

			 NF[0] = 0.008903136788806;
			 NF[1] = 0;
			 NF[2] = -0.02670941036642;
			 NF[3] = 0;
			 NF[4] = 0.02670941036642;
			 NF[5] = 0;
			 NF[6] = -0.008903136788806;

			 DF[0] = 1;
			 DF[1] = -5.020355834273;
			 DF[2] = 10.55332809504;
			 DF[3] = -11.91471978829;
			 DF[4] = 7.632072974672;
			 DF[5] = -2.631416359776;
			 DF[6] = 0.3810922272684;

		}


		  void CreaminoLib::IIR_2_20_500SPS() {

			  NF[0] = 0.001168105334547;
			  NF[1] = 0;
			  NF[2] = -0.00350431600364;
			  NF[3] = 0;
			  NF[4] = 0.00350431600364;
			  NF[5] = 0;
			  NF[6] = -0.001168105334547;

			  DF[0] = 1;
			  DF[1] = -5.530949180194;
			  DF[2] = 12.77384443564;
			  DF[3] = -15.77068359872;
			  DF[4] = 10.97913076853;
			  DF[5] = -4.086827413712;
			  DF[6] = 0.6354851916294;

		 }

}

/*
int ControlFlow = SerialLibrary::SerialLib::getFlowControlMode(Port);
cout << "ControlFlow " << ControlFlow << "\n";
ConfigurazioneRiuscita = SerialLibrary::SerialLib::setFlowControlMode(Port, 0);
ControlFlow = SerialLibrary::SerialLib::getFlowControlMode(Port);
cout << "ControlFlow " << ControlFlow << "\n";
std::vector<int> LineStatus(4);
LineStatus = SerialLibrary::SerialLib::getLinesStatus(Port);
cout << "Stato Linea " << "CTS " <<LineStatus[0] << "DSR " << LineStatus[1] <<  "RING " << LineStatus[2] << "RLSD " << LineStatus[3] << "\n";
Sleep(2000);
*/