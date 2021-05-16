// ESP di MILLIKAN - COSTANTE di PLANCK (h/e)
/*Strumentazione:
- tipi LED:
- voltmetro: Amprobe 37XR-A
- amperometro: Keithley
- reostato:
- alimentatore led: GwINSTEK GPS-3030DD DC
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
/*double mean(int n, double q[n], double qerr[n]){
  for (int i = 0; i < n; ++i ){
    double qmed += q[i] / qerr[i];
    double qerrsum += qerr[i];
  }
  return qmed / qerrsum;} // media pesata e semidispersione massima da array di double
double semidispersione(int n, double v[n]){
  return (max(n,v) - min(n,v)) / 2.;
}*/
double testZ(double mu, double x, double s){
  double Z = abs((x-mu)/s);
  double pvalue = erfc(Z);
  return pvalue;
}

void planck(){

  //MISURE
  const int n = 20; // numero di misure/led
  const int nled = 5; // numero di led
  double led[nled] = {425, 471, 532, 604, 640}; // [nm] lunghezze d'onda dei led
  double U[nled][n] = {{-2.1e-3, -201.7e-3, -350.6e-3, -503.2e-3, -604.1e-3, -705.9e-3, -846.8e-3, -945.3e-3, -1.105, -1.252, -1.406, -1.463, -1.550, -1.653, -1.813, -2.006, -2.201, -2.433, -2.607, -2.797},
                  {-1.9e-3, -199.6e-3, -400.6e-3, -602.8e-3, -801.5e-3, -1.005, -1.054, -1.117, -1.156, -1.197, -1.301, -1.253, -1.315, -1.505, -1.696, -1.900, -2.153, -2.357, -2.813, -1.156},
                  {-1.9e-3, -201.7e-3, -402.1e-3, -498.3e-3, -550.3e-3, -600.7e-3, -652.3e-3, -702.8e-3, -748.3e-3, -800.8e-3, -850.7e-3, -899.7e-3, -950.8e-3, -1.000, -1.053, -1.108, -1.251, -1.509, -1.749, -2.020},
                  {-2.1e-3, -199.2e-3, -103.5e-3, -251.4e-3, -305.9e-3, -349.5e-3, -391.0e-3, -429.1e-3, -468.3e-3, -524.3e-3, -5725e-3, -627.2e-3, -650.4e-3, -701.1e-3, -747.7e-3, -849.2e-3, -913.0e-3, -1.001, -1.200, -1.406},
                  {-2.5e-3, -68.1e-3, -112.1e-3, -173.5e-3, -223.2e-3, -277.7e-3, -319.4e-3, -373.5e-3, -430.3e-3, -463.4e-3, -510.0e-3, -577.4e-3, -616.7e-3, -660.8e-3, -680.2e-3, -715.3e-3, -758.6e-3, -857.7e-3, -965.5e-3, -1.120}}; // [V] potenziale di controcampo per ogni led
  double I[nled][n] = {{20.53, 14.771, 11.044, 7.744, 5.873, 4.266, 2.577, 1.732, 0.821, 0.297, 0.057, 0.023, -0.006, -0.024, -0.041, -0.050, -0.055, -0.058, -0.059, -0.060},
                  {20.214, 13.651, 8.070, 3.966, 1.6048, 0.418, 0.243, 0.095, 0.046, 0.016, -0.017, -0.006, -0.024, -0.033, -0.044, -0.049, -0.050, -0.050, -0.051, 0.048},
                  {21.770, 12.707, 6.089, 3.908, 2.989, 2.257, 1.644, 1.160, 0.806, 0.474, 0.241, 0.108, 0.042, 0.010, 0.077, -0.017, -0.034, -0.044, -0.045, -0.046},
                  {28.880, 9.620, 14.440, 7.366, 5.459, 4.199, 3.182, 2.391, 1.711, 0.975, 0.528, 0.182, 0.104, 0.025, 0.003, -0.026, -0.034, -0.039, -0.041, -0.040},
                  {20.561, 15.762, 12.917, 9.506, 7.201, 5.105, 3.781, 2.377, 1.261, 0.801, 0.365, 0.064, 0.010, -0.016, -0.024, -0.033, -0.039, -0.047, -0.049, -0.051}}; // [nA] fotocorrenti per ogni led

  //errori misure
  double led_err[nled] = {32, 14, 21, 7, 7};
  double Uerr[nled][n] = {{}};
  double Ierr[nled][n] = {{}}; // 3 pA
  for (int j = 0; j < nled; ++j){
    for (int i = 0; i < n; ++i){
      U[j][i] = -U[j][i]; // si considerano i controcampi come positivi
      Ierr[j][i] = 3e-2; // aumentata a 30 pA per forti oscillazioni durante presa misure

      //Uerr [V] [Modificare]
      if (U[j][i] < 1)  Uerr[j][i] = U[j][i] * 0.001 + 5e-4;
      else if (U[j][i] < 10) Uerr[j][i] = U[j][i] * 0.001 + 5e-3;
      else Uerr[j][i] = U[j][i] * 0.001 + 5e-2;
    }
  }

  /*-------------------------------------------------------------------------------------------------------------------------------------------*/

  //GRAFICO I-V
  TCanvas* cIV = new TCanvas("cIV", "I-V", 200, 10, 600, 400);
  cIV->SetFillColor(0);
  cIV->SetGrid();
  cIV->cd();

  TMultiGraph* mgIV = new TMultiGraph("mgIV","mgIV");
  mgIV->SetTitle("I(V_{c}) - curva di Schottky");
  mgIV->GetXaxis()->SetTitle("V_{c} [V]");
  mgIV->GetYaxis()->SetTitle("I [nA]");
  mgIV->GetXaxis()->SetLimits(-0.2,3.);
	mgIV->GetYaxis()->SetRangeUser(-2.,31);

  TGraphErrors* gIV[nled] = {}; //array dei grafici
  const char* gIVtitle[nled] = {"#lambda_{1}","#lambda_{2}","#lambda_{3}","#lambda_{4}","#lambda_{5}"};
  const char* gIVname[nled] = {"gIV_{1}","gIV_{2}","gIV_{3}","gIV_{4}","gIV_{5}"};

  TF1* IV[nled] = {}; //array dei fit
  const char* IVname[nled] = {"IV_1","IV_2","IV_3","IV_4","IV_5"};
  double IVpar[nled][3] = {{5.56e-1,-2.215,1.68},{5.26e-1,-2.784,1.357},{4.69e-1,-4.045,9.98e-1},{0,-1,3.6},{5.169e-1,-5.658,6.833e-1}};
  double Vc0[2][nled] = {}; //array dei potenziali di controcampo di U
  double Vc0_err[2][nled] = {}; // 1. GetX, 2. GetParameter

  TLegend* leg = new TLegend(0.7,0.7,0.89,0.89);
  leg->SetHeader("LED"); //modello di Schottky di I(V_{c}) al variare delle lunghezze d'onda di sorgenti

  for (int i = 0; i < nled; ++i){
    cout << IVname[i] << " led: (" << led[i] << " +- " << led_err[i] << ") nm \n";

    //GRAFICO I-V
    gIV[i] = new TGraphErrors(n, U[i], I[i], Uerr[i], Ierr[i]);
    gIV[i]->SetName(gIVname[i]);
    gIV[i]->SetTitle(gIVtitle[i]);
    gIV[i]->SetMarkerSize(0.7);
    gIV[i]->SetMarkerStyle(21+i);
    gIV[i]->SetDrawOption("AP");
    gIV[i]->SetFillStyle(0);

    //FIT I(V) funzione tipo Schottky
    IV[i] = new TF1(IVname[i], "[0]*(exp([1]*(x-[2]))-1)", min(n,U[i])-0.025, max(n,U[i])+0.1);
    IV[i]->SetParameter(0, IVpar[i][0]);
    IV[i]->SetParameter(1, IVpar[i][1]);
    IV[i]->SetParameter(2, IVpar[i][2]);
    IV[i]->SetParLimits(0, 0, 1e1);
    IV[i]->SetParLimits(1, -1e1, 0);
    IV[i]->SetParLimits(2, 0, 1e1);
    IV[i]->SetLineColor(1 + i);
    IV[i]->SetLineWidth(2);
    gIV[i]->Fit(IV[i], "RMS+");

    cout << "Chi^2:" << IV[i]->GetChisquare() << ", number of DoF: " << IV[i]->GetNDF() << " (Probability: " << IV[i]->GetProb() << ")." << endl;

    Vc0[0][i] = IV[i]->GetX(0); //zeri della funzione
    Vc0_err[0][i] = 0.15; //valore inventato
    Vc0[1][i] = IV[i]->GetParameter(2);
    Vc0_err[1][i] = IV[i]->GetParError(2);

    cout << "Potenziali di arresto: "
         << "Da GetX \t " << "Vc0" << i << " = (" << Vc0[0][i] << " +- " << Vc0_err[0][i] << ") V \n"
         << "Da GetParameter \t " << "Vc0" << i << " = (" << Vc0[1][i] << " +- " << Vc0_err[1][i] << ") V \n"
         << "------------------------------------------------------------------------------" << endl;

    mgIV->Add(gIV[i]);
    leg->AddEntry(IV[i], gIVtitle[i], "pl");
  }

  mgIV->Draw("AP");
  leg->Draw();
  cIV->Print("IVc_planck.png");

  /*-------------------------------------------------------------------------------------------------------------------------------------------*/

  //Costante di Planck

  double c = 2.99792458e8; // [m/s] velocità della luce nel vuoto (ambiente di estrazione elettronica in bulbo a vuoto)
  double e = 1.602e-19; // [C] carica dell'elettrone
  double Planck_const = 6.626e-34 / e; // [eV*s] valore di letteratura
  double KEI = -4.3407; // [eV] -2.30 eV ???  energia di prima ionizzazione del potassio FORSE NON È QUELLO GIUSTO!!! vedi Wikipedia: https://it.wikipedia.org/wiki/Lavoro_di_estrazione#:~:text=Consideriamo,%20a%20titolo%20esemplificativo,%20il,67%20x%2010-19J

  double f[nled] = {}; // [THz] frequenze led
  double ferr[nled] = {};
  for (int i = 0; i < nled; ++i){
    f[i] = 1e-3 * c / led[i];
    ferr[i] = 1e-3 * c * abs(led_err[i] / pow(led[i],2));
  }

  //GRAFICO Vc0(f)
  TCanvas* cVc0f = new TCanvas("cVc0f", "cVc0f", 200, 10, 600, 400);
  cVc0f->SetFillColor(0);
  cVc0f->SetGrid();
  cVc0f->cd();
  TGraphErrors* gVc0f = new TGraphErrors(nled, f, Vc0[1], ferr, Vc0_err[1]); //valori considerati da GetParError
  gVc0f->SetMarkerSize(0.6);
  gVc0f->SetMarkerStyle(21);
  gVc0f->SetTitle("legge di Planck");
  gVc0f->GetXaxis()->SetTitle("#nu [THz]");
  gVc0f->GetYaxis()->SetTitle("V_{c0} [V]");
  gVc0f->GetXaxis()->SetLimits(min(nled,f) - 30, max(nled,f) + 60);
  gVc0f->GetYaxis()->SetLimits(min(nled, Vc0[0]) - 0.2, max(nled, Vc0[0]) + 0.2);
  gVc0f->Draw("AP");

  //FIT Vc0 = pol1(f)
  TF1* Vc0f = new TF1("Vc0f", "[0] + [1]*x", min(nled,f)-5, max(nled,f)+5);
  Vc0f->SetParameter(0, 1e-5); // da settare meglio...
  Vc0f->SetParameter(1, Planck_const * 1e12); // fattore di conversione a THz
  Vc0f->SetParLimits(0, 1e-3, 0);
  Vc0f->SetParLimits(1, 0, 1);
  Vc0f->SetLineColor(4);
  gVc0f->Fit(Vc0f, "RMS+");
  cVc0f->Print("planck_law.png");

  cout << "Chi^2:" << Vc0f->GetChisquare() << ", number of DoF: " << Vc0f->GetNDF() << " (Probability: " << Vc0f->GetProb() << ").\n" << endl;

  double h = Vc0f->GetParameter(1) * 1e-12; // [eV*s] costante di Planck ricavata
  double herr = Vc0f->GetParError(1) * 1e-12;
  double U0 = Vc0f->GetParameter(0); // [eV] potenziale di arresto ottenuto
  double U0err = Vc0f->GetParError(0);

  //test Z di compatibilità di h da letteratura
  double pvalue_h = testZ(Planck_const, h, herr);
  double pvalue_U0 = testZ(KEI, U0, U0err);

  cout << "Potenziale di arresto: "
       << "U0 = (" << U0 << " +- " << U0err << ") eV \n"
       << "Energia di prima ionizzazione del potassio: "
       << "K.E.I. = " << KEI << " eV \n"
       << "pvalue: " << pvalue_U0;
  if (pvalue_U0 > 0.05) { cout << " H0 NON RIFIUTATA \n" << endl; }
  else { cout << " H0 RIFIUTATA \n" << endl; }

  cout << "Costante di Planck: "
       << "h = (" << h << " +- " << herr << ") eV*s \n"
       << "Planck da letteratura: "
       << "h = " << Planck_const << " eV*s \n"
       << "pvalue: " << pvalue_h;
  if (pvalue_h > 0.05) { cout << " H0 NON RIFIUTATA \n" << endl; }
  else { cout << " H0 RIFIUTATA \n" << endl; }
}
