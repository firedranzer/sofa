#!/bin/bash

echo debut

python setupBasic.py build_ext --inplace

echo Premiere etape

python generate_expression.py

echo Seconde etape

python setupLitteral.py build_ext --inplace

echo Trosieme etape

python script2.py

echo Fin
