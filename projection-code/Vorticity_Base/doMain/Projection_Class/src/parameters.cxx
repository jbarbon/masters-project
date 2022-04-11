#include <iostream>
#include "TROOT.h"
#include <TStyle.h>
#include "TSystem.h"
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

#include "../include/parameters.h"
//------------------------------------------------------------------------------------------------------------------------
// Class for the parameters of the evolution
//------------------------------------------------------------------------------------------------------------------------

Parameters::Parameters(TTree* T)
{
    Step = 0.25;

    NumberEntries = T->GetEntries();
    T->SetBranchAddress("tau", &tau);
    T->GetEntry(0); TauInitial = tau;
    T->GetEntry(1); TauSecond = tau;
    T->GetEntry(NumberEntries - 1); TauFinal = tau;
    TauStep = round( (TauSecond - TauInitial) * 1000.0) / 1000.0;
    //Information about the array of taus that will be created
    CompleteTauCuts_Entries =  (Int_t) ( (Int_t) (TauFinal - TauInitial) ) / Step;
    HalfTauCuts_Entries     =  (Int_t) ( (Int_t) (TauFinal - TauInitial) ) / ( 2 * Step );
    SmallTauCuts_Entries    =  (Int_t) ( (Int_t) (TauFinal - TauInitial) ) / ( 10 * Step) ;
    DebugTauCuts_Entries    =  1;

}

void Parameters::PrintParameters() const
{
    std::cout << "Number of Entries of TTree: " << NumberEntries << std::endl;
    std::cout << "Initial tau: "       << TauInitial    << std::endl;
    std::cout << "Second tau : "       << TauSecond     << std::endl;
    std::cout << "Final tau: "         << TauFinal      << std::endl;
    std::cout << "Tau step: "          << TauStep       << std::endl;
    std::cout << "Step of Tau Cuts: "  << Step          << std::endl;
    std::cout << "---------------------------------" << std::endl;


    std::cout << "----------------------------Possible tau array types----------------------------" << std::endl;

    
    // Print Tau array types
    std::cout << "Complete mode" << std::endl;
    std::cout << "Number of entries: " << CompleteTauCuts_Entries << std::endl;
    for(int i = 0; i < CompleteTauCuts_Entries; i++)
    {
        std::cout << TauInitial + ( Step * i ) << " ";
    } 
    std::cout << std::endl;

    std::cout << "Standard mode" << std::endl;
    std::cout << "Number of entries: " << HalfTauCuts_Entries << std::endl;
    for(int i = 0; i < HalfTauCuts_Entries; i++)
    {
        std::cout << TauInitial + ( Step * i ) << "  ";
    }
    std::cout << std::endl;

    std::cout << "Fast mode" << std::endl;
    std::cout << "Number of entries: " << SmallTauCuts_Entries << std::endl;
    for(int i = 0; i < SmallTauCuts_Entries; i++)
    {
        std::cout << TauInitial + ( Step * i ) << " ";
    }
    std::cout << std::endl;

    std::cout << "Debug mode" << std::endl;
    std::cout << "Number of entries: " << DebugTauCuts_Entries << std::endl;
    for(int i = 0; i < DebugTauCuts_Entries; i++)
    {
        std::cout << TauInitial + ( Step * i ) << " ";
    }
    std::cout << std::endl;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
//Methods to generate the array of taus values
//------------------------------------------------------------------------------------------------------------------------------------------------

void Parameters::GenerateCompleteTauArrayValues(Float_t TauArrayValues[])
{
    for(int i = 0; i < CompleteTauCuts_Entries ; i++)
    {
        TauArrayValues[i] = TauInitial + ( Step * i );
    }   
}

void Parameters::GenerateHalfTauArrayValues(Float_t TauArrayValues[])
{
    for(int i = 0; i < HalfTauCuts_Entries ; i++)
    {
        TauArrayValues[i] = TauInitial + ( Step * i );
    }   
}

void Parameters::GenerateSmallTauArrayValues(Float_t TauArrayValues[])
{
    for(int i = 0; i < SmallTauCuts_Entries ; i++)
    {
        TauArrayValues[i] = TauInitial + ( Step * i );
    }   
}

void Parameters::GenerateDebugTauArrayValues(Float_t TauArrayValues[])
{
    for(int i = 0; i < DebugTauCuts_Entries ; i++)
    {
        TauArrayValues[i] = TauInitial + ( Step * i );
    }   
}
//------------------------------------------------------------------------------------------------------------------------------------------------
//Methods to generate the array of taus entries
//------------------------------------------------------------------------------------------------------------------------------------------------
void Parameters::GenerateCompleteTauEntries(Float_t TauEntryArray[])
{
    Float_t TauCutsArray[CompleteTauCuts_Entries];
    for(int i = 0; i < CompleteTauCuts_Entries ; i++)
    {
        TauCutsArray[i] = TauInitial + ( Step * i );
        TauEntryArray[i] =  (TauCutsArray[i] - TauInitial) / (TauStep);
    }   
}
    
void Parameters::GenerateHalfTauEntries(Float_t TauEntryArray[])
{
    Float_t TauCutsArray[HalfTauCuts_Entries];
    for(int i = 0; i < HalfTauCuts_Entries ; i++)
    {
        TauCutsArray[i] = TauInitial + ( Step * i );
        TauEntryArray[i] =  (TauCutsArray[i] - TauInitial) / (TauStep);
    }   
}
    
void Parameters::GenerateSmallTauEntries(Float_t TauEntryArray[])
{
    Float_t TauCutsArray[SmallTauCuts_Entries];
    for(int i = 0; i < SmallTauCuts_Entries ; i++)
    {
        TauCutsArray[i] = TauInitial + ( Step * i );
        TauEntryArray[i] =  (TauCutsArray[i] - TauInitial) / (TauStep);
    }   
}
     
void Parameters::GenerateDebugTauEntries(Float_t TauEntryArray[])
{
    Float_t TauCutsArray[DebugTauCuts_Entries];
    for(int i = 0; i < DebugTauCuts_Entries ; i++)
    {
        TauCutsArray[i] = TauInitial + ( Step * i );
        TauEntryArray[i] =  (TauCutsArray[i] - TauInitial) / (TauStep);
    }   
}

//------------------------------------------------------------------------------------------------------------------------------------------------
// Functions related to the tau values
//------------------------------------------------------------------------------------------------------------------------------------------------

Int_t Parameters::TauArrayEntries(TString CodeSpeed)
{
    Int_t NumberOfTauEntries = 0;
    if(CodeSpeed == "Complete")
        {
            NumberOfTauEntries =  CompleteTauCuts_Entries;
        }
        else if(CodeSpeed == "Standard") 
        {
            NumberOfTauEntries =  HalfTauCuts_Entries;
        }
        else if(CodeSpeed == "Fast") 
        {
            NumberOfTauEntries =  SmallTauCuts_Entries;
        }
        else if(CodeSpeed == "Debug") 
        {
            NumberOfTauEntries =  DebugTauCuts_Entries;
        }
    return NumberOfTauEntries;
}

void Parameters::GetTauEntryArray(Float_t MyArray[], TString CodeSpeed)
{
    if(CodeSpeed == "Complete")
        {
            GenerateCompleteTauEntries(MyArray);
        }
        else if(CodeSpeed == "Standard") 
        {
            GenerateHalfTauEntries(MyArray);
        }
        else if(CodeSpeed == "Fast") 
        {
            GenerateSmallTauEntries(MyArray);
        }
        else if(CodeSpeed == "Debug") 
        {
            GenerateDebugTauEntries(MyArray);
        }   
}

void Parameters::GetTauArrayValues(Float_t MyArray[], TString CodeSpeed)
{
    if(CodeSpeed == "Complete")
        {
            GenerateCompleteTauArrayValues(MyArray);
        }
        else if(CodeSpeed == "Standard") 
        {
            GenerateHalfTauArrayValues(MyArray);
        }
        else if(CodeSpeed == "Fast") 
        {
            GenerateSmallTauArrayValues(MyArray);
        }
        else if(CodeSpeed == "Debug") 
        {
            GenerateDebugTauArrayValues(MyArray);
        }
}
