#ifndef DLGMAIN_H_INCLUDED
#define DLGMAIN_H_INCLUDED



#include "resource.h"
#include "globfun.h"
#include "dlgday.h"
#include "diafield.h"
#include "dlgcompute.h"


BOOL CALLBACK DlgMain(HWND , UINT , WPARAM , LPARAM );

void EseguiBackup(HWND);
int HandOggi(HWND);
void HandPrelimin(HWND);
void HandValGior(HWND,WPARAM);
void HandValMes(HWND,WPARAM);
void HandValAnn(HWND,WPARAM);
int HandCalcola(HWND);


#endif // DLGMAIN_H_INCLUDED
