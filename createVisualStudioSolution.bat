@echo off
rd /s /q ".\VisualStudio"
mkdir VisualStudio
cd VisualStudio
..\cmake\bin\cmake.exe -G "Visual Studio 14 Win64" ..