////////////////////////////////////////////////////////////////////////////////
// Authors: Matteo Chiesi - University of Bologna
// Description: CreaminoADC header
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_CREAMINOADC_H  // makes sure this header is not included more than once
#define INCLUDED_CREAMINOADC_H

#include "BufferedADC.h"
#include "PrecisionTime.h"
#include <Windows.h>
#include "CreaminoLib.h"

class CreaminoADC : public GenericADC
{
 public:
  CreaminoADC();
  ~CreaminoADC();

  void Publish();
  void AutoConfig(const SignalProperties&);
  void Preflight(const SignalProperties&, SignalProperties&) const;
  void Initialize(const SignalProperties&, const SignalProperties&);
  void StartRun();
  void Process(const GenericSignal&, GenericSignal&);
  void Halt();

  void OnTrigger( int ); // for asynchronous triggers only

 private:
  // Use this space to declare any CreaminoADC-specific methods and member variables you'll need
  void* mDeviceHandle;
  float* mpBuffer;
  int mNumberOfSignalChannels;
  int inCount; 
  float* Buffer;
  float* NotchWord;
  float* FilteredWord;
};

#endif // INCLUDED_CREAMINOADC_H
