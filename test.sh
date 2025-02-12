#!/bin/bash

gcc main.c -o main.o -std=c99 -Wall -pedantic || { echo "Compilation failed"; exit 1; }

mkdir -p ./tests

./main.o 1 0.3 0.3 0.6 && python3 plot.py && mv ./motion_plot.png ./tests/test01.png
./main.o 0.4 0.3 0.3 0.6 && python3 plot.py && mv ./motion_plot.png ./tests/test02.png
./main.o 0.4 0.1 0.3 0.6 && python3 plot.py && mv ./motion_plot.png ./tests/test03.png
./main.o 0.01 0.166 0.5 0.1 && python3 plot.py && mv ./motion_plot.png ./tests/test04.png
