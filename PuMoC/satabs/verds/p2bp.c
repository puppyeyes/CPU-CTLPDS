#include "p2bp.h"

/*******************************************************************/
tree *ARITmod(tree *s1,tree *s2)
{
	tree *a1,*a2;
	s1=treedopost(s1,TLFnnfsimp); 
	s2=treedopost(s2,TLFnnfsimp); 
	a1=ARITdiv(s1,s2);
	a2=ARITmul(a1,s2); treefree(a1);
	a1=ARITapply(treecompose("-",treecopy(s1),a2));
	a1=treedopost(a1,TLFnnfsimp); 
	return a1;
}
/*******************************************************************/
tree *ARITdiv(tree *x1,tree *x2)
{
	int i=0;
        int a1=listlength(x1);
	tree **tpos;
        tree *s0,*s1,*s2,*s3,*s4,*s5,*r0;

	tpos=(tree **)malloc(a1*sizeof(tree *));

	s0=treecompose(FALSEs,NULL,NULL);
	for (i=0;i<a1;i++) {
		s1=ARITdoublekt(treecopy(x2),a1-i-1);
		s2=listcombine(ARITadd,s0,s1); s2->node[0]=';';
		s2=treedopost(s2,TLFnnfsimp); 
		s3=treecompose("}",treecopy(x1),s2);
		tpos[i]=ARITapply(s3); tpos[i]=treedopost(tpos[i],TLFnnfsimp); 
		s4=ARITmul(s1,tpos[i]); s4=treedopost(s4,TLFnnfsimp); 
		s5=listcombine(ARITadd,s0,s4); s5->node[0]=';';
		treefree(s0); treefree(s1); treefree(s4); 
		s0=s5; s0=treedopost(s0,TLFnnfsimp); 
		s0=listreduce(s0);
	}
	treefree(s0); 
	r0=tpos[a1-1];

	for (i=a1-2;i>=0;i--) r0=treecompose(";",tpos[i],r0); free(tpos);

	return listreduce(r0);
}
/*******************************************************************/
tree *ARITdoublekt(tree *s2,int k)
{
	int i;
        tree *a0=s2;
	tree *a1=treeconst(0);

	if (k==0) return s2;
        while (a0->node[0]==';') a0=a0->right;
	for (i=1;i<k;i++) a1=treecompose(";",treeconst(0),a1);
        treereplace(a0,treecompose(";",treecopy(a0),a1)); return s2;
}
/*******************************************************************/
tree *listreduce(tree *s1)
{
	tree *s2;
	if (s1==NULL) return s1;
	if (s1->node[0]!=';') return s1;
	if (s1->left->node[0]!='_') return s1;
	s2=s1->right; free(s1->left); free(s1); return listreduce(s2);
}

/*******************************************************************/
#define simploc(s) s=treedopost(s,TLFnnf); s=treedopost(s,TLFnnfsimp); 
/*******************************************************************/
tree *ARITadd(tree *s1,tree *s2,tree *s0)
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
tree *ARITsub(tree *s1,tree *s2,tree *s0)
{
	tree *a1,*b1,*c1,*a0,*c0,*r0;

	a1=s1; b1=s2;
	c0=land(neg(treecopy(a1)),treecopy(b1));
	a0=xor(treecopy(a1),treecopy(b1));
	if (s0==NULL) { return treecompose("-",c0,a0); }
	c1=s0->left;
	treefree(c0);
	c0=land(treecopy(c1),treecopy(b1));
	c0=lor(c0,land(neg(treecopy(a1)),lor(treecopy(c1),treecopy(b1))));
	a0=xor(a0,treecopy(c1));
	r0=treecompose("-",c0,treecompose(";",a0,s0->right));
	treefree(s0->left); free(s0); return r0;
}
/*******************************************************************/
tree *ARITdouble(tree *s2)
{
        tree *a0=s2;

        while (a0->node[0]==';') a0=a0->right;
        treereplace(a0,treecompose(";",treecopy(a0),treeconst(0)));
	return s2;
}
tree *ARITmul(tree *x1,tree *x2)
{
	int i=0;
        tree *t0,*r0,*r1,*r2;
        
        r0=NULL;
        while ((t0=listgetone(x2,i++))!=NULL) {
                if (r0!=NULL) r0=ARITdouble(r0);
                switch (t0->node[0]) {
                        case FALSE: r1=NULL; break;
                        case TRUE:  r1=treecopy(x1); break;
                        default:    r1=listcombinedist(ARITband,x1,t0); 
				    r1=treedopost(r1,TLFnnfsimp);
				    break;
                }
		if (r1==NULL) continue;
                if (r0==NULL) { r0=r1; continue; }
                r2=listcombine(ARITadd,r0,r1); r2->node[0]=';'; 
                treefree(r0); treefree(r1); 
		r0=r2; r0=treedopost(r0,TLFnnfsimp);
	}
        if (r0==NULL) return treeconst(0); else return r0;
}
/*******************************************************************/
tree *ARITeq(tree *s1,tree *s2,tree *s0)
{
	tree *a0;

	a0=leq(treecopy(s1),treecopy(s2));
	if (s0==NULL) return a0;
	return treecompose("&",a0,s0); 
}
tree *ARITband(tree *s1,tree *s2,tree *s0)
{
        tree *a0;

        a0=land(treecopy(s1),treecopy(s2));
        if (s0==NULL) return a0;
        return treecompose(";",a0,s0);
}
tree *ARITbor(tree *s1,tree *s2,tree *s0)
{
        tree *a0;

        a0=lor(treecopy(s1),treecopy(s2));
        if (s0==NULL) return a0;
        return treecompose(";",a0,s0);
}
/*******************************************************************/
tree *ARITapply(tree *t)
{
        tree *t0,*t1;

        if (t==NULL) return NULL;
        switch (t->node[0]) {
          case '+':
                t0=listcombine(ARITadd,t->left,t->right); treefree(t); 
		t0->node[0]=';'; simploc(t0); return t0;
          case '-':
                t0=listcombine(ARITsub,t->left,t->right); treefree(t); 
		t=t0; t0=t0->right; free(t->left); free(t); 
		simploc(t0); return t0;
          case '*':
                t0=ARITmul(t->left,t->right); treefree(t); 
		simploc(t0); return t0;
          case '/':
                t0=ARITdiv(t->left,t->right); treefree(t); 
		simploc(t0); return t0;
          case '%':
                t0=ARITmod(t->left,t->right); treefree(t); 
		simploc(t0); return t0;
          case '=':
                t0=listcombine(ARITeq,t->left,t->right); treefree(t); 
		simploc(t0); return t0;
          case BOR:
                t0=listcombine(ARITbor,t->left,t->right); treefree(t); 
		simploc(t0); return t0;
          case DAND:
                t0=listcombinedist(ARITband,t->left,t->right); treefree(t); 
		simploc(t0); return t0;
          case '<':
                t0=listcombine(ARITsub,t->left,t->right); treefree(t); 
		t=t0; t0=t0->left; free(t->right); free(t); 
		simploc(t0); return t0;
          case '>':
                t0=listcombine(ARITsub,t->left,t->right); 
                t1=listcombine(ARITeq,t->left,t->right); treefree(t); 
		t=t0; t0=t0->left; free(t->right); free(t); 
		return treecompose("&",neg(t0),neg(t1));
          case 'B': t->node[0]='='; return neg(ARITapply(t));
          case '}': t->node[0]='<'; return neg(ARITapply(t));
          case '{': t->node[0]='>'; return neg(ARITapply(t));
        }
        return t;
}

/*******************************************************************/
tree *BOOLconstructnum(int i)
{
	tree *t0=treeconst(i%2); 
	i/=2;
	while (i) { t0=treecompose(";",treeconst(i%2),t0); i/=2; }
	return t0;
}
/*******************************************************************/
tree *BOOLconstructvar(int a,int b)
{
	tree *t0=treenum(b); 
	b--;
	while (b>=a) { t0=treecompose(";",treenum(b),t0); b--; }
	return t0;
}
/*******************************************************************/
void BOOLreplace(tree *t,tree *vlist,int booln)
{
	int boolna=0;
	int n,low,high;
	tree *v;

	if (t==NULL) return;
	if (isdigit(t->node[0])) {
		treereplace(t,BOOLconstructnum(atoi(t->node)));
		return;
	}
	switch (t->node[0]) {
	  case 'O':
		boolna=booln;
	  case 'V':
		n=arrayindex(t->node+1);
		v=treesearchvarlist(t->node+1,n,vlist); 
		if (v==NULL) error("vlist");
		low=atoi(v->left->left->node)+boolna;
		high=atoi(v->left->right->node)+boolna;
		treereplace(t,BOOLconstructvar(low,high)); 
		return; 
	}

	BOOLreplace(t->left,vlist,booln); BOOLreplace(t->right,vlist,booln);
}
/*******************************************************************/
tree *BOOLconverttrans(tree *t)
{
        if (t->node[0]==';') t->node[0]='|';
        else if (t->node[0]==':') t->node[0]='&';
        return t;
}
/*******************************************************************/

/*******************************************************************/
#define ptrans prog->right->right->left
/*******************************************************************/
workspace *p2bp(workspace *ws1)
{
	workspace *ws2=(workspace *)malloc(sizeof(workspace));
	tree *prog;

	ws2->prog=prog=treecopy(ws1->prog);
	ws2->varconstr=treecopy(ws1->varconstr);
	ws2->deadlockfreecond=treecopy(ws1->deadlockfreecond);
	ws2->booln=ws1->booln;
	ws2->puretrans=ws1->puretrans;

	BOOLreplace(prog->right,prog->left,ws1->booln); 
	prog->right=treedopost(prog->right,ARITapply); 
	prog->right=treedopost(prog->right,TLFnnf); 
	ptrans=treedopost(ptrans,BOOLconverttrans); 
	prog->right=treedopost(prog->right,TLFnnfsimp); 

	BOOLreplace(ws2->varconstr,prog->left,ws1->booln); 
	ws2->varconstr=treedopost(ws2->varconstr,ARITapply); 
	ws2->varconstr=treedopost(ws2->varconstr,TLFnnf); 
	ws2->varconstr=treedopost(ws2->varconstr,TLFnnfsimp); 

	BOOLreplace(ws2->deadlockfreecond,prog->left,ws1->booln); 
	ws2->deadlockfreecond=treedopost(ws2->deadlockfreecond,ARITapply); 
	ws2->deadlockfreecond=treedopost(ws2->deadlockfreecond,TLFnnf); 
	ws2->deadlockfreecond=treedopost(ws2->deadlockfreecond,TLFnnfsimp); 

	return ws2;
}
