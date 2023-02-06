#include "dlgnote.h"

/*
Questo file contiene tutta la gestione relativa alla finestra di dialogo "dlgnote" (Finestra annotazioni del giorno)
*/

extern HINSTANCE hInst;


extern const char *gset[];

extern int nlnote;

extern long int numg_riepilogo;




BOOL CALLBACK DlgNote(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    char all[401];
    int i,g,m,a;
    long int numg,lenght,pippo;
    struct tm tcheck;
    FILE *nf;

    HWND PAD =  GetDlgItem(hwndDlg, NOTES);

    switch(uMsg)
    {
        case WM_INITDIALOG:
        {
            //le prossime 3 righe caricano l'icona nella window caption della finestra e ALT+TAB (non ho ben capito quest'ultima)
            HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(icona));
            SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
            SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

            numg=numg_riepilogo;
            Edit_SetText(PAD,"");
            g= numg%100;
            pippo= numg/100;
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
            if(!(nf=fopen("note.tmp","r")))
            {
                //Non trovo note.tmp: errore
                MessageBox(hwndDlg,"Manca il file 'note.tmp'",
                "Problema!",MB_ICONEXCLAMATION | MB_OK);
                scrivilog(1,(char*)"Setup note: Manca il file 'note.tmp'");
                EndDialog(hwndDlg, 0);
            }
            sprintf(all,"NOTE PER IL GIORNO: %02d-%02d-%04d, %s",g,m,a,gset[tcheck.tm_wday]);
            SetDlgItemText(hwndDlg,NOTE_DATA,all);
            scrivilog(1,all);
            if(nlnote)
            {
                for(i=0;i<nlnote;i++)
                {
                    if(!fgets(all,400,nf))
                    {
                        //Mi aspettavo di leggere una stringa ma non la trovo: errore
                        MessageBox(hwndDlg,"Anomalia nel file 'note.tmp'",
                        "Problema!",MB_ICONEXCLAMATION | MB_OK);
                        fclose(nf);
                        scrivilog(1,(char*)"Setup note: Anomalia nel file 'note.tmp'");
                        EndDialog(hwndDlg, 0);
                    }
                    sscanf(all,"%[!-~ ‘’@ςΰωθιμ°η§]\n",all);
                    scrivilog(0,all);
                    strcat(all,"\r\n");
                    //Per inserire una stringa alla fine
                    //prima determino la lunghezza di quanto gia' e' presente
                    //poi mi posiziono "selezionando" l'ultimo carattere (inizio=fine)
                    //quindi sostituisco la "selezione" con la stringa
                    lenght=Edit_GetTextLength(PAD);
                    Edit_SetSel(PAD,lenght,lenght);
                    Edit_ReplaceSel(PAD,all);
                    scrivilog(1,(char*)"Setup note eseguito");
                }
            }
            fclose(nf);

        }
        return TRUE;

        case WM_CLOSE:
        {
            if(MessageBox(hwndDlg,"Attenzione: le eventuali modifiche fatte andranno perse. Confermi annulla?",
            "Annulla e esci",MB_ICONEXCLAMATION | MB_YESNO)==IDNO)
            {
                //Ci ho ripensato, non vado avanti
                scrivilog(1,(char*)"Note: Attenzione: le eventuali modifiche fatte andranno perse. Confermi annulla? NO");
                break;
            }
            else
                //Si', sono sicuro, perdi le modifiche
                EndDialog(hwndDlg, 0);
        }
        return TRUE;

        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case NOTE_OK:
                    NoteHandOK(hwndDlg);
                    EndDialog(hwndDlg, 0);
                    break;

                case NOTE_ANN:
                    if(NoteHandAnnulla(hwndDlg))
                        EndDialog(hwndDlg, 0);
                    break;
            }

        }
        return TRUE;
    }
    return FALSE;
}

void NoteHandOK(HWND hwndDlg)
//Scrivo il testo corrente in note.tmp ed esco
//Alla fine avro' aggiornato note.tmp e nlnote
{
    char all[401];

    int i,k;

    FILE *nf;

    HWND PAD =  GetDlgItem(hwndDlg, NOTES);

    scrivilog(1,(char*)"Note: OK spinto. Nuove note:");
    nlnote=Edit_GetLineCount(PAD);
    sprintf(all,"nlnote=%d",nlnote);
    k=nlnote;
    if(k)
    {
        if(!(Edit_GetLine(PAD,k-1,all,190)))
            nlnote--;
        sprintf(all,"%d",nlnote);
        nf=fopen("note.tmp","w");
        for(i=0;i<k;i++)
        {
            if(Edit_GetLine(PAD,i,all,190))
            {
                fprintf(nf,"%s\n",all);
                scrivilog(0,all);
            }
            else
            {
                fprintf(nf,"\n");
                scrivilog(0,(char*)"");
            }
        }
        fclose(nf);
    }
}

int NoteHandAnnulla(HWND hwndDlg)
//Non modifico note.tmp ed esco dalle note.
//Prima chiedo conferma (e' una cosa sensata?)
{
    if(MessageBox(hwndDlg,"Attenzione: le eventuali modifiche fatte andranno perse. Confermi annulla?",
      "Annulla e esci",MB_ICONEXCLAMATION | MB_YESNO)==IDNO)
    {
        //Ci ho ripensato, non vado avanti
        scrivilog(1,(char*)"Note: Attenzione: le eventuali modifiche fatte andranno perse. Confermi annulla? NO");
        return 0;
    }
    //Si', sono sicuro, ritorno 1 e perdi le modifiche
    return 1;
}

