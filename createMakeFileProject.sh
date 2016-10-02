#!/bin/bash
dir=MakeFileProject
mkdir -p $dir
rm -r $dir/*
cd $dir
cmake -G "Unix Makefiles" ..

