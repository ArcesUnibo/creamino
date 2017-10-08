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

					//Sample Rate
					std::vector<byte> SampleRate(1);
					SampleRate.reserve(1);
					SampleRate[0] = ADSSampleRate;
					if (!SerialLibrary::SerialLib::writeBytes(Port, SampleRate, (int)SampleRate.size())) {
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
		//int BufferByte = (ChNum * 2 + 2); //Two bytes for each channel + 2 synchronization words
		int BufferByte = inCount*(ChNum*2+2); //Two bytes for each channel + 2 synchronization words
		std::vector<int> ByteDisponibili(2);
		std::vector<byte> Buffer(BufferByte);
		int timeout = 0;

		//Synchronization
		//if (!Synch()) {
		//	msgboxID = MessageBox(NULL, "Synchronization failed", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
		//	bciout << "Synchronization failed" << '\n';
		//	return 1;
		//}
		
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
				if ((0xFF & Buffer[j*(ChNum * 2 + 2) + (i * 2)]) > 127) { //if ((0xFF & Buffer[(i * 2)]) > 127) {
					Word[j*ChNum + i] = (float)(2 * 2.4*(-(65536 - ((0xFF & Buffer[j*(ChNum * 2 + 2) + (i * 2)]) * 256 + (0xFF & Buffer[j*(ChNum * 2 + 2) + (i * 2) + 1]) + 1)) / 65536.0 / 47 / 1));
					//Word[j*ChNum + i] = (2 * 2.4*(-(65536 - ((0xFF & Buffer[(i * 2)]) * 256 + (0xFF & Buffer[(i * 2) + 1]) + 1)) / 65536.0 / 47 / 1));
				}
				else {
					Word[j*ChNum + i] = (float)(2 * 2.4*((0xFF & Buffer[(j*(ChNum * 2 + 2) + i * 2)]) * 256 + (0xFF & Buffer[j*(ChNum * 2 + 2) + i * 2 + 1])) / 65536.0 / 47 / 1);
					//Word[j*ChNum + i] = (2 * 2.4*((0xFF & Buffer[(i * 2)]) * 256 + (0xFF & Buffer[i * 2 + 1])) / 65536.0 / 47 / 1);
				}
			}

			
			//if (j < inCount - 1)
				if (!(Buffer[j*(ChNum * 2 + 2) + (i * 2)] == (byte)0xF0) && (Buffer[j*(ChNum * 2 + 2) + i * 2 + 1] == (byte)0xF0)){
					bciout << "Buffer [65] = " << hex << Buffer[j*(ChNum * 2 + 2) + (i * 2)] << '\t'; bciout << "Buffer [66] = " << hex << Buffer[j*(ChNum * 2 + 2) + i * 2 + 1] << '\n';
					if (!Synch()) 
						msgboxID = MessageBox(NULL, "Synchronization failed", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
						bciout << "Synchronization failed" << '\n';
						return 1;
				}
			
			outputStream.write(reinterpret_cast<const char*>(&Buffer[j*(ChNum * 2 + 2)]), ChNum * 2);

			//++j;

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

	 void CreaminoLib::IIR_05_65_250SPS() {

		NF[0] = (float)0.179516478347;
		NF[1] = (float)0;
		NF[2] = (float)-0.5385494350409;
		NF[3] = (float)0;
		NF[4] = (float)0.5385494350409;
		NF[5] = (float)0;
		NF[6] = (float)-0.179516478347;

		DF[0] = (float)1;
		DF[1] = (float)-2.869187222081;
		DF[2] = (float)2.975880684331;
		DF[3] = (float)-1.666797891076;
		DF[4] = (float)0.8747739395112;
		DF[5] = (float)-0.3062731476477;
		DF[6] = (float)-0.00839346201296;

	}

	  void CreaminoLib::IIR_05_40_250SPS() {

		 NF[0] = (float)0.05616238477426;
		 NF[1] = (float)0;
		 NF[2] = (float)-0.1684871543228;
		 NF[3] = (float)0;
		 NF[4] = (float)0.1684871543228;
		 NF[5] = (float)0;
		 NF[6] = (float)-0.05616238477426;

		 DF[0] = (float)1;
		 DF[1] = (float)-4.039588577246;
		 DF[2] = (float)6.762259608584;
		 DF[3] = (float)-6.159223598989;
		 DF[4] = (float)3.311988596231;
		 DF[5] = (float)-0.9972539700135;
		 DF[6] = (float)0.1218188643436;

	 }


	   void CreaminoLib::IIR_05_20_250SPS() {

		  NF[0] = (float)0.009530483219676;
		  NF[1] = (float)0;
		  NF[2] = (float)-0.02859144965903;
		  NF[3] = (float)0;
		  NF[4] = (float)0.02859144965903;
		  NF[5] = (float)0;
		  NF[6] = (float)-0.009530483219676;

		  DF[0] = (float)1;
		  DF[1] = (float)-5.012117534896;
		  DF[2] = (float)10.50594981437;
		  DF[3] = (float)-11.81439380427;
		  DF[4] = (float)7.530761827078;
		  DF[5] = (float)-2.581528031578;
		  DF[6] = (float)0.371327892275;

	  }

	    void CreaminoLib::IIR_2_65_250SPS() {

		   NF[0] = (float)0.1698258468569;
		   NF[1] = (float)0;
		   NF[2] = (float)-0.5094775405707;
		   NF[3] = (float)0;
		   NF[4] = (float)0.5094775405707;
		   NF[5] = (float)0;
		   NF[6] = (float)-0.1698258468569;

		   DF[0] = (float)1;
		   DF[1] = (float)-2.821734605463;
		   DF[2] = (float)2.930468579725;
		   DF[3] = (float)-1.702661969847;
		   DF[4] = (float)0.9063373196691;
		   DF[5] = (float)-0.3101291985499;
		   DF[6] = (float)-0.002094643158845;

	   }

		 void CreaminoLib::IIR_2_40_250SPS() {

			NF[0] = (float)0.05114810063942;
			NF[1] = (float)0;
			NF[2] = (float)-0.1534443019183;
			NF[3] = (float)0;
			NF[4] = (float)0.1534443019183;
			NF[5] = (float)0;
			NF[6] = (float)-0.05114810063942;

			DF[0] = (float)1;
			DF[1] = (float)-4.025482681024;
			DF[2] = (float)6.775625720009;
			DF[3] = (float)-6.257750382527;
			DF[4] = (float)3.426590346399;
			DF[5] = (float)-1.052567934669;
			DF[6] = (float)0.1336447707714;

		}


		  void CreaminoLib::IIR_2_20_250SPS() {

			 NF[0] = (float)0.007721931222771;
			 NF[1] = (float)0;
			 NF[2] = (float)-0.02316579366831;
			 NF[3] = (float)0;
			 NF[4] = (float)0.02316579366831;
			 NF[5] = (float)0;
			 NF[6] = (float)-0.007721931222771;

			 DF[0] = (float)1;
			 DF[1] = (float)-5.03658218624;
			 DF[2] = (float)10.6476953455;
			 DF[3] = (float)-12.11621180377;
			 DF[4] = (float)7.837413196471;
			 DF[5] = (float)-2.733645585296;
			 DF[6] = (float)0.4013417274526;

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