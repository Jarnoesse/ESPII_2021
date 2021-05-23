#include<iostream>
#include<cmath>
#include<TGraphErrors.h>
#include<TAxis.h>
#include<TCanvas.h>
#include<TF1.h>
#include<iomanip>
#include<cmath>

int BJT()
{
    //grafico e dati a corrente 100 uA
    int nmisure100 = 13 ;
    float Vb_100[] ={0.535,0.543,0.55,0.555,0.637,0.637,0.637,0.637,0.637,0.603,0.627,0.629,0.63};  //Volt
    float Ib_100[] ={100,100,100,100,100,100,100,95,90,90,90,90,90};  //microAmpere
    float Vc_100[] ={0.0053,0.015,0.023,0.0292,0.1111,0.2038,0.3255,0.456,0.7685,1.012,1.261,1.499,1.756};  //Volt
    float Ic_100[] ={0.535,0.543,0.55,0.555,0.637,0.637,0.637,0.637,0.637,0.603,0.627,0.629,0.63};  //milliAmpere
    
    float errVb_100[nmisure100];
    float errIb_100[nmisure100];
    float errVc_100[nmisure100];
    float errIc_100[nmisure100];

    for (int i =0;i<nmisure100;i++)
    {
        if(Vc_100[i] < 10) errVc_100[i] = Vc_100[i]*0.001+0.005;   //Volt, Amprobe 37X
        if(Vc_100[i] < 1) errVc_100[i] = Vc_100[i]*0.001+0.0005;
	
	if(Ic_100[i] < 100) errIc_100[i] = 0.005*Ic_100[i]+0.05;
        if(Ic_100[i] < 10) errIc_100[i] = 0.005*Ic_100[i]+0.005;    //mA
        if(Ic_100[i] < 1) errIc_100[i] = 0.005*Ic_100[i]+0.0005;
        if(Ic_100[i] < 0.1) errIc_100[i] = (0.005*Ic_100[i]+0.0001);
        
        errIb_100[i] = 5;
        
        if(Vb_100[i] < 1) errVb_100[i] = Vb_100[i]*0.005+0.003;
        
    }
    
    std::cout << "Ic [mA]      errIc [mA]      Vc [V]     errVc [V]   con Ib = 100 uA" << endl;
for (int i =0;i<nmisure100;i++)
    {
	std::cout << Ic_100[i] << "     " << errIc_100[i] <<"       "<< Vc_100[i] << "      " << errVc_100[i] << endl;
	}
    
    // a Ib=100 uA, costruisco ora il grafico Ic(Vc)
    
    TCanvas *cIV_100 = new TCanvas("BJT_100","G(f)",200,10,600,400);
    cIV_100->SetFillColor(0);
    cIV_100->cd();

    TGraphErrors *gIV_100 = new TGraphErrors(nmisure100,Vc_100,Ic_100,errVc_100,errIc_100);
    gIV_100->SetMarkerStyle(8);
    gIV_100->SetMarkerSize(1);
    gIV_100->SetTitle("Ic (Vc) del transistor BJT con Ib=100 uA");
    gIV_100->GetXaxis()->SetTitle("Vc");
    gIV_100->GetYaxis()->SetTitle("Ic");

    gIV_100->Draw("AP");
    
    
    
    
    
    
    
     //grafico e dati a corrente 150 uA
    int nmisure150 = 36;
    float Vb_150[] ={0.574,0.58,0.582,0.584,0.587,0.588,0.589,0.59,0.59,0.591,0.592,0.0593,0.0594,0.595,0.595,0.618,0.633,0.638,0.644,0.651,0.656,0.668,0.675,0.681,0.685,0.688,0.689,0.689,0.688,0.688,0.688,0.688,0.689,0.689,0.689,0.688};  //Volt
    float Ib_150[] ={150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,145,145,145,145,140,140,140,140,140,140,140,140,140,140,140,140,140,140,140};  //microAmpere
    float Vc_150[] ={0.0048,0.0108,0.0132,0.0155,0.0179,0.0189,0.0199,0.0209,0.022,0.0228,0.0238,0.0246,0.0255,0.0264,0.0273,0.0557,0.0751,0.0812,0.0908,0.1006,0.1102,0.1334,0.1532,0.1738,0.2053,0.3007,0.6858,0.7426,0.785,0.836,0.8812,1.127,2.47,3.406,4.126,6.019};  //Volt
    float Ic_150[] ={0.00423,0.1969,0.2893,0.3843,0.4893,0.5837,0.5836,0.6315,0.6843,0.7295,0.7784,0.8241,0.8731,0.9259,0.9783,3.477,6.3,7.413,9.328,11.47,13.72,19.31,23.65,27.09,30.19,32.38,33.46,33.6,33.68,33.8,33.89,34.4,35.86,37.21,38.09,40.53};  //milliAmpere
    
    float errVb_150[nmisure150];
    float errIb_150[nmisure150];
    float errVc_150[nmisure150];
    float errIc_150[nmisure150];

    for (int i =0;i<nmisure150;i++)
    {
        if(Vc_150[i] < 10) errVc_150[i] = Vc_150[i]*0.001+0.005;   //Volt, Amprobe 37X
        if(Vc_150[i] < 1) errVc_150[i] = Vc_150[i]*0.001+0.0005;
	
	if(Ic_150[i] < 100) errIc_150[i] = 0.005*Ic_150[i]+0.05;
        if(Ic_150[i] < 10) errIc_150[i] = 0.005*Ic_150[i]+0.005;    //mA
        if(Ic_150[i] < 1) errIc_150[i] = 0.005*Ic_150[i]+0.0005;
        if(Ic_150[i] < 0.1) errIc_150[i] = (0.005*Ic_150[i]+0.0001);
        
        errIb_150[i] = 5;
        
        if(Vb_150[i] < 1) errVb_150[i] = Vb_150[i]*0.005+0.003;
        
    }
    
    std::cout << "Ic [mA]      errIc [mA]      Vc [V]     errVc [V]   con Ib = 150 uA" << endl;
for (int i =0;i<nmisure150;i++)
    {
	std::cout << Ic_150[i] << "     " << errIc_150[i] <<"       "<< Vc_150[i] << "      " << errVc_150[i] << endl;
	}
    
    // a Ib=150 uA, costruisco ora il grafico Ic(Vc)
    
    TCanvas *cIV_150 = new TCanvas("BJT_150","G(f)",200,10,600,400);
    cIV_150->SetFillColor(0);
    cIV_150->cd();

    TGraphErrors *gIV_150 = new TGraphErrors(nmisure150,Vc_150,Ic_150,errVc_150,errIc_150);
    gIV_150->SetMarkerStyle(8);
    gIV_150->SetMarkerSize(1);
    gIV_150->SetTitle("Ic (Vc) del transistor BJT con Ib=150 uA");
    gIV_150->GetXaxis()->SetTitle("Vc");
    gIV_150->GetYaxis()->SetTitle("Ic");

    gIV_150->Draw("AP");
    
    
    
    
    
    
    
    
    //grafico e dati a corrente 200 uA
    int nmisure200 = 22;
    float Vb_200[] ={0.61,0.613,0.618,0.626,0.645,0.657,0.664,0.672,0.679,0.686,0.694,0.706,0.718,0.725,0.73,0.733,0.734,0.734,0.735,0.735,0.735,0.735};  //Volt
    float Ib_200[] ={200,200,200,200,200, 200, 200, 200, 200, 200, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190};  //microAmpere
    float Vc_200[] ={0.0048,0.0063,0.011,0.0201,0.0409,0.0548,0.0638,0.074,0.0835,0.0943,0.1063,0.125,0.1516,0.1772,0.2013,0.2546,0.3085,0.5006,1.065,2.043,4.134,6.059};  //Volt
    float Ic_200[] ={0.00451,0.006202,0.2775,0.7999,2.625,4.506,6.05,8.131,10.31,13.27,16.86,22.99,31.47,37.89,41.77,45.19,46.04,46.8,48.24,50.26,54.05,58};  //milliAmpere
    
    float errVb_200[nmisure200];
    float errIb_200[nmisure200];
    float errVc_200[nmisure200];
    float errIc_200[nmisure200];

    for (int i =0;i<nmisure200;i++)
    {
        if(Vc_200[i] < 10) errVc_200[i] = Vc_200[i]*0.001+0.005;   //Volt, Amprobe 37X
        if(Vc_200[i] < 1) errVc_200[i] = Vc_200[i]*0.001+0.0005;
	
	if(Ic_200[i] < 100) errIc_200[i] = 0.005*Ic_200[i]+0.05;
        if(Ic_200[i] < 10) errIc_200[i] = 0.005*Ic_200[i]+0.005;    //mA
        if(Ic_200[i] < 1) errIc_200[i] = 0.005*Ic_200[i]+0.0005;
        if(Ic_200[i] < 0.1) errIc_200[i] = (0.005*Ic_200[i]+0.0001);
       
        errIb_200[i] = 5;
        
        if(Vb_200[i] < 1) errVb_200[i] = Vb_200[i]*0.005+0.003;
        
    }
    
    std::cout << "Ic [mA]      errIc [mA]      Vc [V]     errVc [V]   con Ib = 200 uA" << endl;
for (int i =0;i<nmisure200;i++)
    {
	std::cout << Ic_200[i] << "     " << errIc_200[i] <<"       "<< Vc_200[i] << "      " << errVc_200[i] << endl;
	}
    
    // a Ib=200 uA, costruisco ora il grafico Ic(Vc)
    
    TCanvas *cIV_200 = new TCanvas("BJT_200","G(f)",200,10,600,400);
    cIV_200->SetFillColor(0);
    cIV_200->cd();

    TGraphErrors *gIV_200 = new TGraphErrors(nmisure200,Vc_200,Ic_200,errVc_200,errIc_200);
    gIV_200->SetMarkerStyle(8);
    gIV_200->SetMarkerSize(1);
    gIV_200->SetTitle("Ic (Vc) del transistor BJT con Ib=200 uA");
    gIV_200->GetXaxis()->SetTitle("Vc");
    gIV_200->GetYaxis()->SetTitle("Ic");

    gIV_200->Draw("AP");
    
    //per le curve Ib(Vb) a Vc costante. Il problema qui è che abbiamo solo 3 dati per ogni Vc costante, e in gran parte non abbiamo Vc uguale per tutti e tre i set di dati!
    
    //grafico Bf (Ic) a Vc = 6V
    
    float Bf[] ={25.23/0.090,40.53/0.140, 58/0.190};  // Ic/Ib tutto in milliAmpere    #280.43#  289.50  305.26
    float Ic[] ={25.23,40.53,58};
    float Ib[] ={0.090,0.140,0.190};
    float errIc[3];
    float errIb[3];
    float errBf[3];
    
    for (int i =0;i<3;i++)
    {
	if(Ic[i] < 100) errIc[i] = 0.005*Ic[i]+0.05;
        if(Ic[i] < 10) errIc[i] = 0.005*Ic[i]+0.005;    //mA
        
        errIb[i] = 0.005;
        
        errBf[i] = sqrt(  (  pow(Ib[i],2)*pow(errIc[1],2) +     pow(Ic[i],2)*pow(errIb[1],2)        )   /   pow(Ib[i],4)  );
        
       }
    errBf[0]=5;  //ho dovuto definire errBf[0] perchè nel ciclo for viene inf
    TCanvas *cBF = new TCanvas("BJT_Bf(Ic)","G(f)",200,10,600,400);
    cBF->SetFillColor(0);
    cBF->cd();

    TGraphErrors *gBF = new TGraphErrors(3,Ic,Bf,errIc,errBf);
    gBF->SetMarkerStyle(8);
    gBF->SetMarkerSize(1);
    gBF->SetTitle("Bf (Ic) del transistor BJT con Vc=6V");
    gBF->GetXaxis()->SetTitle("Bf");
    gBF->GetYaxis()->SetTitle("Ic");

    gBF->Draw("AP");
	for (int i =0;i<3;i++)
    {
	std::cout << Ic[i] << "     " << errIc[i] <<"       "<< Bf[i] << "      " << errBf[i] << endl;
	}



    return 0;
}
