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
    Misura lunghezza_onda[3]; //arraya che dovrebbe contenere il risultato dell operazione Calcola Lunghezza.
    string colore;            //questo dovrebbe contenere il nome del colore che l'onda dovrebbe rappresentare

    void Assegna(double array_sx[], double array_dx[], int lunghezza_array=3)
    {
        for(int i=0; i < lunghezza_array; i++)
        {
             angolo_sx[i].valore = 360 - array_sx[i]; //semplice assegnazione dei valori applicata la correzione per l' angolo grande se no uscivo scemo
             angolo_dx[i].valore = array_dx[i];
             //VANNO AGGIUNTI GLI ERRORI NON DIMENTICARTELO
             //AH E METTI GIA LA CONVERSIONE DEI GRADI QUI COSI HAI PIU PRECISIONE NEI DECIMALI
             //QUINDI FORSE CAMBIA GLI ARRAY DA DOUBLE A MISURA
        }
    }
    void CalcolaLunghezza()
    {
        for(int i=0;i<3;i++)
        {
            lunghezza_onda[i].valore = sin( abs( angolo_sx[i].valore - angolo_dx[i].valore )*3.14/360) / ((i+1)*N); //formula
            lunghezza_onda[i].valore = lunghezza_onda[i].valore * 1.0E6;

            std::cout << "finale " << colore << " " << lunghezza_onda[i].valore << "nm"  << endl;
        }
    }

};



double Spettroscopio()
{
    Onda Violetto;
    Violetto.colore = "Violetto";

    double sx[] = {353, 346.133,339.083};
    double dx[] = {-6.908,-13.89,-21.125};

    Violetto.Assegna(sx,dx); // per ora questo è un test!!!!
    Violetto.CalcolaLunghezza();



    return 0;
}
