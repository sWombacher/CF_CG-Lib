@echo off
rd /s /q ".\VisualStudio" 2>nul
mkdir VisualStudio
cd VisualStudio
echo Create Solution, this may take a moment...
..\cmake\bin\cmake.exe -G "Visual Studio 14 Win64" ..

echo
echo Finished, press enter to continue
echo
PAUSE