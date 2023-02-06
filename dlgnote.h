#ifndef DLGNOTE_H_INCLUDED
#define DLGNOTE_H_INCLUDED

#include <windows.h>
#include <windowsx.h>


#include "resource.h"
#include "globfun.h"
#include "struct.h"
#include "define.h"


BOOL CALLBACK DlgNote(HWND , UINT , WPARAM , LPARAM );

void NoteHandOK(HWND);
int NoteHandAnnulla(HWND);


#endif // DLGNOTE_H_INCLUDED
