#ifndef __OpenViBE_AcquisitionServer_CConfigurationCreamino_H__
#define __OpenViBE_AcquisitionServer_CConfigurationCreamino_H__

#include "../ovasCConfigurationBuilder.h"
#include "ovasIDriver.h"

#include <gtk/gtk.h>
#include "CreaminoLib.h"

namespace OpenViBEAcquisitionServer
{
	/**
	 * \class CConfigurationCreamino
	 * \author Matteo Chiesi (University of Bologna)
	 * \date Sun Nov 26 16:10:28 2017
	 * \brief The CConfigurationCreamino handles the configuration dialog specific to the Creamino device.
	 *
	 * TODO: details
	 *
	 * \sa CDriverCreamino
	 */
	class CConfigurationCreamino : public OpenViBEAcquisitionServer::CConfigurationBuilder
	{
	public:

		// you may have to add to your constructor some reference parameters
		// for example, a connection ID:
		CConfigurationCreamino(OpenViBEAcquisitionServer::IDriverContext& rDriverContext, const char* sGtkBuilderFileName, OpenViBE::uint32& rUSBIndex, OpenViBE::CString& COMPort, OpenViBE::uint8& ChipSelect, OpenViBE::uint8& ADSMode, OpenViBE::uint8& Gain);
		virtual ~CConfigurationCreamino(void);

		virtual bool preConfigure(void);
		virtual bool postConfigure(void);

	protected:

		OpenViBEAcquisitionServer::IDriverContext& m_rDriverContext;
		OpenViBE::uint32& m_rUSBIndex;
		OpenViBE::CString& m_COMPort;
		::GtkListStore* m_pListStore;
		OpenViBE::uint8& m_ChipSelect;
		OpenViBE::uint8& m_ADSMode;
		OpenViBE::uint8& m_Gain;

	private:

		/*
		 * Insert here all specific attributes, such as a connection ID.
		 * use references to directly modify the corresponding attribute of the driver
		 * Example:
		 */
		// uint32_t& m_ui32ConnectionID;
	};
};

#endif // __OpenViBE_AcquisitionServer_CConfigurationCreamino_H__
