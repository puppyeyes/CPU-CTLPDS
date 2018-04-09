#ifndef main_h
#define main_h
/***********************************************************************/
#include "conv.h"
#include "arit.h"
#include "config.h"
/***********************************************************************/
typedef struct _pginfo { 
	tree *prog; 
	tree *ndlock;
	tree *cs;
	tree *spec;
	int booln; 
	model *mo; 
	model *mo2; 
} pginfo;    
/***********************************************************************/
#define V101
#define V102
/***********************************************************************/
#include "wfm.h"
#endif
