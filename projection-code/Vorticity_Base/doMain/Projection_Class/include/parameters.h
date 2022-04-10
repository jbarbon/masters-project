#ifndef MYPARAMETERS_H
#define MYPARAMETERS_H

// Class for the parameters of the evolution
class Parameters {
    
private: //variables
    Float_t TauSecond;
    Float_t tau;


public: // variables

    //Step by witch the tau in our analysis will increase
    Float_t Step;

    Parameters(TTree* T);
    
    Float_t TauInitial, TauFinal, TauStep;
    Int_t NumberEntries;

    // Variables used to create the types of tau array
    Int_t CompleteTauCuts_Entries;
    Int_t HalfTauCuts_Entries;
    Int_t SmallTauCuts_Entries;
    Int_t DebugTauCuts_Entries;
    Int_t TauCutsEntries;

private:

    //Methods to generate the array of taus values
    void GenerateCompleteTauArrayValues(Float_t TauArrayValues[]);
    void GenerateHalfTauArrayValues(Float_t TauArrayValues[]);
    void GenerateSmallTauArrayValues(Float_t TauArrayValues[]);
    void GenerateDebugTauArrayValues(Float_t TauArrayValues[]);


    //Methods to generate the array of taus entries
    void GenerateCompleteTauEntries(Float_t TauEntryArray[]);
    void GenerateHalfTauEntries(Float_t TauEntryArray[]);
    void GenerateSmallTauEntries(Float_t TauEntryArray[]);   
    void GenerateDebugTauEntries(Float_t TauEntryArray[]);


public:

    void PrintParameters() const;

    //  Return the Number of entries based on the type o code speed
    Int_t TauArrayEntries(TString CodeSpee);

    //Fills an array with the entries based on the type of code speed
    void GetTauEntryArray(Float_t MyArray[], TString CodeSpeed);

    //Fills an array with the tau values based on the type of code speed
    void GetTauArrayValues(Float_t MyArray[], TString CodeSpeed);

};


#endif /* MYPARAMETERS_H */ 