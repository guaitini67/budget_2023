#include "globfun.h"

// ***************************************************************************************
// ************************ SEZIONE DEFINIZIONE FUNZIONI *********************************
// ***************************************************************************************

extern char stringa[],stringa1[],nomeconv[],mot_f[100],cat_f[100],fnam1[],fnam2[];

extern int i,j,k,nrighe,posiz,nanni,numfonti,g,m,a,num_ops,pa,pb,month[];

extern long int numg_fa,numg_fb;
extern double importo_f,saldo,importo;
extern double saldo;

extern time_t curtime;
extern struct tm *data_ora;


extern FILE *fil,*tmp,*ntt,*mapp,*f1,*f2,*logg;


int controllabasedata(void)
//Scorre il basedata.dat per intero
//per capire se e' formattato bene
{

    char stringa[401];

    if(!(mapp=fopen("basedata.dat","r")))
    {
        //Non trovo basedata.dat: errore
        return -1;
    }
    //Adesso cambio directory e ci creo tempor
    if(chdir("budback")<0)
    {
        //In caso di errore...
        fclose(mapp);
        return -100;
    }
    tmp=fopen("tempor","w");
    if(!fgets(stringa,400,mapp))
    {
        //Non trovo niente in basedata.dat: errore
        fclose(mapp);
        fclose(tmp);
        chdir("..");
        return -2;
    }
    if(sscanf(stringa,"%d\n",&numfonti)!=1)
    {
        //Formattazione non corretta: errore
        fclose(mapp);
        fclose(tmp);
        chdir("..");
        return -3;
    }
    //Tutto OK, copio su temp
    fputs(stringa,tmp);
    for(i=0;i<numfonti;i++)
    {
        if(!fgets(stringa,400,mapp))
        {
            //Mi aspettavo di leggere una riga e non la trovo: errore
            fclose(mapp);
            fclose(tmp);
            chdir("..");
            return -4;
        }
        // nel vecchio programma lo sscanf era come mostrato qui sotto,
        // ma ora mi va in crash percio' ho eliminato la seconda serie di caratteri
        // che comunque non dovrebbero essere mai usati
        // sscanf(stringa,"%d,%[!-~ ΅-ÿ‘’]\n",&posiz,nomeconv);
        if(sscanf(stringa,"%d,%[!-~ ‘’@ςΰωθιμ°η§]\n",&posiz,nomeconv)!=2)
        {
            //Formattazione non corretta: errore
            fclose(mapp);
            fclose(tmp);
            chdir("..");
            return -5;
        }
        //Tutto OK, copio su temp
        fputs(stringa,tmp);
        //Leggo il giorno di apertura (se c'e')
        if(!fgets(stringa,400,mapp))
        {
            //Mi aspettavo di leggere una riga e non la trovo: errore
            fclose(mapp);
            fclose(tmp);
            chdir("..");
            return -6;
        }
        //Cerco le informazioni sul giorno di apertura
        if(!strcmp(stringa,"nil\n"))
            //Ho trovato nil: non c'e' giorno di apertura
            //Tutto OK, copio su temp
            fputs(stringa,tmp);
        else if(sscanf(stringa,"%02d-%02d-%04d\n",&g,&m,&a)==3)
            //C'e' il giono di apertura
            //Tutto OK, copio su temp
            fputs(stringa,tmp);
        else
        {
            //Formattazione non corretta: errore
            fclose(mapp);
            fclose(tmp);
            chdir("..");
            return -7;
        }
        //Leggo il giorno di chiusura (se c'e')
        if(!fgets(stringa,400,mapp))
        {
            fclose(mapp);
            fclose(tmp);
            chdir("..");
            return -8;
        }
        //Cerco le informazioni sul giorno di chiusura
        if(!strcmp(stringa,"nil\n"))
            //Ho trovato nil: non c'e' giorno di chiusura
            //Tutto OK, copio su temp
            fputs(stringa,tmp);
        else if(sscanf(stringa,"%02d-%02d-%04d\n",&g,&m,&a)==3)
            //C'e' il giono di chiusura
            //Tutto OK, copio su temp
            fputs(stringa,tmp);
        else
        {
            //Formattazione non corretta: errore
            fclose(mapp);
            fclose(tmp);
            chdir("..");
            return -9;
        }
        //Leggo quante righe di commento e quanti anni ci sono
        if(!fgets(stringa,400,mapp))
        {
            //Mi aspettavo di leggere una riga e non la trovo: errore
            fclose(mapp);
            fclose(tmp);
            chdir("..");
            return -10;
        }
        if(sscanf(stringa,"%d,%d\n",&nrighe,&nanni)!=2)
        {
            //Formattazione non corretta: errore
            fclose(mapp);
            fclose(tmp);
            chdir("..");
            return -11;
        }
        //Tutto OK, copio su temp
        fputs(stringa,tmp);
        for(j=0;j<nrighe;j++)
        {
            if(!fgets(stringa,400,mapp))
            {
                //Mi aspettavo di leggere una riga e non la trovo: errore
                fclose(mapp);
                fclose(tmp);
                chdir("..");
                return -12;
            }
            //Tutto OK, copio su temp
            fputs(stringa,tmp);
        }
        for(j=0;j<nanni;j++)
        {
            if(!fgets(stringa,400,mapp))
            {
                //Mi aspettavo di leggere una riga e non la trovo: errore
                fclose(mapp);
                fclose(tmp);
                chdir("..");
                return -13;
            }
            if(sscanf(stringa,"%d,%lf\n",&a,&saldo)!=2)
            {
                //Formattazione non corretta: errore
                fclose(mapp);
                fclose(tmp);
                chdir("..");
                return -14;
            }
            //Tutto OK, copio su temp
            fputs(stringa,tmp);
        }
    }
    //Tutto OK: ritorno corretto
    fclose(mapp);
    fclose(tmp);
    chdir("..");
    return 1;
}

int caricav(info_risorse *tabella_risorse,int *numfonti)
{
    char stringa[401],nomeconv[27];

    int i,j,k,posiz,g,m,a,nrighe,nanni;

    FILE *mapp;
    if((k=controllabasedata())<0)
    {
        //In caso di errore
        return k;
    }
    //Se sono qui basedata.dat e' a posto. Non faccio
    //piu' nessun controllo
    mapp=fopen("basedata.dat","r");
    fgets(stringa,400,mapp);
    sscanf(stringa,"%d\n",numfonti);
    for(i=0;i<(*numfonti);i++)
    {
        fgets(stringa,400,mapp);
        sscanf(stringa,"%d,%[!-~ ‘’@ςΰωθιμ°η§]\n",&posiz,nomeconv);
        //Di default tutte le risorse sono mostrabili
        //nel campo di riepilogo
        tabella_risorse[i].mostra=1;
        //Ho superato quanto c'era prima
        //Copio il numero risorsa e il nome nel record
        tabella_risorse[i].posiz=posiz;
        strcpy(tabella_risorse[i].nomeconv,nomeconv);
        //Leggo il giorno di apertura (se c'e')
        fgets(stringa,400,mapp);
        //Cerco le informazioni sul giorno di apertura
        if(!strcmp(stringa,"nil\n"))
            //Ho trovato nil: non c'e' giorno di apertura
            //Scrivo -1 nel record
            tabella_risorse[i].aperto=-1;
        else
        {
            //C'e' il giono di apertura e lo scrivo nel record
            //(notare il cast al secondo membro)
            sscanf(stringa,"%02d-%02d-%04d\n",&g,&m,&a);
            tabella_risorse[i].aperto=
            (long int)a*10000+m*100+g;
        }
        //Leggo il giorno di chiusura (se c'e')
        fgets(stringa,400,mapp);
        //Cerco le informazioni sul giorno di chiusura
        if(!strcmp(stringa,"nil\n"))
            //Ho trovato nil: non c'e' giorno di chiusura
            //Scrivo -1 nel record
            tabella_risorse[i].chiuso=-1;
        else
        {
            //C'e' il giono di chiusura e lo scrivo nel record
            //(notare il cast al secondo membro)
            sscanf(stringa,"%02d-%02d-%04d\n",&g,&m,&a);
            tabella_risorse[i].chiuso=(long int)a*10000+m*100+g;
        }
        //Leggo quante righe di commento e quanti anni ci sono
        fgets(stringa,400,mapp);
        sscanf(stringa,"%d,%d\n",&nrighe,&nanni);
        for(j=0;j<nrighe+nanni;j++)
        fgets(stringa,400,mapp);
    }
    fclose(mapp);
    //Aggiungo alla fine della lista un elemento globale
    sprintf(tabella_risorse[(*numfonti)].nomeconv,"Globale");
    //Questa assegnazione e' arbitraria
    tabella_risorse[(*numfonti)].posiz=0;
    return 0;
}

int controllafiledati(char *nome_file)
//Scorre il file dei dati per intero
//per capire se e' formattato bene
{
    char stringa[401];

    int i,j,k,num_ops,nrighe;
    long int numg_fa;
    FILE *fil,*tmp;

    if(!(fil=fopen(nome_file,"r")))
        //Se non riesco ad aprirlo ci deve essere stato qualche
        //problema perche' se sono qui dovrebbe esistere
        return -1;
    if(chdir("budback")<0)
    {
        //In caso di errore...
        fclose(fil);
        return -100;
    }
    tmp=fopen("tempor","w");
    while(fgets(stringa,400,fil))
    {
        if(sscanf(stringa,"%ld,%d,%d\n",&numg_fa,&num_ops,&nrighe)!=3)
        {
            //Trovo i dati in un formato non valido: errore
            fclose(fil);
            fclose(tmp);
	        chdir("..");
            return -2;
        }
        //Tutto OK, copio su temp
        fputs(stringa,tmp);
        for(i=0;i<num_ops;i++)
        {
            if(!fgets(stringa,400,fil))
            {
                //Mi aspettavo una riga e invece non trovo niente:
                //errore
                fclose(fil);
                fclose(tmp);
                chdir("..");
                return -3;
            }
            if(sscanf(stringa,"%d,%lf,%[!-~ ‘’@ςΰωθιμ°η§]\t,%[!-~ ‘’@ςΰωθιμ°η§]\t,%d,%d,%ld,%d\n",&j,&importo_f,mot_f,cat_f,&pa,&pb,&numg_fb,&k)!=8)
            {
                //Trovo i dati in un formato non valido: errore
                fclose(fil);
                fclose(tmp);
                chdir("..");
                return -4;
            }
            //Tutto OK, copio su temp
            fputs(stringa,tmp);
        }
        for(i=0;i<nrighe;i++)
        {
            if(!fgets(stringa,400,fil))
            {
                //Mi aspettavo una riga e invece non trovo niente:
                //errore
                fclose(fil);
                fclose(tmp);
                chdir("..");
                return -5;
            }
            //Tutto OK, copio su temp
            fputs(stringa,tmp);
        }
    }
    //Se sono qui: raggiunta la fine del file
    //ritorno corretto
    fclose(fil);
    fclose(tmp);
    chdir("..");
    return 1;
}

int backuplog()
//Questa funzione si occupa dei backup del log file
//voglio tenere i log degli ultimi due mesi
{
    time(&curtime);
    data_ora = localtime( &curtime );
    sprintf(stringa,"%02d / %02d / %04d",data_ora->tm_mday,data_ora->tm_mon+1,data_ora->tm_year+1900);

    //Determino il nome del log di questo mese
    sprintf(fnam1,"%04d_%02d.log",data_ora->tm_year+1900,data_ora->tm_mon+1);
    //Determino il nome del log del mese precedente
    //(potrebbe esserci uno dei due o entrambi)
    if(data_ora->tm_mon==0)
        //Se il mese corrente e' gennaio allora il file di log del mese precedente sara' dicembre dell'anno precedente
        sprintf(fnam2,"%04d_12.log",data_ora->tm_year+1899);
    else
        //altrimenti il file di log del mese precedente sara' il mese precedente dell'anno corrente
        sprintf(fnam2,"%04d_%02d.log",data_ora->tm_year+1900,data_ora->tm_mon);
    if((logg=fopen(fnam1,"r")))
    {
        //Se c'e' il file mese corrente
        chdir("budback");
        if(!(tmp=fopen(fnam1,"w")))
            return -1;
        while(!feof(logg))
        {
            if(fgets(stringa1,400,logg))
                fputs(stringa1,tmp);
        }
        fclose(logg);
        fclose(tmp);
        chdir("..");
    }
    if((logg=fopen(fnam2,"r")))
    {
        //Se c'e' il file mese precedente
        chdir("budback");
        if(!(tmp=fopen(fnam2,"w")))
            return -1;
        while(!feof(logg))
        {
            if(fgets(stringa1,400,logg))
                fputs(stringa1,tmp);
        }
        fclose(logg);
        fclose(tmp);
        chdir("..");
        remove(fnam2);
        chdir("budback");
    }
    //Ora determino il file log vecchio di due mesi da cancellare nel backup
    if(data_ora->tm_mon==0)
        //Se sono in gennaio devo cancellare il mese di novembre dell'anno precedente
        sprintf(fnam1,"%04d_11.log",data_ora->tm_year+1899);
    else if(data_ora->tm_mon==1)
        //Se sono in febbraio devo cancellare il mese di dicembre dell'anno precedente
        sprintf(fnam1,"%04d_12.log",data_ora->tm_year+1899);
    else
        //Altrimenti devo cancellare due mesi prima dell'anno corrente
        sprintf(fnam1,"%04d_%02d.log",data_ora->tm_year+1900,data_ora->tm_mon-1);
    //Rimuovo (se esiste) il file di due mesi prima appena determinato
    if((logg=fopen(fnam1,"r")))
    {
        //Se esiste lo chiudo e lo cancello
        fclose(logg);
        remove(fnam1);
    }
    chdir("..");
    return 0;
}


int compareFiles(char *file1, char *file2)
//Funzione che confronta se due files di testo
//sono identici. Se lo sono ritorna 0, se non
//lo sono ritorna >0, in caso di errore
//di apertura file ritorna -1 o -2
{
    if(!(f1=fopen(file1,"r")))
    {
        return -1;
    }
    if(!(f2=fopen(file2,"r")))
    {
        fclose(f1);
        return -2;
    }
    for(;;)
    {
        if(feof(f1))
        {
            //Ho raggiunto la fine di f1
            if(!feof(f2))
            {
                //Quindi se non ho raggiunto la fine di f2
                //allora i due files sono diversi
                //(cioe' f2 e' uguale a f1 ma ha qualcosa in piu')
                //Ritorno 1
                fclose(f1);
                fclose(f2);
                return 1;
            }
            //Se sono qui i files sono identici
            //Ritorno 0
            fclose(f1);
            fclose(f2);
            return 0;
        }
        //Se sono qui f1 non e' finito
        if(feof(f2))
        {
            //Quindi se e' finito f2 allora i due files
            //sono diversi. Chiudo e ritorno 1
            fclose(f1);
            fclose(f2);
            return 2;
        }
        //Se sono qui nessuno dei due e' finito
        //Prendo una riga da f1 e una da f2
        fgets(stringa,400,f1);
        fgets(stringa1,400,f2);
        //Se sono qui sono riuscito a leggere una
        //riga per file e le posso confrontare
        if(strcmp(stringa,stringa1)!=0)
        {
            //Se trovo una differenza
            //chiudo i files e ritorno 1
            fclose(f1);
            fclose(f2);
            return 3;
        }
    }
}

int isbises(int anno)               // controlla se un anno e' bisestile
{
    if(anno%4)                      // Se non e' divisibile per 4
        return 0;                   // non e' bisestile.
    else if(anno%100)               // Se lo e' e non e' la fine del secolo
        return 1;                   // e' bisestile.
    else if(anno%400)               // Se e' fine secolo ma non e' divisibile
        return 0;                   // per 400 non e' bisestile.
    else                            // Se e' fine secolo divisibile per 400
        return 1;                   // e' bisestile.
}

int scrivilog(char pre, char *linea)
//Funzione che crea un log file contenente,
//per ogni comando che viene eseguito,
//la data e l'ora, che cosa ho fatto e l'esito
//Mi serve per motivi di debugging
//Il file e' anno_mese.log e viene allungato ad ogni uso
//del programma. Ad ogni nuovo mese passo ad un file nuovo
//cosi' ho un log delle attivita' mensili
{
    char stringa[401];
    time_t curtime;
    struct tm *data_ora;
    FILE *log;

    time(&curtime);
    data_ora = localtime( &curtime );
    sprintf(stringa,"%04d_%02d.log",data_ora->tm_year+1900,data_ora->tm_mon+1);
    if(!(log=fopen(stringa,"a")))
        return -1;
    if(pre)
        //Solo se pre>0 inserisco data e ora
        fprintf(log,"%02d/%02d/%04d, %02d:%02d:%02d :\n",
                data_ora->tm_mday,data_ora->tm_mon+1,data_ora->tm_year+1900,
                data_ora->tm_hour,data_ora->tm_min,data_ora->tm_sec);
    fprintf(log,"%s\n",linea);
    fclose(log);
    return 0;
}

void mostrasaldi(HWND hwndDlg,int ID_SELB,int ID_SELD,int ID_RESL,info_risorse *tabella_risorse)
//Mostra il dato numerico richiesto
//(saldo, entrate, ecc.)
//per la risorsa selezionata
{
    int index;

    HWND SELBANC =  GetDlgItem(hwndDlg, ID_SELB);
    HWND SELDAT =  GetDlgItem(hwndDlg, ID_SELD);
    HWND RESULT =  GetDlgItem(hwndDlg, ID_RESL);

    if((index=ComboBox_GetCurSel(SELBANC))<0)
    {
        //Se la stringa non corrisponde a una risorsa valida
        //metto la prima risorsa di default
        index=0;
        ComboBox_SetCurSel(SELBANC,0);
    }
    if((k=ComboBox_GetCurSel(SELDAT))<0)
    {
        //Se la stringa non corrisponde a un dato valido
        //metto il saldo a oggi di default
        k=2;
        ComboBox_SetCurSel(SELDAT,2);
    }
    switch(k)
    {
        case 0:
            sprintf(stringa,"%.2lf",tabella_risorse[index].saldoinanno);
            break;
        case 1:
            sprintf(stringa,"%.2lf",tabella_risorse[index].saldoinimese);
            break;
        case 2:
            sprintf(stringa,"%.2lf",tabella_risorse[index].saldooggi);
            break;
        case 3:
            sprintf(stringa,"%.2lf",tabella_risorse[index].saldofinmese);
            break;
        case 4:
            sprintf(stringa,"%.2lf",tabella_risorse[index].saldofinanno);
            break;
        case 5:
            sprintf(stringa,"%.2lf",tabella_risorse[index].entrateinianno);
            break;
        case 6:
            sprintf(stringa,"%.2lf",tabella_risorse[index].entrateinimese);
            break;
        case 7:
            sprintf(stringa,"%.2lf",tabella_risorse[index].entrateoggi);
            break;
        case 8:
            sprintf(stringa,"%.2lf",tabella_risorse[index].entratefinmese);
            break;
        case 9:
            sprintf(stringa,"%.2lf",tabella_risorse[index].entratefinanno);
            break;
        case 10:
            sprintf(stringa,"%.2lf",tabella_risorse[index].usciteinianno);
            break;
        case 11:
            sprintf(stringa,"%.2lf",tabella_risorse[index].usciteinimese);
            break;
        case 12:
            sprintf(stringa,"%.2lf",tabella_risorse[index].usciteoggi);
            break;
        case 13:
            sprintf(stringa,"%.2lf",tabella_risorse[index].uscitefinmese);
            break;
        case 14:
            sprintf(stringa,"%.2lf",tabella_risorse[index].uscitefinanno);
            break;
        case 15:
            sprintf(stringa,"%.2lf",tabella_risorse[index].gaininianno);
            break;
        case 16:
            sprintf(stringa,"%.2lf",tabella_risorse[index].gaininimese);
            break;
        case 17:
            sprintf(stringa,"%.2lf",tabella_risorse[index].gainoggi);
            break;
        case 18:
            sprintf(stringa,"%.2lf",tabella_risorse[index].gainfinmese);
            break;
        case 19:
            sprintf(stringa,"%.2lf",tabella_risorse[index].gainfinanno);
            break;
        default:
            ;
    }
    ComboBox_SetText(RESULT,stringa);
}

int aggiornasaldi(long int num_g,int numfonti,char *nome_file,info_risorse *tabella_risorse)
//Scorre il file dei dati per intero
//calcola i nuovi saldi
//aggiorna il file basedata.dat
{
    int i,j,k,m,a,pp,plink,anno,deltananni,err,num_ops,nrighe,nanni,pa,pb;
    long int pippo,numg_inimese,numg_finmese,numg_fa;
    double deltasaldo,importo_f,saldo;

    FILE *mapp;

    pippo=num_g/100;
    anno=pippo/100;
    a=anno;
    m=pippo%100;
    //Questo mi serve per capire l'inizio del mese
    numg_inimese=(long int)a*10000+m*100+1;
    //Questo mi serve per capire l'inizio del mese successivo
    if(m==12)
    {
        m=1;
        a=anno+1;
    }
    else
        m++;
    numg_finmese=(long int)a*10000+m*100+1;

    //inizializzo i valori dei dati globali da calcolare
    //nell'elemento numfonti+1_esimo del vettore
    //tabella_risorse
    tabella_risorse[numfonti].saldoinanno=0;
    tabella_risorse[numfonti].saldoinimese=0;
    tabella_risorse[numfonti].saldooggi=0;
    tabella_risorse[numfonti].saldofinmese=0;
    tabella_risorse[numfonti].saldofinanno=0;

    tabella_risorse[numfonti].entrateinianno=0;
    tabella_risorse[numfonti].entrateinimese=0;
    tabella_risorse[numfonti].entrateoggi=0;
    tabella_risorse[numfonti].entratefinmese=0;
    tabella_risorse[numfonti].entratefinanno=0;

    tabella_risorse[numfonti].usciteinianno=0;
    tabella_risorse[numfonti].usciteinimese=0;
    tabella_risorse[numfonti].usciteoggi=0;
    tabella_risorse[numfonti].uscitefinmese=0;
    tabella_risorse[numfonti].uscitefinanno=0;

    tabella_risorse[numfonti].gaininianno=0;
    tabella_risorse[numfonti].gaininimese=0;
    tabella_risorse[numfonti].gainoggi=0;
    tabella_risorse[numfonti].gainfinmese=0;
    tabella_risorse[numfonti].gainfinanno=0;

    for(i=0;i<numfonti;i++)
    {
        //inizializzo i valori dei dati da calcolare
        tabella_risorse[i].saldoinimese=tabella_risorse[i].saldoinanno;
        tabella_risorse[i].saldooggi=tabella_risorse[i].saldoinanno;
        tabella_risorse[i].saldofinmese=tabella_risorse[i].saldoinanno;
        tabella_risorse[i].saldofinanno=tabella_risorse[i].saldoinanno;

        tabella_risorse[numfonti].saldoinanno=tabella_risorse[numfonti].saldoinanno+tabella_risorse[i].saldoinanno;
        tabella_risorse[numfonti].saldoinimese=tabella_risorse[numfonti].saldoinimese+tabella_risorse[i].saldoinanno;
        tabella_risorse[numfonti].saldooggi=tabella_risorse[numfonti].saldooggi+tabella_risorse[i].saldoinanno;
        tabella_risorse[numfonti].saldofinmese=tabella_risorse[numfonti].saldofinmese+tabella_risorse[i].saldoinanno;
        tabella_risorse[numfonti].saldofinanno=tabella_risorse[numfonti].saldofinanno+tabella_risorse[i].saldoinanno;


        tabella_risorse[i].entrateinianno=0;
        tabella_risorse[i].entrateinimese=0;
        tabella_risorse[i].entrateoggi=0;
        tabella_risorse[i].entratefinmese=0;
        tabella_risorse[i].entratefinanno=0;

        tabella_risorse[i].usciteinianno=0;
        tabella_risorse[i].usciteinimese=0;
        tabella_risorse[i].usciteoggi=0;
        tabella_risorse[i].uscitefinmese=0;
        tabella_risorse[i].uscitefinanno=0;

        tabella_risorse[i].gaininianno=0;
        tabella_risorse[i].gaininimese=0;
        tabella_risorse[i].gainoggi=0;
        tabella_risorse[i].gainfinmese=0;
        tabella_risorse[i].gainfinanno=0;
    }
    if(!(fil=fopen(nome_file,"r")))
        //Se il file e' nuovo non c'e' da aggiornare niente
        //restituisco 1
        return 1;
    numg_fa=0;
    num_ops=0;
    nrighe=0;
    pa=0;
    pb=0;
    err=0;
    for(;;)
    {
        //Cerco l'importo relativo alla prima operazione disponibile
        importo_f=search_util(fil,(char*)"",1,1,&numg_fa,&num_ops,&nrighe,&pa,&pb,&err);
        if(err<0)
        {
            //Si e' verificato un errore: esco
            fclose(fil);
            return -1;
        }
        if(!importo_f)
        {
            //Visto che non avevo criteri di ricerca, se ottengo un
            //importo nullo vuol dire che il file e' finito.
            //Lo chiudo e esco dal file
            fclose(fil);
            break;
        }
        if(importo_f>0)
        {
            //Se era un'entrata devo fare riferimento a pb
            pp=pb;
            plink=pa;
        }
        else
        {
            //Se era un'uscita devo fare riferimento a pa
            pp=pa;
            plink=pb;
        }
        j=0;
        do
        {
            //Cerco il nome della risorsa associata con pp
            //scorrendo tabella_risorse[].posiz
            if(tabella_risorse[j].posiz==pp)
            {
                //Trovato. Aggiorno i dati della risorsa
                //Da aggiornare sempre:
                //Saldo, guadagno, entrate, uscite
                //da inizio anno a fine anno
                tabella_risorse[j].saldofinanno=tabella_risorse[j].saldofinanno+importo_f;
                tabella_risorse[j].gainfinanno=tabella_risorse[j].gainfinanno+importo_f;
                //Anche se non controllo niente, solo se e' una "pura" entrata
                //o uscita (niente link) avro' un effetto sul dato globale:
                //infatti se c'e' un link quello che e' entrato poi
                //esce da qualche altra parte (a meno che non siano anni
                //diversi)
                tabella_risorse[numfonti].saldofinanno=tabella_risorse[numfonti].saldofinanno+importo_f;
                tabella_risorse[numfonti].gainfinanno=
                tabella_risorse[numfonti].gainfinanno+
                importo_f;
                if(importo_f>=0)
                {
                    tabella_risorse[j].entratefinanno=tabella_risorse[j].entratefinanno+importo_f;
                    if(!plink)
                        //Solo se e' una "pura" entrata (niente link) la conteggio
                        //nelle entrate globali
                    tabella_risorse[numfonti].entratefinanno=tabella_risorse[numfonti].entratefinanno+importo_f;
                }
                else
                {
                    tabella_risorse[j].uscitefinanno=tabella_risorse[j].uscitefinanno-importo_f;
                    if(!plink)
                        //Solo se e' una "pura" uscita (niente link) la conteggio
                        //nelle uscite globali
                        tabella_risorse[numfonti].uscitefinanno=tabella_risorse[numfonti].uscitefinanno-importo_f;
                }
                if(numg_fa<numg_inimese)
                {
                    //Caso specifico per: saldo inizio mese
                    tabella_risorse[j].saldoinimese=tabella_risorse[j].saldoinimese+importo_f;
                    //Dato globale
                    tabella_risorse[numfonti].saldoinimese=tabella_risorse[numfonti].saldoinimese+importo_f;
                }
                if(numg_fa<numg_finmese)
                {
                    //Caso specifico per: saldo fine mese
                    tabella_risorse[j].saldofinmese=tabella_risorse[j].saldofinmese+importo_f;
                //Dato globale
                tabella_risorse[numfonti].saldofinmese=tabella_risorse[numfonti].saldofinmese+importo_f;
                }
                if(numg_fa<=num_g)
                {
                    //Caso specifico per: saldo odierno,
                    //guadagno, entrate, uscite da inizio anno a oggi
                    tabella_risorse[j].saldooggi=tabella_risorse[j].saldooggi+importo_f;
                    tabella_risorse[j].gaininianno=tabella_risorse[j].gaininianno+importo_f;
                    //Dato globale
                    tabella_risorse[numfonti].saldooggi=
                    tabella_risorse[numfonti].saldooggi+importo_f;
                    tabella_risorse[numfonti].gaininianno=tabella_risorse[numfonti].gaininianno+importo_f;
                    if(importo_f>=0)
                    {
                        tabella_risorse[j].entrateinianno=
                        tabella_risorse[j].entrateinianno+
                        importo_f;
                        if(!plink)
                            //Solo se e' una "pura" entrata (niente link) la conteggio
                            //nelle entrate globali
                            tabella_risorse[numfonti].entrateinianno=tabella_risorse[numfonti].entrateinianno+importo_f;
                    }
                    else
                    {
                        tabella_risorse[j].usciteinianno=
                        tabella_risorse[j].usciteinianno-
                        importo_f;
                        if(!plink)
                            //Solo se e' una "pura" uscita (niente link) la conteggio
                            //nelle uscite globali
                            tabella_risorse[numfonti].usciteinianno=tabella_risorse[numfonti].usciteinianno-importo_f;
                    }
                }
                if(numg_fa==num_g)
                {
                    //Caso specifico per: guadagno, entrate, uscite di oggi
                    tabella_risorse[j].gainoggi=tabella_risorse[j].gainoggi+importo_f;
                    //Dato globale
                    tabella_risorse[numfonti].gainoggi=tabella_risorse[numfonti].gainoggi+importo_f;
                    if(importo_f>=0)
                    {
                        tabella_risorse[j].entrateoggi=tabella_risorse[j].entrateoggi+importo_f;
                        if(!plink)
                            //Solo se e' una "pura" entrata (niente link) la conteggio
                            //nelle entrate globali
                            tabella_risorse[numfonti].entrateoggi=tabella_risorse[numfonti].entrateoggi+importo_f;
                    }
                    else
                    {
                        tabella_risorse[j].usciteoggi=tabella_risorse[j].usciteoggi-importo_f;
                        if(!plink)
                            //Solo se e' una "pura" uscita (niente link) la conteggio
                            //nelle uscite globali
                            tabella_risorse[numfonti].usciteoggi=tabella_risorse[numfonti].usciteoggi-importo_f;
                    }
                }
                if((numg_fa>=numg_inimese)&&(numg_fa<=num_g))
                {
                    //Caso specifico per:
                    //guadagno, entrate, uscite da inizio mese a oggi
                    tabella_risorse[j].gaininimese=tabella_risorse[j].gaininimese+importo_f;
                    //Dato globale
                    tabella_risorse[numfonti].gaininimese=tabella_risorse[numfonti].gaininimese+importo_f;
                    if(importo_f>=0)
                    {
                        tabella_risorse[j].entrateinimese=
                        tabella_risorse[j].entrateinimese+
                        importo_f;
                        if(!plink)
                            //Solo se e' una "pura" entrata (niente link) la conteggio
                            //nelle entrate globali
                            tabella_risorse[numfonti].entrateinimese=tabella_risorse[numfonti].entrateinimese+importo_f;
                    }
                    else
                    {
                        tabella_risorse[j].usciteinimese=tabella_risorse[j].usciteinimese-importo_f;
                        if(!plink)
                            //Solo se e' una "pura" uscita (niente link) la conteggio
                            //nelle uscite globali
                            tabella_risorse[numfonti].usciteinimese=tabella_risorse[numfonti].usciteinimese-importo_f;
                    }
                }
                if((numg_fa>=numg_inimese)&&(numg_fa<numg_finmese))
                {
                    //Caso specifico per:
                    //entrate, uscite, guadagno da inizio mese a fine mese
                    tabella_risorse[j].gainfinmese=tabella_risorse[j].gainfinmese+importo_f;
                    //Dato globale
                    tabella_risorse[numfonti].gainfinmese=tabella_risorse[numfonti].gainfinmese+importo_f;
                    if(importo_f>=0)
                    {
                        tabella_risorse[j].entratefinmese=
                        tabella_risorse[j].entratefinmese+
                        importo_f;
                        if(!plink)
                            //Solo se e' una "pura" entrata (niente link) la conteggio
                            //nelle entrate globali
                            tabella_risorse[numfonti].entratefinmese=tabella_risorse[numfonti].entratefinmese+importo_f;
                    }
                    else
                    {
                        tabella_risorse[j].uscitefinmese=tabella_risorse[j].uscitefinmese-importo_f;
                        if(!plink)
                            //Solo se e' una "pura" uscita (niente link) la conteggio
                            //nelle uscite globali
                            tabella_risorse[numfonti].uscitefinmese=tabella_risorse[numfonti].uscitefinmese-importo_f;
                    }
                }
                break;
            }
            j++;
        }
        while(j<numfonti);
    }
    //Se sono qui: raggiunta la fine del file
    //posso passare ad aggiornare basedata.dat
    if((k=controllabasedata())<0)
        //In caso di errore
        return k;
    mapp=fopen("basedata.dat","r");
    tmp=fopen("temp","w");
    fgets(stringa,400,mapp);
    //Posso copiare in tmp
    fputs(stringa,tmp);
    for(i=0;i<numfonti;i++)
    {
        fgets(stringa,400,mapp);
        //Posso copiare in tmp
        fputs(stringa,tmp);
        //Leggo il giorno di apertura (se c'e')
        fgets(stringa,400,mapp);
        //Cerco le informazioni sul giorno di apertura
        fputs(stringa,tmp);
        //Leggo il giorno di chiusura (se c'e')
        fgets(stringa,400,mapp);
        //Cerco le informazioni sul giorno di chiusura
        fputs(stringa,tmp);
        //Leggo quante righe di commento e quanti anni ci sono
        fgets(stringa,400,mapp);
        sscanf(stringa,"%d,%d\n",&nrighe,&nanni);
        //Tutto OK, posso copiare in tmp con il numero
        //corretto di anni
        switch(tabella_risorse[i].casi)
        {
            case 0:
                //c'e' l'anno in corso e l'anno dopo
                //nanni rimane identico
                deltananni=0;
                break;
            case 1:
                //c'e' l'anno in corso ma non l'anno dopo
                //dovro' aggiungere un anno a nanni
                deltananni=1;
                break;
            case 2:
                //non c'e' l'anno in corso ma c'e' l'anno dopo
                //dovro' aggiungere un anno a nanni
                deltananni=1;
                break;
            case 3:
                //non c'e' l'anno in corso e non c'e' l'anno dopo
                //dovro' aggiungere 2 anni a nanni
                deltananni=2;
                break;
            default:
                //Valore sbagliato (non dovrebbe mai succedere)
                fclose(mapp);
                fclose(tmp);
                return -13;
        }
        //Scrivo la nuova informazione su tmp
        //Il valore vecchio di nanni mi servira'
        //comunque nel seguito
        fprintf(tmp,"%d,%d\n",nrighe,nanni+deltananni);
        //Copio le righe di commento
        for(j=0;j<nrighe;j++)
        {
            fgets(stringa,400,mapp);
            //Posso copiare in tmp
            fputs(stringa,tmp);
        }
        //Adesso leggo tutti gli anni fino a quello di
        //interesse. Al piu' mi fermero' al primo
        //anno esistente dopo quello di interesse.
        //Solo gli anni minori di quello di interesse
        //verranno copiati inalterati in tmp
        j=0;
        while(j<nanni)
        {
            j++;
            fgets(stringa,400,mapp);
            sscanf(stringa,"%d,%lf\n",&a,&saldo);
            if(a<anno)
                fputs(stringa,tmp);
            else
                break;
            //Notare che all'uscita da questo loop:
            //se j<nanni  => a>=anno
        }
        //anno e anno+1 vanno sempre riscritti ex-novo
        fprintf(tmp,"%d,%.2lf\n",anno,tabella_risorse[i].saldoinanno);
        fprintf(tmp,"%d,%.2lf\n",anno+1,tabella_risorse[i].saldofinanno);
        //deltasaldo serve per aggiornare il saldo
        //degli anni seguenti a anno+1
        //se ce ne sono
        deltasaldo=
        tabella_risorse[i].saldofinanno-
        tabella_risorse[i].saldofinannoold;
        //Adesso devo decidere il valore giusto di j
        //in maniera da finire a ricopiare tutti i saldi
        //rimasti
        if(j<nanni)
        {
            //Solo se ci sono anni successivi
            //mi pongo il problema di aggiornare j
            if(a==anno)
            {
                //Accade se non ho letto tutti
                //gli anni e l'anno in questione
                //era nella lista
                //Vediamo se c'e' da incrementare j:
                switch(tabella_risorse[i].casi)
                {
                    case 0:
                        //c'e' l'anno in corso e l'anno dopo:
                        //leggo la riga per superarla
                        //e incremento j
                        fgets(stringa,400,mapp);
                        sscanf(stringa,"%d,%lf\n",&a,&saldo);
                        if(a!=anno+1)
                        {
                            //Ci doveva essere anno+1 ma non c'e': errore
                            fclose(mapp);
                            fclose(tmp);
                            return -19;
                        }
                        j++;
                        break;
                    case 1:
                        //c'e' l'anno in corso ma non l'anno dopo
                        //non incremento j
                        break;
                    default:
                        //Tutti gli altri valori sono sbagliati
                        //in questo contesto (non dovrebbe mai succedere)
                        fclose(mapp);
                        fclose(tmp);
                        return -20;
                }
            }
        }
        if(a>anno+1)
        {
            //Se accade => ho letto tutti
            //gli anni oppure no, l'anno in questione
            //non era nella lista e non c'e' anno+1
            //in questo caso copio su temp quello
            //che avevo gia' letto
            fputs(stringa,tmp);
        }
        //Ora riprendo a copiare fino alla fine
        while(j<nanni)
        {
            j++;
            fgets(stringa,400,mapp);
            sscanf(stringa,"%d,%lf\n",&a,&saldo);
            //Posso aggiornare tmp
            fprintf(tmp,"%d,%.2lf\n",a,
            saldo+deltasaldo);
        }
    }
    //Ho finito la copia: chiudo e sostituisco
    //temp a basedata.dat
    fclose(mapp);
    fclose(tmp);
    remove("basedata.dat");
    rename("temp","basedata.dat");
    return 1;
}

int caricasaldi(long int num_g,info_risorse *tabella_risorse)
//Scorre il file basedata.dat per avere
//i saldi di partenza e le informazioni
//fondamentali ad essi correlate
{
    char stringa[401],nomeconv[27];

    int i,j,a,k,nrighe,nanni,anno,posiz;

    long int pippo;

    double saldo;

    FILE *mapp;

    if((k=controllabasedata())<0)
        //In caso di errore
        return k;
    //Se sono qui ho controllato l'integrita' del file. Adesso posso procedere
    //senza fare controlli
    pippo=num_g/100;
    anno=pippo/100;
    mapp=fopen("basedata.dat","r");
    fgets(stringa,400,mapp);
    sscanf(stringa,"%d\n",&numfonti);
    //Tutto OK, posso andare avanti
    for(i=0;i<numfonti;i++)
    {
        fgets(stringa,400,mapp);
        sscanf(stringa,"%d,%[!-~ ‘’@ςΰωθιμ°η§]\n",&posiz,nomeconv);
        //Leggo il giorno di apertura (se c'e')
        fgets(stringa,400,mapp);
        //Leggo il giorno di chiusura (se c'e')
        fgets(stringa,400,mapp);
        //Leggo quante righe di commento e quanti anni ci sono
        fgets(stringa,400,mapp);
        sscanf(stringa,"%d,%d\n",&nrighe,&nanni);
        //Supero le righe di commento
        for(j=0;j<nrighe;j++)
            fgets(stringa,400,mapp);
        j=0;
        //Valore di default del saldo=0 se non trovo
        //niente di utile in basedata.dat
        tabella_risorse[i].saldoinanno=0;
        tabella_risorse[i].saldofinannoold=0;
        tabella_risorse[i].casi=3;
        while(j<nanni)
        {
            j++;
            fgets(stringa,400,mapp);
            sscanf(stringa,"%d,%lf\n",&a,&saldo);
            if(a<anno)
            {
                //Fintanto che non trovo anno:
                //Valore di ....casi = 3 (per ora)
                tabella_risorse[i].casi=3;
                //Valore del saldo fintanto che non trovo anno=
                //saldo dell'anno letto (per ora)
                tabella_risorse[i].saldoinanno=saldo;
                //Valore del saldo anno successivo =
                //saldo dell'anno letto (per ora)
                tabella_risorse[i].saldofinannoold=saldo;
            }
            else if(a==anno)
            {
                //Accade se anno e' parte della lista
                //Valore di ....casi = 1 (per ora)
                tabella_risorse[i].casi=1;
                tabella_risorse[i].saldoinanno=saldo;
                //Valore del saldo anno successivo =
                //saldo di quest'anno (per ora)
                tabella_risorse[i].saldofinannoold=saldo;
            }
            else if(a==anno+1)
            {
                //Accade se anno+1 e' parte della lista
                tabella_risorse[i].saldofinannoold=saldo;
                if(tabella_risorse[i].casi==1)
                    //Se esisteva anno => nuovo valore = 0
                    tabella_risorse[i].casi=0;
                else
                    //Se non esisteva anno => nuovo valore = 2
                    tabella_risorse[i].casi=2;
            }
        }
    }
    fclose(mapp);
    return 1;
}

int controllacampi(HWND hwndDlg,int GG,int MM,int AAAA)
//Differenza rispetto a caricacampi:
//Questa fa solo il controllo sul
//formato della data. Se va bene
//carica le liste corrispondenti
//altrimenti esce con errore
{
    HWND GGior =  GetDlgItem(hwndDlg, GG);
    HWND MMes =  GetDlgItem(hwndDlg, MM);
    HWND AAnn =  GetDlgItem(hwndDlg, AAAA);

    char stringa[20];
    int i,a,m,g;

    //Controllo che l'anno sia valido
    ComboBox_GetText(AAnn,stringa,5);
    if((a=atoi(stringa))<=0)
        return -1;
    //Controllo se l'anno e' bisestile
    if(isbises(a))
        month[1]=29;
    else
        month[1]=28;
    //Controllo che il mese sia valido
    ComboBox_GetText(MMes,stringa,3);
    if(((m=atoi(stringa))<=0)||(m>12))
        return -1;
    //Controllo che il giorno sia valido
    ComboBox_GetText(GGior,stringa,3);
    if(((g=atoi(stringa))<=0)||(g>month[m-1]))
        return -1;
    //Se tutto andava bene:
    //Aggiorno le liste dei tre campi
    ComboBox_ResetContent(GGior);
    ComboBox_ResetContent(MMes);
    ComboBox_ResetContent(AAnn);
    sprintf(stringa,"%02d",g);
    ComboBox_SetText(GGior,stringa);
    sprintf(stringa,"%02d",m);
    ComboBox_SetText(MMes,stringa);
    sprintf(stringa,"%04d",a);
    ComboBox_SetText(AAnn,stringa);
    for(i=1;i<=12;i++)
    {
        sprintf(stringa,"%02d",i);
        ComboBox_AddString(MMes,stringa);
    }
    for(i=1;i<=month[m-1];i++)
    {
        sprintf(stringa,"%02d",i);
        ComboBox_AddString(GGior,stringa);
    }
    //Inserisco se possibile anno-2
    if(a>2)
    {
        sprintf(stringa,"%04d",a-2);
        ComboBox_AddString(AAnn,stringa);
    }
    //Inserisco se possibile anno-1
    if(a>1)
    {
        sprintf(stringa,"%04d",a-1);
        ComboBox_AddString(AAnn,stringa);
    }
    //Inserisco anno
    sprintf(stringa,"%04d",a);
    ComboBox_AddString(AAnn,stringa);
    //Inserisco anno+1
    sprintf(stringa,"%04d",a+1);
    ComboBox_AddString(AAnn,stringa);
    //Inserisco anno+2
    sprintf(stringa,"%04d",a+2);
    ComboBox_AddString(AAnn,stringa);
    return 0;
}

void caricacampi(HWND hwndDlg,int gio,int mes,int ann)
//Controlla la validita' della data
//e nel caso di errore mette la data
//di oggi. Alla fine mette nelle liste
//i valori ammissibili: numero di giorni
//mesi da 1 a 12, e anno in corso +/-2
{

    struct tm *tmpdate;
    char stringa[20];
    int i,a,m,g;

    HWND GGior =  GetDlgItem(hwndDlg, gio);
    HWND MMes =  GetDlgItem(hwndDlg, mes);
    HWND AAnn =  GetDlgItem(hwndDlg, ann);

    //Prendo la data di oggi
    //nel caso mi servisse
    time(&curtime);
    tmpdate = localtime( &curtime );
    //Leggo cosa ho scritto nel campo anno
    //e verifico che sia valido
    ComboBox_GetText(AAnn,stringa,5);
    if((a=atoi(stringa))<=0)
    {
        //Non e' valido sostituisco con l'anno di oggi
        a=tmpdate->tm_year+1900;
    }
    //Vedo se l'anno e' bisestile per determinare
    //i giorni di febbraio
    if(isbises(a))
        month[1]=29;
    else
        month[1]=28;
    //Leggo cosa ho scritto nel campo mese
    //e verifico che sia valido
    ComboBox_GetText(MMes,stringa,3);
    if(((m=atoi(stringa))<=0)||(m>12))
    {
        //Non e' valido sostituisco col mese di oggi
        m=tmpdate->tm_mon+1;
    }
    //Leggo cosa ho scritto nel campo giorno
    //e verifico che sia valido
    ComboBox_GetText(GGior,stringa,3);
    if(((g=atoi(stringa))<=0)||(g>month[m-1]))
    {
        //Non e' valido
        if(tmpdate->tm_mday<=month[m-1])
        {
            //Se il giorno di oggi e' ammesso per il mese
            //in questione metto il giorno di oggi
            g=tmpdate->tm_mday;
        }
        else
        {
            //Se non e' un numero ammesso metto
            //l'ultimo del mese in questione
            g=month[m-1];
        }
    }
    //Adesso pulisco le liste, inserisco
    //la data e creo liste aggiornate
    //per giorno mese e anno
    ComboBox_ResetContent(GGior);
    ComboBox_ResetContent(MMes);
    ComboBox_ResetContent(AAnn);
    sprintf(stringa,"%02d",g);
    ComboBox_SetText(GGior,stringa);
    sprintf(stringa,"%02d",m);
    ComboBox_SetText(MMes,stringa);
    sprintf(stringa,"%04d",a);
    ComboBox_SetText(AAnn,stringa);
    for(i=1;i<=12;i++)
    {
        sprintf(stringa,"%02d",i);
        ComboBox_AddString(MMes,stringa);
    }
    for(i=1;i<=month[m-1];i++)
    {
        sprintf(stringa,"%02d",i);
        ComboBox_AddString(GGior,stringa);
    }
    //Inserisco se possibile anno-2
    if(a>2)
    {
        sprintf(stringa,"%04d",a-2);
        ComboBox_AddString(AAnn,stringa);
    }
    //Inserisco se possibile anno-1
    if(a>1)
    {
        sprintf(stringa,"%04d",a-1);
        ComboBox_AddString(AAnn,stringa);
    }
    //Inserisco anno
    sprintf(stringa,"%04d",a);
    ComboBox_AddString(AAnn,stringa);
    //Inserisco anno+1
    sprintf(stringa,"%04d",a+1);
    ComboBox_AddString(AAnn,stringa);
    //Inserisco anno+2
    sprintf(stringa,"%04d",a+2);
    ComboBox_AddString(AAnn,stringa);
    sprintf(stringa,"%02d",g);
    ComboBox_SelectString(GGior,-1,stringa);
    sprintf(stringa,"%02d",m);
    ComboBox_SelectString(MMes,-1,stringa);
    sprintf(stringa,"%04d",a);
    ComboBox_SelectString(AAnn,-1,stringa);
}


int leggiprepara(long int num_g,char *nome_file)
//Trova il prossimo numero di operazione disponibile
//per il giorno selezionato: se non c'era niente il
//numero e' 1, se c'era qualcosa e' 'operazione
//con numero piu' alto+1'. Se errore: -1
{
    char stringa[401];

    int i,j,k,num_ops,nrighe;

    long int numg_fa;

    FILE *fil;

    if(!(fil=fopen(nome_file,"r")))
    {
        //Se il file e' nuovo lo creo e restituisco 1
        fil=fopen(nome_file,"w");
        fclose(fil);
        return 1;
    }
    fclose(fil);
    if((k=controllafiledati(nome_file))<0)
        //Si e' verificato un errore nel file dati
        return k;
    //Se sono qui sono sicuro che il file dati e' a posto
    //posso procedere senza controllare l'integrita'
    fil=fopen(nome_file,"r");
    do
    {
        if(!fgets(stringa,400,fil))
        {
            //Ho raggiunto la fine del file
            //Restituisco 1
            fclose(fil);
            return 1;
        }
        sscanf(stringa,"%ld,%d,%d\n",&numg_fa,&num_ops,&nrighe);
        if((numg_fa<num_g))
            //Sono a un giorno precedente a quello desiderato
            for(i=0;i<num_ops+nrighe;i++)
                fgets(stringa,400,fil);
    }
    while(numg_fa<num_g);
    if(numg_fa>num_g)
    {
        //Questo puo' accadere se:
        //Il giorno che voglio scrivere
        //non conteneva gia' dei dati ma almeno uno
        //dei giorni successivi li contiene
        //Restituisco 1
            fclose(fil);
            return 1;
    }
    //Se sono qui e' perche' numg_fa==num_g:
    //Il giorno che voglio scrivere conteneva gia' dei dati
    //Individuo il prossimo numero disponibile
    //e lo restituisco
    for(i=0;i<num_ops;i++)
    {
        fgets(stringa,400,fil);
        sscanf(stringa,"%d,%lf,%[!-~ ‘’@ςΰωθιμ°η§]\t,%[!-~ ‘’@ςΰωθιμ°η§]\t,%d,%d,%ld,%d\n",&j,&importo_f,mot_f,cat_f,&pa,&pb,&numg_fb,&k);
    }
    //Se sono arrivato qui il prossimo numero
    //disponibile e' j+1 e lo restituisco
    fclose(fil);
    return (j+1);
}

int scrivifile(char *nome_file,long int num_g,char *datastring)
//Struttura del file:
//giorno
//giorno
//.
//.
//dove giorno e':
//
//num_g,n_o,n_r   (num_g=aaaammgg)
//n1,+/-imp,mot	,cat ,viene_da,va_in,num_g1,m1
//.
//.
//nn_o,+/-imp,mot, cat,	viene_da,va_in,num_gn_o,mn_o
//riga note 1
//.
//.
//riga note n_r
//
//N.B. se non ho dati in un certo giorno
//quel giorno non compare nel file
//NOTA: non creo il file e non controllo l'integrita' del file
//perche' prima di questa funzione va sempre chiamata
//leggiprepara che a sua volta crea il file e chiama controllafiledati
{
    char stringa[401],stringa1[401];

    int i,fine,num_ops,nrighe;

    long int numg_fa;

    FILE *tmp,*fil;

    strcpy(stringa1,datastring);
    if(!(fil=fopen(nome_file,"r")))
        //Il file qui dovrebbe gia' esistere!!! Errore
        return -100;
    //Se sono qui sono sicuro che il file dati e' a posto
    //posso procedere senza controllare l'integrita'
    tmp=fopen("temp","w");
    numg_fa=0;
    fine=0;
    //Entro nella fase di aggiornamento dei dati
    for(;;)
    {
        if(!fgets(stringa,400,fil))
        {
            //Ho raggiunto la fine del file. Esco dal loop con fine=1
            fine=1;
            break;
        }
        sscanf(stringa,"%ld,%d,%d\n",&numg_fa,&num_ops,&nrighe);
        if(numg_fa>=num_g)
            //Ho raggiunto o superato il giorno desiderato
            //esco dal loop
            break;
        fprintf(tmp,"%s",stringa);
        for(i=0;i<num_ops+nrighe;i++)
        {
            fgets(stringa,400,fil);
            //Ricopio la riga
            fprintf(tmp,"%s",stringa);
        }
    }
    if(fine)
    {
        //Questo puo' accadere se:
        //Il giorno che voglio scrivere e quelli successivi
        //non contenevano gia' dei dati (ho raggiunto la fine)
        //Lo scrivo ex novo e poi il file finira'
        fprintf(tmp,"%ld,1,0\n",num_g);
        //Adesso posso scrivere la nuova stringa
        fprintf(tmp,"%s\n",stringa1);
    }
    else if(numg_fa==num_g)
    {
        //Questo puo' accadere se:
        //Il giorno che voglio scrivere conteneva gia' dei dati
        //Aggiungo i nuovi dati, e poi gli appendo (fuori da questo 'do')
        //i giorni successivi (se ci sono)
        fprintf(tmp,"%ld,%d,%d\n",numg_fa,num_ops+1,nrighe);
        for(i=0;i<num_ops;i++)
        {
            fgets(stringa,400,fil);
            //Ricopio la riga
            fprintf(tmp,"%s",stringa);
        }
        //Raggiunta la fine dei dati del giorno
        //Adesso posso scrivere la nuova stringa
        fprintf(tmp,"%s\n",stringa1);
        for(i=0;i<nrighe;i++)
        {
            fgets(stringa,400,fil);
            //Ricopio la riga
            fprintf(tmp,"%s",stringa);
        }
    }
    else if(numg_fa>num_g)
    {
        //Questo puo' accadere se:
        //Il giorno che voglio scrivere
        //non conteneva gia' dei dati ma almeno uno
        //dei giorni successivi li contiene
        //Lo scrivo ex novo e poi gli devo appendere
        //quello che segue
        fprintf(tmp,"%ld,1,0\n",num_g);
        //Adesso posso scrivere la nuova stringa
        fprintf(tmp,"%s\n",stringa1);
        //Ricopio l'intestazione del giorno che segue
        fprintf(tmp,"%s",stringa);
        for(i=0;i<num_ops+nrighe;i++)
        {
            fgets(stringa,400,fil);
            //Ricopio la riga
            fprintf(tmp,"%s",stringa);
        }
    }
    //A questo punto i nuovi dati sono dentro
    //Ricopio quello che segue (se c'e') e finisco
    if(!fine)
        while(!feof(fil))
        {
          fgets(stringa,400,fil);
            if(!feof(fil))
            fprintf(tmp,"%s",stringa);
        }
    fclose(fil);
    fclose(tmp);
    remove(nome_file);
    rename("temp",nome_file);
    return 0;
}

float search_util(FILE *pf,char *what,char where,char how,long int *numg,int *nop,int *nrig,int *pa,int *pb,int *err)
//Funzione che cerca nel file aperto (deve essere di tipo anno.bud)
//la stringa what, e la cerca in motivo (where=0)
//o in categoria (where=1). Se how=0, cerca la corrispondenza esatta
//se how=1 basta che la stringa sia contenuta.
//La prima volta che trova what, ritorna l'importo dell'entrata
//o uscita. Se non la trova o in caso di errore ritorna 0. In caso
//di errore il flag err viene settato.
//Quando trova what, numg contiene il giorno in cui l'ha trovato
//pa e pb contengono le risorse in cui l'ha trovato
{
    char mot_f[100],cat_f[100],stringa[401];
    int i,j,k;
    long int numg_fb;

    double importo_f;

    (*err)=0;
    for(;;)
    {
        for(i=0;i<(*nop);i++)
            //Leggo le righe rimanenti di entrate/uscite
            //(se inizio un nuovo giorno saltero' questa
            //parte perche' (*nop)==0
        {
            if(!fgets(stringa,400,pf))
            {
                //Mi aspettavo una riga e invece non trovo niente:
                //errore
                (*err)=-1;
                return 0;
            }
            if(sscanf(stringa,"%d,%lf,%[!-~ ‘’@ςΰωθιμ°η§]\t,%[!-~ ‘’@ςΰωθιμ°η§]\t,%d,%d,%ld,%d\n",
              &j,&importo_f,mot_f,cat_f,pa,pb,&numg_fb,&k)!=8)
            {
                //Trovo i dati in un formato non valido: errore
                (*err)=-2;
                return 0;
            }
            switch(where)
            {
                case 0:
                    //Cerca su motivo
                    if(//Se avevo messo how=1 e la stringa e' contenuta
                    (how&&strstr(mot_f,what))
                    ||
                    //o se avevo messo how=0 e la stringa e' identica
                    ((!how)&&(!strcmp(mot_f,what))))
                    {
                        //Aggiorno (*nop) tenendo conto che i non e' stato ancora
                        //incrementato
                        (*nop)=(*nop)-i-1;
                        return importo_f;
                    }
                    break;
                case 1:
                    //Cerca su categoria
                    if(//Se avevo messo how=1 e la stringa e' contenuta
                    (how&&strstr(cat_f,what))
                    ||
                    //o se avevo messo how=0 e la stringa e' identica
                    ((!how)&&(!strcmp(cat_f,what))))
                    {
                        //Aggiorno (*nop) tenendo conto che i non e' stato ancora
                        //incrementato
                        (*nop)=(*nop)-i-1;
                        return importo_f;
                    }
                    break;
                default:
                    ;
            }
        }
        for(i=0;i<(*nrig);i++)
        //Leggo le righe di entrate/uscite
        //Alla prima chiamata della routine (*nrig) deve essere 0.
        //Dopo sara' sempre aggiornata al valore del giorno successivo
        {
            if(!fgets(stringa,400,pf))
            {
                //Mi aspettavo una riga e invece non trovo niente:
                //errore
                (*err)=-3;
                return 0;
            }
        }
        if(!fgets(stringa,400,pf))
        {
            //Ho raggiunto la fine del file: non c'e' match.
            //Per sicurezza (forse eccessiva) annullo (*nop) e *nrighe
            (*nop)=0;
            (*nrig)=0;
            return 0;
        }
        if(sscanf(stringa,"%ld,%d,%d\n",numg,nop,nrig)!=3)
        {
            //Trovo i dati in un formato non valido: errore
            (*err)=-4;
            return 0;
        }
    }
}

int search_year(char *nomefile,char *cat,info_smaller *tabellina,int numfonti,long int numg_ini,long int numg_fin)
//Questa funzione utilizza search_util per calcolare
//entrate, uscite, guadagno netto tra una data iniziale (numg_ini)
//e una finale (numg_fin) relativamente a una categoria
//nel file specificato. I risultati vengono messi in tabella_risorse
//e il valore restituito mi segnala gli errori
{
    char stringa1[401];

    int j,num_ops,nrighe,pa,pb,pp,plink,err;

    long int numg_fa;

    double importo_f;

    FILE *fil;

    if(!(fil=fopen(nomefile,"r")))
    {
        //Se il file e' nuovo non c'e' da aggiornare niente
        //restituisco 1
        sprintf(stringa1,"search_year: file doesn't exist");
        scrivilog(1,stringa1);
        return 1;
    }
    numg_fa=0;
    num_ops=0;
    nrighe=0;
    pa=0;
    pb=0;
    err=0;
    for(;;)
    {
        //Cerco l'importo relativo alla prima operazione su quella categoria
        importo_f=search_util(fil,cat,1,0,&numg_fa,&num_ops,&nrighe,&pa,&pb,&err);
        if(err<0)
        {
            //Si e' verificato un errore: esco
            fclose(fil);
            sprintf(stringa1,"search_util error n. %d",err);
            scrivilog(1,stringa1);
            return err;
        }
        if(!importo_f)
        {
            //Se ottengo un importo nullo vuol dire che il file e' finito.
            //Esco dal loop
            scrivilog(1,(char*)"file finito");
            break;
        }
        if(numg_fa>numg_fin)
        {
            //Ho superato la data che mi interessa. Esco dal loop
            scrivilog(1,(char*)"giorno superato");
            break;
        }
        //Se sono qui ho trovato qualcosa prima della data finale
        if(numg_fa>=numg_ini)
        {
            //Il qualcosa che ho trovato e' nell'arco temporale che mi
            //interessa
            if(importo_f>0)
            {
                //Se era un'entrata devo fare riferimento a pb
                pp=pb;
                plink=pa;
            }
            else
            {
                //Se era un'uscita devo fare riferimento a pa
                pp=pa;
                plink=pb;
            }
            tabellina[numfonti].gain=tabellina[numfonti].gain+importo_f;
            if((importo_f>=0)&&(!plink))
                //Solo se e' una "pura" entrata (niente link) la conteggio
                //nelle entrate globali
                tabellina[numfonti].entrate=tabellina[numfonti].entrate+importo_f;
            if((importo_f<0)&&(!plink))
                //Solo se e' una "pura" uscita (niente link) la conteggio
                //nelle uscite globali
                tabellina[numfonti].uscite=tabellina[numfonti].uscite-importo_f;
            j=0;
            do
            {
                if(tabellina[j].posiz==pp)
                {
                    tabellina[j].gain=tabellina[j].gain+importo_f;
                    if(importo_f>=0)
                    {
                        tabellina[j].entrate=tabellina[j].entrate+importo_f;
                    }
                    else
                    {
                        tabellina[j].uscite=tabellina[j].uscite-importo_f;
                    }
                    break;
                }
                j++;
            }
            while(j<numfonti);
        }
    }
    //Chiudo il file e ritorno correttamente
    fclose(fil);
    return 0;
}

