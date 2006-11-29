#!/bin/bash

# make and execute input file generator
echo "++++++++++ default-adapter-input-file-generator"
qmake-qt4 default-adapter-input-file-generator.pro
make clean
echo "----------------------------------------"

# make and execute adapter generator
echo "++++++++++ default-adapter-generator"
qmake-qt4 default-adapter-generator.pro
make clean
echo "----------------------------------------"

# compile the adapter lib
#echo "++++++++++ default-adapter-test"
#qmake-qt4 adapter.pro
#make clean
#echo "----------------------------------------"
