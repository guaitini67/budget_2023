#include "dlgday.h"

/*
Questo file contiene tutta la gestione relativa alla finestra di dialogo "DLGD_AY" (Gestione conti del giorno)
*/

extern const char *gset[];


extern HINSTANCE hInst;

extern char vis,entra,stringa[];

extern int errore,tab[],g,m,a,nlnote,numfonti,month[];

extern long int numg_out,numg_riepilogo;


extern info_risorse t_risorse1[];
extern info_movimenti tabella_movimenti[];



extern time_t curtime;
extern struct tm *data_ora;

extern FILE *fil,*tmp;



BOOL CALLBACK DlgDay(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    char stringa[401],nomeconv[27];
    int k;
    double importo;

    BOOL bRet, fSavedEnabledState;

    HWND hDlg;

    MSG msg;

    HWND RiGior =  GetDlgItem(hwndDlg, ID_GG);
    HWND RiMes =  GetDlgItem(hwndDlg, ID_MM);
    HWND RiAnn =  GetDlgItem(hwndDlg, ID_AAAA);
    HWND UGior =  GetDlgItem(hwndDlg, ID_GUSC);
    HWND UMes =  GetDlgItem(hwndDlg, ID_MUSC);
    HWND UAnn =  GetDlgItem(hwndDlg, ID_AUSC);
    HWND SELBANC =  GetDlgItem(hwndDlg, SELBANCA);
    HWND SELDAT =  GetDlgItem(hwndDlg, SELDATO);
    HWND DESTINA =  GetDlgItem(hwndDlg, C_BANCA_IN);
    HWND PROVEN =  GetDlgItem(hwndDlg, C_BANCA_OUT);
    HWND RIEPILO =  GetDlgItem(hwndDlg, C_RIEPILOG);
    HWND IMPO =  GetDlgItem(hwndDlg, IMPORTO);
    HWND RESULT =  GetDlgItem(hwndDlg, MOSTRADATO);
    HWND FAIVED =  GetDlgItem(hwndDlg, ID_FAIVED);
    HWND BOK =  GetDlgItem(hwndDlg, OG_OK);

    switch(uMsg)
    {
        case WM_INITDIALOG:
        {
            //le prossime 3 righe caricano l'icona nella window caption della finestra e ALT+TAB (non ho ben capito quest'ultima)
            HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(icona));
            SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
            SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
            vis=1; //siccome la finestra non e' modale con vis=1 impedisco alla finestra principale di aprirne altre
            SetTimer(hwndDlg,    // handle to main window
            ID_TIMER,            // timer identifier
            1000,                // 1-second interval
            (TIMERPROC) NULL);   // no timer callback
            // punti di stop per i tabulatori nella
            //finestra di riepilogo
            tab[0]=100;
            tab[1]=150;
            tab[2]=200;
            // messaggio che imposta i tabulatori
            SendMessage(RIEPILO,LB_SETTABSTOPS,3,(LPARAM)tab);

            // ora nel gruppo dei radiobutton entrata uscita seleziono uscita (Triste verita'!)
            CheckRadioButton(hwndDlg,PULSENTR,PULSUSC,PULSUSC);
            //nlnote=numero righe di note. La metto a zero all'inizio
            //(e' necessario?)
            nlnote=0;
            //Inizializzo la data di uscita e quella di riepilogo
            //al valore arrivato dal costruttore della classe
            sprintf(stringa,"%02d",g);
            ComboBox_SetText(RiGior,stringa);
            ComboBox_SetText(UGior,stringa);
            sprintf(stringa,"%02d",m);
            ComboBox_SetText(RiMes,stringa);
            ComboBox_SetText(UMes,stringa);
            sprintf(stringa,"%04d",a);
            ComboBox_SetText(RiAnn,stringa);
            ComboBox_SetText(UAnn,stringa);

            numg_out=(long int)a*10000+m*100+g;
            entra=0;

            //caricavoci mette in una struttura alcune info
            //importanti di basedata.dat
            if(caricaVoci(hwndDlg)<0)
            {
                //C'e' stato un problema nella lettura di basedata.dat
                MessageBox(hwndDlg,"Anomalia nel file 'basedata.dat'",
                "Problema!",MB_ICONEXCLAMATION | MB_OK);
                break;
            }
            //Carico una lista per 'esce da'
            //compatibile col giorno corrente
            caricaLista(hwndDlg,ID_GUSC,ID_MUSC,ID_AUSC,C_BANCA_OUT);
            //Carico una lista per 'entra in'
            //compatibile col giorno corrente
            //Notare che uso comunque la data di uscita
            //perche' non ho ancora quella di entrata
            caricaLista(hwndDlg,ID_GUSC,ID_MUSC,ID_AUSC,C_BANCA_IN);
            //Carico una lista per box riepilogo
            //compatibile col giorno corrente
            caricaLista(hwndDlg,ID_GG,ID_MM,ID_AAAA,SELBANCA);
            //Inserisco la voce per selezionare i dati globali
            ComboBox_SetText(SELBANC,"Globale");
            //Di default, carico il campo 'esce da' e riepilogo
            //con la prima voce (che nelle mie idee e' 'contanti')
            ComboBox_GetLBText(PROVEN,0,stringa);
            ComboBox_SetText(PROVEN,stringa);
            ComboBox_SetText(SELBANC,stringa);

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
            ComboBox_InsertString(SELDAT,-1,"Uscite di oggu");
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

            caricacategorie(hwndDlg);
            riepiloga(hwndDlg,numg_out);
            scrivilog(0,(char*)"");
            scrivilog(1,(char*)"*****");
            scrivilog(0,(char*)"setup oggi eseguito");
            scrivilog(0,(char*)"*****");


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
            remove("note.tmp");
            //Questa funzione interrompe il thread del messaggio
            //Immagino che permette di uscire dal message loop (ciclo while) attivato alla chiamata
            //dalla finestra genitrice
            PostQuitMessage(0);
        }
        return TRUE;

        case WM_TIMER:
        {
            time(&curtime);
            data_ora = localtime( &curtime );
            sprintf(stringa,"%02d : %02d : %02d",data_ora->tm_hour,data_ora->tm_min,data_ora->tm_sec);
            SetDlgItemText(hwndDlg,CLOCK,stringa);
            sprintf(stringa,"%02d / %02d / %04d",data_ora->tm_mday,data_ora->tm_mon+1,data_ora->tm_year+1900);
            SetDlgItemText(hwndDlg,DATA,stringa);
        }
        return TRUE;

        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case ID_GEN:
                    DHandValGen(hwndDlg,wParam);
                    break;

                case ID_MEN:
                    DHandValMen(hwndDlg,wParam);
                    break;

                case ID_AEN:
                    DHandValAen(hwndDlg,wParam);
                    break;

                case ID_GUSC:
                    DHandValGusc(hwndDlg,wParam);
                    break;

                case ID_MUSC:
                    DHandValMusc(hwndDlg,wParam);
                    break;

                case ID_AUSC:
                    DHandValAusc(hwndDlg,wParam);
                    break;

                case ID_GG:
                    DHandValRiG(hwndDlg,wParam);
                    break;

                case ID_MM:
                    DHandValRiM(hwndDlg,wParam);
                    break;

                case ANNUL:
                    HandAnnulla(hwndDlg);
                    break;

                case ID_AAAA:
                    DHandValRiA(hwndDlg,wParam);
                    break;

                case IMPORTO:
                    break;

                case PULSENTR:
                    if(HIWORD(wParam)==BN_CLICKED)
                    {
                        scrivilog(1,(char*)"Mouse chkentrata");
                        CheckEntrata(hwndDlg);
                        SetFocus(IMPO);
                    }
                    break;

                case PULSUSC:
                    if(HIWORD(wParam)==BN_CLICKED)
                    {
                        scrivilog(1,(char*)"Mouse chkentrata");
                        CheckUscita(hwndDlg);
                        SetFocus(IMPO);
                    }
                    break;

                case SCRIVI:
                    HandScrivi(hwndDlg);
                    break;

                case OG_OK:
                    scrivilog(1,(char*)"OK spinto");
                    vis=0; //rimetto vis=0 e chiudo: potro' riaprire questa finestra da quella principale
                    //siccome l'ho aperta non modale devo usare DestryWindow e non EndDialog
                    DestroyWindow( hwndDlg );
                    break;

                case AVANTI:
                    HandGDopo(hwndDlg);
                    break;

                case DIETRO:
                    HandGPrima(hwndDlg);
                    break;

                case ID_NONFARVBUT:
                    scrivilog(1,(char*)"Mouse HandNonVed");
                    nonved(hwndDlg);
                    break;

                case OG_VAI:
                    HandVai(hwndDlg);
                    break;

                case ID_FAIVBUT:
                    scrivilog(1,(char*)"Mouse HandFarVed");
                    ved(hwndDlg);
                    break;

                case ID_SOLNFARVBUT:
                    scrivilog(1,(char*)"Mouse HandSoloNonVed");
                    solononved(hwndDlg);
                    break;

                case ID_SOLFAIVBUT:
                    scrivilog(1,(char*)"Mouse HandSoloVed");
                    soloved(hwndDlg);
                    break;

                case ID_TUTTEVIS:
                    scrivilog(1,(char*)"Mouse HandTutteVis");
                    tuttevis(hwndDlg);
                    break;

                case SELBANCA:
                    if(HIWORD(wParam)==CBN_SELENDOK)
                    {
                        mostrasaldi(hwndDlg,SELBANCA,SELDATO,MOSTRADATO,t_risorse1);
                    }
                    break;

                case SELDATO:
                    if(HIWORD(wParam)==CBN_SELENDOK)
                    {
                     	scrivilog(1,(char*)"HandSelTipoDato");
                        mostrasaldi(hwndDlg,SELBANCA,SELDATO,MOSTRADATO,t_risorse1);
                    }
                    break;

                case C_RIEPILOG:
                    if(HIWORD(wParam)==LBN_DBLCLK)
                    {
                        //Se clicco due volte su un elemento del riepilogo:
                        //carica i campi con i dati relativi a quel movimento
                        scrivilog(1,(char*)"Mouse seleziona 2 click");
                        SimpleSel(hwndDlg);
                    }
                    break;

                case OG_CANC:
                    HandCancella(hwndDlg);
                    break;

                case ACC_CTK:
                    //Se digito CTRL+K, mi equivale a premere CALCOLI
                    //C'e' anche l'effetto speciale del bottone premuto
                    //(faccio un setfocus in piu' per creare ritardo)
                    //NOTA 06-01-2023: l'ho lasciato perche' c'era ma non mi sembra si veda niente di speciale
                    scrivilog(1,(char*)"CTRL-K");
                    SendDlgItemMessage(hwndDlg,ID_CERCA, BM_SETSTATE, TRUE, 0);
                    SetFocus(SELBANC);
                    SetFocus(IMPO);
                    SendDlgItemMessage(hwndDlg,ID_CERCA, BM_SETSTATE, FALSE, 0);
                case ID_CERCA:
                    {
                        hDlg=CreateDialog(hInst, MAKEINTRESOURCE(COMPUTE), hwndDlg, (DLGPROC)DlgCompute);
                        scrivilog(1,(char*)"DlgDay  HandCalcola dopo");
                        //carico gli acceleratori da far usare al dialogo figlio
                        HACCEL hAccel = LoadAccelerators(hInst, MAKEINTRESOURCE(SCORCIATOIE));
                        //per avere un effetto modale mi salvo lo stato corrente di questa finestra
                        //e poi la disabilito
                        bRet = 0;
                        fSavedEnabledState = IsWindowEnabled(hwndDlg);
                        EnableWindow(hwndDlg, FALSE);  // disable parent window, as dialog window is modal

                        while ((bRet = GetMessage(&msg, 0, 0, 0)))
                        {
                            //intercetto il messaggio
                            if ( -1 == bRet )
                            {
                                //in caso di errore esco dal loop
                                break;
                            }
                            if ( !TranslateAccelerator(hDlg, hAccel, &msg) )
                            {
                                //se il messaggio alla finestra figlia non corrisponde a un acceleratore
                                //controllo se e' un messaggio per la finestra dialogo figlia
                                //IsDialogMessage e' una funzione che determina se il messaggio e' per
                                //la finestra di dialogo specificata e nel caso glielo fa avere
                                if ( !IsDialogMessage(hDlg, &msg) )
                                {
                                    //se il messaggio NON e' per la finestra di dialogo figlia
                                    //riprendo la gestione di default del messaggio
                                    //cioe' traduco e trasmetto
                                    TranslateMessage(&msg);
                                    DispatchMessage(&msg);
                                }
                            }
                        }
                        //Chiusa la finestra di dialogo figlia in qualche modo esco dal while (tramite PostQuitMessage sulla figlia?)
                        //Riabilito la finestra corrente
                        EnableWindow(hwndDlg, fSavedEnabledState);  // enable parent window. Dialog was closed
                        //Ora per qualche motivo mi tocca minimizzare e poi rimostrare la finestra
                        //altrimenti mi rimaneva in bachground e dovevo cliccare sulla barra strumenti
                        //1) Minimizzo
                        ShowWindow(hwndDlg,2);
                        //2) Visualizzo
                        ShowWindow(hwndDlg,9);
                    }
                    break;

                case OG_NOTE:
                    HandNote(hwndDlg);
                    break;

                case C_BANCA_IN:
                    break;

                case C_BANCA_OUT:
                    break;

                case ACC_CTA:
                    //Se digito CTRL+A, mi equivale a premere ANNULLA
                    //C'e' anche l'effetto speciale del bottone premuto
                    //(faccio un setfocus in piu' per creare ritardo)
                    //NOTA 06-01-2023: l'ho lasciato perche' c'era ma non mi sembra si veda niente di speciale
                    scrivilog(1,(char*)"CTRL-A");
                    SendDlgItemMessage(hwndDlg,ANNUL, BM_SETSTATE, TRUE, 0);
                    SetFocus(SELBANC);
                    SetFocus(IMPO);
                    SendDlgItemMessage(hwndDlg,ANNUL, BM_SETSTATE, FALSE, 0);
                    HandAnnulla(hwndDlg);
                    break;

                case ACC_CTD:
                    //Se digito CTRL+D, mi posiziona su IMPORTO
                    scrivilog(1,(char*)"CTRL-D");
                    SetFocus(IMPO);
                    break;

                case ACC_CTE:
                    //Se digito CTRL+E, mi seleziona Entrata
                    //e mi posiziona su IMPORTO
                    scrivilog(1,(char*)"CTRL-E");
                    CheckEntrata(hwndDlg);
                    SetFocus(IMPO);
                    break;

                case ACC_CTR:
                    //Se digito CTRL+R, mi seleziona Uscita
                    //e mi posiziona su IMPORTO
                    scrivilog(1,(char*)"CTRL-R");
                    CheckUscita(hwndDlg);
                    SetFocus(IMPO);
                    break;

                case ACC_CTF:
                    //Se digito CTRL+F, mi posiziona sulla lista movimenti
                    //e seleziona la prima voce se c'e'
                    scrivilog(1,(char*)"CTRL-F");
                    SetFocus(RIEPILO);
                    ListBox_SetCurSel(RIEPILO,0);
                    break;

                case ACC_CTG:
                    //Se digito CTRL+G, mi posiziona su data di riepilogo
                    scrivilog(1,(char*)"CTRL-G");
                    SetFocus(RiGior);
                    break;

                case ACC_CTN:
                    //Se digito CTRL+N, mi equivale a premere NOTE
                    //C'e' anche l'effetto speciale del bottone premuto
                    //(faccio un setfocus in piu' per creare ritardo)
                    //NOTA 06-01-2023: l'ho lasciato perche' c'era ma non mi sembra si veda niente di speciale
                    scrivilog(1,(char*)"CTRL-N");
                    SendDlgItemMessage(hwndDlg,OG_NOTE, BM_SETSTATE, TRUE, 0);
                    SetFocus(SELBANC);
                    SetFocus(IMPO);
                    SendDlgItemMessage(hwndDlg,OG_NOTE, BM_SETSTATE, FALSE, 0);
                    HandNote(hwndDlg);
                    break;

                case ACC_CTQ:
                    //Se digito CTRL+Q, mi posiziona su ENTRA IN
                    scrivilog(1,(char*)"CTRL-Q");
                    SetFocus(DESTINA);
                    break;

                case ACC_CTS:
                    //Se digito CTRL+S, mi equivale a premere SCRIVI
                    //Metto il focus su importo per toglierlo
                    //ai campi date e impedire date errate
                    //C'e' anche l'effetto speciale del bottone premuto
                    //(faccio un stfocus in piu' per creare ritardo)
                    //NOTA 06-01-2023: l'ho lasciato perche' c'era ma non mi sembra si veda niente di speciale
                    scrivilog(1,(char*)"CTRL-S");
                    SendDlgItemMessage(hwndDlg,SCRIVI, BM_SETSTATE, TRUE, 0);
                    SetFocus(SELBANC);
                    SetFocus(IMPO);
                    SendDlgItemMessage(hwndDlg,SCRIVI, BM_SETSTATE, FALSE, 0);
                    HandScrivi(hwndDlg);
                    break;

                case ID_FAIVED:
                    break;

                case ACC_CTT:
                    //Se digito CTRL+T, mi posiziona sulla lista visibilita'
                    scrivilog(1,(char*)"CTRL-T");
                    SetFocus(FAIVED);
                    break;

                case ACC_CTW:
                    //Se digito CTRL+W, mi posiziona su ESCE DA
                    scrivilog(1,(char*)"CTRL-W");
                    SetFocus(PROVEN);
                    break;

                case ACC_CTDOWN:
                    //Se digito CTRL+freccia giu', mi visualizza il giorno dopo
                    //Metto il focus su SELDAT per toglierlo
                    //ai campi date e impedire date errate
                    //(inoltre e' anche una cosa desiderabile)
                    //C'e' anche l'effetto speciale del bottone premuto
                    //(faccio un stfocus in piu' per creare ritardo)
                    //NOTA 06-01-2023: l'ho lasciato perche' c'era ma non mi sembra si veda niente di speciale
                    scrivilog(1,(char*)"CTRL-DWN");
                    SendDlgItemMessage(hwndDlg,AVANTI, BM_SETSTATE, TRUE, 0);
                    SetFocus(SELBANC);
                    SetFocus(SELDAT);
                    SendDlgItemMessage(hwndDlg,AVANTI, BM_SETSTATE, FALSE, 0);
                    HandGDopo(hwndDlg);
                    break;

                case ACC_CTUP:
                    //Se digito CTRL+freccia su, mi visualizza il giorno prima
                    //Metto il focus su SELDAT per toglierlo
                    //ai campi date e impedire date errate
                    //(inoltre e' anche una cosa desiderabile)
                    //C'e' anche l'effetto speciale del bottone premuto
                    //(faccio un stfocus in piu' per creare ritardo)
                    scrivilog(1,(char*)"CTRL-UP");
                    SendDlgItemMessage(hwndDlg,DIETRO, BM_SETSTATE, TRUE, 0);
                    SetFocus(SELBANC);
                    SetFocus(SELDAT);
                    SendDlgItemMessage(hwndDlg,DIETRO, BM_SETSTATE, FALSE, 0);
                    HandGPrima(hwndDlg);
                    break;

                case ACC_CTNEXT:
                    //Se digito CTRL+page down, mi equivale a premere VAI
                    //Metto il focus su SELDAT per toglierlo
                    //ai campi date e impedire date errate
                    //(inoltre e' anche una cosa desiderabile)
                    scrivilog(1,(char*)"CTRL-PGDWN");
                    SendDlgItemMessage(hwndDlg,
                    OG_VAI, BM_SETSTATE, TRUE, 0);
                    SetFocus(SELBANC);
                    SetFocus(SELDAT);
                    SendDlgItemMessage(hwndDlg,
                    OG_VAI, BM_SETSTATE, FALSE, 0);
                    HandVai(hwndDlg);
                    break;

                case ACC_ESCAPE:
                    //Se spingo ESC, mi posiziona su OK
                    SetFocus(BOK);
                    break;

                case ACC_F1:
                    //Se spingo F1, mi calcola la conversione
                    //lira -> euro nel campo importo
                    scrivilog(1,(char*)"F1");
                    ComboBox_GetText(IMPO,stringa,27);
                    k=sscanf(stringa,"%lf%s",&importo,nomeconv);
                    if(k==1)
                    {
                        importo=importo/1936.27;
                        sprintf(stringa,"%.2f",importo);
                        ComboBox_SetText(IMPO,"");
                        ComboBox_SetText(IMPO,stringa);
                    }
                    break;

                case ACC_F2:
                    //Se spingo F2, mi calcola la conversione
                    //euro -> lira nel campo importo
                    scrivilog(1,(char*)"F2");
                    ComboBox_GetText(IMPO,stringa,27);
                    k=sscanf(stringa,"%lf%s",&importo,nomeconv);
                    if(k==1)
                    {
                        importo=importo*1936.27;
                        sprintf(stringa,"%.2f",importo);
                        ComboBox_SetText(IMPO,"");
                        ComboBox_SetText(IMPO,stringa);
                    }
                    break;

                case ACC_F3:
                    //Se spingo F3, mi calcola la conversione
                    //lira -> euro nel campo dati
                    scrivilog(1,(char*)"F3");
                    Static_GetText(RESULT,stringa,27);
                    k=sscanf(stringa,"%lf%s",&importo,nomeconv);
                    if(k==1)
                    {
                        importo=importo/1936.27;
                        sprintf(stringa,"%.2f",importo);
                        Static_SetText(RESULT,stringa);
                    }
                    break;

                case ACC_F4:
                    //Se spingo F4, mi calcola la conversione
                    //euro -> lira nel campo dati
                    scrivilog(1,(char*)"F4");
                    Static_GetText(RESULT,stringa,27);
                    k=sscanf(stringa,"%lf%s",&importo,nomeconv);
                    if(k==1)
                    {
                        importo=importo*1936.27;
                        sprintf(stringa,"%.0f",importo);
                        Static_SetText(RESULT,stringa);
                    }
                    break;

                case ACC_F5:
                    //Se spingo F5, equivale a spingere
                    //"selezionate non visibili" (con effetto bottone)
                    scrivilog(1,(char*)"F5");
                    SendDlgItemMessage(hwndDlg,
                    ID_NONFARVBUT, BM_SETSTATE, TRUE, 0);
                    SetFocus(SELBANC);
                    SetFocus(IMPO);
                    SendDlgItemMessage(hwndDlg,
                    ID_NONFARVBUT, BM_SETSTATE, FALSE, 0);
                    nonved(hwndDlg);
                    break;

                case ACC_F6:
                    //Se spingo F6, equivale a spingere
                    //"solo selezionate non visibili" (con effetto bottone)
                    scrivilog(1,(char*)"F6");
                    SendDlgItemMessage(hwndDlg,
                    ID_SOLNFARVBUT, BM_SETSTATE, TRUE, 0);
                    SetFocus(SELBANC);
                    SetFocus(IMPO);
                    SendDlgItemMessage(hwndDlg,
                    ID_SOLNFARVBUT, BM_SETSTATE, FALSE, 0);
                    solononved(hwndDlg);
                    break;

                case ACC_F7:
                    //Se spingo F7, equivale a spingere
                    //"selezionate visibili" (con effetto bottone)
                    scrivilog(1,(char*)"F7");
                    SendDlgItemMessage(hwndDlg,
                    ID_FAIVBUT, BM_SETSTATE, TRUE, 0);
                    SetFocus(SELBANC);
                    SetFocus(IMPO);
                    SendDlgItemMessage(hwndDlg,
                    ID_FAIVBUT, BM_SETSTATE, FALSE, 0);
                    ved(hwndDlg);
                    break;

                case ACC_F8:
                    //Se spingo F8, equivale a spingere
                    //"solo selezionate visibili" (con effetto bottone)
                    scrivilog(1,(char*)"F8");
                    SendDlgItemMessage(hwndDlg,
                    ID_SOLFAIVBUT, BM_SETSTATE, TRUE, 0);
                    SetFocus(SELBANC);
                    SetFocus(IMPO);
                    SendDlgItemMessage(hwndDlg,
                    ID_SOLFAIVBUT, BM_SETSTATE, FALSE, 0);
                    soloved(hwndDlg);
                    break;

                case ACC_F9:
                    //Se spingo F9, equivale a spingere
                    //"tutte visibili" (con effetto bottone)
                    scrivilog(1,(char*)"F9");
                    SendDlgItemMessage(hwndDlg,
                    ID_TUTTEVIS, BM_SETSTATE, TRUE, 0);
                    SetFocus(SELBANC);
                    SetFocus(IMPO);
                    SendDlgItemMessage(hwndDlg,
                    ID_TUTTEVIS, BM_SETSTATE, FALSE, 0);
                    tuttevis(hwndDlg);
                    break;

                case ACC_CTDELETE:
                    //Se digito CTRL+canc, mi equivale a premere CANC
                    //C'e' anche l'effetto speciale del bottone premuto
                    //(faccio un setfocus in piu' per creare ritardo)
                    scrivilog(1,(char*)"CTRL-DEL");
                    SendDlgItemMessage(hwndDlg,
                    OG_CANC, BM_SETSTATE, TRUE, 0);
                    SetFocus(SELBANC);
                    SetFocus(IMPO);
                    SendDlgItemMessage(hwndDlg,
                    OG_CANC, BM_SETSTATE, FALSE, 0);
                    HandCancella(hwndDlg);
                    break;

                case ACC_CTRETURN:
                    //Se digito CTRL+enter, mi equivale a cliccare due volte
                    //in una voce di riepilogo
                    //Alla fine mi posiziona su IMPORTO
                    scrivilog(1,(char*)"CTRL-RET");
                    k=ListBox_GetCurSel(RIEPILO);
                    ListBox_GetText(RIEPILO,k,stringa);
                    if(ListBox_GetText(RIEPILO,k,stringa)>1)
                        SimpleSel(hwndDlg);
                    SetFocus(IMPO);
                    break;

                case ACC_CTALTRET:
                    //Se digito CTRL+alt+enter, mi equivale a cliccare due volte
                    //in una voce di riepilogo, selezionare di nuovo e cancellare
                    //E' utile per sveltire le sostituzioni
                    //Alla fine mi posiziona su IMPORTO
                    scrivilog(1,(char*)"CTRL-ALT-RET");
                    k=ListBox_GetCurSel(RIEPILO);
                    ListBox_GetText(RIEPILO,k,stringa);
                    if(ListBox_GetText(RIEPILO,k,stringa)>1)
                        ComplexSel(hwndDlg);
                    SetFocus(IMPO);
                    break;


            }
        }
        return TRUE;


    }
    return FALSE;
}

int caricaVoci(HWND hwndDlg)
{
    int i;

    HWND DESTINA =  GetDlgItem(hwndDlg, C_BANCA_IN);
    HWND PROVEN =  GetDlgItem(hwndDlg, C_BANCA_OUT);
    HWND SELBANC =  GetDlgItem(hwndDlg, SELBANCA);
    HWND FAIVED =  GetDlgItem(hwndDlg, ID_FAIVED);

    ComboBox_ResetContent(DESTINA);
    ComboBox_ResetContent(PROVEN);
    ComboBox_ResetContent(SELBANC);
    i=caricav(t_risorse1,&numfonti);
    if(i<0)
    //Se c'e' qualche problema da caricav
    {
        sprintf(stringa,"caricaRisorse Problema caricav %d",i);
        MessageBox(hwndDlg,stringa,
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        scrivilog(1,stringa);
        return -1;
    }
    //Faccio vedere tutte le risorse all'inizio
    for(i=0;i<numfonti;i++)
    {
        sprintf(stringa,"VIS:  %s",t_risorse1[i].nomeconv);
        scrivilog(1,stringa);
        ListBox_AddString(FAIVED,stringa);
    }
    return 0;
}

void caricaLista(HWND hwndDlg,int gio, int mes, int ann, int ris)
{
    HWND GGior =  GetDlgItem(hwndDlg, gio);
    HWND MMes =  GetDlgItem(hwndDlg, mes);
    HWND AAnn =  GetDlgItem(hwndDlg, ann);
    HWND RISORSA =  GetDlgItem(hwndDlg, ris);

    int a,m,g,j,i;
    long int num_g;
    char nomeconv[27];


    j=0;
    //Controllo se avevo scritto qualcosa
    //Se e' cosi', lo memorizzo
    if(ComboBox_GetText(RISORSA,nomeconv,27))
        j=1;
    ComboBox_ResetContent(RISORSA);
    ComboBox_GetText(GGior,stringa,3);
    g=atoi(stringa);
    ComboBox_GetText(MMes,stringa,3);
    m=atoi(stringa);
    ComboBox_GetText(AAnn,stringa,5);
    a=atoi(stringa);
    num_g=(long int)a*10000+m*100+g;
    for(i=0;i<numfonti;i++)
    {
        //Casi in cui aggiungero' la risorsa alla lista:
        if(//1: non ho specificato data di apertura e chiusura
            ((t_risorse1[i].aperto==-1)&&(t_risorse1[i].chiuso==-1))
            ||
            //2: ho specificato solo la data di chiusura, ma
            //questa e' posteriore al giorno che mi interessa
            ((t_risorse1[i].aperto==-1)&&(t_risorse1[i].chiuso>num_g))
            ||
            //3: ho specificato solo la data di apertura, ma
            //questa e' anteriore al giorno che mi interessa
            ((t_risorse1[i].aperto<num_g)&&(t_risorse1[i].chiuso==-1))
            ||
            //4: ho specificato entrambe le date, e
            //il giorno che mi interessa e'in mezzo
            ((t_risorse1[i].aperto<num_g)&&(t_risorse1[i].chiuso>num_g)))
        {
            ComboBox_AddString(RISORSA,t_risorse1[i].nomeconv);
        }
    }
    //Se c'era qualcosa scritto, lo rimetto dov'era
    //(ci pensera' HandScrivi a controllare
    //se e' una stringa corretta)
    if(j)
        ComboBox_SetText(RISORSA,nomeconv);
}

void caricacategorie(HWND hwndDlg)
//Carica la lista delle categorie con
//la lista memorizzata in categ.dat
{
    HWND CATE =  GetDlgItem(hwndDlg, CATEG);

    int e,i;
    FILE *tmp;
    char stringa[401],cat[100];

    if(!(tmp=fopen("categ.dat","r")))
    {
        //Non trovo il file, ne inizializzo
        //uno vuoto e ritorno
        tmp=fopen("categ.dat","w");
        fprintf(tmp,"0\n");
        fclose(tmp);
        return;
    }
    ComboBox_ResetContent(CATE);
    fgets(stringa,400,tmp);
    sscanf(stringa,"%d\n",&e);
    for(i=0;i<e;i++)
    {
        fgets(stringa,400,tmp);
        // nel vecchio programma lo sscanf era come mostrato qui sotto,
        // ma ora mi va in crash percio' ho eliminato la seconda serie di caratteri
        // che comunque non dovrebbero essere mai usati
        // sscanf(stringa,"%d,%[!-~ ΅-ÿ‘’]\n",&posiz,nomeconv);
        sscanf(stringa,"%[!-~ ‘’@ςΰωθιμ°η§]\n",cat);
        ComboBox_AddString(CATE,cat);
    }
    fclose(tmp);
}

int riepiloga(HWND hwndDlg, long int num_g)
//Scorre il file selezionato alla ricerca di num_g
//e quando lo trova (se lo trova) riempie il campo di
//riepilogo
//Inoltre scrive le note nel file "note.tmp"
{
    HWND RIEPILO =  GetDlgItem(hwndDlg, C_RIEPILOG);
    HWND RiGior =  GetDlgItem(hwndDlg, ID_GG);
    HWND RiMes =  GetDlgItem(hwndDlg, ID_MM);
    HWND RiAnn =  GetDlgItem(hwndDlg, ID_AAAA);

    int g,m,a,ii,erno,num_ops,i,j,k,nrighe,pa,pb,index;
    long int pippo,numg_fa,numg_fb;
    char nome_file[30],stringa[401],mot_f[100],cat_f[100];
    double importo_f;
    struct tm tcheck;

    ListBox_ResetContent(RIEPILO);
    //numg_riepilogo viene aggiornato solo
    //in questa funzione.
    //Mi viene comodo per la gestione delle note
    numg_riepilogo=num_g;
    //Individuo da num_g anno, mese, giorno
    g= num_g%100;
    pippo= num_g/100;
    m= pippo%100;
    a=pippo/100;

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
    sprintf(nome_file,"%d.bud",a);
    //Scrivo la data nei campi data riepilogo
    sprintf(stringa,"%02d",g);
    ComboBox_SetText(RiGior,stringa);
    sprintf(stringa,"%02d",m);
    ComboBox_SetText(RiMes,stringa);
    sprintf(stringa,"%02d",a);
    ComboBox_SetText(RiAnn,stringa);
    sprintf(stringa,"%s",gset[tcheck.tm_wday]);
    SetDlgItemText(hwndDlg,GSETTIMANA,stringa);

    if((erno=caricasaldi(num_g,t_risorse1))<0)
    {
        //C'e' stato un problema nella lettura di basedata.dat
        sprintf(stringa,"TOggiDia, riepiloga, caricasaldi,controllabasedata: erno n. %d",erno);
        MessageBox(hwndDlg,stringa,
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        scrivilog(1,stringa);
        return -1;
    }
    if((erno=aggiornasaldi(num_g,numfonti,nome_file,t_risorse1))<0)
    {
        //C'e' stato un problema nella lettura del file di dati
        //o nella riscrittura di basedata.dat
        sprintf(stringa,"TOggiDia, riepiloga, aggiornasaldi : erno n. %d",erno);
        MessageBox(hwndDlg,stringa,
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        scrivilog(1,stringa);
        return -1;
    }
    mostrasaldi(hwndDlg,SELBANCA,SELDATO,MOSTRADATO,t_risorse1);

    //Individuo nome file da aprire
    sprintf(nome_file,"%d.bud",a);
    if(!(tmp=fopen("note.tmp","w")))
    {
        //Non riesco a creare il file: errore (precauzione eccessiva?)
        MessageBox(hwndDlg,
        "Non riesco a creare il file 'note.tmp'",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        scrivilog(1,(char*)"riepiloga: Non riesco a creare il file 'note.tmp'");
        return -1;
    }
    if(!(fil=fopen(nome_file,"r")))
    {
        //Se il file non esiste ritorno normalmente
        //con un note.tmp vuoto e nlnote=0
        nlnote=0;
        fclose(tmp);
        return 0;
    }
    do
    {
        if(!fgets(stringa,400,fil))
        {
            //Ho raggiunto la fine del file senza trovare
            //il giorno. Ritorno normalmente con un note.tmp vuoto
            //e nlnote=0
            nlnote=0;
            //Non ci sono righe di commento, non scrive nulla
            SetDlgItemText(hwndDlg,ANNOTAZ,"");
            fclose(tmp);
            fclose(fil);
            return 0;
        }
        if(sscanf(stringa,"%ld,%d,%d\n",&numg_fa,&num_ops,&nrighe)!=3)
        {
            //Trovo i dati in un formato non valido: errore
            MessageBox(hwndDlg,
            "Anomalia nel file dati:r1",
            "Problema!",MB_ICONEXCLAMATION | MB_OK);
            fclose(tmp);
            fclose(fil);
            scrivilog(1,(char*)"riepiloga: Anomalia nel file dati:r1");
            return -1;
        }
        if((numg_fa<num_g))
            //Sono a un giorno precedente a quello desiderato
            for(i=0;i<num_ops+nrighe;i++)
                if(!fgets(stringa,400,fil))
                {
                    //Mi aspettavo una riga e invece non trovo niente: errore
                    MessageBox(hwndDlg,
                    "Anomalia nel file dati:r2",
                    "Problema!",MB_ICONEXCLAMATION | MB_OK);
                    fclose(tmp);
                    fclose(fil);
                    scrivilog(1,(char*)"riepiloga: Anomalia nel file dati:r2");
                    return -1;
                }
    }
    while(numg_fa<num_g);
    if(numg_fa>num_g)
    {
        //Se sono qui:
        //Il giorno che mi interessa
        //non conteneva gia' dei dati ma almeno uno
        //dei giorni successivi li contiene
        //Ritorno normalmente con un note.tmp vuoto
        //e nlnote=0
        nlnote=0;
        //Non ci sono righe di commento, non scrive nulla
        SetDlgItemText(hwndDlg,ANNOTAZ,"");
        fclose(tmp);
        fclose(fil);
        return 0;
    }
    //Se sono qui: numg_fa==num_g
    //Il giorno che mi interessa contiene gia' dei dati
    //Comincia la scrittura della casella
    ii=0;
    for(i=0;i<num_ops;i++)
    {
        if(!fgets(stringa,400,fil))
        {
            //Mi aspettavo una riga e invece non trovo niente: errore
            MessageBox(hwndDlg,
            "Anomalia nel file dati:r3",
            "Problema!",MB_ICONEXCLAMATION | MB_OK);
            fclose(tmp);
            fclose(fil);
            scrivilog(1,(char*)"riepiloga: Anomalia nel file dati:r3");
            return -1;
        }
        if(sscanf(stringa,"%d,%lf,%[!-~ ‘’@ςΰωθιμ°η§]\t,%[!-~ ‘’@ςΰωθιμ°η§]\t,%d,%d,%ld,%d\n",&j,&importo_f,mot_f,cat_f,&pa,&pb,&numg_fb,&k)!=8)
        {
            //Trovo i dati in un formato non valido: errore
            MessageBox(hwndDlg,
            "Anomalia nel file dati:r4",
            "Problema!",MB_ICONEXCLAMATION | MB_OK);
            fclose(tmp);
            fclose(fil);
            scrivilog(1,(char*)"riepiloga: Anomalia nel file dati:r4");
            return -1;
        }
        //Se sono arrivato qui tutto e' a posto
        //Posso scrivere nella casella di riepilogo
        if(importo_f>0)
        {
            //Se e' un'entrata
            index=0;
            while(t_risorse1[index].posiz<pb)
            {
                index++;
            }
            sprintf(stringa,"%s\t| +%.2lf\t| %s\t| %s",t_risorse1[index].nomeconv,importo_f,mot_f,cat_f);
        }
        else
        {
            //Se e' un'uscita
            index=0;
            while(t_risorse1[index].posiz<pa)
            {
                index++;
            }
            sprintf(stringa,"%s\t| %.2lf\t| %s\t| %s",t_risorse1[index].nomeconv,importo_f,mot_f,cat_f);
        }
        if(t_risorse1[index].mostra)
        {
            //Solo se il flag .mostra della risorsa
            //non e' 0 il movimento viene mostrato
            ListBox_AddString(RIEPILO,stringa);
            //Ora scrivo in un record le informazioni relative
            //a questo movimento. Mi serviranno per cancellare
            //e per selezionare e cose simili.
            tabella_movimenti[ii].posiz_da=pa;
            tabella_movimenti[ii].posiz_in=pb;
            tabella_movimenti[ii].numop_a=j;
            tabella_movimenti[ii].numop_b=k;
            tabella_movimenti[ii].numg_a=numg_fa;
            tabella_movimenti[ii].numg_b=numg_fb;
            ii++;
        }
    }
    //Se sono arrivato qui ho finito il riempimento
    //Ora controllo le righe di note: se ci sono
    //le appoggio sul file "note.tmp"
    for(i=0;i<nrighe;i++)
    {
        if(!fgets(stringa,400,fil))
        {
            //Mi aspettavo una riga e invece non trovo niente: errore
            MessageBox(hwndDlg,
            "Anomalia nel file dati:r5",
            "Problema!",MB_ICONEXCLAMATION | MB_OK);
            fclose(fil);
            fclose(tmp);
            scrivilog(1,(char*)"riepiloga: Anomalia nel file dati:r5");
            return -1;
        }
        //Se sono qui posso ricopiare su note.tmp la riga
        fputs(stringa,tmp);
    }
    if(nrighe)
        //Se ci sono righe di commento, lo scrive
        SetDlgItemText(hwndDlg,ANNOTAZ,"Ci sono annotazioni");
    else
        //Se non ci sono righe di commento, non scrive nulla
        SetDlgItemText(hwndDlg,ANNOTAZ,"");
        //nlnote indica il numero di righe di note
        //viene assegnato all'inizio,
        //in questa funzione o dopo aver scritto le note
    nlnote=nrighe;
    fclose(tmp);
    fclose(fil);
    return 0;
}

void DHandValGen(HWND hwndDlg,WPARAM wParam)
{
    char stringa[401];

    HWND DESTINA =  GetDlgItem(hwndDlg, C_BANCA_IN);

    if((!ComboBox_GetText(DESTINA,stringa,27))&&((HIWORD(wParam)==CBN_SETFOCUS)||(HIWORD(wParam)==CBN_KILLFOCUS)))
    {
        //Non ho scritto 'entra in'
        if(!controllacampi(hwndDlg,ID_GEN,ID_MEN,ID_AEN))
            //Solo se la data e' valida,
            //Aggiorna la lista di entrata
            caricaLista(hwndDlg,ID_GEN,ID_MEN,ID_AEN,C_BANCA_IN);
        return;
    }
    //Se ho scritto 'entra in'
    if(HIWORD(wParam)==CBN_SETFOCUS)
    {
        caricacampi(hwndDlg,ID_GEN,ID_MEN,ID_AEN);
        //Aggiorna la lista
        caricaLista(hwndDlg,ID_GEN,ID_MEN,ID_AEN,C_BANCA_IN);
    }
    if(HIWORD(wParam)==CBN_KILLFOCUS)
    {
        caricacampi(hwndDlg,ID_GEN,ID_MEN,ID_AEN);
        //Aggiorna la lista
        caricaLista(hwndDlg,ID_GEN,ID_MEN,ID_AEN,C_BANCA_IN);
    }
}

void DHandValMen(HWND hwndDlg,WPARAM wParam)
{
    char stringa[401];

    HWND DESTINA =  GetDlgItem(hwndDlg, C_BANCA_IN);

    if((!ComboBox_GetText(DESTINA,stringa,27))&&((HIWORD(wParam)==CBN_SETFOCUS)||(HIWORD(wParam)==CBN_KILLFOCUS)))
    {
        //Non ho scritto 'entra in'
        if(!controllacampi(hwndDlg,ID_GEN,ID_MEN,ID_AEN))
            //Solo se la data e' valida,
            //Aggiorna la lista di entrata
            caricaLista(hwndDlg,ID_GEN,ID_MEN,ID_AEN,C_BANCA_IN);
        return;
    }
    //Se ho scritto 'entra in'
    if(HIWORD(wParam)==CBN_SETFOCUS)
    {
        caricacampi(hwndDlg,ID_GEN,ID_MEN,ID_AEN);
        //Aggiorna la lista
        caricaLista(hwndDlg,ID_GEN,ID_MEN,ID_AEN,C_BANCA_IN);
    }
    if(HIWORD(wParam)==CBN_KILLFOCUS)
    {
        caricacampi(hwndDlg,ID_GEN,ID_MEN,ID_AEN);
        //Aggiorna la lista
        caricaLista(hwndDlg,ID_GEN,ID_MEN,ID_AEN,C_BANCA_IN);
    }
}

void DHandValAen(HWND hwndDlg,WPARAM wParam)
{
    char stringa[401];

    HWND DESTINA =  GetDlgItem(hwndDlg, C_BANCA_IN);

    if((!ComboBox_GetText(DESTINA,stringa,27))&&((HIWORD(wParam)==CBN_SETFOCUS)||(HIWORD(wParam)==CBN_KILLFOCUS)))
    {
        //Non ho scritto 'entra in'
        if(!controllacampi(hwndDlg,ID_GEN,ID_MEN,ID_AEN))
            //Solo se la data e' valida,
            //Aggiorna la lista di entrata
            caricaLista(hwndDlg,ID_GEN,ID_MEN,ID_AEN,C_BANCA_IN);
        return;
    }
    //Se ho scritto 'entra in'
    if(HIWORD(wParam)==CBN_SETFOCUS)
    {
        caricacampi(hwndDlg,ID_GEN,ID_MEN,ID_AEN);
        //Aggiorna la lista
        caricaLista(hwndDlg,ID_GEN,ID_MEN,ID_AEN,C_BANCA_IN);
    }
    if(HIWORD(wParam)==CBN_KILLFOCUS)
    {
        caricacampi(hwndDlg,ID_GEN,ID_MEN,ID_AEN);
        //Aggiorna la lista
        caricaLista(hwndDlg,ID_GEN,ID_MEN,ID_AEN,C_BANCA_IN);
    }
}

void DHandValGusc(HWND hwndDlg,WPARAM wParam)
{
    char stringa[401];

    HWND PROVEN =  GetDlgItem(hwndDlg, C_BANCA_OUT);

    if((!ComboBox_GetText(PROVEN,stringa,27))&&((HIWORD(wParam)==CBN_SETFOCUS)||(HIWORD(wParam)==CBN_KILLFOCUS)))
    {
        //Non ho scritto 'entra in'
        if(!controllacampi(hwndDlg,ID_GUSC,ID_MUSC,ID_AUSC))
            //Solo se la data e' valida,
            //Aggiorna la lista di entrata
            caricaLista(hwndDlg,ID_GUSC,ID_MUSC,ID_AUSC,C_BANCA_OUT);
        return;
    }
    //Se ho scritto 'entra in'
    if(HIWORD(wParam)==CBN_SETFOCUS)
    {
        caricacampi(hwndDlg,ID_GUSC,ID_MUSC,ID_AUSC);
        //Aggiorna la lista
        caricaLista(hwndDlg,ID_GUSC,ID_MUSC,ID_AUSC,C_BANCA_OUT);
    }
    if(HIWORD(wParam)==CBN_KILLFOCUS)
    {
        caricacampi(hwndDlg,ID_GUSC,ID_MUSC,ID_AUSC);
        //Aggiorna la lista
        caricaLista(hwndDlg,ID_GUSC,ID_MUSC,ID_AUSC,C_BANCA_OUT);
    }
}

void DHandValMusc(HWND hwndDlg,WPARAM wParam)
{
    char stringa[401];

    HWND PROVEN =  GetDlgItem(hwndDlg, C_BANCA_OUT);

    if((!ComboBox_GetText(PROVEN,stringa,27))&&((HIWORD(wParam)==CBN_SETFOCUS)||(HIWORD(wParam)==CBN_KILLFOCUS)))
    {
        //Non ho scritto 'entra in'
        if(!controllacampi(hwndDlg,ID_GUSC,ID_MUSC,ID_AUSC))
            //Solo se la data e' valida,
            //Aggiorna la lista di entrata
            caricaLista(hwndDlg,ID_GUSC,ID_MUSC,ID_AUSC,C_BANCA_OUT);
        return;
    }
    //Se ho scritto 'entra in'
    if(HIWORD(wParam)==CBN_SETFOCUS)
    {
        caricacampi(hwndDlg,ID_GUSC,ID_MUSC,ID_AUSC);
        //Aggiorna la lista
        caricaLista(hwndDlg,ID_GUSC,ID_MUSC,ID_AUSC,C_BANCA_OUT);
    }
    if(HIWORD(wParam)==CBN_KILLFOCUS)
    {
        caricacampi(hwndDlg,ID_GUSC,ID_MUSC,ID_AUSC);
        //Aggiorna la lista
        caricaLista(hwndDlg,ID_GUSC,ID_MUSC,ID_AUSC,C_BANCA_OUT);
    }
}

void DHandValAusc(HWND hwndDlg,WPARAM wParam)
{
    char stringa[401];

    HWND PROVEN =  GetDlgItem(hwndDlg, C_BANCA_OUT);

    if((!ComboBox_GetText(PROVEN,stringa,27))&&((HIWORD(wParam)==CBN_SETFOCUS)||(HIWORD(wParam)==CBN_KILLFOCUS)))
    {
        //Non ho scritto 'entra in'
        if(!controllacampi(hwndDlg,ID_GUSC,ID_MUSC,ID_AUSC))
            //Solo se la data e' valida,
            //Aggiorna la lista di entrata
            caricaLista(hwndDlg,ID_GUSC,ID_MUSC,ID_AUSC,C_BANCA_OUT);
        return;
    }
    //Se ho scritto 'entra in'
    if(HIWORD(wParam)==CBN_SETFOCUS)
    {
        caricacampi(hwndDlg,ID_GUSC,ID_MUSC,ID_AUSC);
        //Aggiorna la lista
        caricaLista(hwndDlg,ID_GUSC,ID_MUSC,ID_AUSC,C_BANCA_OUT);
    }
    if(HIWORD(wParam)==CBN_KILLFOCUS)
    {
        caricacampi(hwndDlg,ID_GUSC,ID_MUSC,ID_AUSC);
        //Aggiorna la lista
        caricaLista(hwndDlg,ID_GUSC,ID_MUSC,ID_AUSC,C_BANCA_OUT);
    }
}

void DHandValRiG(HWND hwndDlg,WPARAM wParam)
{
    HWND SELBANC =  GetDlgItem(hwndDlg, SELBANCA);

    if(HIWORD(wParam)==CBN_SETFOCUS)
    {
        caricacampi(hwndDlg,ID_GG,ID_MM,ID_AAAA);
        //Aggiorna la lista
        caricaLista(hwndDlg,ID_GG,ID_MM,ID_AAAA,SELBANCA);
        //Inserisco la voce per selezionare i dati globali
        ComboBox_AddString(SELBANC,"Globale");
    }
    if(HIWORD(wParam)==CBN_KILLFOCUS)
    {
        caricacampi(hwndDlg,ID_GG,ID_MM,ID_AAAA);
        //Aggiorna la lista
        caricaLista(hwndDlg,ID_GG,ID_MM,ID_AAAA,SELBANCA);
        //Inserisco la voce per selezionare i dati globali
        ComboBox_AddString(SELBANC,"Globale");
    }
}

void DHandValRiM(HWND hwndDlg,WPARAM wParam)
{
    HWND SELBANC =  GetDlgItem(hwndDlg, SELBANCA);

    if(HIWORD(wParam)==CBN_SETFOCUS)
    {
        caricacampi(hwndDlg,ID_GG,ID_MM,ID_AAAA);
        //Aggiorna la lista
        caricaLista(hwndDlg,ID_GG,ID_MM,ID_AAAA,SELBANCA);
        //Inserisco la voce per selezionare i dati globali
        ComboBox_AddString(SELBANC,"Globale");
    }
    if(HIWORD(wParam)==CBN_KILLFOCUS)
    {
        caricacampi(hwndDlg,ID_GG,ID_MM,ID_AAAA);
        //Aggiorna la lista
        caricaLista(hwndDlg,ID_GG,ID_MM,ID_AAAA,SELBANCA);
        //Inserisco la voce per selezionare i dati globali
        ComboBox_AddString(SELBANC,"Globale");
    }
}

void DHandValRiA(HWND hwndDlg,WPARAM wParam)
{
    HWND SELBANC =  GetDlgItem(hwndDlg, SELBANCA);

    if(HIWORD(wParam)==CBN_SETFOCUS)
    {
        caricacampi(hwndDlg,ID_GG,ID_MM,ID_AAAA);
        //Aggiorna la lista
        caricaLista(hwndDlg,ID_GG,ID_MM,ID_AAAA,SELBANCA);
        //Inserisco la voce per selezionare i dati globali
        ComboBox_AddString(SELBANC,"Globale");
    }
    if(HIWORD(wParam)==CBN_KILLFOCUS)
    {
        caricacampi(hwndDlg,ID_GG,ID_MM,ID_AAAA);
        //Aggiorna la lista
        caricaLista(hwndDlg,ID_GG,ID_MM,ID_AAAA,SELBANCA);
        //Inserisco la voce per selezionare i dati globali
        ComboBox_AddString(SELBANC,"Globale");
    }
}

void CheckEntrata(HWND hwndDlg)
{
//Deseleziono uscita e seleziono entrata
//La destinazione di default e' la prima
//della lista
//Il giorno dell'entrata e' quello corrente
//dell'uscita

    char stringa[401];

    HWND EGior=  GetDlgItem(hwndDlg, ID_GEN);
    HWND EMes=  GetDlgItem(hwndDlg, ID_MEN);
    HWND EAnn=  GetDlgItem(hwndDlg, ID_AEN);
    HWND UGior=  GetDlgItem(hwndDlg, ID_GUSC);
    HWND UMes=  GetDlgItem(hwndDlg, ID_MUSC);
    HWND UAnn=  GetDlgItem(hwndDlg, ID_AUSC);
    HWND DESTINA =  GetDlgItem(hwndDlg, C_BANCA_IN);
    HWND PROVEN =  GetDlgItem(hwndDlg, C_BANCA_OUT);

    //Se era gia' selezionato non faccio nulla
    if(entra)
        return;
    CheckRadioButton(hwndDlg,PULSENTR,PULSUSC,PULSENTR);
    ComboBox_GetLBText(DESTINA,0,stringa);
    ComboBox_SetText(DESTINA,stringa);
    ComboBox_SetText(PROVEN,"");
    ComboBox_GetText(UGior,stringa,3);
    ComboBox_ResetContent(UGior);
    ComboBox_SetText(EGior,stringa);
    ComboBox_GetText(UMes,stringa,3);
    ComboBox_ResetContent(UMes);
    ComboBox_SetText(EMes,stringa);
    ComboBox_GetText(UAnn,stringa,5);
    ComboBox_ResetContent(UAnn);
    ComboBox_SetText(EAnn,stringa);
    entra=1;
}


void CheckUscita(HWND hwndDlg)
{
//Deseleziono entrata e seleziono uscita
//La provenienza di default e' la prima
//della lista
//Il giorno dell'uscita e' quello corrente
//dell'entrata

    char stringa[401];

    HWND EGior=  GetDlgItem(hwndDlg, ID_GEN);
    HWND EMes=  GetDlgItem(hwndDlg, ID_MEN);
    HWND EAnn=  GetDlgItem(hwndDlg, ID_AEN);
    HWND UGior=  GetDlgItem(hwndDlg, ID_GUSC);
    HWND UMes=  GetDlgItem(hwndDlg, ID_MUSC);
    HWND UAnn=  GetDlgItem(hwndDlg, ID_AUSC);
    HWND DESTINA =  GetDlgItem(hwndDlg, C_BANCA_IN);
    HWND PROVEN =  GetDlgItem(hwndDlg, C_BANCA_OUT);

    //Se era gia' selezionato non faccio nulla
    if(!entra)
        return;
    CheckRadioButton(hwndDlg,PULSENTR,PULSUSC,PULSUSC);
    ComboBox_GetLBText(PROVEN,0,stringa);
    ComboBox_SetText(PROVEN,stringa);
    ComboBox_SetText(DESTINA,"");
    ComboBox_GetText(EGior,stringa,3);
    ComboBox_ResetContent(EGior);
    ComboBox_SetText(UGior,stringa);
    ComboBox_GetText(EMes,stringa,3);
    ComboBox_ResetContent(EMes);
    ComboBox_SetText(UMes,stringa);
    ComboBox_GetText(EAnn,stringa,5);
    ComboBox_ResetContent(EAnn);
    ComboBox_SetText(UAnn,stringa);
    entra=0;
}

void HandAnnulla(HWND hwndDlg)
{
    //Cancella i campi e ripristina una configurazione di default

    HWND IMPO =  GetDlgItem(hwndDlg, IMPORTO);
    HWND MOTI =  GetDlgItem(hwndDlg, MOTIVO);
    HWND CATE =  GetDlgItem(hwndDlg, CATEG);
    HWND DESTINA =  GetDlgItem(hwndDlg, C_BANCA_IN);
    HWND PROVEN =  GetDlgItem(hwndDlg, C_BANCA_OUT);
    HWND EGior =  GetDlgItem(hwndDlg, ID_GEN);
    HWND EMes =  GetDlgItem(hwndDlg, ID_MEN);
    HWND EAnn =  GetDlgItem(hwndDlg, ID_AEN);
    HWND UGior =  GetDlgItem(hwndDlg, ID_GUSC);
    HWND UMes =  GetDlgItem(hwndDlg, ID_MUSC);
    HWND UAnn =  GetDlgItem(hwndDlg, ID_AUSC);

    ComboBox_SetText(IMPO,"");
    ComboBox_SetText(MOTI,"");
    ComboBox_SetText(CATE,"");
    ComboBox_SetText(DESTINA,"");
    ComboBox_SetText(PROVEN,"");
    if(entra)
    {
        //Se e' selezionato il pulsante entrata
        //cancello i campi relativi a uscita,
        //mantengo la data dell'entrata, e metto
        //su "va in" il primo elemento della lista
        ComboBox_SetText(UGior,"");
        ComboBox_SetText(UMes,"");
        ComboBox_SetText(UAnn,"");
        caricacampi(hwndDlg,ID_GEN,ID_MEN,ID_AEN);
        //Aggiorna la lista
        caricaLista(hwndDlg,ID_GEN,ID_MEN,ID_AEN,C_BANCA_IN);
        ComboBox_GetLBText(DESTINA,0,stringa);
        ComboBox_SetText(DESTINA,stringa);
    }
    else
    {
        //Se e' selezionato il pulsante uscita
        //cancello i campi relativi a data entrata,
        //mantengo la data dell'uscita, e metto
        //su "viene da" il primo elemento della lista
        ComboBox_SetText(EGior,"");
        ComboBox_SetText(EMes,"");
        ComboBox_SetText(EAnn,"");
        caricacampi(hwndDlg,ID_GUSC,ID_MUSC,ID_AUSC);
        //Aggiorna la lista
        caricaLista(hwndDlg,ID_GUSC,ID_MUSC,ID_AUSC,C_BANCA_OUT);
        ComboBox_GetLBText(PROVEN,0,stringa);
        ComboBox_SetText(PROVEN,stringa);
    }
    scrivilog(1,(char*)"annulla end");
}

void HandScrivi(HWND hwndDlg)
{
    char stringa[401],stringa1[401],nomeconv[27],mot[100],cat[100],
    nomefile_in[30],nomefile_out[30],slog[20];

    int i,j,k,n_lpa,n_lpb,va_in,viene_da,g,m,a,erno;

    long int numg_in,numg_out;

    double importo;

    HWND IMPO =  GetDlgItem(hwndDlg, IMPORTO);
    HWND MOTI =  GetDlgItem(hwndDlg, MOTIVO);
    HWND CATE = GetDlgItem(hwndDlg, CATEG);
    HWND EGior = GetDlgItem(hwndDlg, ID_GEN);
    HWND EMes =  GetDlgItem(hwndDlg, ID_MEN);
    HWND EAnn =  GetDlgItem(hwndDlg, ID_AEN);
    HWND UGior =  GetDlgItem(hwndDlg, ID_GUSC);
    HWND UMes =  GetDlgItem(hwndDlg, ID_MUSC);
    HWND UAnn =  GetDlgItem(hwndDlg, ID_AUSC);
    HWND DESTINA =  GetDlgItem(hwndDlg, C_BANCA_IN);
    HWND PROVEN =  GetDlgItem(hwndDlg, C_BANCA_OUT);

    scrivilog(1,(char*)"Forse Mouse Scrivi");
    ComboBox_GetText(IMPO,stringa,27);
    k=sscanf(stringa,"%lf%s",&importo,nomeconv);
    if(k!=1)
    {
        MessageBox(hwndDlg,
        "Importo inserito non e' un numero valido",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        scrivilog(1,(char*)"HandScrivi: Importo inserito non e' un numero valido");
        return;
    }
    //Il trucco seguente e' per troncare via eventuali
    //cifre decimali oltre la seconda, e scoprire importi di fatto
    //nulli (es. 0.001). Notare che >=0.005 diventa 0.01
    sprintf(stringa,"%.2lf",importo);
    sscanf(stringa,"%lf",&importo);
    if(importo<=0)
    {
        MessageBox(hwndDlg,
        "I valori negativi o nulli non sono ammessi.",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        scrivilog(1,(char*)"HandScrivi: I valori negativi o nulli non sono ammessi.");
        return;
    }
    if(!(ComboBox_GetText(MOTI,mot,82)))
    {
        MessageBox(hwndDlg,"Inserire Motivo",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        scrivilog(1,(char*)"HandScrivi: Inserire Motivo");
        return;
    }
    if(!(ComboBox_GetText(CATE,cat,42)))
    {
        MessageBox(hwndDlg,"Inserire Categoria",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        scrivilog(1,(char*)"HandScrivi: Inserire Categoria");
        return;
    }
    if(entra)
    {
        //Scrivo un'entrata:
        //Controllo che i dati in entrata siano a posto
        //(obbligatorio)
        //Quindi vedo se la somma in entrata viene
        //da un'altra mia risorsa
        if(!(ComboBox_GetText(DESTINA,stringa,27)))
        {
            MessageBox(hwndDlg,"Inserire 'Entra in'",
            "Problema!",MB_ICONEXCLAMATION | MB_OK);
            scrivilog(1,(char*)"HandScrivi: Inserire 'Entra in'");
            return;
        }
        if((k=SendMessage(DESTINA,CB_FINDSTRINGEXACT,-1, (LPARAM) stringa))<0)
        {
            MessageBox(hwndDlg,
            "'Entra in' inserito non corrisponde a una voce valida",
            "Problema!",MB_ICONEXCLAMATION | MB_OK);
            scrivilog(1,(char*)"HandScrivi: 'Entra in' inserito non corrisponde a una voce valida");
            return;
        }
        //Individuo il numero della risorsa selezionata
        i=0;
        while(strcmp(stringa,t_risorse1[i].nomeconv))
            i++;
        va_in=t_risorse1[i].posiz;
        ComboBox_GetText(EAnn,stringa,5);
        sscanf(stringa,"%d",&a);
        ComboBox_GetText(EMes,stringa,3);
        sscanf(stringa,"%d",&m);
        ComboBox_GetText(EGior,stringa,3);
        sscanf(stringa,"%d",&g);
        numg_in=(long int)a*10000+m*100+g;
        //Individuo il file su cui scrivere l'entrata
        sprintf(nomefile_in,"%d.bud",a);
        //Individuo il numero d'ordine dell'entrata
        //nel giorno scelto (serve per il link)
        if((n_lpa=leggiprepara(numg_in,nomefile_in))<0)
        {
            MessageBox(hwndDlg,
            "Anomalia nel file dati entrata",
            "Problema!",MB_ICONEXCLAMATION | MB_OK);
            scrivilog(1,(char*)"HandScrivi: Anomalia nel file dati entrata 1");
            return;
        }
        //Controllo preliminare sulla data di uscita
        //(in questa fase mi e' concesso
        //lasciarla in bianco o scriverla male)
        if((j=controllacampi(hwndDlg,ID_GUSC,ID_MUSC,ID_AUSC))<0)
        {
            //Se non e' un giorno valido:
            //Aggiorno la lista di uscita con il giorno
            //di entrata (mi puo' servire dopo)
            caricaLista(hwndDlg,ID_GEN,ID_MEN,ID_AEN,C_BANCA_OUT);
        }
        //Verifico se l'entrata esce da un'altra mia risorsa
        if(!(ComboBox_GetText(PROVEN,stringa,27)))
        {
            //No, non esce da un'altra mia risorsa
            n_lpb=0;
            viene_da=0;
            numg_out=0;
        }
        else if((k=SendMessage(PROVEN,CB_FINDSTRINGEXACT,-1, (LPARAM) stringa))<0)
        {
            //Ho  scritto qualcosa, ma non corrisponde a niente:
            //chiedo lumi sul da farsi:
            //o faccio come se non avessi scritto niente
            //o annullo
            if(MessageBox(hwndDlg,"'Esce da' inserita non e'una voce valida.La ignoro e vado avanti (e' come se non l'avessi scritta)?",
            "Problema!",MB_ICONEXCLAMATION | MB_YESNO)==IDNO)
            {
                //Ho deciso che voglio correggere prima di scrivere
                scrivilog(1,(char*)"HandScrivi: 'Esce da' inserita non e'una voce valida. La ignoro e vado avanti (e' come se non l'avessi scritta)? NO");
                return;
            }
            //Se sono qui vuol dire che voglio ignorarla
            n_lpb=0;
            viene_da=0;
            numg_out=0;
            ComboBox_SetText(PROVEN,"");
            ComboBox_SetText(UAnn,"");
            ComboBox_SetText(UMes,"");
            ComboBox_SetText(UGior,"");
        }
        else
        {
            //Si, esce da un'altra risorsa
            //Devo scrivere anche questa informazione
            //
            //Prima mi accerto della data inserita
            if(j<0)
            {
                //Se non e' un giorno valido chiedo lumi:
                //o tengo il giorno dell'entrata
                //o annullo
                if(MessageBox(hwndDlg,"La data di uscita non e' corretta o manca. Uso lo stesso giorno dell'entrata?",
                "Problema!",MB_ICONEXCLAMATION | MB_YESNO)==IDNO)
                {
                    //Ho deciso che voglio correggere prima di scrivere
                    scrivilog(1,(char*)"HandScrivi: La data di uscita non e' corretta o manca. Uso lo stesso giorno dell'entrata? NO");
                    return;
                }
                //Se sono qui allora ricopio i campi di entrata
                //nell'uscita
                ComboBox_GetText(EAnn,stringa1,5);
                ComboBox_SetText(UAnn,stringa1);
                ComboBox_GetText(EMes,stringa1,3);
                ComboBox_SetText(UMes,stringa1);
                ComboBox_GetText(EGior,stringa1,3);
                ComboBox_SetText(UGior,stringa1);
            }
            //Individuo il numero della risorsa selezionata
            i=0;
            while(strcmp(stringa,t_risorse1[i].nomeconv))
                i++;
            viene_da=t_risorse1[i].posiz;
            ComboBox_GetText(UAnn,stringa,5);
            sscanf(stringa,"%d",&a);
            ComboBox_GetText(UMes,stringa,3);
            sscanf(stringa,"%d",&m);
            ComboBox_GetText(UGior,stringa,3);
            sscanf(stringa,"%d",&g);
            numg_out=(long int)a*10000+m*100+g;
            //Individuo il file su cui scrivere l'uscita
            //Potrebbe essere diverso da quello di entrata
            //(se gli anni sono diversi)
            sprintf(nomefile_out,"%d.bud",a);
            //Individuo il numero d'ordine dell'uscita
            //nel giorno scelto (serve per il link)
            if((n_lpb=leggiprepara(numg_out,nomefile_out))<0)
            {
                MessageBox(hwndDlg,
                "Anomalia nel file dati uscita",
                "Problema!",MB_ICONEXCLAMATION | MB_OK);
                scrivilog(1,(char*)"HandScrivi: Anomalia nel file dati uscita 2");
                return;
            }
            if(n_lpb==n_lpa)
                //Se i due giorni coincidono, ho ottenuto n_lpa==n_lpb!!
                //Questo e' un errore, questa e' la soluzione
                n_lpa=n_lpb+1;
            //Ho tutte le informazioni per scrivere nel file di uscita
            //Preparo la stringa da scrivere nel file
            sprintf(stringa,"%d,-%.2lf,%s\t,%s\t,%d,%d,%ld,%d",n_lpb,importo,mot,cat,viene_da,va_in,numg_in,n_lpa);
            scrivilog(1,(char*)"HandScrivi: scrivifile 1 - nomefile_out,numg_out,stringa:");
            scrivilog(0,nomefile_out);
            scrivilog(0,ltoa(numg_out,slog,10));
            scrivilog(0,stringa);
            //Scrivo la stringa nel file di uscita
            if((erno=scrivifile(nomefile_out,numg_out,stringa))<0)
            {
                sprintf(stringa,"HandScrivi uscita: scrivifile erno %d",erno);
                MessageBox(hwndDlg,stringa,
                "Problema!",MB_ICONEXCLAMATION | MB_OK);
                scrivilog(1,stringa);
                return;
            }
        }
        //Ho tutte le informazioni per scrivere nel file di entrata
        //Preparo la stringa da scrivere nel file
        sprintf(stringa,"%d,+%.2lf,%s\t,%s\t,%d,%d,%ld,%d",n_lpa,importo,mot,cat,viene_da,va_in,numg_out,n_lpb);
        scrivilog(1,(char*)"HandScrivi: scrivifile 2 - nomefile_in,numg_in,stringa:");
        scrivilog(0,nomefile_in);
        scrivilog(0,ltoa(numg_in,slog,10));
        scrivilog(0,stringa);
        //Scrivo la stringa nel file di entrata
        if((erno=scrivifile(nomefile_in,numg_in,stringa))<0)
        {
            sprintf(stringa,"HandScrivi entrata: scrivifile erno %d",erno);
            MessageBox(hwndDlg,stringa,
            "Problema!",MB_ICONEXCLAMATION | MB_OK);
            scrivilog(1,stringa);
            return;
        }
        //Ora i file sono stati aggiornati con successo
        //Ripulisco i campi importo, motivo, categoria
        //ma non gli altri.
        ComboBox_SetText(IMPO,"");
        ComboBox_SetText(MOTI,"");
        aggiornacategorie(hwndDlg);
        ComboBox_SetText(CATE,"");
        //Quindi aggiorno il riepilogo sul giorno dell'entrata
        riepiloga(hwndDlg,numg_in);
    }
    else
    {
        //Scrivo un'uscita:
        //Controllo che i dati in uscita siano a posto
        //(obbligatorio)
        //Quindi vedo se la somma in uscita va
        //in un'altra mia risorsa
        if(!(ComboBox_GetText(PROVEN,stringa,27)))
        {
            MessageBox(hwndDlg,"Inserire 'Esce da'",
            "Problema!",MB_ICONEXCLAMATION | MB_OK);
            scrivilog(1,(char*)"HandScrivi: Inserire 'Esce da'");
            return;
        }
        if((k=SendMessage(PROVEN,CB_FINDSTRINGEXACT,-1, (LPARAM) stringa))<0)
        {
            MessageBox(hwndDlg,
            "'Esce da' inserito non corrisponde a una voce valida",
            "Problema!",MB_ICONEXCLAMATION | MB_OK);
            scrivilog(1,(char*)"HandScrivi: 'Esce da' inserito non corrisponde a una voce valida");
            return;
        }
        //Individuo il numero della risorsa selezionata
        i=0;
        while(strcmp(stringa,t_risorse1[i].nomeconv))
            i++;
        viene_da=t_risorse1[i].posiz;
        ComboBox_GetText(UAnn,stringa,5);
        sscanf(stringa,"%d",&a);
        ComboBox_GetText(UMes,stringa,3);
        sscanf(stringa,"%d",&m);
        ComboBox_GetText(UGior,stringa,3);
        sscanf(stringa,"%d",&g);
        numg_out=(long int)a*10000+m*100+g;
        //Individuo il file su cui scrivere l'uscita
        sprintf(nomefile_out,"%d.bud",a);
        //Individuo il numero d'ordine dell'uscita
        //nel giorno scelto (serve per il link)
        if((n_lpa=leggiprepara(numg_out,nomefile_out))<0)
        {
            MessageBox(hwndDlg,
            "Anomalia nel file dati uscita",
            "Problema!",MB_ICONEXCLAMATION | MB_OK);
            scrivilog(1,(char*)"HandScrivi: Anomalia nel file dati uscita 8");
            return;
        }
        //Controllo preliminare sulla data di entrata
        //(in questa fase mi e' concesso
        //lasciarla in bianco o scriverla male)
        if((j=controllacampi(hwndDlg,ID_GEN,ID_MEN,ID_AEN))<0)
        {
            //Se non e' un giorno valido:
            //Aggiorno la lista di entrata con il giorno
            //di uscita (mi puo' servire dopo)
            caricaLista(hwndDlg,ID_GUSC,ID_MUSC,ID_AUSC,C_BANCA_IN);
        }
        //Verifico se l'uscita entra in un'altra mia risorsa
        if(!(ComboBox_GetText(DESTINA,stringa,27)))
        {
            //No, non entra in un'altra mia risorsa
            n_lpb=0;
            va_in=0;
            numg_in=0;
        }
        else if((va_in=SendMessage(DESTINA,CB_FINDSTRINGEXACT,-1, (LPARAM) stringa))<0)
        {
            //Ho  scritto qualcosa, ma non corrisponde a niente:
            //chiedo lumi sul da farsi:
            //o faccio come se non avessi scritto niente
            //o annullo
            if(MessageBox(hwndDlg,"'Entra in' inserita non e'una voce valida.La ignoro e vado avanti (e' come se non l'avessi scritta)?",
            "Problema!",MB_ICONEXCLAMATION | MB_YESNO)==IDNO)
            {
                //Ho deciso che voglio correggere prima di scrivere
                scrivilog(1,(char*)"HandScrivi: 'Entra in' inserita non e'una voce valida.La ignoro e vado avanti (e' come se non l'avessi scritta)? NO");
	        return;
            }
            //Se sono qui vuol dire che voglio ignorarla
            n_lpb=0;
            va_in=0;
            numg_in=0;
            ComboBox_SetText(DESTINA,"");
            ComboBox_SetText(EAnn,"");
            ComboBox_SetText(EMes,"");
            ComboBox_SetText(EGior,"");
        }
        else
        {
            //Si, entra in un'altra risorsa
            //Devo scrivere anche questa informazione
            //
            //Prima mi accerto della data inserita
            if(j<0)
            {
                //Se non e' un giorno valido chiedo lumi:
                //o tengo il giorno dell'uscita
                //o annullo
                if
                (MessageBox(hwndDlg,"La data di entrata non e' corretta o manca. Uso lo stesso giorno dell'uscita?",
                "Problema!",MB_ICONEXCLAMATION | MB_YESNO)==IDNO)
                {
                    //Ho deciso che voglio correggere prima di scrivere
                    scrivilog(1,(char*)"HandScrivi: La data di entrata non e' corretta o manca. Uso lo stesso giorno dell'uscita? NO");
                    return;
                }
                //Se sono qui allora ricopio i campi di uscita
                //nell'entrata
                ComboBox_GetText(UAnn,stringa1,5);
                ComboBox_SetText(EAnn,stringa1);
                ComboBox_GetText(UMes,stringa1,3);
                ComboBox_SetText(EMes,stringa1);
                ComboBox_GetText(UGior,stringa1,3);
                ComboBox_SetText(EGior,stringa1);
            }
            //Individuo il numero della risorsa selezionata
            i=0;
            while(strcmp(stringa,t_risorse1[i].nomeconv))
                i++;
            va_in=t_risorse1[i].posiz;
            ComboBox_GetText(EAnn,stringa,5);
            sscanf(stringa,"%d",&a);
            ComboBox_GetText(EMes,stringa,3);
            sscanf(stringa,"%d",&m);
            ComboBox_GetText(EGior,stringa,3);
            sscanf(stringa,"%d",&g);
            numg_in=(long int)a*10000+m*100+g;
            //Individuo il file su cui scrivere l'entrata
            //Potrebbe essere diverso da quello di uscita
            //(se gli anni sono diversi)
            sprintf(nomefile_in,"%d.bud",a);
            //Individuo il numero d'ordine dell'entrata
            //nel giorno scelto (serve per il link)
            if((n_lpb=leggiprepara(numg_in,nomefile_in))<0)
            {
                MessageBox(hwndDlg,
                "Anomalia nel file dati entrata",
                "Problema!",MB_ICONEXCLAMATION | MB_OK);
                scrivilog(1,(char*)"HandScrivi: Anomalia nel file dati entrata 9");
                return;
            }
            if(n_lpb==n_lpa)
                //Se i due giorni coincidono, ho ottenuto n_lpa==n_lpb!!
                //Questo e' un errore, questa e' la soluzione
                n_lpa=n_lpb+1;
            //Ho tutte le informazioni per scrivere nel file di entrata
            //Preparo la stringa da scrivere nel file
            sprintf(stringa,"%d,+%.2lf,%s\t,%s\t,%d,%d,%ld,%d",
		    n_lpb,importo,mot,cat,viene_da,va_in,numg_out,n_lpa);
            scrivilog(1,(char*)"HandScrivi: scrivifile 3 - nomefile_in,numg_in,stringa:");
            scrivilog(0,nomefile_in);
            scrivilog(0,ltoa(numg_in,slog,10));
            scrivilog(0,stringa);
            //Scrivo la stringa nel file di entrata
            if((erno=scrivifile(nomefile_in,numg_in,stringa))<0)
            {
                sprintf(stringa,"HandScrivi entrata: scrivifile erno %d",erno);
                MessageBox(hwndDlg,stringa,
                "Problema!",MB_ICONEXCLAMATION | MB_OK);
                scrivilog(1,stringa);
                return;
            }
        }
        //Ho tutte le informazioni per scrivere nel file di uscita
        //Preparo la stringa da scrivere nel file
        sprintf(stringa,"%d,-%.2lf,%s\t,%s\t,%d,%d,%ld,%d",n_lpa,importo,mot,cat,viene_da,va_in,numg_in,n_lpb);
        scrivilog(1,(char*)"HandScrivi: scrivifile 4 - nomefile_out,numg_out,stringa:");
        scrivilog(0,nomefile_out);
        scrivilog(0,ltoa(numg_out,slog,10));
        scrivilog(0,stringa);
        //Scrivo la stringa nel file di uscita
        if((erno=scrivifile(nomefile_out,numg_out,stringa))<0)
        {
            sprintf(stringa,"HandScrivi uscita: scrivifile erno %d",erno);
            MessageBox(hwndDlg,stringa,
            "Problema!",MB_ICONEXCLAMATION | MB_OK);
            scrivilog(1,stringa);
            return;
        }
        //Ora i file sono stati aggiornati con successo
        //Ripulisco i campi importo, motivo, categoria
        //ma non gli altri.
        ComboBox_SetText(IMPO,"");
        ComboBox_SetText(MOTI,"");
        aggiornacategorie(hwndDlg);
        ComboBox_SetText(CATE,"");
        //Quindi aggiorno il riepilogo sul giorno dell'uscita
        riepiloga(hwndDlg,numg_out);
    }
}

void aggiornacategorie(HWND hwndDlg)
//Aggiorna il file di categorie con quelle
//che trova nella lista
{
    char stringa1[401],cat[100];

    int i,e;

    FILE *tmp;

    HWND CATE = GetDlgItem(hwndDlg, CATEG);

    if(ComboBox_GetText(CATE,stringa1,42)>0)
        //Se c'e' una stringa
        if((SendMessage(CATE,CB_FINDSTRINGEXACT,-1, (LPARAM) stringa1))<0)
            //Se la stringa non esisteva nella lista
            //la aggiungo
            ComboBox_AddString(CATE,stringa1);
    //Adesso ricopio nel file tutti i membri della lista
    if((e=ComboBox_GetCount(CATE))<0)
        return;
    tmp=fopen("categ.dat","w");
    fprintf(tmp,"%d\n",e);
    for(i=0;i<e;i++)
    {
        ComboBox_GetLBText(CATE,i,cat);
        fprintf(tmp,"%s\n",cat);
    }
    fclose(tmp);
}

void HandGDopo(HWND hwndDlg)
{
    //Mostra i movimenti del giorno successivo
    char stringa[401];
    int g,m,a;
    long int numg;

    HWND RiGior =  GetDlgItem(hwndDlg, ID_GG);
    HWND RiMes =  GetDlgItem(hwndDlg, ID_MM);
    HWND RiAnn =  GetDlgItem(hwndDlg, ID_AAAA);


    scrivilog(1,(char*)"Forse Mouse Gdopo");
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
    //Ricavo il numero data e aggiorno il riepilogo
    numg=(long int)a*10000+m*100+g;
    riepiloga(hwndDlg,numg);
    scrivilog(0,(char*)"Giorno corrente:");
    scrivilog(0,ltoa(numg,stringa,10));
}

void HandGPrima(HWND hwndDlg)
{
    //Mostra i movimenti del giorno precedente

    char stringa[401];
    int g,m,a;
    long int numg;

    HWND RiGior =  GetDlgItem(hwndDlg, ID_GG);
    HWND RiMes =  GetDlgItem(hwndDlg, ID_MM);
    HWND RiAnn =  GetDlgItem(hwndDlg, ID_AAAA);

    scrivilog(1,(char*)"Forse Mouse Gprima");
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
    //Ricavo il numero data e aggiorno il riepilogo
    numg=(long int)a*10000+m*100+g;
    riepiloga(hwndDlg,numg);
    scrivilog(0,(char*)"Giorno corrente:");
    scrivilog(0,ltoa(numg,stringa,10));
}

void HandVai(HWND hwndDlg)
{
    //Mostra i movimenti alla data indicata nei campi
    //di riepilogo

    char stringa[401];

    int g,m,a;

    long int numg;

    HWND RiGior = GetDlgItem(hwndDlg, ID_GG);
    HWND RiMes = GetDlgItem(hwndDlg, ID_MM);
    HWND RiAnn = GetDlgItem(hwndDlg, ID_AAAA);

    scrivilog(1,(char*)"Forse Mouse Vai");
    caricacampi(hwndDlg,ID_GG,ID_MM,ID_AAAA);
    ComboBox_GetText(RiAnn,stringa,5);
    sscanf(stringa,"%d",&a);
    ComboBox_GetText(RiMes,stringa,3);
    sscanf(stringa,"%d",&m);
    ComboBox_GetText(RiGior,stringa,3);
    sscanf(stringa,"%d",&g);
    numg=(long int)a*10000+m*100+g;
    riepiloga(hwndDlg,numg);
    scrivilog(0,(char*)"Giorno corrente:");
    scrivilog(0,ltoa(numg,stringa,10));
}

void ved(HWND hwndDlg)
//Fa vedere nel riepilogo le risorse
//selezionate
{
    char stringa[401];

    int i,j,k,ind[100];

    HWND FAIVED =  GetDlgItem(hwndDlg, ID_FAIVED);

    //Prima prendo gli indici delle risorse
    //selezionate nella casella
    k=SendMessage(FAIVED,LB_GETSELITEMS,100,(LPARAM)ind);
    for(i=0;i<k;i++)
    {
        //Gli indici selezionati li metto visibili
        j=ind[i];
        t_risorse1[j].mostra=1;
        sprintf(stringa,"VIS:  %s",t_risorse1[j].nomeconv);
        ListBox_DeleteString(FAIVED,j);
        ListBox_InsertString(FAIVED,j,stringa);
    }
    HandVai(hwndDlg);
}

void soloved(HWND hwndDlg)
//Fa vedere nel riepilogo SOLO le risorse
//selezionate
{
    char stringa[401];

    int i,j,k,ind[100];

    HWND FAIVED =  GetDlgItem(hwndDlg, ID_FAIVED);

    //Prima prendo gli indici delle risorse
    //selezionate nella casella
    k=SendMessage(FAIVED,LB_GETSELITEMS,100,(LPARAM)ind);
    i=ListBox_GetCount(FAIVED);
    for(j=0;j<i;j++)
    {
        //Metto prima tutti gli indici non visibili
        t_risorse1[j].mostra=0;
        sprintf(stringa,"NONVIS:  %s",t_risorse1[j].nomeconv);
        ListBox_DeleteString(FAIVED,j);
        ListBox_InsertString(FAIVED,j,stringa);
    }
    for(i=0;i<k;i++)
    {
        //Gli indici selezionati li metto visibili
        j=ind[i];
        t_risorse1[j].mostra=1;
        sprintf(stringa,"VIS:  %s",t_risorse1[j].nomeconv);
        ListBox_DeleteString(FAIVED,j);
        ListBox_InsertString(FAIVED,j,stringa);
    }
    HandVai(hwndDlg);
}

void tuttevis(HWND hwndDlg)
//Fa vedere nel riepilogo TUTTE le risorse
{
    char stringa[401];

    int i,k;

    HWND FAIVED =  GetDlgItem(hwndDlg, ID_FAIVED);

    k=ListBox_GetCount(FAIVED);
    for(i=0;i<k;i++)
    {
        t_risorse1[i].mostra=1;
        sprintf(stringa,"VIS:  %s",t_risorse1[i].nomeconv);
        ListBox_DeleteString(FAIVED,i);
        ListBox_InsertString(FAIVED,i,stringa);
    }
    HandVai(hwndDlg);
}

void nonved(HWND hwndDlg)
//Non fa vedere nel riepilogo le risorse
//selezionate
{
    char stringa[401];

    int i,j,k,ind[100];

    HWND FAIVED =  GetDlgItem(hwndDlg, ID_FAIVED);

    //Prima prendo gli indici delle risorse
    //selezionate nella casella
    k=SendMessage(FAIVED,LB_GETSELITEMS,100,(LPARAM)ind);
    for(i=0;i<k;i++)
    {
        //Gli indici selezionati li metto non visibili
        j=ind[i];
        t_risorse1[j].mostra=0;
        sprintf(stringa,"NONVIS:  %s",t_risorse1[j].nomeconv);
        ListBox_DeleteString(FAIVED,j);
        ListBox_InsertString(FAIVED,j,stringa);
    }
    HandVai(hwndDlg);
}

void solononved(HWND hwndDlg)
//Non fa vedere nel riepilogo SOLO le risorse
//selezionate
{
    char stringa[401];

    int i,j,k,ind[100];

    HWND FAIVED =  GetDlgItem(hwndDlg, ID_FAIVED);

    //Prima prendo gli indici delle risorse
    //selezionate nella casella
    k=SendMessage(FAIVED,LB_GETSELITEMS,100,(LPARAM)ind);
    i=ListBox_GetCount(FAIVED);
    for(j=0;j<i;j++)
    {
        //Metto prima tutti gli indici visibili
        t_risorse1[j].mostra=1;
        sprintf(stringa,"VIS:  %s",t_risorse1[j].nomeconv);
        ListBox_DeleteString(FAIVED,j);
        ListBox_InsertString(FAIVED,j,stringa);
    }
    for(i=0;i<k;i++)
    {
        //Gli indici selezionati li metto non visibili
        j=ind[i];
        t_risorse1[j].mostra=0;
        sprintf(stringa,"NONVIS:  %s",t_risorse1[j].nomeconv);
        ListBox_DeleteString(FAIVED,j);
        ListBox_InsertString(FAIVED,j,stringa);
    }
    HandVai(hwndDlg);
}

int cancella(HWND hwndDlg,char *nome_file,long int num_g,char *datastring)
{
    char stringa[401];
    int i,k,num_ops,nrighe,match;
    long int numg_fa;
    FILE *fil,*tmp;

    if(!(fil=fopen(nome_file,"r")))
    {
        //Manca il file su cui fare la cancellazione: errore
        MessageBox(hwndDlg,"Manca il file dati: canc1","Problema!",MB_ICONEXCLAMATION | MB_OK);
        return -118;
    }
    fclose(fil);
    if((k=controllafiledati(nome_file))<0)
        //Si e' verificato un errore nel file dati
        return k;
    //Se sono qui sono sicuro che il file dati e' a posto
    //posso procedere senza controllare l'integrita'
    fil=fopen(nome_file,"r");
    tmp=fopen("temp","w");
    numg_fa=0;
    //Entro nella fase di aggiornamento dei dati
    do
    {
        if(!fgets(stringa,400,fil))
        {
            //Ho raggiunto la fine del file senza trovare
            //la stringa: errore
            MessageBox(hwndDlg,
            "Non trovo la stringa nel file di cancellazione: canc2","Problema!",MB_ICONEXCLAMATION | MB_OK);
            fclose(fil);
            fclose(tmp);
            return -1;
        }
        sscanf(stringa,"%ld,%d,%d\n",&numg_fa,&num_ops,&nrighe);
        if(numg_fa<num_g)
        {
            //Sono a un giorno precedente a quello desiderato
            fprintf(tmp,"%s",stringa);
            for(i=0;i<num_ops+nrighe;i++)
            {
                fgets(stringa,400,fil);
                //Ricopio la riga
                fprintf(tmp,"%s",stringa);
            }
        }
    }
    while(numg_fa<num_g);
    if(numg_fa>num_g)
    {
        //Se questo accade:
        //Non ho trovato il giorno di cancellazione:
        //errore
        MessageBox(hwndDlg,"Il giorno della cancellazione non esiste nel file: canc7",
        "Problema!",
        MB_ICONEXCLAMATION | MB_OK);
        fclose(fil);
        fclose(tmp);
        return -1;
    }
    //Se sono qua significa che numg_fa==num_g
    //Ho raggiunto il giorno dell'operazione da cancellare
    if(num_ops)
        num_ops--;
    if(num_ops||nrighe)
        //Solo se ci sono operazioni rimanenti o note
        //scrivo il giorno di interesse
        fprintf(tmp,"%ld,%d,%d\n",numg_fa,num_ops,nrighe);
    match=0;
    for(i=0;i<num_ops+1;i++)
    {
        fgets(stringa,400,fil);
        if(strcmp(datastring,stringa)!=0)
        {
            //Solo se non e' la stringa che voglio cancellare
            //posso copiarla in temp
            if((i==num_ops)&&(!match))
            {
                //Non ho trovato la stringa da cancellare: errore
                MessageBox(hwndDlg,"Non trovo nel file la stringa da cancellare:canc10",
                "Problema!",
                MB_ICONEXCLAMATION | MB_OK);
                fclose(fil);
                fclose(tmp);
                return -1;
            }
            //Se sono qui posso ricopiare in temp la stringa
            fprintf(tmp,"%s",stringa);
        }
        else
            match=1;
    }
    //Scrivo le note
    for(i=0;i<nrighe;i++)
    {
        fgets(stringa,400,fil);
        //Ricopio la riga
        fprintf(tmp,"%s",stringa);
    }
    //A questo punto i nuovi dati sono dentro
    //Ricopio quello che segue (se c'e') e finisco
    while(!feof(fil))
    {
        fgets(stringa,400,fil);
        if(!feof(fil))
            fprintf(tmp,"%s",stringa);
    }
    fclose(fil);
    fclose(tmp);
    return 0;
  }

void HandCancella(HWND hwndDlg)
//Cancella la voce selezionata nel box di riepilogo
{
    char stringa[401],stringa1[401],nomeconv[27],mot[100],cat[100],slog[20],
    nomefile_in[30],nomefile_out[30];
    int k,a,index,viene_da,va_in,n_lpa,n_lpb;
    long int pippo,numg_in,numg_out;
    double importo;

    HWND RIEPILO = GetDlgItem(hwndDlg, C_RIEPILOG);


    scrivilog(1,(char*)"Forse Mouse Cancella");
    //Trovo l'anno relativo alla data di riepilogo
    pippo= numg_riepilogo/100;
    a=pippo/100;
    index=ListBox_GetCurSel(RIEPILO);
    if(index<0)
    {
        //Non era selezionato nulla: ritorno
        scrivilog(1,(char*)"HandCancella: nulla ritorno");
        return;
    }
    k=ListBox_GetCurSel(RIEPILO);
    ListBox_GetText(RIEPILO,k,stringa);
    if(sscanf(stringa,"%[!-~ ‘’@ςΰωθιμ°η§]\t| %lf\t| %[!-~ ‘’@ςΰωθιμ°η§]\t| %[!-~ ‘’@ςΰωθιμ°η§]",nomeconv,&importo,mot,cat)!=4)
    {
        //Per qualche motivo la stringa e' corrotta: Errore
        //(Puo' succedere?)
        MessageBox(hwndDlg,"Anomalia stringa:p1","Problema!",MB_ICONEXCLAMATION | MB_OK);
        scrivilog(1,(char*)"HandCancella: Anomalia stringa:p1");
        return;
    }
    //Ricavo le informazioni aggiuntive
    //della voce selezionata
    viene_da=tabella_movimenti[k].posiz_da;
    va_in=tabella_movimenti[k].posiz_in;
    n_lpa=tabella_movimenti[k].numop_a;
    n_lpb=tabella_movimenti[k].numop_b;
    //Passo preliminare per calcolare l'anno
    //dell'operazione collegata (se c'e')
    pippo= tabella_movimenti[k].numg_b/100;
    if (importo>=0)
    {
        //Ho selezionato un'entrata: nomefile_in
        //e' quello ricavato da numg_riepilogo
        numg_in=numg_riepilogo;
        numg_out=tabella_movimenti[k].numg_b;
        sprintf(nomefile_in,"%d.bud",a);
        //Adesso ricavo l'anno dell'operazione di uscita
        //e ci formo nomefile_out
        a=pippo/100;
        sprintf(nomefile_out,"%d.bud",a);
        //stringa da cercare ed eliminare nel file di entrata
        sprintf(stringa,"%d,+%.2lf,%s\t,%s\t,%d,%d,%ld,%d\n",n_lpa,importo,mot,cat,viene_da,va_in,numg_out,n_lpb);
        scrivilog(1,(char*)"HandCancella: cancella 1 - nomefile_in, numg_in, stringa:");
        scrivilog(0,nomefile_in);
        scrivilog(0,ltoa(numg_in,slog,10));
        scrivilog(0,stringa);
        if((cancella(hwndDlg,nomefile_in,numg_in,stringa))<0)
        {
            //Doveva esserci un'entrata, ma nel file non c'e':
            //errore
            MessageBox(hwndDlg,"Anomalia: il movimento in entrata da cancellare non esiste nel file",
            "Problema!",MB_ICONEXCLAMATION | MB_OK);
            scrivilog(1,(char*)"HandCancella: Anomalia: il movimento in entrata da cancellare non esiste nel file");
            return;
        }
        //Tutto e' andato bene,
        //nomefile_in diventa backin e temp
        //diventa nomefile_in. Lo devo fare qui
        //nel caso che nomefile_out=nomefile_in.
        //In caso di errore devo poter ripristinare il vecchio file.
        rename(nomefile_in,"backin");
        rename("temp",nomefile_in);
        if(viene_da)
        {
            //Solo se c'e' un'uscita formo la seconda stringa
            //per il file di uscita
            sprintf(stringa1,"%d,-%.2lf,%s\t,%s\t,%d,%d,%ld,%d\n",n_lpb,importo,mot,cat,viene_da,va_in,numg_in,n_lpa);
            scrivilog(1,(char*)"HandCancella: cancella 2 - nomefile_out, numg_out, stringa1:");
            scrivilog(0,nomefile_out);
            scrivilog(0,ltoa(numg_out,slog,10));
            scrivilog(0,stringa1);
            if((cancella(hwndDlg,nomefile_out,numg_out,stringa1))<0)
            {
                //Doveva esserci un'uscita, ma nel file non c'e':
                //errore
                MessageBox(hwndDlg,"Anomalia: il movimento in uscita da cancellare non esiste nel file",
                "Problema!",MB_ICONEXCLAMATION | MB_OK);
                //Prima di uscire ripristino il file gia' modificato
                //nella sua versione originale
                remove(nomefile_in);
                rename("backin",nomefile_in);
                scrivilog(1,(char*)"HandCancella: Anomalia: il movimento in uscita da cancellare non esiste nel file");
                return;
            }
            //Tutto e' andato bene, temp diventa nomefile_out
            remove(nomefile_out);
            rename("temp",nomefile_out);
        }
        remove("backin");
    }
    else
    {
        //Ho selezionato un'uscita: nomefile_out
        //e' quello ricavato da numg_riepilogo
        numg_out=numg_riepilogo;
        numg_in=tabella_movimenti[k].numg_b;
        sprintf(nomefile_out,"%d.bud",a);
        //Adesso ricavo l'anno dell'operazione di entrata
        //e ci formo nomefile_in
        a=pippo/100;
        sprintf(nomefile_in,"%d.bud",a);
        importo=-importo;
        //stringa da cercare ed eliminare nel file di uscita
        sprintf(stringa,"%d,-%.2lf,%s\t,%s\t,%d,%d,%ld,%d\n",n_lpa,importo,mot,cat,viene_da,va_in,numg_in,n_lpb);
        scrivilog(1,(char*)"HandCancella: cancella 3 - nomefile_out, numg_out, stringa:");
        scrivilog(0,nomefile_out);
        scrivilog(0,ltoa(numg_out,slog,10));
        scrivilog(0,stringa);
        if((cancella(hwndDlg,nomefile_out,numg_out,stringa))<0)
        {
            //Doveva esserci un'uscita, ma nel file non c'e':
            //errore
            MessageBox(hwndDlg,"Anomalia: il movimento in uscita da cancellare non esiste nel file",
            "Problema!",MB_ICONEXCLAMATION | MB_OK);
            scrivilog(1,(char*)"HandCancella: Anomalia: il movimento in uscita da cancellare non esiste nel file");
            return;
        }
        //Tutto e' andato bene,
        //nomefile_out diventa backout e temp
        //diventa nomefile_out. Lo devo fare qui
        //nel caso che nomefile_in=nomefile_out.
        //In caso di errore devo poter ripristinare il vecchio file.
        rename(nomefile_out,"backout");
        rename("temp",nomefile_out);
        if(va_in)
        {
            //Solo se c'e' un'entrata formo la seconda stringa
            //per il file di entrata
            sprintf(stringa1,"%d,+%.2lf,%s\t,%s\t,%d,%d,%ld,%d\n",n_lpb,importo,mot,cat,viene_da,va_in,numg_out,n_lpa);
            scrivilog(1,(char*)"HandCancella: cancella 4 - nomefile_in, numg_in, stringa1:");
            scrivilog(0,nomefile_in);
            scrivilog(0,ltoa(numg_in,slog,10));
            scrivilog(0,stringa1);
            if((cancella(hwndDlg,nomefile_in,numg_in,stringa1))<0)
            {
                //Doveva esserci un'entrata, ma nel file non c'e':
                //errore
                MessageBox(hwndDlg,"Anomalia: il movimento in uscita da cancellare non esiste nel file",
                "Problema!",MB_ICONEXCLAMATION | MB_OK);
                //Prima di uscire ripristino il file gia' modificato
                //nella sua versione originale
                remove(nomefile_out);
                rename("backout",nomefile_out);
                scrivilog(1,(char*)"HandCancella: Anomalia: il movimento in uscita da cancellare non esiste nel file");
                return;
            }
            //Tutto e' andato bene, temp diventa nomefile_in
            remove(nomefile_in);
            rename("temp",nomefile_in);
        }
        remove("backout");
    }
    //Aggiorno la finestra di riepilogo
    riepiloga(hwndDlg,numg_riepilogo);
}

void SimpleSel(HWND hwndDlg)
{
    char stringa[401],nomeconv[27],mot[100],cat[100];
    int g,m,a,i,j,k;
    long int pippo;
    double importo;

    HWND IMPO =  GetDlgItem(hwndDlg, IMPORTO);
    HWND MOTI =  GetDlgItem(hwndDlg, MOTIVO);
    HWND CATE =  GetDlgItem(hwndDlg, CATEG);
    HWND EGior =  GetDlgItem(hwndDlg, ID_GEN);
    HWND EMes =  GetDlgItem(hwndDlg, ID_MEN);
    HWND EAnn =  GetDlgItem(hwndDlg, ID_AEN);
    HWND UGior =  GetDlgItem(hwndDlg, ID_GUSC);
    HWND UMes =  GetDlgItem(hwndDlg, ID_MUSC);
    HWND UAnn =  GetDlgItem(hwndDlg, ID_AUSC);
    HWND DESTINA =  GetDlgItem(hwndDlg, C_BANCA_IN);
    HWND PROVEN =  GetDlgItem(hwndDlg, C_BANCA_OUT);
    HWND RIEPILO =  GetDlgItem(hwndDlg, C_RIEPILOG);

    ComboBox_SetText(IMPO,"");
    ComboBox_SetText(MOTI,"");
    ComboBox_SetText(CATE,"");
    ComboBox_SetText(EGior,"");
    ComboBox_SetText(EMes,"");
    ComboBox_SetText(EAnn,"");
    ComboBox_SetText(UGior,"");
    ComboBox_SetText(UMes,"");
    ComboBox_SetText(UAnn,"");
    ComboBox_SetText(DESTINA,"");
    ComboBox_SetText(PROVEN,"");
    //Catturo la stringa selezionata e tolgo la selezione

    k=ListBox_GetCurSel(RIEPILO);
    ListBox_GetText(RIEPILO,k,stringa);
    ListBox_SetCurSel(RIEPILO,-1);
    if(sscanf(stringa,"%[!-~ ‘’@ςΰωθιμ°η§]\t| %lf\t| %[!-~ ‘’@ςΰωθιμ°η§]\t| %[!-~ ‘’@ςΰωθιμ°η§]",nomeconv,&importo,mot,cat)!=4)
    {
        //Per qualche motivo la stringa e' corrotta: Errore
        //(Puo' succedere?)
        MessageBox(hwndDlg,"Anomalia stringa:p1",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        scrivilog(1,(char*)"SimpleSel: Anomalia stringa:p1");
        return;
    }
    if(importo>=0)
    {
        //E' un'entrata: attivo il pulsante di entrata
        //e disattivo quello di uscita
        CheckRadioButton(hwndDlg,PULSENTR,PULSUSC,PULSENTR);
        entra=1;
        sprintf(stringa,"%.2lf",importo);
        //Inserisco importo, motivo e categoria
        //nei campi opportuni

        ComboBox_SetText(IMPO,stringa);
        ComboBox_SetText(MOTI,mot);
        ComboBox_SetText(CATE,cat);
        //Recupero il numero risorsa dell'entrata
        j=tabella_movimenti[k].posiz_in;
        i=0;
        while(t_risorse1[i].posiz<j)
        {
            i++;
        }
        ComboBox_SetText(DESTINA,t_risorse1[i].nomeconv);
        //Determino la data di entrata e la inserisco
        //nei campi appositi
        g= tabella_movimenti[k].numg_a%100;
        pippo= tabella_movimenti[k].numg_a/100;
        m= pippo%100;
        a=pippo/100;
        sprintf(stringa,"%02d",g);
        ComboBox_SetText(EGior,stringa);
        sprintf(stringa,"%02d",m);
        ComboBox_SetText(EMes,stringa);
        sprintf(stringa,"%04d",a);
        ComboBox_SetText(EAnn,stringa);
        //Aggiorno la lista delle risorse di entrata
        //per il giorno in questione
        caricaLista(hwndDlg,ID_GEN,ID_MEN,ID_AEN,C_BANCA_IN);
        //Decido se c'e' un'uscita: se il numero risorsa
        //e' 0 non c'e'
        if(!(j=tabella_movimenti[k].posiz_da))
            return;
        //C'e' un'uscita: trovo il nome della risorsa
        i=0;
        while(t_risorse1[i].posiz<j)
        {
            i++;
        }
        ComboBox_SetText(PROVEN,t_risorse1[i].nomeconv);
        //Determino la data di uscita e la inserisco
        //nei campi appositi
        g= tabella_movimenti[k].numg_b%100;
        pippo= tabella_movimenti[k].numg_b/100;
        m= pippo%100;
        a=pippo/100;
        sprintf(stringa,"%02d",g);
        ComboBox_SetText(UGior,stringa);
        sprintf(stringa,"%02d",m);
        ComboBox_SetText(UMes,stringa);
        sprintf(stringa,"%04d",a);
        ComboBox_SetText(UAnn,stringa);
        //Aggiorno la lista delle risorse di uscita
        //per il giorno in questione
        caricaLista(hwndDlg,ID_GUSC,ID_MUSC,ID_AUSC,C_BANCA_OUT);
    }
    else
    {
        //E' un'uscita: attivo il pulsante di uscita
        //e disattivo quello di entrata
        CheckRadioButton(hwndDlg,PULSENTR,PULSUSC,PULSUSC);
        entra=0;
        //Cambio segno a importo per farlo diventare positivo
        //(ora e' selezionato il pulsante di uscita)
        importo=-importo;
        sprintf(stringa,"%.2lf",importo);
        //Inserisco importo, motivo e categoria
        //nei campi opportuni
        ComboBox_SetText(IMPO,stringa);
        ComboBox_SetText(MOTI,mot);
        ComboBox_SetText(CATE,cat);
        //Recupero il numero risorsa dell'uscita
        j=tabella_movimenti[k].posiz_da;
        i=0;
        while(t_risorse1[i].posiz<j)
        {
            i++;
        }
        ComboBox_SetText(PROVEN,t_risorse1[i].nomeconv);
        //Determino la data di uscita e la inserisco
        //nei campi appositi
        g= tabella_movimenti[k].numg_a%100;
        pippo= tabella_movimenti[k].numg_a/100;
        m= pippo%100;
        a=pippo/100;
        sprintf(stringa,"%02d",g);
        ComboBox_SetText(UGior,stringa);
        sprintf(stringa,"%02d",m);
        ComboBox_SetText(UMes,stringa);
        sprintf(stringa,"%04d",a);
        ComboBox_SetText(UAnn,stringa);
        //Aggiorno la lista delle risorse di uscita
        //per il giorno in questione
        caricaLista(hwndDlg,ID_GUSC,ID_MUSC,ID_AUSC,C_BANCA_OUT);
        //Decido se c'e' un'entrata: se il numero risorsa
        //e' 0 non c'e'
        if(!(j=tabella_movimenti[k].posiz_in))
            return;
        //C'e' un'entrata: trovo il nome della risorsa
        i=0;
        while(t_risorse1[i].posiz<j)
        {
            i++;
        }
        ComboBox_SetText(DESTINA,t_risorse1[i].nomeconv);
        //Determino la data di entrata e la inserisco
        //nei campi appositi
        g= tabella_movimenti[k].numg_b%100;
        pippo= tabella_movimenti[k].numg_b/100;
        m= pippo%100;
        a=pippo/100;
        sprintf(stringa,"%02d",g);
        ComboBox_SetText(EGior,stringa);
        sprintf(stringa,"%02d",m);
        ComboBox_SetText(EMes,stringa);
        sprintf(stringa,"%04d",a);
        ComboBox_SetText(EAnn,stringa);
        //Aggiorno la lista delle risorse di entrata
        //per il giorno in questione
        caricaLista(hwndDlg,ID_GEN,ID_MEN,ID_AEN,C_BANCA_IN);
    }
}

void ComplexSel(HWND hwndDlg)
//Seleziona una stringa e la cancella dal box
//di riepilogo. Utile per cambiare
{
    int sele;

    HWND RIEPILO = GetDlgItem(hwndDlg, C_RIEPILOG);

    //Memorizzo la selezione
    sele=ListBox_GetCurSel(RIEPILO);
    //Tiro su la stringa selezionata (che si deseleziona)
    SimpleSel(hwndDlg);
    //Riseleziono la stringa e la cancello
    ListBox_SetCurSel(RIEPILO,sele);
    HandCancella(hwndDlg);
}

void HandNote(HWND hwndDlg)
//Mi manda alla pagina delle note. Quando
//esco scrivo il file
{
    char nomefile_in[30];
    int a;
    long int pippo;

    scrivilog(1,(char*)"Forse Mouse note");
    DialogBox(hInst, MAKEINTRESOURCE(dlgnote), hwndDlg, (DLGPROC)DlgNote);

    pippo= numg_riepilogo/100;
    a=pippo/100;
    sprintf(nomefile_in,"%d.bud",a);
    scrivinote(hwndDlg,nomefile_in,numg_riepilogo,nlnote);
    riepiloga(hwndDlg,numg_riepilogo);
    scrivilog(1,(char*)"HandNote: note end");

}

int scrivinote(HWND hwndDlg, char *nome_file,long int num_g,int nlnote)
//Questa funzione e' simile a scrivifile
//ma serve a scrivere le note e non i movimenti
{
    char stringa[401],stringa1[401];
    int i,k,fine,num_ops,nrighe;
    long int numg_fa;

    FILE *fil,*tmp,*ntt;

    if(!(fil=fopen(nome_file,"r")))
    {
        fil=fopen(nome_file,"w");
        fclose(fil);
    }
    fclose(fil);
    if((k=controllafiledati(nome_file))<0)
        //Si e' verificato un errore nel file dati
        return k;
    //Se sono qui sono sicuro che il file dati e' a posto
    //posso procedere senza controllare l'integrita'
    fil=fopen(nome_file,"r");
    tmp=fopen("temp","w");
    if(!(ntt=fopen("note.tmp","r")))
    {
        MessageBox(hwndDlg,
        "Manca il file 'note.tmp'",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        fclose(fil);
        fclose(tmp);
        return -1;
    }
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
        //Se sono qui sono a un giorno precedente a quello desiderato
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
        //e ho delle note da scrivere
        //Lo scrivo ex novo e poi il file finira'
        if(nlnote)
            //Solo se ho note da scrivere creo il giorno
            fprintf(tmp,"%ld,0,%d\n",num_g,nlnote);
        for(i=0;i<nlnote;i++)
        {
            //Leggo una riga da note.tmp
            if(!fgets(stringa,400,ntt))
            {
                //Mi aspettavo di leggere una stringa ma non la trovo: errore
                MessageBox(hwndDlg,
                "Anomalia nel file 'note.tmp'",
                "Problema!",MB_ICONEXCLAMATION | MB_OK);
                fclose(fil);
                fclose(tmp);
                fclose(ntt);
                return -1;
            }
            //Adesso posso scrivere la nuova stringa
            fputs(stringa,tmp);
        }
    }
    else if(numg_fa==num_g)
    {
        //Questo puo' accadere se:
        //Il giorno che voglio scrivere conteneva gia' dei dati
        //Qui devo controllare se num_ops o nlnote sono non nulli
        //per sapere se mettere il giorno
        //Aggiungo i nuovi dati, e poi gli appendo
        //i giorni successivi (se ci sono)
        if(num_ops||nlnote)
            //Solo se c'e' dato o note (ri)creo il giorno
            fprintf(tmp,"%ld,%d,%d\n",numg_fa,num_ops,nlnote);
        for(i=0;i<num_ops;i++)
        {
            fgets(stringa,400,fil);
            //Ricopio la riga
            fprintf(tmp,"%s",stringa);
        }
        //Raggiunta la fine dei dati del giorno
        //Faccio scorrere in avanti il file originale
        //fino alla fine del giorno
        for(i=0;i<nrighe;i++)
            fgets(stringa,400,fil);
        //Adesso posso scrivere le nuove note
        for(i=0;i<nlnote;i++)
        {
            //Leggo una riga da note.tmp
            if(!fgets(stringa,400,ntt))
            {
                //Mi aspettavo di leggere una stringa ma non la trovo: errore
                MessageBox(hwndDlg,
                "Anomalia nel file 'note.tmp'",
                "Problema!",MB_ICONEXCLAMATION | MB_OK);
                fclose(fil);
                fclose(tmp);
                fclose(ntt);
                return -1;
            }
            //Adesso posso scrivere la nuova stringa
            fputs(stringa,tmp);
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

        //Copio stringa in stringa1 perche' mi servira'
        //(e' l'intestazione del giorno che segue)
        strcpy(stringa1,stringa);
        if(nlnote)
            //Solo se ho note da scrivere creo il giorno
            fprintf(tmp,"%ld,0,%d\n",num_g,nlnote);
        for(i=0;i<nlnote;i++)
        {
            //Leggo una riga da note.tmp
            if(!fgets(stringa,400,ntt))
            {
                //Mi aspettavo di leggere una stringa ma non la trovo: errore
                MessageBox(hwndDlg,
                "Anomalia nel file 'note.tmp'",
                "Problema!",MB_ICONEXCLAMATION | MB_OK);
                fclose(fil);
                fclose(tmp);
                fclose(ntt);
                return -1;
            }
            //Adesso posso scrivere la nuova stringa
            fputs(stringa,tmp);
        }
        //Ricopio l'intestazione del giorno che segue
        fprintf(tmp,"%s",stringa1);
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
    fclose(ntt);
    remove(nome_file);
    rename("temp",nome_file);
    return 0;
}
