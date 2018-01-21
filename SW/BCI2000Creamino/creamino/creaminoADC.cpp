////////////////////////////////////////////////////////////////////////////////
// Authors: Matteo Chiesi - University of Bologna
// Description: CreaminoADC implementation
////////////////////////////////////////////////////////////////////////////////

#include "CreaminoADC.h"
#include "BCIStream.h"
#include "BCIEvent.h"
#include <stdio.h>
#include <sstream>

// In order to help you write a source module, exchange of information
// between amplifier and the BCI2000 source module is indicated by the use of
// macros.
// Once you are done with writing the source module, each occurrence of
// GET_FROM_AMP_API(), CALL_AMP_API(), and AMP_API_SYNC_GET_DATA() should
// have been replaced with actual calls to the amplifier API, or constants.
// By removing or disabling those macros' definitions below, you can then
// make sure that the compiler will notify you of any oversights.

// Depending on the kind of amplifier you have, occurrences of GET_FROM_AMP_API()
// may be read through the amplifier API, or obtained from documentation.
#define GET_FROM_AMP_API( what, example ) (what = example)
#define CALL_AMP_API( what ) (true)
#include "ThreadUtils.h"
#define AMP_API_SYNC_GET_DATA( dataptr ) (ThreadUtils::SleepForMs(50)) // simulate blocking call
#define AMP_API_INVALID_DEVICE_HANDLE (0)

using namespace std;

// Make the source filter known to the framework.
RegisterFilter( CreaminoADC, 1 ); // ADC filters must be registered at location "1" in order to work.

static bool Connected = false;

CreaminoADC::CreaminoADC()
: mDeviceHandle( AMP_API_INVALID_DEVICE_HANDLE ), // Each plain data member should appear in the initializer list.
  mpBuffer( 0 ) // Make sure we can call delete[] without crashing even if we never called new[].
{
}

CreaminoADC::~CreaminoADC()
{
  // The destructor is where to make sure that all acquired resources are released.
  // Memory deallocation (calling delete[] on a NULL pointer is OK).
  delete[] mpBuffer;
  // Closing connection to device.
  // The amplifier's API might choke on an invalid device handle.
  if( mDeviceHandle != AMP_API_INVALID_DEVICE_HANDLE )
    CALL_AMP_API( CloseDevice( mDeviceHandle ) );
}

void
CreaminoADC::OnPublish()
{
  // Declare any parameters that the filter needs....
	//cout << "pippo" << '\n';
	//inCount = 32;
	//Buffer = (float*)malloc(inCount*CH8 * sizeof(float));
	//NotchWord = (float*)malloc(inCount*CH8 * sizeof(float));
	//FilteredWord = (float*)malloc(inCount*CH8 * sizeof(float));
 BEGIN_PARAMETER_DEFINITIONS

    "Source:Signal%20Properties int SourceCh= 8 "
       "8 1 % // number of digitized and stored channels",
	"Source:Signal%20Properties int SampleBlockSize= 32 "
       "32 1 % // number of samples transmitted at a time",
    "Source:Signal%20Properties float SamplingRate= 500 "
       "500Hz 0.0 % // sample rate",
	"Source:Signal%20Properties:DataIOFilter list ChannelNames= 8 FP1 FP2 AF3 AF4 F7 F3 FZ F4" 
	   "% % // list of channel names",
	"Source:Signal%20Properties:DataIOFilter floatlist SourceChOffset= 8 0 0 0 0 0 0 0 0 " 
	   "% % // Offset for channels in A/D units",
	"Source:Signal%20Properties:DataIOFilter floatlist SourceChGain= 8 1 1 1 1 1 1 1 1"
	   "% % // Gain for  each channel",
	 "Source:Signal%20Properties: string COMport= COM17 "
	   "COM17 0 % // COMport for CreaminoAmp",
    "Source:Signal%20Properties int MyAmpSetting= 4 "
       "% % // some user setting specific to your amp",

 END_PARAMETER_DEFINITIONS

  // ...and likewise any state variables.

  // IMPORTANT NOTE ABOUT BUFFEREDADC AND STATES:
  // * BCI2000 States, or "state variables", are additional data channels stored alongside signal data,
  //   with a resolution of one value per source signal sample.
  // * A State is much like a source signal channel, but differs from a source signal channel in the
  //   following respects:
  //   + You choose the number of bits used to represent the State's value, up to 64.
  //   + Unlike source signals, States are transmitted through all stages of processing, and their values
  //     may be modified during processing, allowing all parts of the system to store state information in
  //     data files.
  //   + When loading a BCI2000 data file for analysis, States appear separately, with names, which is
  //     typically more useful for trigger information than being just another channel in the signal.
  //   + States may be set synchronously from inside a filter's Process() handler, or asynchronously using
  //     a "bcievent" interface.
  //   + If your amplifier provides a digital input, or another kind of "trigger" information, it makes sense
  //     to store this information in form of one or more States. From a user perspective, it is probably most
  //     useful if physically distinct amplifier input sockets correspond to States, and single bits to certain
  //     lines or values communicated through such a socket.
  //   + If the amplifier API asynchronously reports trigger information through a callback mechanism, you
  //     should register a callback that uses the "bcievent" interface to set states asynchronously.
  //     This example provides a "MyAsyncTriggers" "event state", and a sample callback function.
  //   + If the amplifier API sends trigger information in the same way as it does for signal data, you should
  //     use a "State channel" to represent it in your source module. This example provides a "MySyncTriggers"
  //     state, and writes to it when acquiring data.

  BEGIN_STREAM_DEFINITIONS
    "CreaminoADCSyncTriggers 8 0", // <name> <bit width> <initial value>
  END_STREAM_DEFINITIONS

  BEGIN_EVENT_DEFINITIONS
    "CreaminoADCAsyncTriggers 8 0", // <name> <bit width> <initial value>
  END_EVENT_DEFINITIONS
}

// For asynchronous trigger notification, register this callback with the amplifier API.
static void
TriggerCallback( void* pData, int trigger )
{
  reinterpret_cast<CreaminoADC*>( pData )->OnTrigger( trigger );
}

void
CreaminoADC::OnTrigger( int trigger )
{
  // The following line will create a time-stamped entry into an event queue.
  // Once the next block of data arrives, the queued trigger value will be applied
  // to the CreaminoADCAsyncTriggers state variable at the sample location that
  // corresponds to the time stamp.
  bcievent << "CreaminoADCAsyncTriggers " << trigger;
}

void
CreaminoADC::OnAutoConfig()
{
  // The user has pressed "Set Config" and some parameters may be set to "auto",
  // indicating that they should be set from the current amplifier configuration.
  // In this handler, we behave as if any parameter were set to "auto", and the
  // framework will transparently make sure to preserve user-defined values.

  // Device availability (or connection parameters) may have changed, so close
  // and reopen the connection to the device before proceeding.
  if( mDeviceHandle != AMP_API_INVALID_DEVICE_HANDLE )
    CALL_AMP_API( CloseDevice( mDeviceHandle ) );
  GET_FROM_AMP_API( mDeviceHandle, (void*)(1) );
  if( mDeviceHandle == AMP_API_INVALID_DEVICE_HANDLE )
    throw bcierr << "Could not connect to device";

  int channels = 0;
  GET_FROM_AMP_API( channels, 2 );
  Parameter( "SourceCh" ) = channels; // Set SourceCh in case of "auto"
  // If SourceCh != auto but e.g. SourceChGain != auto, we need to use the actual
  // rather than the auto-set value of SourceCh in order to consistently configure
  // SourceChGain.
  // For this purpose, an ActualParameter() call will retrieve a parameter's current
  // value, no matter whether auto-configured or user-configured.
  channels = ActualParameter( "SourceCh" );
  Parameter( "ChannelNames" )->SetNumValues( channels );
  Parameter( "SourceChGain" )->SetNumValues( channels );
  Parameter( "SourceChOffset" )->SetNumValues( channels );
  for( int i = 0; i < channels; ++i )
  {
    // For EEG amplifiers, channel names should use 10-20 naming if possible.
    const char* name = 0;
    GET_FROM_AMP_API( name, "SomeChannelName" );
    Parameter( "ChannelNames" )( i ) << name << i; // Omit "<< i" if channel names are unique.

    double gainFactor = 1.0;
    GET_FROM_AMP_API( gainFactor, 1.0 );
    Parameter( "SourceChGain" )( i ) << gainFactor << "muV"; // Always provide correct physical unit for documentation and display purposes.
    // For most amplifiers, offset removal is done within the amp hardware or driver. Otherwise, you may set this to a nonzero value.
    Parameter( "SourceChOffset" )( i ) = 0;
  }
  double samplingRate = 1.0;
  GET_FROM_AMP_API( samplingRate, 500 );
  Parameter( "SamplingRate" ) << samplingRate << "Hz";
  int samplesPerPacket = 1;
  GET_FROM_AMP_API( samplesPerPacket, 32 );
  Parameter( "SampleBlockSize" ) = samplesPerPacket;
  
  //cout << "samplingRate = " << samplingRate << '\n';
  //cout << "samplesPerPacket = " << samplesPerPacket << '\n';

}

void
CreaminoADC::OnPreflight( SignalProperties& Output ) const
{
  // The user has pressed "Set Config" and we need to sanity-check everything.
  // For example, check that all necessary parameters and states are accessible:
  State( "CreaminoADCAsyncTriggers" );
  State( "CreaminoADCSyncTriggers" );

  // Also check that the values of any parameters are sane:
  bool ok;
  ok = CALL_AMP_API( VerifySamplingRate( Parameter( "SamplingRate" ).InHertz() ) );
  if( !ok )
    bcierr << "Amplifier does not support a sampling rate of " << Parameter( "SamplingRate" );
  // If there is no verification function provided by the amplifier API, you might also
  // just try to call a configuration function, and check for an error code:
  ok = CALL_AMP_API( SetDataBlockSize( Parameter( "SampleBlockSize" ).ToNumber() ) );
  if( !ok )
    bcierr << "Amplifier does not support a sample block size of " << Parameter( "SampleBlockSize" );
  //
  // Errors issued in this way, during the Preflight phase, still allow the user
  // to open the Config dialog box, fix bad parameters and re-try.  By contrast,
  // errors and C++ exceptions at any other stage (outside Preflight) will make
  // the system stop, such that BCI2000 will need to be relaunched entirely.

  // Internally, signals are always represented by GenericSignal::ValueType == double.
  // Here, you choose the format in which data will be stored, which may be
  // int16, int32, or float32.
  // Typically, you will choose the format that your amplifier natively provides, in
  // order to avoid loss of precision when converting, e.g., int32 to float32.
  SignalType sigType = SignalType::float32;

  int samplesPerBlock = Output.Elements();
  int numberOfSignalChannels = Output.Channels();
  int numberOfSyncStates = 0;
  Output = SignalProperties( numberOfSignalChannels + numberOfSyncStates, samplesPerBlock, sigType );
  // A channel name starting with @ indicates a trigger channel.
  //Output.ChannelLabels()[numberOfSignalChannels] = "@CreaminoADCSyncTriggers";
  // Note that the CreaminoADC instance itself, and its members, are read-only during the
  // Preflight phase---note the "const" at the end of the OnPreflight prototype above.
  // Any methods called by OnPreflight must also be declared as "const" in the same way.
}

void
CreaminoADC::OnInitialize( const SignalProperties& Output )
{
  // The user has pressed "Set Config" and all Preflight checks have passed.
  // The system will now transition into an "Initialized" state.

  // If the user should be able to control amplifier configuration through BCI2000,
  // you will need to use the amplifier's API here in order to apply parameter settings.
  bool ok = CALL_AMP_API( SetSamplingRate( Parameter( "SamplingRate" ).InHertz() ) );
  ok = ok && CALL_AMP_API( SetDataBlockSize( Parameter( "SampleBlockSize" ).ToNumber() ) );
  ok = ok && CALL_AMP_API( SetNumberOfChannels( Parameter( "SourceCh" ).ToNumber() ) );
  // Preflight() checks should ensure that Initialize() never fails due to misconfiguration.
  // Still, unforeseen cases or hardware conditions may lead to failure.
  if( !ok ) 
    bcierr << "Configuration failed unexpectedly";

  // The signal properties can no longer be modified, but the const limitation has gone, so
  // the CreaminoADC instance itself can be modified. Allocate any memory you need
  // store any information you need in private member variables.
  mNumberOfSignalChannels = Parameter( "SourceCh" );
  int bufferBytes = Output.Channels() * Output.Elements() * sizeof( float );
  delete[] mpBuffer;
  mpBuffer = new float[bufferBytes];
  ::memset( mpBuffer, 0, bufferBytes );
  // Buffer allocation may happen in OnStartAcquisition as well, if more appropriate.
  double samplesPerSecond = Parameter( "SamplingRate" );
  double samplesPerBlock  = Parameter( "SampleBlockSize" );
  
  Buffer = (float*)malloc(samplesPerBlock*CH8 * sizeof(float));
  NotchWord = (float*)malloc(samplesPerBlock*CH8 * sizeof(float));
  FilteredWord = (float*)malloc(samplesPerBlock*CH8 * sizeof(float));


  
  CreaminoLibrary::CreaminoLib::SetNotchFilter();
  CreaminoLibrary::CreaminoLib::IIR_05_20_500SPS();
  cout << "samplesPerSecond = " <<dec<< samplesPerSecond << '\n';
  cout << "samplesPerBlock = " <<dec<< samplesPerBlock << '\n';
  cout << "channels = " << dec << Output.Channels() << '\n';
  cout << "Output.Elements()" << dec << Output.Elements() << '\n';

  cout << "Connected = " << Connected << '\n';
}

void
CreaminoADC::OnStartAcquisition()
{
  // This method is called from the acquisition thread once the system is initialized.
  // You should use this space to start up your amplifier using its API.  Any initialization
  // here is done in the acquisition thread, so non-thread-safe APIs should work if initialized here.
  CALL_AMP_API( StartAcquisition );
  CALL_AMP_API( SetTriggerCallback( &TriggerCallback, this ) );
  string COMport = Parameter("COMport");
  int sampleRate = Parameter("SamplingRate");
  int blockSize = Parameter("SampleBlockSize");
  int numberOfChannels = Parameter("SourceCh");
	
  cout << "COMPort = " <<dec<< COMport << '\n';
  cout << "sampleRate = " <<dec<< sampleRate << '\n';
  cout << "blockSize = " <<dec<< blockSize << '\n';
  cout << "Number of Channels = " <<dec<< numberOfChannels << '\n';
	
  byte ADS_samplerate;
	
  switch (sampleRate)
  {
   case 500:
		cout << "Sample Rate 500Hz \n";
		ADS_samplerate = ADS_OUTPUTDATA_500SPS;
	break;
	case 1000:	
		cout << "Sample Rate 1KHz \n";
		ADS_samplerate = ADS_OUTPUTDATA_1KSPS;
		break;
	case 2000:
		cout << "Sample Rate 2KHz \n";
		ADS_samplerate = ADS_OUTPUTDATA_2KSPS;
		break;
	case 4000: 
		cout << "Sample Rate 4KHz \n";
		ADS_samplerate = ADS_OUTPUTDATA_4KSPS;
		break;
	case 8000: 
		cout << "Sample Rate 8KHz \n";
		ADS_samplerate = ADS_OUTPUTDATA_8KSPS;
		break;
	default: 
		cout << "Sample rate not supported. Device setted to 500Hz";
		ADS_samplerate = ADS_OUTPUTDATA_500SPS;
		break;
	}
	

	cout << "OnStartAcquisition()" << '\n';
	if (Connected == false){
		int init = CreaminoLibrary::CreaminoLib::CreaminoStart(COMport, ADS_samplerate, numberOfChannels, 0x05, 0x01, 0x04, 0x10);
		Connected = true;
	}

	bciout << "Connected = " << Connected << '\n';
	Sleep(5000);
}

void
CreaminoADC::DoAcquire( GenericSignal& Output )
{
  // Now we're acquiring a single SampleBlock of data in the acquisition thread, which is stored
  // in an internal buffer until the main thread is ready to process it.

  // Internally, BufferedADC writes this data to a buffer, then locks a mutex and pushes the data
  // into the GenericSignal output in the main thread.  The size of this buffer is parameterized by
  // "SourceBufferSize" declared by BufferedADC, which will be interpreted as follows:
  // * When set to a naked number, the number will be rounded to the nearest integer, and specify
  //   the number of BCI2000 data blocks (cf the SampleBlockSize parameter). The buffer's
  //   duration in time will vary with changes to the SamplingRate and SampleBlockSize parameters.
  // * When set to a number followed with an SI time unit, the buffer's size will be automatically
  //   chosen to match the specified time duration as close as possible. By default, the value is 2s.
  //   SI time units must be appended without white space, and consist of
  //   a valid SI prefix (such as m for milli=1e-3, mu for micro=1e-6, k for kilo=1e3),
  //   followed with a lowercase s (for seconds).

  // Keep in mind that even though we're writing this data from another thread, the main thread
  // cannot continue without data, so be sure this is done in a timely manner
  // or the system will not be able to perform in real-time.
  // For now, we output flat lines:
  
  
	/*
	for (int el = 0; el < Output.Elements(); el++)
		for (int ch = 0; ch < Output.Channels(); ch++) {
			//NotchWord[el*CH32 + ch] = CreaminoLibrary::CreaminoLib::CreaminoNotchFilteredWord(ch, Buffer[el*CH32 + ch]);
			//FilteredWord[el*CH32 + ch] = CreaminoLibrary::CreaminoLib::CreaminoIIR6(ch, NotchWord[el*CH32 + ch]);
			
			//FilteredWord[el*CH32 + ch] = CreaminoLibrary::CreaminoLib::CreaminoIIR6(ch, Buffer[el*CH32 + ch]);
			//if (ch == 0)
				//Output(index[ch], el) = FilteredWord[el*CH32 + ch] / 1e-6;
			//else
				Output(ch, el) = 0.0;
		}
	*/
	/*
	for (int el = 0; el < 32; el++)
		for (int ch = 0; ch < 8; ch++) {
			//NotchWord[el*CH32 + ch] = CreaminoLibrary::CreaminoLib::CreaminoNotchFilteredWord(ch, Buffer[el*CH32 + ch]);
			//FilteredWord[el*CH32 + ch] = CreaminoLibrary::CreaminoLib::CreaminoIIR6(ch, NotchWord[el*CH32 + ch]);

			//FilteredWord[el*CH32 + ch] = CreaminoLibrary::CreaminoLib::CreaminoIIR6(ch, Buffer[el*CH32 + ch]);
			//if (ch == 0)
			//Output(index[ch], el) = FilteredWord[el*CH32 + ch] / 1e-6;
			//else
			Output(ch, el) = 0.0;
		}
	*/
			
  
  // IMPORTANT NOTES ABOUT BUFFERING

  // Ideally, the BCI2000 processing time ("Roundtrip time") is always shorter than the physical
  // duration of a sample block, i.e. every single block of data has been processed before its
  // successor arrives. In that ideal case, buffering makes no difference, because the main thread will
  // always wait for the acquisition thread to deliver the next block of data into the internal
  // buffer, and copy it from there immediately.

  // If, on average, processing takes longer than the physical duration of a sample block, buffering
  // will not help to improve things either, because the buffer will be filled faster than
  // it is being emptied, and thus it will overflow after a certain time. Depending on buffering strategy,
  // buffer overflow may be dealt with by looping (i.e., overwriting data that has not been delivered yet),
  // or by blocking (not accepting any new data before there is space in the buffer).
  // For scientific purposes -- as opposed to, e.g., entertainment applications -- silently omitting
  // data is not an option, so BufferedADC will use the blocking strategy, and deliver all data blocks,
  // but delayed by the physical duration of its buffer.

  // So the _only_ situation in which buffering is actually useful is for cases when processing is not
  // able to keep up with data acquisition for short amounts of time. Typical examples are lengthy 
  // computations that happen from time to time, such as updating a classifier matrix, or initialization
  // work in algorithm implementations that are not factored out into initialization and update operations
  // (otherwise, you would just do lengthy operations inside Initialize()).
  // In such cases, you should set the SourceBufferSize parameter to an estimate of the longest expected
  // delay.


	//cout << "Elementi  " << dec << Output.Elements() << '\n';
	

  // Call the amplifier API's function for synchronous data transfer here.
  int response = CreaminoLibrary::CreaminoLib::CreaminoWaitforData(Buffer, Output.Elements());
  //AMP_API_SYNC_GET_DATA( mpBuffer );
  //const float* pSignalData = reinterpret_cast<float*>( mpBuffer );
  // Copy values from raw buffer into output signal.
  for (int sample = 0; sample < Output.Elements(); sample++)
  {
	for (int ch = 0; ch < mNumberOfSignalChannels; ch++)
    {
      // Check the amplifier's documentation whether data are sent in
      // channel-major or sample-major format.
      Output( ch, sample ) = Buffer[ch + sample * mNumberOfSignalChannels];
	  // When getting garbage, try this instead:
       //Output( ch, el ) = pSignalData[el + ch * Output.Elements()];
    }
  }
  //const char* pSyncTriggerData = 0;
  //GET_FROM_AMP_API( pSyncTriggerData, mpBuffer );
  //for( int sample = 0; sample < Output.Elements(); sample++ )
  //{
    // just an example, unlikely to work for a certain amplifier
    //Output( mNumberOfSignalChannels, sample ) = pSyncTriggerData[sample];
  //}
  
}

void
CreaminoADC::OnStopAcquisition()
{
  // This method is called from the acquisition thread just before it exits.  Use this method
  // to shut down the amplifier API (undoing whatever was done in OnStartAcquisition).
  // Immediately after this returns, the system will go into an un-initialized state and
  // OnHalt will be called in the main thread.
  CALL_AMP_API( StopAcquisition );
  CALL_AMP_API( RemoveTriggerCallback );
  Connected = false;
  int init = CreaminoLibrary::CreaminoLib::CreaminoStop();
  //ThreadUtils::SleepFor(1000);
  bciout << "Connected = " << Connected << '\n';
}

