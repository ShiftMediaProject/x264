
This is a small list of steps in order to build x264 into a msvc DLL and lib file.

The project contains Release and Debug builds for static lib files (Debug/Release)
  as well as dynamic shared dll files (DebugDLL/ReleaseDLL).
Choose whichever project configuration meets your requirements.

*** Building with YASM ***

In order to build x264 using msvc you must first download and install YASM.
YASM is required to compile all x264 assembly files.

1) Download yasm for Visual Studio from here:
http://yasm.tortall.net/Download.html

Currently only up to VS2010 is supported on the web page so just download that.

2) Follow the instructions found within the downloaded archive for installing YASM
	Note: With newer version of VS the BuildCustomization path should be the version specific to the VS version you are using.
		so for instance the path for Visual Studio 2013 is:
		C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V120\BuildCustomizations
		
3) Currently there is a bug in Visual Studio 2013 so in order to make the build customizations work you must edit a file
	a) Open vsyasm.props that you just extracted
	b) Replace the 1 occurrence of [Input] with "%(FullPath)"  (make sure to include the "s)
	
4) ???

5) Profit


*** Generating OpenCl header file ***

The build system by default will generate the OpenCL required header file 'oclobj.h'.
This should already be generated. If it for some reason is not or needs to be regenerated then perl is required.

1) Download Strawberry Perl for windows from here:
http://strawberryperl.com/

2) Install Perl using the provided installer (you may have to restart to allow Visual Studio to pick up the modifications to PATH)

3) Build project as normal (If regenerating oclobj.h then simply delete the old one then rebuild). 