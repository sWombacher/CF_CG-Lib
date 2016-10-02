#!/bin/bash
dir=XCodeProject
mkdir -p $dir
rm -r $dir/*
cd $dir
cmake -G "Xcode" ..

