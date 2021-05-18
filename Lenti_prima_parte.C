#include<iostream>
#include<cmath>
#include<TGraphErrors.h>
#include<TAxis.h>
#include<TCanvas.h>
#include<TF1.h>
#include<TH1F.h>
#include<iomanip>
#include<TRandom3.h>
#include<TMath.h>
#include<TLegend.h>
#include<string>

double Media(double array[],int lunghezza_array = 30)                 //aggiunta lunghezza array per problemi in Varianza
{
  //Calcola la Media degli elementi degli array

    double media = 0;

    if(lunghezza_array == 0)lunghezza_array = *(&array + 1) - array; // questo if è stato aggiunto per problemi nel chiamare questa funzione in Varianza

    for(int i=0;i<lunghezza_array;i++)media = media + array[i];

    media = media / lunghezza_array;

    return media;
}
double Varianza(double array[],int lunghezza_array=30)
{
  //Calcola la varianza dato un array
    double varianza = 0;


    double media = Media(array,lunghezza_array);

    for(int i=0;i<lunghezza_array;i++) varianza = varianza + pow(array[i]-media,2);

    varianza = varianza / (lunghezza_array-1);

   varianza = sqrt(varianza);

    return varianza;

}
double FindMax(double array[],int dimensione=30)
{
    double max=array[0];
    for(int i =1; i< dimensione; i++)if(array[i] > max) max = array[i];

    return max;
}
double FindMin(double array[],int dimensione=30)
{
    double min=array[0];
    for(int i =1; i< dimensione; i++)if(array[i] < min) min = array[i];

    return min;
}
double Distanza_Focale(double distanza_sorgente, double distanza_lente, double dati[], int lunghezza_array = 30)
{

    // Questa funzione calcola in automatico la distanza focale

    double distanza_lenti = distanza_lente - distanza_sorgente + 5;
    double misure[lunghezza_array];

    for(int i=0;i<lunghezza_array;i++) misure[i] = dati[i] - 5;


    for (int i=0;i<lunghezza_array;i++)misure[i] = misure[i] - distanza_sorgente - distanza_lenti; //tolgo l'offset di tutto ai dati


    double distanza_fuoco = Media(misure,lunghezza_array);

    // trovo la distanza focale
    double distanza_focale = 1 / ( 1 / (distanza_fuoco) + 1 / (distanza_lenti) );


     return distanza_focale;
}

double Errore_Distanza_focale(double distanza_sorgente, double distanza_lente, double dati[], int lunghezza_array = 30)
{
    double distanza_lenti = distanza_lente - distanza_sorgente;
    double misure[lunghezza_array];

    for(int i=0;i<lunghezza_array;i++) misure[i] = dati[i];

    for (int i=0;i<lunghezza_array;i++)misure[i] = misure[i] - distanza_sorgente - distanza_lenti; //tolgo l'offset di tutto ai dati


    double distanza_fuoco = Media(misure,lunghezza_array);

    double errore_distanza;
    errore_distanza = Varianza(misure);
    if (errore_distanza < 1) errore_distanza = 1;
    errore_distanza = errore_distanza * 2;

    double errore;

    errore = sqrt( pow( pow( distanza_lenti ,2) / pow( distanza_fuoco + distanza_lenti ,2) ,2) * pow( errore_distanza ,2) + pow( pow( distanza_fuoco ,2) / pow( distanza_fuoco + distanza_lenti ,2),2) * pow(2,2));

    return errore;

}

void Metodo_Variazionale(double dati[],double risultati[],double distanze_lenti[], double distanza_sorgente, int lunghezza_array = 35, int lunghezza_array_2 = 7)
{
    double misure[lunghezza_array];
    double distanza_lenti[lunghezza_array_2];

    for (int i=0; i < lunghezza_array; i++)misure[i] = dati[i] - 5;
    for( int i=0; i < lunghezza_array_2; i++) distanza_lenti[i] = distanze_lenti[i] + 5;

    double errori_distanza_lenti[lunghezza_array_2];                   // creo l'array di errori delle lenti prima di dimenticarmene
    for(int i=0;i<lunghezza_array_2;i++) errori_distanza_lenti[i] = 2; //mm

    double medie[lunghezza_array_2];
    double varianze[lunghezza_array_2];

    int k=0;
    double batch[5];
    for(int i = 0; i < lunghezza_array_2; i++)
    {
        for(int j=0; j<5; j++)
        {
            batch[j] = misure[k] - distanza_lenti[i];
            k++;
        }

        medie[i] = Media(batch,5);
        varianze[i] = Varianza(batch,5);

        distanza_lenti[i] = distanza_lenti[i] - distanza_sorgente; // correggo la distanza lenti

        std::cout << endl << "media " << medie[i] << " +- " << varianze[i];
    }


    for(int i=0;i<lunghezza_array_2;i++)
    {
        errori_distanza_lenti[i] = abs(errori_distanza_lenti[i]/pow(distanza_lenti[i],2));
        distanza_lenti[i] = 1/distanza_lenti[i];


        varianze[i] = abs(varianze[i] / pow(medie[i],2));
        medie[i] = 1 / medie[i];


        std::cout << "roba invertita " << medie[i] << "pm" << varianze[i] << endl;


    }


    //Dichiaro il canvas
    TCanvas *canvas = new TCanvas("Metodo_Variazionale","Metodo variazionale",200,10,600,400);
    canvas->SetFillColor(0);
    canvas->cd();

    //Creo il grafico
    TGraphErrors *grafico = new TGraphErrors(lunghezza_array_2, distanza_lenti, medie , errori_distanza_lenti,varianze);

    //Setto il grafico
    grafico->SetMarkerStyle(8);
    grafico->SetMarkerSize(1);

    grafico->SetTitle("Metodo Variazionale");
    grafico->GetXaxis()->SetTitle("Distanza Lenti[mm]");
    grafico->GetYaxis()->SetTitle("Distanza del fuoco [mm]");

    grafico->Draw("AP");



    TF1 *funzione = new TF1("funzione","[0]+[1]*x",FindMin(distanza_lenti,lunghezza_array_2), FindMax(distanza_lenti,lunghezza_array_2));

        funzione->SetParameter(1,-1);

    grafico->Fit(funzione,"RM+");


}
void IsGaussian(double misure[],char nome[],bool multifunc = false, int lunghezza_array = 30, int nbins = 10)
{
    double media = Media(misure);

     TH1 *istogramma = new TH1D("h1","h1",nbins,FindMin(misure)-5,FindMax(misure)+5);

    for(int i=0;i < lunghezza_array; i++)
    {
        istogramma->Fill(misure[i]);                              // inserisco i valori nell' istogramma
    }

    TCanvas *canvas = new TCanvas(nome,nome,200,10,600,400); //Creo il canvas

    TF1 *gauss = new TF1("gauss","gaus",FindMin(misure)-5,FindMax(misure)+5);
    TF1 *multigauss1 = new TF1("multigauss","gaus",FindMin(misure)-5,media);
    TF1 *multigauss2 = new TF1("multigauss2","gaus",media,FindMax(misure)+5);
    TF1 *somma = new TF1("somma","gaus(0)+gaus(3)",FindMin(misure)-5,FindMax(misure)+5);

    canvas->SetFillColor(0);
    canvas->cd();

    if(multifunc == false)
    {
        gauss->SetParameter(1,media);
        istogramma->Draw();
        istogramma->Fit(gauss,"R+");
        std::cout << "Fit gaussiano P=" << gauss->GetProb()<<endl<<endl;
    }
    else
    {
        multigauss1->SetParameter(1,1070);
        multigauss2->SetParameter(1,1045);
        istogramma->Draw();
        istogramma->Fit(multigauss1,"R+");
        istogramma->Fit(multigauss2,"R+");

        Double_t par[6];
        multigauss1->GetParameters(&par[0]);
        multigauss2->GetParameters(&par[3]);
        somma->SetParameters(par);
        istogramma->Fit(somma,"R+");
        std::cout << "Fit gaussiano1 P=" << somma->GetProb()<<endl;
    }
}
/*
void tStudent(int numeroEsperimenti, int neventi, const double & nbins = 40, const double& minb = -10, const double& maxb = 10) {

  // valor vero e sigma
  double valor_vero  = 235;
  double sigma = 5;

  // --------------------------------------------------------- //

  // Generatore di eventi casuali (http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/ARTICLES/mt.pdf)
  TRandom3 *random = new TRandom3();

  double variabili_t[numeroEsperimenti];

  // Referenza calcolo varianza in un passaggio: http://www.jstor.org/stable/1266577?seq=1#page_scan_tab_contents
  for (int esp = 0; esp < numeroEsperimenti; ++esp){
    double delta = 0.;
    double media = 0.;
    double M2    = 0.;
    for (int i=0; i<neventi; ++i){
      double d = random->Gaus(valor_vero,sigma);
      delta = d - media;
      media += delta/(i+1);
      M2    += delta*(d-media);
    }
    double std = neventi > 1 ? sqrt(M2/(neventi-1)) : -9.;
    variabili_t[esp] = (media - valor_vero)/(std/sqrt(neventi));
    // Se anziché usare la deviazione standard campionaria si usasse la deviazione standard vera, allora t si distribuirebbe secondo la statistica di
    // Gauss anche per poche misure. Per verificarlo, provare a usare la riga qui sotto anziché quella sopra.
    // variabili_t[esp] = (media - valor_vero)/(sigma/sqrt(neventi));
  }

  TCanvas *ct = new TCanvas("t","t",200,10,600,400);
  ct->cd();

  // Istanza dell'oggetto istogramma (https://root.cern.ch/doc/master/classTH1F.html)
  TH1F *htStudent = new TH1F("sStudent","tStudent",nbins, minb, maxb);
  htStudent->SetStats(kFALSE);
  htStudent->GetXaxis()->SetTitle("t");
  htStudent->GetYaxis()->SetTitle("Conteggi");

  for(int esp=0; esp<numeroEsperimenti;++esp)
    htStudent->Fill(variabili_t[esp]);


  htStudent->Draw();
  htStudent->Fit("gaus","ME");
  TF1 *fit = htStudent->GetFunction("gaus");
  std::cout << "Chi^2:" << fit->GetChisquare() << ", number of DoF: " << fit->GetNDF() << " (Probability: " << fit->GetProb() << ")." << endl;
}

*/

int Lenti_prima_parte()
{

    //LENTE CONVERGENTE

    // PRIMA PARTE
    double distanza_sorgente = 300;
    double distanza_lenti = 412;

   double Q1[] = {1060,1045,1038,1081,1060,1064,1058,1070,1075,1053,1056,1057,1062,1053,1040,1059,1054,1046,1068,1058,1040,1066,1070,1067,1048,1066,1037,1087,1051,1069};
   double Q2[] = {1036,1046,1039,1047,1068,1081,1050,1049,1062,1068,1044,1036,1069,1038,1071,1058,1065,1041,1057,1064,1072,1054,1069,1040,1074,1052,1064,1072,1070,1076};

   char nome1[] = {'I','s','t','o','g','r','a','m','m','a'}; // non mi da errore negli hist
   char nome2[] = {'S','e','c','o','n','d','o',};

   IsGaussian(Q1,nome1,false,30,8);

   IsGaussian(Q2,nome2,true,30,9);


    double distanza_focale = Distanza_Focale(distanza_sorgente,distanza_lenti,Q1);
    double errore = Errore_Distanza_focale(distanza_sorgente,distanza_lenti,Q1);
    std::cout << "Misura " << distanza_focale << " +- " << errore << endl << endl;

    distanza_focale = Distanza_Focale(distanza_sorgente,distanza_lenti,Q2);
    errore = Errore_Distanza_focale(distanza_sorgente,distanza_lenti,Q2);
    std::cout << "Misura " << distanza_focale << " +- " << errore << endl << endl;


    // SECONDA PARTE
    double Q3[] = {1081,1058,1062,1036,1081,  961,966,957,945,958,  864,874,864,895,881, 818,831,834,820,833,  784,801,787,807,794,  748,753,747,745,754,  725,727,728,725,728};
    double distanze_lenti[] = {410,415,420,425,430,440,450};
    double risultati[2];

    Metodo_Variazionale(Q3,risultati,distanze_lenti,300);



    //ULTIMA PARTE
    double Q4[] = {836,839,841,835,835,  810,821,822,823,810,  866,869,870,863,858,  891,889,888,892,885,  859,858,851,857,855,  1567,1504,1512,1545,1531,  828,829,824,828,828};
    double distanze_lenti_parte_2[] = {480,490,470,465,475,430,485};

    Metodo_Variazionale(Q4,risultati,distanze_lenti_parte_2,300);



    return 0;
}
