#!/bin/bash
dir=MakeFileProject

mkdir -p $dir
rm -r $dir/*
cd $dir
cmake -DCMAKE_BUILD_TYPE="Debug" -G "Unix Makefiles" ..
