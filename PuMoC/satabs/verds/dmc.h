#ifndef dmc_h
#define dmc_h
/***********************************************************************/
#include "tbd.h"
#include <string.h>
/***********************************************************************/
#define LOCAL
#define GLOBAL
/***********************************************************************/
#define m2i             m2cache[0]
#define m2rel           (m2cache+1)
#define m2rrel          (m2cache+1+m2rcount)
#define m2rrela         (m2cache+1+m2rcounta)
/************************************************************************/
#define m2xx(w)         m2ex(w,m2rrel,m2aux,TBDds)
#define m2ef(w)         m2fg(w,m2rrel,m2aux,TBDds,TBDds)
#define m2eg(w)         m2fg(w,m2rrel,m2aux,TBDcn,TBDds)
#define m2eu(u,w)       m2ur(u,w,m2rrel,m2aux,TBDds,TBDcn,TBDds)
#define m2er(u,w)       m2ur(u,w,m2rrel,m2aux,TBDcn,TBDds,TBDds)
/************************************************************************/
#define m2xxa(w)        m2ex(w,m2rrela,m2aux,TBDcn)
#define m2efa(w)        m2fg(w,m2rrela,m2aux,TBDds,TBDcn)
#define m2ega(w)        m2fg(w,m2rrela,m2aux,TBDcn,TBDcn)
#define m2eua(u,w)      m2ur(u,w,m2rrela,m2aux,TBDds,TBDcn,TBDcn)
#define m2era(u,w)      m2ur(u,w,m2rrela,m2aux,TBDcn,TBDds,TBDcn)
/************************************************************************/
#define msg()		fprintf(stdout,"APPLYING:   Symbolic model checking\n")
#define m2relation(a,b) m2ptrel(a,b)
/************************************************************************/
/************************************************************************/
#include "m2init.h"
#include "wfdmc.h"
#endif  
