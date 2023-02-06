#include "diafield.h"

/*
Questo file contiene tutta la gestione relativa alla finestra di dialogo "DIAFIELD" (Gestione dati di base)
*/

extern HINSTANCE hInst;

extern char stringa[],nomeconv[],aperto[],chiuso[];

extern int i,j,k,nrighe,posiz,nanni,numfonti,newposiz,gg,mm,aa,anno;

extern double saldo;

extern FILE *mapp,*logg,*tmp;


BOOL CALLBACK DlgDiaField(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_INITDIALOG:
        {
            //le prossime 3 righe caricano l'icona nella window caption della finestra e ALT+TAB (non ho ben capito quest'ultima)
            HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(icona));
            SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
            SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

            aggiornaFile(hwndDlg);
        }
        return TRUE;

        case WM_CLOSE:
        {
            EndDialog(hwndDlg, 0);
        }
        return TRUE;

        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case ID_FINS:
                    HandInsert(hwndDlg);
                    break;
                case ID_FOK:
                    EndDialog(hwndDlg, 0);
                    break;
                case ID_FCAMB:
                    HandCambia(hwndDlg);
                    break;
                case ID_FCANC:
                    HandCanc(hwndDlg);
                    break;
                case ID_FANNUL:
                    HandAnnul(hwndDlg);
                    break;
                case ID_IDENT:
                    if(HIWORD(wParam)==CBN_SELCHANGE)
                        HandSelident(hwndDlg);
                    break;
                case ID_INIANNO:
                    if(HIWORD(wParam)==CBN_SELCHANGE)
                        HandMsgIniAnno(hwndDlg);
                    break;
                case ID_INIZIALE:
                    if(HIWORD(wParam)==CBN_SELCHANGE)
                        HandMsgIniziale(hwndDlg);
                    break;
                case ID_SCRISA:
                    HandScriviSaldo(hwndDlg);
                    break;


            }
        }
        return TRUE;
    }
    return FALSE;
}


void aggiornaFile(HWND hwndDlg)
{
    char stringa[401];

    HWND IDENTI =  GetDlgItem(hwndDlg, ID_IDENT);
    HWND POSTODI = GetDlgItem(hwndDlg, ID_POSTODI);
    HWND INIANNO = GetDlgItem(hwndDlg, ID_INIANNO);
    HWND INIZIALE = GetDlgItem(hwndDlg, ID_INIZIALE);

    newposiz=1;
    if(!(mapp=fopen("basedata.dat","r")))
    {
        mapp=fopen("basedata.dat","w");
        fprintf(mapp,"0\n");
        fclose(mapp);
        return;
    }

    ComboBox_ResetContent(IDENTI);
    ComboBox_ResetContent(POSTODI);
    ComboBox_ResetContent(INIANNO);
    ComboBox_ResetContent(INIZIALE);

    fgets(stringa,400,mapp);
    sscanf(stringa,"%d\n",&numfonti);

    for(i=0;i<numfonti;i++)
    {
        fgets(stringa,400,mapp);
        // nel vecchio programma lo sscanf era come mostrato qui sotto,
        // ma ora mi va in crash percio' ho eliminato la seconda serie di caratteri
        // che comunque non dovrebbero essere mai usati
        // sscanf(stringa,"%d,%[!-~ ΅-ÿ‘’]\n",&posiz,nomeconv);
        sscanf(stringa,"%d,%[!-~ ‘’@ςΰωθιμ°η§]\n",&posiz,nomeconv);
        fgets(stringa,400,mapp);
        fgets(stringa,400,mapp);
        fgets(stringa,400,mapp);
        sscanf(stringa,"%d,%d\n",&nrighe,&nanni);

        ComboBox_AddString(IDENTI,nomeconv);
        ComboBox_AddString(POSTODI,nomeconv);

        for(j=0;j<nrighe+nanni;j++)
            fgets(stringa,400,mapp);
        newposiz=posiz+1;
    }
    fclose(mapp);
}


void HandInsert(HWND hwndDlg)
{
    char stringa[401];

    HWND IDENTI =  GetDlgItem(hwndDlg, ID_IDENT);
    HWND GAPERTO = GetDlgItem(hwndDlg, ID_GAPERTO);
    HWND MAPERTO = GetDlgItem(hwndDlg, ID_MAPERTO);
    HWND AAPERTO = GetDlgItem(hwndDlg, ID_AAPERTO);
    HWND INIANNO = GetDlgItem(hwndDlg, ID_INIANNO);
    HWND GCHIUSO = GetDlgItem(hwndDlg, ID_GCHIUSO);
    HWND MCHIUSO = GetDlgItem(hwndDlg, ID_MCHIUSO);
    HWND ACHIUSO = GetDlgItem(hwndDlg, ID_ACHIUSO);
    HWND INIZIALE = GetDlgItem(hwndDlg, ID_INIZIALE);
    HWND ESTESE = GetDlgItem(hwndDlg, ID_ESTESO);

    if(!(mapp=fopen("basedata.dat","r")))
    {
        //Mancava basedata.dat
        //errore
        MessageBox(hwndDlg,"Manca il file 'basedata.dat'",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        return;
    }
    if(!ComboBox_GetTextLength(IDENTI))
    {
        //Il campo era vuoto
        //errore
        MessageBox(hwndDlg,"Manca il nome",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        fclose(mapp);
        return;
    }
    ComboBox_GetText(IDENTI,nomeconv,27);
    if(SendMessage(IDENTI,CB_FINDSTRINGEXACT,-1, (LPARAM) nomeconv)>=0)
    {
        //Il campo non era vuoto ma corrisponde a una voce esistente
        //errore
        MessageBox(hwndDlg,"Non si puo' dare un nome gia' esistente! Cambialo!",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        fclose(mapp);
        return;
    }
    tmp=fopen("base.tmp","w");
    fgets(stringa,400,mapp);
    sscanf(stringa,"%d",&numfonti);
    fprintf(tmp,"%d\n",numfonti+1);
    while(!feof(mapp))
    {
        fgets(stringa,400,mapp);
        if(!feof(mapp))
            fprintf(tmp,"%s",stringa);
    }
    fprintf(tmp,"%d,%s\n",newposiz,nomeconv);
    i=0;
    if(ComboBox_GetTextLength(GAPERTO))
    {
        ComboBox_GetText(GAPERTO,aperto,5);
        if((gg=atoi(aperto))>0)
            i++;
    }
    if(ComboBox_GetTextLength(MAPERTO))
    {
        ComboBox_GetText(MAPERTO,aperto,5);
        if((mm=atoi(aperto))>0)
            i++;
    }
    if(ComboBox_GetTextLength(AAPERTO))
    {
        ComboBox_GetText(AAPERTO,aperto,7);
        if((aa=atoi(aperto))>0)
            i++;
    }
    ComboBox_GetText(INIANNO,stringa,7);
    if((j=atoi(stringa))<=0)
    {
        MessageBox(hwndDlg,"Inserire anno saldo!",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        fclose(mapp);
        fclose(tmp);
        remove("base.dat");
        return;
    }
    if(i==3)
    {
        if(j<aa)
        {
            MessageBox(hwndDlg,"Anno saldo minore anno apertura!",
            "Problema!",MB_ICONEXCLAMATION | MB_OK);
            fclose(mapp);
            fclose(tmp);
            remove("base.dat");
            return;
        }
        fprintf(tmp,"%02d-%02d-%04d\n",gg,mm,aa);
    }
    else
        fprintf(tmp,"nil\n");
    i=0;
    if(ComboBox_GetTextLength(GCHIUSO))
    {
        ComboBox_GetText(GCHIUSO,chiuso,5);
        if((gg=atoi(chiuso))>0)
            i++;
    }
    if(ComboBox_GetTextLength(MCHIUSO))
    {
        ComboBox_GetText(MCHIUSO,chiuso,5);
        if((mm=atoi(chiuso))>0)
            i++;
    }
    if(ComboBox_GetTextLength(ACHIUSO))
    {
        ComboBox_GetText(ACHIUSO,chiuso,7);
        if((aa=atoi(chiuso))>0)
            i++;
    }
    if(i==3)
    {
        if(j>aa)
        {
            MessageBox(hwndDlg,"Anno saldo maggiore anno chiusura!",
            "Problema!",MB_ICONEXCLAMATION | MB_OK);
            fclose(mapp);
            fclose(tmp);
            remove("base.dat");
            return;
        }
        fprintf(tmp,"%02d-%02d-%04d\n",gg,mm,aa);
    }
    else
        fprintf(tmp,"nil\n");
    ComboBox_GetText(INIZIALE,stringa,14);
    k=sscanf(stringa,"%lf%s",&saldo,nomeconv);
    if(k!=1)
    {
        MessageBox(hwndDlg,"Saldo inserito non e' un numero valido",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        fclose(tmp);
        fclose(mapp);
        remove("base.tmp");
        return;
    }
    nanni=1;
    k=SendMessage(ESTESE,EM_GETLINECOUNT,0,0);
    Edit_GetLine(ESTESE,k-1,stringa,100);
    if(strlen(stringa))
        fprintf(tmp,"%d,%d\n",k,nanni);
    else
        fprintf(tmp,"%d,%d\n",--k,nanni);
    for(i=0;i<k;i++)
    {
        Edit_GetLine(ESTESE,i,stringa,100);
        if(strlen(stringa))
            fprintf(tmp,"%s\n",stringa);
        else
            fprintf(tmp,"\n");
    }
    for(i=0;i<nanni;i++)
    {
        ComboBox_GetText(INIANNO,stringa,7);
        fprintf(tmp,"%s,",stringa);
        ComboBox_GetText(INIZIALE,stringa,14);
        sscanf(stringa,"%lf",&saldo);
        fprintf(tmp,"%.2lf\n",saldo);
    }
    fclose(tmp);
    fclose(mapp);
    remove("basedata.dat");
    rename("base.tmp","basedata.dat");
    aggiornaFile(hwndDlg);
    ComboBox_SetText(GAPERTO,"");
    ComboBox_SetText(MAPERTO,"");
    ComboBox_SetText(AAPERTO,"");
    ComboBox_SetText(GCHIUSO,"");
    ComboBox_SetText(MCHIUSO,"");
    ComboBox_SetText(ACHIUSO,"");
    Edit_SetText(ESTESE,"");
    ComboBox_ResetContent(INIZIALE);
    ComboBox_ResetContent(INIANNO);
}

void HandCambia(HWND hwndDlg)
{
    char stringa[401];

    HWND IDENTI =  GetDlgItem(hwndDlg, ID_IDENT);
    HWND GAPERTO = GetDlgItem(hwndDlg, ID_GAPERTO);
    HWND MAPERTO = GetDlgItem(hwndDlg, ID_MAPERTO);
    HWND AAPERTO = GetDlgItem(hwndDlg, ID_AAPERTO);
    HWND INIANNO = GetDlgItem(hwndDlg, ID_INIANNO);
    HWND GCHIUSO = GetDlgItem(hwndDlg, ID_GCHIUSO);
    HWND MCHIUSO = GetDlgItem(hwndDlg, ID_MCHIUSO);
    HWND ACHIUSO = GetDlgItem(hwndDlg, ID_ACHIUSO);
    HWND INIZIALE = GetDlgItem(hwndDlg, ID_INIZIALE);
    HWND POSTODI = GetDlgItem(hwndDlg, ID_POSTODI);
    HWND ESTESE = GetDlgItem(hwndDlg, ID_ESTESO);

    if(!ComboBox_GetTextLength(IDENTI))
    {
        MessageBox(hwndDlg,"Manca il nome della risorsa!",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        return;
    }
    ComboBox_GetText(IDENTI,nomeconv,27);
    if(!ComboBox_GetTextLength(POSTODI))
    {
        MessageBox(hwndDlg,"Specificare 'Questa risorsa'",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        return;
    }
    ComboBox_GetText(POSTODI,nomeconv,27);

    if((k=SendMessage(POSTODI,CB_FINDSTRINGEXACT,-1, (LPARAM) nomeconv))<0)
    {
        //Sto provando a cambiare una risorsa non esistente
        //errore
        MessageBox(hwndDlg,"Il nome specificato in 'Questa risorsa' non corrisponde",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        return;
    }

    ComboBox_GetText(IDENTI,stringa,27);
    if((i=SendMessage(IDENTI,CB_FINDSTRINGEXACT,-1, (LPARAM) stringa))>=0)
        if(i!=k)
        {
            //Il nome che ho inserito nel campo IDENTI e' uguale a una risorsa diversa da quella
            //che voglio cambiare. Darei lo stesso nome a due risorse uguali
            //errore
            MessageBox(hwndDlg,"Stai provando a dare nomi uguali a risorse diverse!",
            "Problema!",MB_ICONEXCLAMATION | MB_OK);
            return;
        }
    if(!(mapp=fopen("basedata.dat","r")))
    {
        MessageBox(hwndDlg,"Manca il file 'basedata.dat'",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        return;
    }
    tmp=fopen("base.tmp","w");
    fgets(stringa,400,mapp);
    fprintf(tmp,"%s",stringa);
    for(i=0;i<k;i++)
    {
        fgets(stringa,400,mapp);
        fprintf(tmp,"%s",stringa);
        fgets(stringa,400,mapp);
        fprintf(tmp,"%s",stringa);
        fgets(stringa,400,mapp);
        fprintf(tmp,"%s",stringa);
        fgets(stringa,400,mapp);
        fprintf(tmp,"%s",stringa);
        sscanf(stringa,"%d,%d\n",&nrighe,&nanni);
        for(j=0;j<nrighe+nanni;j++)
        {
            fgets(stringa,400,mapp);
            fprintf(tmp,"%s",stringa);
        }
    }
    fgets(stringa,400,mapp);
    sscanf(stringa,"%d,%[!-~ ‘’@ςΰωθιμ°η§]\n",&posiz,nomeconv);
//    sscanf(stringa,"%d,%[!-~ ‘’]\n",&posiz,nomeconv);
    fgets(stringa,400,mapp);
    fgets(stringa,400,mapp);
    fgets(stringa,400,mapp);
    sscanf(stringa,"%d,%d\n",&nrighe,&nanni);
    for(j=0;j<nrighe+nanni;j++)
        fgets(stringa,400,mapp);
    ComboBox_GetText(IDENTI,nomeconv,27);
    fprintf(tmp,"%d,%s\n",posiz,nomeconv);
    i=0;

    if(ComboBox_GetTextLength(GAPERTO))
    {
        ComboBox_GetText(GAPERTO,aperto,5);
        if((gg=atoi(aperto))>0)
            i++;
    }
    if(ComboBox_GetTextLength(MAPERTO))
    {
        ComboBox_GetText(MAPERTO,aperto,5);
        if((mm=atoi(aperto))>0)
            i++;
    }
    if(ComboBox_GetTextLength(AAPERTO))
    {
        ComboBox_GetText(AAPERTO,aperto,7);
        if((aa=atoi(aperto))>0)
            i++;
    }

    k=SendMessage(INIANNO,CB_GETCOUNT,0,0);
    ComboBox_GetText(INIANNO,stringa,7);
    j=atoi(stringa);
    if((j<=0)&&(k<0))
    {
        MessageBox(hwndDlg,"Inserire anno saldo!",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        fclose(mapp);
        fclose(tmp);
        remove("base.dat");
        return;
    }
    if((i==3)&&(j>0))
    {
        if(j<aa)
        {
            MessageBox(hwndDlg,"Anno saldo minore anno apertura!",
            "Problema!",MB_ICONEXCLAMATION | MB_OK);
            fclose(mapp);
            fclose(tmp);
            remove("base.dat");
            return;
        }
        fprintf(tmp,"%02d-%02d-%04d\n",gg,mm,aa);
    }
    else if(i==3)
        fprintf(tmp,"%02d-%02d-%04d\n",gg,mm,aa);
    else
        fprintf(tmp,"nil\n");
    i=0;
    if(ComboBox_GetTextLength(GCHIUSO))
    {
        ComboBox_GetText(GCHIUSO,chiuso,5);
        if((gg=atoi(chiuso))>0)
            i++;
    }
    if(ComboBox_GetTextLength(MCHIUSO))
    {
        ComboBox_GetText(MCHIUSO,chiuso,5);
        if((mm=atoi(chiuso))>0)
            i++;
    }
    if(ComboBox_GetTextLength(ACHIUSO))
    {
        ComboBox_GetText(ACHIUSO,chiuso,7);
        if((aa=atoi(chiuso))>0)
            i++;
    }
    if((i==3)&&(j>0))
    {
        if(j>aa)
        {
            MessageBox(hwndDlg,"Anno saldo maggiore anno chiusura!",
            "Problema!",MB_ICONEXCLAMATION | MB_OK);
            fclose(mapp);
            fclose(tmp);
            remove("base.dat");
            return;
        }
        fprintf(tmp,"%02d-%02d-%04d\n",gg,mm,aa);
    }
    else if(i==3)
        fprintf(tmp,"%02d-%02d-%04d\n",gg,mm,aa);
    else
        fprintf(tmp,"nil\n");
    if(j>0)
    {
        ComboBox_GetText(INIZIALE,stringa,14);
        k=sscanf(stringa,"%lf%s",&saldo,nomeconv);
        if(k!=1)
        {
            MessageBox(hwndDlg,"Saldo inserito non e' un numero valido",
            "Problema!",MB_ICONEXCLAMATION | MB_OK);
            fclose(mapp);
            fclose(tmp);
            remove("base.dat");
            return;
        }
        ComboBox_GetText(INIANNO,stringa,7);

        if((k=SendMessage(INIANNO,CB_FINDSTRINGEXACT,-1, (LPARAM) stringa))>=0)
        {
            ComboBox_DeleteString(INIANNO,k);
            ComboBox_AddString(INIANNO,stringa);
            ComboBox_DeleteString(INIZIALE,k);
            ComboBox_GetText(INIZIALE,stringa,14);
            ComboBox_InsertString(INIZIALE,k,stringa);
        }
        else
        {
            k=SendMessage(INIANNO,CB_ADDSTRING,-0, (LPARAM) stringa);
            ComboBox_GetText(INIZIALE,stringa,14);
            ComboBox_InsertString(INIZIALE,k,stringa);
        }
    }
    nanni=SendMessage(INIANNO,CB_GETCOUNT,0,0);
    k=SendMessage(ESTESE,EM_GETLINECOUNT,0,0);
    Edit_GetLine(ESTESE,k-1,stringa,100);
    if(strlen(stringa))
        fprintf(tmp,"%d,%d\n",k,nanni);
    else
        fprintf(tmp,"%d,%d\n",--k,nanni);
    for(i=0;i<k;i++)
    {
        Edit_GetLine(ESTESE,i,stringa,100);
        if(strlen(stringa))
            fprintf(tmp,"%s\n",stringa);
        else
            fprintf(tmp,"\n");
    }
    for(i=0;i<nanni;i++)
    {
        ComboBox_GetLBText(INIANNO,i,stringa);
        fprintf(tmp,"%s,",stringa);
        ComboBox_GetLBText(INIZIALE,i,stringa);
        fprintf(tmp,"%s\n",stringa);
    }
    while(!feof(mapp))
    {
        fgets(stringa,400,mapp);
        if(!feof(mapp))
            fprintf(tmp,"%s",stringa);
    }
    fclose(mapp);
    fclose(tmp);
    remove("basedata.dat");
    rename("base.tmp","basedata.dat");
    ComboBox_GetText(IDENTI,nomeconv,27);
    k=SendMessage(IDENTI,CB_FINDSTRINGEXACT,-1, (LPARAM) nomeconv);
    aggiornaFile(hwndDlg);
    ComboBox_SetText(GAPERTO,"");
    ComboBox_SetText(MAPERTO,"");
    ComboBox_SetText(AAPERTO,"");
    ComboBox_SetText(GCHIUSO,"");
    ComboBox_SetText(MCHIUSO,"");
    ComboBox_SetText(ACHIUSO,"");
    ComboBox_SetText(INIZIALE,"");
    ComboBox_SetText(INIANNO,"");
    Edit_SetText(ESTESE,"");
    ComboBox_ResetContent(INIZIALE);
    ComboBox_ResetContent(INIANNO);
}

void HandCanc(HWND hwndDlg)
  {

    char stringa[401];

    HWND IDENTI =  GetDlgItem(hwndDlg, ID_IDENT);
    HWND GAPERTO = GetDlgItem(hwndDlg, ID_GAPERTO);
    HWND MAPERTO = GetDlgItem(hwndDlg, ID_MAPERTO);
    HWND AAPERTO = GetDlgItem(hwndDlg, ID_AAPERTO);
    HWND INIANNO = GetDlgItem(hwndDlg, ID_INIANNO);
    HWND GCHIUSO = GetDlgItem(hwndDlg, ID_GCHIUSO);
    HWND MCHIUSO = GetDlgItem(hwndDlg, ID_MCHIUSO);
    HWND ACHIUSO = GetDlgItem(hwndDlg, ID_ACHIUSO);
    HWND INIZIALE = GetDlgItem(hwndDlg, ID_INIZIALE);
    HWND ESTESE = GetDlgItem(hwndDlg, ID_ESTESO);

    if(!ComboBox_GetTextLength(IDENTI))
    {
        MessageBox(hwndDlg,"Inserire il nome",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        return;
    }
    ComboBox_GetText(IDENTI,nomeconv,27);
    if((k=SendMessage(IDENTI,CB_FINDSTRINGEXACT,-1, (LPARAM) nomeconv))<0)
    {
        MessageBox(hwndDlg,"Il nome non corrisponde",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        return;
    }
    if(!(mapp=fopen("basedata.dat","r")))
    {
        MessageBox(hwndDlg,"Manca il file 'basedata.dat'",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        return;
    }
    if(MessageBox(hwndDlg,
        "Se hai gia' usato questa risorsa potresti avere problemi. Sei sicuro?",
        "Attenzione!",MB_ICONEXCLAMATION | MB_YESNO)==IDNO)
    {
        fclose(mapp);
        return;
    }
    tmp=fopen("base.tmp","w");
    fgets(stringa,400,mapp);
    sscanf(stringa,"%d",&numfonti);
    fprintf(tmp,"%d\n",--numfonti);
    for(i=0;i<k;i++)
    {
        fgets(stringa,400,mapp);
        fprintf(tmp,"%s",stringa);
        fgets(stringa,400,mapp);
        fprintf(tmp,"%s",stringa);
        fgets(stringa,400,mapp);
        fprintf(tmp,"%s",stringa);
        fgets(stringa,400,mapp);
        fprintf(tmp,"%s",stringa);
        sscanf(stringa,"%d,%d\n",&nrighe,&nanni);
        for(j=0;j<nrighe+nanni;j++)
        {
            fgets(stringa,400,mapp);
            fprintf(tmp,"%s",stringa);
        }
    }
    fgets(stringa,400,mapp);
    fgets(stringa,400,mapp);
    fgets(stringa,400,mapp);
    fgets(stringa,400,mapp);
    sscanf(stringa,"%d,%d\n",&nrighe,&nanni);
    for(j=0;j<nrighe+nanni;j++)
        fgets(stringa,400,mapp);
    for(i=k;i<numfonti;i++)
    {
        fgets(stringa,400,mapp);
        fprintf(tmp,"%s",stringa);
        fgets(stringa,400,mapp);
        fprintf(tmp,"%s",stringa);
        fgets(stringa,400,mapp);
        fprintf(tmp,"%s",stringa);
        fgets(stringa,400,mapp);
        fprintf(tmp,"%s",stringa);
        sscanf(stringa,"%d,%d\n",&nrighe,&nanni);
        for(j=0;j<nrighe+nanni;j++)
        {
            fgets(stringa,400,mapp);
            fprintf(tmp,"%s",stringa);
        }
    }
    fclose(mapp);
    fclose(tmp);
    remove("basedata.dat");
    rename("base.tmp","basedata.dat");
    aggiornaFile(hwndDlg);

    ComboBox_SetText(GAPERTO,"");
    ComboBox_SetText(MAPERTO,"");
    ComboBox_SetText(AAPERTO,"");
    ComboBox_SetText(GCHIUSO,"");
    ComboBox_SetText(MCHIUSO,"");
    ComboBox_SetText(ACHIUSO,"");
    ComboBox_SetText(INIZIALE,"");
    ComboBox_SetText(INIANNO,"");
    Edit_SetText(ESTESE,"");
    ComboBox_ResetContent(INIZIALE);
    ComboBox_ResetContent(INIANNO);
}

void HandAnnul(HWND hwndDlg)
{
    HWND IDENTI =  GetDlgItem(hwndDlg, ID_IDENT);
    HWND GAPERTO = GetDlgItem(hwndDlg, ID_GAPERTO);
    HWND MAPERTO = GetDlgItem(hwndDlg, ID_MAPERTO);
    HWND AAPERTO = GetDlgItem(hwndDlg, ID_AAPERTO);
    HWND INIANNO = GetDlgItem(hwndDlg, ID_INIANNO);
    HWND GCHIUSO = GetDlgItem(hwndDlg, ID_GCHIUSO);
    HWND MCHIUSO = GetDlgItem(hwndDlg, ID_MCHIUSO);
    HWND ACHIUSO = GetDlgItem(hwndDlg, ID_ACHIUSO);
    HWND INIZIALE = GetDlgItem(hwndDlg, ID_INIZIALE);
    HWND POSTODI = GetDlgItem(hwndDlg, ID_POSTODI);
    HWND ESTESE = GetDlgItem(hwndDlg, ID_ESTESO);

    ComboBox_SetText(IDENTI,"");
    ComboBox_SetText(POSTODI,"");
    ComboBox_SetText(GAPERTO,"");
    ComboBox_SetText(MAPERTO,"");
    ComboBox_SetText(AAPERTO,"");
    ComboBox_SetText(GCHIUSO,"");
    ComboBox_SetText(MCHIUSO,"");
    ComboBox_SetText(ACHIUSO,"");
    ComboBox_SetText(INIZIALE,"");
    ComboBox_SetText(INIANNO,"");
    Edit_SetText(ESTESE,"");
    ComboBox_ResetContent(INIZIALE);
    ComboBox_ResetContent(INIANNO);
}

void HandSelident(HWND hwndDlg)
{
    char stringa[401];

    int i,j,k,text_len;

    HWND IDENTI =  GetDlgItem(hwndDlg, ID_IDENT);
    HWND GAPERTO = GetDlgItem(hwndDlg, ID_GAPERTO);
    HWND MAPERTO = GetDlgItem(hwndDlg, ID_MAPERTO);
    HWND AAPERTO = GetDlgItem(hwndDlg, ID_AAPERTO);
    HWND INIANNO = GetDlgItem(hwndDlg, ID_INIANNO);
    HWND GCHIUSO = GetDlgItem(hwndDlg, ID_GCHIUSO);
    HWND MCHIUSO = GetDlgItem(hwndDlg, ID_MCHIUSO);
    HWND ACHIUSO = GetDlgItem(hwndDlg, ID_ACHIUSO);
    HWND INIZIALE = GetDlgItem(hwndDlg, ID_INIZIALE);
    HWND ESTESE = GetDlgItem(hwndDlg, ID_ESTESO);


    k=ComboBox_GetCurSel(IDENTI);
    if(!(mapp=fopen("basedata.dat","r")))
    {
        return;
    }
    fgets(stringa,400,mapp);
    for(i=0;i<k;i++)
    {
        fgets(stringa,400,mapp);
        fgets(stringa,400,mapp);
        fgets(stringa,400,mapp);
        fgets(stringa,400,mapp);
        sscanf(stringa,"%d,%d\n",&nrighe,&nanni);
        for(j=0;j<nrighe+nanni;j++)
            fgets(stringa,400,mapp);
    }
    ComboBox_SetText(GAPERTO,"");
    ComboBox_SetText(MAPERTO,"");
    ComboBox_SetText(AAPERTO,"");
    ComboBox_SetText(GCHIUSO,"");
    ComboBox_SetText(MCHIUSO,"");
    ComboBox_SetText(ACHIUSO,"");
    ComboBox_SetText(INIZIALE,"");
    ComboBox_SetText(INIANNO,"");
    Edit_SetText(ESTESE,"");
    ComboBox_ResetContent(INIZIALE);
    ComboBox_ResetContent(INIANNO);

    fgets(stringa,400,mapp);
    fgets(stringa,400,mapp);
    sscanf(stringa,"%s\n",aperto);
    if(!strcmp(aperto,"nil"))
    {
    }
    else
    {
        sscanf(aperto,"%02d-%02d-%04d\n",&gg,&mm,&aa);
        sprintf(stringa,"%02d",gg);
        ComboBox_AddString(GAPERTO,stringa);
        sprintf(stringa,"%02d",mm);
        ComboBox_AddString(MAPERTO,stringa);
        sprintf(stringa,"%04d",aa);
        ComboBox_AddString(AAPERTO,stringa);
    }
    fgets(stringa,400,mapp);
    sscanf(stringa,"%s\n",chiuso);
    if(!strcmp(chiuso,"nil"))
    {
    }
    else
    {
        sscanf(chiuso,"%02d-%02d-%04d\n",&gg,&mm,&aa);
        sprintf(stringa,"%02d",gg);
        ComboBox_AddString(GCHIUSO,stringa);
        sprintf(stringa,"%02d",mm);
        ComboBox_AddString(MCHIUSO,stringa);
        sprintf(stringa,"%04d",aa);
        ComboBox_AddString(ACHIUSO,stringa);
    }
    fgets(stringa,400,mapp);
    sscanf(stringa,"%d,%d\n",&nrighe,&nanni);
    for(i=0;i<nrighe;i++)
    {
        fgets(stringa,400,mapp);
        sscanf(stringa,"%[!-~ ‘’@ςΰωθιμ°η§]\n",stringa);//sintassi per non fermarsi di fronte agli spazi
        // '\r'=carriage return + '\n'=line feed
        // insieme mandano a capo nel controllo di edit

        strcat(stringa,"\r\n");
        text_len=Edit_GetTextLength(ESTESE);
        Edit_SetSel(ESTESE,0,text_len);
        Edit_ReplaceSel(ESTESE,stringa);
    }
    for(j=0;j<nanni;j++)
    {
        fgets(stringa,400,mapp);
        sscanf(stringa,"%d,%lf",&aa,&saldo);
        sprintf(stringa,"%d",aa);
        i=ComboBox_AddString(INIANNO,stringa);
        sprintf(stringa,"%.2lf",saldo);
        ComboBox_InsertString(INIZIALE,i,stringa);
    }
    sprintf(stringa,"%d",anno);
    ComboBox_SetText(INIANNO,stringa);
    if((i=ComboBox_FindStringExact(INIANNO,-1,stringa))>=0)
    {
        ComboBox_GetLBText(INIZIALE,i,stringa);
        ComboBox_SetCurSel(INIZIALE,i);
    }
    else
    {
    }
    fclose(mapp);
}

void HandMsgIniAnno(HWND hwndDlg)
{
    char stringa[401];

    HWND INIANNO = GetDlgItem(hwndDlg, ID_INIANNO);
    HWND INIZIALE = GetDlgItem(hwndDlg, ID_INIZIALE);

    i=ComboBox_GetCurSel(INIANNO);
    ComboBox_GetLBText(INIANNO,i,stringa);
    ComboBox_SetCurSel(INIANNO,i);
    ComboBox_GetLBText(INIZIALE,i,stringa);
    ComboBox_SetCurSel(INIZIALE,i);
}

void HandMsgIniziale(HWND hwndDlg)
{
    char stringa[401];

    HWND INIANNO = GetDlgItem(hwndDlg, ID_INIANNO);
    HWND INIZIALE = GetDlgItem(hwndDlg, ID_INIZIALE);

    i=ComboBox_GetCurSel(INIZIALE);
    ComboBox_GetLBText(INIZIALE,i,stringa);
    ComboBox_SetCurSel(INIZIALE,i);
    ComboBox_GetLBText(INIANNO,i,stringa);
    ComboBox_SetCurSel(INIANNO,i);
}

void HandScriviSaldo(HWND hwndDlg)
{
    char stringa[401];

    HWND IDENTI =  GetDlgItem(hwndDlg, ID_IDENT);
    HWND GAPERTO = GetDlgItem(hwndDlg, ID_GAPERTO);
    HWND MAPERTO = GetDlgItem(hwndDlg, ID_MAPERTO);
    HWND AAPERTO = GetDlgItem(hwndDlg, ID_AAPERTO);
    HWND INIANNO = GetDlgItem(hwndDlg, ID_INIANNO);
    HWND GCHIUSO = GetDlgItem(hwndDlg, ID_GCHIUSO);
    HWND MCHIUSO = GetDlgItem(hwndDlg, ID_MCHIUSO);
    HWND ACHIUSO = GetDlgItem(hwndDlg, ID_ACHIUSO);
    HWND INIZIALE = GetDlgItem(hwndDlg, ID_INIZIALE);
    HWND POSTODI = GetDlgItem(hwndDlg, ID_POSTODI);

    if(!ComboBox_GetTextLength(IDENTI))
    {
        MessageBox(hwndDlg,"Manca il nome della risorsa!",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        return;
    }
    ComboBox_GetText(IDENTI,nomeconv,27);
    if((i=SendMessage(IDENTI,CB_FINDSTRINGEXACT,-1, (LPARAM) nomeconv))<0)
    {
        MessageBox(hwndDlg,"La voce di risorsa e' nuova. \
        Usa prima 'Scrivi voce'",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        return;
    }
    i=0;
    if(ComboBox_GetTextLength(GAPERTO))
    {
        ComboBox_GetText(GAPERTO,aperto,5);
        if((gg=atoi(aperto))>0)
            i++;
    }
    if(ComboBox_GetTextLength(MAPERTO))
    {
        ComboBox_GetText(MAPERTO,aperto,5);
        if((mm=atoi(aperto))>0)
            i++;
    }
    if(ComboBox_GetTextLength(AAPERTO))
    {
        ComboBox_GetText(AAPERTO,aperto,7);
        if((aa=atoi(aperto))>0)
            i++;
    }
    ComboBox_GetText(INIANNO,stringa,7);
    if((j=atoi(stringa))<=0)
    {
        MessageBox(hwndDlg,"Inserire anno saldo!",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        return;
    }
    if(i==3)
        if(j<aa)
        {
            MessageBox(hwndDlg,"Anno saldo minore anno apertura!",
            "Problema!",MB_ICONEXCLAMATION | MB_OK);
            return;
        }
    i=0;
    if(ComboBox_GetTextLength(GCHIUSO))
    {
        ComboBox_GetText(GCHIUSO,chiuso,5);
        if((gg=atoi(chiuso))>0)
            i++;
    }
    if(ComboBox_GetTextLength(MCHIUSO))
    {
        ComboBox_GetText(MCHIUSO,chiuso,5);
        if((mm=atoi(chiuso))>0)
            i++;
    }
    if(ComboBox_GetTextLength(ACHIUSO))
    {
        ComboBox_GetText(ACHIUSO,chiuso,7);
        if((aa=atoi(chiuso))>0)
            i++;
    }
    if(i==3)
        if(j>aa)
        {
            MessageBox(hwndDlg,"Anno saldo maggiore anno chiusura!",
            "Problema!",MB_ICONEXCLAMATION | MB_OK);
            return;
        }

    ComboBox_GetText(INIZIALE,stringa,14);
    k=sscanf(stringa,"%lf%s",&saldo,nomeconv);
    if(k!=1)
    {
        MessageBox(hwndDlg,"Saldo inserito non\
        e' un numero valido",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        return;
    }
    ComboBox_GetText(INIANNO,stringa,7);
    if((k=SendMessage(INIANNO,CB_FINDSTRINGEXACT,-1, (LPARAM) stringa))>=0)
    {
        //Se l'anno esisteva gia'
        ComboBox_DeleteString(INIANNO,k);
        ComboBox_AddString(INIANNO,stringa);
        ComboBox_DeleteString(INIZIALE,k);
        ComboBox_GetText(INIZIALE,stringa,14);
        ComboBox_InsertString(INIZIALE,k,stringa);
    }
    else
    {
        k=SendMessage(INIANNO,CB_ADDSTRING,-1, (LPARAM) stringa);
        ComboBox_GetText(INIZIALE,stringa,14);
        ComboBox_InsertString(INIZIALE,k,stringa);
    }
    ComboBox_GetText(IDENTI,nomeconv,27);
    ComboBox_SetText(POSTODI,nomeconv);
    ComboBox_SetText(INIANNO,"");
    ComboBox_SetText(INIZIALE,"");
    HandCambia(hwndDlg);
    HandSelident(hwndDlg);
}
