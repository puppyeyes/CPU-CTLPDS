#include "trop.h"

/*******************************************************************/
#define addxvc(x,t)	newnode[0]=x; \
			strcpy(newnode+1,t->node); \
			strcpy(t->node,newnode); \
/*******************************************************************/
tree *getvtree(char *node,tree *t)
{
        if (t->node[0]!=';') {
                if (mystrcmp(node,t->left->node)==0) return t->left;
                return NULL;
        }
	if (t->left->node[0]==';') error("vlist");
        if (mystrcmp(node,t->left->left->node)==0) return t->left->left;
        return getvtree(node,t->right);
}
/*******************************************************************/
tree *getvtree_a(char *node,tree *t)
{
	tree *t1,*t2;
	if (t==NULL) return NULL;
        if (t->node[0]!='&') {
                if (mystrcmp(node,t->left->node)==0) return treecopy(t);
                return NULL;
        }
        t1=getvtree_a(node,t->left); t2=getvtree_a(node,t->right);

	if (t1==NULL) return t2; 
	if (t2==NULL) return t1; 
	return treecompose("&",t1,t2);
}
/*******************************************************************/
void treeaddx_local(tree *t) 
{
	char newnode[80];

	if (t==NULL) return;
	if (t->node[0]=='='&&!isdigit(t->left->node[0])) { 
		addxvc('X',t->left); return; 
	} 
	treeaddx_local(t->left); treeaddx_local(t->right); 
}
void treeaddx(tree *t) 
{
	if (t==NULL) return;
	if (t->node[0]==':') { treeaddx_local(t->right); return; }
	treeaddx(t->left); treeaddx(t->right);
}
/*******************************************************************/
void treeaddvc(tree *t,tree *v) 
{
	char x;
	char newnode[MAXVARLEN];

	if (t==NULL) return;
	if (isvarconst(t->node)) { 
		if (getvtree(t->node,v)==NULL) x='C'; else x='V'; 
		addxvc(x,t); return; 
	}
	treeaddvc(t->left,v); treeaddvc(t->right,v); return;
}
/*******************************************************************/

/*******************************************************************/
#define treeone treecompose("1",NULL,NULL)
/*******************************************************************/
tree *checkprog(tree *t)
{
	tree *t0;

	t0=t->right->right;
	if (t0->node[0]!='#') t->right->right=treecompose("#",t0,NULL);
	t0=t->right->left;
	if (t0==NULL) t->right->left=treecompose("=",treeone,treeone);
	return t;
}
/*******************************************************************/
module *string2module(char *s)
{
	module *mm,*mm0;
	char *p1,*p2,*p0;
	int last=0;

	p1=s; if (strncmp(p1,"MAIN",4)==0) p1+=4;
	
	p2=p1; 
	while (1) {
		while (*p1&&*p1!='M') p1++;
		if (*p1==0) last=1;
		if (last||(strncmp(p1,"MODULE",6)==0)) { 
			*p1=0; 
			mm=(module *)malloc(sizeof(module));
			mm->parameters=NULL; mm->next=NULL; 
			if (last) { 
				mm->prog=checkprog(string2tree(p2)); 
				return mm; 
			}
			p0=p2;
			while (*p0!='#') p0++; p0++;
			while (*p0!='#') p0++; p0++;
			stringreplacechar(p0,'(',':');
			stringreplacechar(p0,')',' ');
			stringreplacechar(p0,',','|');
			stringcompress_a(p2,' ',' '); 
			mm->prog=checkprog(string2tree(p2)); 
			p1+=6; p2=p1; 
			break;
		} 
	}
	mm0=mm; 
	while (1) {
		while (*p1&&*p1!='M') p1++;
		if (*p1==0) last=1;
		if (last||(strncmp(p1,"MODULE",6)==0)) { 
			*p1=0; 
			mm->next=(module *)malloc(sizeof(module));
			mm=mm->next;
			p0=p2; while (*p0&&*p0!=')') p0++; *p0=0;
			stringreplacechar(p2,'(',':');
			stringreplacechar(p2,')',';');
			stringreplacechar(p2,',','|');
			mm->parameters=string2tree(p2);
			mm->prog=checkprog(string2tree(p0+1)); 
			mm->next=NULL;
			if (last) return mm0; 
			p1+=6; p2=p1; 
		} 
	}
}
/*******************************************************************/
void *stringreplacechar(char *s,char a,char b)
{
	while (*s&&*s!='#') if (*s==a) *s++=b; else s++;
}
char *stringreplacestring(char *s,char *a,char *b)
{
	if (strcmp(s,a)==0) strcpy(s,b);  return s;
}
char *stringreplacestringfixlength(char *s,char *a,char *b)
{
	int la=strlen(a);
	int lb=strlen(b);
	char tmp[MAXVARLEN];

	if (strncmp(s,a,la)==0) { 
		strcpy(tmp+lb,s+la); strncpy(tmp,b,lb); 
		strcpy(s,tmp);
	}
	return s;
}
/*******************************************************************/
tree *merge(module *mm)
{
	tree *var,*init,*trans,*specs;
	tree *t0,*t1,*t2;


	if (mm->next==NULL) return treecopy(mm->prog);
	var=treecopy(mm->prog->left);
	init=treecopy(mm->prog->right->left);
	trans=NULL;
	specs=treecopy(mm->prog->right->right->right);
	t0=mm->prog->right->right->left;
	while (1) {
		t1=listgete(t0); 
		t2=treecopyreplace(t1,mm->next);
		var=listcompose(var,t2->left);
		init=treecompose("&",init,t2->right->left);
		trans=listcompose(trans,t2->right->right->left);
		specs=listcompose(specs,t2->right->right->right);
		if (t0==t1) break; t0=t0->right;
	}
	if (specs==NULL) specs=treecompose("=",treeone,treeone);

	return progcompose(var,init,trans,specs);
}
/*******************************************************************/
void modulewrite(module *mm,FILE *f)
{
	fprintf(f,"MAIN\n"); 
	treewrite(mm->prog,f); fprintf(f,"\n"); mm=mm->next;
	while (mm!=NULL) {
		fprintf(f,"MODULE ("); treewrite(mm->parameters,f); 
		fprintf(f,")\n"); 
		treewrite(mm->prog,f); fprintf(f,"\n"); 
		mm=mm->next;
	}
}
/*******************************************************************/
tree *treecopyreplace(tree *spec,module *ms)
{
	char *s0;
	tree *prog;
	tree *pname=spec->left;
	tree *mname=spec->right->left;
	tree *t1,*t2;

	while (ms!=NULL) 
	if (strcmp(ms->parameters->left->node,mname->node)) ms=ms->next;
	else break;
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
		if (i=isarray(prog->node)) {
			treereplacename(prog->node,t1,t2,1);
			i=isarray(prog->node);
			treereplacenamearray(prog->node,i,t1,t2);
		} else treereplacename(prog->node,t1,t2,0);
	}
}
void treereplacename(char *node,tree *t1,tree *t2,int fl)
{
	if (t1==NULL) return;
	if (isvarconst(t1->node)) {
		if (fl) stringreplacestringfixlength(node,t1->node,t2->node);
		else stringreplacestring(node,t1->node,t2->node);
		return;
	}
	treereplacename(node,t1->left,t2->left,fl);
	treereplacename(node,t1->right,t2->right,fl); return;
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
void cleanstring(char *s)
{
	stringcompress_a(s,' ',' ');
	stringed(s,"STEP:","s_p:");
	stringed(s,"STEP=","s_p="); stringed(s,"STEP","s_p=");
	stringed(s,";goto","&"); stringed(s,":goto",":");
}
void stringcompress_a(char *s,char a,char b)
{
	char *t=s;
	char *p;

	for (;1;) {
		if (*s==9||*s==10||*s==13||*s==a||*s==b) s++; 
		else if (*s=='/'&&*(s+1)=='/') while (*s&&*s!=13&&*s!=10) s++;
		else {
			if (*s=='!') {
				if (*(t-1)==']'&&*(t-2)=='.') *s=CHANW;
			}
			*t++=*s++;
		}
		if (*s==0) { *t=0; break; }
	}
}
char *stringed(char *s,char *a,char *b)
{
	int i=strlen(a);
	int j=strlen(b);
	char *t,*t0;

	for (t=t0=s;1;) {
		while (*s&&*s!=*a) *t++=*s++;
		if (*s==0) { *t=0; break; }
		if (strncmp(s,a,i)==0) { strncpy(t,b,j); t+=j; s+=i; }
		else { *t++=*s++; }
	}
	return t0;
}


/*******************************************************************/
LOCAL char OP5[8]="#";
LOCAL char OP4[8]=";";
LOCAL char OP3[8]="UR:";
LOCAL char OP2[8]="&|";
LOCAL char OP0[8]=">=<?@";
LOCAL char OP9[8]="+-";
LOCAL char OP8[8]="*";
LOCAL char OP1[8]="AEFGX!";
/*******************************************************************/
tree *string2tree_local(char *s,char *op)
{
	char *p=s;
	int l=0;
	tree *t,*t0;

	while (*p&&(l||!local_isin(*p,op))) { 
		if (*p=='('||*p=='[') { 
			l++; 
			if (l<0) error("Syntax");
		}
		else if (*p==')'||*p==']') l--; 
		p++; 
	}
	if (local_isin(*p,op)) { 
		t=(tree *)(malloc(sizeof(tree)));
		t->node[0]=*p; 
		t->node[1]=0; *p=0;
		t->left=string2tree(s); 
		t->right=string2tree(p+1); 
		if (t->right==NULL) { t0=t; t=t0->left; free(t0); } return t;
	}
	return NULL;
}
tree *string2tree(char *s)
{
	char *p=s;
	int l=0;
	tree *t;

	if (*p==0) return NULL;

	if ((t=string2tree_local(s,OP5))!=NULL) return t; 
	if ((t=string2tree_local(s,OP4))!=NULL) return t; 
	if ((t=string2tree_local(s,OP3))!=NULL) return t; 
	if ((t=string2tree_local(s,OP2))!=NULL) return t; 

	if ((t=string2tree_local(s,OP1))!=NULL) return t; 

	if ((t=string2tree_local(s,OP0))!=NULL) return t; 
	if ((t=string2tree_local(s,OP9))!=NULL) return t; 
	if ((t=string2tree_local(s,OP8))!=NULL) return t; 

	if (*p=='(') return string2tree(p+1);
	t=(tree *)(malloc(sizeof(tree)));
	while (*p&&*p!=')') t->node[l++]=*p++; t->node[l]=0;
	t->left=NULL; t->right=NULL; return t;
}
/*******************************************************************/
void stringwrite(char *s,FILE *f)
{
	fwrite(s,strlen(s),1,f); 
}
/*******************************************************************/

/*******************************************************************/
int local_isin(char a,char* s)
{
	while (*s&&*s!=a) s++; return *s;
}
/*******************************************************************/
void treefree(tree *t) 
{
	if (t==NULL) return;
	treefree(t->left); treefree(t->right); free(t);
}
void treereplace(tree *t0,tree *t1) 
{
	treefree(t0->left); treefree(t0->right); 
	t0->left=t1->left; t0->right=t1->right; strcpy(t0->node,t1->node); 
	free(t1);
}
tree *treecompose(char *s,tree *t0,tree *t1) 
{
	tree *t;

	t=(tree *)(malloc(sizeof(tree)));
	strcpy(t->node,s); t->left=t0; t->right=t1; 
	return t;
}
tree *listcompose(tree *t0,tree *t1) 
{
	tree *a0,*a1;

	if (t0==NULL) return t1; if (t1==NULL) return t0;
	a0=t0; while (a0->node[0]==';') a0=a0->right;
	a1=treecompose(";",treecopy(a0),t1); treereplace(a0,a1);
	return t0;
}
tree *listgete(tree *t0) 
{
	if (t0->node[0]==';') return t0->left; else return t0;
}
tree *progcompose(tree *t0,tree *t1,tree *t2,tree *t3) 
{
	return treecompose("#",t0,treecompose("#",t1,treecompose("#",t2,t3)));
}
tree *treenum(int n)
{
	tree *t;

	t=(tree *)(malloc(sizeof(tree)));
	sprintf(t->node,"%i",n); t->left=NULL; t->right=NULL; 
	return t;
}
tree *treecopy(tree *t) 
{
	tree *t0;

	if (t==NULL) return NULL;
	t0=(tree *)(malloc(sizeof(tree)));
	strcpy(t0->node,t->node);
	t0->left=treecopy(t->left); t0->right=treecopy(t->right); return t0;
}
/*******************************************************************/
void treewrite(tree *t,FILE *f)
{
	if (t==NULL) return;
	if (t->left==NULL) { fprintf(f,"%s",t->node); treewrite(t->right,f); }
	else {
		fprintf(f,"("); treewrite(t->left,f); 
		fprintf(f,"%s",t->node); 
		if (t->node[0]==';'||t->node[0]=='#') fprintf(f,"\n"); 
		treewrite(t->right,f); fprintf(f,")"); 
	}
}
int treewrites(tree *t,char *f,int i)
{
	if (t==NULL) return i;
	if (t->left==NULL) { 
		sprintf(f+i,"%s",t->node); i+=strlen(t->node);
		i=treewrites(t->right,f,i); 
	} else {
		sprintf(f+i,"("); i++;
		i=treewrites(t->left,f,i); 
		sprintf(f+i,"%s",t->node); i+=strlen(t->node);
		i=treewrites(t->right,f,i); 
		sprintf(f+i,")"); i++;
	}
	return i;
}
/*******************************************************************/
tree *cleanarray(tree *t)
{
	char *p1,*p2;
	if (isarray(t->node)) {
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
tree *treetraverse(tree *t,int f0(),tree *f1(),tree *f2())
{
	if (f0!=NULL&&f0(t)) return f1(t); else if (t==NULL) return NULL; 
	t->left=treetraverse(t->left,f0,f1,f2); 
	t->right=treetraverse(t->right,f0,f1,f2); 
	if (f2!=NULL) return f2(t); else return t;
}
/*******************************************************************/
tree *treeconst(int positive)
{
	if (positive) return treecompose(TRUEs,NULL,NULL);
	return treecompose(FALSEs,NULL,NULL);
}
tree *treeconstaa(int positive)
{
	tree *t=(tree *)malloc(sizeof(tree));
	tree *t0=(tree *)malloc(sizeof(tree));
	tree *t1=(tree *)malloc(sizeof(tree));

	strcpy(t0->node,"1x"); t0->left=NULL; t0->right=NULL;
	strcpy(t1->node,"!");  t1->left=NULL; t1->right=treecopy(t0);
	if (positive) strcpy(t->node,"|"); else strcpy(t->node,"&"); 
	t->left=t0; t->right=t1;
	return t;
}
tree *treeconsttest(tree *t)
{
	if (t->node[0]==TRUE) return treeconstaa(1);
	if (t->node[0]==FALSE) return treeconstaa(0);
	return t; 
}
/*******************************************************************/
void treet0(tree *t,int f0())
{
	if (f0(t)) return; 
	treet0(t->left,f0); treet0(t->right,f0); 
}
/*******************************************************************/
void treet1(tree *t,void f1())
{
	if (t==NULL) return; 
	treet1(t->left,f1); treet1(t->right,f1); 
	f1(t);
}
/*******************************************************************/
void replacesemiand(tree *t)
{
	if (t->node[0]==';') t->node[0]='&'; 
}
/*******************************************************************/

/*******************************************************************/
int isarray(char *node) 
{
	int i=1;
	while (*node&&*node!='[') { i++; node++; }
	if (*node) return i; else return 0; 
}
/*******************************************************************/
int mystrcmp(char *a,char *b)
{
	int i;
	if (i=isarray(a)) return strncmp(a,b,i); else return strcmp(a,b);
}
/*******************************************************************/
tree *treeadddead_construct(tree *t)
{
	tree *t0;
	if (t==NULL) return NULL;
	if (t->node[0]==';') { 
		if (t->right==NULL) return treeadddead_construct(t->left);
		t0=(tree *)malloc(sizeof(tree)); strcpy(t0->node,"|");
		t0->left=treeadddead_construct(t->left);
		t0->right=treeadddead_construct(t->right);
		return t0;
	}
	return treecopy(t->left);
}
tree *treeadddead(tree *t)
{
	tree *t0,*t1,*t2;
	t0=(tree *)malloc(sizeof(tree)); strcpy(t0->node,"!");
	t0->left=NULL; t0->right=treeadddead_construct(t);
	t1=(tree *)malloc(sizeof(tree)); strcpy(t1->node,":");
	t1->left=t0; t1->right=NULL;
	t2=(tree *)malloc(sizeof(tree)); strcpy(t2->node,";");
	t2->left=t1; t2->right=t;
	return t2;
}
/*******************************************************************/
tree *treeaddeq_constructeq(tree *v)
{
	tree *t0,*t1,*t2;

	if (v==NULL) return NULL;
	if (v->node[0]==';') { 
		if (v->right==NULL) return treeaddeq_constructeq(v->left);
		t0=(tree *)malloc(sizeof(tree)); strcpy(t0->node,"&");
		t0->left=treeaddeq_constructeq(v->left);
		t0->right=treeaddeq_constructeq(v->right);
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
void *treeaddeq_modify(tree *t,tree *v)
{
	tree *t1;

	if (t==NULL) return;
	if (t->node[0]=='&') { 
		treeaddeq_modify(t->left,v);
		treeaddeq_modify(t->right,v); return;
	}
	if (t->node[0]!='=') error("modify"); 

	t1=getvtree_a(t->left->node,v);
	if (t1!=NULL) treereplace(t,t1);
}
void *treeaddeq(tree *t,tree *v)
{
	tree *t0,*t1,*t2;

	if (t==NULL) return;
	if (t->node[0]==';') { 
		treeaddeq(t->left,v);
		treeaddeq(t->right,v); return;
	}
	if (t->node[0]==':') { 
		t0=treeaddeq_constructeq(v); 
		treeaddeq_modify(t0,t->right);
		treefree(t->right); t->right=t0;
	}
}
/*******************************************************************/
void cleancolon(tree *t)
{
	tree *t1,*t2;
	if (t->node[0]==':') {
		cleancolon(t->right);
        	if (t->right->node[0]==':') {
			t1=treecompose("&",treecopy(t->left),
				treecopy(t->right->left));
			t2=treecompose(":",t1,treecopy(t->right->right));
			treereplace(t,t2);
		}
	}
}
/*******************************************************************/
void listtraverse(tree *ts,void f())
{
        tree *tr2;
	if (ts==NULL) return;
        while (1) {
                tr2=listgete(ts); f(tr2);
                if (tr2==ts) break; ts=ts->right;
        }
}


/*******************************************************************/
#define neg(a) 	treecompose("!",NULL,a)
/*******************************************************************/
int vlistcons_count(tree *v)
{
        if (v==NULL) return 1;
        return vlistcons_count(v->left)+vlistcons_count(v->right);
}
int vlistcons(tree *v,int p)
{
        tree *tvar,*tdomain;
        int i,j;
	char low[MAXVARLEN],high[MAXVARLEN];
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
	} else {
		i=vlistcons_count(tdomain)/2;
	} 

	i=i-1; j=1; while ((i=i/2)>0) j++;
	sprintf(low,"%i",p+1);
	sprintf(high,"%i",p+j);
	tvar->left=treecompose(low,NULL,NULL);
	tvar->right=treecompose(high,NULL,NULL);

        if (v->node[0]==';') return vlistcons(v->right,p+j); 
	return  p+j;
}
/*******************************************************************/
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
	} else { i=vlistcons_count(tdomain)/2; } 
	t=neg(treecompose(">",treecopy(tvar),treenum(i-1)));

        if (v->node[0]==';') return treecompose("&",t,constr(v->right)); 
	return  t;
}
/*******************************************************************/
