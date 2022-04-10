#include<string>
#include "ROOT/RDataFrame.hxx"
#include <vector>
#include "TChain.h"

//Gives information about the taus
std::tuple<Float_t, Float_t, Double_t > GetTau(TTree* T){
    Float_t tau ;
    Float_t tau_0 ;
    Float_t tau_1 ;
    Float_t tau_final ;
    Double_t tau_step;
    Int_t n_entries = T->GetEntries();


    T->SetBranchAddress("tau", &tau);
    T->GetEntry(0); tau_0 = tau;
    T->GetEntry(1); tau_1 = tau;
    T->GetEntry(n_entries - 1); tau_final = tau;

    tau_step = round( (tau_1 - tau_0) * 1000.0) / 1000.0;


    return std::make_tuple(tau_0, tau_final, tau_step );
}


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

int main(int argc, char** argv){

    std::cout << "*************************************************"    << std::endl;
    std::cout << " Plot Vorticity vs Phi vs Temp from MUSIC outputs "   << std::endl;
    std::cout << "*************************************************"    << std::endl;
    
    if (argc-1 < 2) {
       std::cout<<"  Improper number of arguments! (received "<<argc-1<<")  \n"
        <<"  Correct call: ./doVortSurf.exec [EvolutionFilePaths.txt] [Output.root]"<< std::endl;
        return -1;
    }

    TString EvoFile      = argv[1];
    TString Output_Name  = argv[2];

    std::vector<std::string> evoPaths = GetPaths(EvoFile); // Get paths from file into a vector

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
        std::cout<<"Evolution File: " << std::endl;
        std::cout<< path << std::endl;
    }            

    std::cout<<"Output name...................: "<< Output_Name.Data()    <<std::endl;

    //Configure TFiles and TTree and TChain

    TChain* ch_evo = new TChain("TFullEvo");

    for(TString evo_line_path : evoPaths)
    {
        ch_evo->Add(evo_line_path);
    }

    //----------------------------------------------------------------------------------------------------
    //Define Variables and pointers
 
    std::vector<Double_t> vec_phi;
    std::vector<Double_t> vec_vort;
    std::vector<Double_t> vec_temp;
    
    Int_t n_bins_y_h2;
    Int_t n_bins_x_h2;
    //Double_t bin_content;

    Int_t nentries_evo     = ch_evo->GetEntries();


    //Background
    TH3F* h3_omega = NULL;
    TH2F* h2_omega_energy = NULL;


    TH3F* h3_energy = NULL;
    TH2F* h2_energy = NULL;

    TH3F* h3_ux = NULL;
    TH3F* h3_uy = NULL;

    TH3F* h3_temperature = NULL;

//Set Tree variables

    ch_evo->SetBranchAddress("homega_kNoSP_xy",&h3_omega);
    ch_evo->SetBranchAddress("hEnergy",&h3_energy);

    ch_evo->SetBranchAddress("hux",&h3_ux);
    ch_evo->SetBranchAddress("huy",&h3_uy);

    ch_evo->SetBranchAddress("hTemperature",&h3_temperature);

//----------------------------------------------------------------------------------------------------
//Background projections
//----------------------------------------------------------------------------------------------------
//_________________________________________________________________________________________________________________________________________________________ 

    ch_evo->GetEntry(0);
    Int_t bin_min = 1 ; 
    Int_t bin_max = h3_omega->GetZaxis()->GetNbins(); 
 
 //Declaration of variables
    h3_omega->Multiply(h3_energy);
    h3_omega->GetZaxis()->SetRange(bin_min, bin_max);
    h3_energy->GetZaxis()->SetRange(bin_min, bin_max);
    h3_ux->GetZaxis()->SetRange(bin_min, bin_max);
    h3_uy->GetZaxis()->SetRange(bin_min, bin_max);

    h2_omega_energy = (TH2F*) h3_omega->Project3D("xy");
    h2_energy = (TH2F*) h3_energy->Project3D("xy");

    h2_omega_energy->Divide(h2_energy); // Tirar média ponderada // Fazer com e sem média ponderada 

//-------------------------------------------------------------------------------------------------------------------------
//Fazer para cada slice também
    TH2D* Temp_vs_phi = new TH2D("Temp_vs_phi", "Dependencia da vorticidade com #phi e T", 100, -TMath::Pi(), TMath::Pi(), 400, 100.5, 500.5);
    Temp_vs_phi->GetXaxis()->SetTitle("#phi");
    Temp_vs_phi->GetYaxis()->SetTitle("Temperature (GeV)");
    Temp_vs_phi->GetZaxis()->SetTitle("#omega");
   // TH2D* Temp_vs_phi = new TH2D("Temp_vs_phi", "Dependencia da vorticidade com #phi e T", 100, 0, 2*TMath::Pi(), 351, 149.5, 500.5);
//Se quiser tempo específico, integrar em temperatura Temp_vs_phi->ProjectionX(), e mudar o loop de nentries para somente o tempo que
// eu quero ver

    TFile *f_out = new TFile(Output_Name, "RECREATE");
    f_out->mkdir("T_vs_phi");
    //TDirectory *Dir_T_vs_phi = f_out->mkdir("T_vs_phi");

    f_out->cd();
    gDirectory->cd("T_vs_phi");

    for(int t = 0; t < nentries_evo ; t++){
        ch_evo->GetEntry(t);

        n_bins_y_h2 = h3_omega->GetYaxis()->GetNbins();
        n_bins_x_h2 = h3_omega->GetXaxis()->GetNbins();
        //Fill th2 with values of th3
        for(int jz = bin_min; jz <= bin_max; jz++){
            for(int jx = 1; jx <= n_bins_x_h2; jx++){
                //Double_t x_val = h3_omega->GetXaxis()->GetBinCenter(jx);

                for(int jy = 1; jy <= n_bins_y_h2; jy++){
                    //Double_t y_val = h3_omega->GetYaxis()->GetBinCenter(jy);
                   
                    Double_t uy = h3_uy->GetBinContent(jx,jy,jz);
                    Double_t ux = h3_ux->GetBinContent(jx,jy,jz);
                    //Double_t phi = TMath::ATan2(y_val, x_val);
                    Double_t phi = TMath::ATan2(uy, ux);
                    //vec_phi.push_back(phi);

                    Double_t bin_content_temp = 1000.0 * h3_temperature->GetBinContent(jx, jy, jz); // Temp in In MeV now (Originally in GeV)
                    Double_t bin_content_vort = h3_omega->GetBinContent(jx, jy, jz);
                    //cout << "Bin content temp: " << bin_content_temp << " | Bin content vort: " << bin_content_vort << "Phi: " << phi <<  endl;

                    Temp_vs_phi->Fill(phi, bin_content_temp, abs(bin_content_vort));
                }
            }
        }
        std::cout << t << " / " << nentries_evo << std::endl;
    }


    Temp_vs_phi->Write();
    Temp_vs_phi->Draw("colz");

    f_out->Close();


}
