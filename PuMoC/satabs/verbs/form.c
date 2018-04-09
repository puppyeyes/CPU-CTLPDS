#include "form.h"

/*******************************************************************/

/*******************************************************************/
#define isatomic(node)	      (isdigit(node[0])||node[0]==TRUE||node[0]==FALSE) 
#define ispconnective(node)   (node[0]=='&'||node[0]=='|'||node[0]=='!')
/*******************************************************************/
int isprop(tree *t)
{
	if (t==NULL) return 1; 
	if (isatomic(t->node)) return 1;
	if (ispconnective(t->node)) return isprop(t->left)&&isprop(t->right); 
	return 0;
}
/*******************************************************************/
LOCAL char dualoperators[16]="AEFGURXX&|";
/*******************************************************************/
int opnumber(char a) 
{
	char *p=dualoperators;
	int i=0;

	while (*p&&*p!=a) { p++; i++; } if (*p) return i+1; else return 0;
}
/*******************************************************************/
tree *negate(tree *t)
{
	tree *t0;
	int i;

	if (t==NULL) return t;
	if (isatomic(t->node)) return treecompose("!",NULL,t);
	if (t->node[0]=='!') { t0=t->right; free(t); return t0; }
	if (t->node[0]==':') { 
		t->node[0]='&'; t->right=negate(t->right); return t; 
	}

	i=opnumber(t->node[0]); 
	if (i==0) return treecompose("!",NULL,t);
	i=i+i%2-(i+1)%2;
	t->node[0]=dualoperators[i-1];
	t->left=negate(t->left); t->right=negate(t->right); return t;
}
void nnftemp(tree *t)
{
	tree *t0;
	int i;

	if (t==NULL) return;
	if (isatomic(t->node)) return;
	if (t->node[0]=='!') { 
		t0=t->right;
		i=opnumber(t0->node[0]); if (i==0) return;
		i=i+i%2-(i+1)%2;
		t0->node[0]=dualoperators[i-1];
		t0->left=negate(t0->left); t0->right=negate(t0->right); 
		nnftemp(t0->left); nnftemp(t0->right); 
		strcpy(t->node,t0->node);
		t->left=t0->left; t->right=t0->right; free(t0); return;
	}
	nnftemp(t->left); nnftemp(t->right); return;
}
/*******************************************************************/
tree *nnf(tree *t)
{
	tree *t0;
	int i;

	if (t==NULL) return t;
	if (isatomic(t->node)) return t;
	if (t->node[0]=='!') { 
		if (isatomic(t->right->node)) return t;
		t0=nnf(negate(t->right)); free(t); return t0;
	}
	t->left=nnf(t->left); t->right=nnf(t->right); return t;
}
/*******************************************************************/
tree *cnf(tree *t)
{
	tree *t0,*t1;
	char landor[4]="&|";

	if (t==NULL) return t;
	if (isatomic(t->node)||t->node[0]=='!') return t;

	t->left=cnf(t->left); t->right=cnf(t->right); 
	if (t->node[0]==landor[0]) return t;
	if (!(t->left->node[0]==landor[0]||t->right->node[0]==landor[0])) 
	return t;

	if (t->left->node[0]!=landor[0]&&t->right->node[0]==landor[0]) { 
		t0=t->left; t->left=t->right; 
		t->right=t0;
	}
		
	t0=(tree *)malloc(sizeof(tree));
	t1=(tree *)malloc(sizeof(tree));
	t0->node[0]=landor[1]; t0->node[1]=0; 
	t0->left=t->left->left; t0->right=t->right;
	t1->node[0]=landor[1]; t1->node[1]=0; 
	t1->left=t->left->right; t1->right=treecopy(t->right);
	t->node[0]=landor[0]; free(t->left); t->left=cnf(t0); t->right=cnf(t1); 
	return t;
}
/*******************************************************************/
tree *forsimplify_basics(tree *t)
{
	tree *t0;
	int i;

	if (t==NULL) return t;
	switch (t->node[0]) {
		case '&': 
			if (t->right->node[0]==FALSE) {
				t0=t->right; treefree(t->left); free(t);
				return t0;
			} 
			if (t->right->node[0]==TRUE) {
				t0=t->left; treefree(t->right); free(t);
				return t0;
			} 
			if (t->left->node[0]==FALSE) {
				t0=t->left; treefree(t->right); free(t);
				return t0;
			} 
			if (t->left->node[0]==TRUE) {
				t0=t->right; treefree(t->left); free(t);
				return t0;
			} 
			break;
		case '|': 
			if (t->right->node[0]==TRUE) {
				t0=t->right; treefree(t->left); free(t);
				return t0;
			} 
			if (t->right->node[0]==FALSE) {
				t0=t->left; treefree(t->right); free(t);
				return t0;
			} 
			if (t->left->node[0]==TRUE) {
				t0=t->left; treefree(t->right); free(t);
				return t0;
			} 
			if (t->left->node[0]==FALSE) {
				t0=t->right; treefree(t->left); free(t);
				return t0;
			} 
			break;
		case '!': 
			if (t->right->node[0]==TRUE) {
				t0=t->right; free(t); t0->node[0]=FALSE; 
				return t0;
			} 
			if (t->right->node[0]==FALSE) {
				t0=t->right; free(t); t0->node[0]=TRUE; 
				return t0;
			} 
			break;
	}
	return t;
}
tree *forsimplify(tree *t)
{
	if (t==NULL) return t;
	t->left=forsimplify(t->left); t->right=forsimplify(t->right); 
	return forsimplify_basics(t);
}
/*******************************************************************/
tree *doall(tree *t,tree *f())
{
	if (isprop(t)) return f(t);
	t->left=doall(t->left,f); t->right=doall(t->right,f); return t;
}
/*******************************************************************/
tree *dnf(tree *t)
{
	tree *t0,*t1;
	char landor[4]="|&";

	if (t==NULL) return t;
	if (isatomic(t->node)||t->node[0]=='!') return t;

	t->left=cnf(t->left); t->right=cnf(t->right); 
	if (t->node[0]==landor[0]) return t;
	if (!(t->left->node[0]==landor[0]||t->right->node[0]==landor[0])) 
	return t;

	if (t->left->node[0]!=landor[0]&&t->right->node[0]==landor[0]) { 
		t0=t->left; t->left=t->right; 
		t->right=t0;
	}
		
	t0=(tree *)malloc(sizeof(tree));
	t1=(tree *)malloc(sizeof(tree));
	t0->node[0]=landor[1]; t0->node[1]=0; 
	t0->left=t->left->left; t0->right=t->right;
	t1->node[0]=landor[1]; t1->node[1]=0; 
	t1->left=t->left->right; t1->right=treecopy(t->right);
	t->node[0]=landor[0]; free(t->left); t->left=cnf(t0); t->right=cnf(t1); 
	return t;
}
/*******************************************************************/
