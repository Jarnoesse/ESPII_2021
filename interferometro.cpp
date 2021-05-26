// INTERFEROMETRO di MICHELSON-MORLEY
/*Strumentazione:
- palmer 1 micron
- interferometro PASCO scientific Model OS-8501 laser: 632.8 nm
- goniometro 0.1°
- spessore vetro
- manometro 100 mbar
- lunghezza camera a vuoto (30.0 +- 0.05) mm
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
double semidispersione(int n, double v[n]){
  return (max(n,v) - min(n,v)) / 2.;
}
double testZ(double mu, double x, double s){ // DA METTERE A POSTO!!!!
  double Z = abs((x-mu)/s);
  double pvalue = erfc(Z) + 0.5; // ritrasformo in funzione di partazione
  return pvalue;
}

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

/*double Media(double array[], int lunghezza_array){
  double media = 0;
  if (lunghezza_array == 0) {lunghezza_array = *(&array + 1) - array;} // questo if è stato aggiunto per problemi nel chiamare questa funzione in Varianza
  for (int i = 0; i < lunghezza_array; i++) {media = media + array[i];}
  media = media / lunghezza_array;
  return media;} //Calcola la Media degli elementi degli array
double Varianza(double array[], int lunghezza_array){ //Calcola la varianza dato un array
  double varianza = 0;
  double media = Media(array,lunghezza_array);
  for (int i = 0; i < lunghezza_array; i++) {varianza = varianza + pow(array[i]-media,2);}
  varianza = varianza / (lunghezza_array-1);
  return varianza;
}*/


void interferometro(){

  // 1. MISURA di LAMBDA di laser He-Ne con interferometro
  const int nmis = 4; // numero di misure
  int frange = 80; // numero di frange apparse/scomparse
  double s[] = {26., 25., 25., 25.}; // [micron] spostamento braccio da palmer
  double serr[nmis] = {};
  double lambda[nmis] = {}; // [nm]
  double lambda_err[nmis] = {};
  for (int i = 0; i < nmis; ++i){
    serr[i] = 1.;
    lambda[i] = 1e3 * 2. * s[i] / frange; // 2* perché specchio mobile dove si conta andata e ritorno del cammino ottico
    lambda_err[i] = 1e3 * 2. * serr[i] / frange;
  }

  // analisi statistica di lambda
  double lambda_mean = mean(nmis, lambda, lambda_err);
  double lambda_mean_err = semidispersione(nmis, lambda);

  //PRINT misure
  cout << "\n1. MISURA di LAMBDA di laser He-Ne con interferometro \n";
  for (int i = 0; i < nmis; ++i){ cout << "lambda = (" << lambda[i] << " +- " << lambda_err[i] << ") nm" << endl; }
  cout << "Valore medio: (" << lambda_mean << " +- " << lambda_mean_err << ") nm \n";

  //-------------------------------------------------------------------------------------------------------------------------------------------

  // 2. INDICE di RIFRAZIONE del vetro
  const int N = 9; // numero di misure
  frange = 50;
  double d = 5.66e-3; // [m] spessore vetro da palmer
  double derr = 0.05;
  double ang[] = {6.0, 3.0, 5.4, 7.0, 6.6, 7.0, 6.9, 6.9, 7.1}; // [deg] inclinazione vetro su cammino ottico
  double angerr[N] = {};
  double n[N] = {}; // indice di rifrazione
  double nerr[N] = {};
  lambda_mean = lambda_mean * 1e-9; // in [m]
  lambda_mean_err = /*lambda_mean_err*/ 25. * 1e-9;
  for (int i = 0; i < N; ++i){
    ang[i] = degtorad(ang[i]); // tutte misure angolari in randianti
    angerr[i] = degtorad(0.1);
    n[i] = (d - lambda_mean * N/2.) * (1. - cos(ang[i])) / (d * (1. - cos(ang[i])) - (lambda_mean * N/2.));
    nerr[i] = sqrt(pow(N,2) * 4 * (pow(lambda_mean * derr,2) + pow(d * lambda_mean_err ,2)) * pow((cos(ang[i]) - 1) * cos(ang[i]),2) + pow(lambda_mean * (N * lambda_mean - 2*d) * angerr[i] * sin(ang[i]),2)) / pow(N * lambda_mean - 2*d + 2*d*cos(ang[i]),2);
  }

  // analisi statistica di n
  double angmean = mean(N, ang, angerr);
  double angmeanerr = semidispersione(N, ang);
  double nmean = mean(N, n, nerr);
  double nmean_maxerr = semidispersione(N, n); // non quadra con la propagazione degli errori

  //PRINT misure
  cout << "\n2. INDICE di RIFRAZIONE del vetro" << endl;
  for (int i = 0; i < N; ++i){ cout << "n = (" << n[i] << " +- " << nerr[i] << ")" << endl; }
  cout << "Valore medio: (" << nmean << " +- " << nmean_maxerr << ") \n" << endl;

  //-------------------------------------------------------------------------------------------------------------------------------------------

  cout << "3. INDICE di RIFRAZIONE in funzione della PRESSIONE " << endl;
  double p0 = 979.5e-3; // [bar] pressione atmosferica locale http://www.meteo.dfg.unito.it/storico-7-5-2021
  double pA = 1.01325; // [bar] pressione atmosferica standard (livello del mare)
  d = 3e-2; // [m] lunghezza della camera a vuoto
  derr = 0.05e-3; // supposta perché non data dal costruttore
  double p[] = {-0.8, -0.5, 0.3, 0.5, 0.7, 0.9, 1.0, 1.2, 1.3}; // [bar] differenza di pressione rispetto a 1 atm
  double m[] = {-17., -10., 6., 9., 15., 19., 20., 26., 27.}; // variazione del numero frange
  double perr[N] = {};
  double merr[N] = {};
  for (int i = 0; i < N; ++i){
    //p[i] += p0; // pressione NON differenziale
    perr[i] = 0.1;
    merr[i] = 0.;
  }

  //GRAFICO m(p)
	TCanvas* cmp = new TCanvas("cmp", "cmp", 200, 10, 600, 400);
	cmp->SetFillColor(0);
  cmp->SetGrid();
	cmp->cd();
	TGraphErrors* gmp = new TGraphErrors(N, p, m, perr, merr);
	gmp->SetMarkerSize(0.6);
	gmp->SetMarkerStyle(21);
	gmp->SetTitle("m(p)");
	gmp->GetXaxis()->SetTitle("p [bar]");
	gmp->GetYaxis()->SetTitle("m [numero frange]");
	gmp->Draw("AP");

  //FIT m = pol1(p)
	TF1* mp = new TF1("mp", "[0] + [1]*x", min(N, p), max(N, p));
	mp->SetParameter(0, 5); // da settare meglio...
	mp->SetParameter(1, 1);
	//mp->SetParLimits(0, 0, );
	//mp->SetParLimits(1, 0, 1);
	mp->SetLineColor(2);
	gmp->Fit(mp, "RMS+");
	cmp->Print("nfrange_pressione.png");

	cout << "Chi^2: " << mp->GetChisquare() << ", number of DoF: " << mp->GetNDF() << " (Probability: " << mp->GetProb() << ")." << endl;

  double Np = mp->GetParameter(1);
  double Nperr = mp->GetParError(1);
  double np = Np * lambda_mean / (2 * d);
  double nperr = sqrt(pow(Nperr * lambda_mean / (2 * d), 2) + pow(Np * lambda_mean_err / (2 * d), 2) + pow(derr * Np * lambda_mean / (2 * d*d), 2));
  double nA = 1. + np * pA; // n(p) = n(0) + dn/dp * p and n(0) = 1 <=> vacuum index
  double nAerr = sqrt(pow(nperr * pA, 2) + pow(np * 1e-3, 2)); // modificato
  double nA_teo = 1.000269;
  cout << "indice di rifrazione dell'aria a 1 atm: "
       << "n = (" << nA << " +- " << nAerr << ") \n"
       << "indice di rifrazione dell'aria a pressione standard e lambda 589.3 nm: "  << setprecision(10) << nA_teo << endl;

  // test Z di compatibilità per nA con nA_teo
  Ztest(nA_teo, nA, 0., nAerr);

  /*
  double pvalue_nA = testZ(nA_teo, nA, nAerr);
  cout << "pvalue: " << pvalue_nA;
  if (pvalue_nA > 0.05) { cout << " COMPATIBILI \n" << endl; }
  else { cout << " NON COMPATIBILI \n" << endl; }*/
  
}
