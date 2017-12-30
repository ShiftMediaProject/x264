
This is a small list of steps in order to build x264 into a msvc DLL and lib file.

The project contains Release and Debug builds for static lib files (Debug/Release)
  as well as dynamic shared dll files (DebugDLL/ReleaseDLL).
Choose whichever project configuration meets your requirements.

*** Building with NASM ***

In order to build x264 using msvc you must first download and install NASM.
NASM is required to compile all x264 assembly files.

1) Visual Studio NASM integration can be downloaded from https://github.com/ShiftMediaProject/VSNASM/releases/latest

2) Once downloaded simply follow the install instructions included in the download.