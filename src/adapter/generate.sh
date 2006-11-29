#!/bin/bash

# make and execute input file generator
qmake-qt4 default-adapter-input-file-generator.pro
make > /dev/null
echo "++++++++++ default-adapter-input-file-generator"
./default-adapter-input-file-generator
echo "----------------------------------------"

touch DefaultAdapterGenerator.cpp

# make and execute adapter generator
qmake-qt4 default-adapter-generator.pro
make clean > /dev/null
make > /dev/null
echo "++++++++++ default-adapter-generator"
./default-adapter-generator
echo "----------------------------------------"

## show the generated adapters ;-)
#echo "++++++++++ ls -1"
#ls -1 *Adapter.h
#echo "----------------------------------------"

# compile the static adapter library
echo "++++++++++ adapter-lib"
qmake-qt4 default-adapter-lib.pro
make clean > /dev/null
make
echo "----------------------------------------"

# touch the factory implementation file
# (-> compile new adatpers into Thekla library)
touch QtAdapterFactory.cpp
