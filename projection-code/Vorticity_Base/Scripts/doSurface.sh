#!/bin/bash


#Important paths
CURRENTPATH=${PWD}

if [[ $# -eq 3 ]]; then
     
    #Arguments
    EVOPATH=$1
    SURFDAT=$2
    OUTNAME=$3

    #Start running code


    #Run VortSurf
    cd ${CURRENTPATH}/Main_Results/Vort_Surf  && ./doVortSurf.exe ${EVOPATH} VortSurf_${OUTNAME}
    echo "VortSurf done!"

    #Run freeze-out surface from surface.dat
    cd ${CURRENTPATH}/Main_Results/FreezeOut_Surf && ./surfaceToRoot.py ${SURFDAT} Surfdat_${OUTNAME}
    echo "Surface.dat at freeze-out done!"

    echo "All done!"
fi