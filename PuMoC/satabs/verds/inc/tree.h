#ifndef tree_h
#define tree_h
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
#define xDEBUG
/***********************************************************************/
#define error(s) 		{fprintf(stderr,"%s error\n",s); exit(0);}
#define pprts(s) 		{fprintf(stdout,"%s\n",s);}
#define pprti(s) 		{fprintf(stdout,"%i\n",s);}
/***********************************************************************/
#define infos(s) 		{fprintf(stdout,"%s\n",s);}
#define infoi(s) 		{fprintf(stdout,"%i\n",s);}
#define infof(s) 		{fprintf(stdout,"%f\n",s);}
/*******************************************************************/
#define tmp1file "satlog.tmp"
#define tmp2file "satlom.tmp"
#define tmp0file "verbs.tmp"
/***********************************************************************/
#ifdef DEBUG
#define prts(s) 		{fprintf(stdout,"%s\n",s);}
#define prti(s) 		{fprintf(stdout,"%i\n",s);}
#define debug()
#else
#define prts(s) 		
#define prti(s) 		
#define debug()
#endif
/*
#define debug() return;
/***********************************************************************/
#define MAXSTRING		0x04000000
#define MAXVARLEN		0x00000040
/***********************************************************************/
#define TRUE			'^'
#define FALSE			'_'
#define TRUEs			"^"
#define FALSEs			"_"
/***********************************************************************/
#define CHANW   		'@'
#define CHANWs  		"@"
#define CHANR   		'?'
#define CHANRs  		"?"
/***********************************************************************/
#define BOR			'M'
#define BORs			"M"
#define DAND			'N'
#define DANDs			"N"
/***********************************************************************/
#define PIDs			"p_id_"
#define PIDkw			"pid"
/***********************************************************************/
#define neg(a)          treecompose("!",NULL,a)
#define lor(a,b)        treecompose("|",a,b)
#define land(a,b)       treecompose("&",a,b)
/*******************************************************************/
#define xor(a,b)        treecompose("&", \
                        treecompose("|",treecopy(a),b), \
                        treecompose("|",neg(a),neg(treecopy(b))))
#define xor_dnf(a,b)    treecompose("|", \
                        treecompose("&",treecopy(a),neg(b)), \
                        treecompose("&",neg(a),treecopy(b))) \
/*******************************************************************/
#define leq(a,b)        treecompose("&", \
                        treecompose("|",neg(treecopy(a)),treecopy(b)), \
                        treecompose("|",a,neg(b)))
#define leq_dnf(a,b)    treecompose("|", \
                        treecompose("&",treecopy(a),treecopy(b)), \
                        treecompose("&",neg(a),neg(b))) \
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
typedef struct _workspace {
        tree *prog;
	tree *spec;
        tree *varconstr;
        tree *deadlockfreecond;
        int booln;
        int puretrans;
	tree *fness;
} workspace;
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
typedef struct _dbase {
        char *file;
        char *string;
	module *tsm;
	tree *ts;
	tree *processes;
	int property;
        char propertytype;
} dbase;
/***********************************************************************/
#include "wftree.h"
#endif
