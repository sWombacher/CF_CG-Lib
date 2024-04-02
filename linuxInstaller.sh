#!/bin/bash
wget https://github.com/sWombacher/CF_CG-Lib/archive/refs/heads/master.zip
unzip master.zip
rm master.zip
 
cd CF_CG-Lib-master
./linux_installDependencies.sh
./createUnixPythonProject.sh
 
cd PythonSolutions
python3 windowRasterized_example.py

unzip 3rdparty/wine_clucalc.zip
 
echo ""
echo ""
echo "Everything set up, you can find the exampls in ~/CF_CG-Lib-master/PythonSolutions"
echo "You can create new files new files for execution in the same directory"
echo "Most importantly: Have fun! :)"
