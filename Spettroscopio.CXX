#include <cmath>
#include <string.h>

const int N = 300; // passo del reticolo mm^-1

double Media(double array[], int lunghezza_array = 3)
{
    double media = 0;

    for(int i=0;i<lunghezza_array;i++) media = media + array[i];

    media = media/lunghezza_array;

    return media;
}

struct Misura // Proviamo a vedere se così funziona
{
    double valore;
    double errore=0;
};

class Onda
{
public:

    Misura angolo_sx[3];      //array di misure relativo all' angolo della banda sx
    Misura angolo_dx[3];      //array di misure relativo all' angolo della banda dx
    Misura lunghezza_onda[3]; //array che dovrebbe contenere il risultato dell operazione Calcola Lunghezza.
    string colore;            //questo dovrebbe contenere il nome del colore che l'onda dovrebbe rappresentare
    string ordine[3] = {"primo ordine", "secondo ordine", "terzo ordine"};

    void Assegna(double gradi_sx[],double primi_sx[], double gradi_dx[],double primi_dx[], int lunghezza_array=3)
    {
        for(int i=0; i < lunghezza_array; i++)
        {

             angolo_sx[i].valore = gradi_sx[i] + primi_sx[i]/60 - 360; //semplice assegnazione dei valori applicata la correzione per l' angolo grande se no uscivo scemo
             angolo_dx[i].valore = gradi_dx[i] + primi_dx[i]/60;
        }
    }
    void CalcolaLunghezza()
    {
        for(int i=0;i<3;i++)
        {
            lunghezza_onda[i].valore = sin( abs( angolo_sx[i].valore - angolo_dx[i].valore )*3.1415/360) / ((i+1)*N); //formula
            lunghezza_onda[i].valore = lunghezza_onda[i].valore * 1.0E6;                                              //riscalo per i nanometri che sono meglio se si parla di lunghezze d'onda

            lunghezza_onda[i].errore = sqrt( pow( cos(abs( angolo_sx[i].valore - angolo_dx[i].valore )*3.1415/360 )*3.1415/((i+1)*N*360),2)*pow(1,2));
            lunghezza_onda[i].errore = lunghezza_onda[i].errore * 1.0E6;

            std::cout << colore << " " << ordine[i] << " " << lunghezza_onda[i].valore<<" +- " << lunghezza_onda[i].errore << "nm"  << endl;
        }
    }

};

void Assegna(Onda classe[], double sx_gradi[], double sx_primi[], double dx_gradi[], double dx_primi[],string nomi[], int  n_linee = 7,int dimensione_array = 21, int dimensione_batch = 3)
{
    double batch_sx_gradi[dimensione_batch]; // non è memory friendly ma almeno nel main non ho 200 array
    double batch_dx_gradi[dimensione_batch];
    double batch_sx_primi[dimensione_batch];
    double batch_dx_primi[dimensione_batch];
    int k=0;                    //indice che si muove sull' array grande

    for(int i=0;i < n_linee;i++) //mi muovo sulle classi
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
}

double Spettroscopio()
{
    int n_linee = 7;
    Onda Linea[7];

    string nomi[] = {"Viola I","Blu I", "Verde Acqua I", "Verde Fluo I", "Giallo I", "Giallo II", "Rosso" };
    double sx_gradi[] = {353,346,339, 352,345,337, 351,343,334, 350,341,331, 350,340,329, 350,340,329, 349,338,327};
    double sx_primi[] = {0,8,5, 0,6,30, 30,9,29, 40,15,26, 7,12,41, 5,7,37, 21,37,59 };
    double dx_gradi[] = {6,13,21, 7,15,23, 8,17,26, 9,19,29, 9,20,31, 9,20,31, 10,22,34};
    double dx_primi[] = {49,55,20, 21,4,7, 24,5,21, 23,5,39, 54,13,34, 57,19,42, 40,0,1};

    Assegna(Linea,sx_gradi,sx_primi,dx_gradi,dx_primi,nomi);

    return 0;
}
