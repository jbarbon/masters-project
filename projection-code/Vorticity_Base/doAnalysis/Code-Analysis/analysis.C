#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
#include "TLegend.h"
#include "TCanvas.h"


tuple<Double_t,Double_t,Double_t>  GetPositionPeak_Temp(TFile* f, Int_t n_entries_tau_cuts){
   //Def vector that contains the peak
   TH1D* Temp_hists_1d[n_entries_tau_cuts];
   Double_t Vector_of_taus[n_entries_tau_cuts];

   for(int i = 0; i < n_entries_tau_cuts; i++){
      Temp_hists_1d[i] = (TH1D*) f->Get(Form("BulletOnly/Temp/Proj_1D/Temp/Temp_tau_%.2f", (i+1) / 4.0));
      Vector_of_taus[i] = (i+1) / 4.0;
   }

   Int_t max_bin = Temp_hists_1d[0]->GetXaxis()->GetNbins();
   Int_t Bin_of_peak[n_entries_tau_cuts];
   Double_t X_of_peak[n_entries_tau_cuts];
   Double_t Height_of_peak[n_entries_tau_cuts];
   
   //For loop to get the peak of all histograms
   for(int index = 0; index < n_entries_tau_cuts; index++){
   
      Bin_of_peak[index] = Temp_Hists_1d[index]->GetMaximumBin();
      X_of_peak[index] = Temp_Hists_1d_[index]->GetBinCenter(Bin_of_peak[index]);
      Height_of_peak[index] = Temp_Hists_1d_[index]->GetBinContent(Bin_of_peak[index]);

   }


   return make_tuple(X_of_peak, Height_of_peak, Vector_of_taus);
}


void GetMaximumPeak(){
//----------------------------------------------------------------------------------------------------/

//Open File
   //TFile* f_in = TFile::Open(lInput_File, "READ");
   //TFile* f_in = TFile::Open("./Chun_BulletEta_Ideal_0p08_Proj_1D_2D_Vorticity_Temperature_Ux_v2.root", "READ");
   TFile* f_in = TFile::Open("/storage/barbon/projects/Vorticity/Vorticity3D_Dev/Plots/Projections_Chun_Code/Bullets-data5/Results/HB_R-.5_EtaOverS-0.01_Proj.root",
                               "READ");

//----------------------------------------------------------------------------------------------------/
//Declare and initialize histograms
   Int_t Number_Entries_tau_cuts = 60/

//----------------------------------------------------------------------------------------------------/
//Create TCanvas
   //TCanvas *c1 = new TCanvas("c1","Fitting",10,10,800,600);
   //gStyle->SetOptStat(0);
   //c1->Close();
   //c1->SetFillColor(33);
   //c1->SetFrameFillColor(41);
   //c1->SetGrid();

/*

TFile *fout = new TFile("Fitting_Maximum_Peak_Temp_0p08_Ideal.root","RECREATE");

TDirectory* Dir_Fit_Temp_0p08  = fout->mkdir("Bullet_0p08/Fit_Temp_0p08");
TDirectory* Dir_Quant_Temp_0p08  = fout->mkdir("Bullet_0p08/Quantities");

TDirectory* Dir_Fit_Temp_ideal  = fout->mkdir("Bullet_Ideal/Fit_Temp_Ideal");
TDirectory* Dir_Quant_Temp_ideal  = fout->mkdir("Bullet_Ideal/Quantities");

//----------------------------------------------------------------------------------------------------/
//Start fitting bullet 0p08
   TFitResultPtr r; // Can define only once


   TF1 *fitFcn[20];
   TF1 *backFcn[20];
   TF1 *signalFcn[20];

   vector<Double_t> height_0p08; 
   vector<Double_t> width_0p08; 
   vector<Double_t> peak_area_0p08;
   vector<Double_t> chi2_0p08;



   fout->cd();
   gDirectory->cd("Bullet_0p08/Fit_Temp_0p08");

   for(int n = 0; n < 20; n++){

      backFcn[n] = new TF1(Form("backFcn_tau_%.1f",(n+1)/2.0),background,-10,10,3);
      backFcn[n]->SetLineColor(kRed);

      signalFcn[n] = new TF1(Form("signalFcn_tau_%.1f",(n+1)/2.0),GaussianPeak,-10,10,3);
      signalFcn[n]->SetLineColor(kBlue);
      signalFcn[n]->SetNpx(1000);

      fitFcn[n] = new TF1(Form("fitFcn_tau_%.1f",(n+1)/2.0),fitFunction,-10,10,6);
      fitFcn[n]->SetNpx(500);
      fitFcn[n]->SetLineWidth(4);
      fitFcn[n]->SetLineColor(kMagenta);

      backFcn[n]->SetParameters(1,1,1);
      Temp_Hists_1d_0p08[n]->Fit(Form("backFcn_tau_%.1f",(n+1)/2.0),"N0R");
      Double_t bg_par[3];
      backFcn[n]->GetParameters(bg_par);

      fitFcn[n]->FixParameter(0,bg_par[0]);
      fitFcn[n]->FixParameter(1,bg_par[1]);
      fitFcn[n]->FixParameter(2,bg_par[2]);
      fitFcn[n]->SetParameter(3,height_2[n]);     // peak height
      fitFcn[n]->SetParameter(4,X_of_peak_0p08[n]);     // peak position
      fitFcn[n]->SetParameter(5,width_2[n]);     // peak width

      Temp_Hists_1d_0p08[n]->Fit(Form("fitFcn_tau_%.1f",(n+1)/2.0),"RV+");
      Temp_Hists_1d_0p08[n]->Write();
      Double_t fit_par[6];
      fitFcn[n]->GetParameters(fit_par);

      signalFcn[n]->SetParameters(&fit_par[3]);
      
      peak_area_0p08.push_back(signalFcn[n]->Integral(-10,10));
      height_0p08.push_back(fit_par[3]);
      width_0p08.push_back(fit_par[5]);
      chi2_0p08.push_back(fitFcn[n]->GetChisquare());
   }

   fout->cd();
   gDirectory->cd("Bullet_0p08/Quantities");

   //TCanvas *c_pos_0p08 = new TCanvas("Bullet Position","Bullet Position",800,600);
   //TCanvas *c_height_0p08 = new TCanvas("Size of peak","Size of peak",800,600);
   //TCanvas *c_width_0p08 = new TCanvas("Width of peak","Width of peak",800,600);
   TH1D* h_temp_position_0p08 = new TH1D("Bullet position", "Bullet position",20,0,10);
   TH1D* h_temp_height_0p08   = new TH1D("Bullet intensity", "Bullet intensity",20,0,10);
   TH1D* h_temp_width_0p08    = new TH1D("Bullet width", "Bullet width",20,0,10);
   TH1D* h_temp_peak_0p08     = new TH1D("Area of peak", "Area of peak",20,0,10);
   TH1D* h_temp_chi2_0p08     = new TH1D("Chi2", "Chi2",20,0,10);

   for(int i = 0; i <= 20; i++){

      h_temp_position_0p08->SetBinContent(i+1, X_of_peak_0p08[i]);
      h_temp_height_0p08  ->SetBinContent(i+1, height_0p08[i]);
      h_temp_width_0p08   ->SetBinContent(i+1, width_0p08[i]);
      h_temp_peak_0p08    ->SetBinContent(i+1, peak_area_0p08[i]);
      h_temp_chi2_0p08    ->SetBinContent(i+1, chi2_0p08[i]);

   }

   h_temp_position_0p08->Write();
   h_temp_height_0p08->Write();
   h_temp_width_0p08->Write();
   h_temp_peak_0p08->Write();
   h_temp_chi2_0p08->Write();


//----------------------------------------------------------------------------------------------------/
//Start fitting bullet ideal

   vector<Double_t> height_ideal; 
   vector<Double_t> width_ideal; 
   vector<Double_t> peak_area_ideal;
   vector<Double_t> chi2_ideal;
   
   fout->cd();
   gDirectory->cd("Bullet_Ideal/Fit_Temp_Ideal");

   for(int n = 0; n < 20; n++){

      backFcn[n] = new TF1(Form("backFcn_tau_%.1f",(n+1)/2.0),background,-10,10,3);
      backFcn[n]->SetLineColor(kRed);

      signalFcn[n] = new TF1(Form("signalFcn_tau_%.1f",(n+1)/2.0),GaussianPeak,-10,10,3);
      signalFcn[n]->SetLineColor(kBlue);
      signalFcn[n]->SetNpx(1000);

      fitFcn[n] = new TF1(Form("fitFcn_tau_%.1f",(n+1)/2.0),fitFunction,-10,10,6);
      fitFcn[n]->SetNpx(500);
      fitFcn[n]->SetLineWidth(4);
      fitFcn[n]->SetLineColor(kMagenta);

      backFcn[n]->SetParameters(1,1,1);
      Temp_Hists_1d_ideal[n]->Fit(Form("backFcn_tau_%.1f",(n+1)/2.0),"N0R");
      Double_t bg_par[3];
      backFcn[n]->GetParameters(bg_par);

      fitFcn[n]->FixParameter(0,bg_par[0]);
      fitFcn[n]->FixParameter(1,bg_par[1]);
      fitFcn[n]->FixParameter(2,bg_par[2]);
      fitFcn[n]->SetParameter(3,height_2[n]);     // peak height
      fitFcn[n]->SetParameter(4,X_of_peak_ideal[n]);     // peak position
      fitFcn[n]->SetParameter(5,width_2[n]);     // peak width

      Temp_Hists_1d_ideal[n]->Fit(Form("fitFcn_tau_%.1f",(n+1)/2.0),"RV+");
      Temp_Hists_1d_ideal[n]->Write();
      Double_t fit_par[6];
      fitFcn[n]->GetParameters(fit_par);

      signalFcn[n]->SetParameters(&fit_par[3]);
      
      peak_area_ideal.push_back(signalFcn[n]->Integral(-10,10));
      height_ideal.push_back(fit_par[3]);
      width_ideal.push_back(fit_par[5]);
      chi2_ideal.push_back(fitFcn[n]->GetChisquare());
   }

   fout->cd();
   gDirectory->cd("Bullet_Ideal/Quantities");

   //TCanvas *c_pos_0p08 = new TCanvas("Bullet Position","Bullet Position",800,600);
   //TCanvas *c_height_0p08 = new TCanvas("Size of peak","Size of peak",800,600);
   //TCanvas *c_width_0p08 = new TCanvas("Width of peak","Width of peak",800,600);
   TH1D* h_temp_position_ideal = new TH1D("Bullet position", "Bullet position",20,0,10);
   TH1D* h_temp_height_ideal   = new TH1D("Bullet intensity", "Bullet intensity",20,0,10);
   TH1D* h_temp_width_ideal    = new TH1D("Bullet width", "Bullet width",20,0,10);
   TH1D* h_temp_peak_ideal     = new TH1D("Area of peak", "Area of peak",20,0,10);
   TH1D* h_temp_chi2_ideal     = new TH1D("Chi2", "Chi2",20,0,10);

   for(int i = 0; i <= 20; i++){

      h_temp_position_ideal->SetBinContent(i+1, X_of_peak_ideal[i]);
      h_temp_height_ideal  ->SetBinContent(i+1, height_ideal[i]);
      h_temp_width_ideal   ->SetBinContent(i+1, width_ideal[i]);
      h_temp_peak_ideal   ->SetBinContent(i+1, peak_area_ideal[i]);
      h_temp_chi2_ideal   ->SetBinContent(i+1, chi2_ideal[i]);

   }

   h_temp_position_ideal->Write();
   h_temp_height_ideal->Write();
   h_temp_width_ideal->Write();
   h_temp_peak_ideal->Write();
   h_temp_chi2_ideal->Write();

   fout->Close();





   
/*
   c_pos_0p08->cd();
   c_pos_0p08->SetGrid();
   h_temp_position_0p08->GetXaxis()->SetTitle("#tau");
   h_temp_position_0p08->GetYaxis()->SetTitle("x (fm)");   
   h_temp_position_0p08->GetXaxis()->SetNdivisions(20);
   h_temp_position_0p08->Draw("L");
   
   c_height_0p08->cd();
   c_height_0p08->SetGrid();
   h_temp_height_0p08->GetXaxis()->SetTitle("#tau");
   h_temp_height_0p08->GetYaxis()->SetTitle("x (fm)");   
   h_temp_height_0p08->GetXaxis()->SetNdivisions(20);
   h_temp_height_0p08->Draw("L");

   c_width_0p08->cd();
   c_width_0p08->SetGrid();
   h_temp_width_0p08->GetXaxis()->SetTitle("#tau");
   h_temp_width_0p08->GetYaxis()->SetTitle("x (fm)");   
   h_temp_width_0p08->GetXaxis()->SetNdivisions(20);
   h_temp_width_0p08->Draw("L");

   c_pos_0p08->Close();
   c_height_0p08->Close();
   c_width_0p08->Close();
*/
/*

//----------------------------------------------------------------------------------------------------/
//Here, what I used to fill the vectors for initial guess--- it's working

   int k = 18;


   TF1 *backFcn = new TF1("backFcn",background,-10,10,3);
   backFcn->SetLineColor(kRed);

   TF1 *signalFcn = new TF1("signalFcn",GaussianPeak,1.3,2.5,3);
   signalFcn->SetLineColor(kBlue);
   signalFcn->SetNpx(1000);

   TF1* fitFcn = new TF1("fitFcn",fitFunction,-10,10,6);
   fitFcn->SetNpx(500);
   fitFcn->SetLineWidth(4);
   fitFcn->SetLineColor(kMagenta);

   backFcn->SetParameters(1,1,1);
   Temp_Hists_1d_0p08[k]->Fit("backFcn","N0R");
   Double_t bg_par[3];
   backFcn->GetParameters(bg_par);

   fitFcn->FixParameter(0,bg_par[0]);
   fitFcn->FixParameter(1,bg_par[1]);
   fitFcn->FixParameter(2,bg_par[2]);
   fitFcn->SetParameter(3,0.0016);     // peak height
   fitFcn->SetParameter(4, 6);     // peak position
   fitFcn->SetParameter(5,0.37);     // peak width

   Temp_Hists_1d_0p08[k]->Fit("fitFcn","RV+");
   Double_t fit_par[6];
   fitFcn->GetParameters(fit_par);

   signalFcn->SetParameters(&fit_par[3]);
   signalFcn->Draw("same");
   



   cout << "Peak height is: " << fit_par[3] << endl;
   cout << "Peak position is: " << fit_par[4] << endl;
   cout << "Peak width is: " << fit_par[5] << endl;

   Double_t height_2[] = {
                            0.0764625,  0.078435, 0.0639601, 0.0496677, 0.0377769, 
                            0.0280522, 0.0210142, 0.0163627, 0.0134688, 0.0116679,
                            0.010234,  0.00911255,0.00814229,0.00719683,0.0063033,
                            0.00533489, 0.00391456, 0.00241549, 0.00162913,0.00111349

   };

   Double_t pos_2[] = {
                            0.371313, 0.708189, 1.11023, 1.49359, 1.86612, 
                            2.24545,  2.60304,  2.93751, 3.24821, 3.54431,
                            3.83079,  4.11299,  4.39092, 4.67349, 4.9433,
                            5.21244, 5.46123, 5.71929, 5.99688, 6.26981


   };
   Double_t width_2[] = {
                            0.239464,0.242097, 0.236319, 0.245658, 0.254865, 
                            0.266703, 0.284704, 0.315575,   0.355646, 0.400506, 
                             0.44986, 0.488862, 0.528995,   0.562785, 0.569011,
                             0.560542, 0.500156, 0.418805, 0.378352, 0.325782


   };


   TCanvas *c_pos = new TCanvas("Bullet Position","Bullet Position",800,600);
   TCanvas *c_height = new TCanvas("Size of peak","Size of peak",800,600);
   TCanvas *c_width = new TCanvas("Width of peak","Width of peak",800,600);

   TH1D* h_temp_position = new TH1D("Bullet position", "Bullet position",20,0,10);
   TH1D* h_temp_height = new TH1D("Bullet height", "Bullet height",20,0,10);
   TH1D* h_temp_width = new TH1D("Bullet width", "Bullet width",20,0,10);

   for(int i = 0; i <= 20; i++){

      h_temp_position->SetBinContent(i+1, pos_2[i]);
      h_temp_height->SetBinContent(i+1, height_2[i]);
      h_temp_width->SetBinContent(i+1, width_2[i]);

   }
   c_pos->cd();
   c_pos->SetGrid();
   h_temp_position->GetXaxis()->SetTitle("#tau");
   h_temp_position->GetYaxis()->SetTitle("x (fm)");   
   h_temp_position->GetXaxis()->SetNdivisions(20);
   h_temp_position->Draw("L");
   
   c_height->cd();
   c_height->SetGrid();
   h_temp_height->GetXaxis()->SetTitle("#tau");
   h_temp_height->GetYaxis()->SetTitle("x (fm)");   
   h_temp_height->GetXaxis()->SetNdivisions(20);
   h_temp_height->Draw("L");

   c_width->cd();
   c_width->SetGrid();
   h_temp_width->GetXaxis()->SetTitle("#tau");
   h_temp_width->GetYaxis()->SetTitle("x (fm)");   
   h_temp_width->GetXaxis()->SetNdivisions(20);
   h_temp_width->Draw("L");

c_pos->Close();
c_height->Close();
c_width->Close();
*/
   
//----------------------------------------------------------------------------------------------------/
//Down here, other stuff --- Do not use
/*
   Double_t height[] = {
                        0.0802609, 0.0639601, 0.0525595, 0.040159,  0.0297552,
                        0.0222575, 0.0173595, 0.0144485, 0.0126114, 0.0112259, 
                        0.0100818, 0.00913595,0.00834852,0.00777058,0.00711989,
                        0.00686769,0.00528364,0.00405942,0.00367739,
                        };

   Double_t pos[] = {
                     0.366192, 0.0639601, 1.49495, 1.86673, 2.24505, 
                     2.59902,  2.93223,   3.24259, 3.53678, 3.82559, 
                     4.10493,  4.38451,   4.66343, 4.9405,  5.22703,
                     5.51917,  5.79177,   6.1015,  6.48393, 
                     
                     };

   Double_t width[] = {
      
                       -0.2576159,-0.236265,-0.260885,-0.273595, 0.288483, 
                       -0.315076 , 0.352881, 0.403354,-0.460652,-0.522063, 
                       -0.590133, -0.659603,-0.721815,-0.773921,-0.826074,
                       -0.902238, -0.831426,-0.810563,-0.885315, 
                       };

*/

/*
   backFcn->SetParameters(1,1,1);
   TestHist->Fit("backFcn","0R");
   Double_t par_back[3];
   backFcn->GetParameters(par_back);

   //second try: set start values for some parameters
   fitFcn->FixParameter(0,par_back[0]);
   fitFcn->FixParameter(1,par_back[1]);
   fitFcn->FixParameter(2,par_back[2]);
   fitFcn->SetParameter(3,0.5);     // peak height
   fitFcn->SetParameter(4,0.6);     // peak position
   fitFcn->SetParameter(5,0.5);     // peak width
   TestHist->Fit("fitFcn","RV+");
   Double_t par[6];
   fitFcn->GetParameters(par);

   signalFcn->SetParameters(&par[3]);
   signalFcn->Draw("same");

//----------------------------------------------------------------------------------------------------/
   // draw the legend
   TLegend *legend=new TLegend(0.6,0.65,0.88,0.85);
   legend->SetTextFont(72);
   legend->SetTextSize(0.04);
   legend->AddEntry(TestHist,"Data","lpe");
   legend->AddEntry(backFcn,"Background fit","l");
   legend->AddEntry(signalFcn,"Signal fit","l");
   legend->AddEntry(fitFcn,"Global Fit","l");
   legend->Draw();
   

*/

}
