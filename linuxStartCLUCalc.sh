#!/bin/bash

dir=$(dirname $0)
sudo wine $(find "$dir"/wine_clucalc/ -name 'CLUCalc.exe')

