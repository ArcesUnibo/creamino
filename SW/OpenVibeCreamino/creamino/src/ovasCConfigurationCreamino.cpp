#include "ovasCConfigurationCreamino.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBEAcquisitionServer;
using namespace std;

/*_________________________________________________

Insert callback to specific widget here
Example with a button that launch a calibration of the device:

//Callback connected to a dedicated gtk button:
static void button_calibrate_pressed_cb(::GtkButton* pButton, void* pUserData)
{
	CConfigurationCreamino* l_pConfig=static_cast<CConfigurationCreamino*>(pUserData);
	l_pConfig->buttonCalibratePressedCB();
}

//Callback actually called:
void CConfigurationGTecGUSBamp::buttonCalibratePressedCB(void)
{
	// Connect to the hardware, ask for calibration, verify the return code, etc.
}
_________________________________________________*/

// If you added more reference attribute, initialize them here
CConfigurationCreamino::CConfigurationCreamino(IDriverContext& rDriverContext, const char* sGtkBuilderFileName, OpenViBE::uint32& rUSBIndex, OpenViBE::CString& COMPort, OpenViBE::uint8& ChipSelect, OpenViBE::uint8& ADSMode, OpenViBE::uint8& Gain, OpenViBE::CString& CalibrationFile)
	:CConfigurationBuilder(sGtkBuilderFileName)
	,m_rDriverContext(rDriverContext)
	,m_rUSBIndex(rUSBIndex)
	,m_ChipSelect(ChipSelect)
	,m_COMPort(COMPort)
	,m_ADSMode(ADSMode)
	,m_Gain(Gain)
	,m_CalibrationFile(CalibrationFile)
{
	m_pListStore = gtk_list_store_new(1, G_TYPE_STRING);
}

CConfigurationCreamino::~CConfigurationCreamino(void)
{
	g_object_unref(m_pListStore);

}


bool CConfigurationCreamino::preConfigure(void)
{
	if(! CConfigurationBuilder::preConfigure())
	{
		return false;
	}


	::GtkComboBox* l_pComboBox = GTK_COMBO_BOX(gtk_builder_get_object(m_pBuilderConfigureInterface, "combobox_device"));
	
	g_object_unref(m_pListStore);
	m_pListStore = gtk_list_store_new(1, G_TYPE_STRING);

	gtk_combo_box_set_model(l_pComboBox, GTK_TREE_MODEL(m_pListStore));
	
	char l_sBuffer[1024];
	bool l_bSelected = false;

	std::vector<std::string>  PortName;
	PortName = SerialLibrary::SerialLib::getSerialPortNames();

	for (int i = 0; i < PortName.size(); i++)
	{
#if defined TARGET_OS_Windows
		//::sprintf(l_sBuffer, "\\\\.\\COM%i", i);
		::sprintf(l_sBuffer, PortName[i].c_str());
#elif defined TARGET_OS_Linux
		if (i<10)
		{
			::sprintf(l_sBuffer, i<10 ? "/dev/ttyS%d" : "/dev/ttyUSB%d", i);
		}
		else
		{
			::sprintf(l_sBuffer, "/dev/ttyUSB%d", i - 10);
		}
#else
		::sprintf(l_sBuffer, "");
#endif
		::gtk_combo_box_append_text(l_pComboBox, l_sBuffer);
		if (m_rUSBIndex == i)
		{
			::gtk_combo_box_set_active(l_pComboBox, i - 1);
			l_bSelected = true;
		}
	}

	if (!l_bSelected)
	{
		::gtk_combo_box_set_active(l_pComboBox, 0);
	}



	// Connect here all callbacks
	// Example:
	// g_signal_connect(gtk_builder_get_object(m_pBuilderConfigureInterface, "button_calibrate"), "pressed", G_CALLBACK(button_calibrate_pressed_cb), this);

	// Insert here the pre-configure code.
	// For example, you may want to check if a device is currently connected
	// and if more than one are connected. Then you can list in a dedicated combo-box 
	// the device currently connected so the user can choose which one he wants to acquire from.

	return true;
}

bool CConfigurationCreamino::postConfigure(void)
{
	::GtkComboBox* l_pComboBox = GTK_COMBO_BOX(gtk_builder_get_object(m_pBuilderConfigureInterface, "combobox_device"));
	::GtkComboBox* l_pChipSelect = GTK_COMBO_BOX(gtk_builder_get_object(m_pBuilderConfigureInterface, "combobox_chipselect"));
	::GtkComboBox* l_pADSMode = GTK_COMBO_BOX(gtk_builder_get_object(m_pBuilderConfigureInterface, "combobox_ADSMode"));
	::GtkComboBox* l_pGain = GTK_COMBO_BOX(gtk_builder_get_object(m_pBuilderConfigureInterface, "combobox_Gain"));
	::GtkEntry* l_pCalibrationFile = GTK_ENTRY(gtk_builder_get_object(m_pBuilderConfigureInterface, "Calibration_name"));
	
	if(m_bApplyConfiguration)
	{
		int l_iUSBIndex = gtk_combo_box_get_active(l_pComboBox);
		int l_iChipSelect = gtk_combo_box_get_active(l_pChipSelect);
		int l_ipADSMode = gtk_combo_box_get_active(l_pADSMode);
		int l_ipGain = gtk_combo_box_get_active(l_pGain);
		m_CalibrationFile = gtk_entry_get_text(l_pCalibrationFile);

		if (l_iUSBIndex >= 0)
		{
			m_rUSBIndex = (uint32)l_iUSBIndex + 1;
			m_COMPort = (CString)gtk_combo_box_get_active_text(l_pComboBox);
		}

		if (l_iChipSelect == 0)
			m_ChipSelect = 4;
		else if(l_iChipSelect == 1)
			m_ChipSelect = 10;
		else
			m_ChipSelect = 52;

		if (l_ipADSMode == 0)
			m_ADSMode = 5;
		else
			m_ADSMode = 0;
		

	if (l_ipGain == 0)
			m_Gain = 0x10;
	else if (l_ipGain == 1)
			m_Gain = 0x20;
	else if (l_ipGain == 2)
			m_Gain = 0x30;
	else if (l_ipGain == 3)
			m_Gain = 0x40;
	else if (l_ipGain == 4)
			m_Gain = 0x00;
	else if (l_ipGain == 5)
			m_Gain = 0x50;
	else if (l_ipGain == 6)
			m_Gain = 0x60;
	else 
			m_Gain = 0x10;

	}

	if(! CConfigurationBuilder::postConfigure()) // normal header is filled (Subject ID, Age, Gender, channels, sampling frequency), ressources are realesed
	{
		return false;
	}

	return true;
}
