/*Matteo Chiesi - University of Bologna*/
//SerialLib.cpp

#include "SerialLib.h"
#include <vector>
using namespace std;

namespace SerialLibrary
{

	/*
	* Get serial port names
	*/

	std::vector<std::string> SerialLib::getSerialPortNames()
	{

		HKEY phkResult;
		LPCWSTR lpSubKey = (LPCWSTR)"HARDWARE\\DEVICEMAP\\SERIALCOMM\\";
		std::vector<std::string>  returnArray;

		if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, (LPCSTR)lpSubKey, 0, KEY_READ, &phkResult) == ERROR_SUCCESS) {
			bool hasMoreElements = true;
			DWORD keysCount = 0;
			char valueName[256];
			DWORD valueNameSize;
			DWORD enumResult;
			while (hasMoreElements) {
				valueNameSize = 256;
				enumResult = RegEnumValueA(phkResult, keysCount, valueName, &valueNameSize, NULL, NULL, NULL, NULL);
				if (enumResult == ERROR_SUCCESS) {
					keysCount++;
				}
				else if (enumResult == ERROR_NO_MORE_ITEMS) {
					hasMoreElements = false;
				}
				else {
					hasMoreElements = false;
				}
			}
			if (keysCount > 0) {
				returnArray.resize(keysCount);

				char lpValueName[256];
				DWORD lpcchValueName;
				byte lpData[256];
				DWORD lpcbData;
				DWORD result;
				for (DWORD i = 0; i < keysCount; i++) {
					lpcchValueName = 256;
					lpcbData = 256;
					result = RegEnumValueA(phkResult, i, lpValueName, &lpcchValueName, NULL, NULL, lpData, &lpcbData);
					if (result == ERROR_SUCCESS) {
						returnArray[i].append((char*)lpData);
					}
				}
			}
			CloseHandle(phkResult);
		}
		return returnArray;
	}


	/*
	* Port opening.
	*
	* In 2.2.0 added useTIOCEXCL (not used in Windows, only for compatibility with _nix version)
	*/
	HANDLE SerialLib::openPort(string portName, bool useTIOCEXCL) {

		string prefix = "\\\\.\\";
		string portFullName = prefix + portName;

		std::string stemp = std::string(portFullName.begin(), portFullName.end());
		LPCSTR swportFullName = stemp.c_str();

		HANDLE hComm = CreateFile(swportFullName,
			GENERIC_READ | GENERIC_WRITE, //access ( read and write)
			0, //(share) 0:cannot share the COM port
			0, //security  (None) 
			OPEN_EXISTING, // creation : open_existing
			FILE_FLAG_OVERLAPPED,  // we dont want overlapped operation
			0); // no templates file for COM port...

				//since 2.3.0 ->
		if (hComm != INVALID_HANDLE_VALUE) {
			DCB *dcb = new DCB();
			if (!GetCommState(hComm, dcb)) {
				CloseHandle(hComm);//since 2.7.0
				hComm = (HANDLE)ERR_INCORRECT_SERIAL_PORT;//(-4)Incorrect serial port
			}
			delete dcb;
		}
		else {
			DWORD errorValue = GetLastError();
			if (errorValue == ERROR_ACCESS_DENIED) {
				hComm = (HANDLE)ERR_PORT_BUSY;//(-1)Port busy
			}
			else if (errorValue == ERROR_FILE_NOT_FOUND) {
				hComm = (HANDLE)ERR_PORT_NOT_FOUND;//(-2)Port not found
			}
		}
		//<- since 2.3.0
		return hComm;//since 2.4.0 changed to jlong
	};


	/*
	* Setting serial port params.
	*
	* In 2.6.0 added flags (not used in Windows, only for compatibility with _nix version)
	*/
	bool SerialLib::setParams(HANDLE portHandle, int baudRate, int byteSize, int stopBits, int parity, bool setRTS, bool setDTR, int flags) {
		HANDLE hComm = (HANDLE)portHandle;
		DCB *dcb = new DCB();
		bool returnValue = false;
		if (GetCommState(hComm, dcb)) {
			dcb->BaudRate = baudRate;
			dcb->ByteSize = byteSize;
			dcb->StopBits = stopBits;
			dcb->Parity = parity;

			//since 0.8 ->
			if (setRTS == true) {
				dcb->fRtsControl = RTS_CONTROL_ENABLE;
			}
			else {
				dcb->fRtsControl = RTS_CONTROL_DISABLE;
			}
			if (setDTR == false) {
				dcb->fDtrControl = DTR_CONTROL_ENABLE;
			}
			else {
				dcb->fDtrControl = DTR_CONTROL_DISABLE;
			}
			dcb->fOutxCtsFlow = FALSE;
			dcb->fOutxDsrFlow = FALSE;
			dcb->fDsrSensitivity = FALSE;
			dcb->fTXContinueOnXoff = TRUE;
			dcb->fOutX = FALSE;
			dcb->fInX = FALSE;
			dcb->fErrorChar = FALSE;
			dcb->fNull = FALSE;
			dcb->fAbortOnError = FALSE;
			dcb->XonLim = 2048;
			dcb->XoffLim = 512;
			dcb->XonChar = (char)17; //DC1
			dcb->XoffChar = (char)19; //DC3
									  //<- since 0.8

			if (SetCommState(hComm, dcb)) {

				//since 2.1.0 -> previously setted timeouts by another application should be cleared
				COMMTIMEOUTS *lpCommTimeouts = new COMMTIMEOUTS();
				lpCommTimeouts->ReadIntervalTimeout = 0;
				lpCommTimeouts->ReadTotalTimeoutConstant = 0;
				lpCommTimeouts->ReadTotalTimeoutMultiplier = 0;
				lpCommTimeouts->WriteTotalTimeoutConstant = 0;
				lpCommTimeouts->WriteTotalTimeoutMultiplier = 0;
				if (SetCommTimeouts(hComm, lpCommTimeouts)) {
					returnValue = true;
				}
				delete lpCommTimeouts;
				//<- since 2.1.0
			}
		}
		delete dcb;
		return returnValue;
	}

	/*
	* PurgeComm
	*/
	bool SerialLib::purgePort(HANDLE portHandle, int flags) {
		HANDLE hComm = (HANDLE)portHandle;
		DWORD dwFlags = (DWORD)flags;
		return (PurgeComm(hComm, dwFlags) ? true : false);
	}

	/*
	* Port closing
	*/
	bool  SerialLib::closePort(HANDLE portHandle) {
		HANDLE hComm = (HANDLE)portHandle;
		return (CloseHandle(hComm) ? true : false);
	}

	/*
	* Set events mask
	*/
	bool SerialLib::setEventsMask(HANDLE portHandle, int mask) {
		HANDLE hComm = (HANDLE)portHandle;
		DWORD dwEvtMask = (DWORD)mask;
		return (SetCommMask(hComm, dwEvtMask) ? true : false);
	}



	/*
	* Get events mask
	*/
	int  SerialLib::getEventsMask(HANDLE portHandle) {
		HANDLE hComm = (HANDLE)portHandle;
		DWORD lpEvtMask;
		if (GetCommMask(hComm, &lpEvtMask)) {
			return (int)lpEvtMask;
		}
		else {
			return -1;
		}
	}

	/*
	* Change RTS line state (ON || OFF)
	*/
	bool SerialLib::setRTS(HANDLE portHandle, bool enabled) {
		HANDLE hComm = (HANDLE)portHandle;
		if (enabled == true) {
			return (EscapeCommFunction(hComm, SETRTS) ? true : false);
		}
		else {
			return (EscapeCommFunction(hComm, CLRRTS) ? true : false);
		}
	}

	/*
	* Change DTR line state (ON || OFF)
	*/
	bool SerialLib::setDTR(HANDLE portHandle, bool enabled) {
		HANDLE hComm = (HANDLE)portHandle;
		if (enabled == true) {
			return (EscapeCommFunction(hComm, SETDTR) ? true : false);
		}
		else {
			return (EscapeCommFunction(hComm, CLRDTR) ? true : false);
		}
	}

	/*
	* Wait event
	* portHandle - port handle
	*/
	vector<int> SerialLib::waitEvents(HANDLE portHandle) {
		HANDLE hComm = (HANDLE)portHandle;
		DWORD lpEvtMask = 0;
		DWORD lpNumberOfBytesTransferred = 0;
		OVERLAPPED *overlapped = new OVERLAPPED();
		vector<int>  returnValues(2);
		bool functionSuccessful = false;
		overlapped->hEvent = CreateEventA(NULL, true, false, NULL);
		if (WaitCommEvent(hComm, &lpEvtMask, overlapped)) {
			functionSuccessful = true;
		}
		else if (GetLastError() == ERROR_IO_PENDING) {
			if (WaitForSingleObject(overlapped->hEvent, INFINITE) == WAIT_OBJECT_0) {
				if (GetOverlappedResult(hComm, overlapped, &lpNumberOfBytesTransferred, false)) {
					functionSuccessful = true;
				}
			}
		}
		if (functionSuccessful) {
			bool executeGetCommModemStatus = false;
			bool executeClearCommError = false;
			DWORD events[9];//fixed since 0.8 (old value is 8)
			int eventsCount = 0;
			if ((EV_BREAK & lpEvtMask) == EV_BREAK) {
				events[eventsCount] = EV_BREAK;
				eventsCount++;
			}
			if ((EV_CTS & lpEvtMask) == EV_CTS) {
				events[eventsCount] = EV_CTS;
				eventsCount++;
				executeGetCommModemStatus = true;
			}
			if ((EV_DSR & lpEvtMask) == EV_DSR) {
				events[eventsCount] = EV_DSR;
				eventsCount++;
				executeGetCommModemStatus = true;
			}
			if ((EV_ERR & lpEvtMask) == EV_ERR) {
				events[eventsCount] = EV_ERR;
				eventsCount++;
				executeClearCommError = true;
			}
			if ((EV_RING & lpEvtMask) == EV_RING) {
				events[eventsCount] = EV_RING;
				eventsCount++;
				executeGetCommModemStatus = true;
			}
			if ((EV_RLSD & lpEvtMask) == EV_RLSD) {
				events[eventsCount] = EV_RLSD;
				eventsCount++;
				executeGetCommModemStatus = true;
			}
			if ((EV_RXCHAR & lpEvtMask) == EV_RXCHAR) {
				events[eventsCount] = EV_RXCHAR;
				eventsCount++;
				executeClearCommError = true;
			}
			if ((EV_RXFLAG & lpEvtMask) == EV_RXFLAG) {
				events[eventsCount] = EV_RXFLAG;
				eventsCount++;
				executeClearCommError = true;
			}
			if ((EV_TXEMPTY & lpEvtMask) == EV_TXEMPTY) {
				events[eventsCount] = EV_TXEMPTY;
				eventsCount++;
				executeClearCommError = true;
			}
			/*
			* Execute GetCommModemStatus function if it's needed (get lines status)
			*/
			int statusCTS = 0;
			int statusDSR = 0;
			int statusRING = 0;
			int statusRLSD = 0;
			bool successGetCommModemStatus = false;
			if (executeGetCommModemStatus) {
				DWORD lpModemStat;
				if (GetCommModemStatus(hComm, &lpModemStat)) {
					successGetCommModemStatus = true;
					if ((MS_CTS_ON & lpModemStat) == MS_CTS_ON) {
						statusCTS = 1;
					}
					if ((MS_DSR_ON & lpModemStat) == MS_DSR_ON) {
						statusDSR = 1;
					}
					if ((MS_RING_ON & lpModemStat) == MS_RING_ON) {
						statusRING = 1;
					}
					if ((MS_RLSD_ON & lpModemStat) == MS_RLSD_ON) {
						statusRLSD = 1;
					}
				}
				else {
					int lastError = (int)GetLastError();
					statusCTS = lastError;
					statusDSR = lastError;
					statusRING = lastError;
					statusRLSD = lastError;
				}
			}
			/*
			* Execute ClearCommError function if it's needed (get count of bytes in buffers and errors)
			*/
			int bytesCountIn = 0;
			int bytesCountOut = 0;
			int communicationsErrors = 0;
			bool successClearCommError = false;
			if (executeClearCommError) {
				DWORD lpErrors;
				COMSTAT *comstat = new COMSTAT();
				if (ClearCommError(hComm, &lpErrors, comstat)) {
					successClearCommError = true;
					bytesCountIn = (int)comstat->cbInQue;
					bytesCountOut = (int)comstat->cbOutQue;
					communicationsErrors = (int)lpErrors;
				}
				else {
					int lastError = (int)GetLastError();
					bytesCountIn = lastError;
					bytesCountOut = lastError;
					communicationsErrors = lastError;
				}
				delete comstat;
			}
			/*
			* Create int[][] for events values
			*/
			//returnArray = env->NewObjectArray(eventsCount, intClass, NULL);
			/*
			* Set events values
			*/
			for (int i = 0; i < eventsCount; i++) {
				switch (events[i]) {
				case EV_BREAK:
					returnValues[0] = (int)events[i];
					returnValues[1] = 0;
					goto forEnd;
				case EV_CTS:
					returnValues[1] = statusCTS;
					goto modemStatus;
				case EV_DSR:
					returnValues[1] = statusDSR;
					goto modemStatus;
				case EV_ERR:
					returnValues[1] = communicationsErrors;
					goto bytesAndErrors;
				case EV_RING:
					returnValues[1] = statusRING;
					goto modemStatus;
				case EV_RLSD:
					returnValues[1] = statusRLSD;
					goto modemStatus;
				case EV_RXCHAR:
					returnValues[1] = bytesCountIn;
					goto bytesAndErrors;
				case EV_RXFLAG:
					returnValues[1] = bytesCountIn;
					goto bytesAndErrors;
				case EV_TXEMPTY:
					returnValues[1] = bytesCountOut;
					goto bytesAndErrors;
				default:
					returnValues[0] = (int)events[i];
					returnValues[1] = 0;
					goto forEnd;
				};
			modemStatus: {
				if (successGetCommModemStatus) {
					returnValues[0] = (int)events[i];
				}
				else {
					returnValues[0] = -1;
				}
				goto forEnd;
			}
					 bytesAndErrors: {
						 if (successClearCommError) {
							 returnValues[0] = (int)events[i];
						 }
						 else {
							 returnValues[0] = -1;
						 }
						 goto forEnd;
					 }
								 forEnd: {
									 ;
								 };
			}
		}
		else {

			returnValues[0] = -1;
			returnValues[1] = (int)GetLastError();
		};
		CloseHandle(overlapped->hEvent);
		delete overlapped;
		return returnValues;
	}

	/*
	* Read data from port
	* portHandle - port handle
	* byteCount - count of bytes for reading
	*/
	vector<byte> SerialLib::readBytes(HANDLE portHandle, int byteCount) {

		HANDLE hComm = (HANDLE)portHandle;
		DWORD lpNumberOfBytesTransferred;
		DWORD lpNumberOfBytesRead;
		OVERLAPPED *overlapped = new OVERLAPPED();
		BOOL fWaitingOnRead = FALSE;
		vector<byte>  lpBuffer(byteCount);
		//byte* lpBuffer2 = (byte*)malloc(byteCount*sizeof(byte));
		overlapped->hEvent = CreateEventA(NULL, true, false, NULL);
		fWaitingOnRead = ReadFile(hComm, &lpBuffer[0], (DWORD)byteCount, &lpNumberOfBytesRead, overlapped);
		if (!fWaitingOnRead)
			if (GetLastError() == ERROR_IO_PENDING) {
				if (WaitForSingleObject(overlapped->hEvent, INFINITE) == WAIT_OBJECT_0) { //wait until the object is signaled -> This does not mean that it was completed successfully, just that it was completed.
					if (GetOverlappedResult(hComm, overlapped, &lpNumberOfBytesTransferred, false)) {
						fWaitingOnRead = true;
					}
				}
			}
		CloseHandle(overlapped->hEvent);
		delete overlapped;
		return lpBuffer;
	}

	/*
	* Write data to port
	* portHandle - port handle
	* buffer - byte array for sending
	*/
	bool  SerialLib::writeBytes(HANDLE portHandle, vector<byte> &buffer, int lenght) {
		HANDLE hComm = (HANDLE)portHandle;
		DWORD lpNumberOfBytesTransferred;
		DWORD lpNumberOfBytesWritten;
		OVERLAPPED *overlapped = new OVERLAPPED();
		bool returnValue = false;
		overlapped->hEvent = CreateEventA(NULL, true, false, NULL);
		if (WriteFile(hComm, &buffer[0], (DWORD)lenght, &lpNumberOfBytesWritten, overlapped)) {
			returnValue = true;
		}
		else if (GetLastError() == ERROR_IO_PENDING) {
			if (WaitForSingleObject(overlapped->hEvent, INFINITE) == WAIT_OBJECT_0) {
				if (GetOverlappedResult(hComm, overlapped, &lpNumberOfBytesTransferred, false)) {
					returnValue = true;
				}
			}
		}
		CloseHandle(overlapped->hEvent);
		delete overlapped;
		return returnValue;
	}

	/*
	* Get bytes count in serial port buffers (Input and Output)
	*/
	vector<int> SerialLib::getBuffersBytesCount(HANDLE portHandle) {
		HANDLE hComm = (HANDLE)portHandle;
		vector<int> returnValues(2);
		returnValues[0] = -1;
		returnValues[1] = -1;
		DWORD lpErrors;
		COMSTAT *comstat = new COMSTAT();
		if (ClearCommError(hComm, &lpErrors, comstat)) {
			returnValues[0] = (int)comstat->cbInQue;
			returnValues[1] = (int)comstat->cbOutQue;
		}
		else {
			returnValues[0] = -1;
			returnValues[1] = -1;
		}
		delete comstat;
		return returnValues;
	}

	//since 0.8 ->
	const int FLOWCONTROL_NONE = 0;
	const int FLOWCONTROL_RTSCTS_IN = 1;
	const int FLOWCONTROL_RTSCTS_OUT = 2;
	const int FLOWCONTROL_XONXOFF_IN = 4;
	const int FLOWCONTROL_XONXOFF_OUT = 8;
	//<- since 0.8

	/*
	* Setting flow control mode
	*
	* since 0.8
	*/
	bool SerialLib::setFlowControlMode(HANDLE portHandle, int mask) {
		HANDLE hComm = (HANDLE)portHandle;
		bool returnValue = false;
		DCB *dcb = new DCB();
		if (GetCommState(hComm, dcb)) {
			dcb->fRtsControl = RTS_CONTROL_ENABLE;
			dcb->fOutxCtsFlow = FALSE;
			dcb->fOutX = FALSE;
			dcb->fInX = FALSE;
			if (mask != FLOWCONTROL_NONE) {
				if ((mask & FLOWCONTROL_RTSCTS_IN) == FLOWCONTROL_RTSCTS_IN) {
					dcb->fRtsControl = RTS_CONTROL_HANDSHAKE;
				}
				if ((mask & FLOWCONTROL_RTSCTS_OUT) == FLOWCONTROL_RTSCTS_OUT) {
					dcb->fOutxCtsFlow = TRUE;
				}
				if ((mask & FLOWCONTROL_XONXOFF_IN) == FLOWCONTROL_XONXOFF_IN) {
					dcb->fInX = TRUE;
				}
				if ((mask & FLOWCONTROL_XONXOFF_OUT) == FLOWCONTROL_XONXOFF_OUT) {
					dcb->fOutX = TRUE;
				}
			}
			if (SetCommState(hComm, dcb)) {
				returnValue = true;
			}
		}
		delete dcb;
		return returnValue;
	}

	/*
	* Getting flow control mode
	*
	* since 0.8
	*/
	int SerialLib::getFlowControlMode(HANDLE portHandle) {
		HANDLE hComm = (HANDLE)portHandle;
		int returnValue = 0;
		DCB *dcb = new DCB();
		if (GetCommState(hComm, dcb)) {
			if (dcb->fRtsControl == RTS_CONTROL_HANDSHAKE) {
				returnValue |= FLOWCONTROL_RTSCTS_IN;
			}
			if (dcb->fOutxCtsFlow == TRUE) {
				returnValue |= FLOWCONTROL_RTSCTS_OUT;
			}
			if (dcb->fInX == TRUE) {
				returnValue |= FLOWCONTROL_XONXOFF_IN;
			}
			if (dcb->fOutX == TRUE) {
				returnValue |= FLOWCONTROL_XONXOFF_OUT;
			}
		}
		delete dcb;
		return returnValue;
	}


	/*
	* Get lines status
	*
	* returnValues[0] - CTS
	* returnValues[1] - DSR
	* returnValues[2] - RING
	* returnValues[3] - RLSD
	*
	*/
	vector<int> SerialLib::getLinesStatus(HANDLE portHandle) {
		HANDLE hComm = (HANDLE)portHandle;
		DWORD lpModemStat;
		vector<int> returnValues(4);
		for (int i = 0; i < 4; i++) {
			returnValues[i] = 0;
		}
		if (GetCommModemStatus(hComm, &lpModemStat)) {
			if ((MS_CTS_ON & lpModemStat) == MS_CTS_ON) {
				returnValues[0] = 1;
			}
			if ((MS_DSR_ON & lpModemStat) == MS_DSR_ON) {
				returnValues[1] = 1;
			}
			if ((MS_RING_ON & lpModemStat) == MS_RING_ON) {
				returnValues[2] = 1;
			}
			if ((MS_RLSD_ON & lpModemStat) == MS_RLSD_ON) {
				returnValues[3] = 1;
			}
		}

		return returnValues;
	}

	/*
	* Send break for setted duration
	*
	* since 0.8
	*/
	bool SerialLib::sendBreak(HANDLE portHandle, int duration) {
		HANDLE hComm = (HANDLE)portHandle;
		bool returnValue = false;
		if (duration > 0) {
			if (SetCommBreak(hComm) > 0) {
				Sleep(duration);
				if (ClearCommBreak(hComm) > 0) {
					returnValue = true;
				}
			}
		}
		return returnValue;
	}

}