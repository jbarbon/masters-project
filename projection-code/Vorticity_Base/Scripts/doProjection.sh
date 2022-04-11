#!/bin/bash


#Important paths
CURRENTPATH=${PWD}
PROJ_PATH=Vorticity_Base/doMain/Projection_Class



if [[ $# -eq 3 ]]; then
    
    #Arguments
    EVOPATH=$1
    BGPATH=$2
    OUTNAME=$3

    #Start running code
    #Run projection
    cd ${CURRENTPATH}/Main_Results/Projections && ./doProjection.exe ${EVOPATH} ${BGPATH} Proj_${OUTNAME}
    echo "Projection done!"

fi

if [[ $# -eq 2 ]]; then
    
    #Arguments
    EVOPATH=$1
    OUTNAME=$2

    #Start running code
    #Run projection
    cd ${CURRENTPATH}/Main_Results/Projections && ./doProjection.exe ${EVOPATH} Proj_${OUTNAME}
    echo "Projection done!"

fi

