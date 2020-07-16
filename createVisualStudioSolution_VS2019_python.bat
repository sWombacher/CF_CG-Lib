@echo off
rd /s /q ".\VisualStudio" 2>nul
mkdir VisualStudio_python
cd VisualStudio_python
echo Create Solution, this may take a moment...
..\cmake\bin\cmake.exe -DCMAKE_BUILD_TYPE=RELEASE -DCOMPILE_WITH_PYTHON_SUPPORT=ON -G "Visual Studio 16 2019" -D3RD_PARTY_DIR="3rdparty/VS2019" ..
..\cmake\bin\cmake.exe --build . --config Release

mkdir ..\PythonSolutions
mkdir ..\PythonSolutions\cf
mkdir ..\PythonSolutions\chaos_files
mkdir ..\PythonSolutions\geometry_files

copy ..\lib\Release\_cf.pyd ..\PythonSolutions\cf
copy ..\bin\Release\*.dll ..\PythonSolutions\cf
copy swig\cf.py ..\PythonSolutions\cf

copy ..\python\*.py ..\PythonSolutions
copy ..\python\*.pdf ..\PythonSolutions

move ..\PythonSolutions\helper.py ..\PythonSolutions\cf

xcopy ..\geometry_files ..\PythonSolutions\geometry_files
xcopy ..\chaos_files ..\PythonSolutions\chaos_files

echo To start the examples:
echo   - Change directory into 'PythonSolutions'
echo   - run 'python ./computerGeometry_example.py'

echo Finished, press enter to continue

PAUSE
