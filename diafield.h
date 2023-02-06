#ifndef DIAFIELD_H_INCLUDED
#define DIAFIELD_H_INCLUDED

#include <windows.h>
#include <windowsx.h>

#include "resource.h"
#include "globfun.h"
#include "define.h"

BOOL CALLBACK DlgDiaField(HWND , UINT , WPARAM , LPARAM );

void aggiornaFile(HWND);
void HandInsert(HWND);
void HandCambia(HWND);
void HandCanc(HWND);
void HandAnnul(HWND);
void HandSelident(HWND);
void HandMsgIniAnno(HWND);
void HandMsgIniziale(HWND);
void HandScriviSaldo(HWND);


#endif // DIAFIELD_H_INCLUDED
