#ifndef STRUCT_H_INCLUDED
#define STRUCT_H_INCLUDED

//La struttura che segue memorizza i dati
//di ciascuna risorsa relativamente al giorno
//selezionato
struct info_risorse
{
    int posiz;
    char mostra;
	char nomeconv[27];
    long int aperto,chiuso;
    double saldoinanno;
    double saldoinimese;
    double saldooggi;
    double saldofinmese;
    double saldofinanno;
    double saldofinannoold;
    double entrateinianno;
    double entrateinimese;
    double entrateoggi;
    double entratefinmese;
    double entratefinanno;
    double usciteinianno;
    double usciteinimese;
    double usciteoggi;
    double uscitefinmese;
    double uscitefinanno;
    double gaininianno;
    double gaininimese;
    double gainoggi;
    double gainfinmese;
    double gainfinanno;
    char casi;
};

//La struttura che segue memorizza i dati
//essenziali per i calcoli "ad hoc"
struct info_smaller
{
    int posiz;
    double gain;
    double entrate;
    double uscite;
};

//La struttura che segue memorizza i dati
//di ciascun movimento del giorno selezionato
struct info_movimenti
{
    int posiz_da;
    int posiz_in;
    int numop_a;
    int numop_b;
    long int numg_a;
    long int numg_b;
};


#endif // STRUCT_H_INCLUDED
