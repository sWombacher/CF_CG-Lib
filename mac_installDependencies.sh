#!/bin/bash

# install package manager
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"

brew install homebrew/science/opencv
brew install glm
brew install cmake
brew install clang
