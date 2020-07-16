#!/bin/bash
dir=MakeFileProject
pythonDir="../PythonSolutions"

mkdir -p $dir
rm -r $dir/*
cd $dir
cmake -DCMAKE_BUILD_TYPE=RELEASE -DCOMPILE_WITH_PYTHON_SUPPORT=ON -G "Unix Makefiles" ..
make -j4

# copy all python related stuff into 'PythonVersion'
mkdir -p $pythonDir $pythonDir/cf
cp ../lib/Release/_cf.so swig/cf.py $pythonDir/cf
cp -r ../geometry_files ../chaos_files $pythonDir
cp ../python/*.py $pythonDir/cf
mv $pythonDir/helper.py $pythonDir/cf

echo To start the examples:
echo "  - Change directory into 'PythonSolution'"
echo "  - run 'python ./computerGeometry_example.py'"

