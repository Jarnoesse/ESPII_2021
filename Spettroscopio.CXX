#include <cmath>
#include <string.h>
#include<TGraphErrors.h>
#include<TAxis.h>
#include<TCanvas.h>
#include<TF1.h>
#include<iomanip>


const int N = 300; // passo del reticolo mm^-1.
const double ALPHA = 1.047197551; // angolo del prisma alpha, radianti.

double Media(double array[], int lunghezza_array = 3)
{
    //Calcola la media.
    double media = 0;

    for(int i=0;i<lunghezza_array;i++) media = media + array[i];

    media = media/lunghezza_array;

    return media;
}
double Varianza(double array[],int lunghezza_array=3)
{
    //Calcola la varianza dato un array.
    double varianza = 0;


    double media = Media(array,lunghezza_array);

    for(int i=0;i<lunghezza_array;i++) varianza = varianza + pow(array[i]-media,2);

    varianza = varianza / (lunghezza_array-1);

    varianza = sqrt(varianza);

    return varianza;

}
int FindMax(int array[],int lunghezza = 7)
{
    int max = array[0];

    for (int i=1;i < lunghezza; i++) if (array[i] > max) max = array[i];

    return max;
}
double Errore_media(double array[], int lunghezza_array=3)
{
    //questa funzione è anta dal fatto che non mi piaceva vedere il calcolo fatto a linea, così è molto più elegante.
    double errore;

    for(int i=0; i< lunghezza_array; i++) errore = errore + array[i];

    errore = errore / sqrt(lunghezza_array);

    return errore;
}
void Grafico(TCanvas *canvas,TGraphErrors *grafico,TF1 *funzione)
{
    canvas->SetFillColor(0);
    canvas->cd();

    std::cout << "--------FIT IN CORSO ------" << endl;
    grafico->SetMarkerStyle(8);
    grafico->SetMarkerSize(1);
    grafico->SetTitle("Indice di Rifrazione del vetro");
    grafico->GetYaxis()->SetTitle("Indice di Rifrazione");
    grafico->GetXaxis()->SetTitle("Lunghezza d'onda [nm]");
    grafico->GetXaxis()->SetLimits(380,630); // setto manualmente i limiti se no veniva troppo zoommato
    grafico->GetYaxis()->SetLimits(1,2);
    grafico->Draw("AP");
    grafico->Fit(funzione,"RM+");
    std::cout <<"Chi^2= " << funzione->GetChisquare() << " P= " << funzione->GetProb() << endl;


}



struct Misura // Proviamo a vedere se così funziona.
{
    double valore;
    double errore=0;
};



class Reticolo
{
public:

    Misura angolo_sx[3];      //array di misure relativo all' angolo della banda sx.
    Misura angolo_dx[3];      //array di misure relativo all' angolo della banda dx.
    Misura lunghezza_onda;    //array che dovrebbe contenere il risultato dell operazione Calcola Lunghezza.
    Misura alpha;             //Lo dichiaro come misura.
    string colore;            //questo dovrebbe contenere il nome del colore che l'onda dovrebbe rappresentare.
    string ordine[3] = {"primo ordine", "secondo ordine", "terzo ordine"};

    void Assegna(double gradi_sx[],double primi_sx[], double gradi_dx[],double primi_dx[], int lunghezza_array=3)
    {
        for(int i=0; i < lunghezza_array; i++)
        {

             angolo_sx[i].valore = gradi_sx[i] + primi_sx[i]/60 - 360; //semplice assegnazione dei valori applicata la correzione per l' angolo grande se no uscivo scemo.
             angolo_dx[i].valore = gradi_dx[i] + primi_dx[i]/60;
        }
    }
    void CalcolaLunghezza()
    {
        double lunghezza_onda_temp[3];
        double errore[3];

        for(int i=0;i<3;i++)
        {
            lunghezza_onda_temp[i] = sin( abs( angolo_sx[i].valore - angolo_dx[i].valore )*3.1415/360) / ((i+1)*N); //formula.
            lunghezza_onda_temp[i] = lunghezza_onda_temp[i] * 1.0E6;                                                //riscalo per i nanometri che sono meglio se si parla di lunghezze d'onda.

            errore[i] = sqrt( pow( cos(abs( angolo_sx[i].valore - angolo_dx[i].valore )*3.1415/360 )*3.1415/((i+1)*N*360),2)*pow(0.5,2));
            errore[i] = errore[i] * 1.0E6;

            std::cout << colore << " " << ordine[i] << " " << lunghezza_onda_temp[i] <<" +- " << errore[i] << "nm"  << endl;

        }

        lunghezza_onda.valore = Media(lunghezza_onda_temp);
        lunghezza_onda.errore = Errore_media(errore);                                                                     // l'idea era propagare l'errore della media.
        if (lunghezza_onda.errore < Varianza(lunghezza_onda_temp)) lunghezza_onda.errore = Varianza(lunghezza_onda_temp); // semplice verifica se l'errore da propagazione è maggiore della varianza.


        std::cout << endl << "MISURA FINALE " << colore << " "  << lunghezza_onda.valore<<" +- " << lunghezza_onda.errore << "nm"  << endl << endl;
    }

};

class Prisma
{
public:
    Misura angolo_deviazione_minima;
    Misura indice_di_rifrazione;
    Misura alpha;
    string nome;

    void Assegna(double gradi[],double primi[], int lunghezza_array=3)
    {
        double angolo[lunghezza_array];                      //messo 4 perchè il dinamico non gli piaceva...
        for (int i=0; i < lunghezza_array; i++)
        {
            angolo[i] = 360 - gradi[i] - primi[i]/60;
            angolo[i] = angolo[i] * 3.14/180;
        }

        double errore = 3.14; //questo messo perchè root non sa fare i conti
        errore = errore/60;
        errore = errore * 3.14;
        errore = errore/180;

        angolo_deviazione_minima.valore = Media(angolo,lunghezza_array);
        angolo_deviazione_minima.errore = errore;

        if (Varianza(angolo,lunghezza_array) > angolo_deviazione_minima.errore)angolo_deviazione_minima.errore = Varianza(angolo,lunghezza_array);

        std::cout << nome << " " << angolo_deviazione_minima.valore << " pm " << angolo_deviazione_minima.errore << endl << endl;

        alpha.valore = ALPHA;
        alpha.errore = 1/60;

    }
    void CalcolaIndice()
    {
        indice_di_rifrazione.valore = sin( (angolo_deviazione_minima.valore + alpha.valore)/2 ) / sin(alpha.valore/2);
        indice_di_rifrazione.errore = sqrt(pow(0.5*cos((angolo_deviazione_minima.valore + alpha.valore)/2) / sin(alpha.valore/2),2)*pow(angolo_deviazione_minima.errore,2)+pow(0.5*1/sin(alpha.valore/2)*(cos((angolo_deviazione_minima.valore+alpha.valore)/2)-1/tan(alpha.valore/2)*sin((angolo_deviazione_minima.valore+alpha.valore)/2)),2)*pow(alpha.errore,2));
                std::cout<<"Indice di rifrazione " << indice_di_rifrazione.valore << " pm " << indice_di_rifrazione.errore << endl << endl;
    }

};

int ParteI(Reticolo classe[], double sx_gradi[], double sx_primi[], double dx_gradi[], double dx_primi[],string nomi[], int  n_linee = 7,int dimensione_array = 21, int dimensione_batch = 3)
{
    std::cout << "--PRIMA PARTE--" << endl << endl;
    double batch_sx_gradi[dimensione_batch]; // non è memory friendly ma almeno nel main non ho 200 array.
    double batch_dx_gradi[dimensione_batch];
    double batch_sx_primi[dimensione_batch];
    double batch_dx_primi[dimensione_batch];
    int k=0;                                //indice che si muove sull' array grande.

    for(int i=0;i < n_linee;i++)            //mi muovo sulle classi.
    {
        for(int j=0; j < dimensione_batch; j++)
        {
            batch_sx_gradi[j] = sx_gradi[k];
            batch_dx_gradi[j] = dx_gradi[k];
            batch_sx_primi[j] = sx_primi[k];
            batch_dx_primi[j] = dx_primi[k];
            k++;
        }
        classe[i].Assegna(batch_sx_gradi,batch_sx_primi,batch_dx_gradi,batch_dx_primi);
        classe[i].colore = nomi[i];
        classe[i].CalcolaLunghezza();
    }
    return 0;
}
int ParteII(Prisma classe[],Reticolo reticolo[], double gradi[],double primi[],int lunghezze_misure[],int n_linee,string nomi[])
{
    std::cout << "--SECONDA PARTE--" << endl << endl;
    int max = FindMax(lunghezze_misure);
    int k = 0;
    double batch_gradi[4];
    double batch_primi[4];

    for(int i=0;i<6;i++)
    {
        for(int j=0; j < lunghezze_misure[i] ; j++)
        {
            batch_gradi[j] = gradi[k];
            batch_primi[j] = primi[k];
            k++;
        }
        classe[i].nome = nomi[i];
        classe[i].Assegna(batch_gradi,batch_primi,lunghezze_misure[i]);
        classe[i].CalcolaIndice();


    }
    double indici[n_linee];
    double errori_indici[n_linee];
    double lunghezze_onda[n_linee];
    double errori_lunghezze_onda[n_linee];
    k=0;

    for(int i=0; i < n_linee;i++)
    {
        if(i==4)
        {
            indici[i]         = classe[i].indice_di_rifrazione.valore;
            errori_indici[i]  = classe[i].indice_di_rifrazione.errore;
            lunghezze_onda[i] = reticolo[k].lunghezza_onda.valore;
            errori_lunghezze_onda[i] = reticolo[k].lunghezza_onda.errore;
            k=k+2;
        }
        else
        {
            indici[i]         = classe[i].indice_di_rifrazione.valore;
            errori_indici[i]  = classe[i].indice_di_rifrazione.errore;
            lunghezze_onda[i] = reticolo[k].lunghezza_onda.valore;
            errori_lunghezze_onda[i] = reticolo[k].lunghezza_onda.errore;
            k++;
        }

    }

    TCanvas *canvas       = new TCanvas("Indice di Rifrazione del vetro in funzione della frequenza","Indice di Rifrazione del vetro in funzione della frequenza",200,10,600,400);
    TGraphErrors *grafico = new TGraphErrors(n_linee,lunghezze_onda,indici,errori_lunghezze_onda,errori_indici);
    TF1 *funzione         = new TF1("funzione","[0]+[1]/(x^2)+[2]/(x^4)",400,700);
    funzione->SetParameter(0,1.6700);
    funzione->SetParameter(1,743);
    Grafico(canvas,grafico,funzione);

    return 0;
}

int PlottaOnde(Reticolo reticolo[],int n_linee)
{
    n_linee = n_linee +1;
    double valore_linea[n_linee];
    double errore_linea[n_linee];
    double y[n_linee];
    double err_y[n_linee];


    for(int i=0;i<n_linee;i++)
    {
        valore_linea[i] = reticolo[i].lunghezza_onda.valore;
        errore_linea[i] = reticolo[i].lunghezza_onda.errore;
        y[i] = 1;
        err_y[i] = 0;
        std::cout <<"TEST " << reticolo[i].lunghezza_onda.valore << " +- " << reticolo[i].lunghezza_onda.valore << endl << endl;
    }
    TCanvas *canvas2       = new TCanvas("Linee","Linee",200,10,600,400);
    TGraphErrors *grafico2 = new TGraphErrors(n_linee,valore_linea,y,errore_linea,err_y);

    grafico2->SetMarkerStyle(8);
    grafico2->SetMarkerSize(1);
    grafico2->SetTitle("Linee di emissione");
    grafico2->GetXaxis()->SetTitle("Lunghezza d'onda [nm]");

    grafico2->Draw("AP");

    return 0;


}


double Spettroscopio()
{
    // PRIMA PARTE
    int n_linee = 7;
    Reticolo LineaI[n_linee];

    string nomiI[] = {"Viola I","Blu I", "Verde Acqua I", "Verde Fluo I", "Giallo I", "Giallo II", "Rosso I" };
    double sx_gradi[] = {353,346,339, 352,345,337, 351,343,334, 350,341,331, 350,340,329, 350,340,329, 349,338,327};
    double sx_primi[] = {0,8,5, 0,6,30, 30,9,29, 40,15,26, 7,12,41, 5,7,37, 21,37,59 };
    double dx_gradi[] = {6,13,21, 7,15,23, 8,17,26, 9,19,29, 9,20,31, 9,20,31, 10,22,34};
    double dx_primi[] = {49,55,20, 21,4,7, 24,5,21, 23,5,39, 54,13,34, 57,19,42, 40,0,1};

    ParteI(LineaI,sx_gradi,sx_primi,dx_gradi,dx_primi,nomiI);

    //SECONDA PARTE
    n_linee = 6;
    Prisma LineaII[n_linee];

    string nomiII[] = {"Viola I", "Blu I", "Verde Acqua I", "Verde Fluo I", "Giallo I e II", "Rosso I"};
    int lunghezze_array[] = {4,3,3,3,3,3};
    double gradi[] = {308,308,308,308, 309,309,309, 310,310,310, 311,311,311, 311,311,311, 312,312,312};
    double primi[] = {46,58,51,19, 43,44,45, 50,49,49, 30,30,30, 49,48,49, 10,11,11}; //maggiore errrore nella prima riconducibile al mio occhio che non si era ancora abituato?

    ParteII(LineaII,LineaI,gradi,primi,lunghezze_array,n_linee,nomiII);


    PlottaOnde(LineaI,n_linee);



    return 0;
}
