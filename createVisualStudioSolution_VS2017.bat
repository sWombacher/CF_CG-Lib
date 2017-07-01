@echo off
rd /s /q ".\VisualStudio" 2>nul
mkdir VisualStudio
cd VisualStudio
echo Create Solution, this may take a moment...
..\cmake\bin\cmake.exe -G "Visual Studio 15 Win64" -D3RD_PARTY_DIR="3rdparty/VS2017" ..

echo Finished, press enter to continue

PAUSE