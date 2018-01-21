
/*Matteo Chiesi - University of Bologna*/
// SerialLib.h

#include <Windows.h>
#include <string>
#include <vector>

using namespace std;

#undef jSSC_NATIVE_LIB_VERSION
#define jSSC_NATIVE_LIB_VERSION "2.8"

#undef jssc_SerialNativeInterface_OS_LINUX
#define jssc_SerialNativeInterface_OS_LINUX 0L
#undef jssc_SerialNativeInterface_OS_WINDOWS
#define jssc_SerialNativeInterface_OS_WINDOWS 1L
#undef jssc_SerialNativeInterface_OS_SOLARIS
#define jssc_SerialNativeInterface_OS_SOLARIS 2L
#undef jssc_SerialNativeInterface_OS_MAC_OS_X
#define jssc_SerialNativeInterface_OS_MAC_OS_X 3L
#undef ERR_PORT_BUSY
#define ERR_PORT_BUSY -1LL
#undef ERR_PORT_NOT_FOUND
#define ERR_PORT_NOT_FOUND -2LL
#undef ERR_PERMISSION_DENIED
#define ERR_PERMISSION_DENIED -3LL
#undef ERR_INCORRECT_SERIAL_PORT
#define ERR_INCORRECT_SERIAL_PORT -4LL


namespace SerialLibrary
{
	class SerialLib
	{
	public:

		/*
		* Class:    SerialLibrary
		* Method:   getSerialPortNames
		*/
		static std::vector<std::string> getSerialPortNames();

		/*
		* Class:	SerialLibrary
		* Method:	openPort
		*/

		static HANDLE openPort(string, bool);

		/*
		* Class:     SerialLibrary
		* Method:    setParams
		*/

		static bool setParams(HANDLE, int, int, int, int, bool, bool, int);

		/*
		* Class:     SerialLibrary
		* Method:    purgePort
		*/

		static bool  purgePort(HANDLE, int);

		/*
		* Class:     SerialLibrary
		* Method:    closePort
		*/

		static bool  closePort(HANDLE);

		/*
		* Class:     SerialLibrary
		* Method:    setEventsMask
		*/
		static bool setEventsMask(HANDLE, int);

		/*
		* Class:     SerialLibrary
		* Method:    getEventsMask
		*/
		static int getEventsMask(HANDLE);

		/*
		* Class:     SerialLibrary
		* Method:    waitEvents
		*/
		static vector<int> waitEvents(HANDLE);

		/*
		* Class:     SerialLibrary
		* Method:    setRTS
		*/
		static bool setRTS(HANDLE, bool);

		/*
		* Class:     SerialLibrary
		* Method:    setDTR
		*/
		static bool setDTR(HANDLE, bool);

		/*
		* Class:     SerialLibrary
		* Method:    readBytes
		*/
		static vector<BYTE> readBytes(HANDLE, int);

		/*
		* Class:     SerialLibrary
		* Method:    writeBytes
		*/
		static bool writeBytes(HANDLE, vector<BYTE> &, int);

		/*
		* Class:     SerialLibrary
		* Method:    getBuffersBytesCount
		*/
		static vector<int> getBuffersBytesCount(HANDLE);

		/*
		* Class:     SerialLibrary
		* Method:    setFlowControlMode
		*/
		static bool setFlowControlMode(HANDLE, int);

		/*
		* Class:     SerialLibrary
		* Method:    getFlowControlMode
		*/
		static int getFlowControlMode(HANDLE);

		/*
		* Class:     SerialLibrary
		* Method:    getLinesStatus
		*/
		static vector<int> getLinesStatus(HANDLE);

		/*
		* Class:     SerialLibrary
		* Method:    sendBreak
		*/
		static bool sendBreak(HANDLE, int);


	};
}