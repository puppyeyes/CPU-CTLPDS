#ifndef tbd_h
#define tbd_h
/************************************************************************/
#include <stdio.h>
#include <stdlib.h>
/************************************************************************/
typedef struct _TBD { int x,y; struct _TBD *a,*b,*c,*n; } TBD; 
/************************************************************************/
void TBDvl(char **vlist);
TBD *TBDvr(char *var);
TBD *TBDng(TBD *s);
TBD *TBDcn(TBD *s,TBD *t);
TBD *TBDbs(int x,TBD *w);
int TBDtr(TBD *s);
void TBDkp(TBD *w);
void TBDdr(TBD *w);
void TBDcl();
void TBDrl(char **vlist);
TBD *TBDrn(TBD *w);
int TBDrc(TBD *w);
TBD *TBDds(TBD *s,TBD *t);
TBD *TBDxb(int x,TBD *s);
int TBDmp(TBD *s);
/***********************************************************************/
#endif
