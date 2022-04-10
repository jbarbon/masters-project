#!/bin/bash

#Script to compile and create output folders
CURRENTPATH=${PWD}
PROJ_PATH=${CURRENTPATH}/Vorticity_Base/doMain/Projection_Class                                 
VORT_SURF_PATH=${CURRENTPATH}/Vorticity_Base/doMain/Surface     

#Compiling projection code
echo "Re/Compiling projection code..."
cd ${PROJ_PATH}
make clean 
make
cd ${CURRENTPATH}

#Compiling orticity @ freeze-out surf
echo "Re/Compiling  Vort @ freeze-out surf..."
cd ${VORT_SURF_PATH}
make clean
make
cd ${CURRENTPATH}

#Check if executables are in place
if [ ! -f ${PROJ_PATH}/doProjection.exe ]; then
    echo "/doProjection.exe missing!"
    exit 1
elif [ ! -f ${VORT_SURF_PATH}/doVortSurf.exe ]; then
    echo "/doVortSurf.exe missing!"
    exit 1
elif [ ! -f ${VORT_SURF_PATH}/surfaceToRoot.py ]; then
    echo "/surfaceToRoot.py missing!"
    exit 1
else
    echo "All executables are ok!"
fi
 

#Copy executable files to corresponding folders
mkdir -p ${CURRENTPATH}/Main_Results/{Projections,Vort_Surf,FreezeOut_Surf}
cp ${PROJ_PATH}/doProjection.exe ${CURRENTPATH}/Main_Results/Projections/doProjection.exe
cp ${VORT_SURF_PATH}/doVortSurf.exe ${CURRENTPATH}/Main_Results/Vort_Surf/doVortSurf.exe
cp ${VORT_SURF_PATH}/surfaceToRoot.py ${CURRENTPATH}/Main_Results/FreezeOut_Surf/surfaceToRoot.py

echo "Compilation completed!"
