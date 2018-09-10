#!/bin/bash
dir=XCodeProject
pythonDir="../PythonSolutions"

mkdir -p $dir
rm -r $dir/*
cd $dir
cmake -DCOMPILE_WITH_PYTHON_SUPPORT=ON -G "Xcode" ..
make -j4

# move all python related stuff into 'PythonVersion'
mkdir -p $pythonDir
cp ../lib/Debug/_cf.so swig/cf.py ../python/computerGeometry_example.py $pythonDir
cp -r ../geometry_files/ $pythonDir

echo To start the examples:
echo "  - Change directory into 'PythonSolution'"
echo "  - run 'python ./computerGeometry_example.py'"

