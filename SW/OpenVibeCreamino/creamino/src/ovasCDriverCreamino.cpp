#include "ovasCDriverCreamino.h"
#include "ovasCConfigurationCreamino.h"
#include <string>

#include <toolkit/ovtk_all.h>

using namespace OpenViBEAcquisitionServer;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace std;

//___________________________________________________________________//
//                                                                   //

CDriverCreamino::CDriverCreamino(IDriverContext& rDriverContext)
	:IDriver(rDriverContext)
	, m_oSettings("AcquisitionServer_Driver_Creamino", m_rDriverContext.getConfigurationManager())
	, m_pCallback(NULL)
	, m_ui32SampleCountPerSentBlock(0)
	, m_ui32DeviceIdentifier(uint32(-1))
	, m_COMPort("COM4")
	, m_ADS_SampleRate(uint8(6))
	, m_ChipSelect(uint8(4))
	, m_ADSNum(uint8(0x01))
	, m_ADSMode(uint8(0x00))
	, m_Gain(uint8(0x10))
	, m_pSample(NULL)
	, m_ui32TotalSampleCount(0)
	, m_ui32StartTime(0)
	, m_bSendPeriodicStimulations(false)
{
	
	m_rDriverContext.getLogManager() << LogLevel_Trace << "CDriverCreamino::CDriverCreamino\n";
	m_oHeader.setSamplingFrequency(500);
	m_oHeader.setChannelCount(64);
	
	// The following class allows saving and loading driver settings from the acquisition server .conf file
	m_oSettings.add("Header", &m_oHeader);
	// To save your custom driver settings, register each variable to the SettingsHelper
	m_oSettings.add("DeviceIdentifier", &m_ui32DeviceIdentifier);
	m_oSettings.load();	
}

CDriverCreamino::~CDriverCreamino(void)
{
}

const char* CDriverCreamino::getName(void)
{
	return "Creamino Driver";
}

//___________________________________________________________________//
//                                                                   //

bool CDriverCreamino::initialize(
	const uint32_t ui32SampleCountPerSentBlock,
	IDriverCallback& rCallback)
{
	m_rDriverContext.getLogManager() << LogLevel_Trace << "CDriverCreamino::initialize\n";

	if(m_rDriverContext.isConnected()) return false;
	if(!m_oHeader.isChannelCountSet()||!m_oHeader.isSamplingFrequencySet()) return false;
	
	// Builds up a buffer to store
	// acquired samples. This buffer
	// will be sent to the acquisition
	// server later...
	cout << "Number of channels " << m_oHeader.getChannelCount() << '\n';
	cout << "Sample Count per sent block " << ui32SampleCountPerSentBlock << '\n';
	
	m_ADSNum = m_oHeader.getChannelCount() / 8;
	m_OutputBuffer = new float32[m_oHeader.getChannelCount()*ui32SampleCountPerSentBlock];
	m_pSample=new float32[m_oHeader.getChannelCount()*ui32SampleCountPerSentBlock];
	m_NotchFilteredWord = new float32[m_oHeader.getChannelCount()*ui32SampleCountPerSentBlock];
	m_FilteredWord = new float32[m_oHeader.getChannelCount()*ui32SampleCountPerSentBlock];

	if(!m_pSample)
	{
		delete [] m_pSample;
		m_pSample=NULL;
		return false;
	}
	
	switch (m_oHeader.getSamplingFrequency()){
		case(500):
			m_ADS_SampleRate = 6;
			break;
		case(1000) :
			m_ADS_SampleRate = 5;
			break;
		case(2000) :
			m_ADS_SampleRate = 4;
			break;
		case(4000) :
			m_ADS_SampleRate = 3;
			break;
		case(8000) :
			m_ADS_SampleRate = 2;
			break;
		default:
			m_ADS_SampleRate = 6;

	}

	cout << "SAMPLE COUNT PER BLOCK : " << ui32SampleCountPerSentBlock << "\n";
	cout << "COM PORT : " << m_COMPort << " \n";
	cout << "ADS SAMPLE RATE : " << unsigned(m_ADS_SampleRate) << " \n";
	cout << "Chip Select : " << unsigned(m_ChipSelect) << " \n";
	cout << "ADS Num : " << unsigned(m_ADSNum) << " \n";
	cout << "ADS Mode : " << unsigned(m_ADSMode) << " \n";
	cout << "Gain : " << unsigned(m_Gain) << " \n";
	
	// Saves parameters
	m_pCallback=&rCallback;
	m_ui32SampleCountPerSentBlock=ui32SampleCountPerSentBlock;
	return true;
}

bool CDriverCreamino::start(void)
{
	if(!m_rDriverContext.isConnected()) return false;
	if(m_rDriverContext.isStarted()) return false;


	// ...
	// request hardware to start
	// sending data
	// ...


	CreaminoLibrary::CreaminoLib::ResetNotchFilter();
	CreaminoLibrary::CreaminoLib::NoFilter();
	CreaminoLibrary::CreaminoLib::IIR_05_65_500SPS();

	int init = CreaminoLibrary::CreaminoLib::CreaminoStart((string)m_COMPort, m_ADS_SampleRate, (unsigned char)m_oHeader.getChannelCount(), m_ADSMode, m_ADSNum, m_ChipSelect, m_Gain);

	return true;
}

bool CDriverCreamino::loop(void)
{
	if(!m_rDriverContext.isConnected()) return false;
	if(!m_rDriverContext.isStarted()) return true;

	OpenViBE::CStimulationSet l_oStimulationSet;

	CreaminoLibrary::CreaminoLib::CreaminoWaitforData(m_OutputBuffer, m_ui32SampleCountPerSentBlock);
	
	//Uncomment and set the filter if you want to filter the signal
	/*
	for (int j = 0; j < m_ui32SampleCountPerSentBlock; j++) 
		for (int i = 0; i < m_oHeader.getChannelCount(); i++) {
			m_NotchFilteredWord[i + j*m_oHeader.getChannelCount()] = CreaminoLibrary::CreaminoLib::CreaminoIIR6(i, m_OutputBuffer[i + j*m_oHeader.getChannelCount()]);
			//m_FilteredWord[i + j*m_oHeader.getChannelCount()] = CreaminoLibrary::CreaminoLib::CreaminoIIR6(i, m_NotchFilteredWord[i + j*m_oHeader.getChannelCount()]);
//			m_FilteredWord[i + j*m_oHeader.getChannelCount()] = CreaminoLibrary::CreaminoLib::CreaminoIIR6(i, m_NotchFilteredWord[i + j*m_oHeader.getChannelCount()]);
			//m_pSample[j + i*m_ui32SampleCountPerSentBlock] =  m_FilteredWord[j + i*m_ui32SampleCountPerSentBlock];
	}
	
*/



	// ...
	// receive samples from hardware
	// put them the correct way in the sample array
	// whether the buffer is full, send it to the acquisition server
	//...
	m_pCallback->setSamples(m_OutputBuffer);
	//m_pCallback->setSamples(m_NotchFilteredWord);
	//m_pCallback->setSamples(m_FilteredWord);
	
	// When your sample buffer is fully loaded, 
	// it is advised to ask the acquisition server 
	// to correct any drift in the acquisition automatically.
	m_rDriverContext.correctDriftSampleCount(m_rDriverContext.getSuggestedDriftCorrectionSampleCount());

	// ...
	// receive events from hardware
	// and put them the correct way in a CStimulationSet object
	//...
	m_pCallback->setStimulationSet(l_oStimulationSet);

	return true;
}

bool CDriverCreamino::stop(void)
{
	if(!m_rDriverContext.isConnected()) return false;
	if(!m_rDriverContext.isStarted()) return false;

	int init = CreaminoLibrary::CreaminoLib::CreaminoStop();
	// ...
	// request the hardware to stop
	// sending data
	// ...

	return true;
}

bool CDriverCreamino::uninitialize(void)
{
	if(!m_rDriverContext.isConnected()) return false;
	if(m_rDriverContext.isStarted()) return false;

	// ...
	// uninitialize hardware here
	// ...

	delete [] m_pSample;
	m_pSample=NULL;
	m_pCallback=NULL;

	return true;
}

//___________________________________________________________________//
//                                                                   //
bool CDriverCreamino::isConfigurable(void)
{
	return true; // change to false if your device is not configurable
}

bool CDriverCreamino::configure(void)
{
	// Change this line if you need to specify some references to your driver attribute that need configuration, e.g. the connection ID.
	CConfigurationCreamino m_oConfiguration(m_rDriverContext, OpenViBE::Directories::getDataDir() + "/applications/acquisition-server/interface-Creamino.ui", m_rUSBIndex, m_COMPort, m_ChipSelect, m_ADSMode, m_Gain);
	
	
	if(!m_oConfiguration.configure(m_oHeader))
	{
		return false;
	}
	m_oSettings.save();
	
	return true;
}
