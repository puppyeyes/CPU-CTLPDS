#include "arit.h"

/*******************************************************************/
#define neg(a)		treecompose("!",NULL,a)
#define lor(a,b) 	treecompose("|",a,b)
#define land(a,b) 	treecompose("&",a,b)
/*******************************************************************/
#define xor(a,b) 	treecompose("&", \
			treecompose("|",treecopy(a),b), \
			treecompose("|",neg(a),neg(treecopy(b)))) \
/*
#define xor(a,b) 	treecompose("|", \
			treecompose("&",treecopy(a),neg(b)), \
			treecompose("&",neg(a),treecopy(b))) \
/*******************************************************************/
#define leq(a,b) 	treecompose("&", \
			treecompose("|",neg(treecopy(a)),treecopy(b)), \
			treecompose("|",a,neg(b))) \
/*
#define leq(a,b) 	treecompose("|", \
			treecompose("&",treecopy(a),treecopy(b)), \
			treecompose("&",neg(a),neg(b))) \
/*******************************************************************/
tree *comblistdlen(tree *f(),tree *x1,tree *x2)
{ 	
	int a1=treelength(x1);
	int a2=treelength(x2);
	tree *r0,*r1;
	tree *t1,*t2;

	t1=treecopy(x1); t2=treecopy(x2);
	for (;a2>a1;a1++) t1=treecompose(";",treecompose(FALSEs,NULL,NULL),t1); 
	for (;a1>a2;a2++) t2=treecompose(";",treecompose(FALSEs,NULL,NULL),t2); 

	r0=comblist(f,t1,t2); treefree(t1); treefree(t2);

	switch (r0->node[0]) {
		case '+': r0->node[0]=';'; return r0;
		case '-': r1=r0->right; free(r0->left); free(r0); return r1;
		default:  return r0;
	}
}
/*******************************************************************/
tree *comblist(tree *f(),tree *l1,tree *l2)
{	
	tree *l0;
	if (l1->node[0]!=';') return f(l1,l2,NULL);
	l0=comblist(f,l1->right,l2->right);
	return f(l1->left,l2->left,l0);
}
/*******************************************************************/
tree *arit_add(tree *s1,tree *s2,tree *s0)
{
	tree *a1,*b1,*c1,*a0,*c0,*r0;

	a1=s1; b1=s2;
	c0=land(treecopy(a1),treecopy(b1));
	a0=xor(treecopy(a1),treecopy(b1));
	if (s0==NULL) return treecompose("+",c0,a0);
	c1=s0->left;
	c0=lor(c0,land(lor(treecopy(a1),treecopy(b1)),treecopy(c1)));
	a0=xor(a0,treecopy(c1));
	r0=treecompose("+",c0,treecompose(";",a0,s0->right));
	treefree(s0->left); free(s0); return r0;
}
/*******************************************************************/
tree *arit_sub(tree *s1,tree *s2,tree *s0)
{
	tree *a1,*b1,*c1,*a0,*c0,*r0;

	a1=s1; b1=s2;
	c0=land(neg(treecopy(a1)),treecopy(b1));
	a0=xor(treecopy(a1),treecopy(b1));
	if (s0==NULL) return treecompose("-",c0,a0);
	c1=s0->left;
	treefree(c0);
	c0=land(treecopy(c1),treecopy(b1));
	c0=lor(c0,land(neg(treecopy(a1)),lor(treecopy(c1),treecopy(b1))));
	a0=xor(a0,treecopy(c1));
	r0=treecompose("-",c0,treecompose(";",a0,s0->right));
	treefree(s0->left); free(s0); return r0;
}
/*******************************************************************/
tree *arit_eq(tree *s1,tree *s2,tree *s0)
{
	tree *a0;

	a0=leq(treecopy(s1),treecopy(s2));
	if (s0==NULL) return a0;
	return treecompose("&",a0,s0); 
}
/*******************************************************************/
void arithsimplify(tree *t)
{
        int a,b,c;

        if (t->left==NULL||!isdigit(t->left->node[0])) return;
        if (t->right==NULL||!isdigit(t->right->node[0])) return;

	a=atoi(t->left->node); b=atoi(t->right->node);
        switch (t->node[0]) {
                case '-': c=a-b; if (c<0) c=1;
                          treereplace(t,treenum(c)); return;
                case '+': treereplace(t,treenum(a+b)); return;
                case '*': treereplace(t,treenum(a*b)); return;
                case '>': treereplace(t,treeconst(a>b)); return;
                case '<': treereplace(t,treeconst(a<b)); return;
                case '=': treereplace(t,treeconst(a==b)); return;
	}
}
void formulasimplify(tree *t)
{
        switch (t->node[0]) {
                case '&': 
                case DAND: 
			  if (t->left->node[0]==FALSE) {
                	  	treereplace(t,treeconst(0)); return;
			  }
                	  if (t->right->node[0]==FALSE) {
                	  	treereplace(t,treeconst(0)); return;
			  }
                	  if (t->left->node[0]==TRUE) {
                	  	treereplace(t,treecopy(t->right)); return;
			  }
                	  if (t->right->node[0]==TRUE) {
                	  	treereplace(t,treecopy(t->left)); return;
			  }
			  break;
                case '|': 
                case BOR: 
                case DOR: 
			  if (t->left->node[0]==TRUE) {
                	  	treereplace(t,treeconst(1)); return;
			  }
                	  if (t->right->node[0]==TRUE) {
                	  	treereplace(t,treeconst(1)); return;
			  }
                	  if (t->left->node[0]==FALSE) {
                	  	treereplace(t,treecopy(t->right)); return;
			  }
                	  if (t->right->node[0]==FALSE) {
                	  	treereplace(t,treecopy(t->left)); return;
			  }
			  break;
		case '!':
                	  if (t->right->node[0]==TRUE) {
				t->node[0]=FALSE; 
				free(t->right); t->right=NULL; return;
			  }
                	  if (t->right->node[0]==FALSE) {
				t->node[0]=TRUE; 
				free(t->right); t->right=NULL; return;
			  }
			  break;
	}
}

void simplifyarrays(tree *t)
{
        if (t==NULL) return;
        simplifyarrays(t->left); simplifyarrays(t->right);
	arithsimplify(t);
	formulasimplify(t);
}
int puretrans(tree *t)
{
	if (t==NULL) return 1;
	if (t->node[0]==';') return puretrans(t->left)&&puretrans(t->right);
	if (t->node[0]==':') return puretrans(t->right);
	if (t->node[0]=='&') return puretrans(t->left)&&puretrans(t->right);
	if (t->node[0]=='=') if (t->left->node[0]=='X') return 1; else return 0;
	return 1;
}

/*******************************************************************/
#define lor(a,b) 	treecompose("|",a,b)
#define land(a,b) 	treecompose("&",a,b)
/*******************************************************************/
tree *arit_bor(tree *s1,tree *s2,tree *s0)
{
	tree *a0;

	a0=lor(treecopy(s1),treecopy(s2));
	if (s0==NULL) return a0;
	return treecompose(";",a0,s0); 
}
/*******************************************************************/
tree *arit_band(tree *s1,tree *s2,tree *s0)
{
	tree *a0;

	a0=land(treecopy(s1),treecopy(s2));
	if (s0==NULL) return a0;
	return treecompose(";",a0,s0); 
}
/*******************************************************************/
tree *comblistdist(tree *f(),tree *x1,tree *x2)
{ 	
	int a1=treelength(x1);
	int a2=treelength(x2);
	tree *r0,*r1;
	tree *t1,*t2;

	t1=treecopy(x1); t2=treecopy(x2);
	for (;a2>a1;a1++) t1=treecompose(";",treecopy(x1),t1); 
	for (;a1>a2;a2++) t2=treecompose(";",treecopy(x2),t2); 

	r0=comblist(f,t1,t2); treefree(t1); treefree(t2);
	return r0;
}
/*******************************************************************/
tree *arraycond(tree *t,int i)
{
	char node[MAXVARLEN];
	char *p0,*p1; 
	tree *t1,*t2;

	if (t==NULL) return NULL;
	if (t->node[0]=='=') {
		strcpy(node,t->left->node);
		p1=node; 
		while (*p1!='[') p1++; p0=p1+1; 
		while (*p1!=']') p1++; *p1=0;
		return treecompose("=",string2tree(p0),treenum(i));
	} else {
		t1=arraycond(t->left,i);
		t2=arraycond(t->right,i);
		return treecompose("|",t1,t2);
	}
}
tree *replacearraybody1construct(tree *t,char *node,int k,int m,int n)
{
	int i;
	char var[MAXVARLEN];
	char nvar[MAXVARLEN];
	tree *tvar,*tcond;
	tree *left,*right;

	strcpy(var+1,node); var[k]=0; var[0]='X';

	for (tvar=NULL,i=n;i>=m;i--) {
		tcond=arraycond(t,i);
		sprintf(nvar,"%s_%i",var,i); left=treecompose(nvar,NULL,NULL);
		nvar[0]='V'; right=treecompose(nvar,NULL,NULL);
		if (tvar==NULL) 
			tvar=
			treecompose("|",tcond,treecompose("=",left,right));
		else 
			tvar=
			treecompose("&",
			treecompose("|",tcond,treecompose("=",left,right)),
			tvar);
	}
	return tvar;
}
tree *array1var(char *node,int m,int n)
{
	int i;
	char node1[MAXVARLEN];
	char *p1=node1;
	char *p0;
	char var[MAXVARLEN];
	char nvar[MAXVARLEN];
	tree *index,*tvar,*tcond;

	strcpy(node1,node);
	i=0; while (*p1&&*p1!='[') var[i++]=*p1++; var[i]=0;
	p0=p1+1;
	while (*p1&&*p1!=']') p1++; 
	if (*p1) { *p1=0; index=string2tree(p0); *p1=']'; } else error("array");

	for (tvar=NULL,i=n;i>=m;i--) {
		tcond=treecompose("=",treecopy(index),treenum(i));
		sprintf(nvar,"%s_%i",var,i);
		if (tvar==NULL) 
			tvar=
			treecompose(DANDs,tcond,treecompose(nvar,NULL,NULL));
		else 
			tvar=
			treecompose(
			BORs,
			treecompose(DANDs,tcond,treecompose(nvar,NULL,NULL)),
			tvar);
	}
	treefree(index); return tvar;
}
/*******************************************************************/
tree *array1varlist(char *node,int m,int n,tree *vd,tree *vnext)
{
	int i=0;
	char *p1=node;
	char var[MAXVARLEN];
	char nvar[MAXVARLEN];
	tree *index,*tvar,*tcond;

	while (*p1&&*p1!='[') var[i++]=*p1++; var[i]=0;

	for (tvar=NULL,i=n;i>=m;i--) {
		sprintf(nvar,"%s_%i",var,i);
		if (tvar==NULL) {
			tvar=treecompose(":",treecompose(nvar,NULL,NULL),
				treecopy(vd));
			if (vnext!=NULL) tvar=treecompose(";",tvar,vnext);
		} else 
			tvar=
			treecompose(
			";",
		    	treecompose(":",treecompose(nvar,NULL,NULL),
				treecopy(vd)),
		    	tvar);
	}
	return tvar;
}
/*******************************************************************/
tree *replacearray(tree *body,tree *vars)
{
	int i=0,k=0;
	char *p1;
	tree *t,*vnext,*v;
	int low,high;

	
	if (vars->node[0]!=';') { vnext=NULL; v=vars; }
	else { vnext=replacearray(body,vars->right); v=vars->left; }

	p1=v->left->node;
	while (*p1&&*p1!='[') { p1++; k++; }
	if (*p1==0) { if (vnext!=NULL) vars->right=vnext; return vars; }

	p1++;
	low=atoi(p1); 
	while (isdigit(*p1)) p1++; while (!isdigit(*p1)) p1++;
	high=atoi(p1); 
	while (*p1&&*p1!='[') p1++; if (*p1) error("array");
	
	replacearraybody1(body->right->left,v->left->node,k+1,low,high);
	replacearraybody2(body,v->left->node,k+1,low,high);
	t=array1varlist(v->left->node,low,high,v->right,vnext);
	treefree(v->left); return t;
}
/*******************************************************************/
void replacearraybody1(tree *t,char *node,int k,int m,int n)
{
	tree *t1,*t2;
	tree *vcur;
	char var[MAXVARLEN];

        if (t==NULL) return;
	if (t->node[0]!=';') { vcur=t; }
	else { replacearraybody1(t->right,node,k,m,n); vcur=t->left; } 

	strcpy(var+1,node); var[k+1]=0; var[0]='X'; 
	t1=getvtree_a(var,vcur->right);
	t2=replacearraybody1construct(t1,node,k,m,n); treefree(t1);
	t1=treecompose("&",treecopy(vcur->right),t2);
	treereplace(vcur->right,t1); 
}       
/*******************************************************************/
void replacearraybody2(tree *t,char *node,int k,int m,int n)
{
	tree *t0;

        if (t==NULL) return;
	if (strncmp(t->node+1,node,k)==0) {
		t0=array1var(t->node,m,n);
		treereplace(t,t0); return; 
	}
        replacearraybody2(t->left,node,k,m,n);
        replacearraybody2(t->right,node,k,m,n);
}       
/*******************************************************************/


/*******************************************************************/
int ischan(char *node)
{
	int i;
	while (*node&&*node!='[') node++;
	if (*node==0) return 0;
	node++; i=atoi(node); while (*node&&isdigit(*node)) node++; 
	if (*node==']') return i;
	return 0;
}
/*******************************************************************/
void replacechan(tree *body,tree *vars)
{
	int j;
	char v1[MAXVARLEN];
	char v2[MAXVARLEN];
	char *p,*q;
	tree *mylist=vars;
	tree *l0;
	tree *var1,*var2;

	while (1) {
		l0=listgete(mylist);
		if (j=ischan(l0->left->node)) {
			p=l0->left->node; strcpy(v1,p); q=v1;
			while (*p!='[') { p++; q++; }
			sprintf(p,"[0..%i]",j-1); sprintf(q,"_");
			sprintf(v2,"0..%i",j); 
			var1=treecompose(v2,NULL,NULL);
			var1=treecompose(":",treecompose(v1,NULL,NULL),var1);
			var2=treecompose(";",var1,treecopy(mylist)); 
			treereplace(mylist,var2); 
			strcpy(q,"[.]");
			replacechanbody(body->right,v1,j);
			replacechaninit(body->left,v1,j);
		}
		if (l0==mylist) break; mylist=mylist->right;
	}
}
/*******************************************************************/
void replacechanbody(tree *t,char *node,int j)
{
	int i;
	char *p1,*p2;
	char var[MAXVARLEN];
	char var2[MAXVARLEN];
	char node1[MAXVARLEN];
	tree *t1,*t2,*t3;

	if (t==NULL) return;
        replacechanbody(t->left,node,j);
        replacechanbody(t->right,node,j);
	if (t->node[0]==CHANW) {
		if (strcmp(t->left->node,node)!=0) return;
		strcpy(node1,node); p1=node1; while (*p1!='[') p1++; *p1=0;
		sprintf(var,"%s%c",node1,'_');
		t1=treecompose("<",treecompose(var,NULL,NULL),treenum(j));
		t2=treecompose("=",treecompose(var,NULL,NULL),
			treecompose("+",treecompose(var,NULL,NULL),treenum(1)));
		sprintf(var,"%s[%s%c]",node1,node1,'_');
		t3=treecompose("=",treecompose(var,NULL,NULL),
				   treecopy(t->right));
		treereplace(t,treecompose("&",t1,treecompose("&",t2,t3)));
	} else if (t->node[0]==CHANR) {
		if (strcmp(t->left->node,node)!=0) return;
		strcpy(node1,node); p1=node1; while (*p1!='[') p1++; *p1=0;
		sprintf(var,"%s%c",node1,'_');
		t1=treecompose(">",treecompose(var,NULL,NULL),treenum(0));
		t2=treecompose("=",treecompose(var,NULL,NULL),
			treecompose("-",treecompose(var,NULL,NULL),treenum(1)));
		sprintf(var,"%s[0]",node1);
		t3=treecompose("=",treecopy(t->right),
			treecompose(var,NULL,NULL));
		t3=treecompose("&",t1,treecompose("&",t2,t3));
		for (i=0;i<j-1;i++) {
			sprintf(var,"%s[%i]",node1,i);
			sprintf(var2,"%s[%i]",node1,i+1);
			t1=treecompose("=",treecompose(var,NULL,NULL),
				treecompose(var2,NULL,NULL));
			t3=treecompose("&",t1,t3);
		}
		treereplace(t,t3); 
	}
}       
/*******************************************************************/
void replacechaninit(tree *t,char *node,int j)
{
	int i;
	char *p1;
	char var[MAXVARLEN];
	tree *t1,*t2;

	p1=node; while (*p1!='[') p1++; *p1=0;
	sprintf(var,"%s%c",node,'_');
	t2=treecompose("=",treecompose(var,NULL,NULL),treenum(0));
	for (i=0;i<j;i++) {
		sprintf(var,"%s[%i]",node,i);
		t1=treecompose("=",treecompose(var,NULL,NULL),treenum(0));
		t2=treecompose("&",t1,t2);
	}
	treereplace(t,treecompose("&",t2,treecopy(t)));
}       
/*******************************************************************/

/*******************************************************************/
#define neg(a)		treecompose("!",NULL,a)
/*******************************************************************/
tree *combarit_less(tree *x1,tree *x2)
{ 	
	int a1=treelength(x1);
	int a2=treelength(x2);
	tree *r0,*r1;
	tree *t1,*t2;

	t1=treecopy(x1); t2=treecopy(x2);
	for (;a2>a1;a1++) t1=treecompose(";",treecompose(FALSEs,NULL,NULL),t1); 
	for (;a1>a2;a2++) t2=treecompose(";",treecompose(FALSEs,NULL,NULL),t2); 

	r0=comblist(arit_sub,t1,t2); treefree(t1); treefree(t2);
	r1=r0->left; free(r0->right); free(r0); 
	return r1;
}
tree *combarit_greater(tree *x1,tree *x2)
{ 	
	int a1=treelength(x1);
	int a2=treelength(x2);
	tree *r0,*r1;
	tree *t1,*t2;

	t1=treecopy(x1); t2=treecopy(x2);
	for (;a2>a1;a1++) t1=treecompose(";",treecompose(FALSEs,NULL,NULL),t1); 
	for (;a1>a2;a2++) t2=treecompose(";",treecompose(FALSEs,NULL,NULL),t2); 

	r0=comblist(arit_sub,t1,t2); 
	r1=comblist(arit_eq,t1,t2); treefree(t1); treefree(t2);

	r1=treecompose("&",neg(r0->left),neg(r1));
	free(r0->right); free(r0); return r1;
}

/*******************************************************************/
int ismarray(char *node)
{
	int i=1;
	while (*node&&*node!='[') { node++; i++; }
	if (*node==0) return 0;
	node++; while (*node&&*node!='[') node++; 
	if (*node==0) return 0;
	return i;
}
/*******************************************************************/
void marraycount(char *node,int *ma)
{
	*ma=0;
	while (*node)
	if (*node=='[') {
			while (*node!='.') node++; 
			while (!isdigit(*node)) node++;
			ma[++*ma]=atoi(node)+1;
	} else node++;
}
/*******************************************************************/
void replacemarray(tree *body,tree *vars)
{
	int i,j,mb;
	int ma[32]; 
	tree *mylist=vars;
	tree *l0;
	char *p1;

	while (1) {
		l0=listgete(mylist);
		if (j=ismarray(l0->left->node)) {
			marraycount(l0->left->node,ma);
			for (mb=1,i=1;i<=ma[0];i++) mb*=ma[i];
			p1=l0->node;
			while (*p1!='[') p1++; 
			sprintf(p1,"[0..%i]",mb-1); 
			replacemarraybody(body,l0->left->node,j,ma);
		}
		if (l0==mylist) break;
		mylist=mylist->right;
	}
}
/*******************************************************************/
void replacemarraybody(tree *t,char *node,int j,int *ma)
{
	int i;
	char *p1,*p2;
	char var[MAXVARLEN];

	if (t==NULL) return;
        replacemarraybody(t->left,node,j,ma);
        replacemarraybody(t->right,node,j,ma);
	if (strncmp(t->node,node,j)!=0) return;

	p2=t->node+j-1; 
	strcpy(var,t->node); p1=var+j; 
	for (i=1;i<*ma;i++) *p1++='(';
	for (i=0;i<*ma;i++) {
		if (i>0) { sprintf(p1,")*%i+",ma[i+1]); p1+=strlen(p1); }
		p2++; 
		if (i>0) *p1++='('; 
		while (*p2!=']') *p1++=*p2++; 
		if (i>0) *p1++=')';
		p2++;
	}
	*p1++=']'; *p1=0;
	strcpy(t->node,var);
}       
/*******************************************************************/

/*******************************************************************/
#define treeconst(x) treecompose(FALSEs,NULL,NULL)
/*******************************************************************/
void arit_m2(tree *s2)
{
	tree *a0=s2;

	while (a0->node[0]==';') a0=a0->right;
	treereplace(a0,treecompose(";",treecopy(a0),treeconst(0)));
}
tree *comblistmul(tree *x1,tree *x2)
{ 	
	tree *r0,*r1,*r2;
	tree *t2;

	r0=NULL;
	while (1) {
		t2=listgete(x2);
		switch (t2->node[0]) {
			case FALSE: r1=NULL; break;
			case TRUE:  r1=treecopy(x1); break;
			default:    r1=comblistdist(arit_band,x1,t2); break;
		}
		if (r0==NULL) r2=r1;
		else if (r1==NULL) r2=r0;
		else {
			r2=comblistdlen(arit_add,r0,r1);
			treefree(r0); treefree(r1);
		}
		r0=r2;
		if (t2!=x2) arit_m2(r0); else 
		if (r0==NULL) return treeconst(0); else return r0; 
		x2=x2->right;
	}
}
/*******************************************************************/

/*******************************************************************/
tree *var2tree(char *var,tree *vlist,int bn)
{ 	
	int a1=0;
	int b1=0;
	char s[MAXVARLEN];
	tree *v1,*t0;

	if (*var=='X') a1=b1=bn; var++;
	v1=getvtree(var,vlist);
	a1+=atoi(v1->left->node); b1+=atoi(v1->right->node);

	sprintf(s,"%ix",b1); t0=treecompose(s,NULL,NULL);

	for (b1--;a1<=b1;b1--) {
		sprintf(s,"%ix",b1);
		t0=treecompose(";",treecompose(s,NULL,NULL),t0);
	}
	return t0;
}
tree *const2tree(char *cnum)
{ 	
	int a1,a2;
	char s[MAXVARLEN];
	tree *t0;

	a1=atoi(cnum); 

	a2=a1%2;
	if (a2==0) strcpy(s,FALSEs); else strcpy(s,TRUEs); 
	t0=treecompose(s,NULL,NULL); 

	for (a1/=2;a1;a1/=2) {
		a2=a1%2;
		if (a2==0) strcpy(s,FALSEs); else strcpy(s,TRUEs); 
		t0=treecompose(";",treecompose(s,NULL,NULL),t0);
	}
	return t0;
}
/*******************************************************************/
int treelength(tree *t1)
{ 	
	int i=1;
	while (t1->node[0]==';') { t1=t1->right; i++; }
	return i;
}
/*******************************************************************/
tree *replacevarconst(tree *t,tree *vlist,int bn)
{
	tree *t0;

        if (t==NULL) return NULL;
	if (isdigit(t->node[0])) { 
		t0=const2tree(t->node); treefree(t); return t0; 
	}
	if (t->node[1]&&(t->node[0]=='X'||t->node[0]=='V')) { 
		t0=var2tree(t->node,vlist,bn); treefree(t); return t0; 
	}
	t->left=replacevarconst(t->left,vlist,bn);
	t->right=replacevarconst(t->right,vlist,bn); 

	switch (t->node[0]) {
		case '+':
			t0=comblistdlen(arit_add,t->left,t->right); 
			treefree(t); return t0; 
		case '-': 
			t0=comblistdlen(arit_sub,t->left,t->right); 
			treefree(t); return t0; 
		case '*': 
			t0=comblistmul(t->left,t->right); 
			treefree(t); return t0; 
		case '=': 
			t0=comblistdlen(arit_eq,t->left,t->right); 
			treefree(t); return t0; 
		case '>': 
			t0=combarit_greater(t->left,t->right); 
			treefree(t); return t0; 
		case '<': 
			t0=combarit_less(t->left,t->right); 
			treefree(t); return t0; 
		case BOR:
			t0=comblistdlen(arit_bor,t->left,t->right); 
			treefree(t); return t0; 
		case DAND:
			t0=comblistdist(arit_band,t->left,t->right); 
			treefree(t); return t0; 
		case DOR:
			t0=comblistdist(arit_bor,t->left,t->right); 
			treefree(t); return t0; 
	}
	return t;
}
