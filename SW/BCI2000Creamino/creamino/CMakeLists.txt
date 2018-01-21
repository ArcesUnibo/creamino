###########################################################################
## Authors:
## Description: Build information for the creamino source module
###########################################################################

# Use the bci2000_include macro if you need to link with frameworks from /src/extlib:
# bci2000_include( MATH )

bci2000_add_signal_source_module(
  creamino
  creaminoADC.cpp
  CreaminoLib.cpp
  SerialLib.cpp
)
