#include "opt1.h"
#include "opt1.h"

/************************************************************************/
LOCAL char *O1lits;
LOCAL int O1threshold;
LOCAL int O1thresholdstep;
/************************************************************************/
#define expc(a,b)	(a+b)
/************************************************************************/
#define expcond(i)	(1)
/*
#define expcond(i)	(expc(cc->LL[i],cc->LL[-i])>=O1threshold)
/************************************************************************/
int O1prepare(model *mm,int low,int high,FILE *g,int stage)
{
	static mstruct *cc;
	static char *clauseind;
	int i,k;
	int *p;

	pprts("O1threshold,stage");
	pprti(O1threshold);
	pprti(stage);

	switch (stage) {
		case 2: freemdata(cc,NULL,mm->n); 
			free(clauseind); free(O1lits-mm->n); return 0;
		case 0: cc=model2mstruct(mm);
			O1threshold=O1calculate1(cc,mm->n);
			O1thresholdstep=O1threshold/1+1;
/*
			O1threshold=O1thresholdstep;
*/
        		clauseind=(char *)calloc((mm->m),sizeof(char));
			O1lits=(char *)calloc((2*mm->n+1),sizeof(char))+mm->n;
			for (i=0;i<mm->m;i++) if (clauseind[i]==0) {
				p=cc->Clause[i];
				while (*p) 
				{ if (O1lits[*p]) clauseind[i]=1; p++; }
			}
			return 0;
	}
	for (i=0;i<mm->m;i++) if (clauseind[i]) {
		p=cc->Clause[i];
		while (*p) { if (expcond(*p)) O1lits[*p]=O1lits[-*p]=1; p++; }
	}
	for (i=0;i<mm->m;i++) if (clauseind[i]==0) {
		p=cc->Clause[i];
		while (*p) { if (O1lits[*p]) clauseind[i]=1; p++; }
	}
	for (k=0,i=0;i<mm->m;i++) if (clauseind[i]) k++;
	cnfmstructwrite(mm,cc,clauseind,k,g);

	O1threshold-=O1thresholdstep;

	return k;
}
/************************************************************************/
void O1rmodelappend(model *mm,char *fn,int max,FILE *g)
{
	int *literals=(int *)malloc((mm->n+1)*sizeof(int));
	char *ll=file2string(fn);
	int i=0;
	int *la=literals;
	char *l0=ll;
	int a;

	while (1) {
		while (*l0&&*l0!='-'&&!isdigit(*l0)) l0++; 
		if (*l0==0) { *la=0; break; }
		*la=atoi(l0); if (*la==0) break; 
		if (literals[*la]&&*la<=max) la++;
		while (*l0=='-'||isdigit(*l0)) l0++; 
		i++; if (i>mm->n) error("rmodel");
	}
	free(ll);
	cnfmodelappendwrite(mm,literals,g);
	free(literals);
}
/************************************************************************/
void O1prepareliterals(tree *spec,int nstates,int booln)
{
	int i,b;

	if (spec==NULL) return;
	if (!isdigit(spec->node[0])) {
		O1prepareliterals(spec->left,nstates,booln);
		O1prepareliterals(spec->right,nstates,booln);
		return;
	}
/*
	b=atoi(spec->node);
	for (i=1;i<=nstates;i++) { O1lits[b]=O1lits[-b]=1; b+=booln; }
*/
	b=atoi(spec->node);
	b=rand()%booln+1;
	for (i=1;i<=nstates/4+1;i++) { O1lits[b]=O1lits[-b]=1; b+=booln; }
}
/************************************************************************/
int O1calculate1(mstruct *cc,int n)
{
	int i,k1,k2;

	for (k2=0,i=1;i<=n;i++) {
		k1=expc(cc->LL[i],cc->LL[-i]); if (k1>k2) k2=k1;
	}
	return k2;
};

/********************************************************************/
int O3repformula(int type,int bv,char *solver,workspace *ws2,tree *sp,int k)
{
	int r;
	model *mm0;
	tree *t0;

	if (type==3&&bv==1) {
		mm0=createnonrepmm0(ws2,k);
		r=SOLVEapply(solver,mm0,1); 
		free(mm0->mstring); free(mm0); 
/*
		if (r) { treereplace(sp,axnonrep(sp,k-1)); }
*/
		if (r) { treereplace(sp,agnonrep(treecopy(sp),k-1)); }
		return r;
	}
	return 0;
}
/*******************************************************************/
tree *axnonrep(tree *spec,int k)
{
        tree *t;
        tree *t0=spec->right->right;

	if (k==0) return treecopy(t0);
	t=axnonrep(spec,k-1);
	t=treecompose("&",treecopy(t0),
	       treecompose("A",NULL,treecompose("X",NULL,t)));
	return t;
}
/*******************************************************************/
tree *agnonrep(tree *spec,int k)
{
	if (spec->right->node[0]=='G') spec->right->left=treenum(k);
	return spec;
/*not correct*******************************************************
	if (spec==NULL) return NULL;
	spec->left=agnonrep(spec->left,k); spec->right=agnonrep(spec->right,k); 
	if (spec->node[0]=='G') spec->left=treenum(k);
/***********************************************NNF ACTL is assumed*/ 
}
/*******************************************************************/
