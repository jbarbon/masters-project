#!/bin/bash

#Script made by João Barbon.

#This script runs compiles and runs in a screen terminal the main C++ code.

#The main C++ code creates a 2D and 1D projection of the 3D evolution.root file
#Some of the quantities projected are: Energy, velocity, Temperature, Vorticity.

# It also reads the surface

#Checking number of arguments
if [[ ($# -lt  3) || ($# -gt 4) ]]; then
    echo "Invalid number of arguments." 
    echo "Correct call with evolution and background files: ./Run-Screen.sh [EVOPATH.root] [BGPATH.root] [Surface.dat] [OUTPUTNAME.root]"
    echo "Correct call with evolution but without background files: ./Run-Screen.sh [EVOPATH.root] [Surface.dat] [OUTPUTNAME.root]"
    exit
fi

if [[ $# -eq 4 ]];then
    
    EVOPATH=${1}
    BGPATH=${2}
    SURF=${3}
    OUTNAME=${4}

    bash ./Vorticity_Base/Scripts/Compile_Main.sh
    screen -L -Logfile ${OUTNAME%.*}.log -S ${OUTNAME%.*} -dm ./Vorticity_Base/Scripts/doMain.sh ${PWD}/${EVOPATH} ${PWD}/${BGPATH} ${SURF} ${OUTNAME}
fi

if [[ $# -eq 3 ]];then
    
    EVOPATH=${1}
    SURF=${2}
    OUTNAME=${3}

    bash ./Vorticity_Base/Scripts/Compile_Main.sh
    screen -L -Logfile ${OUTNAME%.*}.log -S ${OUTNAME%.*} -dm ./Vorticity_Base/Scripts/doMain.sh ${PWD}/${EVOPATH} ${SURF} ${OUTNAME}
fi
