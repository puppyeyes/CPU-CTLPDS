#include "tbda.h"
/************************************************************************/
#define x(t)	(t->x)
/************************************************************************/
#define s(t)	if (s==t) return s;
/************************************************************************/
void TBDkp(TBD *w)
{
        TBD *e0=TBDomg;

	if (istoo(w)) return;
/*
        while (e0->c!=NULL&&difftct(e0->c,w)) { e0=e0->n; }
*/
        while (e0->c!=NULL) { e0=e0->n; }
        if (e0->c!=NULL) { return; } 
	e0->c=w; e0->n=(TBD *)calloc(1,sizeof(TBD)); 
	if (e0->n==NULL) { TBDrealloc(TBDctb,e0->n); }
}
/************************************************************************/
void TBDdr(TBD *w)
{
        TBD *e1,*e0=TBDomg;

	if (istoo(w)) return;
        while (e0->c!=NULL&&difftct(e0->c,w)) { e0=e0->n; }
        if (e0->c==NULL) { return; } 
	e1=e0->n; e0->c=e1->c; e0->n=e1->n; free(e1);
}
/************************************************************************/
void TBDcl()
{
	int k=0;
	TBD *e0;

	TBDomg->y=TBDtau->y=0;
        for (e0=TBDomg;e0->c!=NULL;e0=e0->n) k+=TBD3mnd(e0->c);
	TBD3ftc(TBDttb); 
	TBD3ftc(TBDctb); 
	TBDttb->nodes=k; TBDctb->nodes=0;
}
/************************************************************************/
int TBD3mnd(TBD *w)
{
	int i;

        if (istoo(w)||w->y>0) return 0; w->y+=8;
       	return TBD3mnd(w->a)+TBD3mnd(w->b)+TBD3mnd(w->c)+1;
}       
/************************************************************************/
void TBD3ftc(TBDtb *cache)
{
        int i;

        for (i=0;i<cache->tablesz;i++)
        if (cache->table[i]!=NULL) cache->table[i]=TBD3fls(cache->table[i]); 
}
/************************************************************************/
TBD *TBD3fls(TBD *w)
{
	TBD *r;

	if (w==NULL) return NULL;
	if (w->y>0) { w->y-=8; w->n=TBD3fls(w->n); return w; }
	if (w->y<0) { w->n=TBD3fls(w->n); return w; }
	r=TBD3fls(w->n); free(w); return r;
}
/************************************************************************/
void TBD3und(TBDtb *cache)
{
        int i;
        for (i=0;i<cache->tablesz;i++) TBD3uls(cache->table[i]); 
}
/************************************************************************/
void TBD3uls(TBD *w)
{
	if (w==NULL) return; if (w->y<0) w->y=0; TBD3uls(w->n); 
}
/************************************************************************/

/************************************************************************/
GLOBAL TBD* TBDomg=NULL;
GLOBAL TBD* TBDtau=NULL;
GLOBAL TBDtb *TBDttb=NULL;
GLOBAL TBDtb *TBDctb=NULL;
GLOBAL TBDtv *TBDvtb=NULL;
/************************************************************************/
#define add1(x) ((unsigned int)(x))
#define add2(x,y) (add1(x)+add1(y))
#define add3(x,y,z) (add1(x)+add1(y)+add1(z))
/************************************************************************/
#define pair(a,b) add1((add1(add2(a,b)*add3(a,b,1)))/(add1(2))+add1(a))
#define triple(a,b,c) (pair(pair(a,b),c))
#define quadruple(a,b,c,d) (pair(pair(a,b),pair(c,d)))
/************************************************************************/
#define tableitem(t,v) (t->table[v])
/************************************************************************/
#define hvaluetct(t,w) (quadruple(w->x,w->a,w->b,w->c)%t->tablesz)
#define hvaluectb(t,x,a,b) (triple(x,a,b)%t->tablesz)
/************************************************************************/
void TBD0ntl()
{
	if (TBDomg!=NULL) return;
        TBDtau=(TBD *)calloc(1,sizeof(TBD)); TBDtau->x=0;
        TBDomg=(TBD *)calloc(1,sizeof(TBD)); TBDomg->x=TBDCSTOMG;
        TBDmalloc(TBDttb,ICACHESIZE); TBDmalloc(TBDctb,ICACHESIZE); 
}
/************************************************************************/
TBD *TBD0dtc(TBDtb *TBDttb,TBD *w)
{
        int hv=hvaluetct(TBDttb,w);
        TBD *e0=tableitem(TBDttb,hv);
        TBD *e1=e0;

        while (e1!=NULL&&difftct(e1,w)) { e0=e1; e1=e1->n; }
        if (e1!=NULL) { free(w); return e1; }
        if (e1==e0) TBDttb->table[hv]=w; else { e0->n=w; } TBDttb->nodes++;
	if (TBDttb->nodes>TBDttb->cap) TBD0rtc(TBDttb,TBD0dtc);
	return w;
}
/************************************************************************/
int TBDrc(TBD *w)
{
        int hv;
        TBD *e0,*e1;

	if (w==NULL) { TBD3und(TBDttb); return 1; }
	if (istoo(w)) { e1=w; } else {
        	hv=hvaluetct(TBDttb,w); e0=tableitem(TBDttb,hv); e1=e0;
       		while (e1!=NULL&&difftct(e1,w)) { e0=e1; e1=e1->n; }
        	if (e1==NULL) return 0;
	}
	if (e1->y!=TBDCSTMARK) { e1->y=TBDCSTMARK; return 0; } 
	TBD3und(TBDttb); return 1; 
}
/************************************************************************/
TBD *TBD0gtc(TBDtb *TBDctb,int x,TBD *a,TBD *b)
{
        int hv=hvaluectb(TBDctb,x,a,b);
        TBD *e0=tableitem(TBDctb,hv);
        TBD *e1=e0;
        TBD *w;

        while (e1!=NULL&&diffctb(e1,x,a,b)) { e0=e1; e1=e1->n; }
        if (e1!=NULL) { return e1->c; }
        w=TBD0cnd(x,a,b,NULL);
        if (e1==e0) TBDctb->table[hv]=w; else { e0->n=w; } TBDctb->nodes++;
	if (TBDctb->nodes>TBDctb->cap) TBD0rtc(TBDctb,TBD0dnc);
	return w;
}
/************************************************************************/
TBD *TBD0dnc(TBDtb *nc,TBD *w)
{
        int hv=hvaluectb(nc,w->x,w->a,w->b);
        TBD *e0=tableitem(nc,hv);
        TBD *e1=e0;

        while (e1!=NULL&&diffctb(e1,w->x,w->a,w->b)) { e0=e1; e1=e1->n; }
        if (e1!=NULL) { error("TBD0dnc"); }
        if (e1==e0) nc->table[hv]=w; else { e0->n=w; }
        nc->nodes++; return w;
}
/************************************************************************/
void TBD0rtc(TBDtb *cache,TBD *f())
{
        int i;
        TBDtb *tbdc;

	ckcache(cache); 
	TBDmalloc(tbdc,2*cache->tablesz);
        for (i=0;i<cache->tablesz;i++)
        if (cache->table[i]!=NULL) { TBD0rls(tbdc,cache->table[i],f); }
        free(cache->table); free(cache);
        if (f==TBD0dtc) TBDttb=tbdc; else TBDctb=tbdc;
}
void TBD0rls(TBDtb *newcache,TBD *w,TBD *f())
{
        if (w==NULL) return;
        TBD0rls(newcache,w->n,f); w->n=NULL; f(newcache,w);
}
/************************************************************************/
TBD *TBD0cnd(int x,TBD *a,TBD *b,TBD *c)
{
 	TBD *r=(TBD *)malloc(1*sizeof(TBD));
	if (r==NULL) { TBDrealloc(TBDctb,r); }
        r->y=0; r->n=NULL; r->x=x; r->a=a; r->b=b; r->c=c; return r;
}
/************************************************************************/
TBD *TBD0crn(int x,TBD *a,TBD *b,TBD *c)
{
        if (isomg(a)&&isomg(b)) { if (x>0) return TBDomg; return TBDtau; }
        if (isomg(c)) { if (x>0) return TBDomg; return TBDtau; }

	if (a==b&&istau(a)) { if (x>0) return c; return TBDng(c); }
	if (a==b&&istau(c)) { if (x>0) return a; return TBDng(a); }

        if (isomg(b)&&!istau(a)&&istau(c)) return TBD0crn(x,TBDtau,TBDomg,a);
        if (isomg(a)&&!istau(b)&&istau(c)) return TBD0crn(x,TBDomg,TBDtau,b);

        if (b==c&&!istau(c)) return TBD0crn(x,a,TBDtau,c);
        if (a==c&&!istau(c)) return TBD0crn(x,TBDtau,b,c);

	if (a==c&&istau(c)&&x<0) return TBD0crn(-x,TBDomg,TBDng(b),c); 
	if (b==c&&istau(c)&&x<0) return TBD0crn(-x,TBDng(a),TBDomg,c); 

        return TBD0dtc(TBDttb,TBD0cnd(x,a,b,c));
}
/*******************************************************************/
/************************************************************************/
TBD *TBDng(TBD *s)
{
        if (isomg(s)) { return TBDtau; }
        if (istau(s)) { return TBDomg; }
	return TBD0crn(-s->x,s->a,s->b,s->c);
}
/************************************************************************/
TBD *TBDpos(TBD *s)
{
        TBD *r0,*r,*a,*b,*c;

	return s;
        if (istoo(s)) { return s; }
        r0=TBD0gtc(TBDctb,0x1600,s,NULL);
        if (istoo(r0)||r0->c!=NULL) { return r0; }

        if (s->x>0) r=TBD0crn(s->x,TBDpos(s->a),TBDpos(s->b),TBDpos(s->c));
        else {
                a=TBDpos(TBDng(TBDcn(s->a,s->c)));
                b=TBDpos(TBDng(TBDcn(s->b,s->c)));
                r=TBD0crn(-s->x,a,b,TBDtau);
        }
        r0->c=r; return r;
}
/*******************************************************************/
TBD *TBDcn(TBD *s,TBD *t)
{
        TBD *r,*r0,*a,*b,*c;

	TBDck(s,t);
        r0=TBD0gtc(TBDctb,TBDCSTND,s,t); 
	if (istoo(r0)||r0->c!=NULL) { return r0; }

	switch (TBD2ccs(s,t)) {
	    case 1: a=TBDcn(s->a,t->a); b=TBDcn(s->b,t->b); 
		    c=TBDcn(s->c,t->c); r=TBD0crn(s->x,a,b,c); break;
	    case 3: 
		    if (x(t)) {
		    a=TBDnnd(TBDcn(t->a,t->c),TBDnnd(s->a,s->c));
		    b=TBDnnd(TBDcn(t->b,t->c),TBDnnd(s->b,s->c)); c=TBDtau;
		    r=TBD0crn(s->x,a,b,c); break;
		    } else {
	            a=TBDcn(t->a,TBDnnd(s->a,s->c));
		    b=TBDcn(t->b,TBDnnd(s->b,s->c)); c=t->c;
		    r=TBD0crn(t->x,a,b,c); break;
		    }
	    case 2:
		    if (x(s)) {
	            a=TBDnnd(TBDcn(s->a,s->c),TBDnnd(t->a,t->c));
		    b=TBDnnd(TBDcn(s->b,s->c),TBDnnd(t->b,t->c)); c=TBDtau;
		    r=TBD0crn(t->x,a,b,c); break;
		    } else {
	            a=TBDcn(s->a,TBDnnd(t->a,t->c));
		    b=TBDcn(s->b,TBDnnd(t->b,t->c)); c=s->c;
		    r=TBD0crn(s->x,a,b,c); break;
		    }
	    case 4: 
		    a=TBDnnd(TBDnnd(s->a,s->c),TBDnnd(t->a,t->c));
		    b=TBDnnd(TBDnnd(s->b,s->c),TBDnnd(t->b,t->c));
		    c=TBDtau;
		    r=TBD0crn(t->x,a,b,c); break;
	    case 5: r=TBD0crn(s->x,s->a,s->b,TBDcn(s->c,t)); break;
	    case 6: 
		    if (x(t)) {
		    a=TBDnnd(t,TBDng(s->a)); b=TBDnnd(t,TBDng(s->b));
		    c=TBDnnd(t,TBDng(s->c)); r=TBD0crn(s->x,a,b,c); break;
		    } else { 
	            a=TBDnnd(s->a,s->c); b=TBDnnd(s->b,s->c);
		    r=TBD0crn(-s->x,a,b,t); break;
		    }
	    case 7: r=TBD0crn(t->x,t->a,t->b,TBDcn(t->c,s)); break;
	    case 8: 
		    if (x(s)) {
		    a=TBDnnd(s,TBDng(t->a)); b=TBDnnd(s,TBDng(t->b));
		    c=TBDnnd(s,TBDng(t->c)); r=TBD0crn(t->x,a,b,c); break;
		    } else {
	            a=TBDnnd(t->a,t->c); b=TBDnnd(t->b,t->c);
		    r=TBD0crn(-t->x,a,b,s); break;
		    }
	}
	r0->c=r; return r;
}
/************************************************************************/
TBD *TBDbs(int x,TBD *w)
{
        return TBDcn(TBD2crs(w,x,0),TBD2crs(w,x,1));
}
/************************************************************************/
int TBDtr(TBD *s)
{
	return (TBD2ctb(s)==TBDtau);
}
/************************************************************************/

/************************************************************************/
int TBD2ccs(TBD *s,TBD *t)
{
	int sx=(s->x>0?s->x:-s->x); 
	int tx=(t->x>0?t->x:-t->x);

	if (sx<tx) { if (s->x>0) return 5; else return 6; } 
	else if (sx>tx) { if (t->x>0) return 7; else return 8; } 
	else if (s->x>0) { if (t->x>0) return 1; else return 2; } 
	else if (t->x>0) return 3; 
	else return 4; 
}
/************************************************************************/
#define reg(r)
/*
#define reg(r)	r1->c=r
/************************************************************************/
TBD *TBD2crs(TBD* r0,int x,int v)
{
        TBD *a,*b,*c,*r1,*r;
	int y;

	if (istoo(r0)||abs(r0->x)>x) {
		return r0;
	}
        y=((abs(x))<<2)+(v<<1)+(x<0);
        r1=TBD0gtc(TBDctb,TBDCSTRS,r0,(TBD *)y); 
	if (istoo(r1)||r1->c!=NULL) { return r1; }
        if (r0->x==x) {
 		if (v==0) r=TBDcn(r0->a,r0->c); else r=TBDcn(r0->b,r0->c); 
	} else if (r0->x==-x) {
 		if (v==0) r=TBDnnd(r0->a,r0->c); else r=TBDnnd(r0->b,r0->c); 
	} else {
        	a=TBD2crs(r0->a,x,v); b=TBD2crs(r0->b,x,v); 
		c=TBD2crs(r0->c,x,v); r=TBD0crn(r0->x,a,b,c); 
	}
	r1->c=r; return r;
}
/************************************************************************/
TBD *TBD2ctb(TBD *w)
{
        if (istoo(w)) return w; 
	return TBD2ctb(TBDbs(TBD2cml(w),w));
}
/*******************************************************************/
int TBD2cml(TBD *w)
{
	int x,y,z,n;

        if (istoo(w)) return 0; 
	x=TBD2cml(w->a); y=TBD2cml(w->b); z=TBD2cml(w->c);
        n=abs(w->x); if (n>x&&n>y&&n>z) return n;
	if (x>y) return (x>z?x:z); else return (y>z?y:z); 
}
/*******************************************************************/

/************************************************************************/
TBD *TBDds(TBD *s,TBD *t)
{
        return TBDnnd(TBDng(s),TBDng(t));
}
TBD *TBDxb(int x,TBD *s)
{
        return TBDng(TBDbs(x,TBDng(s)));
}
int TBDmp(TBD *s)
{
        return TBDtr(TBDng(s));
}
/************************************************************************/

/************************************************************************/
void TBDvl(char **vlist)
{
	int n,i;

	for (n=0;vlist[n]!=NULL;n++);
	if (TBDvtb==NULL) { TBD0ntl(); TBDvtb=(TBDtv *)malloc(sizeof(TBDtv)); }
	else { free(TBDvtb->name); free(TBDvtb->ival); free(TBDvtb->rtab); }
		
	TBDvtb->rtab=(int *)malloc((n+1)*sizeof(int)); 
	TBDvtb->ival=(int *)malloc((n+1)*sizeof(int)); 
	TBDvtb->name=(char **)malloc((n+1)*sizeof(char*)); 
	for (i=0;i<n;i++) { TBDvtb->name[i]=vlist[i]; TBDvtb->ival[i]=i+1; }

	TBD1svl(TBDvtb->name,TBDvtb->ival,0,n-1); TBDvtb->count=n;
}
/************************************************************************/
TBD *TBDvr(char *var)
{
	int i;

	if (*var=='^') return TBDds(TBD1clt(1),TBD1clt(-1)); 
	if (*var=='_') return TBDcn(TBD1clt(1),TBD1clt(-1)); 
	i=TBD1snm(var,TBDvtb->name,TBDvtb->ival,0,TBDvtb->count-1);
	if (i==0) { error("variable not in the given list"); }
	return TBD1clt(i);
}
/************************************************************************/
TBD *TBD1clt(int a)
{
	return TBD0crn(a,TBDomg,TBDtau,TBDtau);
}
/************************************************************************/
void TBD1svl(char **a,int *b,int l,int r) /* increasing */
{
        int i,j,k;
	char *v,*t;
        if (r>l) {

                v=a[r]; i=l-1;j=r;
                for (;;) {
			for (i++;strcmp(a[i],v)<0;i++);
			for (j--;j>=l&&strcmp(a[j],v)>0;j--); 
	/*BUG* if (j<l) j=1; */
			if (i>=j) break;
                        t=a[i]; a[i]=a[j]; a[j]=t; k=b[i]; b[i]=b[j]; b[j]=k;
                }
                t=a[i]; a[i]=a[r]; a[r]=t; k=b[i]; b[i]=b[r]; b[r]=k;
                TBD1svl(a,b,l,i-1); TBD1svl(a,b,i+1,r);
        }
}
/**********************************************************************/
int TBD1snm(char *s,char **a,int *b,int l,int r)
{
        int i;
	if (l>r) return 0; i=(l+r)/2;
	if (strcmp(s,a[i])<0) return TBD1snm(s,a,b,l,i-1);
	if (strcmp(s,a[i])>0) return TBD1snm(s,a,b,i+1,r);
	return b[i];
}
/**********************************************************************/
/*
xTBD *TBDformula(TBDtree *prop,char **ovlist)
{
	if (ovlist!=NULL) TBDvtbinit(ovlist); 
	if (prop==NULL) return NULL; return TBDfml(prop);
}
xTBD *TBDfml(TBDtree *prop)
{
        TBD *r;

        switch (prop->node[0]) {
            case '|': r= TBDor(TBDfml(prop->left),TBDfml(prop->right)); break;
            case '&': r= TBDcn(TBDfml(prop->left),TBDfml(prop->right)); break;
            case '^': r= TBDor(TBD1clt(1),TBD1clt(-1)); break;
            case '_': r= TBDcn(TBD1clt(1),TBD1clt(-1)); break;
            case '!': r= TBDng(TBDfml(prop->right)); break;
            default:  r= TBDvtb(prop->node); break;
        }
        return r;
}
/************************************************************************/
void TBDrl(char **vlist)
{
	int i;

	for (i=0;i<TBDvtb->count;i++) 
		TBDvtb->rtab[i+1]=TBD1snm(vlist[i],
			TBDvtb->name,TBDvtb->ival,0,TBDvtb->count-1);
}
/************************************************************************/
TBD *TBDrn(TBD *w)
{
        TBD *a,*b,*c,*r,*r0;
	int x;

        if (istoo(w)) return w;
        r0=TBD0gtc(TBDctb,TBDCSTLB,w,(TBD *)TBDvtb->rtab); 
	if (istoo(r0)||r0->c!=NULL) return r0;
        a=TBDrn(w->a); b=TBDrn(w->b); c=TBDrn(w->c); 
	if (w->x>0) x=TBDvtb->rtab[w->x]; else x=-TBDvtb->rtab[-w->x]; 
	r=TBD0crn(x,a,b,c); r0->c=r; return r;
}
/*******************************************************************/
