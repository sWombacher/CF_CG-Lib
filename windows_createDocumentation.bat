@echo off

echo Create Documentation, this may take a moment...

.\3rdparty\tools\doxygen\doxygen.exe .\doxygen.conf

echo Documentation creation finished
echo To open the html verison go to 'Documentation' and open 'index.html'
echo Finished, press enter to continue

PAUSE
