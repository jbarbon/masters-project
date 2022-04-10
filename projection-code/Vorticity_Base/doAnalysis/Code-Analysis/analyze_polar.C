//This code analyze the trento background vorticity TH2 projection YZ and transfers it into polar coordinates

void analyze_polar(TString inputFile){

    TFile* f_in = TFile::Open(inputFile, "READ");

    Int_t n_taus = 23;

    TH2D* h2[n_taus];
    
    for( int i = 0; i < n_taus; i++){
        h2[i] = (TH2D*) f_in->Get(Form("Background/Vort/Proj_2D/Vort_tau_%.2f", (i+1) / 4.0  )) ;
    }



    Int_t n_bins_x = h2[0]->GetXaxis()->GetNbins(); cout << "X bins " << n_bins_x << endl;
    Int_t n_bins_y = h2[0]->GetYaxis()->GetNbins(); cout << "Y bins " << n_bins_y << endl;

    Double_t x[n_bins_x];
    Double_t y[n_bins_y];
    Double_t phi[n_bins_y];

    for(int jx = 0; jx < n_bins_x; jx++){
        x[jx] = h2[0]->GetXaxis()->GetBinCenter(jx+1);
    }

    for(int jy = 0; jy < n_bins_y; jy++){
        y[jy] = h2[0]->GetXaxis()->GetBinCenter(jy+1);
    }

    Double_t phi[] = Atan2(y/x)

    omega = GetBinContent(binx, biny,)

    //cout << h2[0]->GetXaxis
    


    h2[20]->Draw("colz");










































}