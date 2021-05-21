#include<iostream>
#include<cmath>
#include<TGraphErrors.h>
#include<TAxis.h>
#include<TCanvas.h>
#include<TF1.h>
#include<iomanip>
#include<cmath>

int Diodi()
{
    /*
    //PARTE RIGUARDANTE IL DIODO DUALE
    int nmisure = 25;

    float V_d_diodo[] = {417.7,485.5,511.2,528.6,541.4,552,560.7,568.1,574.3,579.7,584.3,588.6,593.5,600.4,606.4,612,616.6,620.9,624.8,628.5,631.6,97.5,191.5,302.8,381.4};
    float I_d_diodo[] = {0.0406,0.256,0.485,0.686,0.923,1.155,1.392,1.63,1.855,2.089,2.324,2.566,2.798,3.276,3.744,4.224,4.694,5.167,5.655,6.133,6.594,0.00005,0.00012,0.00124,0.01125};

    float err_V_d_diodo[nmisure];
    float err_I_d_diodo[nmisure];

for (int i =0;i<nmisure;i++)
    {
        err_V_d_diodo[i] = V_d_diodo[i]*0.001+0.5;

        if(I_d_diodo[i] < 10) err_I_d_diodo[i] = 0.005*I_d_diodo[i]+0.005;
        if(I_d_diodo[i] < 1) err_I_d_diodo[i] = 0.005*I_d_diodo[i]+0.0005;
        if(I_d_diodo[i] < 0.1) err_I_d_diodo[i] = (0.005*I_d_diodo[i]+0.0001);
        
    }
std::cout << "I [mA]      errI [mA]      V [mV]     errV [mV]" << endl;
for (int i =0;i<nmisure;i++)
    {
	std::cout << I_d_diodo[i] << "     " << err_I_d_diodo[i] <<"       "<< V_d_diodo[i] << "      " << err_V_d_diodo[i] << endl;
	}	
	
    TCanvas *cIV = new TCanvas("passa_basso","G(f)",200,10,600,400);
    cIV->SetFillColor(0);
    cIV->cd();

    TGraphErrors *gIV = new TGraphErrors(nmisure,V_d_diodo,I_d_diodo,err_V_d_diodo,err_I_d_diodo);
    gIV->SetMarkerStyle(8);
    gIV->SetMarkerSize(1);
    gIV->SetTitle("Caratteristica I-V del Diodo");
    gIV->GetXaxis()->SetTitle("mV");
    gIV->GetYaxis()->SetTitle("Id");

    gIV->Draw("AP");

    TF1 *funz1 = new TF1 ("funz1","[0]*(exp((x)/([1]*26))-1)",0,635);
    funz1->SetLineColor(2);
    funz1->SetParameter(1,1.74);
    funz1->SetParameter(0,0.00000591125);
    gIV->Fit(funz1,"RM+");
    std::cout << "Probabilità:  " << funz1->GetProb();
*/

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

*/
    float
I_d_led[]={0.5094,0.8243,1.039,1.294,1.511,1.8,1.903,2.041,2.273,2.512,3.201,3.442,3.672,4.363,5.321,5.779,6.25,6.717,7.66,8.633};
    float V_d_led[]={1.818,1.833,1.843,1.855,1.864,1.875,1.877,1.882,1.888,1.896,1.915,1.92,1.925,1.942,1.962,1.97,1.979,1.988,2.005,2.022};


    int Nmisure = 20;    //attenzione: in base al set dati in uso ricordarsi di modificare il numero di misure

    TCanvas *cVI = new TCanvas("passa_basso","G(f)",200,10,600,400);
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
    std::cout << "I [mA]      errI [mA]      V [mV]     errV [mV]" << endl;
for (int i =0;i<Nmisure;i++)
    {
	std::cout << I_d_led[i] << "     " << err_I_d_led[i] <<"       "<< V_d_led[i] << "      " << err_V_d_led[i] << endl;
	}

    TGraphErrors *gVI = new TGraphErrors(Nmisure,I_d_led,V_d_led,err_I_d_led,err_V_d_led);
    gVI->SetMarkerStyle(8);
    gVI->SetMarkerSize(1);

    gVI->SetTitle("Caratteristica VI del LED");
    gVI->GetXaxis()->SetTitle("mV");
    gVI->GetYaxis()->SetTitle("Id");

    gVI->Draw("AP");

    TF1 *funz2 = new TF1 ("funz1","[0]*log(x*[1]+1)+[2]*x",0,9);
    funz2->SetLineColor(2);
    funz2->SetParLimits(0,0,999);
    funz2->SetParLimits(2,0,1);

    funz2->SetParameter(0,9.04E-2);
    funz2->SetParameter(1,6.16E8);
    funz2->SetParameter(2,2.26E-6);

    gVI->Fit(funz2,"RM+");
    std::cout << " Probabilità: " << funz2->GetProb() << endl;








    return 0;
}
