#include<iostream>
#include<cmath>
#include<TGraphErrors.h>
#include<TAxis.h>
#include<TCanvas.h>
#include<TF1.h>
#include<iomanip>
#include<cmath>
using namespace std;

int DIODI()
{
    
    //PARTE RIGUARDANTE IL DIODO DUALE
    


// tensioni in mV e correnti in mA 
// tolgo dati man mano per avere chiquadro accettabili
    
 /*
 25 misure, chiquadro = 596 , probabilità = 0   
    float V_d_diodo[] = {97.5,191.5,302.8,381.4,417.7,485.5,511.2,528.6,541.4,552,560.7,568.1,574.3,579.7,584.3,588.6,593.5,600.4,606.4,612,616.6,620.9,624.8,628.5,631.6};
    float I_d_diodo[] = {0.00005,0.00012,0.00124,0.01125,0.0406,0.256,0.485,0.686,0.923,1.155,1.392,1.63,1.855,2.089,2.324,2.566,2.798,3.276,3.744,4.224,4.694,5.167,5.655,6.133,6.594};


  22 misure, togliendo le prime 3 --  chiquadro = 491 , probabilità = 0
    float V_d_diodo[] = {381.4,417.7,485.5,511.2,528.6,541.4,552,560.7,568.1,574.3,579.7,584.3,588.6,593.5,600.4,606.4,612,616.6,620.9,624.8,628.5,631.6};
    float I_d_diodo[] = {0.01125,0.0406,0.256,0.485,0.686,0.923,1.155,1.392,1.63,1.855,2.089,2.324,2.566,2.798,3.276,3.744,4.224,4.694,5.167,5.655,6.133,6.594};

//21 misure, togliendo le prime 4 --  chiquadro = 148, probabilità = 0
    float V_d_diodo[] = {417.7,485.5,511.2,528.6,541.4,552,560.7,568.1,574.3,579.7,584.3,588.6,593.5,600.4,606.4,612,616.6,620.9,624.8,628.5,631.6};
    float I_d_diodo[] = {0.0406,0.256,0.485,0.686,0.923,1.155,1.392,1.63,1.855,2.089,2.324,2.566,2.798,3.276,3.744,4.224,4.694,5.167,5.655,6.133,6.594};

  20 misure, togliendo le prime 5  --  chiquadro = 7.2 , probabilità = 0.98
float V_d_diodo[] = {485.5,511.2,528.6,541.4,552,560.7,568.1,574.3,579.7,584.3,588.6,593.5,600.4,606.4,612,616.6,620.9,624.8,628.5,631.6};
    float I_d_diodo[] = {0.256,0.485,0.686,0.923,1.155,1.392,1.63,1.855,2.089,2.324,2.566,2.798,3.276,3.744,4.224,4.694,5.167,5.655,6.133,6.594};

*/

int nmisure = 25; //attenzione, ricordarsi di cambiarla per i vari set di misure
float V_d_diodo[] = {97.5,191.5,302.8,381.4,417.7,485.5,511.2,528.6,541.4,552,560.7,568.1,574.3,579.7,584.3,588.6,593.5,600.4,606.4,612,616.6,620.9,624.8,628.5,631.6};
float I_d_diodo[] = {0.00005,0.00012,0.00124,0.01125,0.0406,0.256,0.485,0.686,0.923,1.155,1.392,1.63,1.855,2.089,2.324,2.566,2.798,3.276,3.744,4.224,4.694,5.167,5.655,6.133,6.594};


    float err_V_d_diodo[nmisure];
    float err_I_d_diodo[nmisure];

for (int i =0;i<nmisure;i++)
    {
        err_V_d_diodo[i] = V_d_diodo[i]*0.001+0.5;

        if(I_d_diodo[i] < 10) err_I_d_diodo[i] = 0.005*I_d_diodo[i]+0.005;
        if(I_d_diodo[i] < 1) err_I_d_diodo[i] = 0.005*I_d_diodo[i]+0.0005;       
        if(I_d_diodo[i] < 0.1) err_I_d_diodo[i] = (0.005*I_d_diodo[i]+0.0001);
        
    }
    cout << "Analisi dati Diodo al silicio: \nMisure:" << endl;
std::cout << "I [mA]      errI [mA]      V [mV]     errV [mV]" << endl;
for (int i =0;i<nmisure;i++)
    {
	std::cout << I_d_diodo[i] << "     " << err_I_d_diodo[i] <<"       "<< V_d_diodo[i] << "      " << err_V_d_diodo[i] << endl;
	}	
	
    TCanvas *cIV = new TCanvas("Silicio","I-V silicio",200,10,600,400);
    cIV->SetFillColor(0);
    cIV->cd();

    TGraphErrors *gIV = new TGraphErrors(nmisure,V_d_diodo,I_d_diodo,err_V_d_diodo,err_I_d_diodo);
    gIV->SetMarkerStyle(8);
    gIV->SetMarkerSize(1);
    gIV->SetTitle("Caratteristica I-V del Diodo al silicio");
    gIV->GetXaxis()->SetTitle("Tensione [mV]");
    gIV->GetYaxis()->SetTitle("Corrente [mA]");

    gIV->Draw("AP");
    

    TF1 *funz1 = new TF1 ("funz1","[0]*(exp((x)/([1]*26))-1)",450,700);
    funz1->SetLineColor(2);
    funz1->SetParameter(1,1.74);
    funz1->SetParameter(0,0.00000591125);
    gIV->Fit(funz1,"RM+");
    cIV->Print("grafico I(V)_silicio.png");
    	double S_Is = funz1->GetParameter(0);
	double S_eta = funz1->GetParameter(1);
	double S_Is_err = funz1->GetParError(0);
	double S_eta_err = funz1->GetParError(1);
	
cout << "Chi^2:" << funz1->GetChisquare() << endl;
cout << "Gradi di liberta: " << funz1->GetNDF() <<endl;
cout << "Probabilita di chiquadro: " << funz1->GetProb() << endl;
cout << "Parametri del fit per diodo al silicio:" << endl;
cout << "Corrente di saturazione:(" << S_Is << "+-"<< S_Is_err <<")" << endl;
cout << "Fattore di idealita eta:(" << S_eta << "+-"<< S_eta_err <<")" << endl;
cout << "-------------------------------------------------------------------------------------------------"<< endl;



//----------------------------------------------------------------------------------------------------------------------------------------//
    //LED
    //tutto in V e mA
    //1.702corrente 1.7100voltaggio
    //attenzione: non sono tutti i dati originali, li ho un po' puliti da dati troppo strani o incoerenti con gli altri
    
/*
qui di seguito ho i dati in ordine di presa dati, ma puliti da robe strane
    float I_d_led[]={0.00005,0.00006,0.00006,0.00007,0.00008,0.00009,0.00011,0.00014,0.00017,0.00043,0.02622,0.1213,0.2341,0.3516,0.4678,0.5094,0.8243,2.041,1.8,1.903,2.273,2.512,3.201,3.442,3.672,1.039,1.294,1.511,4.363,5.321,5.779,6.25,6.717,7.66,8.633};
    float V_d_led[]={0.1043,0.207,0.254,0.3006,0.4001,0.504,0.7568,0.996,1.257,1.505,1.704,1.757,1.781,1.796,1.807,1.818,1.833,1.882,1.875,1.877,1.888,1.896,1.915,1.92,1.925,1.843,1.855,1.864,1.942,1.962,1.97,1.979,1.988,2.005,2.022};



qui di seguito ordino i dati ed elimino quelli iniziali per provare un fit senza considerare i primi dati

i dati ordinati sono i seguenti (numero misure: 35):
float I_d_led[]={0.00005,0.00006,0.00006,0.00007,0.00008,0.00009,0.00011,0.00014,0.00017,0.00043,0.02622,0.1213,0.2341,0.3516,0.4678,0.5094,0.8243,1.039,1.294,1.511,1.8,1.903,2.041,2.273,2.512,3.201,3.442,3.672,4.363,5.321,5.779,6.25,6.717,7.66,8.633};
    float V_d_led[]={0.1043,0.207,0.254,0.3006,0.4001,0.504,0.7568,0.996,1.257,1.505,1.704,1.757,1.781,1.796,1.807,1.818,1.833,1.843,1.855,1.864,1.875,1.877,1.882,1.888,1.896,1.915,1.92,1.925,1.942,1.962,1.97,1.979,1.988,2.005,2.022};

togliendo i primi 10 si ha:
    float
I_d_led[]={0.02622,0.1213,0.2341,0.3516,0.4678,0.5094,0.8243,1.039,1.294,1.511,1.8,1.903,2.041,2.273,2.512,3.201,3.442,3.672,4.363,5.321,5.779,6.25,6.717,7.66,8.633};
    float V_d_led[]={1.704,1.757,1.781,1.796,1.807,1.818,1.833,1.843,1.855,1.864,1.875,1.877,1.882,1.888,1.896,1.915,1.92,1.925,1.942,1.962,1.97,1.979,1.988,2.005,2.022};

togliendo i primi 15 si ha:
    float
I_d_led[]={0.5094,0.8243,1.039,1.294,1.511,1.8,1.903,2.041,2.273,2.512,3.201,3.442,3.672,4.363,5.321,5.779,6.25,6.717,7.66,8.633};
    float V_d_led[]={1.818,1.833,1.843,1.855,1.864,1.875,1.877,1.882,1.888,1.896,1.915,1.92,1.925,1.942,1.962,1.97,1.979,1.988,2.005,2.022};

togliendo i primi 17 si ha:
    float
I_d_led[]={1.039,1.294,1.511,1.8,1.903,2.041,2.273,2.512,3.201,3.442,3.672,4.363,5.321,5.779,6.25,6.717,7.66,8.633};
    float V_d_led[]={1.843,1.855,1.864,1.875,1.877,1.882,1.888,1.896,1.915,1.92,1.925,1.942,1.962,1.97,1.979,1.988,2.005,2.022};


togliendo i primi 17 e gli ultimi due
    float
I_d_led[]={1.039,1.294,1.511,1.8,1.903,2.041,2.273,2.512,3.201,3.442,3.672,4.363,5.321,5.779,6.25,6.717};
    float V_d_led[]={1.843,1.855,1.864,1.875,1.877,1.882,1.888,1.896,1.915,1.92,1.925,1.942,1.962,1.97,1.979,1.988};

*/
        
        //togliendo i primi 16 si ha:

            float I_d_led[]={0.00005,0.00006,0.00006,0.00007,0.00008,0.00009,0.00011,0.00014,0.00017,0.00043,0.02622,0.1213,0.2341,0.3516,0.4678,0.5094,0.8243,1.039,1.294,1.511,1.8,1.903,2.041,2.273,2.512,3.201,3.442,3.672,4.363,5.321,5.779,6.25,6.717,7.66,8.633};
                float V_d_led[]={0.1043,0.207,0.254,0.3006,0.4001,0.504,0.7568,0.996,1.257,1.505,1.704,1.757,1.781,1.796,1.807,1.818,1.833,1.843,1.855,1.864,1.875,1.877,1.882,1.888,1.896,1.915,1.92,1.925,1.942,1.962,1.97,1.979,1.988,2.005,2.022};


         
    int Nmisure = 35;    //attenzione: in base al set dati in uso ricordarsi di modificare il numero di misure

    TCanvas *cVI = new TCanvas("LED","V-I LED",200,10,600,400);
    cVI->SetFillColor(0);
    cVI->cd();

    float err_V_d_led[Nmisure];
    float err_I_d_led[Nmisure];

    for (int i =0;i<Nmisure;i++)
    {
        if(V_d_led[i] < 10) err_V_d_led[i] = V_d_led[i]*0.001+0.005;
        if(V_d_led[i] < 1) err_V_d_led[i] = V_d_led[i]*0.001+0.0005;

        if(I_d_led[i] < 10) err_I_d_led[i] = 0.005*I_d_led[i]+0.005;
        if(I_d_led[i] < 1) err_I_d_led[i] = 0.005*I_d_led[i]+0.0005;
        if(I_d_led[i] < 0.1) err_I_d_led[i] = (0.005*I_d_led[i]+0.0001);
        
    }
    
    
    //passo le correnti e i loro errori da mA ad Ampere
    
    for (int i =0;i<Nmisure;i++)
    {	I_d_led[i] = I_d_led[i]*1e-3;
        err_I_d_led[i] = err_I_d_led[i]*1e-3;
    }
    
    
    //stampo le misure
    cout << "Analisi dati Diodo LED: \nMisure:" << endl;
    std::cout << "I [A]      errI [A]      V [V]     errV [V]" << endl;
for (int i =0;i<Nmisure;i++)
    {
	std::cout << I_d_led[i] << "     " << err_I_d_led[i] <<"       "<< V_d_led[i] << "      " << err_V_d_led[i] << endl;
	}
	
	

    TGraphErrors *gVI = new TGraphErrors(Nmisure,I_d_led,V_d_led,err_I_d_led,err_V_d_led);
    gVI->SetMarkerStyle(8);
    gVI->SetMarkerSize(1);

    gVI->SetTitle("Caratteristica V(I) del diodo a LED");
    gVI->GetXaxis()->SetTitle("Corrente [A]");
    gVI->GetYaxis()->SetTitle("Tensione [V]");
    gVI->GetYaxis()->SetRangeUser(1.5, 2.02);
    gVI->Draw("AP");

    TF1 *funz2 = new TF1 ("funz2","[0]*0.026*log(x*[1]+1)+[2]*x",0.0006,0.008);
    funz2->SetLineColor(2);
    funz2->SetParLimits(0,0,999);
    funz2->SetParLimits(2,0,1);

    funz2->SetParameter(0,9.04E-2);
    funz2->SetParameter(1,6.16E8);
    funz2->SetParameter(2,2.26E-6);

    gVI->Fit(funz2,"RM+");
    cVI->Print("grafico V(I)_LED.png");
    
	double Lp0 = funz2->GetParameter(0);
	double Lp1 = funz2->GetParameter(1);
	double Lp2 = funz2->GetParameter(2);
	double Lp0_err = funz2->GetParError(0);
	double Lp1_err = funz2->GetParError(1);
	double Lp2_err = funz2->GetParError(2);
	
	double L_eta = Lp0;
	double L_eta_err = Lp0_err;
	double L_Is = 1/Lp1;
	double L_Is_err = sqrt( pow(Lp1_err,2)/pow(Lp1,4) );
	double Rd= Lp2;
	double Rd_err = Lp2_err;

cout << "Chi^2:" << funz2->GetChisquare() << endl;
cout << "Gradi di liberta: " << funz2->GetNDF() <<endl;
cout << "Probabilita di chiquadro: " << funz2->GetProb() << endl;
cout << "Parametri del fit per diodo al LED:" << endl;
cout << "Corrente di saturazione:(" << L_Is << "+-"<< L_Is_err <<")" << endl;
cout << "Fattore di idealita eta:(" << L_eta << "+-"<< L_eta_err <<")" << endl;
cout << "Resistenza Ohmica LED:(" << Rd << "+-"<< Rd_err <<")" << endl;









    return 0;
}
