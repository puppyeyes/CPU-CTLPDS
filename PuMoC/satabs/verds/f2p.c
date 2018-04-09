#include "f2p.h"
#include "prep.h"

/*******************************************************************/
tree *PROGaddeq_constructeq(tree *v)
{
	tree *t0,*t1,*t2;

	if (v==NULL) return NULL;
	if (v->node[0]==';') { 
		if (v->right==NULL) return PROGaddeq_constructeq(v->left);
		t0=(tree *)malloc(sizeof(tree)); strcpy(t0->node,"&");
		t0->left=PROGaddeq_constructeq(v->left);
		t0->right=PROGaddeq_constructeq(v->right);
		return t0;
	} 
	if (v->node[0]!=':') error("read"); 

	t0=(tree *)malloc(sizeof(tree)); strcpy(t0->node,"=");
	t1=(tree *)malloc(sizeof(tree)); strcpy(t1->node,v->left->node);
	t2=(tree *)malloc(sizeof(tree)); strcpy(t2->node,v->left->node);
	t0->left=t1; t0->right=t2;
	t1->left=NULL; t1->right=NULL;
	t2->left=NULL; t2->right=NULL;
	return t0;
}
void *PROGaddeq_modify(tree *t,tree *v)
{
	tree *t1;

	if (t==NULL) return;
	if (t->node[0]=='&') { 
		PROGaddeq_modify(t->left,v);
		PROGaddeq_modify(t->right,v); return;
	}
	if (t->node[0]!='=') error("modify"); 

	t1=treesearchvarall(t->left->node,arrayindex(t->left->node),v);
	if (t1!=NULL) treereplace(t,t1);
}
void *PROGaddeq(tree *t,tree *v)
{
	int lc=0;
	tree *t0,*l0;

	while ((l0=listgetone(t,lc++))!=NULL) 
	if (l0->node[0]==':') { 
		t0=PROGaddeq_constructeq(v); 
		PROGaddeq_modify(t0,l0->right);
		treefree(l0->right); l0->right=t0;
	}
}
/*******************************************************************/

/*******************************************************************/
tree *PROGarraycond(tree *t,int i)
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
		t1=PROGarraycond(t->left,i);
		t2=PROGarraycond(t->right,i);
		return treecompose("|",t1,t2);
	}
}
tree *PROGreplacearraybody1construct(tree *t,char *node,int k,int m,int n)
{
	int i;
	char var[MAXVARLEN];
	char nvar[MAXVARLEN];
	tree *tvar,*tcond;
	tree *left,*right;

	strcpy(var+1,node); var[k]=0; var[0]='O';

	for (tvar=NULL,i=n;i>=m;i--) {
		tcond=PROGarraycond(t,i);
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
tree *PROGarray1var(char *node,int m,int n)
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
tree *PROGarray1varlist(char *node,int m,int n,tree *vd,tree *vnext)
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
void PROGreplacearraybody1(tree *t,char *node,int k,int m,int n)
{
	tree *t1,*t2;
	tree *vcur;
	char var[MAXVARLEN];

        if (t==NULL) return;
	if (t->node[0]!=';') { vcur=t; }
	else { PROGreplacearraybody1(t->right,node,k,m,n); vcur=t->left; } 

	strcpy(var+1,node); var[k+1]=0; var[0]='O'; 
	t1=treesearchvarall(var,arrayindex(var),vcur->right);
	t2=PROGreplacearraybody1construct(t1,node,k,m,n); treefree(t1);
	t1=treecompose("&",treecopy(vcur->right),t2);
	treereplace(vcur->right,t1); 
}       
/*******************************************************************/
void PROGreplacearraybody2(tree *t,char *node,int k,int m,int n)
{
	tree *t0;

        if (t==NULL) return;
	if (strncmp(t->node+1,node,k)==0) {
		t0=PROGarray1var(t->node,m,n);
		treereplace(t,t0); return; 
	}
        PROGreplacearraybody2(t->left,node,k,m,n);
        PROGreplacearraybody2(t->right,node,k,m,n);
}       
/*******************************************************************/
tree *PROGreplacearray(tree *body,tree *vars)
{
	int i=0,k=0;
	char *p1;
	tree *t,*vnext,*v;
	int low,high;

	
	if (vars->node[0]!=';') { vnext=NULL; v=vars; }
	else { vnext=PROGreplacearray(body,vars->right); v=vars->left; }

	p1=v->left->node;
	while (*p1&&*p1!='[') { p1++; k++; }
	if (*p1==0) { if (vnext!=NULL) vars->right=vnext; return vars; }

	p1++;
	low=atoi(p1); 
	while (isdigit(*p1)) p1++; while (!isdigit(*p1)) p1++;
	high=atoi(p1); 
	while (*p1&&*p1!='[') p1++; if (*p1) error("array");
	
	PROGreplacearraybody1(body->right->left,v->left->node,k+1,low,high);
	PROGreplacearraybody2(body,v->left->node,k+1,low,high);
	t=PROGarray1varlist(v->left->node,low,high,v->right,vnext);
	treefree(v->left); return t;
}
/*******************************************************************/
tree *PROGcleanarray(tree *t)
{
        char *p1,*p2;
        if (arrayindex(t->node)) {
                p1=t->node; p2=t->node;
                while (*p1) {
                        if (*p1=='.'&&*(p1+1)=='.') while (*p1!=']') p1++;
                        *p2++=*p1++;
                }
                *p2++=*p1++;
        }
        return t;
}
/*******************************************************************/

/*******************************************************************/
void PROGreplacecolon(tree *t)
{
        tree *t1,*t2;
        if (t->node[0]==':') {
                PROGreplacecolon(t->right);
                if (t->right->node[0]==':') {
                        t1=treecompose("&",treecopy(t->left),
                                treecopy(t->right->left));
                        t2=treecompose(":",t1,treecopy(t->right->right));
                        treereplace(t,t2);
                }
        }
}
/*******************************************************************/
void PROGreplacesemiand(tree *t)
{
        if (t->node[0]==';') t->node[0]='&';
}
/*******************************************************************/
void PROGreplacechan(tree *body,tree *vars)
{
        tree *l0;
        int j;
        char v1[MAXVARLEN],v2[MAXVARLEN];
        char *p,*q;
        tree *mylist=vars;
        tree *var1,*var2;

        while (1) {
		if (mylist->node[0]==';') l0=mylist->left; else l0=mylist;
                if (j=chanindex(l0->left->node)) {
                        p=l0->left->node; strcpy(v1,p); q=v1;
                        while (*p!='[') { p++; q++; }
                        sprintf(p,"[0..%i]",j-1); sprintf(q,"_");
                        sprintf(v2,"0..%i",j); 
                        var1=treecompose(v2,NULL,NULL);
                        var1=treecompose(":",treecompose(v1,NULL,NULL),var1);
                        var2=treecompose(";",var1,treecopy(mylist));
                        treereplace(mylist,var2);
                        strcpy(q,"[.]");
                        PROGreplacechanbody(body->right,v1,j);
                        PROGreplacechaninit(body->left,v1,j);
                }
                if (l0==mylist) break; mylist=mylist->right;
        }
}
/*******************************************************************/
void PROGreplacechanbody(tree *t,char *node,int j)
{
	int i;
	char *p1,*p2;
	char var[MAXVARLEN];
	char var2[MAXVARLEN];
	char node1[MAXVARLEN];
	tree *t1,*t2,*t3;

	if (t==NULL) return;
        PROGreplacechanbody(t->left,node,j);
        PROGreplacechanbody(t->right,node,j);
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
void PROGreplacechaninit(tree *t,char *node,int j)
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
tree *PROGreplacesynnotations(tree *prog)
{
	listtraverse(prog->right->right->left,PROGreplacecolon);
        treedopostvoid(prog->right->left,PROGreplacesemiand);
        PROGreplacechan(prog->right,prog->left);
	return prog;
}
/*******************************************************************/

/*******************************************************************/
workspace *p2ws(tree *prog)
{
	workspace *ws1=(workspace *)malloc(sizeof(workspace));

	ws1->prog=prog;
	ws1->varconstr=INFOconstr_a(prog->left);
        ws1->booln=INFOvlist(prog->left,0);
	ws1->deadlockfreecond=INFOdeadlockfreecond(prog->right->right->left);
        ws1->puretrans=INFOpuret(prog->right->right->left);
	ws1->prog=prog;



	return ws1;
}
/*******************************************************************/


/*******************************************************************/
tree *f2p0(char *fn,tree *processes)
{
	char *string;
	module *mm;
        tree *prog;

        prts("read file"); 
	string=PREPfile2txt(fn);
	mm=PROGtxt2modules(string); 
	prog=PROGmerge(mm,processes); 
	prog=PROGreplacesynnotations(prog);

        free(string); return prog;
}
tree *f2p1(tree *prog)
{
	prog=PROGreplacemarray(prog);
	prog->right=TLFnnf(prog->right); 
	prog->right->right->left=adddeadlocktr(prog->right->right->left,prog); 
	PROGaddeq(prog->right->right->left,prog->left);
	prog->right=treedopost(prog->right,PROGcleanarray);
	PROGaddnextv(prog,1);
	prog->left=PROGreplacearray(prog->right,prog->left);
	PROGaddnextv(prog,0);
	prog->right=treedopost(prog->right,TLFnnfsimp);


	return prog;
}

/*******************************************************************/
tree *adddeadlocktr(tree *t,tree *prog)
{
	tree *t0;

	t0=treecompose(":",TLFnegate(INFOdeadlockfreecond(t)),NULL);
	PROGaddeq(t0,prog->left); 

	return listmerge(t0,t);
}
/*******************************************************************/

/*******************************************************************/
int INFOvlist_count(tree *v)
{
        if (v==NULL) return 1;
        return INFOvlist_count(v->left)+INFOvlist_count(v->right);
}
int INFOvlist(tree *v,int p)
{
        tree *tvar,*tdomain;
        int i,j;
	char low[MAXVARLEN],high[MAXVARLEN];
	char *p1;

        if (v==NULL) error("tree");
        if (v->node[0]==';') { tvar=v->left->left; tdomain=v->left->right; }
        else if (v->node[0]==':') { tvar=v->left; tdomain=v->right; }
	else error("teee");

	if (tvar->left!=NULL) free(tvar->left);
	if (tvar->right!=NULL) free(tvar->right);

	if (tdomain->left==NULL&&tdomain->right==NULL) {
		i=1;
		p1=tdomain->node;
		while (isdigit(*p1)) p1++; 
		if (*p1++=='.'&&*p1++=='.') i+=atoi(p1);
	} else {
		i=INFOvlist_count(tdomain)/2;
	} 

	i=i-1; j=1; while ((i=i/2)>0) j++;
	sprintf(low,"%i",p+1);
	sprintf(high,"%i",p+j);
	tvar->left=treecompose(low,NULL,NULL);
	tvar->right=treecompose(high,NULL,NULL);

        if (v->node[0]==';') return INFOvlist(v->right,p+j); 
	return  p+j;
}
/*******************************************************************/
int INFOpuret(tree *t)
{
        if (t==NULL) return 1;
        if (t->node[0]==';') return INFOpuret(t->left)&&INFOpuret(t->right);
        if (t->node[0]==':') return INFOpuret(t->right);
        if (t->node[0]=='&') return INFOpuret(t->left)&&INFOpuret(t->right);
        if (t->node[0]=='=') if (t->left->node[0]=='O') return 1; else return 0;
        return 1;
}
/*******************************************************************/
tree *INFOdeadlockfreecond(tree *t)
{
	int lc=0;
        tree *t0,*l0;

	t0=NULL;
	while ((l0=listgetone(t,lc++))!=NULL) if (l0->left!=NULL) {
		if (t0==NULL) t0=treecopy(l0->left);
		else t0=treecompose("|",t0,treecopy(l0->left));
        }
        return t0;
/*
	l0=listgetone(t,lc++); 
	if (l0!=NULL) return TLFnegate(treecopy(l0->left)); return NULL;
*/
}
/*******************************************************************/
tree *INFOconstr_a(tree *v)
{
        tree *tvar,*tdomain,*t;
        int i;
	char *p1;
	char node[MAXVARLEN];

        if (v==NULL) error("tree");
        if (v->node[0]==';') { tvar=v->left->left; tdomain=v->left->right; }
        else if (v->node[0]==':') { tvar=v->left; tdomain=v->right; }
	else error("teee");

	if (tdomain->left==NULL&&tdomain->right==NULL) {
		i=1;
		p1=tdomain->node;
		while (isdigit(*p1)) p1++; 
		if (*p1++=='.'&&*p1++=='.') i+=atoi(p1);
	} else { i=INFOvlist_count(tdomain)/2; } 
	
	strcpy(node+1,tvar->node); node[0]='V';
	t=neg(treecompose(">",treecompose(node,NULL,NULL),treenum(i-1)));

        if (v->node[0]==';') return treecompose("&",t,INFOconstr_a(v->right)); 
	return  t;
}
/**
tree *constr(tree *v)
{
        tree *tvar,*tdomain,*t;
        int i;
	char *p1;

        if (v==NULL) error("tree");
        if (v->node[0]==';') { tvar=v->left->left; tdomain=v->left->right; }
        else if (v->node[0]==':') { tvar=v->left; tdomain=v->right; }
	else error("teee");

	if (tdomain->left==NULL&&tdomain->right==NULL) {
		i=1;
		p1=tdomain->node;
		while (isdigit(*p1)) p1++; 
		if (*p1++=='.'&&*p1++=='.') i+=atoi(p1);
	} else { i=INFOvlist_count(tdomain)/2; } 
	t=neg(treecompose(">",treecopy(tvar),treenum(i-1)));

        if (v->node[0]==';') return treecompose("&",t,constr(v->right)); 
	return  t;
}
/*******************************************************************/

/*******************************************************************/
void PROGmarraycount(char *node,int *ma)
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
tree *PROGreplacemarray(tree *prog)
{
	int lc=0;
	int i,j,mb;
	int ma[32]; 
	tree *l0;
	char *p1;
	tree *body,*vars;

	body=prog->right;	
	vars=prog->left;
	while ((l0=listgetone(vars,lc++))!=NULL) 
	if (j=marrayindex(l0->left->node)) {
		PROGmarraycount(l0->left->node,ma);
		for (mb=1,i=1;i<=ma[0];i++) mb*=ma[i];
		p1=l0->node;
		while (*p1!='[') p1++; 
		sprintf(p1,"[0..%i]",mb-1); 
		PROGreplacemarraybody(body,l0->left->node,j,ma);
	}
	return prog;
}
/*******************************************************************/
void PROGreplacemarraybody(tree *t,char *node,int j,int *ma)
{
	int i;
	char *p1,*p2;
	char var[MAXVARLEN];

	if (t==NULL) return;
        PROGreplacemarraybody(t->left,node,j,ma);
        PROGreplacemarraybody(t->right,node,j,ma);
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
void reppar(char *s) 
{
	for (;*s;s++) 
	if (*s=='('&&*(s+1)==')'&&*(s+2)==';') { *(s)=':'; *(s+1)='1'; }
}
/*******************************************************************/
tree *PROGcheck(tree *t)
{
        tree *t0;

        t0=t->right->right;
        if (t0->node[0]!='#') t->right->right=treecompose("#",t0,NULL);
        t0=t->right->left;
        if (t0==NULL) t->right->left=treecompose("=",treenum(1),treenum(1));
        return t;
}
/*******************************************************************/
tree *PROGcompose(tree *t0,tree *t1,tree *t2,tree *t3)
{
        return treecompose("#",t0,treecompose("#",t1,treecompose("#",t2,t3)));
}
/*******************************************************************/
void PROGgetmain(char *p2,module *mm,int last)
{
	char *p0=p2;

	if (!last) {
		reppar(p0);
		while (*p0!='#') p0++; p0++;
		while (*p0!='#') p0++; p0++;
		stringreplacechar(p0,'(',':');
		stringreplacechar(p0,')',' ');
		stringreplacechar(p0,',','|');
		stringcompress(p2,' ',' '); 
	}
	mm->parameters=NULL;
	mm->prog=PROGcheck(string2tree(p2)); 
}
void PROGgetmodule(char *p2,module *mm)
{
	char *p0=p2;

	while (*p0&&*p0!=')') p0++; 
	*p0=0;
	stringreplacechar(p2,'(',':');
	stringreplacechar(p2,')',';');
	stringreplacechar(p2,',','|');

	mm->parameters=string2tree(p2);
	if (mm->parameters->node[0]!=':') 
	mm->parameters=treecompose(":",mm->parameters,treenum(1)); /*reppar*/
	mm->prog=PROGcheck(string2tree(p0+1)); 
}
module *PROGtxt2modules(char *s)
{
	module *mm,*mm0;
	char *p1,*p2;
	int last=0;

	p1=s; 

	if (strncmp(p1,"MAIN",4)==0) p1+=4;
	else if (strncmp(p1,"VVM",3)==0) p1+=3;
	else error("VVM must be specified at the beginning of the input file.");

	p2=p1; 
	while (1) {
		while (*p1&&*p1!='M') p1++; if (*p1==0) last=1;
		if (last||(strncmp(p1,"MODULE",6)==0)) { 
			*p1=0; 
			mm=(module *)malloc(sizeof(module));
			PROGgetmain(p2,mm,last); 
			if (last) { mm->next=NULL; return mm; }
			p1+=6; p2=p1; break;
		} 
	}
	mm0=mm; 
	while (1) {
		while (*p1&&*p1!='M') p1++; if (*p1==0) last=1;
		if (last||(strncmp(p1,"MODULE",6)==0)) { 
			*p1=0; 
			mm->next=(module *)malloc(sizeof(module));
			mm=mm->next;
			PROGgetmodule(p2,mm);
			if (last) { mm->next=NULL; return mm0; }
			p1+=6; p2=p1; 
		} 
	}
}
/*******************************************************************/
tree *PROGmerge(module *mm,tree *processes)
{
	int lc=0;
	tree *var,*init,*trans,*specs;
	tree *t0,*t1,*t2;
	tree *p1=processes;
	char ni[64];


	mm->prog->right->right->right=
		specmergeg1(mm->prog->right->right->right);
	if (mm->next==NULL) { 
		t0=treecopy(mm->prog);
		p1->left=t0->right->right->left;
		p1->right=treecompose(";",NULL,NULL);
		return t0;
	}
	var=treecopy(mm->prog->left);
	init=treecopy(mm->prog->right->left);
	trans=NULL;
	specs=treecopy(mm->prog->right->right->right);
	t0=mm->prog->right->right->left;
	while ((t1=listgetone(t0,lc++))!=NULL) {
		t2=treecopyreplace(t1,mm->next);

                sprintf(ni,"%3i",lc-1);
                if (ni[0]==' ') ni[0]='0'; if (ni[1]==' ') ni[1]='0';
 		replacepid1(t2,ni);

		var=listmerge(var,t2->left);
		init=treecompose("&",init,t2->right->left);
		trans=listmerge(trans,t2->right->right->left);
/*
		p1->left=(tree *)listlen(t2->right->right->left);
*/
		p1->left=(t2->right->right->left);
		p1->right=treecompose(";",NULL,NULL); p1=p1->right;
/*
		specs=listmerge(specs,t2->right->right->right,lc-1);
*/
		specs=specmerge(specs,t2->right->right->right,lc-1);
	}
	if (specs->right==NULL) specs->right=
		treecompose("=",treenum(1),treenum(1));

	return PROGcompose(var,init,trans,specs);
}
/*******************************************************************/
tree *treecopyreplace(tree *spec,module *ms)
{
	char *s0;
	tree *prog;
	tree *pname=spec->left;
	tree *mname=spec->right->left;
	tree *t1,*t2;


	while (ms!=NULL) {
	  if (mname==NULL) break;
	  if (strcmp(ms->parameters->left->node,mname->node)) ms=ms->next;
	  else break;
	}
	if (ms==NULL) error("module");

	prog=treecopy(ms->prog);

	if (prog->left==NULL) { 
		t1=ms->parameters->right; t2=spec->right->right;
	} else {
		t1=treecompose(":",NULL,NULL); t2=treecompose(":",NULL,NULL);
		constructt1t2(pname->node,prog->left,t1,t2);
		t1=treecompose("|",t1,ms->parameters->right);
		t2=treecompose("|",t2,spec->right->right);
	}
	if (t1!=NULL) { treereplacecleanname(t1); treereplacecleanname(t2); }
	treereplacenames(prog,t1,t2);

	return prog;
}
void constructt1t2(char *pname,tree *vars,tree *t1,tree *t2)
{
	tree *a1,*a2,*a3,*a4;
	char *p;

	if (vars->node[0]==':') {
		strcpy(t1->node,vars->left->node);
		p=t1->node; 
		while (*p&&*p!='[') p++; if (*p=='[') { p++; *p++=']'; *p=0; }
		sprintf(t2->node,"%s.%s",pname,t1->node); return;
	}
	a1=treecompose(":",NULL,NULL); a2=treecompose(":",NULL,NULL);
	a3=treecompose(":",NULL,NULL); a4=treecompose(":",NULL,NULL);
	constructt1t2(pname,vars->left,a1,a2);
	constructt1t2(pname,vars->right,a3,a4);
	treereplace(t1,treecompose("|",a1,a3));
	treereplace(t2,treecompose("|",a2,a4));
}
void treereplacenames(tree *prog,tree *t1,tree *t2)
{
	int i;
	if (prog==NULL) return;
	treereplacenames(prog->left,t1,t2);
	treereplacenames(prog->right,t1,t2);
	if (isvarconst(prog->node)) {
		if (i=arrayindex(prog->node)) {
			treereplacename(prog->node,t1,t2,1);
			i=arrayindex(prog->node);
			treereplacenamearray(prog->node,i,t1,t2);
		} else treereplacename(prog->node,t1,t2,0);
	}
}
int treereplacename(char *node,tree *t1,tree *t2,int fl)
{
	if (t1==NULL) return;
	if (isvarconst(t1->node)) {
		if (fl==0) return stringreplacestring(node,t1->node,t2->node);
		return stringreplacestringfixlength(node,t1->node,t2->node);
	}
	if (!treereplacename(node,t1->left,t2->left,fl)) 
	return treereplacename(node,t1->right,t2->right,fl); return 0;
}
void treereplacecleanname(tree *t1)
{
	char *p1;
	if (t1==NULL) return;
	if (t1->node[0]=='|') {
		treereplacecleanname(t1->left);
		treereplacecleanname(t1->right); return;
	}
	p1=t1->node; while (*p1&&*p1!='[') p1++;
	if (*p1) *(p1+1)=0;
}
void treereplacenamearray(char *node,int i,tree *t1,tree *t2)
{
	char *s0;
	char tmp[MAXVARLEN],tmp1[MAXVARLEN],tmp2[MAXVARLEN];
	tree *t0;
	int j;

	strcpy(tmp,node); 
	s0=tmp+i; while (*s0!=']') s0++; *s0=0;

	strcpy(tmp2,tmp); 
	t0=string2tree(tmp+i);
	treereplacenames(t0,t1,t2);
	treewrites(t0,tmp1,0);
	stringreplacestringfixlength(node+i,tmp2+i,tmp1);
	j=i+strlen(tmp1)+1;
	if (*(node+j)=='[') treereplacenamearray(node,j+1,t1,t2);
}
/*******************************************************************/
tree *specmergeg1(tree *s1)
{
	tree *s1l,*s1r;
	if (s1==NULL) { 
		s1l=NULL; 
		s1r=NULL; 
	} else if (s1->node[0]=='#') {
                s1l=treecopy(s1->left->right);
                s1r=s1->right; treefree(s1->left); free(s1);
        } else if (s1->node[0]==';'&&s1->left->node[0]=='=') {
		s1l=s1->right;
		s1r=NULL; treefree(s1->left); free(s1);
	} else {
		s1l=NULL;
		s1r=s1;
	}
	return treecompose("#",s1l,s1r);
}
tree *specmerge(tree *s1,tree *s2,int n)
{
	tree *t1;
	char id[64];
	int lc=0;

	s2=specmergeg1(s2); 
	reprunning2(s2->left,n);
/*
	if (s2->left!=NULL) {
	 for (t1=s2->left;t1->node[0]==';';t1=t1->right) reprunning(t1->left,n);
	 reprunning(t1,n); 
	}
*/
	t1=treecompose("#",
		listmerge(s1->left,s2->left), listmerge(s1->right,s2->right));
	free(s1); free(s2);
 
	return t1;
}
void reprunning(tree *t1,int n)
{
	char id[64];

	if (strcmp(t1->node,"running")==0) {
		sprintf(id,"%i",n);
		treereplace(t1,
		treecompose("=",treecompose(PIDs,NULL,NULL),
		treecompose(id,NULL,NULL)));
	}
}
void reprunning2(tree *t1,int n)
{
	char id[64];

	if (t1==NULL) return;
	if (strcmp(t1->node,"running")==0) {
		if (n>=0) {
			sprintf(id,"%i",n);
			treereplace(t1,
			treecompose("=",treecompose(PIDs,NULL,NULL),
			treecompose(id,NULL,NULL)));
		} else {
			treereplace(t1,treecompose("=",treenum(1),treenum(1)));
		}
	}
	reprunning2(t1->left,n); reprunning2(t1->right,n);
}
/*******************************************************************/
#define separator(c)    (!(isalnum(c)||c=='_'))
/*******************************************************************/
void replacepid1(tree *ll,char *ni)
{
        char *node;
	int i,k;

        if (ll==NULL) return;
        replacepid1(ll->left,ni); replacepid1(ll->right,ni);
        if (strcmp(ll->node,PIDkw)==0) { strcpy(ll->node,ni); return; }
        for (node=ll->node+1,i=strlen(node),k=3;k<=i;node++,k++) {
        	if (strncmp(node,PIDkw,3)==0&&separator(node[3]))
        	{ memcpy(node,ni,3*sizeof(char)); k+=3; node+=3; }
	}
}
/*******************************************************************/

/*******************************************************************/
#define isconstant(node)   	(node[0]=='C')
/********************************************************************
* constants are of the form Cs0wait,Cs1think                        *
/*******************************************************************/
#define addnextvc(x,t)		newnode[0]=x; \
				strcpy(newnode+1,t->node); \
				strcpy(t->node,newnode); \
/*******************************************************************/
void PROGaddnext_assign(tree *t) 
{
	char newnode[MAXVARLEN];

	if (t==NULL) return;
	if (t->node[0]=='='&&!isdigit(t->left->node[0])) { 
		addnextvc('O',t->left); return; 
	} 
	PROGaddnext_assign(t->left); PROGaddnext_assign(t->right); 
}
void PROGaddnext(tree *t) 
{
	if (t==NULL) return;
	if (t->node[0]==':') { PROGaddnext_assign(t->right); return; }
	PROGaddnext(t->left); PROGaddnext(t->right);
}
/*******************************************************************/
void PROGaddvc(tree *t,tree *v) 
{
	char x;
	char newnode[MAXVARLEN];

	if (t==NULL) return;
	if (isvarconst(t->node)) { 
	  x=((treesearchvarlist(t->node,arrayindex(t->node),v)==NULL)?'C':'V');
	  addnextvc(x,t); 
	  return; 
	}
	PROGaddvc(t->left,v); PROGaddvc(t->right,v); return;
}
/*******************************************************************/
void PROGreplaceconstants(tree *t)
{
        char *p,*q;

        if (t==NULL) { return; }
        if (isconstant(t->node)) {
                p=q=t->node;
                while (!isdigit(*p)) p++;
                while (isdigit(*p)) *q++=*p++; *q=0; return;
        }
        PROGreplaceconstants(t->left); PROGreplaceconstants(t->right); return;
}
/*******************************************************************/
void PROGaddnextv(tree *prog,int addnext) 
{
	if (addnext) PROGaddnext(prog->right->right->left); 
	PROGaddvc(prog->right,prog->left); 
	PROGreplaceconstants(prog->right);
}
/*******************************************************************/
