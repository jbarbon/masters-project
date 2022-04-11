// Class of projection made by Joao Barbon
//#include "ROOT/RDataFrame.hxx"
#include <iostream>
#include <TROOT.h>
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


#include "../include/projection.h"

//-------------------------------------------------------------------------------------------------------
//----------------------------------Functions for the projection class-----------------------------------
//-------------------------------------------------------------------------------------------------------
    //counter function
int Projection::AddGlobalCounter() { return id++; }

//Constructor for projection. Accepts evolution (Bullet+BG) and background evolution files
Projection::Projection(TString MyNameOfObservable, TTree* MyTEvo, TTree* MyTBG, TString MyCodeSpeed, TFile* Myf_out)
    : NameOfObservable(MyNameOfObservable), TEvo(MyTEvo), TBG(MyTBG), CodeSpeed(MyCodeSpeed), f_out(Myf_out) {}

//Constructor for projection. Accepts only evolution (Bullet+BG) 
Projection::Projection(TString MyNameOfObservable, TTree* MyTEvo, TString MyCodeSpeed, TFile* Myf_out)
    : NameOfObservable(MyNameOfObservable), TEvo(MyTEvo), CodeSpeed(MyCodeSpeed), f_out(Myf_out)  {}

//------------------------------------------------------------------------------------------------------------------------
//Creates 1D histogram title. This method is used to create the title of a histogram, provided the parameters passed to it
//------------------------------------------------------------------------------------------------------------------------
template<typename M1, typename M2>
void Projection::CreateTitle1D(TString Axis, M1 h3, M2 h1, Float_t BinAmin, Float_t BinAmax, Float_t BinBmin, Float_t BinBmax)
{
    TString Title;


    if(Axis == "x")
    {
        //Get the actual values of the bins you passed to the function
        Double_t A_range_min_val = h3->GetYaxis()->GetBinCenter(BinAmin);
        Double_t A_range_max_val = h3->GetYaxis()->GetBinCenter(BinAmax);
        Double_t B_range_min_val = h3->GetZaxis()->GetBinCenter(BinBmin);
        Double_t B_range_max_val = h3->GetZaxis()->GetBinCenter(BinBmax);  

        // Creates strings that will be used to make the title. If Axis == "x", this means that the
        // projection will be made on both "y" and "z" coordinates.
        TString A_range_str;
        if(BinAmin == BinAmax) A_range_str = Form("Y = %.4f", A_range_min_val);
        else A_range_str = Form("%.4f < Y < %.4f",A_range_min_val, A_range_max_val );

        TString B_range_str;
        if(BinBmin == BinBmax) B_range_str = Form("Z = %.4f", B_range_min_val);
        else B_range_str = Form("%.2f < Z < %.2f",B_range_min_val, B_range_max_val );   

        Title = Axis + " projection of " + NameOfObservable + " with " + B_range_str + " , " + A_range_str ;

    }else if(Axis == "y")
    {
        Double_t A_range_min_val = h3->GetYaxis()->GetBinCenter(BinAmin);
        Double_t A_range_max_val = h3->GetYaxis()->GetBinCenter(BinAmax);
        Double_t B_range_min_val = h3->GetZaxis()->GetBinCenter(BinBmin);
        Double_t B_range_max_val = h3->GetZaxis()->GetBinCenter(BinBmax);  

        TString A_range_str;
        if(BinAmin == BinAmax) A_range_str = Form("X = %.4f", A_range_min_val);
        else A_range_str = Form("%.4f < X < %.4f",A_range_min_val, A_range_max_val );

        TString B_range_str;
        if(BinBmin == BinBmax) B_range_str = Form("Z = %.4f", B_range_min_val);
        else B_range_str = Form("%.2f < Z < %.2f",B_range_min_val, B_range_max_val );   

        Title = Axis + " projection of " + NameOfObservable + " with " + B_range_str + " , " + A_range_str ;

    }else if(Axis == "z") 
    {
        Double_t A_range_min_val = h3->GetXaxis()->GetBinCenter(BinAmin);
        Double_t A_range_max_val = h3->GetXaxis()->GetBinCenter(BinAmax);
        Double_t B_range_min_val = h3->GetYaxis()->GetBinCenter(BinBmin);
        Double_t B_range_max_val = h3->GetYaxis()->GetBinCenter(BinBmax);  

        TString A_range_str;
        if(BinAmin == BinAmax) A_range_str = Form("X = %.4f", A_range_min_val);
        else A_range_str = Form("%.4f < X < %.4f",A_range_min_val, A_range_max_val );

        TString B_range_str;
        if(BinBmin == BinBmax) B_range_str = Form("Y = %.4f", B_range_min_val);
        else B_range_str = Form("%.2f < Y < %.2f",B_range_min_val, B_range_max_val );   

        Title = Axis + " projection of " + NameOfObservable + " with " + B_range_str + " , " + A_range_str ;
    }else
    {
        std::cout << "Error creating title of the 1D histogram." << std::endl;
    }

    h1->SetTitle(Title);
}

//------------------------------------------------------------------------------------------------------------------------
//Creates 2D histogram title. This method is used to create the title of a histogram, provided the parameters passed to it
//------------------------------------------------------------------------------------------------------------------------
template<typename T3, typename T2>
void Projection::CreateTitle2D(TString Axis, T3 h3, T2 h2, Float_t BinAmin, Float_t BinAmax)
{
    TString Title;

    if(Axis == "xy" || Axis == "yx")
    {
        Double_t A_range_min_val = h3->GetZaxis()->GetBinCenter(BinAmin);
        Double_t A_range_max_val = h3->GetZaxis()->GetBinCenter(BinAmax);


        TString A_range_str;
        if(BinAmin == BinAmax) A_range_str = Form("Z = %.4f", A_range_min_val);
        else A_range_str = Form("%.4f < Z < %.4f",A_range_min_val, A_range_max_val );

        Title = Axis + " projection of " + NameOfObservable + " with " + A_range_str ;

    }else if(Axis == "yz" || Axis == "zy")
    {
        Double_t A_range_min_val = h3->GetXaxis()->GetBinCenter(BinAmin);
        Double_t A_range_max_val = h3->GetXaxis()->GetBinCenter(BinAmax);

        TString A_range_str;
        if(BinAmin == BinAmax) A_range_str = Form("X = %.4f", A_range_min_val);
        else A_range_str = Form("%.4f < X < %.4f",A_range_min_val, A_range_max_val );

        Title = Axis + " projection of " + NameOfObservable + " with " + A_range_str ;

    }else if(Axis == "xz" || Axis == "zx")
    {
        Double_t A_range_min_val = h3->GetYaxis()->GetBinCenter(BinAmin);
        Double_t A_range_max_val = h3->GetYaxis()->GetBinCenter(BinAmax);

        TString A_range_str;
        if(BinAmin == BinAmax) A_range_str = Form("Y = %.4f", A_range_min_val);
        else A_range_str = Form("%.4f < Y < %.4f",A_range_min_val, A_range_max_val );

        Title = Axis + " projection of " + NameOfObservable + " with " + A_range_str ;
    }else
    {
        std::cout << "Error creating title of the 1D histogram." << std::endl;
    }

    h2->SetTitle(Title);
}


TString Projection::GenerateDirPath(TString TypeOfProj, TString Axis)
{
    return TypeOfProj + "/" + NameOfObservable + "/Proj_1D_" + Axis ;
}

//------------------------------------------------------------------------------------------------------------------------
// Project 1D function
//------------------------------------------------------------------------------------------------------------------------

void Projection::Project1D(TString Axis, TString TypeOfProj, const char* Quantity, Float_t BinAmin, Float_t BinAmax, Float_t BinBmin, Float_t BinBmax)
{

//---------------------------------------------------------------------------------------------------
    AddGlobalCounter();

//---------------------------------------------------------------------------------------------------
// Use Parameters class to obtain the parameters from evolution.root file
    Parameters MyPar3 = Parameters(TEvo);

    //Get number of entries from TTree - CodeSpeed determines how many parameters I use (for debug purposes)
    Int_t NumberOfTauEntries = MyPar3.TauArrayEntries(CodeSpeed);

    Float_t TauEntryArray[NumberOfTauEntries];
    Float_t TauValArray[NumberOfTauEntries];

    // Use method to fill array with the tau entry values
    MyPar3.GetTauEntryArray(TauEntryArray, CodeSpeed);
    MyPar3.GetTauArrayValues(TauValArray, CodeSpeed);

    f_out->cd();
    TString MyPath = GenerateDirPath(TypeOfProj, Axis);
    f_out->mkdir(MyPath);
    gDirectory->cd(MyPath);

    //Histograms used inside loop
    TH1F* h1 = NULL;
    TH3F* h3 = NULL;
    TH3F* h3_bg = NULL;
//--------------------------Start loop----------------------------------
    for(int j = 0; j < NumberOfTauEntries; j++)
    {

        TEvo ->SetBranchAddress(Quantity,&h3);
        TEvo->GetEntry(TauEntryArray[j]);            

        // Condition to set branch for bullet only or background 
        if(TypeOfProj == "BulletOnly" || TypeOfProj == "Background" )
        {
            TBG ->SetBranchAddress(Quantity,&h3_bg);
            TBG ->GetEntry(TauEntryArray[j]); 
        }
        
        // condition to subtract background
        if (TypeOfProj == "BulletOnly" ){
            h3->Add(h3_bg, -1.0); 
        }        
        
        if((TypeOfProj == "BulletOnly") || (TypeOfProj == "Bullet+BG" ) )
        {
            //Chooses which in which axis to integrate
            if(Axis == "x"){
                h3->GetYaxis()->SetRange(BinAmin,BinAmax);
                h3->GetZaxis()->SetRange(BinBmin,BinBmax); 
            }else if(Axis == "y"){
                h3->GetXaxis()->SetRange(BinAmin,BinAmax);
                h3->GetZaxis()->SetRange(BinBmin,BinBmax); 
            } else if(Axis == "z"){
                h3->GetXaxis()->SetRange(BinAmin,BinAmax);
                h3->GetYaxis()->SetRange(BinBmin,BinBmax); 
            }else{
                std::cout << "Not a valid Axis" << std::endl;
                break;
            }

            h1 = (TH1F*) h3->Project3D(Axis); // Projection

        }else if(TypeOfProj == "Background" ){

            //Chooses which in which axis to integrate
            if(Axis == "x"){
                h3_bg->GetYaxis()->SetRange(BinAmin,BinAmax);
                h3_bg->GetZaxis()->SetRange(BinBmin,BinBmax); 
            }else if(Axis == "y"){
                h3_bg->GetXaxis()->SetRange(BinAmin,BinAmax);
                h3_bg->GetZaxis()->SetRange(BinBmin,BinBmax); 
            } else if(Axis == "z"){
                h3_bg->GetXaxis()->SetRange(BinAmin,BinAmax);
                h3_bg->GetYaxis()->SetRange(BinBmin,BinBmax); 
            }else{
                std::cout << "Not a valid Axis" << std::endl;
                break;
            }
            h1 = (TH1F*) h3_bg->Project3D(Axis); 

            //Reset bg histogram
            h3_bg->Reset("ICES");
            
        }else{
            std::cout << "Wrong Type of Projection" << std::endl;
            break;
        }

        h1->SetName(Form(NameOfObservable + "_tau_%.2f",TauValArray[j]));
        
        CreateTitle1D(Axis, h3, h1, BinAmin, BinAmax, BinBmin, BinBmax);

        h1->SetDirectory(gDirectory);
        h1->Write();

        //Clear histograms
        h1->Reset("ICES");
        h3->Reset("ICES");

        std::cout << "Creating 1D "<< Axis << " projection" << " from " + NameOfObservable<< " " << TypeOfProj << ". Local " << j+1 <<"/"<< NumberOfTauEntries << " " <<
        " Global: " << id << "/" << idGlobal << std::endl; 

    }  
        //std::cout << "Creating 1D "<< Axis << " projection" << " from " + NameOfObservable<< " " << TypeOfProj << ". Global: " << id << "/" << idGlobal << std::endl; 

}
    
//------------------------------------------------------------------------------------------------------------------------
//Project 2D function
//------------------------------------------------------------------------------------------------------------------------

void Projection::Project2D(TString Axis, TString TypeOfProj, const char* Quantity, Float_t BinAmin, Float_t BinAmax)
{
    
    AddGlobalCounter();

//---------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
    Parameters MyPar2 = Parameters(TEvo);

    Int_t NumberOfTauEntries = MyPar2.TauArrayEntries(CodeSpeed);

    Float_t TauEntryArray[NumberOfTauEntries];
    Float_t TauValArray[NumberOfTauEntries];

    MyPar2.GetTauEntryArray(TauEntryArray, CodeSpeed);

    MyPar2.GetTauArrayValues(TauValArray, CodeSpeed);

    f_out->cd();
    TString MyPath = GenerateDirPath(TypeOfProj, Axis);
    //TDirectory* MyDirectory  = f_out->mkdir(MyPath);
    f_out->mkdir(MyPath);
    //CreateDirectory(MyPath);
    gDirectory->cd(MyPath);

    TH2F* h2 = NULL;     
    TH3F* h3 = NULL;     
    TH3F* h3_bg = NULL;   
//--------------------------Start loop----------------------------------
    for(int j = 0; j < NumberOfTauEntries; j++)
    {

        // Declare histogramsBackground

        TEvo ->SetBranchAddress(Quantity,&h3);
        TEvo->GetEntry(TauEntryArray[j]);         

        // Condition to set branch for bullet only or background 
        if(TypeOfProj == "BulletOnly" || TypeOfProj == "Background" )
        {
            TBG ->SetBranchAddress(Quantity,&h3_bg);
            TBG ->GetEntry(TauEntryArray[j]); 
        }
        
        // condition to subtract background
        if (TypeOfProj == "BulletOnly" ){
            h3->Add(h3_bg, -1.0); 
        }        
        
        
        if((TypeOfProj == "BulletOnly") || (TypeOfProj == "Bullet+BG" ) )
        {
            //Chooses which in which axis to integrate
            if(Axis == "xy" || Axis == "yx"){
                h3->GetZaxis()->SetRange(BinAmin,BinAmax);
            }else if(Axis == "xz" || Axis == "zx"){ 
                h3->GetYaxis()->SetRange(BinAmin,BinAmax);
            } else if(Axis == "yz" || Axis == "zy"){
                h3->GetXaxis()->SetRange(BinAmin,BinAmax);
            }else{
                std::cout << "Not a valid Axis" << std::endl;
                break;
            }
            h2 = (TH2F*) h3->Project3D(Axis); // Projection

        }else if(TypeOfProj == "Background"){

            //Chooses which in which axis to integrate
            if(Axis == "xy" || Axis == "yx"){
                h3_bg->GetZaxis()->SetRange(BinAmin,BinAmax);
            }else if(Axis == "xz" || Axis == "zx"){ 
                h3_bg->GetYaxis()->SetRange(BinAmin,BinAmax);
            } else if(Axis == "yz" || Axis == "zy"){
                h3_bg->GetXaxis()->SetRange(BinAmin,BinAmax);
            }else{
                std::cout << "Not a valid Axis" << std::endl;
                break;
            }
            h2 = (TH2F*) h3_bg->Project3D(Axis); // Projection

            //Clear h3_bg histogram
            h3_bg->Reset("ICES");

        }else{
            std::cout << "Wrong Type of Projection" << std::endl;
            break;
        }
        
        h2->SetName(Form(NameOfObservable + "_tau_%.2f",TauValArray[j]));            
        CreateTitle2D(Axis, h3, h2, BinAmin, BinAmax);

        h2->SetDirectory(gDirectory);
        h2->Write();

        //Clear histogram
        h2->Reset("ICES");
        h3->Reset("ICES");


        std::cout << "Creating 1D "<< Axis << " projection" << " from " + NameOfObservable<< " " << TypeOfProj << ". Local " << j+1 <<"/"<< NumberOfTauEntries << " " <<
        " Global: " << id << "/" << idGlobal << std::endl;      
    }
}


