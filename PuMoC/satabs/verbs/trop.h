#ifndef trop_h
#define trop_h
/***********************************************************************/
#include "util.h"
/***********************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
/***********************************************************************/
#define CHANW   '@'
#define CHANWs  "@"
#define CHANR   '?'
#define CHANRs  "?"
#define TRUE    '^'
#define TRUEs   "^"
#define FALSE   '_'
#define FALSEs  "_"
/***********************************************************************/
#define BOR     'O'
#define BORs    "O"
#define DOR     'P'
#define DORs    "P"
#define DAND    'Q'
#define DANDs   "Q"
/***********************************************************************/
#define nl 		{fprintf(stdout,"\n\n");}
#define error(s) 	{fprintf(stderr,"%s error\n",s); exit(0);}
#define prs(s) 		{fprintf(stdout,"%s\n",s);}
#define pri(s) 		{fprintf(stdout,"%i\n",s);}
/***********************************************************************/
#define isvarconst(a)	(a[0]<='z'&&a[0]>='a')
/***********************************************************************/
typedef struct _tree { 
	char node[MAXVARLEN]; 
	struct _tree *left; 
	struct _tree *right; 
} tree;
/***********************************************************************/
typedef struct _module { 
	tree *parameters;
	tree *prog;
	struct _module *next; 
} module;
/***********************************************************************/
#include "wft.h"
#endif
