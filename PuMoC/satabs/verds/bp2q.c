#include "bp2q.h"

/************************************************************************/
qmdata *bp2q(qinfo *qi,tree *bp,tree *bs,int booln,int k,int bv);
/************************************************************************/
qmdata *bp2q(qinfo *qi,tree *bp,tree *bs,int booln,int k,int bv)
{
	qmdata *md;

	qi->n=0;
	if (!bv) bs=TLFnegate(treecopy(bs));
	md=bp2q0(qi,bp,bs,booln,k,0,0); 
/*
	qmdsimp(md,qi);
*/
	if (!bv) treefree(bs);
	return md;
}
qmdata *bp2q0(qinfo *qi,tree *bp,tree *bs,int booln,int k,int cur,int lv)
{
        qmdata *m0,*m1,*m2;
	int ae,k1;
	int a,b,c;
        qmdata *ek=NULL;

        switch (bs->node[0]) {
                case 'E': 
                case 'A': k1=k; lv++; 
			  if (lv==1) cur=qi->n;
			  if (bs->node[0]=='A') ae='a'; else ae='e';
			  if (bs->right->node[0]=='X') k1=1;
			  m0=bp2qPATH(qi,bp,booln,k1,cur,lv); 
			  a=cur; b=qi->n-k1*booln; c=qi->n-booln; 
			  m1=bp2q0(qi,bp,bs->right,booln,k,cur,lv);
/*
 qmdsimp(m0,qi);
 qmdsimp(m1,qi);
 qmdwrite((m0),qi,stdout);
 qmdwrite((m1),qi,stdout);
*/
			  m1=bp2qEA(ae,qi,booln,k1,cur,b,lv,m0,m1);
			  return m1;
                case 'X': a=cur; b=qi->n-booln; 
			  return bp2q0(qi,bp,bs->right,booln,k,b,lv);
                case 'F': 
                case 'G': a=cur; c=qi->n-booln; 
			  if (k==0) b=cur; else b=qi->n-k*booln; 
			  if (bs->node[0]=='G') ek=bp2qeqPATH(a,b,c,booln);
			  return bp2qFG(qi,bp,bs,booln,k,cur,b,lv,ek);
                case 'U': 
                case 'R': a=cur; c=qi->n-booln; 
			  if (k==0) b=cur; else b=qi->n-k*booln; 
			  if (bs->node[0]=='R') ek=bp2qeqPATH(a,b,c,booln);
			  return bp2qUR(qi,bp,bs,booln,k,cur,b,lv,0,ek);
                case '&': m0=bp2q0(qi,bp,bs->left,booln,k,cur,lv);
			  m1=bp2q0(qi,bp,bs->right,booln,k,cur,lv);
                          return mdcompose('c',m0,m1);
                case '|': m0=bp2q0(qi,bp,bs->left,booln,k,cur,lv);
			  m1=bp2q0(qi,bp,bs->right,booln,k,cur,lv);
                          return mdcompose('d',m0,m1);
                case '!':
                default:
			m0=tree2md(bs); mdstep1(m0,cur,cur,booln);
			return m0;
        }
}
/************************************************************************/
qmdata *bp2qEA(int e,qinfo *qi,int booln,int k,int cur,int cur1,int lv,qmdata *m0,qmdata *m1)
{
	qmdata *md;
	int *p0;
	int i;

	if (lv==1) {
		cur=cur1-booln;
		md=(qmdata *)calloc(1,sizeof(qmdata));
		p0=md->Clause=(int *)malloc((booln+2)*sizeof(int));
		*p0++=e; for (i=cur+1;i<=cur+booln;i++) *p0++=i; *p0=0;
		md->q=bp2qEA(e,qi,booln,k,cur,cur1,0,m0,m1);
	} else if (k==0) {
		if (e=='e') md=mdcompose('c',m0,m1); 
			else md=mdcompose('I',m0,m1); 
	} else {
		md=(qmdata *)calloc(1,sizeof(qmdata));
		p0=md->Clause=(int *)malloc((booln+2)*sizeof(int));
		*p0++=e; for (i=cur1+1;i<=cur1+booln;i++) *p0++=i; *p0=0;
		md->q=bp2qEA(e,qi,booln,k-1,cur,cur1+booln,0,m0,m1);
	}
	return md;
}
qmdata *bp2qFG(qinfo *qi,tree *bp,tree *bs,int booln,int k,int cur,int cur1,int lv,qmdata *ek)
{
	int x0,i;
        qmdata *md0,*md,*md1;
	int cur0;

	if (ek==NULL) { x0='d'; } else { x0='c'; }

        md=(qmdata *)calloc(1,sizeof(qmdata));
        md->Clause=(int *)malloc((2)*sizeof(int));
        md->Clause[0]=x0; md->Clause[1]=0;

        md0=md;
        for (i=0;i<=k;i++) {
		if (i==0) { cur0=cur; } else { cur0=cur1; cur1+=booln; }
		md->q=bp2q0(qi,bp,bs->right,booln,k,cur0,lv);
                md->next=(qmdata *)calloc(1,sizeof(qmdata)); md1=md;
                md=md->next;
        }
	if (ek==NULL) { free(md); md1->next=NULL; } else md->q=ek;
        return md0;
}
qmdata *bp2qUR(qinfo *qi,tree *bp,tree *bs,int booln,int k,int cur,int cur1,int lv,int start,qmdata *ek)
{
	int x0,x1;
        qmdata *m0,*m1,*m2;
	int cur0;

	if (start==0) { cur0=cur; } else { cur0=cur1; cur1+=booln; }
	if (ek==NULL) { x0='d'; x1='c'; } else { x0='c'; x1='d'; }

	m0=bp2q0(qi,bp,bs->right,booln,k,cur0,lv);
	if (start==k) { 
		if (ek==NULL) return m0;
		m1=bp2q0(qi,bp,bs->left,booln,k,cur0,lv); m2=ek;
		return mdcompose(x0,m0,mdcompose(x1,m1,m2)); 

	} 
	m1=bp2q0(qi,bp,bs->left,booln,k,cur0,lv);
	m2=bp2qUR(qi,bp,bs,booln,k,cur,cur1,lv,start+1,ek);

	return mdcompose(x0,m0,mdcompose(x1,m1,m2)); 
}
/************************************************************************/

/************************************************************************/
qmdata *irid(qinfo *qi,tree *bp,tree *bs,int booln,int k,int bv);
/************************************************************************/

/************************************************************************/
#define bpi bp->right->left
#define bpt bp->right->right->left
/************************************************************************/
qmdata *bp2qPATH(qinfo *qi,tree *bp,int booln,int k,int cur,int lev)
{
	int i;
	qmdata *md0,*md,*md1;
	int cur1=qi->n;

	if (k==0&&lev!=1) return NULL;

	md=(qmdata *)calloc(1,sizeof(qmdata));
	md->Clause=(int *)malloc((2)*sizeof(int));
	md->Clause[0]='c'; md->Clause[1]=0;

	md0=md;
	if (lev==1) {
		md->q=tree2md(bpi); mdstep1(md->q,cur,cur,booln); 
		md->next=(qmdata *)calloc(1,sizeof(qmdata)); md1=md;
		md=md->next;
		qi->n+=booln; 
	} 

	cur1=qi->n-booln;
	for (i=0;i<k;i++) {
		md->q=tree2md(bpt); 
		if (i==0) { mdstep1(md->q,cur,cur1,booln); }
		else { mdstep1(md->q,cur1,cur1,booln); }
		md->next=(qmdata *)calloc(1,sizeof(qmdata)); md1=md;
		md=md->next;
		cur1+=booln; 
		qi->n+=booln;
	}
	free(md); md1->next=NULL; 
	return md0;
}
/************************************************************************/
qmdata *bp2qeqPATH(int a,int b,int c,int booln)
{
        int i,j;
        tree *t0;
	int pa=1;
	qmdata *md;

	a=a/booln; b=b/booln; c=c/booln;

        if (a==b) {
		t0=treeconstab(0,a*booln+1);
		md=tree2md(t0); treefree(t0); return md;
	}
        t0=eqstate(pa,a,pa,b,-1);

        for (i=b-1;i<c;i++)
        for (j=i+1;j<=c;j++) {
		if (i==b-1) {
                	if (j==b) continue;
                	t0=treecompose("|",t0,eqstate(pa,a,pa,j,-1));
		} else { 
                	t0=treecompose("|",t0,eqstate(pa,i,pa,j,-1));
		}
	}
	eqstateconstruct(t0,booln);

	md=tree2md(t0); treefree(t0);
	return md;
}

/************************************************************************/
void freeqmdata(qmdata *md)
{
        if (md->q!=NULL) freeqmdata(md->q);
        if (md->next!=NULL) freeqmdata(md->next);
        if (md->Clause!=NULL) free(md->Clause); free(md);
}
/************************************************************************/
int qmdwrite(qmdata *md,qinfo *qi,FILE *g)
{
	fprintf(g,"QBF\n%i\n",qi->n); qmdwrite1(md,g,0); fprintf(g,"QBF\n"); 
}
int qmdwrite1(qmdata *md,FILE *g,int mode)
{
	int t0;
	if (md==NULL) return; t0=md->Clause[0];
	switch (t0) {
		case 'a': 
		case 'e': 
			if (mode!='q') fprintf(g,"q\n"); 
			fprintf(g,"%c ",t0); 
			intwrite(md->Clause+1,0,intlen(md->Clause+1)-1,g);
			qmdwrite1(md->q,g,'q');
			if (mode!='q') fprintf(g,"/q\n"); 
			break;
		case 'c': 
		case 'd': 
			fprintf(g,"%c\n",t0); 
			pnintwrite(md->Clause+1,g);
			qmdwrite1(md->q,g,0);
			for (;md->next!=NULL;md=md->next)
				qmdwrite1(md->next->q,g,0);
			fprintf(g,"/%c\n",t0); 
			break;
	}
}
/*******************************************************************/
void pnintwrite(int *ss,FILE *g)
{
        int i=0;
        for (i=0;ss[i];i++) if (ss[i]>0) { fprintf(g,"%i ",ss[i]); }
        fprintf(g,"\n");
        for (i=0;ss[i];i++) if (ss[i]<0) { fprintf(g,"%i ",-ss[i]); }
        fprintf(g,"\n");
}
/*******************************************************************/
#define single(md) \
	((md->q==NULL)&& \
	 (md->Clause[0]=='c'||md->Clause[0]=='d')&& \
	 (md->Clause[2]==0))
/************************************************************************/
void qmdsimp(qmdata *md,qinfo *qi)
{
	int t0;
	qmdata *md0; /* empty clauses are not considered */

	if (md==NULL) return; t0=md->Clause[0];
	switch (t0) {
	    case 'a': 
	    case 'e': 
		qmdsimp(md->q,qi); 
		break;
	    case 'c': 
	    case 'd': 
		while (md->q!=NULL) {
			if (md->q->Clause[0]==t0||single(md->q)) {
				qi->m--; qi->l-=2;
				mdmoveup1(md,md,md->q); continue;
			}
			qmdsimp(md->q,qi); break;
		}
		for (md0=md;md->next!=NULL;) {
			if (md->next->q==NULL) {
				free(md->next); md->next=NULL; break;
			}
			if (md->next->q->Clause[0]==t0||single(md->next->q)) {
				qi->m--; qi->l-=2;
				mdmoveup1(md0,md->next,md->next->q); 
				continue;
			}
			qmdsimp(md->next->q,qi); md=md->next;
		}
		break;
	}
}
/************************************************************************/
void mdmoveup1(qmdata *md0,qmdata *md,qmdata *mdq)
{
	qmdata *mdn=md->next;

	md0->Clause=mdintegrate(md0->Clause,mdq->Clause);

	if (mdq->q!=NULL) {
		md->q=mdq->q;
		md->next=mdq->next;
		while (md->next!=NULL) md=md->next; md->next=mdn;
		free(mdq->Clause); free(mdq);
	} else if (mdn!=NULL) {
		md->q=mdn->q; md->next=mdn->next; free(mdn);
		free(mdq->Clause); free(mdq);
	} else {
		free(mdq->Clause); free(mdq); 
		md->q=NULL;
	}
}

int *mdintegrate(int *a1,int *a2)
{
	int x=intlen(a1);
	int y=intlen(a2);
	int i;

	a1=(int *)realloc(a1,(x+y)*sizeof(int));
	for (i=1;i<=y;i++) a1[x+i-1]=a2[i]; return a1;
}
int qmdlen(qmdata *md)
{
	int k=0;

	if (md==NULL) return 0; 
	switch (md->Clause[0]) {
		case 'a': 
		case 'e': k=intlen(md->Clause+1)+1; k+=qmdlen(md->q)+1;
			  break;
		case 'c': 
		case 'd': k=intlen(md->Clause+1)+1; 
			  if (md->q==NULL) break; k+=qmdlen(md->q)+1; 
			  for (;md->next!=NULL;md=md->next)
			   	k+=qmdlen(md->next->q)+1;
			  break;
	}
	return k;
}
/*******************************************************************/
int qmdclauses(qmdata *md)
{
	int k=0;

	if (md==NULL) return 0; 
	switch (md->Clause[0]) {
		case 'a': 
		case 'e': k+=qmdclauses(md->q); break;
		case 'c': 
		case 'd': if (md->q==NULL) break; k+=qmdclauses(md->q); 
			  for (;md->next!=NULL;md=md->next)
			   	k+=qmdclauses(md->next->q);
			  break;
	}
	return k+1;
}
/*******************************************************************/

/************************************************************************/
qmdata *tree2md(tree *t)
{
	int x;
	qmdata *md;
	if (t==NULL) return NULL;

	md=(qmdata *)calloc(1,sizeof(qmdata));
	switch (t->node[0]) {
		case '|': 
		case '&': 
			  md->Clause=(int *)malloc((2)*sizeof(int));
			  if (t->node[0]=='|') x='d'; else x='c';
			  md->Clause[0]=x; md->Clause[1]=0;
			  md->q=tree2md(t->left);
			  md->next=(qmdata *)calloc(1,sizeof(qmdata));
			  md->next->q=tree2md(t->right);
			  return md;
		case '^': 
		case '_': 
			  md->Clause=(int *)malloc((4)*sizeof(int));
			  if (t->node[0]=='^') x='d'; else x='c';
			  md->Clause[0]=x; 
			  md->Clause[1]=1; md->Clause[2]=-1; md->Clause[3]=0;
			  return md;
		default: 
			  md->Clause=(int *)malloc((3)*sizeof(int));
			  if (t->node[0]=='!') x=-atoi(t->right->node);
			  else x=atoi(t->node); 
			  md->Clause[0]='d'; md->Clause[1]=x; md->Clause[2]=0;
			  return md;
	}
	
}
/************************************************************************/
qmdata *mdnegate(qmdata *m0)
{
	int *p0;
	qmdata *m1=m0;
	if (m0==NULL) return NULL;
	switch (m0->Clause[0]) {
		case 'a': m0->Clause[0]='e'; break;
		case 'e': m0->Clause[0]='a'; break;
		case 'c': m0->Clause[0]='d'; 
			  for (p0=m0->Clause+1;*p0;p0++) *p0=-*p0; break;
		case 'd': m0->Clause[0]='c'; 
			  for (p0=m0->Clause+1;*p0;p0++) *p0=-*p0; break;
	}
	mdnegate(m0->q);
	while (m0->next!=NULL) { mdnegate(m0->next->q); m0=m0->next; }
	return m1;
}
/************************************************************************/
void mdstep1(qmdata *m0,int i,int j,int booln)
{
	int *p0;
	if (m0==NULL) return;
	for (p0=m0->Clause+1;*p0;p0++) {
		if (*p0>0) { if (*p0<=booln) *p0+=i; else *p0+=j; }
		else if (*p0>=-booln) *p0-=i; else *p0-=j;
	}
	mdstep1(m0->q,i,j,booln);
	while (m0->next!=NULL) { mdstep1(m0->next->q,i,j,booln); m0=m0->next; }
}
/************************************************************************/
qmdata *mdcompose(int a,qmdata *m0,qmdata *m1)
{
	qmdata *md;

	if (m0==NULL) return m1; if (m1==NULL) return m0;
	if (a=='I') return mdcompose('d',mdnegate(m0),m1);
	
	md=(qmdata *)calloc(1,sizeof(qmdata));
	md->Clause=(int *)malloc((2)*sizeof(int));
	md->Clause[0]=a; md->Clause[1]=0; md->q=m0;
	md->next=(qmdata *)calloc(1,sizeof(qmdata)); md->next->q=m1;
	return md;
}
/************************************************************************/
tree *md2tree(qmdata *md)
{
	tree *t1,*t2;
	char c[2]="0";

	switch (md->Clause[0]) {
		case 'd': 
		case 'c': 
			if (md->Clause[0]=='c') c[0]='&'; else c[0]='|';
			t1=maketree1(c,md->Clause+1);
			t2=maketree2(c,md->q,md->next);
			return treecompose(c,t1,t2);
		case 'a': 
		case 'e': 
			return md2tree(md->q);
	}
}
/************************************************************************/
tree *maketree1(char *c,int *s)
{
	tree *t;
	if (*s==0) { if (c[0]=='&') return treeconst(1); return treeconst(0); }
	t=maketree1(c,s+1); if (s[0]>0) return treecompose(c,treenum(s[0]),t);
	return treecompose(c,treecompose("!",NULL,treenum(-s[0])),t);
}
tree *maketree2(char *c,qmdata *q1,qmdata *qn)
{
     if (q1==NULL) { if (c[0]=='&') return treeconst(1); return treeconst(0);}
     if (qn==NULL) { return md2tree(q1); }
     return treecompose(c,md2tree(q1),maketree2(c,qn->q,qn->next));
}
/************************************************************************/
int md2treetype(qmdata *md)
{
	int r=0;
	if (md==NULL) return r;
	switch (md->Clause[0]) { case 'a': r=1; break; case 'e': r=2; break; }
	r=r|md2treetype(md->q);
	while (md->next!=NULL) { r=r|md2treetype(md->next->q); md=md->next; }
	return r;
}
/************************************************************************/
