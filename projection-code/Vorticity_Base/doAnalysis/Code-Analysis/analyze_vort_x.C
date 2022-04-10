#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
#include "TLegend.h"
#include "TCanvas.h"

//Tuple that contain 
tuple <Double_t*, Double_t*,Double_t*, Double_t*, Double_t*, Double_t* , Double_t* >  Analyze(TFile* f, Int_t n_entries_tau_cuts){

   //Declaration of variables
   Int_t nbins;
   TH1D* hists_1d[n_entries_tau_cuts];
   Double_t* Vector_of_taus = new Double_t[n_entries_tau_cuts];

   //TH1D* hists_positive = (TH1D*) hists_1d->Clone("hists_positive");
   //hists_positive->Reset("ICES");


   for(int i = 0; i < n_entries_tau_cuts; i++){
      hists_1d[i] = (TH1D*) f->Get(Form("BulletOnly/Vort/Proj_1D_x/Vort_tau_%.2f", (i+1) / 4.0));
      Vector_of_taus[i] = (i+1) / 4.0;
   }

   nbins = hists_1d[0]->GetNbinsX();


   Int_t Bin_of_peak[n_entries_tau_cuts];

   Double_t* X_of_peak            = new Double_t[n_entries_tau_cuts];
   Double_t* X_of_peak_error      = new Double_t[n_entries_tau_cuts];

   Double_t* Height_of_peak       = new Double_t[n_entries_tau_cuts];
   Double_t* Height_of_peak_error = new Double_t[n_entries_tau_cuts];
   
   Double_t* RMS_of_peak          = new Double_t[n_entries_tau_cuts];


   Double_t* Integral_peak        = new Double_t[n_entries_tau_cuts];
   
   //For loop to get the peak of all histograms
   for(int index = 0; index < n_entries_tau_cuts; index++){
      Bin_of_peak[index]          = hists_1d[index]->GetMaximumBin();

      X_of_peak[index]            = hists_1d[index]->GetBinCenter(Bin_of_peak[index]);
      X_of_peak_error[index]      = hists_1d[index]->GetBinError(Bin_of_peak[index]);

      Height_of_peak[index]       = hists_1d[index]->GetBinContent(Bin_of_peak[index]);
      Height_of_peak_error[index] = hists_1d[index]->GetBinError(Bin_of_peak[index]);

      Integral_peak[index]        = hists_1d[index]->Integral(1.0,nbins);

      RMS_of_peak[index]          = hists_1d[index]->GetRMS();
   }

   return make_tuple( Vector_of_taus, X_of_peak, X_of_peak_error, Height_of_peak, Height_of_peak_error, Integral_peak, RMS_of_peak);

}



void analyze_vort_x(){
//----------------------------------------------------------------------------------------------------/

//Open File
   //TFile* f_in = TFile::Open(lInput_File, "READ");
   //TFile* f_in = TFile::Open("./Chun_BulletEta_Ideal_0p08_Proj_1D_2D_Vorticity_Temperature_Ux_v2.root", "READ");
   TFile* f_in_0p08 = TFile::Open("/home/joao/jbarbon/projects/Vorticity/Vorticity3D_Dev/Plots/Projections_Chun_Code/Bullets-data5/Results/Run3/HB_R-.5_EtaOverS-0.08_Proj_v3.root");
   TFile* f_in_0p01 = TFile::Open("/home/joao/jbarbon/projects/Vorticity/Vorticity3D_Dev/Plots/Projections_Chun_Code/Bullets-data5/Results/Run3/HB_R-.5_EtaOverS-0.01_Proj_v3.root");
   TFile* f_in_0p001 = TFile::Open("/home/joao/jbarbon/projects/Vorticity/Vorticity3D_Dev/Plots/Projections_Chun_Code/Bullets-data5/Results/Run3/HB_R-.5_EtaOverS-0.001_Proj_v3.root");

//----------------------------------------------------------------------------------------------------/
//Declare and initialize histograms
   Int_t Number_Entries_tau_cuts = 60;

   Double_t* Vector_of_taus      = get<0>(Analyze(f_in_0p08, Number_Entries_tau_cuts) );
   
   Double_t* X_of_peak_0p08             = get<1>(Analyze(f_in_0p08, Number_Entries_tau_cuts) );
   Double_t* X_of_peak_error_0p08       = get<2>(Analyze(f_in_0p08, Number_Entries_tau_cuts) );
   Double_t* height_of_peak_0p08        = get<3>(Analyze(f_in_0p08, Number_Entries_tau_cuts) );
   Double_t* height_of_peak_error_0p08  = get<4>(Analyze(f_in_0p08, Number_Entries_tau_cuts) );
   Double_t* area_of_peak_0p08          = get<5>(Analyze(f_in_0p08, Number_Entries_tau_cuts) );
   Double_t* RMS_of_peak_0p08           = get<6>(Analyze(f_in_0p08, Number_Entries_tau_cuts) );

   Double_t* X_of_peak_0p01             = get<1>(Analyze(f_in_0p01, Number_Entries_tau_cuts) );
   Double_t* X_of_peak_error_0p01       = get<2>(Analyze(f_in_0p01, Number_Entries_tau_cuts) );
   Double_t* height_of_peak_0p01        = get<3>(Analyze(f_in_0p01, Number_Entries_tau_cuts) );
   Double_t* height_of_peak_error_0p01  = get<4>(Analyze(f_in_0p01, Number_Entries_tau_cuts) );
   Double_t* area_of_peak_0p01          = get<5>(Analyze(f_in_0p01, Number_Entries_tau_cuts) );
   Double_t* RMS_of_peak_0p01           = get<6>(Analyze(f_in_0p01, Number_Entries_tau_cuts) );

   Double_t* X_of_peak_0p001             = get<1>(Analyze(f_in_0p001, Number_Entries_tau_cuts) );
   Double_t* X_of_peak_error_0p001       = get<2>(Analyze(f_in_0p001, Number_Entries_tau_cuts) );
   Double_t* height_of_peak_0p001        = get<3>(Analyze(f_in_0p001, Number_Entries_tau_cuts) );
   Double_t* height_of_peak_error_0p001  = get<4>(Analyze(f_in_0p001, Number_Entries_tau_cuts) );
   Double_t* area_of_peak_0p001          = get<5>(Analyze(f_in_0p001, Number_Entries_tau_cuts) );
   Double_t* RMS_of_peak_0p001           = get<6>(Analyze(f_in_0p001, Number_Entries_tau_cuts) );

//TGraphs for x vs tau
   
   TGraphErrors* TG_X_vs_tau_0p08 = new TGraphErrors(Number_Entries_tau_cuts, Vector_of_taus, X_of_peak_0p08, 0, 0 );

   TG_X_vs_tau_0p08->SetMarkerStyle(8);
   TG_X_vs_tau_0p08->SetMarkerSize(2);
   TG_X_vs_tau_0p08->SetLineWidth(2);
   TG_X_vs_tau_0p08->SetLineColor(kGreen);
   TG_X_vs_tau_0p08->SetMarkerColor(kGreen);

   TGraphErrors* TG_X_vs_tau_0p01 = new TGraphErrors(Number_Entries_tau_cuts, Vector_of_taus, X_of_peak_0p01, 0, 0 );
   TG_X_vs_tau_0p01->SetMarkerStyle(8);
   TG_X_vs_tau_0p01->SetMarkerSize(2);
   TG_X_vs_tau_0p01->SetLineWidth(2);
   TG_X_vs_tau_0p01->SetLineColor(kOrange+7);
   TG_X_vs_tau_0p01->SetMarkerColor(kOrange+7);
   
   TGraphErrors* TG_X_vs_tau_0p001 = new TGraphErrors(Number_Entries_tau_cuts, Vector_of_taus, X_of_peak_0p001, 0, 0 );
   TG_X_vs_tau_0p001->SetMarkerStyle(8);
   TG_X_vs_tau_0p001->SetMarkerSize(2);
   TG_X_vs_tau_0p001->SetLineWidth(2);
   TG_X_vs_tau_0p001->SetLineColor(kBlue);
   TG_X_vs_tau_0p001->SetMarkerColor(kBlue   );



//TGraphs for height vs tau
   
   TGraphErrors* TG_height_vs_tau_0p08 = new TGraphErrors(Number_Entries_tau_cuts, Vector_of_taus, height_of_peak_0p08, 0, 0 );

   TG_height_vs_tau_0p08->SetMarkerStyle(8);
   TG_height_vs_tau_0p08->SetMarkerSize(2);
   TG_height_vs_tau_0p08->SetLineWidth(2);
   TG_height_vs_tau_0p08->SetLineColor(kGreen);
   TG_height_vs_tau_0p08->SetMarkerColor(kGreen);

   TGraphErrors* TG_height_vs_tau_0p01 = new TGraphErrors(Number_Entries_tau_cuts, Vector_of_taus, height_of_peak_0p01, 0, 0 );
   TG_height_vs_tau_0p01->SetMarkerStyle(8);
   TG_height_vs_tau_0p01->SetMarkerSize(2);
   TG_height_vs_tau_0p01->SetLineWidth(2);
   TG_height_vs_tau_0p01->SetLineColor(kOrange+7);
   TG_height_vs_tau_0p01->SetMarkerColor(kOrange+7);
   
   TGraphErrors* TG_height_vs_tau_0p001 = new TGraphErrors(Number_Entries_tau_cuts, Vector_of_taus, height_of_peak_0p001, 0, 0 );
   TG_height_vs_tau_0p001->SetMarkerStyle(8);
   TG_height_vs_tau_0p001->SetMarkerSize(2);
   TG_height_vs_tau_0p001->SetLineWidth(2);
   TG_height_vs_tau_0p001->SetLineColor(kBlue);
   TG_height_vs_tau_0p001->SetMarkerColor(kBlue);



//TGraphs for area vs tau
   
   TGraphErrors* TG_area_vs_tau_0p08 = new TGraphErrors(Number_Entries_tau_cuts, Vector_of_taus, area_of_peak_0p08, 0, 0 );
   TG_area_vs_tau_0p08->SetTitle("Vorticity Peak - area vs tau");
   TG_area_vs_tau_0p08->GetXaxis()->SetTitle("#tau");
   TG_area_vs_tau_0p08->GetYaxis()->SetTitle("height (fm)");

   TG_area_vs_tau_0p08->SetMarkerStyle(8);
   TG_area_vs_tau_0p08->SetMarkerSize(2);
   TG_area_vs_tau_0p08->SetLineWidth(2);
   TG_area_vs_tau_0p08->SetLineColor(kGreen);
   TG_area_vs_tau_0p08->SetMarkerColor(kGreen);

   TGraphErrors* TG_area_vs_tau_0p01 = new TGraphErrors(Number_Entries_tau_cuts, Vector_of_taus, area_of_peak_0p01, 0, 0 );
   TG_area_vs_tau_0p01->SetMarkerStyle(8);
   TG_area_vs_tau_0p01->SetMarkerSize(2);
   TG_area_vs_tau_0p01->SetLineWidth(2);
   TG_area_vs_tau_0p01->SetLineColor(kOrange+7);
   TG_area_vs_tau_0p01->SetMarkerColor(kOrange+7);
   
   TGraphErrors* TG_area_vs_tau_0p001 = new TGraphErrors(Number_Entries_tau_cuts, Vector_of_taus, area_of_peak_0p001, 0,0 );
   TG_area_vs_tau_0p001->SetMarkerStyle(8);
   TG_area_vs_tau_0p001->SetMarkerSize(2);
   TG_area_vs_tau_0p001->SetLineWidth(2);
   TG_area_vs_tau_0p001->SetLineColor(kBlue);
   TG_area_vs_tau_0p001->SetMarkerColor(kBlue);

//TGraphs for RMS vs tau
   
   TGraphErrors* TG_RMS_vs_tau_0p08 = new TGraphErrors(Number_Entries_tau_cuts, Vector_of_taus, RMS_of_peak_0p08, 0, 0 );
   TG_RMS_vs_tau_0p08->SetTitle("Vorticity Peak - RMS vs tau");
   TG_RMS_vs_tau_0p08->GetXaxis()->SetTitle("#tau");
   TG_RMS_vs_tau_0p08->GetYaxis()->SetTitle("RMS (fm)");

   TG_RMS_vs_tau_0p08->SetMarkerStyle(8);
   TG_RMS_vs_tau_0p08->SetMarkerSize(2);
   TG_RMS_vs_tau_0p08->SetLineWidth(2);
   TG_RMS_vs_tau_0p08->SetLineColor(kGreen);
   TG_RMS_vs_tau_0p08->SetMarkerColor(kGreen);

   TGraphErrors* TG_RMS_vs_tau_0p01 = new TGraphErrors(Number_Entries_tau_cuts, Vector_of_taus, RMS_of_peak_0p01, 0, 0 );
   TG_RMS_vs_tau_0p01->SetMarkerStyle(8);
   TG_RMS_vs_tau_0p01->SetMarkerSize(2);
   TG_RMS_vs_tau_0p01->SetLineWidth(2);
   TG_RMS_vs_tau_0p01->SetLineColor(kOrange+7);
   TG_RMS_vs_tau_0p01->SetMarkerColor(kOrange+7);
   
   TGraphErrors* TG_RMS_vs_tau_0p001 = new TGraphErrors(Number_Entries_tau_cuts, Vector_of_taus, RMS_of_peak_0p001, 0,0 );
   TG_RMS_vs_tau_0p001->SetMarkerStyle(8);
   TG_RMS_vs_tau_0p001->SetMarkerSize(2);
   TG_RMS_vs_tau_0p001->SetLineWidth(2);
   TG_RMS_vs_tau_0p001->SetLineColor(kBlue);
   TG_RMS_vs_tau_0p001->SetMarkerColor(kBlue);


//Multigraph Draw
   // draw the legend
   TLegend *legend=new TLegend(0.6,0.65,0.88,0.85);
   legend->SetTextFont(72);
   legend->SetTextSize(0.04);
   legend->AddEntry(TG_area_vs_tau_0p08,"Bullet #eta/S 0.08","l");
   legend->AddEntry(TG_area_vs_tau_0p01,"Bullet #eta/S 0.01","l");
   legend->AddEntry(TG_area_vs_tau_0p001,"Bullet #eta/S 0.001","l");
   


   TCanvas* c_area_tau = new TCanvas("Graph Area of Vort Peak vs tau", "Graph Area of Vort Peak vs tau",800,600);
   c_area_tau->SetGrid();
   TMultiGraph *MG_area_vs_tau = new TMultiGraph();
   MG_area_vs_tau->Add(TG_area_vs_tau_0p08);
   MG_area_vs_tau->Add(TG_area_vs_tau_0p01);
   MG_area_vs_tau->Add(TG_area_vs_tau_0p001);

   MG_area_vs_tau->SetTitle("Vorticity Peak - area vs tau");
   MG_area_vs_tau->GetXaxis()->SetTitle("#tau");
   MG_area_vs_tau->GetYaxis()->SetTitle("area (fm*(fm/c)^{-1})");
   MG_area_vs_tau->Draw("a");
   //MG_area_vs_tau->GetXaxis()->SetRangeUser(0.0,14.1);
   legend->Draw("same");


   TCanvas* c_height_tau = new TCanvas("Graph Height of Vort Peak vs tau", "Graph Height of Vort Peak vs tau",800,600);
   c_height_tau->SetGrid();
   TMultiGraph *MG_height_vs_tau = new TMultiGraph();
   MG_height_vs_tau->Add(TG_height_vs_tau_0p08);
   MG_height_vs_tau->Add(TG_height_vs_tau_0p01);
   MG_height_vs_tau->Add(TG_height_vs_tau_0p001);

   MG_height_vs_tau->SetTitle("Height of Vort Peak - height vs tau");
   MG_height_vs_tau->GetXaxis()->SetTitle("#tau");
   MG_height_vs_tau->GetYaxis()->SetTitle("height (fm)");
   MG_height_vs_tau->Draw("a");
   //MG_height_vs_tau->GetXaxis()->SetRangeUser(0.0,14.1);;
   legend->Draw("same");

   TCanvas* c_RMS_tau = new TCanvas("Graph RMS vs tau", "Graph RMS vs tau",800,600);
   c_RMS_tau->SetGrid();
   TMultiGraph *MG_RMS_vs_tau = new TMultiGraph();
   MG_RMS_vs_tau->Add(TG_RMS_vs_tau_0p08);
   MG_RMS_vs_tau->Add(TG_RMS_vs_tau_0p01);
   MG_RMS_vs_tau->Add(TG_RMS_vs_tau_0p001);

   MG_RMS_vs_tau->SetTitle("RMS of Vort Peak - RMS vs tau");
   MG_RMS_vs_tau->GetXaxis()->SetTitle("#tau");
   MG_RMS_vs_tau->GetYaxis()->SetTitle("RMS (fm)");
   MG_RMS_vs_tau->Draw("a");
   //MG_RMS_vs_tau->GetXaxis()->SetRangeUser(0.0,14.1);;
   legend->Draw("same");


   TCanvas* c_x_tau = new TCanvas("Graph X of Vort Peak vs tau", "Graph X of Vort Peak vs tau",800,600);
   c_x_tau->SetGrid();
   TMultiGraph *MG_x_vs_tau = new TMultiGraph();
   MG_x_vs_tau->Add(TG_X_vs_tau_0p08);
   MG_x_vs_tau->Add(TG_X_vs_tau_0p01);
   MG_x_vs_tau->Add(TG_X_vs_tau_0p001);

   MG_x_vs_tau->SetTitle("X position of Vort Peak - x vs tau");
   MG_x_vs_tau->GetXaxis()->SetTitle("#tau");
   MG_x_vs_tau->GetYaxis()->SetTitle("x (fm)");
   MG_x_vs_tau->Draw("a");

   //MG_x_vs_tau->GetXaxis()->SetRangeUser(0.0,14.1);
   MG_x_vs_tau->GetYaxis()->SetRangeUser(-0.1,12.0);
   legend->Draw("same");

   delete[] X_of_peak_0p08             ;
   delete[] X_of_peak_error_0p08       ;
   delete[] height_of_peak_0p08        ;
   delete[] height_of_peak_error_0p08  ;
   delete[] area_of_peak_0p08          ;
   delete[] X_of_peak_0p01             ;
   delete[] X_of_peak_error_0p01       ;
   delete[] height_of_peak_0p01        ;
   delete[] height_of_peak_error_0p01  ;
   delete[] area_of_peak_0p01          ;
   delete[] X_of_peak_0p001            ;
   delete[] X_of_peak_error_0p001      ;
   delete[] height_of_peak_0p001       ;
   delete[] height_of_peak_error_0p001 ;
   delete[] area_of_peak_0p001         ;
 

   gSystem->Exec("mkdir ProjX_Vort");
   c_x_tau->Print("./ProjX_Vort/X_vs_tau.png");
   c_RMS_tau->Print("./ProjX_Vort/RMS_vs_tau.png");
   c_height_tau->Print("./ProjX_Vort/height_vs_tau.png");
   c_area_tau->Print("./ProjX_Vort/area_vs_tau.png");
}
