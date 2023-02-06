#include "dlgmain.h"

/*
Questo file contiene tutta la gestione relativa alla finestra di dialogo "DLG_MAIN" (Finestra principale)
*/

extern HINSTANCE hInst;

extern char vis,stringa[],stringa1[];

extern int i,r,errore,fatto,anno,numfonti,g,m,a,month[];


extern time_t curtime;
extern struct tm *data_ora;

extern struct _finddata_t ffblk,ffblk1;
//extern intptr_t hFile,hFile1;

extern FILE *mapp;


BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    char stringa[401];

    BOOL bRet, fSavedEnabledState;

    HWND hDlg;

    HWND GGior =  GetDlgItem(hwndDlg, ID_GG);
    HWND MMes =  GetDlgItem(hwndDlg, ID_MM);
    HWND AAnn =  GetDlgItem(hwndDlg, ID_AAAA);

    MSG msg;

    intptr_t hFile;

    switch(uMsg)
    {
        case WM_INITDIALOG: //All'apertura della finestra faccio partire il timer ID_TIMER
        {
            if((hFile=_findfirst64i32("budback",&ffblk))<0)
            {
                //Se non esiste la dir budback, la creo
                _findclose( hFile );
                if(mkdir("budback")<0)
                {
                    //In caso di errore...
                    _findclose( hFile );
                    MessageBox(hwndDlg,"Non riesco a creare la dir per il backup","Backup non verra' eseguito",MB_ICONEXCLAMATION | MB_OK);
                    //Chiudo la finestra di dialogo
                    EndDialog(hwndDlg, 0);
                }
            }
            else if(!(ffblk.attrib&_A_SUBDIR))
            {
                //budback esisteva ma non era una directory
                _findclose( hFile );
                MessageBox(hwndDlg,"trovato budback ma non e' una directory","Backup non verra' eseguito",MB_ICONEXCLAMATION | MB_OK);
                //Chiudo la finestra di dialogo
                EndDialog(hwndDlg, 0);
            }
            _findclose( hFile );

            //le prossime 3 righe caricano l'icona nella window caption della finestra e ALT+TAB (non ho ben capito quest'ultima)
            HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(icona));
            SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
            SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
            vis=0;
            SetTimer(hwndDlg,    // handle to main window
            ID_TIMER,            // timer identifier
            1000,                // 1-second interval
            (TIMERPROC) NULL);   // no timer callback
            time(&curtime);
            data_ora = localtime( &curtime );
            sprintf(stringa,"%02d",data_ora->tm_mday);
            ComboBox_SetText(GGior,stringa);
            sprintf(stringa,"%02d",data_ora->tm_mon+1);
            ComboBox_SetText(MMes,stringa);
            sprintf(stringa,"%02d",data_ora->tm_year+1900);
            ComboBox_SetText(AAnn,stringa);
            caricacampi(hwndDlg,ID_GG,ID_MM,ID_AAAA);
        }
        return TRUE;

        case WM_CLOSE:
        {
            EseguiBackup(hwndDlg);
            KillTimer(hwndDlg, ID_TIMER);  //Uccido il timer alla chiusura del dialogo
            EndDialog(hwndDlg, 0);
        }
        return TRUE;

        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case ID_DBASE:
                    HandPrelimin(hwndDlg);
                    break;
                case ID_OGGI:
                    if(HandOggi(hwndDlg))
                    {
                        //Se HandOggi mi da' il via libera ad aprire la finestra di dialogo
                        //procedo con la chiamata non modale, necessaria per poter usare gli acceleratori
                        hDlg=CreateDialog(hInst, MAKEINTRESOURCE(DLG_DAY), hwndDlg, (DLGPROC)DlgDay);
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
                case ID_CALC:
                    if(HandCalcola(hwndDlg))
                    {
                        scrivilog(1,(char*)"DlgMain  HandCalcola prima");
                        //Se HandCalcola mi da' il via libera ad aprire la finestra di dialogo
                        //procedo con la chiamata non modale, necessaria per poter usare gli acceleratori
                        hDlg=CreateDialog(hInst, MAKEINTRESOURCE(COMPUTE), hwndDlg, (DLGPROC)DlgCompute);
                        scrivilog(1,(char*)"DlgMain  HandCalcola dopo");
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
                case ID_GG:
                    HandValGior(hwndDlg,wParam);
                    break;
                case ID_MM:
                    HandValMes(hwndDlg,wParam);
                    break;
                case ID_AAAA:
                    HandValAnn(hwndDlg,wParam);
                    break;

            }
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
    }

    return FALSE;
}


void EseguiBackup(HWND hwndDlg)
{
    //Prima di chiudere normalmente
    //controlla l'integrita' dei dati ed esegue
    //una copia di backup
    //Mantengo fino a 5 copie di backup per file
    //La piu' recente e' quella con l'estensione .bk5
    //Ad ogni backup , la .bk1 va persa e sostituita
    //da .bk2, e cosi' via.
    //Pero':
    //Per tutti i files da backuppare voglio
    //vedere se sono diversi dall'ultima versione
    //salvata. Se sono identici non faccio il backup
    //evitando di perdere inutilmente le versioni
    //precedenti solo perche' ho aperto e richiuso
    //il programma senza cambiare niente.
    //Questo mi assicura anche che tutti i backup sono
    //diversi l'uno dall'altro

    char stringa[401],stringa1[401];

    intptr_t hFile,hFile1;

    int a,go;


    if((hFile=_findfirst64i32("basedata.dat",&ffblk))>0)
    {
        //Se esiste basedata.dat ne controlla l'integrita'
        _findclose( hFile );
        if((r=controllabasedata())<0)
        {
            //In caso di errore...
            sprintf(stringa,"Anomalia in basedata.dat per il seguente errore: %d",r);
            MessageBox(hwndDlg,stringa,"Backup non verra' eseguito",MB_ICONEXCLAMATION | MB_OK);
            //Chiudo la finestra di dialogo
            EndDialog(hwndDlg, 0);
        }
        //Tutto OK
        //Adesso cerco di capire se c'e'
        //differenza con l'ultimo backup
        chdir("budback");
        for(i=5;i>0;i--)
        //Questo ciclo cerca il backup piu' recente
        //appena lo trova esco dal ciclo, senno' il
        //ciclo si conclude
        {
            sprintf(stringa,"basedata.bk%d",i);
            if((hFile=_findfirst64i32(stringa,&ffblk1))>0)
            {
                //Se trovo il file esco dal ciclo e metto i negativa
                //per far capire che il ciclo non si e' concluso
                //naturalmente
                _findclose( hFile );
                i=-1;
                break;
            }
            _findclose( hFile );
        }
        if(i<0)
        {
            //i<0 vuol dire che ho trovato un file di backup
            //il cui nome e' in stringa
            go=compareFiles((char*)"tempor",stringa);
        }
        else
            //altrimenti i==0, non c'erano backup:
            //fanne uno di certo
            go=1;
        //go<0 e' un errore, ==0 significa files uguali
        //(non fare il backup), >0 significa fai il backup
        if(go<0)
        {
            //Si e' verificato un errore (improbabile arrivare qui)
            sprintf(stringa,"Problema nel confronto files n: %d",go);
            MessageBox(hwndDlg,stringa,"Backup di questo file non verra' eseguito",MB_ICONEXCLAMATION | MB_OK);
            remove("tempor");
        }
        else if(!go)
            //Non c'e' da fare backup. Rimuovo tempor
            remove("tempor");
        else if(go>0)
        {
            //Ho il via libera al backup
            //Se c'e' budback\basedata.bk2, lo faccio diventare
            //budback\basedata.bk1
            if((hFile=_findfirst64i32("basedata.bk2",&ffblk1))>0)
            {
                _findclose( hFile );
                //Controllo se c'e' la versione piu' vecchia:
                //Se c'e' devo cancellarla perche' senno' il
                //rename successivo non mi funziona. Tutte le versioni
                //successive andranno poi bene automaticamente
                if((hFile=_findfirst64i32("basedata.bk1",&ffblk1))>0)
                {
                    _findclose( hFile );
                    remove("basedata.bk1");
                }
                _findclose( hFile );
                rename("basedata.bk2","basedata.bk1");
            }
            _findclose( hFile );
            //Se c'e'  budback\basedata.bk3, lo faccio diventare
            //budback\basedata.bk2
            if((hFile=_findfirst64i32("basedata.bk3",&ffblk1))>0)
            {
                _findclose( hFile );
                rename("basedata.bk3","basedata.bk2");
            }
            _findclose( hFile );
            //Se c'e' budback\basedata.bk4, lo faccio diventare
            //budback\basedata.bk3
            if((hFile=_findfirst64i32("basedata.bk4",&ffblk1))>0)
            {
                _findclose( hFile );
                rename("basedata.bk4","basedata.bk3");
            }
            _findclose( hFile );
            //Se c'e' budback\basedata.bk5, lo faccio diventare
            //budback\basedata.bk4
            if((hFile=_findfirst64i32("basedata.bk5",&ffblk1))>0)
            {
                _findclose( hFile );
                rename("basedata.bk5","basedata.bk4");
            }
            _findclose( hFile );
            //Alla fine budback\tempor diventa budback\basedata.bk5
            rename("tempor","basedata.bk5");
        }
        chdir("..");
    }
    //cerco il primo file di dati
    fatto=1;
    if((hFile1=_findfirst64i32("*.bud",&ffblk))>0)
    {
        sprintf(stringa,"%s",ffblk.name);
        //se lo trovo metto fatto=0 (mi serve per usare _findnext piu' avanti)
        fatto=0;
    }
    while(!fatto)
    {
        //ho trovato un file di tipo *.bud
        if(sscanf(stringa,"%d.bud",&a)==1)
        {
            //Solo se e' del tipo "anno.bud"
            if((r=controllafiledati(stringa))<0)
            {
                //In caso di errore...
                sprintf(stringa,"Anomalia in %d.bud per il seguente errore: %d",a,r);
                MessageBox(hwndDlg,stringa,"Backup non verra' eseguito",MB_ICONEXCLAMATION | MB_OK);
                break;
            }
            //Tutto OK.
            //Adesso cerco di capire se c'e' differenza con
            //l'ultimo backup
            chdir("budback");
            for(i=5;i>0;i--)
            //Questo ciclo cerca il backup piu' recente
            //appena lo trova esco dal ciclo, senno' il
            //ciclo si conclude
            {
                sprintf(stringa,"%d.bk%d",a,i);
                if((hFile=_findfirst64i32(stringa,&ffblk1))>0)
                {
                    //Se trovo il file esco dal ciclo e metto i negativa
                    //per far capire che il ciclo non si e' concluso
                    //naturalmente
                    _findclose( hFile );
                    i=-1;
                    break;
                }
                _findclose( hFile );
            }
            if(i<0)
                //i<0 vuol dire che ho trovato un file di backup
                //il cui nome e' in stringa
                go=compareFiles((char*)"tempor",stringa);
            else
                //altrimenti i==0, non c'erano backup:
                //fanne uno di certo
                go=1;
            //go<0 e' un errore, ==0 significa files uguali
            //(non fare il backup), >0 significa fai il backup
            if(go<0)
            {
                //Si e' verificato un errore (improbabile arrivare qui)
                sprintf(stringa,"Problema nel confronto files n: %d",go);
                MessageBox(hwndDlg,stringa,"Backup di questo file non verra' eseguito",MB_ICONEXCLAMATION | MB_OK);
                remove("tempor");
            }
            else if(!go)
                //Non c'e' da fare backup. Rimuovo tempor
                remove("tempor");
            else if(go>0)
            {
                //Ho il via libera al backup
                //Se c'e' budback\anno.bk2, lo faccio diventare
                //budback\anno.bk1
                sprintf(stringa,"%d.bk2",a);
                sprintf(stringa1,"%d.bk1",a);
                if((hFile=_findfirst64i32(stringa,&ffblk1))>0)
                {
                    //Controllo se c'e' la versione piu' vecchia:
                    //Se c'e' devo cancellarla perche' senno' il
                    //rename successivo non mi funziona. Tutte le versioni
                    //successive andranno poi bene automaticamente
                    _findclose( hFile );
                    if(!(hFile=_findfirst64i32(stringa1,&ffblk1)))
                    {
                        _findclose( hFile );
                        remove(stringa1);
                    }
                    _findclose( hFile );
                    rename(stringa,stringa1);
                }
                _findclose( hFile );
                //Se c'e' budback\anno.bk3, lo faccio diventare
                //budback\anno.bk2
                sprintf(stringa,"%d.bk3",a);
                sprintf(stringa1,"%d.bk2",a);
                if((hFile=_findfirst64i32(stringa,&ffblk1))>0)
                {
                    _findclose( hFile );
                    rename(stringa,stringa1);
                }
                _findclose( hFile );
                //Se c'e' budback\anno.bk4, lo faccio diventare
                //budback\anno.bk3
                sprintf(stringa,"%d.bk4",a);
                sprintf(stringa1,"%d.bk3",a);
                if((hFile=_findfirst64i32(stringa,&ffblk1))>0)
                {
                    _findclose( hFile );
                    rename(stringa,stringa1);
                }
                _findclose( hFile );
                //Se c'e' budback\anno.bk5, lo faccio diventare
                //budback\anno.bk4
                sprintf(stringa,"%d.bk5",a);
                sprintf(stringa1,"%d.bk4",a);
                if((hFile=_findfirst64i32(stringa,&ffblk1))>0)
                {
                   _findclose( hFile );
                    rename(stringa,stringa1);
                }
                _findclose( hFile );
                //Alla fine budback\tempor diventa budback\anno.bk5
                rename("tempor",stringa);
            }
            chdir("..");
        }
        //Cerco il prossimo "*.bud"
        fatto=_findnext64i32(hFile1,&ffblk);
        sprintf(stringa,"%s",ffblk.name);
    }
    _findclose( hFile1 );
    //Faccio il backup del log
    backuplog();
}

int HandOggi(HWND hwndDlg)
{
    HWND GGior =  GetDlgItem(hwndDlg, ID_GG);
    HWND MMes =  GetDlgItem(hwndDlg, ID_MM);
    HWND AAnn =  GetDlgItem(hwndDlg, ID_AAAA);

    int vero;
    char stringa[401];

    if(vis==1)
        return 0;
    if(!(mapp=fopen("basedata.dat","r")))
    {
        MessageBox(hwndDlg,"Mancano i dati preliminari",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        HandPrelimin(hwndDlg);
        return 0;
    }
    vero=0;
    numfonti=0;
    fgets(stringa,400,mapp);
    sscanf(stringa,"%d\n",&numfonti);
    fclose(mapp);
    if(!numfonti)
    {
        MessageBox(hwndDlg,"Mancano i dati preliminari",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        HandPrelimin(hwndDlg);
        return 0;
    }
    ComboBox_GetText(GGior,stringa,3);
    g=atoi(stringa);
    ComboBox_GetText(MMes,stringa,3);
    m=atoi(stringa);
    ComboBox_GetText(AAnn,stringa,5);
    a=atoi(stringa);
    //controlliamo se e' una data valida
    if(a>0)
    {
        //controlla se e' bisestile
        if(isbises(a))
            //se lo e': giorni di febbraio=29
            month[1]=29;
        else
            //se non lo e': giorni di febbraio=28
            month[1]=28;
        if((m>=1)&&(m<=12)&&(g>=1)&&(g<=month[m-1]))
            vero=1;
    }
    if(vero)
    {
        //la data e' valida
        //inizializzo i valori da passare alla finestra del giorno
        //ritorno 1 e permetto l'apertura della finestra
        return 1;
    }
    else
    {
        //la data non era valida
        //ripulisco tutto, immetto la data di oggi e non apro la finestra
        //in realta' qui non ci dovrei mai arrivare perche'
        //appena tolgo il focus dalla data per cliccare il bottone
        //la data si aggiusta al valore valido

        ComboBox_SetText(GGior,"");
        ComboBox_SetText(MMes,"");
        ComboBox_SetText(AAnn,"");
        time(&curtime);
        data_ora = localtime( &curtime );
        sprintf(stringa,"%02d",data_ora->tm_mday);
        ComboBox_SetText(GGior,stringa);
        sprintf(stringa,"%02d",data_ora->tm_mon+1);
        ComboBox_SetText(MMes,stringa);
        sprintf(stringa,"%02d",data_ora->tm_year+1900);
        ComboBox_SetText(AAnn,stringa);
    }
    return 0;
}

void HandPrelimin(HWND hwndDlg)
{
    if(vis==1)
        return;
    time(&curtime);
    data_ora = localtime( &curtime );
    anno=data_ora->tm_year+1900;
    DialogBox(hInst, MAKEINTRESOURCE(DIAFIELD), hwndDlg, (DLGPROC)DlgDiaField);
}


void HandValGior(HWND hwndDlg,WPARAM wParam)
{
    if(vis==1)
        return;
    if(HIWORD(wParam)==CBN_SETFOCUS)
    {
        caricacampi(hwndDlg,ID_GG,ID_MM,ID_AAAA);
    }
    if(HIWORD(wParam)==CBN_KILLFOCUS)
        caricacampi(hwndDlg,ID_GG,ID_MM,ID_AAAA);
}

void HandValMes(HWND hwndDlg,WPARAM wParam)
{
    if(vis==1)
        return;
    if(HIWORD(wParam)==CBN_SETFOCUS)
    {
        caricacampi(hwndDlg,ID_GG,ID_MM,ID_AAAA);
    }
    if(HIWORD(wParam)==CBN_KILLFOCUS)
        caricacampi(hwndDlg,ID_GG,ID_MM,ID_AAAA);
}

void HandValAnn(HWND hwndDlg,WPARAM wParam)
{
    if(vis==1)
        return;
    if(HIWORD(wParam)==CBN_SETFOCUS)
    {
        caricacampi(hwndDlg,ID_GG,ID_MM,ID_AAAA);
    }
    if(HIWORD(wParam)==CBN_KILLFOCUS)
        caricacampi(hwndDlg,ID_GG,ID_MM,ID_AAAA);
}

int HandCalcola(HWND hwndDlg)
{
    if(vis==1)
        return 0;
    else
        return 1;
}

