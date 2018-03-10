#ifndef __OpenViBE_AcquisitionServer_CDriverCreamino_H__
#define __OpenViBE_AcquisitionServer_CDriverCreamino_H__

#include "ovasIDriver.h"
#include "../ovasCHeader.h"
#include <openvibe/ov_all.h>

#include "../ovasCSettingsHelper.h"
#include "../ovasCSettingsHelperOperators.h"

namespace OpenViBEAcquisitionServer
{
	/**
	 * \class CDriverCreamino
	 * \author Matteo Chiesi (University of Bologna)
	 * \date Sun Nov 26 16:10:28 2017
	 * \brief The CDriverCreamino allows the acquisition server to acquire data from a Creamino device.
	 *
	 * TODO: details
	 *
	 * \sa CConfigurationCreamino
	 */
	class CDriverCreamino : public OpenViBEAcquisitionServer::IDriver
	{
	public:

		CDriverCreamino(OpenViBEAcquisitionServer::IDriverContext& rDriverContext);
		virtual ~CDriverCreamino(void);
		virtual const char* getName(void);

		virtual bool initialize(
			const uint32_t ui32SampleCountPerSentBlock,
			OpenViBEAcquisitionServer::IDriverCallback& rCallback);
		virtual bool uninitialize(void);

		virtual bool start(void);
		virtual bool stop(void);
		virtual bool loop(void);

		virtual bool isConfigurable(void);
		virtual bool configure(void);
		virtual const OpenViBEAcquisitionServer::IHeader* getHeader(void) { return &m_oHeader; }
		
		virtual bool isFlagSet(
			const OpenViBEAcquisitionServer::EDriverFlag eFlag) const
		{
			//return eFlag==DriverFlag_IsUnstable;
			return eFlag == 1;
		}

	protected:
		
		SettingsHelper m_oSettings;
		
		OpenViBEAcquisitionServer::IDriverCallback* m_pCallback;

		// Replace this generic Header with any specific header you might have written
		OpenViBEAcquisitionServer::CHeader m_oHeader;

		uint32_t m_ui32SampleCountPerSentBlock;
		OpenViBE::float32* m_pSample;
		OpenViBE::float32* m_FilteredWord;
		OpenViBE::float32* m_NotchFilteredWord;
		OpenViBE::uint32 m_ui32TotalSampleCount;
		OpenViBE::uint32 m_ui32StartTime;
		OpenViBE::float32* m_OutputBuffer;
		OpenViBE::uint32 m_ui32DeviceIdentifier;
		OpenViBE::uint32 m_rUSBIndex;
		OpenViBE::uint8 m_ADS_SampleRate;
		OpenViBE::uint8 m_ADSNum;
		OpenViBE::CString m_COMPort;
		OpenViBE::uint8 m_ChipSelect;
		OpenViBE::uint8 m_ADSMode;
		OpenViBE::uint8 m_Gain;
		OpenViBE::CString m_CalibrationFile;

	private:

		OpenViBE::boolean m_bSendPeriodicStimulations;
		/*
		 * Insert here all specific attributes, such as USB port number or device ID.
		 * Example :
		 */
		// uint32_t m_ui32ConnectionID;
	};
};

#endif // __OpenViBE_AcquisitionServer_CDriverCreamino_H__
