// Class of projection made by Joao Barbon
#ifndef MYPROJECTION_H
#define MYPROJECTION_H

#include "parameters.h"

 
// Class to create Projections
class Projection
{
private: //private variables
    TString NameOfObservable;
    TTree* TEvo;
    TTree* TBG;
    TString CodeSpeed;
    TFile* f_out;

    // Variables for the counter
    static int AddGlobalCounter() ;
    static int id;  

public:

    //constructor
    Projection(TString MyNameOfObservable, TTree* MyTEvo, TTree* MyTBG, TString MyCodeSpeed, TFile* Myf_out);
    Projection(TString MyNameOfObservable, TTree* MyTEvo, TString MyCodeSpeed, TFile* Myf_out);
    //Projection();

    //Variable for the counter
    static int idGlobal;

    ///////////////////// Main methods used
private:

    //Creates histogram title
    template<typename M1, typename M2>
    void CreateTitle1D(TString Axis, M1 h3, M2 h1, Float_t BinAmin, Float_t BinAmax, Float_t BinBmin, Float_t BinBmax);

    template<typename T3, typename T2>
    void CreateTitle2D(TString Axis, T3 h3, T2 h2, Float_t BinAmin, Float_t BinAmax);

    // Generate directory paths
    TString GenerateDirPath(TString TypeOfProj, TString Axis);
   
public:

    // Project 1D
    void Project1D(TString Axis, TString TypeOfProj, const char* Quantity, Float_t BinAmin, Float_t BinAmax, Float_t BinBmin, Float_t BinBmax);
    
    //Project 2D
    void Project2D(TString Axis, TString TypeOfProj, const char* Quantity, Float_t BinAmin, Float_t BinAmax);
};

#endif /* MYPROJECTION_H */