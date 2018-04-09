#include "bool.h"

/*******************************************************************/
tree *cleanbool(tree *t)
{
        if (t->node[0]==';') t->node[0]='|'; 
        else if (t->node[0]==':') t->node[0]='&'; 
	return t;
}
/*******************************************************************/

/*******************************************************************/
#define isconst(node) (node[0]=='C')
/********************************************************************
* constants are of the form s0wait,s1think			    *
/*******************************************************************/
tree *constructliteral(int i,int pos)
{
	tree *t0,*t1;

	t0=(tree *)(malloc(sizeof(tree))); t0->left=NULL; t0->right=NULL; 
	sprintf(t0->node,"%ix",i);
	if (pos) return t0;
	t1=(tree *)(malloc(sizeof(tree))); t1->left=NULL; t1->right=t0; 
	strcpy(t1->node,"!"); 
	return t1;
}
/*******************************************************************/
tree *constructbool(int start,int j,int k)
{
	int i;
	tree *t,*t0,*t1,*t2;

	t=t0=(tree *)(malloc(sizeof(tree))); 
	strcpy(t->node,"&"); t->left=NULL; t->right=NULL;
	for (i=1;i<=j;i++) {
		t->left=constructliteral(start+i,k%2);
		t->right=(tree *)(malloc(sizeof(tree))); t=t->right;
		strcpy(t->node,"&"); t->left=NULL; t->right=NULL; k=k/2;
	}
	t=t0; t1=t0->right;
	if (t1->right==NULL) { t2=t->left; free(t1); free(t); return t2; }
	while (t->right->right!=NULL) {t1=t; t=t->right; }
	t1->right=t->left; free(t->right); free(t); return t0;
}
/*******************************************************************/
void replaceconstants(tree *t) 
{
	char *p,*q;

	if (t==NULL) { return; }
	if (isconst(t->node)) { 
		p=q=t->node;
		while (!isdigit(*p)) p++;
		while (isdigit(*p)) *q++=*p++; *q=0; return;
	}
	replaceconstants(t->left); replaceconstants(t->right); return;
} 
/*******************************************************************/
tree *replacevariable_eq(int a1,int b1,int a2,int b2)
{
	int start1=a1-1;
	int start2=a2-1;
	int i,j=b1-a1+1;
	tree *t,*t0,*t1,*t2;

	t=t0=(tree *)(malloc(sizeof(tree))); 
	strcpy(t->node,"&"); t->left=NULL; t->right=NULL;
	for (i=2;i<=2*j+1;i++) {
		t1=(tree *)(malloc(sizeof(tree))); strcpy(t1->node,"|"); 
		t1->left=constructliteral(start1+i/2,i%2);
		t1->right=constructliteral(start2+i/2,(i+1)%2);
		t->left=t1; 
		t->right=(tree *)(malloc(sizeof(tree))); t=t->right;
		strcpy(t->node,"&"); t->left=NULL; t->right=NULL;
	}
	t=t0; t1=t0->right;
	if (t1->right==NULL) { t2=t->left; free(t1); free(t); return t2; }
	while (t->right->right!=NULL) {t1=t; t=t->right; }
	t1->right=t->left; free(t->right); free(t); return t0;
}
void replacevariable(tree *t,tree *vlist,int bn) 
{
	tree *v1;
	char *var1,*var2;
	int a1,a2,b1,b2;

	if (t==NULL) { return; }
	if (t->node[0]!='=') {
		replacevariable(t->left,vlist,bn);
		replacevariable(t->right,vlist,bn); return;
	}

	var1=t->left->node; var2=t->right->node;
	a1=b1=a2=b2=0;

	if (!isdigit(*var1)) { 
		if (*var1=='X') { a1=b1=bn; } var1++; 
		v1=getvtree(var1,vlist);
		a1+=atoi(v1->left->node); b1+=atoi(v1->right->node);
	}
	if (!isdigit(*var2)) { 
		if (*var2=='X') { a2=b2=bn; } var2++; 
		v1=getvtree(var2,vlist);
		a2+=atoi(v1->left->node); b2+=atoi(v1->right->node);
	}

	if (a1>0&&a2>0) v1=replacevariable_eq(a1,b1,a2,b2);
	if (a1>0&&a2==0) v1=constructbool(a1-1,b1-a1+1,atoi(var2)); 
	if (a2>0&&a1==0) v1=constructbool(a2-1,b2-a2+1,atoi(var1)); 
	if (a1==0&&a2==0) error("variable");

	free(t->left); free(t->right); 
	t->left=v1->left; t->right=v1->right; 
	strcpy(t->node,v1->node); free(v1);
}
/*******************************************************************/
