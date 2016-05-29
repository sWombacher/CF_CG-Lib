@echo off
mkdir VisualStudio
cd VisualStudio
..\cmake\bin\cmake.exe -G "Visual Studio 14 Win64" ..

set SCRIPT="%TEMP%\%RANDOM%-%RANDOM%-%RANDOM%-%RANDOM%.vbs"
 
echo Set oWS = WScript.CreateObject("WScript.Shell") >> %SCRIPT%
echo sLinkFile = ".\Project.lnk" >> %SCRIPT%
echo Set oLink = oWS.CreateShortcut(sLinkFile) >> %SCRIPT%
echo oLink.TargetPath = ".\VisualStudio\ALL_BUILD.vcxproj" >> %SCRIPT%
echo oLink.WorkingDirectory =".\VisualStudio\" >> %SCRIPT%
echo oLink.Save >> %SCRIPT%
 
cscript /nologo %SCRIPT%
 
del %SCRIPT%