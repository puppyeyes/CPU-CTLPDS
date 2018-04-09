#include "conv.h"
#include "form.h"

/*******************************************************************/
tree *eqstate(int pa,int i,int pb,int j,int k)
{
	tree *t1=(tree *)malloc(sizeof(tree));
	tree *t2=(tree *)malloc(sizeof(tree));

	sprintf(t1->node,"%i",(pa-1)*(k+1)+i); t1->left=NULL; t1->right=NULL; 
	sprintf(t2->node,"%i",(pb-1)*(k+1)+j); t2->left=NULL; t2->right=NULL; 
	return treecompose("=",t1,t2);
}
/*******************************************************************/
tree *eqpath(int pa,int k)
{
	int i,j;
	tree *t0;
	
	if (k==0) return treeconst(0);
	t0=eqstate(pa,0,pa,1,k);

	for (i=0;i<k;i++)
	for (j=i+1;j<=k;j++) { 
		if (j==1) continue;
		t0=treecompose("|",t0,eqstate(pa,i,pa,j,k));
	}
	return t0;
}
/*******************************************************************/
tree *constructeq(int a,int b,tree *vars,int bn)
{
	tree *t;
	tree *t1=(tree *)malloc(sizeof(tree));

	t=treeaddeq_constructeq(vars);
	strcpy(t1->node,":"); t1->left=NULL; t1->right=t; 
	treeaddx(t1); free(t1);
	treeaddvc(t,vars); 
	replacevariable(t,vars,bn);
	return treetr(t,a,b,bn);
}
/*******************************************************************/
tree *actlreplaceeq(tree *t,tree *vars,int bn)
{
	int a,b;
	tree *t0;

	if (t==NULL) return t;
	if (t->node[0]=='=') {
		a=atoi(t->left->node); free(t->left);
		b=atoi(t->right->node); free(t->right);
		t0=constructeq(a,b,vars,bn);
		strcpy(t->node,t0->node); t->left=t0->left; t->right=t0->right;
		free(t0);
		return t;
	}
	t->left=actlreplaceeq(t->left,vars,bn);
	t->right=actlreplaceeq(t->right,vars,bn); return t;
}
/*******************************************************************/
int fkxy(tree *t,int k,int lpath)
{
	int x,y,z;
	if (isprop(t)) return 0;
	if (t->node[0]=='X') lpath=(lpath+1)%2;
	switch (t->node[0]) {
		case '&': x=fkxy(t->left,k,lpath); y=fkxy(t->right,k,lpath); 
			  return x>y?x:y;
		case '|': x=fkxy(t->left,k,lpath); y=fkxy(t->right,k,lpath); 
			  return x+y;
		case 'A': return fkxy(t->right,k,lpath); 
		case 'X': return fkxy(t->right,k,lpath)+lpath;
		case 'F': return (k+1)*fkxy(t->right,k,lpath)+lpath;
		case 'G': return fkxy(t->right,k,lpath)+lpath;
		case 'U': x=fkxy(t->left,k,lpath); y=fkxy(t->right,k,lpath);
			  z=(x>y?x:y); return k*z+y+lpath;
		case 'R': x=fkxy(t->left,k,lpath); y=fkxy(t->right,k,lpath); 
			  z=(x>y?x:y); return k*x+z+lpath;
	}
}
/*******************************************************************/
int gkxy(tree *t,int k,int lpath)
{
	int x,y,z;
	if (isprop(t)) return 0;
	if (t->node[0]=='X') lpath=(lpath+1)%2;
	switch (t->node[0]) {
		case '&': x=gkxy(t->left,k,lpath); y=gkxy(t->right,k,lpath); 
			  return x+y;
		case '|': x=gkxy(t->left,k,lpath); y=gkxy(t->right,k,lpath); 
			  return x>y?x:y;
		case 'E': return gkxy(t->right,k,lpath); 
		case 'X': return gkxy(t->right,k,lpath)+lpath;
		case 'F': return gkxy(t->right,k,lpath)+lpath;
		case 'G': return (k+1)*gkxy(t->right,k,lpath)+lpath;
		case 'U': x=gkxy(t->left,k,lpath); y=gkxy(t->right,k,lpath);
			  return k*x+y+lpath;
		case 'R': x=gkxy(t->left,k,lpath); y=gkxy(t->right,k,lpath); 
			  z=(x>y?x:y); return k*z+x+y+lpath;
	}
}
/*******************************************************************/
int fkx(tree *t,int k)
{
	int x,y,z;
	if (isprop(t)) return 0;
	switch (t->node[0]) {
		case '&': x=fkx(t->left,k); y=fkx(t->right,k); return x>y?x:y;
		case '|': x=fkx(t->left,k); y=fkx(t->right,k); return x+y;
		case 'A': return fkx(t->right,k)+1; 
		case 'X': return fkx(t->right,k);
		case 'F': return (k+1)*fkx(t->right,k);
		case 'G': return fkx(t->right,k);
		case 'U': x=fkx(t->left,k); y=fkx(t->right,k);
			  z=(x>y?x:y); return k*y+z;
		case 'R': x=fkx(t->left,k); y=fkx(t->right,k); 
			  z=(x>y?x:y); return k*x+z;
	}
}
/*******************************************************************/
int gkx(tree *t,int k)
{
	int x,y,z;
	if (isprop(t)) return 0;
	switch (t->node[0]) {
		case '&': x=gkx(t->left,k); y=gkx(t->right,k); return x+y;
		case '|': x=gkx(t->left,k); y=gkx(t->right,k); return x>y?x:y;
		case 'E': return gkx(t->right,k)+1; 
		case 'X': return gkx(t->right,k);
		case 'F': return gkx(t->right,k);
		case 'G': return (k+1)*gkx(t->right,k);
		case 'U': x=gkx(t->left,k); y=gkx(t->right,k); 
			  return k*x+y;
		case 'R': x=gkx(t->left,k); y=gkx(t->right,k); 
			  return (k+1)*y+x;
	}
}
/*******************************************************************/

/*******************************************************************/
int formulatype(tree *t)
{
	if (t==NULL) return 0;
	if (t->node[0]=='A'||t->node[0]=='E') return 1;
	return formulatype(t->left)||formulatype(t->right); 
}
/*******************************************************************/
tree *treetr(tree *tr,int a,int b,int pn)
{
	return 
	addnumberconditional(addnumber(treecopy(tr),a*pn),(b-a-1)*pn,(a+1)*pn);
}
/*******************************************************************/
tree *addnumber(tree *t,int i)
{
	if (t==NULL) return NULL;
	if (isdigit(t->node[0])) sprintf(t->node,"%i",atoi(t->node)+i); 
	t->left=addnumber(t->left,i); t->right=addnumber(t->right,i); return t;
}
/*******************************************************************/
tree *addnumberconditional(tree *t,int i,int c)
{
	int n;
	if (t==NULL) return NULL;
	if (isdigit(t->node[0])) {
		n=atoi(t->node); if (n>c) sprintf(t->node,"%i",n+i); 
	}
	t->left=addnumberconditional(t->left,i,c); 
	t->right=addnumberconditional(t->right,i,c); return t;
}
/*******************************************************************/
void cnfwrite(int *mstring,int mstringc)
{
	int i;
	for (i=0;i<mstringc;i++)
	if (mstring[i]) fprintf(stdout," %i ",mstring[i]);
	else fprintf(stdout," %i\n",mstring[i]);
}
/*******************************************************************/
void cnfwritestring(int *mstring,int mstringc,model *mo)
{
	int i;
	int *mm=mo->mstring+mo->l;

	if ((mo->l+=mstringc)>MAXSTRING) error("MAXSTRING"); 
	for (i=0;i<mstringc;i++) { *mm++=mstring[i]; if (!mstring[i]) mo->m++; }
}
/*******************************************************************/

/*******************************************************************/
void conv2write(int *s1,int *s2,int n,int a1,int a2,int b1,int b2,int bn,int aux1,int auxp)
{
	int j,s;
	int a1a,a1b,a2a,a2b,b1a,b2a,aux1a;

	a1a=(a1-1)*bn+1; a1b=a1*bn;
	a2a=(a2-1)*bn+1; a2b=a2*bn;
	b1a=(b1-1)*bn+1-a1a; b2a=(b2-1)*bn+1-a2a;
	aux1a=auxp-aux1;

	for (j=0;j<n;j++) 
	if (s1[j]==0) s2[j]=0; else {
	    if (s1[j]>0) s=1; else s=-1; 
	    if (a1a<=s*s1[j]&&s*s1[j]<=a1b) s2[j]=s1[j]+b1a*s;
	    else if (a2a<=s*s1[j]&&s*s1[j]<=a2b) s2[j]=s1[j]+b2a*s;
	    else s2[j]=s1[j]+aux1a*s;
	}
}
/*******************************************************************/
model *conv2model(tree *init,tree *trans,tree *spec,int bn,int k,int auxp)
{
	int *tmstring,*tm2string,*mm,*m1;
	int i,j,n,a,s,q,np;
	int aux1,aux0;
	model *mo;
	tree *tr;

	mo=(model *)malloc(sizeof(model)); 
	mo->mstring=(int *)malloc(sizeof(int)*MAXSTRING); 
	mo->n=mo->m=mo->l=0; 

	tmstring=(int *)malloc(sizeof(int)*MAXSTRING); *tmstring=0;
	tm2string=(int *)malloc(sizeof(int)*MAXSTRING); 

	tr=negate(treecopy(init)); tr=doall(tr,nnf);
	aux1=auxp; auxp=f2cnf(tr,0,0,auxp,tmstring); treefree(tr);
	aux0=auxp-aux1;
	for (n=0,i=2;i<=k+1;i++,n++) {
		conv2write(tmstring+1,tm2string+*tmstring*n,
			*tmstring,1,1,i,i,bn,aux1,aux1+n*aux0);
	}
	auxp=aux1+n*aux0;
	cnfwritestring(tm2string,*tmstring*n,mo);

	*tmstring=0; 
	tr=negate(treecopy(trans)); tr=doall(tr,nnf); treet0(tr,lcnf);
	aux1=auxp; auxp=f2cnf(tr,0,0,auxp,tmstring); treefree(tr);
	aux0=auxp-aux1;

	for (n=0,i=1;i<=k+1;i++) {
		for (j=i+2;j<=k+1;j++,n++) {
			conv2write(tmstring+1,tm2string+*tmstring*n,
				*tmstring,1,2, i,j,bn,aux1,aux1+n*aux0);
		}
	}
	auxp=aux1+n*aux0;
	cnfwritestring(tm2string,*tmstring*n,mo);

	free(tmstring); free(tm2string);
	mo->n=auxp; mo->mstring=(int *)realloc(mo->mstring,sizeof(int)*(mo->l));
	return mo;
}

/*******************************************************************/
tree *spec2f1(tree *spec,int bn,int k,tree *vars,tree *tr)
{
	tree *t0,*t1;

	t0=treecopy(spec);
	if (formulatype(t0)==1) {
		t1=actl2f1(t0,k,vars,bn); 
	} else { 
		t0=negate(t0); /* t0=doall(t0,cnf); */
		t1=ltl2f1(t0,k,0,bn); 
	} 

	treefree(t0); return doall(treeconsttest(doall(t1,forsimplify)),nnf);
}
tree *spec2f0(tree *spec,int bn,int k,tree *vars,tree *tr)
{
	tree *t0,*t1;

	t0=treecopy(spec);
	if (formulatype(t0)==1) {
		t0=negate(t0); 
		t1=actl2f0(t0,k,vars,bn);
	} else { 
		t0=negate(t0); /* t0=doall(t0,cnf); */
		t1=ltl2f0(t0,k,-1,bn,tr); 
	} 

	treefree(t0); return doall(treeconsttest(doall(t1,forsimplify)),nnf);
}
/*******************************************************************/
model *convmodel(tree *init,tree *trans,tree *spec,int bn,int k,int npath)
{
	int *tmstring,*mm,*m1;
	int i,j,n,a,s,q,np;
	int auxp;
	model *mo;
	tree *tr;

	mo=(model *)malloc(sizeof(model)); 
	mo->mstring=(int *)malloc(sizeof(int)*MAXSTRING); 
	mo->n=mo->m=mo->l=0; 

	tmstring=(int *)malloc(sizeof(int)*MAXSTRING); 
	auxp=bn*(k+1)*npath;

	tr=treecopy(init);
	*tmstring=0; auxp=f2cnf(tr,0,0,auxp,tmstring); treefree(tr);
	cnfwritestring(tmstring+1,*tmstring,mo);

	tr=treecopy(trans);
	*tmstring=0; auxp=f2cnf(tr,0,0,auxp,tmstring); treefree(tr);
	/* cnfwrite(tmstring+1,*tmstring); nl; */

	n=*tmstring*k*npath; /*tmstring=realloc(tmstring,sizeof(int)*n+1);*/
	mm=tmstring+1+*tmstring;

	q=auxp-bn*(k+1)*npath; a=2*bn; 
	for (np=0;np<npath;np++)
	for (i=0;i<k;i++) {
		if (np==0&&i==0) continue;
		for (j=1;j<=*tmstring;j++) 
		if (tmstring[j]==0) *mm++=0; else {
		    if (tmstring[j]>0) s=1; else s=-1; 
		    if (s*tmstring[j]<=a) *mm++=tmstring[j]+(i+(np)*(k+1))*bn*s;
		    else *mm++=tmstring[j]+(i+(np)*(k))*q*s; 
		}
	}
	auxp=auxp+q*(k-1)+q*(npath-1)*k;
	cnfwritestring(tmstring+1,n,mo);
/*
	tr=treecopy(spec);
	*tmstring=0; auxp=f2cnf(tr,0,0,auxp,tmstring); treefree(tr);
	cnfwritestring(tmstring+1,*tmstring,mo);
*/
	free(tmstring);
	mo->n=auxp;
	mo->mstring=(int *)realloc(mo->mstring,sizeof(int)*(mo->l));

	return mo;
}
model *convmodelspec(tree *spec,int auxp)
{
	int *tmstring;
	model *mo;
	tree *tr;

	mo=(model *)malloc(sizeof(model)); 
	mo->mstring=(int *)malloc(sizeof(int)*MAXSTRING); 
	mo->n=mo->m=mo->l=0; 

	tmstring=(int *)malloc(sizeof(int)*MAXSTRING); *tmstring=0; 
	tr=treecopy(spec); auxp=f2cnf(tr,0,0,auxp,tmstring); treefree(tr);
	cnfwritestring(tmstring+1,*tmstring,mo); free(tmstring);

	mo->n=auxp;
	mo->mstring=(int *)realloc(mo->mstring,sizeof(int)*(mo->l));
	return mo;
}
model *concatmodel(model *m1,model *m2)
{
	int i;
	int *p1,*p2;
	int *mstring;

	m1->mstring=(int *)realloc(m1->mstring,(m1->l+m2->l)*sizeof(int)); 
	m1->n=m2->n; m1->m+=m2->m; 

	p1=m1->mstring+m1->l; 
	for (p2=m2->mstring,i=0;i<m2->l;i++) { *p1++=*p2++; } m1->l+=m2->l; 
	free(m2->mstring); free(m2);

	return m1;
}

/*******************************************************************/
tree *actl2f1a(tree *t,int k,int pn,int pa,int x,int y)
{
	tree *t0,*t1,*t2; 

	switch (t->node[0]) {
		case 'A': t1=actl2f1a(t->right,k,pn,pa+1,pa+1,0);
			  if (x==0) return t1;
			  t0=eqstate(x,y,pa+1,0,k);
			  return treecompose(":",t0,t1);
		case 'X': if (y==k) return treeconst(0);
			  return t1=actl2f1a(t->right,k,pn,pa,x,y+1);
		case 'F': t0=actl2f1a(t->right,k,pn,pa,x,y); 
			  if (y==k) return t0;
			  pa+=fkx(t->right,k);
			  t2=actl2f1a(t,k,pn,pa,x,y+1); 
			  return treecompose("|",t0,t2);
		case 'G': t0=actl2f1a(t->right,k,pn,pa,x,y); 
			  if (y==k) return treecompose("&",t0,eqpath(x,k));
			  t2=actl2f1a(t,k,pn,pa,x,y+1); 
			  return treecompose("&",t0,t2);
		case 'U': t0=actl2f1a(t->right,k,pn,pa,x,y); 
			  if (y==k) return t0;
			  pa+=fkx(t->right,k);
			  t1=actl2f1a(t->left,k,pn,pa,x,y); 
			  t2=actl2f1a(t,k,pn,pa,x,y+1); 
			  return treecompose("|",t0,treecompose("&",t1,t2));
		case 'R': t0=actl2f1a(t->right,k,pn,pa,x,y); 
			  t1=actl2f1a(t->left,k,pn,pa,x,y); 
			  if (y==k) return treecompose("&",t0, 
				treecompose("|",t1,eqpath(x,k)));
			  pa+=fkx(t->left,k);
			  t2=actl2f1a(t,k,pn,pa,x,y+1); 
			  return treecompose("&",t0,treecompose("|",t1,t2));
		case '&': t0=actl2f1a(t->left,k,pn,pa,x,y); 
			  t1=actl2f1a(t->right,k,pn,pa,x,y); 
			  return treecompose("&",t0,t1);
		case '|': t0=actl2f1a(t->left,k,pn,pa,x,y); 
			  t1=actl2f1a(t->right,k,pn,pa,x,y); 
			  return treecompose("|",t0,t1);
		case '!': 
		default:  return addnumber(treecopy(t),(x-1)*pn*(k+1)+y*pn);
	}
}
tree *actl2f1(tree *t,int k,tree *vars,int pn)
{
	tree *t0;

	t0=actl2f1a(t,k,pn,0,0,0); 
	t0=actlreplaceeq(t0,vars,pn);
	return negate(t0);
}
/*******************************************************************/
tree *actl2f0a(tree *t,int k,int pn,int pa,int x,int y)
{
	tree *t0,*t1,*t2; 

	switch (t->node[0]) {
		case 'E': t1=actl2f0a(t->right,k,pn,pa+1,pa+1,0);
			  if (x==0) return t1;
			  t0=eqstate(x,y,pa+1,0,k);
			  return treecompose(":",t0,t1);
		case 'X': if (y==k) return treeconst(0);
			  return t1=actl2f0a(t->right,k,pn,pa,x,y+1);
		case 'F': t0=actl2f0a(t->right,k,pn,pa,x,y); 
			  if (y==k) return t0;
			  t2=actl2f0a(t,k,pn,pa,x,y+1); 
			  return treecompose("|",t0,t2);
		case 'G': t0=actl2f0a(t->right,k,pn,pa,x,y); 
			  pa+=gkx(t->right,k);
			  if (y==k) return treecompose("&",t0,eqpath(x,k));
			  t2=actl2f0a(t,k,pn,pa,x,y+1); 
			  return treecompose("&",t0,t2);
		case 'U': t0=actl2f0a(t->right,k,pn,pa,x,y); 
			  if (y==k) return t0;
			  t1=actl2f0a(t->left,k,pn,pa,x,y); 
			  pa+=gkx(t->left,k);
			  t2=actl2f0a(t,k,pn,pa,x,y+1); 
			  return treecompose("|",t0,treecompose("&",t1,t2));
		case 'R': t0=actl2f0a(t->right,k,pn,pa,x,y); 
			  pa+=gkx(t->right,k);
			  t1=actl2f0a(t->left,k,pn,pa,x,y); 
			  if (y==k) return treecompose("&",t0,
				treecompose("|",t1,eqpath(x,k)));
			  t2=actl2f0a(t,k,pn,pa,x,y+1); 
			  return treecompose("&",t0,treecompose("|",t1,t2));
		case '&': t0=actl2f0a(t->left,k,pn,pa,x,y); 
			  t1=actl2f0a(t->right,k,pn,pa,x,y); 
			  return treecompose("&",t0,t1);
		case '|': t0=actl2f0a(t->left,k,pn,pa,x,y); 
			  t1=actl2f0a(t->right,k,pn,pa,x,y); 
			  return treecompose("|",t0,t1);
		case '!': 
		default:  return addnumber(treecopy(t),(x-1)*pn*(k+1)+y*pn);
	}
}
tree *actl2f0(tree *t,int k,tree *vars,int pn)
{
	tree *t0;

	t0=actl2f0a(t,k,pn,0,0,0); 
	t0=actlreplaceeq(t0,vars,pn);
        t0=treetraverse(t0,NULL,NULL,cleanbool);
	return t0;
}

/*******************************************************************/
#define myprints0(f,s0,m) 	{ if (s0) m[++*m]=-s0; m[++*m]=0; }
#define myprints(f,s,m)   	{ m[++*m]=s; }
/*******************************************************************/
int convmodel_one1(tree *t,char c,int s0,int s,int *mstring)
{
	int i;
	if (t==NULL) return s;
	if (t->node[0]==c) { 
		i=convmodel_one1(t->left,c,s0,s,mstring); 
		i=convmodel_one1(t->right,c,s0,i,mstring); return i;
	} 
	if (t->node[0]=='!') {
		myprints(stdout,-atoi(t->right->node),mstring);
		if (c=='&') myprints0(stdout,s0,mstring); 
	} else if (isdigit(t->node[0])) {
		myprints(stdout,atoi(t->node),mstring);
		if (c=='&') myprints0(stdout,s0,mstring); 
	} else if (c=='&') {
		sprintf(t->node+1,"%i",s0); 
	} else /*if (c=='|')*/ {
		myprints(stdout,++s,mstring); sprintf(t->node+1,"%i",s);
		if (c=='&') myprints0(stdout,s0,mstring); 
	}
	return s;
}
int convmodel_one2(tree *t,char c,int s,int *mstring)
{
	int i;
	if (t==NULL) return s;
	if (t->node[0]==c) { 
		i=convmodel_one2(t->left,c,s,mstring); 
		i=convmodel_one2(t->right,c,i,mstring); return i;
	} 
	if (t->node[0]=='!') return s; 
	else if (isdigit(t->node[0])) return s;
	else {
		return f2cnf(t,t->node[0],atoi(t->node+1),s,mstring);
	}
}
int f2cnf(tree *t,char c,int s0,int auxp,int *mstring)
{
	int i;

	if (t==NULL) return auxp;
	if (c==0) c=t->node[0];
	i=convmodel_one1(t,c,s0,auxp,mstring); 
	if (c=='|') myprints0(stdout,s0,mstring); 
	i=convmodel_one2(t,c,i,mstring); 
	return i;
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
int lcnf(tree *t)
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

/*******************************************************************/
tree *makeform_ll(tree *f(),tree *t,int k,int i,int j,char c,int pn)
{
	tree *t0,*t1;;

	if (i>j) return NULL;
	if (i==j) return f(t,k,i,pn); 
	t0=(tree *)malloc(sizeof(tree));	
	t0->node[0]=c; t0->node[1]=0; 
	t0->left=f(t,k,i,pn); t0->right=makeform_ll(f,t,k,i+1,j,c,pn); 
	return t0;
}
/*******************************************************************/
tree *ltl2f1(tree *t,int k,int i,int pn)
{
	tree *t0,*t1,*t2; 

	switch (t->node[0]) {
		case 'X': if (k==i) return treeconst(1);
			  else return ltl2f1(t->right,k,i+1,pn); 
		case 'G': return makeform_ll(ltl2f1,t->right,k,i,k,'&',pn); 
		case 'F': return treeconst(1);
		case 'U': t0=ltl2f1(t->right,k,i,pn); 
			  t1=ltl2f1(t->left,k,i,pn);
			  if (i==k) return treecompose("|",t0,t1);
			  t2=ltl2f1(t,k,i+1,pn); 
			  return treecompose("|",t0,treecompose("&",t1,t2));
		case 'R': t0=ltl2f1(t->right,k,i,pn); 
			  if (i==k) return t0;
			  t1=ltl2f1(t->left,k,i,pn);
			  t2=ltl2f1(t,k,i+1,pn); 
			  return treecompose("&",t0,treecompose("|",t1,t2));
		case '&': t0=ltl2f1(t->left,k,i,pn); 
			  t1=ltl2f1(t->right,k,i,pn); 
			  return treecompose("&",t0,t1);
		case '|': t0=ltl2f1(t->left,k,i,pn); 
			  t1=ltl2f1(t->right,k,i,pn); 
			  return treecompose("|",t0,t1);
		case '!': 
		default:  return addnumber(treecopy(t),i*pn);
	}
}
/*******************************************************************/
tree *makeform_l0(tree *f(),tree *t,int k,int i,int j,char c,int pn,int ll)
{
	tree *t0,*t1;;

	if (i>j) return NULL;
	if (i==j) return f(t,k,i,pn,ll); 
	t0=(tree *)malloc(sizeof(tree));	
	t0->node[0]=c; t0->node[1]=0; 
	t0->left=f(t,k,i,pn,ll,0); t0->right=makeform_l0(f,t,k,i+1,j,c,pn,ll); 
	return t0;
}
/*******************************************************************/
tree *ltl2f0b(tree *t,int k,int i,int pn,int ll,int j)
{
	tree *t0,*t1,*t2,*t3; 

	switch (t->node[0]) {
		case 'X': if (k==i&&ll>=0) i=ll; else i=i+1;
			  if (i>k) return treeconst(0); 
			  return ltl2f0b(t->right,k,i+1,pn,ll,j); 
		case 'G': if (ll<0) return treeconst(0); 
			  if (ll<i) i=ll;
			  return makeform_l0(ltl2f0b,t->right,k,i,k,'&',pn,ll);
		case 'F': if (i>ll&&ll>=0) i=ll;
			  return makeform_l0(ltl2f0b,t->right,k,i,k,'|',pn,ll); 
		case 'U': t->node[0]++; t0=ltl2f0b(t,k,i,pn,ll,k); t->node[0]--;
			  if (!(i>ll&&ll>=0)) return t0;
			  t1=makeform_l0(ltl2f0b,t->left,k,i,k,'&',pn,ll); 
			  t->node[0]++; t2=ltl2f0b(t,k,ll,pn,ll,i-1); 
			  t->node[0]--;
			  return treecompose("|",t0,treecompose("&",t1,t2));
		case 'R': t->node[0]++; t0=ltl2f0b(t,k,i,pn,ll,k); t->node[0]--;
			  if (!(ll>=0)) return t0;
			  t1=makeform_l0(ltl2f0b,t->left,k,i,k,'&',pn,ll); 
		          if (i<=ll) return treecompose("|",t0,t1);
			  t->node[0]++; t2=ltl2f0b(t,k,ll,pn,ll,i-1); 
			  t->node[0]--;
			  t3=makeform_l0(ltl2f0b,t->right,k,ll,i-1,'&',pn,ll); 
			  t2=treecompose("|",t2,t3);
			  return treecompose("|",t0,treecompose("&",t1,t2));
		case 'V': t0=ltl2f0b(t->right,k,i,pn,ll,j); 
			  if (i==j) return t0;
			  t1=ltl2f0b(t->left,k,i,pn,ll,j); 
			  t2=ltl2f0b(t,k,i+1,pn,ll,j); 
			  return treecompose("|",t0,treecompose("&",t1,t2));
		case 'S': t0=ltl2f0b(t->right,k,i,pn,ll,j); 
			  t1=ltl2f0b(t->left,k,i,pn,ll,j); 
			  if (i==j) return treecompose("&",t0,t1);
			  t2=ltl2f0b(t,k,i+1,pn,ll,j); 
			  return treecompose("&",t0,treecompose("|",t1,t2));
		case '&': t0=ltl2f0b(t->left,k,i,pn,ll,j); 
			  t1=ltl2f0b(t->right,k,i,pn,ll,j); 
			  return treecompose("&",t0,t1);
		case '|': t0=ltl2f0b(t->left,k,i,pn,ll,j); 
			  t1=ltl2f0b(t->right,k,i,pn,ll,j); 
			  return treecompose("|",t0,t1);
		case '!': 
		default:  return addnumber(treecopy(t),i*pn);
	}
}
/*******************************************************************/
tree *ltl2f0(tree *t,int k,int lstart,int pn,tree *tr)
{
	tree *t0,*t1; 

	if (lstart==-1) {
		t0=(tree *)malloc(sizeof(tree));	
		t0->node[0]='|'; t0->node[1]=0; 
		t0->left=ltl2f0b(t,k,0,pn,lstart,0); 
		t0->right=ltl2f0(t,k,lstart+1,pn,tr); 
		return t0;
	} else { 
		t0=(tree *)malloc(sizeof(tree));
		t0->node[0]='&'; t0->node[1]=0; 
		t0->left=treetr(tr,k,lstart,pn);
		t0->right=ltl2f0b(t,k,0,pn,lstart,0); if (lstart==k) return t0;
		t1=(tree *)malloc(sizeof(tree));
		t1->node[0]='|'; t1->node[1]=0; 
		t1->left=t0; 
		t1->right=ltl2f0(t,k,lstart+1,pn,tr); return t1;
	}
}
