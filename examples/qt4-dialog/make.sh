#!/bin/bash
qmake-qt4 input.pro && make clean && make
qmake-qt4 output.pro && make clean && make
