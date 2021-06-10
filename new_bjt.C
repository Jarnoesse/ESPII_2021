
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
