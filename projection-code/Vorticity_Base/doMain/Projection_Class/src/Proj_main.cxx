// Code developed by João Barbon
// This code creates 1D and 2D projections of energy, velocity Ux, vorticity and temperature. 
// You can define the ranges x,y or z of projection. For example, in the line
//        Energy.Project1D("x" , "Bullet+BG" ,  EnergyInput, BinY_Zero, BinY_Zero, BinZ_Zero, BinZ_Zero  ); 
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>


#include "TROOT.h"
#include <TStyle.h>
#include "TSystem.h"
#include "TChain.h"
#include "TApplication.h"
#include "TGenerator.h"
#include "TObjArray.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TF1.h"
#include "Riostream.h"
#include "cstdlib" 
//#include "SDL/SDL.h"
#include "math.h"
#include "TStopwatch.h"
#include "TParticle.h"
#include "TParticlePDG.h"
#include "TDatabasePDG.h"
#include "TPDGCode.h"
#include "TLeaf.h" 
#include "TGTab.h"
#include "TParameter.h"
#include "TMath.h"
#include "Math/IFunction.h"

#include "../include/projection.h"
#include "../include/parameters.h"

//using namespace std;

// Number of projections made in main(). This is used to count how many proj are left
#define NUMBER_PROJECTIONS 27;

int Projection::idGlobal = NUMBER_PROJECTIONS;
int Projection::id  = 0;

// The purpose of this class is to create two methods, both called main. The first "main" method will accept the
// background evolution file, that will be used to generate the Bullet Only projections
// The second "main" method is a simplified version of the first one, and will only accept the Bullet+BG file.
class MainOverload_Class
{

public:
    
    //Define variables
    TString CODESPEED;
    const char* EnergyInput     ;
    const char* TemperatureInput;
    const char* VelocityxInput  ;
    const char* VorticityzInput ;

    //Constructor 
    MainOverload_Class(){

        //Define code speed - Complete: All entries, 
        //                    Standard: half total entries, 
        //                    Fast: 1/10 total entries
        //                    Debug: 1 entry
        CODESPEED = "Complete"; 

        // Define which histograms to pick from evolution file 
        EnergyInput       = "hEnergy";
        TemperatureInput  = "hTemperature";
        VelocityxInput    = "hux";
        VorticityzInput   = "homega_kNoSP_xy";

    }

    

    //Main method - Accepts background
    void main(std::vector<std::string> inputFileEvo, std::vector<std::string> inputFileBG, TString OutputName)
    {


   
        //Configure TFiles and TTree and TChain

        TChain* ch_evo = new TChain("TFullEvo");
        TChain* ch_bg = new TChain("TFullEvo");

        for(TString evo_line_path : inputFileEvo){
            ch_evo->Add(evo_line_path);
        }

        for(TString bg_line_path : inputFileBG){
            ch_bg->Add(bg_line_path);
        }

        //TFile* file_evo    = TFile::Open(inputFileEvo  , "READ"); 
        //TFile* file_evo_bg = TFile::Open(inputFileBG, "READ");
        //TTree* T_evo    = (TTree*) file_evo->Get("TFullEvo");
        //TTree* T_evo_bg = (TTree*) file_evo_bg->Get("TFullEvo");

    //------------------------------------------------------------------
    //Get Bin values -> to be used in the projections for SetRange()
        TH3F* h3_histogram = NULL;
        ch_evo->SetBranchAddress("homega_kNoSP_xy", &h3_histogram);
        ch_evo->GetEntry(0);

        //Int_t BinX_Zero = h3_histogram->GetXaxis()->FindBin(0.0); // 141
        Int_t BinY_Zero = h3_histogram->GetYaxis()->FindBin(0.0); // 141
        Int_t BinZ_Zero = h3_histogram->GetZaxis()->FindBin(0.0); // 101
        Int_t BinX_First = 1;
        //Int_t BinY_First = 1;
        //Int_t BinZ_First = 1;
        Int_t BinX_Last = h3_histogram->GetXaxis()->GetNbins(); // 281
        //Int_t BinY_Last = h3_histogram->GetYaxis()->GetNbins(); // 281
        //Int_t BinZ_Last = h3_histogram->GetZaxis()->GetNbins();  // 201
    //------------------------------------------------------------------

        TFile* f_out = new TFile(OutputName,"RECREATE");

        // Create projection objects

        // Make projections of Energy
        Projection Energy      = Projection("Energy", ch_evo, ch_bg, CODESPEED, f_out);

        Energy.Project1D("x" , "Bullet+BG" ,  EnergyInput, BinY_Zero, BinY_Zero, BinZ_Zero, BinZ_Zero  ); 
        Energy.Project2D("yx", "Bullet+BG" ,  EnergyInput, BinZ_Zero, BinZ_Zero);   
                          
        Energy.Project1D("x" , "Background",  EnergyInput, BinY_Zero, BinY_Zero, BinZ_Zero, BinZ_Zero);   
        Energy.Project2D("yx", "Background",  EnergyInput, BinZ_Zero, BinZ_Zero);         

    
        Energy.Project1D("x" , "BulletOnly",  EnergyInput, BinY_Zero, BinY_Zero, BinZ_Zero, BinZ_Zero );  
        Energy.Project2D("yx", "BulletOnly",  EnergyInput, BinZ_Zero, BinZ_Zero);                    

        // Make projections of Temperature
        Projection Temperature = Projection("Temperature", ch_evo, ch_bg, CODESPEED, f_out);

        Temperature.Project1D("x" , "Bullet+BG" ,  TemperatureInput, BinY_Zero, BinY_Zero, BinZ_Zero, BinZ_Zero );
        Temperature.Project2D("yx", "Bullet+BG" ,  TemperatureInput, BinZ_Zero, BinZ_Zero);   

        Temperature.Project1D("x" , "Background",  TemperatureInput, BinY_Zero, BinY_Zero, BinZ_Zero, BinZ_Zero );
        Temperature.Project2D("yx", "Background",  TemperatureInput, BinZ_Zero, BinZ_Zero);    
    
        Temperature.Project1D("x" , "BulletOnly",  TemperatureInput, BinY_Zero, BinY_Zero, BinZ_Zero, BinZ_Zero );
        Temperature.Project2D("yx", "BulletOnly",  TemperatureInput, BinZ_Zero, BinZ_Zero);                       

        // Make projections of velocity
        Projection Ux          = Projection("Velocity", ch_evo, ch_bg, CODESPEED, f_out);
        
        Ux.Project1D("x" , "Bullet+BG" ,  VelocityxInput, BinY_Zero, BinY_Zero, BinZ_Zero, BinZ_Zero );
        Ux.Project2D("yx", "Bullet+BG" ,  VelocityxInput, BinZ_Zero, BinZ_Zero);  

        Ux.Project1D("x" , "Background",  VelocityxInput, BinY_Zero, BinY_Zero, BinZ_Zero, BinZ_Zero );
        Ux.Project2D("yx", "Background",  VelocityxInput, BinZ_Zero, BinZ_Zero);  
                            
        Ux.Project1D("x" , "BulletOnly",  VelocityxInput, BinY_Zero, BinY_Zero, BinZ_Zero, BinZ_Zero );
        Ux.Project2D("yx", "BulletOnly",  VelocityxInput, BinZ_Zero, BinZ_Zero);                       

        // Make projections of Vorticity
        Projection Vorticity   = Projection("Vorticity", ch_evo, ch_bg, CODESPEED, f_out);

        Vorticity.Project1D("x" , "Bullet+BG" ,  VorticityzInput, BinY_Zero,  221,        BinZ_Zero, BinZ_Zero );
        Vorticity.Project1D("y" , "Bullet+BG" ,  VorticityzInput, BinX_First, BinX_Last,  BinZ_Zero, BinZ_Zero );
        Vorticity.Project2D("yx", "Bullet+BG" ,  VorticityzInput, BinZ_Zero,  BinZ_Zero);
        
        Vorticity.Project1D("x" , "Background",  VorticityzInput, BinY_Zero,  221,        BinZ_Zero, BinZ_Zero );
        Vorticity.Project1D("y" , "Background",  VorticityzInput, BinX_First, BinX_Last,  BinZ_Zero, BinZ_Zero );
        Vorticity.Project2D("yx", "Background",  VorticityzInput, BinZ_Zero,  BinZ_Zero);

        Vorticity.Project1D("x" , "BulletOnly",  VorticityzInput, BinY_Zero,  221,        BinZ_Zero, BinZ_Zero );
        Vorticity.Project1D("y" , "BulletOnly",  VorticityzInput, BinX_First, BinX_Last,  BinZ_Zero, BinZ_Zero );
        Vorticity.Project2D("yx", "BulletOnly",  VorticityzInput, BinZ_Zero,  BinZ_Zero);

        f_out->Close(); 

        std::cout << "Finished!" << std::endl;

    }


    //Main method - No background
    void main(std::vector<std::string> inputFileEvo, TString OutputName)
    {
    

        //Configure TFiles and TTree and TChain

        TChain* ch = new TChain("TFullEvo");

        for(TString evo_line_path : inputFileEvo )
        {
            ch->Add(evo_line_path);
        }

        //TFile* file_evo    = TFile::Open(inputFileEvo  , "READ"); 
        //TTree* T_evo    = (TTree*) file_evo->Get("TFullEvo");

    /////////////////////////////////////////Get Bin values
        TH3F* h3_histogram = NULL;
        ch->SetBranchAddress("homega_kNoSP_xy", &h3_histogram);
        ch->GetEntry(0);

        //Int_t BinX_Zero = h3_histogram->GetXaxis()->FindBin(0.0); // 141
        Int_t BinY_Zero = h3_histogram->GetYaxis()->FindBin(0.0); // 141
        Int_t BinZ_Zero = h3_histogram->GetZaxis()->FindBin(0.0); // 101
        Int_t BinX_First = 1;
        //Int_t BinY_First = 1;
        //Int_t BinZ_First = 1;
        Int_t BinX_Last = h3_histogram->GetXaxis()->GetNbins(); // 281
        //Int_t BinY_Last = h3_histogram->GetYaxis()->GetNbins(); // 281
        //Int_t BinZ_Last = h3_histogram->GetZaxis()->GetNbins();  // 201
    /////////////////////////////////////////

        TFile* f_out = new TFile(OutputName,"RECREATE");

        // Create projection objects

        // Make projections of Energy
        Projection Energy      = Projection("Energy", ch, CODESPEED, f_out);
        Energy.Project1D("x" , "Bullet+BG" ,  EnergyInput, BinY_Zero, BinY_Zero, BinZ_Zero, BinZ_Zero  ); 
        Energy.Project2D("yx", "Bullet+BG" ,  EnergyInput, BinZ_Zero, BinZ_Zero);                     

        // Make projections of Temperature
        Projection Temperature = Projection("Temperature", ch, CODESPEED, f_out);

        Temperature.Project1D("x" , "Bullet+BG" ,  TemperatureInput, BinY_Zero, BinY_Zero, BinZ_Zero, BinZ_Zero );
        Temperature.Project2D("yx", "Bullet+BG" ,  TemperatureInput, BinZ_Zero, BinZ_Zero);   

        // Make projections of velocity
        Projection Ux          = Projection("Velocity", ch, CODESPEED, f_out);
        Ux.Project1D("x" , "Bullet+BG" ,  VelocityxInput, BinY_Zero, BinY_Zero, BinZ_Zero, BinZ_Zero );
        Ux.Project2D("yx", "Bullet+BG" ,  VelocityxInput, BinZ_Zero, BinZ_Zero);                    

        // Make projections of Vorticity
        Projection Vorticity   = Projection("Vorticity", ch, CODESPEED, f_out);
        Vorticity.Project1D("x" , "Bullet+BG" ,  VorticityzInput, BinY_Zero,  221,        BinZ_Zero, BinZ_Zero );
        Vorticity.Project1D("y" , "Bullet+BG" ,  VorticityzInput, BinX_First, BinX_Last,  BinZ_Zero, BinZ_Zero );
        Vorticity.Project2D("yx", "Bullet+BG" ,  VorticityzInput, BinZ_Zero,  BinZ_Zero);

        f_out->Close(); 

        std::cout << "Finished!" << std::endl;
    }
};



std::vector<std::string> GetPaths(const char*  evoFile ) // Return paths inside of file
{

  std::vector<std::string> Vec_Paths;

  std::ifstream inFile (evoFile);
  std::string line;

  //Check file was opened
  if (!inFile) {
      std::cerr << "Unable to open file evolution_paths.txt";
      exit(1);   // call system to stop
  }

  if (inFile.is_open())
  {
  while ( std::getline (inFile ,line ) )
  {
    Vec_Paths.push_back(line);
  }
  inFile.close();
  }    

  return Vec_Paths;

}


int main(int argc, char **argv){


    std::cout << "***************************************"  << std::endl;
    std::cout << " Projection from MUSIC outputs "          << std::endl;
    std::cout << "***************************************"  << std::endl;
    

//-----------------------------------------------------------------------------------------------
    //Guard to correct call of arguments
    if ( ( argc - 1 > 3 ) || (argc - 1 < 2 ) ) {
        std::cout <<"  Improper number of arguments! (received "<<argc-1<<")" << std::endl;
        std::cout <<"  Correct call for Evolution + Background: ./doProjection.exec [evolution paths file] [background paths file0] [OutputName.root] "<< std::endl;
        std::cout <<"  Correct call for Evolution: ./doProjection.exec [evolution paths file] [OutputName.root] "<< std::endl;
        std::cout <<"  If you want to run evolution without background, you must have a file containing ";
        std::cout << "the paths for the evolution.root in separate lines. Else, if you want to run evolution with background, ";
        std::cout << "you must have two files containing the paths for both evolution.root and background.root files." << std::endl;
        return -1;
    }

//-----------------------------------------------------------------------------------------------
    //Create the Class
    MainOverload_Class MainOverload;
    
    // Variables to be used
    TString inputFileEvo;
    TString inputFileBG ;
    TString OutputName  ;


//-----------------------------------------------------------------------------------------------
//Case for two arguments. Input for evolution (Bullet+BG) and output.root
    if (argc - 1 == 2)
    {

    std::cout << "You chose evolution with no background projection." << std::endl;

    inputFileEvo = argv[1];
    OutputName   = argv[2];

    std::vector<std::string> evoPaths = GetPaths(inputFileEvo);

    //Check that the provided input name corresponds to an existing file.
    for(std::string path : evoPaths)
    {

        std::ifstream is(path);                                                                                                                                              
        if (!is) {                                                                                                                                                         
            std::cout << "  File " << path << " was not found." << std::endl;                                                                                                  
            std::cout << "  Program stopped! " << std::endl;                                                                                                                              
            exit(1);                                                                                                                                                      
        }     

        // cout paths of evolution files
        std::cout<<"Bullet Evolution: " << std::endl;
        std::cout<< path << std::endl;
    }                                                                                        

    std::cout<<"Output name...................: "<< OutputName.Data()    <<std::endl;

    //Calls the methods that runs the projection codes
    MainOverload.main(evoPaths, OutputName);

    }

//-----------------------------------------------------------------------------------------------
//Case for three arguments. Input for evolution (Bullet+BG), background and output.root
    else if(argc - 1 == 3)
    {
        std::cout << "You chose Evolution with background projection." << std::endl;
        inputFileEvo = argv[1];
        inputFileBG  = argv[2];
        OutputName   = argv[3];


        std::vector<std::string> evoPaths = GetPaths(inputFileEvo);
        std::vector<std::string> bgPaths = GetPaths(inputFileBG);

    //Check that the provided input name corresponds to an existing file.     
    for(std::string evo_path : evoPaths)
    {


        std::ifstream is(evo_path);                                                                                                                                              
        if (!is) {                                                                                                                                                         
            std::cout << "  File " << evo_path << " was not found." << std::endl;                                                                                                  
            std::cout << "  Program stopped! " << std::endl;                                                                                                                              
            exit(1);                                                                                                                                                      
        }     

        std::cout <<"Bullet Evolution : " << std::endl;
        std::cout << evo_path << std::endl;
    }   

    for(std::string bg_path : bgPaths)
    {

        std::ifstream is2(bg_path);                                                                                                                                              
        if (!is2) {                                                                                                                                                         
            std::cout << "  File " << bg_path << " was not found." << std::endl;                                                                                                  
            std::cout << "  Program stopped! " << std::endl;                                                                                                                              
            exit(1);                                                                                                                                                      
        }    

        std::cout <<"Background Evolution : " << std::endl;
        std::cout << bg_path << std::endl;
    }

    std::cout<<"Output name...................: "<< OutputName.Data()    <<std::endl;

    //Call function to make projection
    MainOverload.main(evoPaths, bgPaths, OutputName);
    }
}