
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
        and the path for Visual Studio 2015 would be:
        C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V140\BuildCustomizations

3) In order to use version 1.3.0 of vsyasm you will also have to fix a error in the distributed build customizations
    a) Open vsyasm.props
    b) Replace the 1 occurrence of $(Platform) with win$(PlatformArchitecture)