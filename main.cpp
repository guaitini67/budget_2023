#include "main.h"

HINSTANCE hInst;
HICON hIcon1;   // icon handle

const char *gset[]={"Domenica","Lunedi","Martedi","Mercoledi","Giovedi","Venerdi","Sabato",""};

int month[12] = {31,28,31,30,31,30,31,31,30,31,30,31};


char stringa[401],stringa1[401],nomeconv[27],vis,visetto,entra,
     fnam1[20],fnam2[20],mot_f[100],cat_f[100],aperto[12],chiuso[12];

int i,j,k,fine,num_ops,nrighe,pa,pb,pp,match,r,fatto,anno,nlnote,
    posiz,nanni,numfonti,g,m,a,gg,mm,aa,err,plink,indice,errore,newposiz,tab[5];

long int numg_fa,numg_fb,numg_out,numg_riepilogo;
double saldo;

time_t curtime;
struct tm *data_ora;

FILE *fil,*tmp,*ntt,*mapp,*f1,*f2,*logg;

struct _finddata_t ffblk,ffblk1;
/*
 * Attributes of files as returned by _findfirst et al. (struct _finddata_t)

#define	_A_NORMAL	0x00000000
#define	_A_RDONLY	0x00000001
#define	_A_HIDDEN	0x00000002
#define	_A_SYSTEM	0x00000004
#define	_A_VOLID	0x00000008
#define	_A_SUBDIR	0x00000010
#define	_A_ARCH		0x00000020

*/


info_movimenti tabella_movimenti[100];
info_risorse t_risorse1[100];

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst=hInstance;
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
//    hDlg=CreateDialog(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);

}

