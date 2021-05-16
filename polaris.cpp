// INTERFEROMETRO di MICHELSON-MORLEY
/*Strumentazione:
- lampada al Sodio
- Amperometro: AMPROBE 30XR-A

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
double Pi  = acos(-1);
double degtorad(double deg){
  return Pi * deg / 180.;
}
double_t min(int dim, double v[dim]){
  int min_index = 0;
  for (int i = 0; i < dim; ++i){
	if (v[min_index] > v[i]){
	  min_index=i;
	}
	//else continue;
  }
  double_t result = v[min_index];
  return result;}//funzioni per trovare elemento minimo e massimo di un array
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
double mean(int n, double q[], double qerr[]){
  double num = 0;
  double qerrsum = 0;
  for (int i = 0; i < n; ++i ){
    num += q[i] / qerr[i];
    qerrsum += 1. / qerr[i];
  }
  return num / qerrsum;} // media pesata e semidispersione massima da array di double

void polaris(){

  cout << "ATTENZIONE: potrebbe ricompilare male dopo la prima volta. In tal caso chiudere e riaprire ROOT\n";

  cout << "\n1. CONCENTRAZIONI di SOLUZIONI da ROTAZIONE del PIANO di POLARIZZAZIONE\n";
  string sostanza[] = {"FRUTTOSIO", "SACCAROSIO tubo porta campione lungo", "SACCAROSIO tubo porta campione corto"};
  const int ns = 3; // numero di campioni
  const int n = 5; // numero di misure
  double alfa[ns][n] = {{-5.65, -5.75, -5.7, -5.7, -5.75}, {6.8, 6.8, 6.85, 6.9, 6.85}, {3.6, 3.8, 3.65, 3.6, 3.65}}; // [deg] minimo di intensità luminosa
  double alfaerr[ns][n] = {{}};
  double alfa0[] = {-0.5, 0., 0., 0.15}; // misure di angolo di minima per avere offset della misura differenziale di alfa
  double alfa0_err[] = {0.05, 0.05, 0.05, 0.05};
  double offset = mean(4, alfa0, alfa0_err);
  double offset_err = 0.05;
  for (int i = 0; i < ns; ++i){
    for (int j = 0; j < n; ++j){
      alfa[i][j] -= offset; // sovrascrivere per l'angolo di MASSIMA luminosità??
      alfaerr[i][j] = 0.05 + offset_err; // offset_err potrebbe essere ridondante
    }
  }

  //analisi dati
  double alfamean[ns] = {}; // [deg] media degli alfa
  double alfamean_err[ns] = {};
  double prs[ns] = {-92.00, 66.37, 66.37}; // [deg*cm^3/(dm*g)] poteri rotatori specifici
  double prs_err = 0.01;
  double l[] = {2., 2., 1.}; // [dm] lunghezza tubo porta compione DA CHIEDERE AL PROF!!!
  double lerr[] = {1e-2, 1e-2, 1e-2,}; // DA CHIEDERE AL PROF!!!
  double rho[ns] = {}; // [g/cm^3] concentrazione sostanze nelle provette
  double rhoerr[ns]= {};
  for (int i = 0; i < ns; ++i){

    alfamean[i] = mean(n, alfa[i], alfaerr[i]);
    alfamean_err[i] = alfaerr[0][0]; // invarianza sotto propagazione degli errori di media aritmetica

    rho[i] = l[i] * alfamean[i] / prs[i];
    rhoerr[i] = sqrt(pow(lerr[i] * alfamean[i] / prs[i], 2) + pow(l[i] * alfamean_err[i] / prs[i], 2) + pow(l[i] * alfamean[i] * prs_err / (prs[i]*prs[i]), 2));

    //PRINT
    cout << "Campione: " << sostanza[i]
         << " \t Concentrazione: " << rho[i] << " +- " << rhoerr[i] << ") g/cm^3" << endl;
  }

  //------------------------------------------------------------------------------------------------------------------------------------------------------------

  cout << "\n2. LEGGE di MALUS\n";
  const int N = 37;
  double deg[N] = {}; // [rad]
  double degerr[N] = {};
  double I[] = {5.7, 8.3, 10.9, 13.8, 16.5, 18.5, 19.9, 19.7, 18.4, 16.5, 13.8, 11.1, 8.4, 5.8, 4.1, 3., 2.6, 3.8, 6., 8.8, 12.4, 15.2, 17.7, 19.8, 20.8, 20.7, 19.4, 17.4, 14.5, 11.4, 8.3, 5.6, 3.6, 2.6, 2.8, 3.9, 5.8}; // [uA] corrente da fotodiodo
  double Ierr[N] = {};
  for (int i = 0; i < N; ++i){
    deg[i] = degtorad(10. * i);
    degerr[i] = degtorad(1.); // con 2. probfit quadra
    Ierr[i] = 1.5e-2 * I[i] + 0.1; // con 2. o 2.5 probfit quadra
  }

  // GRAFICO I(deg)
  TCanvas* cIdeg = new TCanvas("cIdeg", "cIdeg", 200, 10, 600, 400);
	cIdeg->SetFillColor(0);
  cIdeg->SetGrid();
	cIdeg->cd();
	TGraphErrors* gIdeg = new TGraphErrors(N, deg, I, degerr, Ierr);
	gIdeg->SetMarkerSize(0.6);
	gIdeg->SetMarkerStyle(21);
	gIdeg->SetTitle("I(#theta)");
	gIdeg->GetXaxis()->SetTitle("#theta [rad]");
	gIdeg->GetYaxis()->SetTitle("I [#muA]");
	gIdeg->Draw("AP");

  //FIT I = I0 * cos^2(theta)
	TF1* Ideg = new TF1("Ideg", "[0] * pow(cos(x + [1]), 2) + [2]", min(N, deg), max(N, deg));
	Ideg->SetParameter(0, max(N, I));
	Ideg->SetParameter(1, 0);
  Ideg->SetParameter(2, 0);
	//Ideg->SetParLimits(0, 0, );
	//Ideg->SetParLimits(1, 0, 1);
  //Ideg->SetParLimits(2, 0, 1);
  Ideg->SetLineColor(2);
	gIdeg->Fit(Ideg, "RMS+");
	cIdeg->Print("legge_di_Malus.png");

	cout << "Chi^2: " << Ideg->GetChisquare() << ", number of DoF: " << Ideg->GetNDF() << " (Probability: " << Ideg->GetProb() << ").\n" << endl;

  //------------------------------------------------------------------------------------------------------------------------------------------------------------

  cout << "\n3. RIFO LEGGE di MALUS\n";
  const int N1 = 36;
  double deg1[N1] = {}; // [rad]
  double deg1err[N1] = {};
  double I1[] = {5.7, 8.3, 10.9, 13.8, 16.5, 18.5, 19.9, 19.7, 18.4, 16.5, 13.8, 11.1, 8.4, 5.8, 4.1, 3., 2.6, 3.8, 6., 8.8, 12.4, 15.2, 17.7, 19.8, 20.8, 20.7, 19.4, 17.4, 14.5, 11.4, 8.3, 5.6, 3.6, 2.6, 2.8, 3.9, 5.8}; // [uA] corrente da fotodiodo
  double I1err[N] = {};
  for (int i = 0; i < N1; ++i){
    deg[i] = degtorad(10. * i);
    degerr[i] = degtorad(1.); // con 2. probfit quadra
    Ierr[i] = 1.5e-2 * I[i] + 0.1; // con 2. o 2.5 probfit quadra
  }

  // GRAFICO I(deg)
  TCanvas* cIdeg1 = new TCanvas("cIdeg1", "cIdeg1", 200, 10, 600, 400);
	cIdeg1->SetFillColor(0);
  cIdeg1->SetGrid();
	cIdeg1->cd();
	TGraphErrors* gIdeg1 = new TGraphErrors(N1, deg1, I1, deg1err, I1err);
	gIdeg1->SetMarkerSize(0.6);
	gIdeg1->SetMarkerStyle(21);
	gIdeg1->SetTitle("I(#theta) ");
	gIdeg1->GetXaxis()->SetTitle("#theta [rad]");
	gIdeg1->GetYaxis()->SetTitle("I [#muA]");
	gIdeg1->Draw("AP");

  //FIT I = I0 * cos^2(theta)
	TF1* Ideg = new TF1("Ideg", "[0] * pow(cos(x + [1]), 2) + [2]", min(N, deg), max(N, deg));
	Ideg->SetParameter(0, max(N, I));
	Ideg->SetParameter(1, 0);
  Ideg->SetParameter(2, 0);
	//Ideg->SetParLimits(0, 0, );
	//Ideg->SetParLimits(1, 0, 1);
  //Ideg->SetParLimits(2, 0, 1);
  Ideg->SetLineColor(2);
	gIdeg->Fit(Ideg, "RMS+");
	cIdeg->Print("legge_di_Malus.png");

	cout << "Chi^2: " << Ideg->GetChisquare() << ", number of DoF: " << Ideg->GetNDF() << " (Probability: " << Ideg->GetProb() << ").\n" << endl;

  //------------------------------------------------------------------------------------------------------------------------------------------------------------

}
