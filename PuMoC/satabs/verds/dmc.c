#include "dmc.h"
#include "tbda.h"
/* problem when #include "dmc.h" with ox10
#include "tree.h"
/************************************************************************/
TBD *TBDpos(TBD *);	/*BUGFIX ADDED*/
/************************************************************************/
#define infos(s)	fprintf(stdout,"%s\n",s)
#define infoi(s)	fprintf(stdout,"%i\n",s)
/************************************************************************/
extern int m2aux;
extern TBD **m2cache;
extern TBD **m2fness;
/************************************************************************/
LOCAL TBD *DMCfair=NULL;
/************************************************************************/
#define fair() 		f2eg(TBDtau)
/************************************************************************/
#define dreturn(r0,rr)		{ TBDkp(r0); r=rr; TBDdr(r0); return r; }
#define dreturn2(r0,r0a,rr)	\
	{ TBDkp(r0); TBDkp(r0a); r=rr; TBDdr(r0); TBDdr(r0a); return r; }
/************************************************************************/
TBD *f2eg(TBD* r0)
{
        TBD *n2,*n0,*n1=TBDtau;
	TBD **fn=m2fness;
	TBD *r;
	int i,m,k;

	if (fn[0]==NULL) return m2eg(r0);
	if (r0==TBDtau&&DMCfair!=NULL) return DMCfair;

	TBDkp(r0);
	for (n0=TBDtau;1;n0=n1) {
		TBDkp(n0);
	  	for (n1=r0,i=0;fn[i]!=NULL;i++) {
			TBDkp(n1); 
			n1=TBDcn(n1,m2xx(m2eu(r0,TBDcn(n0,fn[i]))));
			TBDdr(n1);
		}
		TBDdr(n0);
	  	if (TBDmp(TBDcn(n0,TBDng(n1)))) {
	  		if (r0==TBDtau) { n1=TBDpos(n1); TBDkp(n1); DMCfair=n1;}
			return n1; 
	  	}
	}
}
TBD *f2eu(TBD *r0,TBD *r0a)
{
	TBD **fn=m2fness;
	TBD *r;
	if (fn[0]==NULL) return m2eu(r0,r0a);
	dreturn(r0a,m2eu(r0,TBDcn(r0a,fair()))); 
}
TBD *f2ef(TBD *r0)
{
	TBD **fn=m2fness;
	TBD *r;
	if (fn[0]==NULL) return m2ef(r0);
	dreturn(r0,m2ef(TBDcn(r0,fair()))); 
}
TBD *f2er(TBD *r0,TBD *r0a)
{
	TBD **fn=m2fness;
	TBD *r,*r1;
	if (fn[0]==NULL) return m2er(r0,r0a);
	TBDkp(r0); TBDkp(r0a); 
	r1=f2eg(r0a); TBDkp(r1); 
 	r=m2eu(r0a,TBDcn(TBDcn(r0a,r0),fair())); r=TBDds(r,r1);
	TBDdr(r0); TBDdr(r0a); TBDdr(r1); return r;

	dreturn(r0,TBDds(m2eu(r0a,TBDcn(TBDcn(r0a,r0),fair())),f2eg(r0a))); 
}
TBD *f2xx(TBD *r0)
{
	return m2xx(TBDcn(r0,fair())); 
}
/************************************************************************/
TBD *f2ega(TBD* r0)
{
	TBD **fn=m2fness;
	TBD *r;
	if (fn[0]==NULL) return m2ega(r0);
	dreturn(r0,TBDng(f2ef(TBDng(r0))));
}
TBD *f2eua(TBD *r0,TBD *r0a)
{
	TBD **fn=m2fness;
	TBD *r;
	if (fn[0]==NULL) return m2eua(r0,r0a);
	dreturn2(r0,r0a,TBDng(f2er(TBDng(r0),TBDng(r0a))));
}
TBD *f2efa(TBD *r0)
{
	TBD **fn=m2fness;
	TBD *r;
	if (fn[0]==NULL) return m2efa(r0);
	dreturn(r0,TBDng(f2eg(TBDng(r0)))); 
}
TBD *f2era(TBD *r0,TBD *r0a)
{
	TBD **fn=m2fness;
	TBD *r;
	if (fn[0]==NULL) return m2era(r0,r0a);
	dreturn2(r0,r0a,TBDng(f2eu(TBDng(r0),TBDng(r0a))));
}
TBD *f2xxa(TBD *r0)
{
	return TBDng(f2xx(TBDng(r0))); 
}
/************************************************************************/

/************************************************************************/
/************************************************************************/
#define m2add(r)	{ m2cache[m2count++]=r; TBDkp(r); }
#define m2add1(r)	{ r1=(r); m2cache[m2count++]=r1; TBDkp(r1);}
/*
#define m2add1(r)	{ r1=(r); m2cache[m2count++]=r1; TBDkp(r1); }
/************************************************************************/
#define m2value(node)	m2order[m2oi[atoi(node)]]
/************************************************************************/
#define m2rhsetaf(w,es) m2fgaf(w,es,m2rel,m2rrel,m2aux)
#define m2rhsetag(w,es) m2fgag(w,es,m2rel,m2rrel,m2aux)
/*
#define m2rhsetag(w,es) m2fgag2x(w,m2rel,es,m2aux)
#define m2rhsetag(w,es) m2fgag2(es,m2rrel,m2aux,TBDds,TBDds,w,m2rel,es)
#define m2rhsetag(w,es)	m2fgag1(w,m2rel,m2aux,TBDds,TBDds,es)
/************************************************************************/
#define m2rhsetonly(w) 	m2fg(w,m2rel,m2aux,TBDds,TBDds)
/************************************************************************/
GLOBAL int m2rcount;
GLOBAL int m2rcounta;
/************************************************************************/
LOCAL int m2aux;
LOCAL TBD **m2cache;
LOCAL int m2count=0;
LOCAL TBD **m2fness;
/************************************************************************/
GLOBAL int *m2order;
GLOBAL int *m2oi;
GLOBAL int *m2varcount;
GLOBAL char **m2vlist;
GLOBAL char **m2rlist;
/************************************************************************/
#define OTT     1
#define ODF     2
#define ORDM	20
/************************************************************************/
int m2start(tree *init,tree *trs,tree *fness,tree *prop,int n,int opt,char **v1,char **v2,int *v0)
{
	int i;
	TBD *r1;
	int k;
	tree *t0;

	m2aux=n; 
        m2varcount=(int *)calloc((2*n+1),sizeof(int))+n; 

	m2orderinit(v1,v2,v0,n);
        m2oi=(int *)malloc((2*n+1)*sizeof(int)); m2initorder(trs,n);

	m2orderinterleave(n,0); 

	k=3*(m2relation(trs,0)+m2TLFsub(prop));
	m2cache=(TBD **)calloc(k,sizeof(TBD *)); 

	if (fness==NULL) {
		m2fness=(TBD **)malloc((1)*sizeof(TBD *)); 
		m2fness[0]=NULL;
	} else {
		t0=fness;
		for (i=0;t0->node[0]==';';i++,t0=t0->right);
        	m2fness=(TBD **)malloc((i+2)*sizeof(TBD *)); 
		for (i=0,t0=fness;t0->node[0]==';';i++,t0=t0->right) {
			m2fness[i]=m2tbd(t0->left); TBDkp(m2fness[i]);
		}
		m2fness[i]=m2tbd(t0); TBDkp(m2fness[i]); 
		m2fness[i+1]=NULL;
	}

	m2add1(m2tbd(init)); m2relation(trs,1); m2orderinterleave(n,1); 
	k=m2TLFtype(prop); 

	switch (k) {
	  default: m2relation(trs,1); m2rcount/=2; break;




	}
	m2orderinterleave(n,2); 
	switch (opt) {
	    case OTT:
		return TBDmp(TBDng(m2ex(TBDvr("^"),m2rrel,m2aux,TBDds)));
	    case ODF:
		r1=m2rhsetonly(m2i); TBDkp(r1); 
		return TBDmp(TBDcn(r1,TBDng(m2ex(r1,m2rrel,m2aux,TBDds))));
	    default:
		break;
	}

	i=TBDmp(m2mc(prop));
	return i;
}
TBD *m2mc(tree *prop)
{
	return TBDcn(m2i,m2prop(m2TLFnegate(prop)));
}
/************************************************************************/
TBD *m2prop(tree *prop)
{
	TBD *r;
	switch (prop->node[0]) {
	    case 'A': r= TBDng(m2prope(m2TLFnegate(prop->right))); break;
	    case 'E': r= m2prope(prop->right); break;
	    case '|': r= TBDds(m2prop(prop->left),m2prop(prop->right)); break;
	    case '&': r= TBDcn(m2prop(prop->left),m2prop(prop->right)); break;
	    case '!': r= TBDng(m2prop(prop->right)); break;
	    default:  r= TBDvr(prop->node); break;
/*
	    case '^': r= TBDds(TBDap(1),TBDap(-1)); break;
	    case '_': r= TBDcn(TBDap(1),TBDap(-1)); break;
	    case '!': r= TBDap(-m2value(prop->right->node)); break;
	    default:  r= TBDap(m2value(prop->node)); break;
*/
	}
	m2add(r); 
	return r;
}
/************************************************************************/
TBD *m2prope(tree *prop)
{
	TBD *r=NULL;
	switch (prop->node[0]) {
	    case 'X': r= f2xx(m2prop(prop->right)); break;
	    case 'F': r= f2ef(m2prop(prop->right)); break;
	    case 'G': r= f2eg(m2prop(prop->right)); break;
	    case 'U': r= f2eu(m2prop(prop->left),m2prop(prop->right)); break;
	    case 'R': r= f2er(m2prop(prop->left),m2prop(prop->right)); break;
	}
	m2add(r); return r;
}
/************************************************************************/
TBD *m2tbd(tree *prop)
{
	TBD *r;
	switch (prop->node[0]) {
	    case '|': r= TBDds(m2tbd(prop->left),m2tbd(prop->right)); break;
	    case '&': r= TBDcn(m2tbd(prop->left),m2tbd(prop->right)); break;
	    case '!': r= TBDng(m2tbd(prop->right)); break;
	    default:  r= TBDvr(prop->node); break;
/*
	    case '_': r= TBDcn(TBD1clt(1),TBD1clt(-1)); break;
	    case '^': r= TBDds(TBDap(1),TBDap(-1)); break;
	    case '_': r= TBDcn(TBDap(1),TBDap(-1)); break;
	    case '!': r= TBDap(-m2value(prop->right->node)); break;
	    default:  r= TBDap(m2value(prop->node)); break;
*/
	}
	return r;
}
/************************************************************************/
int m2ordersequential(int n,int reverse)
{
	int i;

	switch (reverse) {
	    case 0: for (i=1;i<=n;i++) { 
			m2order[i]=i; m2order[i+n]=n+i; 
		    }; 
		    break;
	    case 1: for (i=1;i<=n;i++) { 
			m2order[i]=n+i; m2order[i+n]=i; 
		    }; 
		    break;
	}
}
int m2orderinterleave(int n,int reverse)
{
	int i;
	char *x;

	switch (reverse) {
	    case 0: TBDvl(m2vlist); TBDrl(m2rlist); break;
	    case 1: TBDvl(m2rlist); TBDrl(m2vlist); break;
	    case 2: TBDvl(m2vlist); TBDrl(m2rlist); break;
/*
		    for (i=1;i<=n;i++) { 
			x=m2vlist[2*i-1-1];
			m2vlist[2*i-1-1]=m2vlist[2*i-1]; m2vlist[2*i-1]=x;
		    }; 
*/
	}
}
/************************************************************************/
int m2trel(tree *prop,int c)
{
	TBD *r1;
	tree *t;
	switch (c) {
	    case 0: return 1; 
	    case 1: m2add1(m2tbd(prop)); break;
	    case 2: t=m2TLFnegate(prop); m2add1(m2tbd(t)); t=m2TLFnegate(t); 
	}
	m2rcount++; return 0; 
}
/************************************************************************/
int m2ptrel(tree *prop,int c)
{
	if (prop->node[0]!='|') return m2trel(prop,c);
        return m2ptrel(prop->left,c)+m2ptrel(prop->right,c);
}
/************************************************************************/
int m2orderinit(char **v1,char **v2,int *v0,int n)
{
	int i,k;

        m2order=(int *)malloc((2*n+1)*sizeof(int)); 
        m2vlist=(char **)malloc((2*n+1)*sizeof(char *)); 
        m2rlist=(char **)malloc((2*n+1)*sizeof(char *)); 
	for (i=0;i<2*n;i++) m2vlist[i]=(char *)malloc(ORDM*sizeof(char)); 
	for (i=0;i<2*n;i++) m2rlist[i]=(char *)malloc(ORDM*sizeof(char)); 
	m2vlist[2*n]=NULL; m2rlist[2*n]=NULL;



	if (v1==NULL)
	for (i=1;i<=n;i++) { 
		m2order[i]=2*i-1; m2order[i+n]=2*i;
		sprintf(m2vlist[2*i-1-1],"%i",i);
		sprintf(m2vlist[2*i-1],"%i",n+i);
		sprintf(m2rlist[2*i-1-1],"%i",n+i);
		sprintf(m2rlist[2*i-1],"%i",i);
    	} 
	else 
	for (i=1;i<=n;i++) { 
		m2order[i]=v0[i]; m2order[i+n]=v0[i+n];
		sprintf(m2vlist[2*i-1-1],"%s",v1[2*i-1-1]);
		sprintf(m2vlist[2*i-1],"%s",v1[2*i-1]);
		sprintf(m2rlist[2*i-1-1],"%s",v2[2*i-1-1]);
		sprintf(m2rlist[2*i-1],"%s",v2[2*i-1]);
    	} 
}
/************************************************************************/

/************************************************************************/
TBD *m2fg2(TBD* r0,TBD **r1,int n,TBD *fg(),TBD *f())
{
        TBD *new,*new0;
        TBD *inc,*inc0;

	TBDcl();
        for (inc0=r0,new0=r0,new=r0;1;inc0=inc,new0=new) {
            inc=m2ex2(new,r1,n,f); new=fg(r0,inc); if (TBDrc(new)) break; 
        }
	return new;
}
TBD *m2ur2(TBD* r0,TBD *r0a,TBD **r1,int n,TBD *f(),TBD *g(),TBD *exf())
{
        TBD *new,*new0;
        TBD *inc,*inc0;

	TBDcl();
        for (inc0=r0a,new0=r0a,new=r0a;1;inc0=inc,new0=new) {
            inc=m2ex2(new,r1,n,exf); new=f(r0a,g(r0,inc));if (TBDrc(new)) break;
        }
	return new;
}
/************************************************************************/
TBD *m2exone2(TBD* r0,TBD *r1,int bn,int len,TBD *f())
{
	int i;
        TBD *new;

	if (f==TBDds) {
		new=TBDcn(r0,r1);
		for (i=bn+1;i<=2*bn;i++) new=TBDxb(m2order[i],new);
		for (i=2*bn+1;i<=len;i++) new=TBDxb(m2order[i],new);
	} else {
		new=TBDds(r0,r1); 
		for (i=bn+1;i<=2*bn;i++) new=TBDbs(m2order[i],new);
		for (i=2*bn+1;i<=len;i++) new=TBDbs(m2order[i],new);
	}
        return new;
}
/************************************************************************/
TBD *m2ex2(TBD* r0,TBD **r1,int n,TBD *f())
{
        int i;
        TBD *r;

	r0=TBDrn(r0);
        r=m2exone2(r0,r1[0],n,2*n,f); 
        for (i=1;i<m2rcount;i++) r=f(r,m2exone2(r0,r1[i],n,2*n,f)); 
        return r;
}
/************************************************************************/


/************************************************************************/
#define TBDcl1(r0)	TBDkp(r0); TBDcl(); TBDkp(r0); 
#define TBDcl2(r0,r1)	TBDkp(r0); TBDkp(r1); TBDcl(); TBDkp(r0); TBDdr(r1);
/************************************************************************/
TBD *m2fg(TBD* r0,TBD **r1,int n,TBD *fg(),TBD *f())
{
        TBD *new,*new0;
        TBD *inc,*inc0;

	TBDcl1(r0); 
        for (inc0=r0,new0=r0,new=r0;1;inc0=inc,new0=new) {
            	inc=m2ex(new,r1,n,f); new=fg(r0,inc); 
		if (TBDrc(new)) break; 
/*
		if (TBDmp(TBDcn(new,TBDng(new0)))) break; 
*/
        }
	return new;
}
TBD *m2ur(TBD* r0,TBD *r0a,TBD **r1,int n,TBD *f(),TBD *g(),TBD *exf())
{
        TBD *new,*new0;
        TBD *inc,*inc0;

	TBDcl2(r0,r0a);
        for (inc0=r0a,new0=r0a,new=r0a;1;inc0=inc,new0=new) {
            	inc=m2ex(new,r1,n,exf); new=f(r0a,g(r0,inc)); 
		if (TBDrc(new)) break; 
/*
		if (TBDmp(TBDcn(new,TBDng(new0)))) break;
*/
        }
	return new;
}
/************************************************************************/
TBD *m2exone(TBD* r0,TBD *r1,int bn,int len,TBD *f())
{
	int i;
        TBD *new;

	if (f==TBDds) {
		new=TBDcn(r0,r1);
		for (i=1;i<=bn;i++) {
			new=TBDxb(m2order[i],new);
		}
		for (i=2*bn+1;i<=len;i++) new=TBDxb(m2order[i],new);
	} else {
		new=TBDds(r0,r1); 
		for (i=1;i<=bn;i++) new=TBDbs(m2order[i],new);
		for (i=2*bn+1;i<=len;i++) new=TBDbs(m2order[i],new);
	}
        return TBDrn(new);
}
/************************************************************************/
TBD *m2ex(TBD* r0,TBD **r1,int n,TBD *f())
{
        int i;
        TBD *r;
        r=m2exone(r0,r1[0],n,2*n,f); 
        for (i=1;i<m2rcount;i++) r=f(r,m2exone(r0,r1[i],n,2*n,f)); 
        return r;
}
/************************************************************************/

/************************************************************************/
void m2var(tree *prop,int n)
{
	int k;
	switch (prop->node[0]) {
	    case '|': m2var(prop->left,n); m2var(prop->right,n); break;
	    case '&': m2var(prop->left,n); m2var(prop->right,n); break;
	    case '^': break;
	    case '_': break;
	    case '!': if ((k=atoi(prop->right->node))>n) k-=n; m2varcount[-k]++;
		      break;
	    default:  if ((k=atoi(prop->node))>n) k-=n; m2varcount[k]++;
		      break;
	}
}
/************************************************************************/
void m2initorder(tree *trs,int n)
{
	int i;

	for (i=1;i<=n;i++) m2oi[i]=i;
/*
	m2randorder(m2oi,1,n);
	for (i=1;i<=n;i++) m2neworder(trs,i,n); 
*/
	for (i=1;i<=n;i++) m2oi[n+i]=n+m2oi[i];
}
void m2neworder(tree *trs,int i,int n)
{
	int j;
	int x,y,z,k,m;

	for (j=0;j<=n;j++) m2varcount[j]=m2varcount[-j]=0;
	if (i==1) m2var(trs,n); else m2var1(trs,n,m2oi[i-1]);

	x=i; m=m2oi[i]; z=m2varcount[m]+m2varcount[-m];
	for (k=i+1;k<=n;k++) {
		m=m2oi[k]; y=m2varcount[m]+m2varcount[-m];
		if (z>y) { x=k; z=y; }
	}
	y=m2oi[i]; m2oi[i]=m2oi[x]; m2oi[x]=y;
}
/************************************************************************/
int m2var1(tree *prop,int n,int v)
{
	int k;
	switch (prop->node[0]) {
	    case '&': 
	    case '|': k=m2var1(prop->left,n,v); 
		      if (k==1) { m2var(prop->right,n); return 0; }
	    	      k=m2var1(prop->right,n,v); 
		      if (k==1) { m2var(prop->left,n); return 0; }
	    case '^': 
	    case '_': return 0;
	    case '!': if ((k=atoi(prop->right->node))>n) k-=n; return k==v;
	    default:  if ((k=atoi(prop->node))>n) k-=n; return k==v;
	}
}
/************************************************************************/
void m2randorder(int *clause,int a,int b)
{
        int *cl=clause+a;
        int n=b-a+1;
        int i,x,y;

        srand(0);
        for (i=n;i>=2;i--) { x=rand()%i; y=cl[x]; cl[x]=cl[0]; cl[0]=y; cl++; }
}
/*******************************************************************/

/************************************************************************/

/************************************************************************/
LOCAL char m2DUALOPRS[19]="EAXXFGUR&|>{<}=B^_";
/*******************************************************************/
int m2TLFsub(tree *tf)
{
        int t;
        if (tf==NULL) return 0;
        switch (tf->node[0]) {
            case 'A': 
            case 'E': return m2TLFsub(tf->right);
            case '|': 
            case '&': 
            case 'R': 
            case 'U': return m2TLFsub(tf->left)+m2TLFsub(tf->right)+1;
            case 'X': 
            case 'F': 
            case 'G': return m2TLFsub(tf->right)+1;
            default:  return 1; 
        }
        return t;
}
/*******************************************************************/
int m2TLFisalla(tree *tf)
{
        int t;
        if (tf==NULL) return 1;
        switch (tf->node[0]) {
            case 'A': return 1;
            case 'E': return 0; 
            case '|': 
            case '&': return m2TLFisalla(tf->left)&&m2TLFisalla(tf->right);
            default:  return 1; 
        }
}
/*******************************************************************/
int m2TLFtype(tree *tf)
{
        int t;
        if (tf==NULL) return 0;
        switch (tf->node[0]) {
            case 'A': return 1|m2TLFtype(tf->right->right);
            case 'E': return 2|m2TLFtype(tf->right->right);
            case '|': return m2TLFtype(tf->left)|m2TLFtype(tf->right);
            case '&': return m2TLFtype(tf->left)|m2TLFtype(tf->right);
            default:  return 0; 
        }
}
/*******************************************************************/
tree *m2TLFgetag(tree *t)
{
	if (t->node[0]=='A'&&t->right->node[0]=='G') return t->right->right;
	return t;
}
/*******************************************************************/
tree *m2TLFgetaf(tree *t)
{
	if (t->node[0]=='A'&&t->right->node[0]=='F') return t->right->right;
	return t;
}
/*******************************************************************/
/*** TLF 
/*******************************************************************/
int m2TLFdualopr(char a)
{
        char *p=m2DUALOPRS;
        int i=0; 
        while (*p&&*p!=a) { p++; i++; } if (*p) return i+1; else return 0;
}
/*******************************************************************/
tree *m2TLFnegate(tree *t)
{
	int i;
        tree *t0;
        if (t==NULL) return NULL;
	if (isdigit(t->node[0])) {
        	t0=(tree *)(malloc(sizeof(tree)));
        	strcpy(t0->node,"!"); t0->left=NULL; t0->right=t; return t0;
	} 
	if (i=m2TLFdualopr(t->node[0])) {
        	i=i+i%2-(i+1)%2; t->node[0]=m2DUALOPRS[i-1];
		if (i<=10) {
    			t->left=m2TLFnegate(t->left); 
			t->right=m2TLFnegate(t->right); 
		}
		return t;		
	}
	switch (t->node[0]) {
	  case '!': t0=t->right; free(t); return t0;
	  case 'I': t->node[0]='&'; t->right=m2TLFnegate(t->right); return t;
	  case '~': t->right=m2TLFnegate(t->right); return t;
	}
	error("undefined operator 0");
}
/*******************************************************************/

/************************************************************************/
