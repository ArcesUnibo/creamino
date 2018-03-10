1) Download and Compile OpenVIbE on Windows

Download a stable source code of OpenViBE from here: http://openvibe.inria.fr/downloads/

Before to build OpenVibe download and install Phyton3 from here: https://www.python.org/downloads/ 
Once you have installed python open a cmd prompt as administrator and install jinja2 in this way:
type "c:\python3X\Scripts\pip.exe install jinja2" (Check if the installation path of python is correct, 
otherwise modify the path)

Use the same cmd prompt and go inside the OpenViBE folder than run install_dependencies.cmd

Be sure to have gtk library installed. If not Download MSYS2 installer from here: http://www.msys2.org/
Download and install the version that matchs your platform. Open the MSYS2 shell and type: pacman -S mingw-w64-x86_64-gtk3

Be sure to have Microsoft Visual Studio 2013 installed.
Once the dependencies have been installed you can run the build.cmd --vsbuild-all if you intend to use Visual Studio as we did.

For further details follow the instructions reported here : http://openvibe.inria.fr/build-instructions/


2) Put the acquisition driver inside OpenVIbE

Put the source files that you can find in C:/WORK/creamino/SW/OpenVibeCreamino/creamino/src
in your local repository:
[openvibe-repository]/contrib/plugins/server-drivers/creamino/src/ 

Put the glade interface that you can find in C:/WORK/creamino/SW/OpenVibeCreamino/creamino/share interface-Creamino.glade in your local repository:
[openvibe-repository]/contrib/plugins/server-drivers/[my-device]/share/

Don't forget to declare your driver to the Acquisition Server application.
This happens in "./contrib/common/contribAcquisitionServer.inl". There you 
will find examples of such declarations. You also need to register the source
files to CMake, modifying the corresponding "contribAcquisitionServer.cmake".

For more information about building a new driver and filling your skeleton, please read the official tutorial:
http://openvibe.inria.fr/tutorial-creating-a-new-driver-for-the-acquisition-server/

Run once again the command build.cmd --vsbuild-all

Put the file Calibration.csv inside the folder [openvibe-repository]/dist/extras/Release

You are ready to use Creamino inside OpenViBE!