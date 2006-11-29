#!/bin/bash

# make and execute input file generator
echo "++++++++++ default-adapter-input-file-generator"
qmake-qt4 default-adapter-input-file-generator.pro
make
echo "----------------------------------------"

# make and execute adapter generator
echo "++++++++++ default-adapter-generator"
qmake-qt4 default-adapter-generator.pro
make
echo "----------------------------------------"

## compile the adapter library
#echo "++++++++++ adapter-lib"
#qmake-qt4 adapter.pro
#make
#echo "----------------------------------------"
