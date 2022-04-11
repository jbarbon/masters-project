#!/bin/bash

#Script wrapper made by João Barbon.

#This script compiles and runs the surface projections in a screen terminal

#Check README.md for more details

#Checking number of arguments
if [[ $# -ne 3 ]]; then
    echo "Invalid number of arguments." 
    echo "Correct call: ./run-surface.sh [EVOPATHs.txt] [Surface.dat path] [OUTPUTNAME.root]"
    exit
fi

if [[ $# -eq 3 ]];then

    EVOPATH=${1}
    SURF=${2}
    OUTNAME=${3}

    #Compile the main code
    bash ./Vorticity_Base/Scripts/Compile_Main.sh

    #Run the code in a screen terminal
    screen -L -Logfile ${OUTNAME%.*}.log -S ${OUTNAME%.*} -dm ./Vorticity_Base/Scripts/doSurface.sh ${PWD}/${EVOPATH} ${SURF} ${OUTNAME}

fi
