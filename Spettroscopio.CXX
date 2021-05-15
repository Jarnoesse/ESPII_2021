#include <cmath>

const int N = 300; // passo del reticolo mm^-1

double Media(double array[], int lunghezza_array = 3)
{
    double media = 0;

    for(int i=0;i<lunghezza_array;i++) media = media + array[i];

    media = media/lunghezza_array;

    return media;
}

class Dato
{
public:
    double Gradi;
    double Primi;
    double Valore;

    void trasforma()
    {
        Valore = Gradi + Primi/60;
    }

}
class Onda
{
public:
    double banda_sx[3];
    double banda_dx[3];

    double lambda[3];

    void Assegna_sx(Dato array[], int lunghezza_array =3)
    {
        for(int i = 0; i < lunghezza_array;i++)
        {
            array[i].trasforma();
            banda_sx[i] = array[i].Valore;
        }
    }

    void Calcola()
    {

        for (int i=0; i<3; i++)lambda[i] = sin( abs(banda_dx[i]-banda_sx[i]) * 3.14 / 180 )/((i+1) * N);
    }

};



double Spettroscopio()
{



    return 0;
}
