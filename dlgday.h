#ifndef DLGDAY_H_INCLUDED
#define DLGDAY_H_INCLUDED

#include <windows.h>
#include <windowsx.h>


#include "resource.h"
#include "globfun.h"
#include "struct.h"
#include "dlgnote.h"
#include "dlgcompute.h"


BOOL CALLBACK DlgDay(HWND , UINT , WPARAM , LPARAM );

int caricaVoci(HWND);
void caricaLista(HWND,int,int,int,int);
void caricacategorie(HWND);
int riepiloga(HWND,long int);
void DHandValGen(HWND,WPARAM);
void DHandValMen(HWND,WPARAM);
void DHandValAen(HWND,WPARAM);
void DHandValGusc(HWND,WPARAM);
void DHandValMusc(HWND,WPARAM);
void DHandValAusc(HWND,WPARAM);
void DHandValRiG(HWND,WPARAM);
void DHandValRiM(HWND,WPARAM);
void DHandValRiA(HWND,WPARAM);
void CheckEntrata(HWND);
void CheckUscita(HWND);
void HandAnnulla(HWND);
void HandScrivi(HWND);
void aggiornacategorie(HWND);
void HandGDopo(HWND);
void HandGPrima(HWND);
void HandVai(HWND);
void ved(HWND);
void soloved(HWND);
void tuttevis(HWND);
void nonved(HWND);
void solononved(HWND);
int cancella(HWND,char *,long int,char *);
void HandCancella(HWND);
void SimpleSel(HWND);
void ComplexSel(HWND);
void HandNote(HWND);
int scrivinote(HWND,char *,long int,int);






#endif // DLGDAY_H_INCLUDED
