# Vorticity Projection Code

This code accepts an evolution.root 3D file and creates a .root file with 2D and 1D projections of energy, temperature, velocity and vorticity.

## Projection code

The [Proj_main_cxx](https://github.com/jbarbon/masters-project/blob/main/projection-code/Vorticity_Base/doMain/Projection_Class/src/Proj_main.cxx) code is where you will control your projections. It has two "main" methods. The first "main" method will accept the complete evolution file (Bullet+Background) and the background evolution file that will be used to generate the Bullet Only projections, and the second "main" method is a simplified version of the first one that will only accept the Bullet+Background file.

Now let's look at some important variables:

* **NUMBER_PROJECTIONS** is just a counter of how many projections are left to run. Put the number equal to the number of projections you are performing.

* **CODESPEED** controlls how fast you can run your code based on the number of entries. When set to *Complete*, the code uses all the entries of the evolution file. When set to *Debug*, it uses only one.

* **EnergyInput**, **TemperatureInput**, **VelocityxInput**, **VorticityzInput** will define which 3D histograms you will get from the 3D evolution.root file.

Inside the first main class you will be able to call the *Projection1D* and *Projection2D* functions from the *Projection* class. Note that here we use the background evolution file to create the *BulletOnly* projection. For example, we create an object called "Energy" of the class *Projection*

`        Projection Energy      = Projection("Energy", ch_evo, ch_bg, CODESPEED, f_out);
`

and call a 1D projection in the "x" axis by doing

`        Energy.Project1D("x" , "Bullet+BG" ,  EnergyInput, BinY_Zero, BinY_Zero, BinZ_Zero, BinZ_Zero  ); 
`

or a 2D projection in the "yx" plane 

`        Energy.Project2D("yx", "Bullet+BG" ,  EnergyInput, BinZ_Zero, BinZ_Zero);   
`

Arguments in the *Project1D* method,

* 1st argument is the projection axis,"x", "y" or "z",
* 2nd second argument is the type of projection, where "Bullet+BG" references the projection of the bullet and the background together, "Background" is projection of the background only, and "BulletOnly" is the projection of the bullet only,
* 3rd argument is the histogram name defined in the constructor,
* 4th and 5th arguments are the first and last bin from the axis being projected,
* 6th and 7th arguments are the first and last bin from the other axis being projected.
* **NOTE**: if you choose a projection in "x", the 4th and 5th arguments are refered to the "y" axis, and 6th and 7th to the "z" axis. If you choose projection in the "y" axis, 4th and 5th arguments are refered to the "z" axis, and 6th and 7th to the "x" axis, and so on.

Arguments in the *Project2D* method,

* 1st argument is the projection plane,"yx", "yz" or "zx",
* 2nd second argument is the type of projection, where "Bullet+BG" references the projection of the bullet and the background altogether, "Background" is projection of the background only, and "BulletOnly" is the projection of the bullet only,
* 3rd argument is the histogram name defined in the constructor,
* 4th and 5th arguments are the first and last bin of the only axis being projected.

The second main class is pretty much like the first one, but without the contribution of the background evolution file.

Finally, the "int main" is the part where we decide whether to use the background for the evolution (first main method), or not (second main method). 

## Surface code

The hypersurface code is reponsible for reading the freeze-out hypersurface and creating a .root file with its contents. The vorticity surface code reads the evolution file and creates a histogram with the vorticity plotted agains the angle phi, defined as the ArcTan(uy,ux), and the temperature.


## How to run

The fastest way to run the codes is to use the wrapper at the top level folder. In order to run the projections you must create a text file containing the paths to the evolution.root 
files. If you just want to run the Bullet+BG evolution without the background evolution, you can create a text file with complete paths and run

`./run-projection.sh [EVOPATHs.txt] [OUTPUTNAME.root].
`

If you want to add the background contribution, you must also create a text file with the background evolution files and run

`./run-projection.sh [EVOPATHs.txt] [BGPATHs.txt] [OUTPUTNAME.root].
`

Note that if you want to run multiple projections at once, the paths from [BGPATHs.txt] must be in the same order as the paths from [EVOPATHs.txt], that is, if [EVOPATHs.txt] has the paths to the simulation model A,B and C, [BGPATHs.txt] must contain the paths to A,B and C in the same order.


Finally, in order to run the surface code, you must create a text file containing the paths to the evolution files and also get the path for the [Surface.dat]. The correct call for this case is

`./run-surface.sh [EVOPATHs.txt] [Surface.dat path] [OUTPUTNAME.root]`

The wrappers will automatically compile the C++ codes and create a folder called Main_Results where the final projections and analysis will be. Also, at the top level folder there will be a .log file with the progress of the code.