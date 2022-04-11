#!/bin/bash

#Script wrapper made by João Barbon.

#This script compiles and runs main C++ code in a screen terminal.

#The main C++ code creates a 2D and 1D projection of the 3D evolution.root file
#Some of the quantities projected are: Energy, velocity, Temperature, Vorticity.
#Check README.md for more details

#Checking number of arguments
if [[ ($# -lt  2) || ($# -gt 3) ]]; then
    echo "Invalid number of arguments." 
    echo "Correct call with evolution and background files: ./run-projection.sh [EVOPATH.root] [BGPATH.root] [OUTPUTNAME.root]"
    echo "Correct call with evolution but without background files: ./run-projection.sh [EVOPATH.root] [OUTPUTNAME.root]"
    exit
fi
 
if [[ $# -eq 3 ]];then
    
    EVOPATH=${1}
    BGPATH=${2}
    OUTNAME=${3}

    bash ./Vorticity_Base/Scripts/Compile_Main.sh
    screen -L -Logfile ${OUTNAME%.*}.log -S ${OUTNAME%.*} -dm ./Vorticity_Base/Scripts/doProjection.sh ${PWD}/${EVOPATH} ${PWD}/${BGPATH} ${OUTNAME}
fi

if [[ $# -eq 2 ]];then
    
    EVOPATH=${1}
    OUTNAME=${3}

    bash ./Vorticity_Base/Scripts/Compile_Main.sh
    screen -L -Logfile ${OUTNAME%.*}.log -S ${OUTNAME%.*} -dm ./Vorticity_Base/Scripts/doProjection.sh ${PWD}/${EVOPATH} ${OUTNAME}
fi
