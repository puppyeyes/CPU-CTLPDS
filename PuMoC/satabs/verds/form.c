#include "form.h"

/*******************************************************************/
#define isatomic(node)	      		(isdigit(node[0])) 
/*******************************************************************/
LOCAL char DUALOPRS[19]="EAXXFGUR&|>{<}=B^_";
/*
LOCAL char TLFOPRS[9]="EAXXFGUR";
LOCAL char BOFOPRS[6]="&|~I!";
LOCAL char ATFOPRS[9]=">{<}=B^_";
/*******************************************************************/
int TLFdualopr(char a)
{
        char *p=DUALOPRS;
        int i=0; 
        while (*p&&*p!=a) { p++; i++; } if (*p) return i+1; else return 0;
}
/*******************************************************************/
tree *TLFnegate(tree *t)
{
	int i;
        tree *t0;
        if (t==NULL) return NULL;
        if (isatomic(t->node)) return treecompose("!",NULL,t);
	if (i=TLFdualopr(t->node[0])) {
        	i=i+i%2-(i+1)%2; t->node[0]=DUALOPRS[i-1];
		if (i<=10) {
    			t->left=TLFnegate(t->left); 
			t->right=TLFnegate(t->right); 
		}
		return t;		
	}
	switch (t->node[0]) {
	  case '!': t0=t->right; free(t); return t0;
	  case 'I': t->node[0]='&'; t->right=TLFnegate(t->right); return t;
	  case '~': t->right=TLFnegate(t->right); return t;
	}
 	pprts(t->node);
	error("undefined operator 0");
}
/*******************************************************************/
tree *TLFnnf(tree *t)
{
	int i;
        tree *t0,*t1,*t2;

        if (t==NULL) return NULL;
        if (isatomic(t->node)) return t;
	if (t->node[0]=='#'||t->node[0]==';'||t->node[0]==':') {
    		t->left=TLFnnf(t->left); t->right=TLFnnf(t->right); 
		return t;
	}
	if (i=TLFdualopr(t->node[0])) {
        	i=i+i%2-(i+1)%2;
		if (i<=10) {
    			t->left=TLFnnf(t->left); 
			t->right=TLFnnf(t->right); 
		}
		return t;
	}
	switch (t->node[0]) {
	  case '!': 
		if (isatomic(t->right->node)) return t;
		t0=TLFnegate(t->right); free(t); return TLFnnf(t0);
	  case 'I': 
		t->node[0]='|'; 
		t->left=TLFnnf(TLFnegate(t->left)); t->right=TLFnnf(t->right); 
		return t;
	  case '~': 
		t->node[0]='&'; 
		t1=treecompose("|",
			TLFnnf(neg(treecopy(t->left))),
			TLFnnf(treecopy(t->right)));
		t2=treecompose("|",
			TLFnnf(t->left),
			TLFnnf(neg(t->right)));
		t->left=t1; t->right=t2; return t;
	}
 	pprts(t->node);
	error("undefined operator 1");
}
/*******************************************************************/
#define keepleft(t)   { t0=t->left; treefree(t->right); free(t); return t0; }
#define keepright(t)  { t0=t->right; treefree(t->left); free(t); return t0; }
#define modright(t,a) { t0=t->right; t0->node[0]=a; free(t); return t0; }
/*******************************************************************/
tree *TLFnnfsimpATF(tree *t)
{
        int a,b,c;

        if (t->left==NULL||!isdigit(t->left->node[0])) return t;
        if (t->right==NULL||!isdigit(t->right->node[0])) return t;
        a=atoi(t->left->node); b=atoi(t->right->node);
        switch (t->node[0]) {
          case '-': c=a-b; 
		if (c<0) { a=1; while (a<=b) a*=2; c+=a; }
		treereplace(t,treenum(c)); break;
          case '+': treereplace(t,treenum(a+b)); break;
          case '*': treereplace(t,treenum(a*b)); break;
          case '/': treereplace(t,treenum(a/b)); break;
          case '%': treereplace(t,treenum(a%b)); break;
/*zwh
*/
          case '>': treereplace(t,treeconst(a>b)); break;
          case '{': treereplace(t,treeconst(a<=b)); break;
          case '<': treereplace(t,treeconst(a<b)); break;
          case '}': treereplace(t,treeconst(a>=b)); break;
          case '=': treereplace(t,treeconst(a==b)); break;
          case 'B': treereplace(t,treeconst(a!=b)); break;
        }
	return t;
}
/*******************************************************************/
tree *TLFnnfsimp(tree *t)
{
	tree *t0;
	if (t==NULL) return t;

	switch (t->node[0]) {
	  case '&': 
	  case DAND:
		if (t->right->node[0]==FALSE) { keepright(t); }
		if (t->right->node[0]==TRUE) { keepleft(t); }
		if (t->left->node[0]==FALSE) { keepleft(t); }
		if (t->left->node[0]==TRUE) { keepright(t); }
		return t;
	  case '|': 
	  case BOR:
		if (t->right->node[0]==FALSE) { keepleft(t); }
		if (t->right->node[0]==TRUE) { keepright(t); }
		if (t->left->node[0]==FALSE) { keepright(t); }
		if (t->left->node[0]==TRUE) { keepleft(t); }
		return t;
	  case '!': 
		if (t->right->node[0]==TRUE) { modright(t,FALSE); }
		if (t->right->node[0]==FALSE) { modright(t,TRUE); }
		return t;
	}
	return TLFnnfsimpATF(t);
}
/************************************************treedopost(t,TLFnnfsimp)
tree *TLFnnfsimpall(tree *t)
{
	if (t==NULL) return t;
	t->left=TLFnnfsimpall(t->left); t->right=TLFnnfsimpall(t->right); 
	return TLFnnfsimp(t);
}
/*******************************************************************/

/*******************************************************************/
#define isliteral(node)	      (isdigit(node[0])||node[0]=='!')
/*******************************************************************/
int iscclause(tree *t) 
{
	if (t==NULL) return 0; 
	if (isliteral(t->node)) return 1;
	if (t->node[0]=='&') return iscclause(t->left)&&iscclause(t->right);
	return 0;
}
/*******************************************************************/
tree *lcnfcombine(tree *t1,tree *t2)
{
	tree *s1,*s2;
	if (t1->node[0]=='&') { 
		s1=lcnfcombine(t1->left,t2);
		s2=lcnfcombine(t1->right,t2);
		if (s1->node[0]==FALSE) return s2;
		if (s2->node[0]==FALSE) return s1;
		return treecompose("&",s1,s2);
	} 
	if (t2->node[0]=='&') { 
		s1=lcnfcombine(t1,t2->left);
		s2=lcnfcombine(t1,t2->right);
		if (s1->node[0]==FALSE) return s2;
		if (s2->node[0]==FALSE) return s1;
		return treecompose("&",s1,s2);
	}
	if (t1->node[0]=='!') {
		if (t2->node[0]=='!') {
			if (strcmp(t1->right->node,t2->right->node)==0)
			return treecopy(t1); 
			return treecompose("|",treecopy(t1),treecopy(t2));
		} else 
		if (strcmp(t1->right->node,t2->node)==0) return treeconst(0);
		return treecompose("|",treecopy(t1),treecopy(t2));
	} 
	if (t2->node[0]=='!') {
		if (strcmp(t1->node,t2->right->node)==0) return treeconst(0);
		return treecompose("|",treecopy(t1),treecopy(t2));
	}
	if (strcmp(t1->node,t2->node)==0) return treecopy(t1); 
	return treecompose("|",treecopy(t1),treecopy(t2));
}
/*******************************************************************/
int TLFlcnf(tree *t)
{
	tree *t0;

	if (t==NULL) return 1; 
	if (t->node[0]=='|'&&(iscclause(t->left)&&iscclause(t->right))) {
		t0=lcnfcombine(t->left,t->right);
		treereplace(t,t0); 
		return 1;
	}
	return 0;
}
