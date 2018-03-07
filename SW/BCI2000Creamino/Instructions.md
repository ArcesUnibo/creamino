1) Download and Compile BCI2000 on Windows

Detailed instructions can be found here: https://www.bci2000.org/mediawiki/index.php/Programming_Reference:BCI2000_Source_Code
The BCI2000 source code is managed with the help of Subversion, an open-source cross-platform versioning tool widely used in various kinds of projects.
We suggest you to use TortoiseSVN, a comfortable-to-use SVN client integrated into the Explorer shell
Tortoise SVN can be downloaded here https://tortoisesvn.net/downloads.html
Access to the BCI2000 source code requires a BCI2000 user account, and agreeing to the GNU Public License (GPL). 
You may create a user account online, as described on the "Creating a User Account" page. https://www.bci2000.org/mediawiki/index.php/User_Accounts

If TortoiseSVN gives you an error like this "Error running context: An error occurred during authentication" during the download follow these steps:

1. Start | Control Panel | Credential Manager, 

2. Click 'Add a Windows Credential', 

3. As 'Internet or network address' enter the FQDN of VisualSVN Server's machine, 

4. As 'Username' enter the <DOMAIN>\<username> of user account that exists in domain and has access rights to VisualSVN Server, 

Complete the password field and click OK. 
Verify that you can authenticate to VisualSVN Server under the selected user account after completing the above steps. 

In order to Compile BCI2000 follow the instruction reported here:
https://www.bci2000.org/mediawiki/index.php/Programming_Reference:Build_System

2) Put the acquisition driver inside BCI2000

In order to put the acquisition driver inside BCI2000 run the executable NewBCI2000Module that you can find inside the folder [BCI2000-repository]\BCI2000\build
and follow these instructions:

Enter module type [default is 2]: 1
Enter module name: Creamino
Enter parent directory [recommended default is "src/custom" ]: src/custom

A new SignalSource project has been created at src/custom/creamino
-- created src/custom/creamino/CMakeLists.txt
Adding filter CreaminoADC to module src/custom/creamino/
-- created src/custom/creamino/CreaminoADC.cpp
-- created src/custom/creamino/CreaminoADC.h
-- added CreaminoADC.cpp to src/custom/creamino/CMakeLists.txt

Put the source files that you can find in C:/WORK/creamino/SW/BCI2000/creamino/src
in your local repository:
[BCI2000-repository]/src/custom/creamino/ 

Put the Calibration file (Calibration.csv) that you can find in C:/WORK/creamino/SW/BCI2000/creamino/src
in your local repository:
[BCI2000-repository]/prog 


NOTE: Run CMake again to ensure that this module is included in the build.

To expand the new module, edit the following files:
* src/custom/creamino/Creamino/CMakeLists.txt	
* src/custom/creamino/Creamino/CreaminoADC.cpp
* src/custom/creamino/Creamino/CreaminoADC.h






You are ready to use Creamino inside BCI2000!