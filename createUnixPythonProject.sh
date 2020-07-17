#!/bin/bash
dir=MakeFileProject
pythonDir="../PythonSolutions"
pythonLibDir="$pythonDir/libcfcg"

mkdir -p $dir
rm -r $dir/*
cd $dir
cmake -DCMAKE_BUILD_TYPE=Release -DCOMPILE_WITH_PYTHON_SUPPORT=ON -G "Unix Makefiles" ..
make -j4

# copy all python related stuff into 'PythonVersion'
mkdir -p $pythonDir $pythonLibDir

cp ../python/*.py $pythonDir
mv $pythonDir/helper.py $pythonLibDir

cp ../lib/Release/_cf.so swig/cf.py $pythonLibDir
cp -r ../geometry_files ../chaos_files $pythonDir

echo To start the examples:
echo "  - Change directory into 'PythonSolution'"
echo "  - run 'python ./computerGeometry_example.py'"

