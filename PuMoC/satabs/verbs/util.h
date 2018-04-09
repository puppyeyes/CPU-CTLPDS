#ifndef util_h
#define util_h
/***********************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
/***********************************************************************/
#define ZWH
#define LOCAL
/***********************************************************************/
#define nl 		{fprintf(stdout,"\n\n");}
#define error(s) 	{fprintf(stderr,"%s error\n",s); exit(0);}
#define prs(s) 		{fprintf(stdout,"%s\n",s);}
#define pri(s) 		{fprintf(stdout,"%i\n",s);}
/***********************************************************************/
#define MAXSTRING		0x800000
#define MAXVARLEN		0x000040
/***********************************************************************/
typedef struct _model { 
	int n;
	int m;
	int l;
	int *mstring; 
} model;
/***********************************************************************/
typedef struct _mstruct {
        int **Clause; 
        int *CL; 
        int *LiteralMatrix; 
        int *LS; 
        int *LL; 
} mstruct;
/***********************************************************************/
typedef struct _mdata {
        char *InactiveMatrix;
        int *InactiveList;
        int *Literals;
        int *Current; 
} mdata;
/***********************************************************************/
#include "wfu.h"
#endif
