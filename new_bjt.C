
#include <iostream>
#include <cmath>
#include <string.h>
#include <TGraphErrors.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TF1.h>
#include <iomanip>
#include <TFitResultPtr.h>
#include <TFitResult.h>
#include <TLegend.h>
#include <TMultiGraph.h>
#include <TLatex.h>
#include <TMath.h>
using namespace std;


/*double mean(int n, double q[n], double qerr[n]){
  for (int i = 0; i < n; ++i ){
    double qmed += q[i] / qerr[i];
    double qerrsum += qerr[i];
  }
  return qmed / qerrsum;} // media pesata e semidispersione massima da array di double
double semidispersione(int n, double v[n]){
  return (max(n,v) - min(n,v)) / 2.;
}*/
void Ztest(double x1,double x2, double err1, double err2, double alpha = 0.05){

	double z = abs(x1 - x2) / sqrt(pow(err1, 2) + pow(err2, 2));  //definisco la variabile Z

	double prob_z;
	double pvalue_z;
	prob_z = (ROOT::Math::normal_cdf(z,1,0) - 0.5)*2;
	pvalue_z = (1-ROOT::Math::normal_cdf(z,1,0))*2;


	if(pvalue_z > alpha){
    cout 	<< "Test Z" << endl
    << "alpha = " << alpha << endl
		<< "Z = " << z << endl
		<< "P(Z) = "<< prob_z << endl
		<< "pvalue: "<< pvalue_z <<endl
		<< "pvalue > alpha => H0 NON RIFIUTATA." << endl << endl;
	}

	if(pvalue_z <= alpha){
		cout 	<< "Test Z" << endl
    << "alpha = " << alpha << endl
    << "Z = " << z << endl
		<< "P(Z) = "<< prob_z << endl
		<< "pvalue: "<< pvalue_z <<endl
		<< "pvalue < alpha, H0 RIFIUTATA" << endl << endl;
  }
}

void new_bjt(){

  //parte 1: caratteristica del transistor BJT in uscita
  
  const int n = 19; // numero di misure per ogni set
  const int nIb = 4; // numero di set di misure 
  double Ib[nIb] = {100.,200.,300.,400.}; // [uA] correnti Ib per riconoscere la caratteristica in uscita
  double Ic[nIb][n] = {{0.887, 3.469, 8.356, 15.61, 23.05, 27.62, 30.40, 31.43, 31.74, 31.91, 32.04, 32.20, 33.17, 33.92, 34.60, 35.46, 36.12, 36.95, 37.91},
                  {1.918, 7.23, 17.55, 31.24, 50.52, 56.76, 63.27, 66.10, 67.21, 67.70, 68.06, 68.40, 70.58, 72.31, 74.22, 75.68, 77.69, 80.30, 83.04},
                  {2.79, 10.11, 23.71, 42.61, 66.81, 76.15, 84.78, 89.65, 91.60, 92.61, 93.24, 93.91, 97.18, 99.6, 101.9, 103.8, 107.6, 111.1, 115.4},
                  {4.05, 13.4, 30.38, 53.15, 82.93, 96.35, 106.4, 113.4, 116.6, 118.4, 120, 121.1, 125.9, 129.5, 132.1, 134.7, 138.2, 143.3, 149.2}}; // [mA] corrente di collettore per ogni Ib
  double Vc[nIb][n] = {{0.0353, 0.0686, 0.0991, 0.1304, 0.1735, 0.1944, 0.235, 0.2815, 0.3208, 0.3603, 0.400, 0.4715, 1.204, 1.915, 2.598, 3.219, 4.022, 5.049, 6.002},
                  {0.0353, 0.068, 0.0996, 0.1295, 0.1735, 0.1951, 0.235, 0.2788, 0.3235, 0.3612, 0.4029, 0.4701, 1.199, 1.907, 2.59, 3.209, 4.024, 5.037, 5.999},
                  {0.0348, 0.068, 0.0993, 0.1307, 0.1729, 0.1969, 0.235, 0.2801, 0.32, 0.3618, 0.403, 0.47, 1.204, 1.905, 2.625, 3.2, 4.028, 5.036, 6.001},
                  {0.0366, 0.0684, 0.0995, 0.1303, 0.1724, 0.2001, 0.2352, 0.2803, 0.3203, 0.358, 0.4049, 0.47, 1.213, 1.984, 2.665, 3.182, 4.024, 5.071, 6.001}}; // [V] tensioni collettore-emettitore per ogni Ib

  //errori misure

  double Icerr[nIb][n] = {{}};
  double Vcerr[nIb][n] = {{}}; // 3 pA
  for (int j = 0; j < nIb; ++j){
    for (int i = 0; i < n; ++i){
    
    	if(Vc[j][i] < 10) Vcerr[j][i] = Vc[j][i]*0.001+0.005;   //Volt, Amprobe 37X
        if(Vc[j][i] < 1)  Vcerr[j][i] = Vc[j][i]*0.001+0.0005;
	
	if(Ic[j][i] < 100) Icerr[j][i] = 0.005*Ic[j][i]+0.05;
        if(Ic[j][i] < 10)  Icerr[j][i] = 0.005*Ic[j][i]+0.005;    //mA, corrente misurata con Amprobe 37X
        if(Ic[j][i] < 1)   Icerr[j][i] = 0.005*Ic[j][i]+0.0005;
        if(Ic[j][i] < 0.1) Icerr[j][i] = (0.005*Ic[j][i]+0.0001);
    
    }
  }

  /*-------------------------------------------------------------------------------------------------------------------------------------------*/

  //GRAFICO I-V
  TCanvas* cIV = new TCanvas("cIV", "I-V", 200, 10, 600, 400);
  cIV->SetFillColor(0);
  cIV->SetGrid();
  cIV->cd();

  TMultiGraph* mgIV = new TMultiGraph("mgIV","mgIV");
  mgIV->SetTitle("I_{c}(V_{c}) transistor BJT caratteristica in uscita");
  mgIV->GetXaxis()->SetTitle("V_{c} [V]");
  mgIV->GetYaxis()->SetTitle("I_{c} [mA]");
  mgIV->GetXaxis()->SetLimits(/*-0.2,3.*/ 0., 6.5);
  mgIV->GetYaxis()->SetRangeUser(/*-2.,31*/-0.075, 0.1);

  TGraphErrors* gIV[nIb] = {}; //array dei grafici
  const char* gIVtitle[nIb] = {"#Ib_{1} = 100 uA","#Ib_{2} = 200 uA","#Ib_{3} = 300 uA","#Ib_{4} = 400 uA"};
  const char* gIVname[nIb] = {"gIV_{1}","gIV_{2}","gIV_{3}","gIV_{4}"};


  TLegend* leg = new TLegend(0.7,0.7,0.89,0.89);
  leg->SetHeader("Iv(Vc)"); 

  for (int i = 0; i < nIb; ++i){
    cout << gIVname[i] << " Corrente di base: (" << Ib[i] << ") uA \n";

    //GRAFICO I-V
    gIV[i] = new TGraphErrors(n, Vc[i], Ic[i], Vcerr[i], Icerr[i]);
    gIV[i]->SetName(gIVname[i]);
    gIV[i]->SetTitle(gIVtitle[i]);
    gIV[i]->SetMarkerSize(0.7);
    gIV[i]->SetMarkerStyle(21+i);
    gIV[i]->SetLineColor(1+i); // serve solo per la legenda
    if (i==4) gIV[i]->SetLineColor(1 + i +1); // no fit giallo per legenda
    gIV[i]->SetDrawOption("APE");
    gIV[i]->SetFillStyle(0);


        cout << "------------------------------------------------------------------------------" << endl;

    mgIV->Add(gIV[i]);
    leg->AddEntry(gIV[i], gIVtitle[i], "ple");
  }

  mgIV->Draw("APE");
  //leg->Draw();
  cIV->BuildLegend(0.6, 0.6, 0.9, 0.9, "BJT in uscita", "ple");
  cIV->Print("IcVc_BJT_out.png");

  /*--------------------------------------------------------------------------------------------------------------------------------*/

  //parte 2: caratteristica del transistor BJT in entrata

 
}
/* BJT: data refreshed in last lab session
Strumenti:
- BJT: ACA31A TIP31A-EQV ACA HN ? 7507
- Ponte RLC: Agilent U1731A		R = 9.870 kOhm (1 kHz)
- Multimetri digitali: Amprobe 37XR-A
- Multimetro analogico: ICE SuperTester 680 RF
- Generatore doppio: thandar TS3022S
*/

#include <iostream>
#include <cmath>
#include <string.h>
#include <TGraphErrors.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TF1.h>
#include <iomanip>
#include <TFitResultPtr.h>
#include <TFitResult.h>
#include <TLegend.h>
#include <TMultiGraph.h>
#include <TLatex.h>
#include <TMath.h>
using namespace std;

// funzioni di lavoro
double_t min(int dim, double v[dim]){
  int min_index = 0;
  for (int i = 0; i < dim; ++i){
	if (v[min_index] > v[i]){
	  min_index=i;
	}
	//else continue;
  }
  double_t result = v[min_index];
  return result;}
double_t max(int dim, double v[dim]){
  int max_index = 0;
  for (int i = 0; i < dim; ++i){
	if (v[max_index] < v[i]){
	  max_index=i;
	}
	//else continue;
  }
  double_t result = v[max_index];
  return result;
}
/*double transpose(int row, int col, double mat[row][col]){
  double Tmat[col][row] = {{}};
  for (int i = 0; i < row; ++i){
    for (int j = 0; j < col; ++j){
      Tmat[j][i] = mat[i][j];
    }
  }
  return Tmat
}
*/

void new_bjt(){
	const int n = 19; // numero di misure per ogni set
  const int nIb = 4; // numero di set di misure

  // 1. Caratteristica in USCITA
  double Ic[nIb][n] = {{0.887, 3.469, 8.356, 15.61, 23.05, 27.62, 30.40, 31.43, 31.74, 31.91, 32.04, 32.20, 33.17, 33.92, 34.60, 35.46, 36.12, 36.95, 37.91},
                  		 {1.918, 7.23, 17.55, 31.24, 50.52, 56.76, 63.27, 66.10, 67.21, 67.70, 68.06, 68.40, 70.58, 72.31, 74.22, 75.68, 77.69, 80.30, 83.04},
                  		 {2.79, 10.11, 23.71, 42.61, 66.81, 76.15, 84.78, 89.65, 91.60, 92.61, 93.24, 93.91, 97.18, 99.6, 101.9, 103.8, 107.6, 111.1, 115.4},
                  		 {4.05, 13.4, 30.38, 53.15, 82.93, 96.35, 106.4, 113.4, 116.6, 118.4, 120, 121.1, 125.9, 129.5, 132.1, 134.7, 138.2, 143.3, 149.2}}; // [mA] corrente di collettore per ogni Ib
  double Vc[nIb][n] = {{0.0353, 0.0686, 0.0991, 0.1304, 0.1735, 0.1944, 0.235, 0.2815, 0.3208, 0.3603, 0.400, 0.4715, 1.204, 1.915, 2.598, 3.219, 4.022, 5.049, 6.002},
                  		 {0.0353, 0.068, 0.0996, 0.1295, 0.1735, 0.1951, 0.235, 0.2788, 0.3235, 0.3612, 0.4029, 0.4701, 1.199, 1.907, 2.59, 3.209, 4.024, 5.037, 5.999},
                  		 {0.0348, 0.068, 0.0993, 0.1307, 0.1729, 0.1969, 0.235, 0.2801, 0.32, 0.3618, 0.403, 0.47, 1.204, 1.905, 2.625, 3.2, 4.028, 5.036, 6.001},
                  		 {0.0366, 0.0684, 0.0995, 0.1303, 0.1724, 0.2001, 0.2352, 0.2803, 0.3203, 0.358, 0.4049, 0.47, 1.213, 1.984, 2.665, 3.182, 4.024, 5.071, 6.001}}; // [V] tensioni collettore-emettitore per ogni Ib

  // errori misure
  double Icerr[nIb][n] = {{}};
  double Vcerr[nIb][n] = {{}};
  for (int j = 0; j < nIb; ++j){
    for (int i = 0; i < n; ++i){
			if(Ic[j][i] < 100) Icerr[j][i] = 5e-3 * Ic[j][i] + 5e-2; // [mA], corrente misurata con Amprobe 37X
      if(Ic[j][i] < 10)  Icerr[j][i] = 5e-3 * Ic[j][i] + 5e-3;
			if(Ic[j][i] < 1)   Icerr[j][i] = 5e-3 * Ic[j][i] + 5e-4;
			if(Ic[j][i] < 0.1) Icerr[j][i] = 5e-3 * Ic[j][i] + 1e-4;

    	if(Vc[j][i] < 10) Vcerr[j][i] = Vc[j][i] * 1e-3 + 5e-3; // [V] AMPROBE 37XR-A
      if(Vc[j][i] < 1)  Vcerr[j][i] = Vc[j][i] * 1e-3 + 5e-4;
    }
  }

  //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  // GRAFICO Ic-Vc
  TCanvas* cIVout = new TCanvas("cIVout", "Ic-Vc", 800, 10, 600, 300);
  cIVout->SetFillColor(0);
  cIVout->SetGrid();
  cIVout->cd();

  TMultiGraph* mgIVout = new TMultiGraph("mgIVout","mgIVout");
  mgIVout->SetTitle("I_{c}(V_{c}) transistor BJT caratteristica in uscita");
  mgIVout->GetXaxis()->SetTitle("V_{c} [V]");
  mgIVout->GetYaxis()->SetTitle("I_{c} [mA]");
  mgIVout->GetXaxis()->SetLimits(-0.5, 9);
  mgIVout->GetYaxis()->SetRangeUser(0., 1.6e2);

  TGraphErrors* gIVout[nIb] = {}; //array dei grafici
  const char* gIVouttitle[nIb] = {"I_{b1} = (100 #pm 5) #muA","I_{b2} = (200 #pm 5) #muA","I_{b3} = (300 #pm 5) #muA","I_{b4} = (400 #pm 5) #muA"};

  for (int i = 0; i < nIb; ++i){
    gIVout[i] = new TGraphErrors(n, Vc[i], Ic[i], Vcerr[i], Icerr[i]);
    gIVout[i]->SetTitle(gIVouttitle[i]);
    gIVout[i]->SetMarkerSize(0.8);
    gIVout[i]->SetMarkerStyle(21+i);
    gIVout[i]->SetLineColor(1+i); // serve solo per la legenda
    gIVout[i]->SetFillStyle(0);
    mgIVout->Add(gIVout[i]);
  }
  mgIVout->Draw("AP");
  cIVout->BuildLegend(0.7, 0.6, 0.9, 0.9, "Correnti di base fissate", "ple");

  //======================================================================================================================================================================================================================

  // 2. Caratteristica in ENTRATA
  // T sta per trasposta
	double TIb[nIb][n] = {{100, 95, 95, 90, 90 ,90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90},
			 {210, 205, 205, 200, 200, 200 ,200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200},
			 {300, 295, 295, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290},
			 {400, 400, 395, 395, 390, 390, 390, 390, 390, 390, 390, 390, 390, 390, 390, 390, 390, 390, 390}}; // [uA] correnti di base
  double TVb[nIb][n] = {{0.4920, 0.5209, 0.5440, 0.5625, 0.5747, 0.5806, 0.5836, 0.5846, 0.5848, 0.5847, 0.5847, 0.5847, 0.5843, 0.5837, 0.5829, 0.5804, 0.5796, 0.5787, 0.5764},
		 {0.5083, 0.5407, 0.5656 ,0.5853, 0.6026, 0.6069, 0.6107, 0.6121, 0.6124, 0.6124, 0.6124, 0.6123, 0.6118, 0.6103, 0.6086, 0.6056, 0.6031, 0.5992, 0.5930},
		 {0.5137, 0.5464, 0.5730, 0.5944, 0.6123, 0.6179, 0.6219, 0.6238, 0.6244, 0.6246, 0.6246, 0.6246, 0.6237, 0.6218, 0.6195, 0.6170, 0.6105, 0.6052, 0.5984},
		 {0.5178, 0.5495, 0.5779, 0.5997, 0.6189, 0.6261, 0.6304, 0.6327, 0.6340, 0.6345, 0.6345, 0.6347, 0.6337, 0.6316, 0.6290, 0.6258, 0.6215, 0.6145, 0.6036}}; // [V] tensioni di base

  // Transpose TIb and TVb for plotting with constant Vc
  double Ib[n][nIb] = {{}};
  double Vb[n][nIb] = {{}};
  for (int i = 0; i < n; ++i){
    for (int j = 0; j < nIb; ++j){
	Ib[i][j] = TIb[j][i];
	Vb[i][j] = TVb[j][i];
    }
  }
  
  // errori misure
  double Iberr[n][nIb];
  double Vberr[n][nIb];
  for (int i = 0; i < n; ++i){
    for (int j = 0; j < nIb; ++j){
	Iberr[i][j] = 500./100.; // [uA], 1% fondo scala corrente misurata con ICE SuperTester 680 RF
	if(Vb[i][j] < 10) Vberr[i][j] = Vb[i][j] * 1e-3 + 5e-3; // [V] AMPROBE 37XR-A
	if(Vb[i][j] < 1)  Vberr[i][j] = Vb[i][j] * 1e-3 + 5e-4;
    }
  }

  //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  // GRAFICO Ib-Vb
  TCanvas* cIVin = new TCanvas("cIVin", "Ib-Vb", 800, 700, 900, 315);
  cIVin->SetFillColor(0);
  cIVin->SetGrid();
  cIVin->cd();

  TMultiGraph* mgIVin = new TMultiGraph("mgIVin","mgIVin");
  mgIVin->SetTitle("I_{b}(V_{b}) transistor BJT caratteristica in entrata");
  mgIVin->GetXaxis()->SetTitle("V_{b} [V]");
  mgIVin->GetYaxis()->SetTitle("I_{b} [#muA]");
  mgIVin->GetXaxis()->SetLimits(0.48, 0.7);
  mgIVin->GetYaxis()->SetRangeUser(80., 410.);

  TGraphErrors* gIVin[n] = {}; //array dei grafici
  const char* gIVintitle[n] =   {"V_{c1} = ( #pm ) V",
				 "V_{c2} = ( #pm ) V",
				 "V_{c3} = ( #pm ) V",
				 "V_{c4} = ( #pm ) V",
				 "V_{c5} = ( #pm ) V",
				 "V_{c6} = ( #pm ) V",
				 "V_{c7} = ( #pm ) V",
				 "V_{c8} = ( #pm ) V",
				 "V_{c9} = ( #pm ) V",
				 "V_{c10} = ( #pm ) V",
				 "V_{c11} = ( #pm ) V",
				 "V_{c12} = ( #pm ) V",
				 "V_{c13} = ( #pm ) V",
				 "V_{c14} = ( #pm ) V",
				 "V_{c15} = ( #pm ) V",
				 "V_{c16} = ( #pm ) V",
				 "V_{c17} = ( #pm ) V",
				 "V_{c18} = ( #pm ) V",
				 "V_{c19} = ( #pm ) V",};

  for (int i = 0; i < n; ++i){
    gIVin[i] = new TGraphErrors(n, Vb[i], Ib[i], Vberr[i], Iberr[i]);
    gIVin[i]->SetTitle(gIVintitle[i]);
    gIVin[i]->SetMarkerSize(0.8);
    gIVin[i]->SetMarkerStyle(20+i);
    gIVin[i]->SetLineColor(1+i); // serve solo per la legenda
    gIVin[i]->SetFillStyle(0);
    mgIVin->Add(gIVin[i]);
  }
  mgIVin->Draw("AP");
  cIVin->BuildLegend(0.7, 0.1, 0.9, 0.9, "Tensioni in uscita", "pe");
  cIVin->Print("IbVb_BJT_in.png");

  //======================================================================================================================================================================================================================

  // 3. bf(Ic) con Vc = 6 V
  double bf[nIb] = {};
  double bferr[nIb] = {};
  for (int i = 0; i < nIb; ++i){
    bf[i] = 1e3 * Ic[i][n-1] / Ib[n-1][i]; // um uniforme e array valutati in V = 6 V
    bferr[i] = 1e3 * sqrt(pow(Icerr[i][n-1] / Ib[n-1][i], 2) + pow(Ic[i][n-1] * Iberr[n-1][i] / pow(Ib[n-1][i], 2), 2));
  }

  // Transpose Ic
  double TIc[n][nIb] = {{}};
  double TIcerr[n][nIb] = {{}};
  for (int i = 0; i < n; ++i){
    for (int j = 0; j < nIb; ++j){
  	TIc[i][j] = Ic[j][i];
	TIcerr[i][j] = Icerr[j][i];
    }
  }

  TCanvas* cbfIc = new TCanvas("cbfIc", "bf-Ic", 50, 50, 450, 350);
  cbfIc->SetFillColor(0);
  cbfIc->SetGrid();
  cbfIc->cd();


  TGraphErrors* gbfIc = new TGraphErrors(nIb, TIc[n-1], bf, TIcerr[n-1], bferr);
  gbfIc->GetXaxis()->SetTitle("I_{c} [mA]");
  gbfIc->GetYaxis()->SetTitle("#beta_{f}");
  gbfIc->GetXaxis()->SetLimits(min(nIb, TIc[n-1]) -20., max(nIb, TIc[n-1]) + 20.);
  gbfIc->GetYaxis()->SetLimits(min(nIb, bf) - 20., max(nIb, bf) + 10.);
  gbfIc->SetTitle("#beta_{f}(I_{c}), V = 6 V");
  gbfIc->SetMarkerSize(0.8);
  gbfIc->SetMarkerStyle(21);
  gbfIc->SetFillStyle(0);
  gbfIc->Draw("AP");
  cbfIc->Print("bfIc_BJT.png");

  //======================================================================================================================================================================================================================

  // 4. Tensione di Early in regione attiva di Ic(Vc)
  TF1* IVout[nIb] = {}; //array dei fit
  const char* IVoutname[nIb] = {"IVout_1","IVout_2","IVout_3","IVout_4"};
  double_t fitstart[] = {0.35, 0.3, 0.5, 0.6};
  cIVout->cd();

  double_t EARLY[nIb] = {};
  double_t EARLYerr[nIb] = {};

  // FIT lineare Ic(Vc)
  for (int i = 0; i < nIb; ++i){
    IVout[i] = new TF1(IVoutname[i], "[0] + [1]*x", fitstart[i], 6.2);
    IVout[i]->SetParameter(0, 50.);
    IVout[i]->SetParameter(1, 20./6.);
    IVout[i]->SetParLimits(0, 0, 160.);
    IVout[i]->SetParLimits(1, 0, 10.);
    IVout[i]->SetLineColor(1 + i);
    IVout[i]->SetLineWidth(2);
    gIVout[i]->Fit(IVout[i], "RMS+");
    cout << "Chi^2:" << IVout[i]->GetChisquare() << ", number of DoF: " << IVout[i]->GetNDF() << " (Probability: " << IVout[i]->GetProb() << ").\n";

    //EARLY[i] = IVout[i]->GetX(0., -35, 0); // questo è una gran fregatura se non puoi avere l'errore
    //gIVout[i]->SetPoint(i, EARLY[i], 0.);
    EARLY[i] = - IVout[i]->GetParameter(0) / IVout[i]->GetParameter(1);
    EARLYerr[i] = sqrt(pow(-IVout[i]->GetParError(0) / IVout[i]->GetParameter(1), 2) + pow(IVout[i]->GetParameter(0) * IVout[i]->GetParError(1)/ pow(IVout[i]->GetParameter(1), 2), 2));
    cout << "EARLY_" << i << " = (" << EARLY[i] << " +- " << EARLYerr[i] << ") V \n"
         << "-------------------------------------------------------------------------------\n";
  }
  //mgIVout->Draw("AP"); // in caso non disegnasse tutto
  //cIVout->Update(); // in caso non disegnasse tutto
  cIVout->BuildLegend(0.7, 0.6, 0.9, 0.9, "BJT in uscita", "ple");
  cIVout->Print("IcVc_BJT_out.png");
}
