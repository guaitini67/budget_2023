#ifndef GLOBFUN_H_INCLUDED
#define GLOBFUN_H_INCLUDED

// ***************************************************************************************
// ************************ SEZIONE DICHIARAZIONE FUNZIONI *******************************
// ***************************************************************************************

#include <stdio.h>
#include <dos.h>
#include <time.h>
#include <windows.h>
#include <windowsx.h>

#include "define.h"
#include "struct.h"



int controllabasedata();
int caricav(info_risorse *,int *);
int controllafiledati(char *);
int backuplog();
int compareFiles(char *, char *);
int isbises(int);
int scrivilog(char, char *);
void mostrasaldi(HWND,int,int,int,info_risorse *);
int caricasaldi(long int,info_risorse *);
int aggiornasaldi(long int,int,char *,info_risorse *);
int controllacampi(HWND,int,int,int);
void caricacampi(HWND,int,int,int);
int leggiprepara(long int,char *);
int scrivifile(char *,long int,char *);
float search_util(FILE *,char *,char,char,long int *,int *,int *,int *,int *,int *);
int search_year(char *,char *,info_smaller *,int,long int,long int);




#endif // GLOBFUN_H_INCLUDED
