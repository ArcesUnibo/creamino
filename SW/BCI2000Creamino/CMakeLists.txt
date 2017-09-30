###########################################################################
## $Id:  $
## Authors:
## Description: Build information for the Creamino source module
##              
##              
###########################################################################

# Set the executable name
SET( EXECUTABLE_NAME Creamino )

# Set the project specific sources
SET( SRC_PROJECT
  CreaminoADC.cpp
)

SET( HDR_PROJECT
  # perhaps you will need to add the manufacturer's SDK headers here...
  CreaminoADC.h
)

INCLUDE_DIRECTORIES(
   # ...and if the headers are not in the current directory, specify the directories where they can be found
)

LINK_DIRECTORIES(
   # ...the same goes for directories where third-party libraries are found (see below) 
)

# Use the BCI2000_INCLUDE macro if you need to link with frameworks from /src/extlib:
# BCI2000_INCLUDE( "MATH" )

# Add the source module to the Makefile or compiler project file:
BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  "${EXECUTABLE_NAME}" 
  "${SRC_PROJECT}" "${HDR_PROJECT}" 
)

# finally, specify any third-party libraries that need to be linked:
# 
# TARGET_LINK_LIBRARIES( ${EXECUTABLE_NAME} TheManufacturersSDK )

