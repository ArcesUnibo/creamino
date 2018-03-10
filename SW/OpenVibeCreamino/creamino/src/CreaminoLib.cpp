/*Matteo Chiesi - University of Bologna*/

#include "CreaminoLib.h"
#include <fstream>

using namespace std;

namespace CreaminoLibrary {

	static std::vector<BYTE> DataBuffer(100000);
	static bool start = false;
	ofstream outputStream;
	static HANDLE Port;
	static int ChNum;
	static double Cal[CH64];

	bool CreaminoLib::StartADS(string COMPort, BYTE ADSSampleRate, int ChannelsNumber, BYTE ADSMode, BYTE ADSNum, BYTE ChipSelect, BYTE Gain)
	{

		std::vector<std::string>  PortName;
		ChNum = ChannelsNumber;
		PortName = SerialLibrary::SerialLib::getSerialPortNames();
		int msgboxID;

		if (PortName.size() == 0) {
			msgboxID = MessageBox(NULL, "Port not found", "Error Message", MB_ICONWARNING | MB_OK | MB_DEFBUTTON2);
			
			return false;
		}
		else {
			for (int i = 0; i < (int)PortName.size(); i++)
				if (COMPort.compare(PortName[i]) == 0) {
					Port = SerialLibrary::SerialLib::openPort(COMPort, true);
					if (!SerialLibrary::SerialLib::setParams(Port, 256000, 8, 0, 0, true, true, 0)) {
						msgboxID = MessageBox(NULL, "Error during configuration", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
						return false;
					}

					Sleep(100);

					if (!SerialLibrary::SerialLib::purgePort(Port, PURGE_RXCLEAR | PURGE_TXCLEAR)) {
						msgboxID = MessageBox(NULL, "Error while purging the port", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
						return false;
					}

					Sleep(100);

					//Connecting Word
					std::vector<BYTE> ConnectingWord(7);
					ConnectingWord.reserve(7);
					ConnectingWord[0] = 'C';	ConnectingWord[1] = 'o';	ConnectingWord[2] = 'n';	ConnectingWord[3] = 'n';	ConnectingWord[4] = 'e';	ConnectingWord[5] = 'c';	ConnectingWord[6] = 't';
					if (!SerialLibrary::SerialLib::writeBytes(Port, ConnectingWord, (int)ConnectingWord.size())) {
						msgboxID = MessageBox(NULL, "Error writing connection word", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
						return false;
					}

					//Acnowledge
					std::vector<int> ByteDisponibili(2);
					while (ByteDisponibili[0] == 0) { //Wait untill bytes are available on the input buffer
						ByteDisponibili = SerialLibrary::SerialLib::getBuffersBytesCount(Port);
						Sleep(10);
					}

					std::vector<BYTE> Ack(ByteDisponibili[0]);
					Ack = SerialLibrary::SerialLib::readBytes(Port, ByteDisponibili[0]);
					string AckWord(Ack.begin(), Ack.end());
					if (AckWord.compare("Ready"))
						cout << Ack[0] << Ack[1] << Ack[2] << Ack[3] << Ack[4] << Ack[5] << Ack[6] << "\n";
					else {
						msgboxID = MessageBox(NULL, "Error reading the Acknowledge", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
						return false;
					}

					//Init Parameters
					std::vector<BYTE> InitParams(5);
					InitParams.reserve(5);
					InitParams[0] = ADSSampleRate;
					InitParams[1] = ADSMode;
					InitParams[2] = ADSNum;
					InitParams[3] = ChipSelect;
					InitParams[4] = Gain;

					if (!SerialLibrary::SerialLib::writeBytes(Port, InitParams, (int)InitParams.size())) {
						msgboxID = MessageBox(NULL, "Error writing the sample rate", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
						return false;
					}


					if (!SerialLibrary::SerialLib::closePort(Port)) {
						msgboxID = MessageBox(NULL, "Error closing the port", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
						return false;
					}

					ConnectingWord.clear();
					Ack.clear();

				}
		}

		return true;

	}


	bool CreaminoLib::Initialize(string FileName, string COMPort, string CalibrationFile) {


		int msgboxID;
		string Channel;
		string Value;
		int i = 0;
		ifstream ip(CalibrationFile);
		if (!ip.is_open()) std::cout << "Error opening File" << '\n';

		for (i = 0; i < CH64; i++) {
			Cal[i] = 1.0;
		}
		//reset counter
		i = 0;

		while (ip.good()) {
			getline(ip, Channel, ';');
			getline(ip, Value, '\n');


			Cal[i] = atof((Value).c_str());

			std::cout << "Channel Name = " << Channel << '\t';
			std::cout << "Value = " << Value << '\n';
			i++;
		}

		ip.close();

		outputStream.open("temp.bin", ios::out | ios::binary);

		Port = SerialLibrary::SerialLib::openPort(COMPort, true);
		if (!SerialLibrary::SerialLib::setParams(Port, 256000, 8, 0, 0, true, true, 0)) {
			msgboxID = MessageBox(NULL, "Error during configuration", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
			return false;
		}
		Sleep(100);

		if (!SerialLibrary::SerialLib::setEventsMask(Port, MASK_RXCHAR)) {
			msgboxID = MessageBox(NULL, "Error setting the mask", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
			return false;
		}

		if (!resetCreamino()) {
			msgboxID = MessageBox(NULL, "Error resetting creamino", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
			return false;
		}

		Synch();

		return true;

	}


	bool CreaminoLib::resetCreamino() {

		int msgboxID;

		if (!SerialLibrary::SerialLib::setDTR(Port, true)) {
			msgboxID = MessageBox(NULL, "Error setting DTR", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
			return false;
		}

		Sleep(100);

		if (!SerialLibrary::SerialLib::setDTR(Port, false)) {
			msgboxID = MessageBox(NULL, "Error resetting DTR", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
			return false;
		}

		Sleep(100);

		return true;
	}

	bool CreaminoLib::Synch() {

		
		int msgboxID;
		std::vector<int> ByteDisponibili(2);
		start = false;
		
		do {
			int timeout = 0;
			
		
			while (ByteDisponibili[0] == 0 && timeout < 500) { //Wait untill bytes are available on the input buffer
				ByteDisponibili = SerialLibrary::SerialLib::getBuffersBytesCount(Port);
				Sleep(10);
				++timeout;
			}

			//Error message in case of timeout
			if (timeout == 500) {
				msgboxID = MessageBox(NULL, "Timeout error during synchronization", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
				return false;
			}

		
			DataBuffer = SerialLibrary::SerialLib::readBytes(Port, 1);
			cout << "Byte " << hex << int(DataBuffer[0]) << "\n";
			if (DataBuffer[0] == (BYTE)0xF0) {
				DataBuffer = SerialLibrary::SerialLib::readBytes(Port, 1);
				cout << "Byte " << hex << int(DataBuffer[0]) << "\n";
				if (DataBuffer[0] == (BYTE)0xF0) {
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
		std::vector<BYTE> Buffer(BufferByte);
		int timeout = 0;

		
		while (ByteDisponibili[0] <BufferByte && timeout < 500) { //Wait untill bytes are available on the input buffer
			ByteDisponibili = SerialLibrary::SerialLib::getBuffersBytesCount(Port);
			Sleep(10);
			++timeout;}

		//Error message in case of timeout
		if (timeout == 500) {
			msgboxID = MessageBox(NULL, "Timeout error during synchronization", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
			return 1;}

		Buffer = SerialLibrary::SerialLib::readBytes(Port, BufferByte);
		
		for (j = 0; j < inCount; j ++){
			for (i = 0; i < ChNum; i++) {
				if ((0xFF & Buffer[j*(ChNum * 3 + 2) + (i * 3)]) > 127) { 
					Word[j*ChNum + i] = (float)(2 * 2.4*(-(16777216 - ((0xFF & Buffer[j*(ChNum * 3 + 2) + (i * 3)]) * 65536 + (0xFF & Buffer[j*(ChNum * 3 + 2) + (i * 3) + 1]) * 256 + (0xFF & Buffer[j*(ChNum * 3 + 2) + (i * 3) + 2]) + 1)) / 16777216.0 / 1 / 1));
					}
				else {
					Word[j*ChNum + i] = (float)(2 * 2.4*((((0xFF & Buffer[j*(ChNum * 3 + 2) + (i * 3)]) * 65536 + (0xFF & Buffer[j*(ChNum * 3 + 2) + (i * 3) + 1]) * 256 + (0xFF & Buffer[j*(ChNum * 3 + 2) + (i * 3) + 2]) + 1)) / 16777216.0 / 1 / 1));
				}
			}

				if (!(Buffer[j*(ChNum * 3 + 2) + (i * 3)] == (BYTE)0xF0) && (Buffer[j*(ChNum * 3 + 2) + i * 3 + 1] == (BYTE)0xF0)){
					if (!Synch()) 
						msgboxID = MessageBox(NULL, "Synchronization failed", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
						return 1;
				}
			
			outputStream.write(reinterpret_cast<const char*>(&Buffer[j*(ChNum * 3 + 2)]), ChNum * 3);

		}


		
		return 0;
	}


	//Close the native USB port
	int CreaminoLib::CreaminoStop() {

		int msgboxID;
		
		Sleep(100);

		std::vector<BYTE> DisconnectingWord(10);
		DisconnectingWord.reserve(10);
		DisconnectingWord[0] = 'D';	DisconnectingWord[1] = 'i';	DisconnectingWord[2] = 's';	DisconnectingWord[3] = 'c';	DisconnectingWord[4] = 'o';	DisconnectingWord[5] = 'n';	DisconnectingWord[6] = 'n';	DisconnectingWord[7] = 'e';	DisconnectingWord[8] = 'c';	DisconnectingWord[9] = 't';
		cout << DisconnectingWord[0] << DisconnectingWord[1] << DisconnectingWord[2] << DisconnectingWord[3] << DisconnectingWord[4] << DisconnectingWord[5] << DisconnectingWord[6] << DisconnectingWord[7] << DisconnectingWord[8] << DisconnectingWord[9] << "\n";

		if (!SerialLibrary::SerialLib::writeBytes(Port, DisconnectingWord, (int)DisconnectingWord.size())) {
			msgboxID = MessageBox(NULL, "Error disconnecting the device", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
			return 1;
		}

		Sleep(100);

		if (!SerialLibrary::SerialLib::purgePort(Port, PURGE_RXCLEAR | PURGE_TXCLEAR)) {
			msgboxID = MessageBox(NULL, "Error while purging the port", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
			return 1;
		}


		if (!SerialLibrary::SerialLib::closePort(Port)) {
			msgboxID = MessageBox(NULL, "Error closing the port", "Error Message", MB_ICONERROR | MB_OK | MB_DEFBUTTON2);
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


	int CreaminoLib::CreaminoStart(string COMPort, BYTE ADSSampleRate, int ChannelsNumber, BYTE ADSMode, BYTE ADSNum, BYTE ChipSelect, BYTE Gain, string CalibrationFile) {

		bool config = StartADS(COMPort, ADSSampleRate, ChannelsNumber, ADSMode, ADSNum, ChipSelect, Gain);
		string FileName = "temp.bin";
		bool init = Initialize(FileName, COMPort, CalibrationFile);

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
		
		NF_Notch[0] = (float)0.9509718879234;
		NF_Notch[1] = (float)-4.617572789989;
		NF_Notch[2] = (float)10.32666567385;
		NF_Notch[3] = (float)-13.26734617205;
		NF_Notch[4] = (float)10.32666567385;
		NF_Notch[5] = (float)-4.617572789989;
		NF_Notch[6] = (float)0.9509718879234;

		DF_Notch[0] = (float)1;
		DF_Notch[1] = (float)-4.774287021015;
		DF_Notch[2] = (float)10.49847986078;
		DF_Notch[3] = (float)-13.26345558518;
		DF_Notch[4] = (float)10.15244773137;
		DF_Notch[5] = (float)-4.464749145826;
		DF_Notch[6] = (float)0.9043475313924;
		
	}


	void CreaminoLib::ResetNotchFilter() {
	
		NF_Notch[0] = (float)1;
		NF_Notch[1] = (float)0;
		NF_Notch[2] = (float)0;
		NF_Notch[3] = (float)0;
		NF_Notch[4] = (float)0;
		NF_Notch[5] = (float)0;
		NF_Notch[6] = (float)0;

		DF_Notch[0] = (float)1;
		DF_Notch[1] = (float)0;
		DF_Notch[2] = (float)0;
		DF_Notch[3] = (float)0;
		DF_Notch[4] = (float)0;
		DF_Notch[5] = (float)0;
		DF_Notch[6] = (float)0;
		
	}


	/*Filter Coefficients*/
	void CreaminoLib::NoFilter() {

		NF[0] = (float)1.0;
		NF[1] = (float)0.0;
		NF[2] = (float)0.0;
		NF[3] = (float)0.0;
		NF[4] = (float)0.0;
		NF[5] = (float)0.0;
		NF[6] = (float)0.0;

		DF[0] = (float)1.0;
		DF[1] = (float)0.0;
		DF[2] = (float)0.0;
		DF[3] = (float)0.0;
		DF[4] = (float)0.0;
		DF[5] = (float)0.0;
		DF[6] = (float)0.0;

	}


	 void CreaminoLib::IIR_05_65_500SPS() {

		 NF[0] = (float)0.03425508502224;
		 NF[1] = (float)0;
		 NF[2] = (float)-0.1027652550667;
		 NF[3] = (float)0;
		 NF[4] = (float)0.1027652550667;
		 NF[5] = (float)0;
		 NF[6] = (float)-0.03425508502224;

		 DF[0] = (float)1;
		 DF[1] = (float)-4.399260782223;
		 DF[2] = (float)8.069715648566;
		 DF[3] = (float)-7.997069830767;
		 DF[4] = (float)4.569085792934;
		 DF[5] = (float)-1.429498613132;
		 DF[6] = (float)0.1870278540999;


	}

	  void CreaminoLib::IIR_05_40_500SPS() {

		  NF[0] = (float)0.009853425385308;
		  NF[1] = (float)0;
		  NF[2] = (float)-0.02956027615592;
		  NF[3] = (float)0;
		  NF[4] = (float)0.02956027615592;
		  NF[5] = (float)0;
		  NF[6] = (float)-0.009853425385308;

		  DF[0] = (float)1;
		  DF[1] = (float)-5.007967649796;
		  DF[2] = (float)10.48221355455;
		  DF[3] = (float)-11.76433667406;
		  DF[4] = (float)7.480442929736;
		  DF[5] = (float)-2.556885366478;
		  DF[6] = (float)0.3665332263403;

	 }


	   void CreaminoLib::IIR_05_20_500SPS() {

		   NF[0] = (float)0.001460477432912;
		   NF[1] = (float)0;
		   NF[2] = (float)-0.004381432298736;
		   NF[3] = (float)0;
		   NF[4] = (float)0.004381432298736;
		   NF[5] = (float)0;
		   NF[6] = (float)-0.001460477432912;

		   DF[0] = (float)1;
		   DF[1] = (float)-5.506711995898;
		   DF[2] = (float)12.65172137807;
		   DF[3] = (float)-15.52654408635;
		   DF[4] = (float)10.73658286157;
		   DF[5] = (float)-3.966858017298;
		   DF[6] = (float)0.6118098630404;

	  }

	    void CreaminoLib::IIR_2_65_500SPS() {

			NF[0] = (float)0.03232054543789;
			NF[1] = (float)0;
			NF[2] = (float)-0.09696163631367;
			NF[3] = (float)0;
			NF[4] = (float)0.09696163631367;
			NF[5] = (float)0;
			NF[6] = (float)-0.03232054543789;

			DF[0] = (float)1;
			DF[1] = (float)-4.398965204498;
			DF[2] = (float)8.095717822694;
			DF[3] = (float)-8.073908635408;
			DF[4] = (float)4.65195893902;
			DF[5] = (float)-1.469877270362;
			DF[6] = (float)0.1950788334642;

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