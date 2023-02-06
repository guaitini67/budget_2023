#include "dlgcompute.h"

/*
Questo file contiene tutta la gestione relativa alla finestra di dialogo "COMPUTE" (Finestra calcoli)
*/

extern const char *gset[];


extern HINSTANCE hInst;

extern char vis;

extern int month[];

extern time_t curtime;
extern struct tm *data_ora;

// Queste sono variabili globali che servono esclusivamente a questa finestra
char cat[100],creris,crerie,crerie1,crerie2,crerie3;;

int numfonti_c,numcat,stato;

long int numg, numg_ini,numg_fin;

double importo,importo_f;

info_smaller t_risorse2[100];
info_risorse t_risorse3[100];

BOOL CALLBACK DlgCompute(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    char stringa[401];

    int i,j,tab[5];



    HWND ELITN =  GetDlgItem(hwndDlg, ELITNUL);
    HWND ELIGN =  GetDlgItem(hwndDlg, ELIGNUL);
    HWND ELIEN =  GetDlgItem(hwndDlg, ELIENUL);
    HWND ELIUN =  GetDlgItem(hwndDlg, ELIUNUL);
    HWND RIEPILO =  GetDlgItem(hwndDlg, C_RIEPILOG);
    HWND RIEPRIS =  GetDlgItem(hwndDlg, C_RIEPRIS);
    HWND DaGior =  GetDlgItem(hwndDlg, ID_GDA);
    HWND DaMes =  GetDlgItem(hwndDlg, ID_MDA);
    HWND DaAnn =  GetDlgItem(hwndDlg, ID_ADA);
    HWND AGior =  GetDlgItem(hwndDlg, ID_GA);
    HWND AMes =  GetDlgItem(hwndDlg, ID_MA);
    HWND AAnn =  GetDlgItem(hwndDlg, ID_AA);
    HWND RiGior =  GetDlgItem(hwndDlg, ID_GG);
    HWND RiMes =  GetDlgItem(hwndDlg, ID_MM);
    HWND RiAnn =  GetDlgItem(hwndDlg, ID_AAAA);
    HWND SELDAT =  GetDlgItem(hwndDlg, SELDATO);
    HWND BFATTO =  GetDlgItem(hwndDlg, C_DONE);


    switch(uMsg)
    {
        case WM_INITDIALOG: //All'apertura della finestra faccio partire il timer ID_TIMER
        {
            //le prossime 3 righe caricano l'icona nella window caption della finestra e ALT+TAB (non ho ben capito quest'ultima)
            HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(icona));
            SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
            SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

            scrivilog(1,(char*)"TCompuDia SetuWindow: inizio setup");
            // punti di stop per i tabulatori nella
            //finestra di riepilogo
            tab[0]=100;
            tab[1]=150;
            tab[2]=200;
            // messaggi che impostano i tabulatori
            SendMessage(RIEPILO,LB_SETTABSTOPS,3,(LPARAM)tab);

            //Default: Elimina voci tutto nullo
            CheckRadioButton(hwndDlg,ELIGNUL,ELITNUL,ELITNUL);
            stato=4;
            //Carico le risorse
            cmp_caricaRisorse(hwndDlg);
            //Carico le categorie
            cmp_caricacategorie(hwndDlg);
            //Il default per "da" e' 01-01-anno corrente
            //e per "a" e' il giorno corrente
            //(Calcoli da inizio anno a oggi)

            time(&curtime);
            data_ora = localtime( &curtime );
            sprintf(stringa,"%02d",data_ora->tm_mday);
            ComboBox_SetText(DaGior,"01");
            ComboBox_SetText(AGior,stringa);
            ComboBox_SetText(RiGior,stringa);
            sprintf(stringa,"%02d",data_ora->tm_mon+1);
            ComboBox_SetText(DaMes,"01");
            ComboBox_SetText(AMes,stringa);
            ComboBox_SetText(RiMes,stringa);
            sprintf(stringa,"%04d",data_ora->tm_year+1900);
            ComboBox_SetText(DaAnn,stringa);
            ComboBox_SetText(AAnn,stringa);
            ComboBox_SetText(RiAnn,stringa);

            //0 = saldoinanno
            ComboBox_InsertString(SELDAT,-1,"Saldo inizio anno");
            //1 = saldoinimese
            ComboBox_InsertString(SELDAT,-1,"Saldo inizio mese");
            //2 = saldooggi
            ComboBox_InsertString(SELDAT,-1,"Saldo a oggi");
            //3 = saldofinmese
            ComboBox_InsertString(SELDAT,-1,"Saldo fine mese");
            //4 = saldofinanno
            ComboBox_InsertString(SELDAT,-1,"Saldo fine anno");
            //5 = entrateinianno
            ComboBox_InsertString(SELDAT,-1,"Entrate da inizio anno");
            //6 = entrateinimese
            ComboBox_InsertString(SELDAT,-1,"Entrate da inizio mese");
            //7 = entrateoggi
            ComboBox_InsertString(SELDAT,-1,"Entrate di oggi");
            //8 = entratefinmese
            ComboBox_InsertString(SELDAT,-1,"Entrate del mese");
            //9 = entratefinanno
            ComboBox_InsertString(SELDAT,-1,"Entrate fino a fine anno");
            //10 = usciteinianno
            ComboBox_InsertString(SELDAT,-1,"Uscite da inizio anno");
            //11 = usciteinimese
            ComboBox_InsertString(SELDAT,-1,"Uscite da inizio mese");
            //12 = usciteoggi
            ComboBox_InsertString(SELDAT,-1,"Uscite di oggi");
            //13 = uscitefinmese
            ComboBox_InsertString(SELDAT,-1,"Uscite del mese");
            //14 = uscitefinanno
            ComboBox_InsertString(SELDAT,-1,"Uscite fino a fine anno");
            //15 = gaininianno
            ComboBox_InsertString(SELDAT,-1,"Guadagno da inizio anno");
            //16 = gaininimese
            ComboBox_InsertString(SELDAT,-1,"Guadagno da inizio mese");
            //17 = gainoggi
            ComboBox_InsertString(SELDAT,-1,"Guadagno di oggi");
            //18 = gainfinmese
            ComboBox_InsertString(SELDAT,-1,"Guadagno fino a fine mese");
            //19 = gainfinanno
            ComboBox_InsertString(SELDAT,-1,"Guadagno fino a fine anno");
            //Valore di default
            ComboBox_SetText(SELDAT,"Saldo a oggi");

            //Default di ordinamento: decrescenti per i numeri
            //e crescenti per le stringhe di testo
            creris=1;
            crerie=1;
            crerie1=0;
            crerie2=0;
            crerie3=0;

            cmp_riepiloga(hwndDlg);
            scrivilog(1,(char*)"TCompuDia: Eseguito setup");
        }
        return TRUE;

        case WM_CLOSE:
        {
            vis=0; //rimetto vis=0 e chiudo: potro' riaprire questa finestra da quella principale
            //siccome l'ho aperta non modale devo usare DestryWindow e non EndDialog
            DestroyWindow( hwndDlg );
        }
        return TRUE;

        case WM_DESTROY:
        {
            //Questa funzione interrompe il thread del messaggio
            //Immagino che permette di uscire dal message loop (ciclo while) attivato alla chiamata
            //dalla finestra genitrice
            PostQuitMessage(0);
        }
        return TRUE;

        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case OG_VAI:
                    //Mostra i movimenti alla data indicata nei campi
                    //di riepilogo
                    scrivilog(1,(char*)"TCompuDia: Forse Mouse Vai");
                    cmp_riepiloga(hwndDlg);
                    break;

                case OG_CANC:
                    cmp_HandCancella(hwndDlg);
                    break;

                case AVANTI:
                    cmp_HandGDopo(hwndDlg);
                    break;

                case DIETRO:
                    cmp_HandGPrima(hwndDlg);
                    break;

                case SELBANCA1:
                    HandSelSaldoRisorsa(hwndDlg,wParam);
                    break;

                case SELDATO:
                    HandSelTipoDato(hwndDlg,wParam);
                    break;

                case ELIGNUL:
                    HandCheckEliGN(hwndDlg,wParam);
                    break;

                case ELIENUL:
                    HandCheckEliEN(hwndDlg,wParam);
                    break;

                case ELIUNUL:
                    HandCheckEliUN(hwndDlg,wParam);
                    break;

                case ELITNUL:
                    HandCheckEliTN(hwndDlg,wParam);
                    break;

                case DOIT:
                    HandEsegui(hwndDlg);
                    break;

                case ID_GG:
                    cmp_HandValRiG(hwndDlg,wParam);
                    break;

                case ID_MM:
                    cmp_HandValRiM(hwndDlg,wParam);
                    break;

                case ID_AAAA:
                    cmp_HandValRiA(hwndDlg,wParam);
                    break;

                case ID_GDA:
                    cmp_HandValRiGDa(hwndDlg,wParam);
                    break;

                case ID_MDA:
                    cmp_HandValRiMDa(hwndDlg,wParam);
                    break;

                case ID_ADA:
                    cmp_HandValRiADa(hwndDlg,wParam);
                    break;

                case ID_GA:
                    cmp_HandValRiGA(hwndDlg,wParam);
                    break;

                case ID_MA:
                    cmp_HandValRiMA(hwndDlg,wParam);
                    break;

                case ID_AA:
                    cmp_HandValRiAA(hwndDlg,wParam);
                    break;

                case C_CALCO:
                    cmp_HandCalcola(hwndDlg);
                    break;

                case ANNUL:
                    HandPulisci(hwndDlg);
                    break;

                case C_DONE:
                    scrivilog(1,(char*)"Compute: Fatto spinto");
                    vis=0; //rimetto vis=0 e chiudo: potro' riaprire questa finestra da quella principale
                    //siccome l'ho aperta non modale devo usare DestryWindow e non EndDialog
                    DestroyWindow( hwndDlg );
                    break;

                case C_RIEPILOG:
                    HandRiepilo(hwndDlg,wParam);
                    break;

                case C_RIEPILOG1:
                    HandRiep1(hwndDlg,wParam);
                    break;

                case C_RIEPILOG2:
                    HandRiep2(hwndDlg,wParam);
                    break;

                case C_RIEPILOG3:
                    HandRiep3(hwndDlg,wParam,uMsg);
                    break;

                case C_RIEPRIS:
                    HandRiepRis(hwndDlg,wParam);
                    break;

                case C_ORDRIS:
                    HandOrdRis(hwndDlg,wParam);
                    break;

                case C_ORDRIEP:
                    HandOrdRiep(hwndDlg,wParam);
                    break;

                case C_ORDRIEP1:
                    HandOrdRiep1(hwndDlg,wParam);
                    break;

                case C_ORDRIEP2:
                    HandOrdRiep2(hwndDlg,wParam);
                    break;

                case C_ORDRIEP3:
                    HandOrdRiep3(hwndDlg,wParam);
                    break;

                case ACC_CTNEXT:
                    //Se digito CTRL+page down, mi equivale a premere VAI
                    //Metto il focus su SELDAT per toglierlo
                    //ai campi date e impedire date errate
                    //(inoltre e' anche una cosa desiderabile)
                    scrivilog(1,(char*)"TCompuDia CTRL-PGDWN");
                    SendDlgItemMessage(hwndDlg,
                    OG_VAI, BM_SETSTATE, TRUE, 0);
                    //Loop di ritardo (gioco con i e j per far passare il tempo)
                    for(i=0;i<10000;i++)
                        j=2*i;
                    i=j;
                    SetFocus(SELDAT);
                    SendDlgItemMessage(hwndDlg,
                    OG_VAI, BM_SETSTATE, FALSE, 0);
                    scrivilog(1,(char*)"TCompuDia: Forse Mouse Vai");
                    cmp_riepiloga(hwndDlg);
                    break;

                case ACC_CTUP:
                    //Se digito CTRL+freccia su, mi visualizza il giorno prima
                    //Metto il focus su SELDAT per toglierlo
                    //ai campi date e impedire date errate
                    //(inoltre e' anche una cosa desiderabile)
                    //C'e' anche l'effetto speciale del bottone premuto
                    scrivilog(1,(char*)"TCompuDia CTRL-UP");
                    SendDlgItemMessage(hwndDlg,
                    DIETRO, BM_SETSTATE, TRUE, 0);
                    //Loop di ritardo
                    for(i=0;i<10000;i++)
                        j=2*i;
                    SetFocus(SELDAT);
                    SendDlgItemMessage(hwndDlg,
                    DIETRO, BM_SETSTATE, FALSE, 0);
                    cmp_HandGPrima(hwndDlg);
                    break;

                case ACC_CTDOWN:
                    //Se digito CTRL+freccia giu', mi visualizza il giorno dopo
                    //Metto il focus su SELDAT per toglierlo
                    //ai campi date e impedire date errate
                    //(inoltre e' anche una cosa desiderabile)
                    //C'e' anche l'effetto speciale del bottone premuto
                    scrivilog(1,(char*)"TCompuDia CTRL-DWN");
                    SendDlgItemMessage(hwndDlg,
                    AVANTI, BM_SETSTATE, TRUE, 0);
                    //Loop di ritardo
                    for(i=0;i<10000;i++)
                        j=2*i;
                    SetFocus(SELDAT);
                    SendDlgItemMessage(hwndDlg,
                    AVANTI, BM_SETSTATE, FALSE, 0);
                    cmp_HandGDopo(hwndDlg);
                    break;

                case ACC_CTA:
                    //CTRL+A Mi posiziona sulla data A
                    scrivilog(1,(char*)"TCompuDia CTRL-A");
                    SetFocus(AGior);
                    break;

                case ACC_CTD:
                    //CTRL+D Mi posiziona sulla data DA
                    scrivilog(1,(char*)"TCompuDia CTRL-D");
                    SetFocus(DaGior);
                    break;

                case ACC_CTE:
                    //CTRL+E Mi equivale a premere PULISCI
                    scrivilog(1,(char*)"TCompuDia CTRL-E");
                    SendDlgItemMessage(hwndDlg,ANNUL, BM_SETSTATE, TRUE, 0);
                    //Loop di ritardo
                    for(i=0;i<10000;i++)
                        j=2*i;
                    SendDlgItemMessage(hwndDlg,ANNUL, BM_SETSTATE, FALSE, 0);
                    HandPulisci(hwndDlg);
                    break;

                case ACC_CTG:
                    //Se digito CTRL+G, mi posiziona su data di riepilogo
                    scrivilog(1,(char*)"TCompuDia CTRL-G");
                    SetFocus(RiGior);
                    break;

                case ACC_CTR:
                    //Se digito CTRL+R, mi mette il fuoco sul check selezionato
                    scrivilog(1,(char*)"TCompuDia CTRL-R");
                    switch(stato)
                    {
                        case 1:
                            SetFocus(ELIGN);
                            break;
                        case 2:
                            SetFocus(ELIEN);
                            break;
                        case 3:
                            SetFocus(ELIUN);
                            break;
                        case 4:
                            SetFocus(ELITN);
                            break;
                        default:
                        ;
                    }
                    break;

                case ACC_CTS:
                    //CTRL+S Mi posiziona sul riepilogo box risorse
                    scrivilog(1,(char*)"TCompuDia CTRL-S");
                    SetFocus(RIEPRIS);
                    break;

                case ACC_CTW:
                    //CTRL+W Mi equivale a premere CALCOLA
                    scrivilog(1,(char*)"TCompuDia CTRL-W");
                    SendDlgItemMessage(hwndDlg,C_CALCO, BM_SETSTATE, TRUE, 0);
                    //Loop di ritardo
                    for(i=0;i<10000;i++)
                        j=2*i;
                    SendDlgItemMessage(hwndDlg,C_CALCO, BM_SETSTATE, FALSE, 0);
                    lanciacalc(hwndDlg);
                    break;

                case ACC_ESCAPE:
                    //Se spingo ESC, mi posiziona su FATTO
                    scrivilog(1,(char*)"TCompuDia CTRL-ESC");
                    SetFocus(BFATTO);
                    Button_SetState(BFATTO,TRUE);
                    break;

                case ACC_F1:
                    //Se spingo F1, mi equivale a ordinare secondo le risorse
                    scrivilog(1,(char*)"TCompuDia F1");
                    SendDlgItemMessage(hwndDlg,C_ORDRIS, BM_SETSTATE, TRUE, 0);
                    //Loop di ritardo
                    for(i=0;i<10000;i++)
                        j=2*i;
                    SendDlgItemMessage(hwndDlg,C_ORDRIS, BM_SETSTATE, FALSE, 0);
                    HandOrdRis(hwndDlg,wParam);
                    break;

                case ACC_F2:
                    //Se spingo F2, mi equivale a ordinare secondo le categorie
                    scrivilog(1,(char*)"TCompuDia F2");
                    SendDlgItemMessage(hwndDlg,C_ORDRIEP, BM_SETSTATE, TRUE, 0);
                    //Loop di ritardo
                    for(i=0;i<10000;i++)
                        j=2*i;
                    SendDlgItemMessage(hwndDlg,C_ORDRIEP, BM_SETSTATE, FALSE, 0);
                    HandOrdRiep(hwndDlg,wParam);
                    break;

                case ACC_F3:
                    //Se spingo F3, mi equivale a ordinare secondo il guadagno
                    scrivilog(1,(char*)"TCompuDia F3");
                    SendDlgItemMessage(hwndDlg,C_ORDRIEP1, BM_SETSTATE, TRUE, 0);
                    //Loop di ritardo
                    for(i=0;i<10000;i++)
                        j=2*i;
                    SendDlgItemMessage(hwndDlg,C_ORDRIEP1, BM_SETSTATE, FALSE, 0);
                    HandOrdRiep1(hwndDlg,wParam);
                    break;

                case ACC_F4:
                    //Se spingo F4, mi equivale a ordinare secondo le entrate
                    scrivilog(1,(char*)"TCompuDia F4");
                    SendDlgItemMessage(hwndDlg,C_ORDRIEP2, BM_SETSTATE, TRUE, 0);
                    //Loop di ritardo
                    for(i=0;i<10000;i++)
                        j=2*i;
                    SendDlgItemMessage(hwndDlg,C_ORDRIEP2, BM_SETSTATE, FALSE, 0);
                    HandOrdRiep2(hwndDlg,wParam);
                    break;

                case ACC_F5:
                    //Se spingo F5, mi equivale a ordinare secondo le uscite
                    scrivilog(1,(char*)"TCompuDia F5");
                    SendDlgItemMessage(hwndDlg,C_ORDRIEP3, BM_SETSTATE, TRUE, 0);
                    //Loop di ritardo
                    for(i=0;i<10000;i++)
                        j=2*i;
                    SendDlgItemMessage(hwndDlg,C_ORDRIEP3, BM_SETSTATE, FALSE, 0);
                    HandOrdRiep3(hwndDlg,wParam);
                    break;

                case ACC_CTDELETE:
                    //Se digito CTRL+canc, mi equivale a premere CANCELLA
                    //C'e' anche l'effetto speciale del bottone premuto
                    scrivilog(1,(char*)"TCompuDia CTRL-DEL");
                    SendDlgItemMessage(hwndDlg,OG_CANC, BM_SETSTATE, TRUE, 0);
                    //Loop di ritardo
                    for(i=0;i<10000;i++)
                        j=2*i;
                    SendDlgItemMessage(hwndDlg,OG_CANC, BM_SETSTATE, FALSE, 0);
                    cmp_HandCancella(hwndDlg);
                    break;

                default:
                    ;
            }
        }
        return TRUE;


    }

    return FALSE;
}

int cmp_riepiloga(HWND hwndDlg)
{
    char stringa[401],nomefile[30];

    int a,m,g,erno;

    long int numg;

    struct tm tcheck;

    HWND RiGior =  GetDlgItem(hwndDlg, ID_GG);
    HWND RiMes =  GetDlgItem(hwndDlg, ID_MM);
    HWND RiAnn =  GetDlgItem(hwndDlg, ID_AAAA);

    ComboBox_GetText(RiAnn,stringa,5);
    sscanf(stringa,"%d",&a);
    ComboBox_GetText(RiMes,stringa,3);
    sscanf(stringa,"%d",&m);
    ComboBox_GetText(RiGior,stringa,3);
    sscanf(stringa,"%d",&g);
    numg=(long int)a*10000+m*100+g;
    sprintf(nomefile,"%d.bud",a);
    //con tcheck e la funzione mktime determino il
    //giorno della settimana
    tcheck.tm_year=a-1900;
    tcheck.tm_mon=m-1;
    tcheck.tm_mday=g;
    tcheck.tm_hour=0;
    tcheck.tm_min=0;
    tcheck.tm_sec=1;
    tcheck.tm_isdst=-1;
    if(mktime(&tcheck)==-1)
        tcheck.tm_wday=7;
    sprintf(stringa,"%s",gset[tcheck.tm_wday]);
    SetDlgItemText(hwndDlg,GSETTIMANA,stringa);

    if((erno=caricasaldi(numg,t_risorse3))<0)
    {
        //C'e' stato un problema nella lettura di basedata.dat
        sprintf(stringa,"TCompuDia, riepiloga, caricasaldi,controllabasedata: erno n. %d",erno);
        MessageBox(hwndDlg,stringa,
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        scrivilog(1,stringa);
        return -1;
    }
    if((erno=aggiornasaldi(numg,numfonti_c,nomefile,t_risorse3))<0)
    {
        //C'e' stato un problema nella lettura del file di dati
        //o nella riscrittura di basedata.dat
        sprintf(stringa,"TCompuDia, riepiloga, aggiornasaldi : erno n. %d",erno);
        MessageBox(hwndDlg,stringa,
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        scrivilog(1,stringa);
        return -1;
    }
    mostrasaldi(hwndDlg,SELBANCA1,SELDATO,MOSTRADATO,t_risorse3);
    return 0;
}

void cmp_caricaRisorse(HWND hwndDlg)
//Legge basedata.dat ed estrae le informazioni sulle
//risorse. Molte qui non servono
{
    char stringa[401];
    int i,erno;

    HWND SELBANC =  GetDlgItem(hwndDlg, SELBANCA);
    HWND SELBANC1 =  GetDlgItem(hwndDlg, SELBANCA1);


    erno=caricav(t_risorse3,&numfonti_c);
    if(erno<0)
    {
        //Se c'e' qualche problema da caricav
        sprintf(stringa,"caricaRisorse Problema caricav erno= %d",erno);
        MessageBox(hwndDlg,stringa,
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        scrivilog(1,stringa);
        return;
    }
    ComboBox_ResetContent(SELBANC);
    ComboBox_ResetContent(SELBANC1);
    for(i=0;i<=numfonti_c;i++)
    {
        t_risorse2[i].posiz=t_risorse3[i].posiz;
        ComboBox_AddString(SELBANC,t_risorse3[i].nomeconv);
        ComboBox_AddString(SELBANC1,t_risorse3[i].nomeconv);
    }
    //Default SELBANC e' globale
    ComboBox_GetLBText(SELBANC,numfonti_c,stringa);
    ComboBox_SetText(SELBANC,stringa);
    //Default SELBANC1 e' prima voce
    ComboBox_GetLBText(SELBANC1,0,stringa);
    ComboBox_SetText(SELBANC1,stringa);
}

void cmp_caricacategorie(HWND hwndDlg)
//Carica la lista delle categorie con
//la lista memorizzata in categ.dat
{
    char stringa[401],cat[100];
    int i;

    FILE *tmp;

    HWND CATE =  GetDlgItem(hwndDlg, CATEG);

    if(!(tmp=fopen("categ.dat","r")))
    {
        //Non trovo il file, ne inizializzo
        //uno vuoto e ritorno
        tmp=fopen("categ.dat","w");
        fprintf(tmp,"0\n");
        fclose(tmp);
        ComboBox_InsertString(CATE,0,"Tutte le categorie");
        //Default e' tutte le categorie
        ComboBox_SetText(CATE,"Tutte le categorie");
        return;
    }
    ComboBox_ResetContent(CATE);
    fgets(stringa,400,tmp);
    sscanf(stringa,"%d\n",&numcat);
    for(i=0;i<numcat;i++)
    {
        fgets(stringa,400,tmp);
        sscanf(stringa,"%[!-~ ‘’@òàùèéì°ç§]\n",cat);
        ComboBox_AddString(CATE,cat);
    }
    fclose(tmp);
    ComboBox_InsertString(CATE,0,"Tutte le categorie");
    //Default e' tutte le categorie
    ComboBox_SetText(CATE,"Tutte le categorie");
}

void EseguiCalcolo(HWND hwndDlg,int a,int numfile)
{
    char stringa[401],nomefile[30];
    int i,k;

    //Uso l'ultimo elemento della tabella per
    //contenere i dati di tutte le risorse
    for(i=0;i<=numfonti_c;i++)
    {
        t_risorse2[i].entrate=0;
        t_risorse2[i].uscite=0;
        t_risorse2[i].gain=0;
    }
    for(i=0;i<=numfile;i++)
    {
        //La mia ricerca sara' su tutti i file che mi interessano
        sprintf(nomefile,"%d.bud",a+i);
        k=search_year(nomefile,cat,t_risorse2,numfonti_c,numg_ini,numg_fin);
        if(k<0)
        {
            sprintf(stringa,"Compute:search_year err. n. %d",k);
            MessageBox(hwndDlg,stringa,
            "Problema!",MB_ICONEXCLAMATION | MB_OK);
            scrivilog(1,stringa);
            return;
        }
    }
}

void mostradato(HWND hwndDlg)
//Mostra il dato numerico
//(guadagno, entrate, uscite)
//per la risorsa selezionata
{
    char stringa[401];

    int index;

    HWND SELBANC =  GetDlgItem(hwndDlg, SELBANCA);
    HWND RIEPILO =  GetDlgItem(hwndDlg, C_RIEPILOG);
    HWND RIEP1 =  GetDlgItem(hwndDlg, C_RIEPILOG1);
    HWND RIEP2 =  GetDlgItem(hwndDlg, C_RIEPILOG2);
    HWND RIEP3 =  GetDlgItem(hwndDlg, C_RIEPILOG3);
    HWND RIEPRIS =  GetDlgItem(hwndDlg, C_RIEPRIS);

    ComboBox_GetCurSel(SELBANC);

    if((index=ComboBox_GetCurSel(SELBANC))<0)
    {
        //Se la stringa non corrisponde a una risorsa valida
        //metto il globale di default
        index=numfonti_c;
        ComboBox_SetCurSel(SELBANC,numfonti_c);
    }
    sprintf(stringa,"%s",t_risorse3[index].nomeconv);
    ListBox_AddString(RIEPRIS,stringa);
    ListBox_AddString(RIEPILO,cat);
    sprintf(stringa,"%.2lf\t",t_risorse2[index].gain);
    ListBox_AddString(RIEP1,stringa);
    sprintf(stringa,"%.2lf\t",t_risorse2[index].entrate);
    ListBox_AddString(RIEP2,stringa);
    sprintf(stringa,"%.2lf\t",t_risorse2[index].uscite);
    ListBox_AddString(RIEP3,stringa);
}

void cmp_HandGDopo(HWND hwndDlg)
//Mostra i movimenti del giorno successivo
{
    char stringa[401];
    int g,m,a;

    HWND RiGior =  GetDlgItem(hwndDlg, ID_GG);
    HWND RiMes =  GetDlgItem(hwndDlg, ID_MM);
    HWND RiAnn =  GetDlgItem(hwndDlg, ID_AAAA);

    scrivilog(1,(char*)"TCoumpuDia: Forse Mouse Gdopo");
    //Assicuro la validita' della data
    //sul riepilogo e la ricavo
    caricacampi(hwndDlg,ID_GG,ID_MM,ID_AAAA);
    ComboBox_GetText(RiAnn,stringa,5);
    sscanf(stringa,"%d",&a);
    ComboBox_GetText(RiMes,stringa,3);
    sscanf(stringa,"%d",&m);
    ComboBox_GetText(RiGior,stringa,3);
    sscanf(stringa,"%d",&g);
    //Determino qual e' il giorno dopo
    if(g==month[m-1])
    {
        //se e' l'ultimo del mese
        if(m<12)
        {
            //se non e' dicembre incrementa mese
            //e g diventa 1
            m++;
            g=1;
        }
        else
        {
            //Se e' dicembre:
            //incrementa anno, mese=gennaio
            //giorno=1
            a++;
            m=1;
            g=1;
            if(isbises(a+1))
                //Se l'anno successivo e' bisestile: giorni di febbraio=29;
                month[1]=29;
            else
                //Se non e' bisestile: giorni di febbraio=28;
                month[1]=28;
        }
    }
    else
        //Se non e' l'ultimo del mese incrementa g
        g++;
    ComboBox_SetText(RiGior,"");
    ComboBox_SetText(RiMes,"");
    ComboBox_SetText(RiAnn,"");
    sprintf(stringa,"%02d",g);
    ComboBox_SetText(RiGior,stringa);
    sprintf(stringa,"%02d",m);
    ComboBox_SetText(RiMes,stringa);
    sprintf(stringa,"%04d",a);
    ComboBox_SetText(RiAnn,stringa);
    cmp_riepiloga(hwndDlg);
  }

 void cmp_HandGPrima(HWND hwndDlg)
//Mostra i movimenti del giorno precedente
{
    char stringa[401];
    int g,m,a;

    HWND RiGior =  GetDlgItem(hwndDlg, ID_GG);
    HWND RiMes =  GetDlgItem(hwndDlg, ID_MM);
    HWND RiAnn =  GetDlgItem(hwndDlg, ID_AAAA);

    scrivilog(1,(char*)"TCompuDia: Forse Mouse Gprima");
    //Assicuro la validita' della data
    //sul riepilogo e la ricavo
    caricacampi(hwndDlg,ID_GG,ID_MM,ID_AAAA);
    ComboBox_GetText(RiAnn,stringa,5);
    sscanf(stringa,"%d",&a);
    ComboBox_GetText(RiMes,stringa,3);
    sscanf(stringa,"%d",&m);
    ComboBox_GetText(RiGior,stringa,3);
    sscanf(stringa,"%d",&g);
    //Determino qual e' il giorno prima
    if(g==1)
    {
        //Se e' il primo del mese
        if(m>1)
        {
            //Se il mese non e' gennaio posso decrementare
            m--;
            //g diventa l'ultimo del mese precedente
            g=month[m-1];
        }
        else
        {
            //Se il mese e' gennaio:
            //decrementa anno, mese=dicembre
            //giorno=31
            a--;
            m=12;
            g=31;
        }
    }
    else
        //Se non e' il primo del mese decrementa il giorno
        g--;
    ComboBox_SetText(RiGior,"");
    ComboBox_SetText(RiMes,"");
    ComboBox_SetText(RiAnn,"");
    sprintf(stringa,"%02d",g);
    ComboBox_SetText(RiGior,stringa);
    sprintf(stringa,"%02d",m);
    ComboBox_SetText(RiMes,stringa);
    sprintf(stringa,"%04d",a);
    ComboBox_SetText(RiAnn,stringa);
    cmp_riepiloga(hwndDlg);
}

void HandSelSaldoRisorsa(HWND hwndDlg, WPARAM wParam)
//Mi fa vedere il dato della risorsa che seleziono
{
    if(HIWORD(wParam)==CBN_SELENDOK)
    {
        mostrasaldi(hwndDlg,SELBANCA1,SELDATO,MOSTRADATO,t_risorse3);
    }
}

void HandSelTipoDato(HWND hwndDlg, WPARAM wParam)
//Mi fa vedere il tipo di dato che seleziono
{
    if(HIWORD(wParam)==CBN_SELENDOK)
    {
        mostrasaldi(hwndDlg,SELBANCA1,SELDATO,MOSTRADATO,t_risorse3);
    }
}

void HandCheckEliGN(HWND hwndDlg, WPARAM wParam)
//Seleziono elimina guadagno nullo e deseleziono gli altri
{
    if(HIWORD(wParam)==BN_CLICKED)
    {
        scrivilog(1,(char*)"TCompuDia Mouse HEliGN");
        EliGN(hwndDlg);
    }
}

void EliGN(HWND hwndDlg)
{
    if(stato==1)
        //Se era gia' selezionato non faccio nulla
        return;
    CheckRadioButton(hwndDlg,ELIGNUL,ELITNUL,ELIGNUL);
    stato=1;
}

void HandCheckEliEN(HWND hwndDlg, WPARAM wParam)
//Seleziono elimina entrate nulle e deseleziono gli altri
{
    if(HIWORD(wParam)==BN_CLICKED)
    {
        scrivilog(1,(char*)"TCompuDia Mouse HEliEN");
        EliEN(hwndDlg);
    }
}

void EliEN(HWND hwndDlg)
{
    if(stato==2)
        //Se era gia' selezionato non faccio nulla
        return;
    CheckRadioButton(hwndDlg,ELIGNUL,ELITNUL,ELIENUL);
    stato=2;
}

void HandCheckEliUN(HWND hwndDlg, WPARAM wParam)
//Seleziono elimina uscite nulle e deseleziono gli altri
{
    if(HIWORD(wParam)==BN_CLICKED)
    {
        scrivilog(1,(char*)"TCompuDia Mouse HEliUN");
        EliUN(hwndDlg);
    }
}

void EliUN(HWND hwndDlg)
{
    if(stato==3)
        //Se era gia' selezionato non faccio nulla
        return;
    CheckRadioButton(hwndDlg,ELIGNUL,ELITNUL,ELIUNUL);
    stato=3;
}


void HandCheckEliTN(HWND hwndDlg, WPARAM wParam)
//Seleziono elimina tutto nullo e deseleziono gli altri
{
    if(HIWORD(wParam)==BN_CLICKED)
    {
        scrivilog(1,(char*)"TCompuDia Mouse HEliTN");
        EliTN(hwndDlg);
    }
}

void EliTN(HWND hwndDlg)
{
    if(stato==4)
        //Se era gia' selezionato non faccio nulla
        return;
    CheckRadioButton(hwndDlg,ELIGNUL,ELITNUL,ELITNUL);
    stato=4;
}

void HandEsegui(HWND hwndDlg)
{

    char cancella,stringa[401];
    int i,k;
    double gain,in,out;

    HWND RIEP1 =  GetDlgItem(hwndDlg, C_RIEPILOG1);
    HWND RIEP2 =  GetDlgItem(hwndDlg, C_RIEPILOG2);
    HWND RIEP3 =  GetDlgItem(hwndDlg, C_RIEPILOG3);
    HWND RIEPILO =  GetDlgItem(hwndDlg, C_RIEPILOG);
    HWND RIEPRIS =  GetDlgItem(hwndDlg, C_RIEPRIS);


    scrivilog(1,(char*)"TCompuDia  HandEsegui");
    k=ListBox_GetCount(RIEPILO);
    for(i=0;i<k;i++)
    {
        ListBox_GetText(RIEP1,i,stringa);
        sscanf(stringa,"%lf",&gain);
        ListBox_GetText(RIEP2,i,stringa);
        sscanf(stringa,"%lf",&in);
        ListBox_GetText(RIEP3,i,stringa);
        sscanf(stringa,"%lf",&out);
        cancella=0;
        switch(stato)
        {
            case 1:
                if(!gain)
                    cancella=1;
                break;
            case 2:
                if(!in)
                    cancella=1;
                break;
            case 3:
                if(!out)
                    cancella=1;
                break;
            case 4:
                if((!gain)&&(!in)&&(!out))
                    cancella=1;
                break;
            default:
                ;
        }
        if(cancella)
        {
            ListBox_DeleteString(RIEPILO,i);
            ListBox_DeleteString(RIEP1,i);
            ListBox_DeleteString(RIEP2,i);
            ListBox_DeleteString(RIEP3,i);
            ListBox_DeleteString(RIEPRIS,i);
            i--;
            k--;
        }
    }
}

void HandValData(HWND hwndDlg,int FFst,int FF1,int FF2,int FF3,WPARAM wParam)
{
    if(HIWORD(wParam)==CBN_SETFOCUS)
    {
        caricacampi(hwndDlg,FF1,FF2,FF3);
    }
    if(HIWORD(wParam)==CBN_KILLFOCUS)
    {
        caricacampi(hwndDlg,FF1,FF2,FF3);
    }
}

void cmp_HandValRiG(HWND hwndDlg,WPARAM wParam)
{
    HandValData(hwndDlg,ID_GG,ID_GG,ID_MM,ID_AAAA,wParam);
}

void cmp_HandValRiM(HWND hwndDlg,WPARAM wParam)
{
    HandValData(hwndDlg,ID_MM,ID_GG,ID_MM,ID_AAAA,wParam);
}

void cmp_HandValRiA(HWND hwndDlg,WPARAM wParam)
{
    HandValData(hwndDlg,ID_AAAA,ID_GG,ID_MM,ID_AAAA,wParam);
}

void cmp_HandValRiGDa(HWND hwndDlg,WPARAM wParam)
{
    HandValData(hwndDlg,ID_GDA,ID_GDA,ID_MDA,ID_ADA,wParam);
}

void cmp_HandValRiMDa(HWND hwndDlg,WPARAM wParam)
{
    HandValData(hwndDlg,ID_MDA,ID_GDA,ID_MDA,ID_ADA,wParam);
}

void cmp_HandValRiADa(HWND hwndDlg,WPARAM wParam)
{
    HandValData(hwndDlg,ID_ADA,ID_GDA,ID_MDA,ID_ADA,wParam);
}

void cmp_HandValRiGA(HWND hwndDlg,WPARAM wParam)
{
    HandValData(hwndDlg,ID_GA,ID_GA,ID_MA,ID_AA,wParam);
}

void cmp_HandValRiMA(HWND hwndDlg,WPARAM wParam)
{
    HandValData(hwndDlg,ID_MA,ID_GA,ID_MA,ID_AA,wParam);
}

void cmp_HandValRiAA(HWND hwndDlg,WPARAM wParam)
{
    HandValData(hwndDlg,ID_AA,ID_GA,ID_MA,ID_AA,wParam);
}

void cmp_HandCalcola(HWND hwndDlg)
{
    scrivilog(1,(char*)"Mouse: Calcola spinto");
    lanciacalc(hwndDlg);
}

void lanciacalc(HWND hwndDlg)
{
    char stringa[401];
    int j,k,g,m,a,numfile;

    HWND DaGior =  GetDlgItem(hwndDlg, ID_GDA);
    HWND DaMes =  GetDlgItem(hwndDlg, ID_MDA);
    HWND DaAnn =  GetDlgItem(hwndDlg, ID_ADA);
    HWND AGior =  GetDlgItem(hwndDlg, ID_GA);
    HWND AMes =  GetDlgItem(hwndDlg, ID_MA);
    HWND AAnn =  GetDlgItem(hwndDlg, ID_AA);
    HWND CATE =  GetDlgItem(hwndDlg, CATEG);
    HWND WAIT =  GetDlgItem(hwndDlg, C_ASPETTA);

    scrivilog(1,(char*)"Compute: Calcola spinto");
    //Per iniziare il giorno di inizio e quello di fine
    //Mi assicuro che la data di partenza sia corretta
    caricacampi(hwndDlg,ID_GDA,ID_MDA,ID_ADA);
    ComboBox_GetText(DaAnn,stringa,5);
    sscanf(stringa,"%d",&a);
    ComboBox_GetText(DaMes,stringa,3);
    sscanf(stringa,"%d",&m);
    ComboBox_GetText(DaGior,stringa,3);
    sscanf(stringa,"%d",&g);
    numg_ini=(long int)a*10000+m*100+g;
    //Mi assicuro che la data di arrivo sia corretta
    caricacampi(hwndDlg,ID_GA,ID_MA,ID_AA);
    ComboBox_GetText(AAnn,stringa,5);
    sscanf(stringa,"%d",&a);
    ComboBox_GetText(AMes,stringa,3);
    sscanf(stringa,"%d",&m);
    ComboBox_GetText(AGior,stringa,3);
    sscanf(stringa,"%d",&g);
    numg_fin=(long int)a*10000+m*100+g;
    if(numg_ini>numg_fin)
    //Se le due date non sono successive esco
    {
        MessageBox(hwndDlg,
        "Data di partenza deve essere anteriore a data finale",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        scrivilog(1,(char*)"Compute: Data di partenza deve essere anteriore a data finale");
        return;
    }
    //Determino su quanti file devo lavorare.
    //Infatti le due date potrebbero essere su anni diversi
    ComboBox_GetText(DaAnn,stringa,5);
    sscanf(stringa,"%d",&a);
    ComboBox_GetText(AAnn,stringa,5);
    sscanf(stringa,"%d",&j);
    numfile=j-a;
    ComboBox_GetText(CATE,cat,42);
    if((k=ComboBox_FindStringExact(CATE,-1,cat))<0)
    {
        //Se la cosa scritta non corrisonde a una categoria non fare nulla
        //(ripulisco la casella per evidenziare il "misfatto")
        ComboBox_SetText(CATE,"");
        return;
    }
    Static_SetText(WAIT,"Attendere prego...");
    if(!k)
    {
        //Se ho chiesto tutte le categorie
        for(j=1;j<=numcat;j++)
        {
            sprintf(stringa,"Attendere prego... %d",j);
            Static_SetText(WAIT,stringa);
            ComboBox_GetLBText(CATE,j,cat);
            EseguiCalcolo(hwndDlg,a,numfile);
            mostradato(hwndDlg);
        }
    }
    else
    {
        EseguiCalcolo(hwndDlg,a,numfile);
        mostradato(hwndDlg);
    }
    //Ordino di default per guadagno netto decrescente
    crerie1=0;
    ordinanum(hwndDlg,C_RIEPILOG1,C_RIEPILOG2,C_RIEPILOG3,C_RIEPILOG,C_RIEPRIS,crerie1);
    Static_SetText(WAIT,"Ordinato decrescente per guadagno netto");
    crerie1=1;
    scrivilog(1,(char*)"Compute: Calcola eseguito");
}

void ordinanum(HWND hwndDlg,int TTarget,int LL1,int LL2,int RRIEPILO,int RRIEPRIS,char cresc)
//Questa funzione ordina le listbox contenenti numeri in
//ordine decrescente (cresc=0) o crescente (cresc=1)
//sincronizzando allo stesso tempo le altre listbox
{
    char stringa[400],stringa1[401];
    int i,j,numitem;

    HWND Target =  GetDlgItem(hwndDlg, TTarget);
    HWND L1 =  GetDlgItem(hwndDlg, LL1);
    HWND L2 =  GetDlgItem(hwndDlg, LL2);
    HWND RIEPILO =  GetDlgItem(hwndDlg, RRIEPILO);
    HWND RIEPRIS =  GetDlgItem(hwndDlg, RRIEPRIS);

    //Prima calcolo quanti oggetti ci sono
    numitem=ListBox_GetCount(Target);
    for(i=1;i<numitem;i++)
    {
        //Estraggo una stringa alla volta e la inserisco ordinata
        //(La prima stringa non c'e' da estrarla)
        ListBox_GetText(Target,i,stringa);
        ListBox_DeleteString(Target,i);
        sscanf(stringa,"%lf",&importo);
        scrivilog(1,stringa);
        for(j=0;j<i;j++)
        {
            ListBox_GetText(Target,j,stringa1);
            scrivilog(1,stringa1);
            sscanf(stringa1,"%lf",&importo_f);
            if((cresc && (importo<importo_f))||((!cresc) && (importo>importo_f)))
                break;
        }
        ListBox_InsertString(Target,j,stringa);
        ListBox_GetText(RIEPILO,i,stringa);
        ListBox_DeleteString(RIEPILO,i);
        ListBox_InsertString(RIEPILO,j,stringa);
        ListBox_GetText(RIEPRIS,i,stringa);
        ListBox_DeleteString(RIEPRIS,i);
        ListBox_InsertString(RIEPRIS,j,stringa);
        ListBox_GetText(L1,i,stringa);
        ListBox_DeleteString(L1,i);
        ListBox_InsertString(L1,j,stringa);
        ListBox_GetText(L2,i,stringa);
        ListBox_DeleteString(L2,i);
        ListBox_InsertString(L2,j,stringa);
    }
}

void ordinalet(HWND hwndDlg,int TTarget,int LL1,int RRIEP1,int RRIEP2,int RRIEP3,char cresc)
//Questa funzione ordina le listbox contenenti testo in
//ordine decrescente (cresc=0) o crescente (cresc=1)
//sincronizzando allo stesso tempo le altre listbox
{
    char stringa[400],stringa1[401];
    int i,j,numitem;

    HWND Target =  GetDlgItem(hwndDlg, TTarget);
    HWND L1 =  GetDlgItem(hwndDlg, LL1);
    HWND RIEP1 =  GetDlgItem(hwndDlg, RRIEP1);
    HWND RIEP2 =  GetDlgItem(hwndDlg, RRIEP2);
    HWND RIEP3 =  GetDlgItem(hwndDlg, RRIEP3);

    //Prima calcolo quanti oggetti ci sono
    numitem=ListBox_GetCount(Target);
    for(i=1;i<numitem;i++)
    {
        //Estraggo una stringa alla volta e la inserisco ordinata
        //(La prima stringa non c'e' da estrarla)
        ListBox_GetText(Target,i,stringa);
        ListBox_DeleteString(Target,i);
        for(j=0;j<i;j++)
        {
            ListBox_GetText(Target,j,stringa1);
            if((cresc && (strcmp(stringa,stringa1)<0))||((!cresc) && (strcmp(stringa,stringa1)>0)))
            break;
        }
        ListBox_InsertString(Target,j,stringa);
        ListBox_GetText(L1,i,stringa);
        ListBox_DeleteString(L1,i);
        ListBox_InsertString(L1,j,stringa);
        ListBox_GetText(RIEP1,i,stringa);
        ListBox_DeleteString(RIEP1,i);
        ListBox_InsertString(RIEP1,j,stringa);
        ListBox_GetText(RIEP2,i,stringa);
        ListBox_DeleteString(RIEP2,i);
        ListBox_InsertString(RIEP2,j,stringa);
        ListBox_GetText(RIEP3,i,stringa);
        ListBox_DeleteString(RIEP3,i);
        ListBox_InsertString(RIEP3,j,stringa);

    }
}

void HandPulisci(HWND hwndDlg)
{
    HWND RIEPILO =  GetDlgItem(hwndDlg, C_RIEPILOG);
    HWND RIEP1 =  GetDlgItem(hwndDlg, C_RIEPILOG1);
    HWND RIEP2 =  GetDlgItem(hwndDlg, C_RIEPILOG2);
    HWND RIEP3 =  GetDlgItem(hwndDlg, C_RIEPILOG3);
    HWND RIEPRIS =  GetDlgItem(hwndDlg, C_RIEPRIS);
    HWND WAIT =  GetDlgItem(hwndDlg, C_ASPETTA);

    scrivilog(1,(char*)"TCompuDia  HandPulisci");
    ListBox_ResetContent(RIEPILO);
    ListBox_ResetContent(RIEP1);
    ListBox_ResetContent(RIEP2);
    ListBox_ResetContent(RIEP3);
    ListBox_ResetContent(RIEPRIS);
    Edit_SetText(WAIT,"");
}

void HandRiepilo(HWND hwndDlg,WPARAM wParam)
{
    int k;

    HWND RIEPILO =  GetDlgItem(hwndDlg, C_RIEPILOG);
    HWND RIEP1 =  GetDlgItem(hwndDlg, C_RIEPILOG1);
    HWND RIEP2 =  GetDlgItem(hwndDlg, C_RIEPILOG2);
    HWND RIEP3 =  GetDlgItem(hwndDlg, C_RIEPILOG3);
    HWND RIEPRIS =  GetDlgItem(hwndDlg, C_RIEPRIS);

    if(HIWORD(wParam)==LBN_SELCHANGE)
    {
        k=ListBox_GetCurSel(RIEPILO);
        ListBox_SetCurSel(RIEP1,k);
        ListBox_SetCurSel(RIEP2,k);
        ListBox_SetCurSel(RIEP3,k);
        ListBox_SetCurSel(RIEPRIS,k);
    }
}

void HandRiep1(HWND hwndDlg,WPARAM wParam)
{
    int k;

    HWND RIEPILO =  GetDlgItem(hwndDlg, C_RIEPILOG);
    HWND RIEP1 =  GetDlgItem(hwndDlg, C_RIEPILOG1);
    HWND RIEP2 =  GetDlgItem(hwndDlg, C_RIEPILOG2);
    HWND RIEP3 =  GetDlgItem(hwndDlg, C_RIEPILOG3);
    HWND RIEPRIS =  GetDlgItem(hwndDlg, C_RIEPRIS);

    if(HIWORD(wParam)==LBN_SELCHANGE)
    {
        k=ListBox_GetCurSel(RIEP1);
        ListBox_SetCurSel(RIEPILO,k);
        ListBox_SetCurSel(RIEP2,k);
        ListBox_SetCurSel(RIEP3,k);
        ListBox_SetCurSel(RIEPRIS,k);
    }
}

void HandRiep2(HWND hwndDlg,WPARAM wParam)
{
    int k;

    HWND RIEPILO =  GetDlgItem(hwndDlg, C_RIEPILOG);
    HWND RIEP1 =  GetDlgItem(hwndDlg, C_RIEPILOG1);
    HWND RIEP2 =  GetDlgItem(hwndDlg, C_RIEPILOG2);
    HWND RIEP3 =  GetDlgItem(hwndDlg, C_RIEPILOG3);
    HWND RIEPRIS =  GetDlgItem(hwndDlg, C_RIEPRIS);

    if(HIWORD(wParam)==LBN_SELCHANGE)
    {
        k=ListBox_GetCurSel(RIEP2);
        ListBox_SetCurSel(RIEPILO,k);
        ListBox_SetCurSel(RIEP1,k);
        ListBox_SetCurSel(RIEP3,k);
        ListBox_SetCurSel(RIEPRIS,k);
    }
}

void HandRiep3(HWND hwndDlg,WPARAM wParam,UINT uMsg)
{
    int k;

    HWND RIEPILO =  GetDlgItem(hwndDlg, C_RIEPILOG);
    HWND RIEP1 =  GetDlgItem(hwndDlg, C_RIEPILOG1);
    HWND RIEP2 =  GetDlgItem(hwndDlg, C_RIEPILOG2);
    HWND RIEP3 =  GetDlgItem(hwndDlg, C_RIEPILOG3);
    HWND RIEPRIS =  GetDlgItem(hwndDlg, C_RIEPRIS);

//Tutte le righe sotto sono esperimenti falliti per gestire lo scrollbar comune,
//cosa che mi riusciva con le classi Borland ma non qui.
//Per ora ho semplicemente eliminato lo scrollbar dalla listbox RIEP3
/*    int posy;
    SCROLLINFO info,info1;
    RECT clRect, rcInvalidate;
    info.cbSize=sizeof(SCROLLINFO);
    info.fMask=SIF_ALL;
    info1.cbSize=sizeof(SCROLLINFO);
    info1.fMask=SIF_ALL;

    GetClientRect(RIEP1, &clRect);
    rcInvalidate = clRect;

//    posy=GetScrollPos(RIEP3,SB_VERT);
//    GetScrollInfo(RIEP3,SB_VERT,&info);
//    GetScrollInfo(RIEP1,SB_VERT,&info1);
                    sprintf(stringa,"posy: %d",posy);
                MessageBox(hwndDlg,
                stringa,"Backup non verra' eseguito",
                MB_ICONEXCLAMATION | MB_OK);
//    SetScrollPos(RIEP3,SB_VERT,posy+2,TRUE);
//    ScrollWindowEx(RIEP1, 0, info.nPos-info1.nPos, NULL, NULL, NULL, &rcInvalidate, SW_INVALIDATE);

//    SetScrollInfo(RIEP1,SB_VERT,&info,TRUE);

    SetScrollPos(RIEP1,SB_VERT,posy,TRUE);
    SetScrollPos(RIEP2,SB_VERT,posy,TRUE);
    SetScrollPos(RIEPILO,SB_VERT,posy,TRUE);
    SetScrollPos(RIEPRIS,SB_VERT,posy,TRUE);
//    posy=SendMessage(RIEP1,WM_VSCROLL,SB_THUMBPOSITION,SB_LINEDOWN);
//    sprintf(stringa,"retrun= %d",posy);
//                MessageBox(hwndDlg,stringa,
//        "Problema!",MB_ICONEXCLAMATION | MB_OK);
    SendMessage(RIEP2,WM_VSCROLL,SB_THUMBPOSITION,posy);
    SendMessage(RIEPILO,WM_VSCROLL,SB_THUMBPOSITION,posy);
    SendMessage(RIEPRIS,WM_VSCROLL,SB_THUMBPOSITION,posy);

    posy=ListBox_GetTopIndex(RIEP3);
    ListBox_SetTopIndex(RIEPILO,posy);
    ListBox_SetTopIndex(RIEP1,posy);
    ListBox_SetTopIndex(RIEP2,posy);
    ListBox_SetTopIndex(RIEPRIS,posy);*/

    if(HIWORD(wParam)==LBN_SELCHANGE)
    {
        k=ListBox_GetCurSel(RIEP3);
        ListBox_SetCurSel(RIEPILO,k);
        ListBox_SetCurSel(RIEP1,k);
        ListBox_SetCurSel(RIEP2,k);
        ListBox_SetCurSel(RIEPRIS,k);
    }
}

void HandRiepRis(HWND hwndDlg,WPARAM wParam)
{
    int k;

    HWND RIEPILO =  GetDlgItem(hwndDlg, C_RIEPILOG);
    HWND RIEP1 =  GetDlgItem(hwndDlg, C_RIEPILOG1);
    HWND RIEP2 =  GetDlgItem(hwndDlg, C_RIEPILOG2);
    HWND RIEP3 =  GetDlgItem(hwndDlg, C_RIEPILOG3);
    HWND RIEPRIS =  GetDlgItem(hwndDlg, C_RIEPRIS);

    if(HIWORD(wParam)==LBN_SELCHANGE)
    {
        k=ListBox_GetCurSel(RIEPRIS);
        ListBox_SetCurSel(RIEPILO,k);
        ListBox_SetCurSel(RIEP1,k);
        ListBox_SetCurSel(RIEP2,k);
        ListBox_SetCurSel(RIEP3,k);
    }
}

void HandOrdRis(HWND hwndDlg,WPARAM wParam)
{
    char stringa[100];

    HWND WAIT =  GetDlgItem(hwndDlg, C_ASPETTA);

    scrivilog(1,(char*)"TCompuDia  HandOrdRis");
    ordinalet(hwndDlg,C_RIEPRIS,C_RIEPILOG,C_RIEPILOG1,C_RIEPILOG2,C_RIEPILOG3,creris);
    sprintf(stringa,"HandOrdRis crerie2: %d",creris);
    scrivilog(1,stringa);
    if(!creris)
    {
        Static_SetText(WAIT,"Ordinato decrescente per risorse");
        creris=1;
    }
    else
    {
        Static_SetText(WAIT,"Ordinato crescente per risorse");
        creris=0;
    }
}

void HandOrdRiep(HWND hwndDlg,WPARAM wParam)
{
    char stringa[100];

    HWND WAIT =  GetDlgItem(hwndDlg, C_ASPETTA);

    scrivilog(1,(char*)"TCompuDia  HandOrdRiep");
    ordinalet(hwndDlg,C_RIEPILOG,C_RIEPRIS,C_RIEPILOG1,C_RIEPILOG2,C_RIEPILOG3,crerie);
    sprintf(stringa,"HandOrdRiep crerie: %d",crerie);
    scrivilog(1,stringa);
    if(!crerie)
    {
        Static_SetText(WAIT,"Ordinato decrescente per categorie");
        crerie=1;
    }
    else
    {
        Static_SetText(WAIT,"Ordinato crescente per categorie");
        crerie=0;
    }
}

void HandOrdRiep1(HWND hwndDlg,WPARAM wParam)
{
    char stringa[100];

    HWND WAIT =  GetDlgItem(hwndDlg, C_ASPETTA);

    scrivilog(1,(char*)"TCompuDia  HandOrdRiep1");
    ordinanum(hwndDlg,C_RIEPILOG1,C_RIEPILOG2,C_RIEPILOG3,C_RIEPILOG,C_RIEPRIS,crerie1);
    sprintf(stringa,"HandOrdRiep1 crerie1: %d",crerie1);
    scrivilog(1,stringa);
    if(!crerie1)
    {
        Static_SetText(WAIT,"Ordinato decrescente per guadagno netto");
        crerie1=1;
    }
    else
    {
        Static_SetText(WAIT,"Ordinato crescente per guadagno netto");
        crerie1=0;
    }
}

void HandOrdRiep2(HWND hwndDlg,WPARAM wParam)
{
    char stringa[100];

    HWND WAIT =  GetDlgItem(hwndDlg, C_ASPETTA);

    scrivilog(1,(char*)"TCompuDia  HandOrdRiep2");
    ordinanum(hwndDlg,C_RIEPILOG2,C_RIEPILOG1,C_RIEPILOG3,C_RIEPILOG,C_RIEPRIS,crerie2);
    sprintf(stringa,"HandOrdRiep2 crerie2: %d",crerie2);
    scrivilog(1,stringa);
    if(!crerie2)
    {
        Static_SetText(WAIT,"Ordinato decrescente per entrate");
        crerie2=1;
    }
    else
    {
        Static_SetText(WAIT,"Ordinato crescente per entrate");
        crerie2=0;
    }
}

void HandOrdRiep3(HWND hwndDlg,WPARAM wParam)
{
    char stringa[100];

    HWND WAIT =  GetDlgItem(hwndDlg, C_ASPETTA);

    scrivilog(1,(char*)"TCompuDia  HandOrdRiep3");
    ordinanum(hwndDlg,C_RIEPILOG3,C_RIEPILOG1,C_RIEPILOG2,C_RIEPILOG,C_RIEPRIS,crerie3);
    sprintf(stringa,"HandOrdRiep3 crerie3: %d",crerie3);
    scrivilog(1,stringa);
    if(!crerie3)
    {
        Static_SetText(WAIT,"Ordinato decrescente per uscite");
        crerie3=1;
    }
    else
    {
        Static_SetText(WAIT,"Ordinato crescente per uscite");
        crerie3=0;
    }
}

void cmp_HandCancella(HWND hwndDlg)
//Cancella la voce selezionata nel box di riepilogo
{
    int index;

    HWND RIEPILO =  GetDlgItem(hwndDlg, C_RIEPILOG);
    HWND RIEP1 =  GetDlgItem(hwndDlg, C_RIEPILOG1);
    HWND RIEP2 =  GetDlgItem(hwndDlg, C_RIEPILOG2);
    HWND RIEP3 =  GetDlgItem(hwndDlg, C_RIEPILOG3);
    HWND RIEPRIS =  GetDlgItem(hwndDlg, C_RIEPRIS);

    scrivilog(1,(char*)"TCompuDia  HandCancella");
    index=ListBox_GetCurSel(RIEPILO);
    if(index<0)
    {
        //Non era selezionato nulla: ritorno
        scrivilog(1,(char*)"TCompuDia HandCancella: nulla ritorno");
        return;
    }
    ListBox_DeleteString(RIEPILO,index);
    ListBox_DeleteString(RIEP1,index);
    ListBox_DeleteString(RIEP2,index);
    ListBox_DeleteString(RIEP3,index);
    ListBox_DeleteString(RIEPRIS,index);
}

