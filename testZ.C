//implementazione di una funzione per eseguire test Z di compatibilità tra valori. I test sono a due code con livelli di 
//significatività vari e incertezze su entrambi i valori


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
#include <TLatex.h>
using namespace std;
void Ztest(double x1,double x2, double err1, double err2, double alpha){

	double z = abs(x1 - x2) / sqrt(pow(err1, 2) + pow(err2, 2));  //definisco la variabile Z
	
	double prob_z;
	double pvalue_z;
	prob_z = (ROOT::Math::normal_cdf(z,1,0) - 0.5)*2;
	pvalue_z = (1-ROOT::Math::normal_cdf(z,1,0))*2;
	
	
	if(pvalue_z > alpha){
		cout 	<< "Test Z con livello di significativita alpha " << alpha << endl
		<< "Valore di Z = " << z << endl
		<< "Probabilita di avere tale valore di Z: "<< prob_z << endl
		<< "p-value di Z, ossia probabilita di ottenere |Z| maggiore di quanto osservato: "<< pvalue_z <<endl
		<< "Poiche p-value > alpha, H0 non viene rigettata. Il test ha successo." << endl;
	}
	
	if(pvalue_z <= alpha){
		cout 	<< "Test Z con livello di significativita alpha " << alpha << endl
		<< "Valore di Z = " << z << endl
		<< "Probabilita di avere tale valore di Z: "<< prob_z << endl
		<< "p-value di Z, ossia probabilita di ottenere |Z| maggiore di quanto osservato: "<< pvalue_z <<endl
		<< "Poiche p-value < alpha, H0 viene rigettata. Il test non ha successo. La differenza tra i due valori di Z non puo essere attribuita interamente a fattori aleatori." << endl;
	}


}



void testZ(){
	double IQ_tot = 100;     //esercizio per controllare che sia giusto il codice
	double err_tot = sqrt(256/64);
	double IQ_stud = 106;
	double err_stud = 0;
	double alpha = 0.05;
	Ztest(IQ_tot, IQ_stud,err_tot,err_stud,alpha);
	
	return 0;
	
}
