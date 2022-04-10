#!/bin/bash

#Checking number of arguments
#if [[ ($# -lt  3) || ( $# -gt 4) ]]; then
#    echo "Invalid number of arguments." 
#    echo "Correct call: with evolution and BG files ./doMain.sh [EVOPATH.root] [BGPATH.root] [Surface.dat] [OUTPUTNAME.root]"
#    echo "Correct call: with evolution./doMain.sh [EVOPATH.root] [Surface.dat] [OUTPUTNAME.root]"
#    exit
#fi

#Important paths
CURRENTPATH=${PWD}
PROJ_PATH=Vorticity_Base/doMain/Projection_Class
VORT_SURF_PATH=Vorticity_Base/doMain/Surface

if [[ $# -eq 4 ]]; then
    
    #Arguments
    EVOPATH=$1
    BGPATH=$2
    SURFDAT=$3
    OUTNAME=$4

    #Start running code
    #Run projection
    cd ${CURRENTPATH}/Main_Results/Projections && ./doProjection.exe ${EVOPATH} ${BGPATH} Proj_${OUTNAME}
    echo "Projection done!"

    #Run VortSurf
    cd ${CURRENTPATH}/Main_Results/Vort_Surf  && ./doVortSurf.exe ${EVOPATH} VortSurf_${OUTNAME}
    echo "VortSurf done!"

    #Run freeze-out surface from surface.dat
    cd ${CURRENTPATH}/Main_Results/FreezeOut_Surf && ./surfaceToRoot.py ${SURFDAT} Surfdat_${OUTNAME}
    echo "Surface.dat at freeze-out done!"

    echo "All done!"

fi

if [[ $# -eq 3 ]]; then
    
    #Arguments
    EVOPATH=$1
    SURFDAT=$2
    OUTNAME=$3

    #Start running code
    #Run projection
    cd ${CURRENTPATH}/Main_Results/Projections && ./doProjection.exe ${EVOPATH} Proj_${OUTNAME}
    echo "Projection done!"

    #Run VortSurf
    cd ${CURRENTPATH}/Main_Results/Vort_Surf  && ./doVortSurf.exe ${EVOPATH} VortSurf_${OUTNAME}
    echo "VortSurf done!"

    #Run freeze-out surface from surface.dat
    cd ${CURRENTPATH}/Main_Results/FreezeOut_Surf && ./surfaceToRoot.py ${SURFDAT} Surfdat_${OUTNAME}
    echo "Surface.dat at freeze-out done!"

    echo "All done!"

fi

