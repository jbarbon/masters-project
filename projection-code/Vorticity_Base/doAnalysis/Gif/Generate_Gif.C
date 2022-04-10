template<typename T2 >
void F_Generate_Gifs_th2(T2 hist, TString Name, TString path, TFile* f_in, 
                        TString draw_opt, TString Zaxis_Name, Double_t z_min, Double_t z_max, char Palette, Int_t NumberHist){

    Int_t gifcnt = 0; // Gif counter


        TCanvas* c = new TCanvas("c", "c", 700, 500);
        //c->SetGrid();
        gStyle->SetOptStat(0);

    // Start loop to create th2 gifs
    for(Int_t i = 0; i < NumberHist; i++){
        
        // Get histogram from path
        hist[i] = (TH2D*) f_in->Get(Form(path,(i+1)/4.0));
        
        //Draw histogram and set configs
        hist[i]->Draw(draw_opt);
        hist[i]->GetZaxis()->SetTitle(Zaxis_Name);
        hist[i]->GetZaxis()->SetRangeUser(z_min, z_max);
        gStyle->SetPalette(Palette);


        if(draw_opt == "surf3 FB")
        {
            hist[i]->GetXaxis()->SetRangeUser(-5,5);
            hist[i]->GetYaxis()->SetRangeUser(-5,5);
        }
    
        TString OG_Title = hist[i]->GetTitle();
        TString Tau_Title = Form(". Time #tau = %.2f", (i+1)/4.0); 
        hist[i]->SetTitle(OG_Title + Tau_Title);

        c->Update();
        char gif[24];
        sprintf(gif, Name+"_%.2d.gif", gifcnt++);
        c->SaveAs(gif);
    }

    gSystem->Exec("gifsicle --delay=60 --loop=0 " + Name + "*.gif > Final_" + Name + ".gif");
    gSystem->Exec("rm " + Name + "*.gif" );

}

template<typename T1>
void F_Generate_Gifs_th1(T1 hist, TString Name, TString path, TFile* f_in, 
                         TString draw_opt, TString Yaxis_Name, Double_t y_min, Double_t y_max, Int_t NumberHist){

    Int_t gifcnt = 0;

    TCanvas* c = new TCanvas("c", "c", 700, 500);
    //c->SetGrid();
    gStyle->SetOptStat(0);


    for(int i = 0; i < NumberHist; i++){
        hist[i] = (TH1D*) f_in->Get(Form(path,(i+1)/4.0));
        hist[i]->Draw(draw_opt);
        hist[i]->GetYaxis()->SetTitle(Yaxis_Name);
        hist[i]->GetYaxis()->SetRangeUser(y_min, y_max);


        TString OG_Title = hist[i]->GetTitle();
        TString Tau_Title = Form(". Time #tau = %.2f", (i+1)/4.0); 
        hist[i]->SetTitle(OG_Title + Tau_Title);


        c->Update();
        char gif[24];
        sprintf(gif, Name+"_%.2d.gif", gifcnt++);
        c->SaveAs(gif);
        c->Close();
    }

    gSystem->Exec("gifsicle --delay=60 --loop=0 " + Name + "*.gif > Final_" + Name + ".gif");
    gSystem->Exec("rm " + Name + "*.gif" );

}

void Generate_Gif(TString root_input){

    TFile* f_in = TFile::Open(root_input, "READ");

    gDirectory->cd("Bullet+BG/Energy/Proj_1D_x");
    Int_t NumberOfHistograms = gDirectory->GetNkeys();

    //Declare objects


    TH1D* Energy_x[NumberOfHistograms];
    TH2D* Energy_xy[NumberOfHistograms];
    TH2D* Energy_BG_xy[NumberOfHistograms];
    TH1D* Vorticity_x[NumberOfHistograms];
    TH1D* Vorticity_y[NumberOfHistograms];
    TH2D* Vorticity_xy[NumberOfHistograms];

    F_Generate_Gifs_th1(Energy_x,    "energy_x",        "Bullet+BG/Energy/Proj_1D_x/Energy_tau_%.2f", f_in, "" , "Energy (GeV)", -0.1, 150.0, NumberOfHistograms );
    F_Generate_Gifs_th2(Energy_xy,   "energy_xy",       "Bullet+BG/Energy/Proj_1D_yx/Energy_tau_%.2f", f_in, "colz", "", -5, 50.0 , kLightTemperature, NumberOfHistograms);
    //F_Generate_Gifs_th2(Energy_BG_xy,"energy_bg_xy",    "Bullet+BG/Energy/Proj_1D_yx/Energy_tau_%.2f", f_in, "colz" , "", -5, 220.0 , kLightTemperature, NumberOfHistograms);
    F_Generate_Gifs_th2(Vorticity_xy,"vorticity_xy",    "Bullet+BG/Vorticity/Proj_1D_yx/Vorticity_tau_%.2f", f_in, "surf3 FB", "", -0.1, 0.1, kLightTemperature, NumberOfHistograms);
    F_Generate_Gifs_th1(Vorticity_x, "vorticity_1d_x",  "Bullet+BG/Vorticity/Proj_1D_x/Vorticity_tau_%.2f", f_in, "" , "Vorticity (c/fm)", -0.1, 2.0, NumberOfHistograms);
    F_Generate_Gifs_th1(Vorticity_y, "vorticity_1d_y",  "Bullet+BG/Vorticity/Proj_1D_y/Vorticity_tau_%.2f", f_in, "", "Vorticity (c/fm)", -3.5, 3.5, NumberOfHistograms);



}
