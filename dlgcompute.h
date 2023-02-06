#ifndef DLGCOMPUTE_H_INCLUDED
#define DLGCOMPUTE_H_INCLUDED

#include <stdio.h>
#include <dos.h>
#include <time.h>
#include <windows.h>
#include <commctrl.h>


#include "resource.h"
#include "globfun.h"


BOOL CALLBACK DlgCompute(HWND , UINT , WPARAM , LPARAM );

int cmp_riepiloga(HWND);
void cmp_caricaRisorse(HWND);
void cmp_caricacategorie(HWND);
void EseguiCalcolo(HWND,int,int);
void mostradato(HWND);
void cmp_HandGDopo(HWND);
void cmp_HandGPrima(HWND);
void HandSelSaldoRisorsa(HWND,WPARAM);
void HandSelTipoDato(HWND,WPARAM);
void HandCheckEliGN(HWND,WPARAM);
void EliGN(HWND);
void HandCheckEliEN(HWND,WPARAM);
void EliEN(HWND);
void HandCheckEliUN(HWND,WPARAM);
void EliUN(HWND);
void HandCheckEliTN(HWND,WPARAM);
void EliTN(HWND);
void HandEsegui(HWND);
void HandValData(HWND,int,int,int,int,WPARAM);
void cmp_HandValRiG(HWND,WPARAM);
void cmp_HandValRiM(HWND,WPARAM);
void cmp_HandValRiA(HWND,WPARAM);
void cmp_HandValRiGDa(HWND,WPARAM);
void cmp_HandValRiMDa(HWND,WPARAM);
void cmp_HandValRiADa(HWND,WPARAM);
void cmp_HandValRiGA(HWND,WPARAM);
void cmp_HandValRiMA(HWND,WPARAM);
void cmp_HandValRiAA(HWND,WPARAM);
void cmp_HandCalcola(HWND);
void cmp_HandCancella(HWND);
void lanciacalc(HWND);
void ordinanum(HWND,int,int,int,int,int,char);
void ordinalet(HWND,int,int,int,int,int,char);
void HandPulisci(HWND);
void HandRiepilo(HWND,WPARAM);
void HandRiep1(HWND,WPARAM);
void HandRiep2(HWND,WPARAM);
void HandRiep3(HWND,WPARAM,UINT);
void HandRiepRis(HWND,WPARAM);
void HandOrdRis(HWND,WPARAM);
void HandOrdRiep(HWND,WPARAM);
void HandOrdRiep1(HWND,WPARAM);
void HandOrdRiep2(HWND,WPARAM);
void HandOrdRiep3(HWND,WPARAM);


#endif // DLGCOMPUTE_H_INCLUDED
