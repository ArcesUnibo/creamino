////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors:
// Description: CreaminoADC implementation
//
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
//
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
//
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////

/*Matteo Chiesi - University of Bologna*/

#include "PCHIncludes.h"
#pragma hdrstop

#include "CreaminoADC.h"
#include "BCIError.h"
#include <stdio.h>
#include "ThreadUtils.h" // for SleepFor()

using namespace std;

RegisterFilter( CreaminoADC, 1 );
     // Change the location as appropriate, to determine where your filter gets
     // sorted into the chain. By convention:
     //  - filter locations within SignalSource modules begin with "1."
     //  - filter locations within SignalProcessing modules begin with "2."
     //       (NB: SignalProcessing modules must specify this with a Filter() command in their PipeDefinition.cpp file too)
     //  - filter locations within Application modules begin with "3."
static const int index[] = { 29,30,25,26,21,20,17,16,
							 15,11,12, 8, 7, 6, 3, 1,
							  0, 2,10, 4, 5, 9,13,14,
							 19,24,23,18,22,27,28,31 };

static bool Connected = false;


CreaminoADC::CreaminoADC()
{

	// Declare any parameters that the filter needs....
	inCount = 8;
	Buffer = (float*)malloc(inCount*CH32 * sizeof(float));
	NotchWord = (float*)malloc(inCount*CH32 * sizeof(float));
	FilteredWord = (float*)malloc(inCount*CH32 * sizeof(float));
	
 BEGIN_PARAMETER_DEFINITIONS

    "Source:Signal%20Properties int SourceCh= 32 "
       "32 1 % // number of digitized and stored channels",
    "Source:Signal%20Properties int SampleBlockSize= 8 "
       "32 1 % // number of samples transmitted at a time",
    "Source:Signal%20Properties float SamplingRate= 250Hz "
       "125Hz 0.0 % // sample rate",
	"Source:Signal%20Properties:DataIOFilter list ChannelNames= 32 FP1 FP2 AF3 AF4 F7 F3 FZ F4 F8 FC5 FC1 FC2 FC6 T7 C3 CZ C4 T8 CP5 CP1 CP2 CP6 P7 P3 PZ P4 P8 PO7 PO3 PO4 PO8 OZ " //ch33 ch34 ch35 ch36 ch37 ch38 ch39 ch40 ch41 ch42 ch43 ch44 ch45 ch46 ch47 ch48 ch49 ch50 ch51 ch52 ch53 ch54 ch55 ch56 ch57 ch58 ch59 ch60 ch61 ch62 ch63 ch64
	   "// list of channel names",
	"Source:Signal%20Properties:DataIOFilter floatlist SourceChOffset= 32 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 " //0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
	   "% % // Offset for channels in A/D units",
	"Source:Signal%20Properties:DataIOFilter floatlist SourceChGain= 32 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1"
	   "% % // Gain for  each channel",
	 "Source:Signal%20Properties: string COMport= COM4 "
	   "COM4 0 % // COMport for CreaminoAmp",

 END_PARAMETER_DEFINITIONS

  // ...and likewise any state variables.

 BEGIN_STATE_DEFINITIONS

  // IMPORTANT NOTE ABOUT BUFFEREDADC AND STATES:  
  // * BCI2000 States can only be populated "per sample" if they're populated synchronously.
  // * States can only be populated synchronously by calling State( "Y" )( idx ) = X; in OnProcess.
  // * States can also be populated asynchronously by calling bcievent << "Y" << x; in DoAcquire.
  // * Asynchronous state population using bcievent cannot occur per sample, but only per sampleblock.
  // * If a state changes multiple times in a single sample block, data WILL BE LOST using the bcievent interface.
  // * This presents a problem for many amplifier APIs because many of them must be instantiated and run in a single
  //   thread (they are not thread-safe) and access to API state data must be managed carefully if its needed in the main thread.

   "SomeState       8 0 0 0",    // These are just examples. Change them, or remove them.
   "SomeOtherState 16 0 0 0",

 END_STATE_DEFINITIONS

}

CreaminoADC::~CreaminoADC()
{

}

void
CreaminoADC::OnHalt()
{
  // De-allocate any memory reserved in OnInitialize, stop any threads, etc.
  // Good practice is to write the OnHalt() method such that it is safe to call it even *before*
  // OnInitialize, and safe to call it twice (e.g. make sure you do not delete [] pointers that
  // have already been deleted:  set them to NULL after deletion).
  
  // Note that OnStopAcquisition() will be called immediately before this, in the acquisition
  // thread. OnStopAcquisition() is the proper place to do any amplifier-API cleanup.
}

void
CreaminoADC::OnPreflight( SignalProperties& Output ) const
{
	
	if( (double)Parameter( "SamplingRate" ) == 0.0 )
		bcierr << "SamplingRate cannot be zero" << endl;
	int sampleRate = Parameter( "SamplingRate" );
	int blockSize = Parameter( "SampleBlockSize" );
  
	int numberOfChannels = Parameter( "SourceCh" );
	int samplesPerBlock  = Parameter( "SampleBlockSize" );
	SignalType sigType = SignalType::float32;  // could also parameterize this
	Output = SignalProperties( numberOfChannels, samplesPerBlock, sigType );
	
  //
  // Note that the CreaminoADC instance itself, and its members, are read-only during the
  // Preflight phase---note the "const" at the end of the OnPreflight prototype above.
  // Any methods called by OnPreflight must also be declared as "const" in the same way.
}

void
CreaminoADC::OnInitialize( const SignalProperties& Output )
{
  // The user has pressed "Set Config" and all Preflight checks have passed.
  // The system will now transition into an "Initialized" state.
  // The signal properties can no longer be modified, but the const limitation has gone, so
  // the CreaminoADC instance itself can be modified. Allocate any memory you need
  // store any information you need in private member variables.

  // As long as the system is in an initialized state, the module should be pushing data from
  // the amplifier through the system.  Initialized != Running, however.  The system can call
  // StartRun and StopRun many times while remaining initialized.  The system will only become
  // uninitialized again once OnHalt is called.

  // Don't bother with any amplifier-API stuff here, however: instead, do this in
  // OnStartAcquisition() which will be called in the acquisition thread immediately after this
  // method finishes.

  double samplesPerSecond = Parameter( "SamplingRate" );
  double samplesPerBlock  = Parameter( "SampleBlockSize" );
  mMsecPerBlock = 1000.0 * samplesPerBlock / samplesPerSecond;

  mLastTime = PrecisionTime::Now();
  
  CreaminoLibrary::CreaminoLib::SetNotchFilter();
  CreaminoLibrary::CreaminoLib::IIR_05_20_250SPS();

  bciout << "Connected = " << Connected << '\n';

}

void 
CreaminoADC::OnStartAcquisition()
{
  // This method is called from the acquisition thread once the system is initialized.
  // You should use this space to start up your amplifier using its API.  Any initialization
  // here is done in the acquisition thread, so non-thread-safe APIs should work if initialized here.
	//int init = CreaminoLibrary::CreaminoLib::CreaminoStart("COM4", ADS_OUTPUTDATA_125SPS, CH32);
	//bciout << "Configurazione = " << init << '\n';

	string COMport = Parameter("COMport");
	int sampleRate = Parameter("SamplingRate");
	int blockSize = Parameter("SampleBlockSize");
	int numberOfChannels = Parameter("SourceCh");
	

	bciout << "COMPort = " << COMport << '\n';
	bciout << "Number of Channels = " << numberOfChannels << '\n';
	
	byte ADS_samplerate;
	
	switch (sampleRate)
	{
	case 125: bciout << "Sample Rate 125Hz \n";
		ADS_samplerate = ADS_OUTPUTDATA_125SPS;
		break;
	case 250: bciout << "Sample Rate 250Hz \n";
		ADS_samplerate = ADS_OUTPUTDATA_250SPS;
		break;
	case 500: bciout << "Sample Rate 500Hz \n";
		ADS_samplerate = ADS_OUTPUTDATA_500SPS;
		break;
	case 1000: bciout << "Sample Rate 1KHz \n";
		ADS_samplerate = ADS_OUTPUTDATA_1KSPS;
		break;
	case 2000: bciout << "Sample Rate 2KHz \n";
		ADS_samplerate = ADS_OUTPUTDATA_2KSPS;
		break;
	case 4000: bciout << "Sample Rate 4KHz \n";
		ADS_samplerate = ADS_OUTPUTDATA_4KSPS;
		break;
	case 8000: bciout << "Sample Rate 8KHz \n";
		ADS_samplerate = ADS_OUTPUTDATA_8KSPS;
		break;
	default: bciout << "Sample rate not supported. Device setted to 250Hz";
		ADS_samplerate = ADS_OUTPUTDATA_250SPS;
		break;
	}
	

	bciout << "OnStartAcquisition()" << '\n';
	//int init = CreaminoLibrary::CreaminoLib::CreaminoStart(COMport, ADS_samplerate, numberOfChannels);
	if (Connected == false){
		//int init = CreaminoLibrary::CreaminoLib::CreaminoStart("COM4", ADS_OUTPUTDATA_250SPS, CH32);
		int init = CreaminoLibrary::CreaminoLib::CreaminoStart(COMport, ADS_samplerate, numberOfChannels);
		Connected = true;
	}

	bciout << "Connected = " << Connected << '\n';
	//OSThread::SleepFor(2000);
}

void
CreaminoADC::DoAcquire( GenericSignal& Output )
{
  // Now we're acquiring a single SampleBlock of data in the acquisition thread.
  // Keep in mind that even though we're writing this data from another thread, the main thread
  // will block until we've written the SampleBlock, so be sure this is done in a timely manner 
  // or the system will not be able to perform in real-time.

  // Internally, BufferedADC writes this data to a buffer, then locks a mutex and pushes the data
  // into the GenericSignal output in the main thread.  The size of this buffer (in units of time)
  // is parameterized by "SourceBufferSize" declared by BufferedADC.  Be careful not to overflow!

  // For now, we output flat lines:
	//bciout << "Output Channels " << Output.Channels() << '\n';
	//bciout << "Output Elements " << Output.Elements() << '\n';

	//mTimeBefore = PrecisionTime::Now();
	int response = CreaminoLibrary::CreaminoLib::CreaminoWaitforData(Buffer, inCount);

	//mTimeAfter = PrecisionTime::Now();

	//bciout << "Time Creamino = " << PrecisionTime::UnsignedDiff(mTimeAfter, mTimeBefore) << '\n';


	for (int el = 0; el < Output.Elements(); el++)
		for (int ch = 0; ch < Output.Channels(); ch++) {
			//NotchWord[el*CH32 + ch] = CreaminoLibrary::CreaminoLib::CreaminoNotchFilteredWord(ch, Buffer[el*CH32 + ch]);
			//FilteredWord[el*CH32 + ch] = CreaminoLibrary::CreaminoLib::CreaminoIIR6(ch, NotchWord[el*CH32 + ch]);
			
			FilteredWord[el*CH32 + ch] = CreaminoLibrary::CreaminoLib::CreaminoIIR6(ch, Buffer[el*CH32 + ch]);
			//if (ch == 0)
				Output(index[ch], el) = FilteredWord[el*CH32 + ch] / 1e-6;
			//else
				//Output(ch, el) = 0.0;
		}

			
			
  // Here is a wait loop to ensure that we do not deliver the signal faster than real-time
  // (In your final implementation, you should remove this: the hardware will play this role then.)
  //while( PrecisionTime::UnsignedDiff( PrecisionTime::Now(), mLastTime ) < mMsecPerBlock ) ThreadUtils::SleepFor(1);
  mLastTime = PrecisionTime::Now();

}

void
CreaminoADC::StartRun()
{
  // The user has just pressed "Start" (or "Resume")
  bciout << "Hello World!" << endl;

  mLastTime = PrecisionTime::Now();
  //int init = CreaminoLibrary::CreaminoLib::CreaminoStart("COM4", ADS_OUTPUTDATA_125SPS, CH32);

  bciout << "Connected = " << Connected << '\n';
}

void
CreaminoADC::StopRun()
{
  // The Running state has been set to 0, either because the user has pressed "Suspend",
  // because the run has reached its natural end.
  bciout << "Goodbye World." << endl;
  //int init = CreaminoLibrary::CreaminoLib::CreaminoStop();
  bciout << "Connected = " << Connected << '\n';
}

void
CreaminoADC::OnStopAcquisition()
{
	//int init = CreaminoLibrary::CreaminoLib::CreaminoStop();
  // This method is called from the acquisition thread just before it exits.  Use this method
  // to shut down the amplifier API (undoing whatever was done in OnStartAcquisition).
  // Immediately after this returns, the system will go into an un-initialized state and
  // OnHalt will be called in the main thread: (there you can undo whatever was done in
  // OnInitialize). 

  // This method will always be called before OnHalt is called.
	Connected = false;
	int init = CreaminoLibrary::CreaminoLib::CreaminoStop();
	ThreadUtils::SleepFor(1000);
	bciout << "Connected = " << Connected << '\n';
}

