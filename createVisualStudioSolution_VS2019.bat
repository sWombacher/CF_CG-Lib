@echo off
rd /s /q ".\VisualStudio" 2>nul
mkdir VisualStudio
cd VisualStudio
echo Create Solution, this may take a moment...
..\cmake\bin\cmake.exe -DCOMPILE_WITH_PYTHON_SUPPORT=OFF -G "Visual Studio 16 2019" -D3RD_PARTY_DIR="3rdparty/VS2019" ..

echo Finished, press enter to continue

PAUSE