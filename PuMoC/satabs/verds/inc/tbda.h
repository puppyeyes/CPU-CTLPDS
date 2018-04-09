#ifndef p2var_h
#define p2var_h
/***********************************************************************/
#include "tbd.h"
/************************************************************************/
typedef struct _TBDtv { int count; int *ival; int *rtab; char **name; } TBDtv; 
typedef struct _TBDtb { int tablesz; int nodes; int cap; TBD **table; } TBDtb;
/************************************************************************/
#define GLOBAL
#define LOCAL
#define TBDCSTMARK      -1
#define TBDCSTOMG       0x01000000
#define MCACHESIZE      0x08000000
#define ICACHESIZE      0x00001000
#define TBDCSTND        0x00000100
#define TBDCSTOR        0x00000200
#define TBDCSTLB        0x00000400
#define TBDCSTRS        0x00000800
/************************************************************************/
#define ckalloc(s)      {if (s==NULL) error("low memory");}
/***********************************************************************/
#define TBDmalloc(tbdc,n) \
        tbdc=(TBDtb *)malloc(1*sizeof(TBDtb)); ckalloc(tbdc); \
        tbdc->tablesz=n; tbdc->cap=n/2; tbdc->nodes=0; \
        tbdc->table=(TBD **)calloc(tbdc->tablesz,sizeof(TBD *)); \
        ckalloc(tbdc->table); \
/************************************************************************/
#define TBDrealloc(tbdc,r) \
        TBD3ftc(tbdc); tbdc->tablesz/=2; tbdc->cap/=2; tbdc->nodes=0; \
        tbdc->table=(TBD **)realloc(tbdc->table,tbdc->tablesz*sizeof(TBD *));\
        if ((r=(TBD *)calloc(1,sizeof(TBD)))==NULL) error("low memory"); \
/************************************************************************/
#define diffctb(s,x1,a1,b1) ((s->x!=x1)||(s->a!=a1)||(s->b!=b1))
#define difftct(s,t) ((s->x!=t->x)||(s->a!=t->a)||(s->b!=t->b)||(s->c!=t->c))
#define ckcache(cch) if (cch->tablesz>=MCACHESIZE) { cch->nodes=0; return; }
/************************************************************************/
#define istau(w) 	(w==TBDtau)
#define isomg(w) 	(w==TBDomg)
#define istoo(w) 	(istau(w)||isomg(w))
#define isomt(s,t) 	(isomg(s)||istau(t))
/************************************************************************/
#define TBDnnd(s,t) 	TBDng(TBDcn(s,t))
#define TBDck(s,t) 	/* if (s==t) return s; */\
			if (isomt(s,t)) return s; if (isomt(t,s)) return t; \
/************************************************************************/
/*
#define TBDemp(s)	(TBDisimp(TBDng(s))==TBDtau)
#define TBDemp(s)	(TBD8tr(TBDng(s))==0)
#define TBDemp(s)	(TBD8mp(s))
#define TBDemp(s)	(TBDisimp(TBDng(s))==TBDomg)
/************************************************************************/
extern TBD* TBDomg;;
extern TBD* TBDtau;;
extern TBDtb *TBDttb;;
extern TBDtb *TBDctb;;
extern TBDtv *TBDvtb;;
/************************************************************************/
void TBD0ntl();
TBD *TBD0dtc(TBDtb *TBDttb,TBD *w);
TBD *TBD0gtc(TBDtb *TBDctb,int x,TBD *a,TBD *b);
TBD *TBD0dnc(TBDtb *nc,TBD *w);
void TBD0rtc(TBDtb *cache,TBD *f());
void TBD0rls(TBDtb *newcache,TBD *w,TBD *f());
TBD *TBD0cnd(int x,TBD *a,TBD *b,TBD *c);
TBD *TBD0crn(int x,TBD *a,TBD *b,TBD *c);
TBD *TBD1clt(int a);
void TBD1svl(char **a,int *b,int l,int r) /* increasing */;
int TBD1snm(char *s,char **a,int *b,int l,int r);
int TBD2ccs(TBD *s,TBD *t);
TBD *TBD2crs(TBD* r0,int x,int v);
TBD *TBD2ctb(TBD *w);
int TBD2cml(TBD *w);
int TBD3mnd(TBD *w);
void TBD3ftc(TBDtb *cache);
TBD *TBD3fls(TBD *w);
void TBD3und(TBDtb *cache);
void TBD3uls(TBD *w);
int TBD4cnd(TBD *w);
int TBD4mnd(TBD *w);
void TBD4und(TBD *w);
TBD *TBDnnf(TBD *w);
TBD *TBDbdd(TBD *w);
TBD *TBDsimp(TBD *w,TBD *c);
TBD *TBDnnfsimp(TBD *w);
/************************************************************************/
#endif
