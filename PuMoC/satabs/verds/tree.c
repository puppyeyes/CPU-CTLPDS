#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/times.h>
#include <time.h>
#include "tree.h"
#include "tree.h"

/*******************************************************************/
int listlength(tree *t1)
{
        int i=1;
        while (t1->node[0]==';') { t1=t1->right; i++; }
        return i;
}
/*******************************************************************/
tree *listget1n(tree *t,int n)
{
        int lc=0;
        tree *spec=NULL;
        while (lc<n) spec=listgetone(t,lc++); return spec;
}
/*******************************************************************/
tree *listgetone(tree *t,int next) 
{
	tree *t1;
	static tree *t0;

	if (next==0) t0=t;

	if (t0==NULL) return NULL;
        if (t0->node[0]==';') 
	{ t1=t0->left; t0=t0->right; } else { t1=t0; t0=NULL; } return t1;
}
tree *listmerge(tree *t0,tree *t1)
{
        tree *a0,*a1;

        if (t0==NULL) return t1; if (t1==NULL) return t0;
        a0=t0; while (a0->node[0]==';') a0=a0->right;
        a1=treecompose(";",treecopy(a0),t1); treereplace(a0,a1);
        return t0;
}
/*******************************************************************/
tree *listcombone(tree *f(),tree *l1,tree *l2)
{
        tree *l0;
        if (l1->node[0]!=';') return f(l1,l2,NULL);
        l0=listcombone(f,l1->right,l2->right);
        return f(l1->left,l2->left,l0);
}
tree *listcombine(tree *f(),tree *x1,tree *x2)
{
        int a1=listlength(x1);
        int a2=listlength(x2);
        tree *r0,*t1,*t2;

        t1=treecopy(x1); t2=treecopy(x2);
        for (;a2>a1;a1++) t1=treecompose(";",treecompose(FALSEs,NULL,NULL),t1);
        for (;a1>a2;a2++) t2=treecompose(";",treecompose(FALSEs,NULL,NULL),t2);

        r0=listcombone(f,t1,t2); treefree(t1); treefree(t2);
	return r0;
}
tree *listcombinedist(tree *f(),tree *x1,tree *x2)
{
        int a1=listlength(x1);
        int a2=listlength(x2);
        tree *r0,*r1,*t1,*t2;

        t1=treecopy(x1); t2=treecopy(x2);
        for (;a2>a1;a1++) t1=treecompose(";",treecopy(x1),t1);
        for (;a1>a2;a2++) t2=treecompose(";",treecopy(x2),t2);

        r0=listcombone(f,t1,t2); treefree(t1); treefree(t2);
	return r0;
}
/*******************************************************************/
void listtraverse(tree *ts,void f())
{
	int lc=0;
        tree *tr2;
        while ((tr2=listgetone(ts,lc++))!=NULL) { f(tr2); }
}
/*******************************************************************/
tree *listaddstring(char *s,tree *v0)
{
	tree *v1;
	if (v0==NULL) return treecompose(s,NULL,NULL);
	v1=v0;
        while (v1->node[0]==';') 
	if (strcmp(v1->left->node,s)==0) return v0; else v1=v1->right;
        if (strcmp(v1->node,s)==0) return v0;
	v1->left=treecopy(v1); v1->right=treecompose(s,NULL,NULL);
	v1->node[0]=';'; v1->node[1]=0;
	return v0;
}
/*******************************************************************/
int listrmstring(char *s,tree *v0)
{
	tree *v1;
	if (v0==NULL) return 0;
        for (v1=v0;v1->node[0]==';';v1=v1->right) 
	if (strcmp(v1->left->node,s)==0) { 
		treereplace(v1,treecopy(v1->right)); return 1;
	} 
        if (strcmp(v1->node,s)==0) { 
		treereplace(v1,treecopy(v1->right)); return 1; 
	}
	return 0;
}
/*******************************************************************/
/*
int listsearchstring(char *s,tree *v0)
{
	tree *v1;
	if (v0==NULL) return 0;
        for (v1=v0;v1->node[0]==';';v1=v1->right) 
	if (strcmp(v1->left->node,s)==0) { return 1; } 
        if (strcmp(v1->node,s)==0) { return 1; }
	return 0;
}
/*******************************************************************/

/*******************************************************************/
#define SOLmsg0	"no solvers are specified"
#define SOLmsg1	"not found, may need to add a path\n"
#define SOLmsg2	"not found\n"
/*******************************************************************/
#define apply0(s0,ss) fprintf(stdout,"APPLY:      %s-solver '%s'\n",s0,ss)
/*******************************************************************/
#define warning0() \
       	fprintf(stdout,"WARNING:    %s\n",SOLmsg0); \
       	fprintf(stdout,"WARNING:    an internal solver is used\n"); \
/*******************************************************************/
#define warning5(s0,ss,ssb) \
    	fprintf(stdout,"WARNING:    %s-solver '%s' ",s0,ss); \
    	if (ssb) fprintf(stdout,SOLmsg2); else fprintf(stdout,SOLmsg1); \
/*******************************************************************/
#define warning1(s0,ss,ssb) \
	warning5(s0,ss,ssb); \
       	fprintf(stdout,"WARNING:    an internal solver is used\n"); \
/*******************************************************************/
#define QBF	1
#define SATQBF	0
/*******************************************************************/
int SOLVEcheck(char *s1,char *s2,int qbf,int bmc)
{
        FILE *g;
	int c=0;
	int s1a=0,s2a=0;
	int s1b,s2b;
	int i;

        if (*s1=='_'||*s2=='_') return (*s2=='_'); if (bmc==4) return QBF;

        if (bmc==3) bmc=bmc-1-qbf;
        if (bmc==1) *s1=0; else if (bmc==2) *s2=0;
        if (bmc==2&&qbf==1) {
                fprintf(stdout,
                "ERROR:      invalid BMC option for a CTL-formula\n");
                exit(0);
        }

        if (*s2!=0) { if ((g=fopen(s2,"r"))!=NULL) { fclose(g); s2a=1; } }
        if (*s1!=0) { if ((g=fopen(s1,"r"))!=NULL) { fclose(g); s1a=1; } }

        if (*s2!=0&&s2a==0)
        for (i=0;s2[i];i++) if (s2[i]=='\\'||s2[i]=='/') break; s2b=s2[i];

        if (*s1!=0&&s1a==0)
        for (i=0;s1[i];i++) if (s1[i]=='\\'||s1[i]=='/') break; s1b=s1[i];

        switch (bmc) {
            case 1:
                    if (*s2==0) { warning0(); return QBF; }
                    else if (s2a) { apply0("qbf",s2); return QBF; }
                    else { warning1("qbf",s2,s2b); *s2=0; return QBF; }
            case 2:
                    if (*s1==0) { warning0(); return SATQBF; }
                    else if (s1a) { apply0("sat",s1); return SATQBF; }
                    else { warning1("sat",s1,s1b); *s1=0; return SATQBF; }
	}
}
/*******************************************************************/
int SOLVEapply(char *solver,model *mo,int bv)
{
	FILE *g;
	int r;

	if (*solver==0) r=SATapply(mo); 
	else {
       		g=fopen(tmp0file,"w"); cnfmodelwrite(mo,g); fclose(g);
		r=SOLVEapply_0(solver); if (r==-1) return 0;
	}
	if (bv&&r==0) return 1;
	if (!bv&&!r==0) return -1;
	return 0;
}
/*******************************************************************/
int SOLVEapply_0(char *solver)
{
	char *ll,*l0;
	char cmd[256];
	int r;

	sprintf(cmd,"%s %s %s > %s",solver,tmp0file,tmp2file,tmp1file);
	system(cmd);
	ll=file2string(tmp1file);
	l0=stringsearch(ll,"UNSAT","SATISFIABLE"); 
	switch (*l0) {
		case 'S': r=1; break;
		case 'U': r=0; break;
		case  0:  r=-1; break;
	}
	free(ll); return r;
}
/*******************************************************************/

/*******************************************************************/
tree *eqstate(int pa,int i,int pb,int j,int k)
{
        tree *t1=(tree *)malloc(sizeof(tree));
        tree *t2=(tree *)malloc(sizeof(tree));

        if (k==0) return treeconst(1); /*for satbmc k=0*/
	if (k<0) k=0; /*for qbfbmc, org k=0, zwh*/

        sprintf(t1->node,"%i",(pa-1)*(k+1)+i); t1->left=NULL; t1->right=NULL;
        sprintf(t2->node,"%i",(pb-1)*(k+1)+j); t2->left=NULL; t2->right=NULL;
        return treecompose("=",t1,t2);
}
void eqstateconstruct(tree *t,int bn)
{
	int a,b,i;
	tree *t0,*t1;

	if (t==NULL) return;
	if (t->node[0]=='=') {
		a=atoi(t->left->node); b=atoi(t->right->node);
		i=1; t0=leq(treenum(a*bn+i),treenum(b*bn+i)); 
		while (i<bn) {
			i++; t1=leq(treenum(a*bn+i),treenum(b*bn+i)); 
			t0=treecompose("&",t0,t1);
		}
		treereplace(t,t0); return;
	}
	else if (t->node[0]=='B') {
		a=atoi(t->left->node); b=atoi(t->right->node);
		i=1; t0=xor(treenum(a*bn+i),treenum(b*bn+i)); 
		while (i<bn) {
			i++; t1=xor(treenum(a*bn+i),treenum(b*bn+i)); 
			t0=treecompose("|",t0,t1);
		}
		treereplace(t,t0); return;
	} 
	eqstateconstruct(t->left,bn); eqstateconstruct(t->right,bn); 
}
/*******************************************************************/

/*******************************************************************/
LOCAL char OP5[9]="#";
LOCAL char OP4[9]=";";
LOCAL char OP3[9]="UR:";
LOCAL char OP2[9]="&|";
LOCAL char OP0[9]=">=<?@{}B";
LOCAL char OP9[9]="+-";
LOCAL char OP8[9]="*/%";
LOCAL char OP1[9]="AEFGX!";
/*******************************************************************/
tree *string2tree_local(char *s,char *op)
{
	char *p=s;
	int l=0;
	tree *t,*t0;

	while (*p&&(l||!charsearch(*p,op))) { 
		if (*p=='('||*p=='[') { 
			l++; 
			if (l<0) error("Syntax");
		}
		else if (*p==')'||*p==']') l--; 
		p++; 
	}
	if (charsearch(*p,op)) { 
		t=(tree *)(malloc(sizeof(tree)));
		t->node[0]=*p; 
		t->node[1]=0; *p=0;
		t->left=string2tree(s); 
		t->right=string2tree(p+1); 
		if (t->right==NULL) { t0=t; t=t0->left; free(t0); } 
		return t;
	}
	return NULL;
}
tree *string2tree(char *s)
{
	char *p=s;
	int i,l=0;
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
	for (i=1;*p;p++) {
		if (*p=='(') i++; if (*p==')') i--; if (i==0) break;
		t->node[l++]=*p; 
	}
/*new
	while (*p&&*p!=')') t->node[l++]=*p++; 
*/
	t->node[l]=0;
	t->left=NULL; t->right=NULL; return t;
}

/*******************************************************************/
#define iscomment(s)			(*s=='/'&&*(s+1)=='/')
/*******************************************************************/
void stringcompress(char *s,char a,char b)
{
        char *t=s;
        char *p=s;

        while (*s) {
                if (*s==9||*s==10||*s==13||*s==a||*s==b) { s++; continue; }
                if (iscomment(s)) { while (*s&&*s!=13&&*s!=10) s++; continue; }
                if (*s=='!') { if (*(t-1)==']'&&*(t-2)=='.') *s=CHANW; }
                *t++=*s++;
        }
	*t=*s;
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
char *stringsearch(char *s,char *a,char *b)
{
	int i=strlen(a),j;

	if (b!=NULL) 
	for (j=strlen(b);1;s++) {
                while (*s&&*s!=*a&&*s!=*b) s++; if (*s==0) { return s; }
                if (strncmp(s,a,i)==0||strncmp(s,b,j)==0) { return s; }
        }
	for (;1;s++) {
                while (*s&&*s!=*a) s++; if (*s==0) { return s; }
                if (strncmp(s,a,i)==0) { return s; }
        }
}
/*******************************************************************/
void *stringreplacechar(char *s,char a,char b)
{
        while (*s&&*s!='#') if (*s==a) *s++=b; else s++;
}
int stringreplacestring(char *s,char *a,char *b)
{
        if (strcmp(s,a)==0) { strcpy(s,b); return 1; } return 0;
}
int stringreplacestringfixlength(char *s,char *a,char *b)
{
        int la=strlen(a);
        int lb=strlen(b);
        char tmp[MAXVARLEN];

        if (strncmp(s,a,la)==0) {
                strcpy(tmp+lb,s+la); strncpy(tmp,b,lb);
                strcpy(s,tmp); return 1;
        }
        return 0;
}
/*******************************************************************/

/*******************************************************************/
int treesize(tree *t0) 
{
	if (t0==NULL) return 0;
	return treesize(t0->left)+treesize(t0->right)+1;
}
tree *treesearchvarlist(char *s,int n,tree *t)
{
	int i;
	int lc=0;
	tree *l0;
	
	while ((l0=listgetone(t,lc++))!=NULL) {
		if (n) i=strncmp(l0->left->node,s,n); 
		else i=strcmp(l0->left->node,s); 
		if (i==0) return l0;
	}

	return NULL;
}
tree *treesearchstringlist(char *s,int n,tree *t)
{
	int i;
	int lc=0;
	tree *l0;
	
	while ((l0=listgetone(t,lc++))!=NULL) {
		if (n) i=strncmp(l0->node,s,n); else i=strcmp(l0->node,s); 
		if (i==0) return l0;
	}

	return NULL;
}
tree *treesearchvarall(char *node,int n,tree *t)
{
	int i;
        tree *t1,*t2;

        if (t==NULL) return NULL;
        if (t->node[0]!='&') {
		if (n) i=strncmp(t->left->node,node,n); 
		else i=strcmp(t->left->node,node); 
                if (i==0) return treecopy(t); return NULL;
        }
        t1=treesearchvarall(node,n,t->left); 
	t2=treesearchvarall(node,n,t->right);
        if (t1==NULL) return t2; if (t2==NULL) return t1;
        return treecompose("&",t1,t2);
}
tree *treedopost(tree *t,tree *f())
{
	if (t==NULL) return t;
	t->left=treedopost(t->left,f); t->right=treedopost(t->right,f); 
	return f(t);
}
void treedopostvoid(tree *t,void f())
{
	if (t==NULL) return;
	treedopostvoid(t->left,f); treedopostvoid(t->right,f); f(t);
}
/*
tree *treedopre(tree *t,int f0())
{
        if (isprop(t)) return (f0(t));
	t->left=treedopre(t->left,f0); t->right=treedopre(t->right,f0); 
	return t;
}
/*******************************************************************/
void treedoprevoid(tree *t,int f0())
{
        if (f0(t)) return;
        treedoprevoid(t->left,f0); treedoprevoid(t->right,f0);
}
/*******************************************************************/
void treefree(tree *t) 
{
	if (t==NULL) return;
	treefree(t->left); treefree(t->right); free(t);
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
/*******************************************************************/
tree *treeconst(int positive)
{               
        switch (positive) {
          case 1: return treecompose(TRUEs,NULL,NULL);
          case 0: treecompose(FALSEs,NULL,NULL);
        }       
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
tree *treeconstab(int positive,int i)
{
        tree *t=(tree *)malloc(sizeof(tree));
        tree *t0=(tree *)malloc(sizeof(tree));
        tree *t1=(tree *)malloc(sizeof(tree));

	sprintf(t0->node,"%ix",i); t0->left=NULL; t0->right=NULL;
        strcpy(t1->node,"!");  t1->left=NULL; t1->right=treecopy(t0);
        if (positive) strcpy(t->node,"|"); else strcpy(t->node,"&");
        t->left=t0; t->right=t1;
        return t;
}
tree *treenum(int n)
{
        tree *t=(tree *)(malloc(sizeof(tree)));
        sprintf(t->node,"%i",n); t->left=NULL; t->right=NULL;
        return t;
}
tree *treesearch(char *s,int n,tree *t)
{
	int i;
	tree *t0;
	if (t==NULL) return NULL;
	if (n) i=strncmp(t->node,s,n); else i=strcmp(t->node,s); 
	if (i==0) return t;
	t0=treesearch(s,n,t->left); if (t0!=NULL) return t0;
	return treesearch(s,n,t->right); 
}
/*******************************************************************/
tree *treetr(tree *tr,int a,int b,int pn)
{
        return
        treeaddnumberconditional(
	treeaddnumber(treecopy(tr),a*pn),(b-a-1)*pn,(a+1)*pn);
}
/*******************************************************************/
tree *treeaddnumber(tree *t,int i)
{
        if (t==NULL) return NULL;
        if (isdigit(t->node[0])) sprintf(t->node,"%i",atoi(t->node)+i);
        t->left=treeaddnumber(t->left,i); t->right=treeaddnumber(t->right,i); 
	return t;
}
/*******************************************************************/
tree *treeaddnumberconditional(tree *t,int i,int c)
{
        int n;
        if (t==NULL) return NULL;
        if (isdigit(t->node[0])) {
                n=atoi(t->node); if (n>c) sprintf(t->node,"%i",n+i);
        }
        t->left=treeaddnumberconditional(t->left,i,c);
        t->right=treeaddnumberconditional(t->right,i,c); return t;
}
/************************************************************************/
int intlen(int *aa)
{
	int i=0; while (*aa) { aa++; i++; } return i;
}
/************************************************************************/








/**********************************************************************/
#ifndef CLK_TCK
#define CLK_TCK       100 /*CLOCKS_PER_SEC*/
#endif
/**********************************************************************/
float utiltime(int finish)
{
        struct tms *strtemps=(struct tms *)malloc(sizeof(struct tms));
        static clock_t start;

        if (finish==0) {
                times(strtemps);
                start= strtemps->tms_utime;
                return 0.0;
        }

        times(strtemps);
        return (float)(strtemps->tms_utime-start)/(float)CLK_TCK;
}
/*******************************************************************/
int fileok(char *s)
{
	int i=1;
	FILE *g=fopen(s,"r");
	if (g==NULL) { fprintf(stdout,"%s not found\n",s); return 0; } 
	fclose(g); return i;
}
int solverok(char *s,char *m)
{
	int c;
	FILE *g=fopen(s,"r");
	if (g!=NULL) { fclose(g); return 1; }

	fprintf(stderr,"%s\n",m);
	fprintf(stdout,"Should I use the internal (slow) SAT-solver (Y/N)? ");
        if ((c=getc(stdin))=='Y'||c=='y') { return 0; }
	exit(0);
}

char *file2string(char *fn)
{
        int len;
        char *ff;
	FILE *f=fopen(fn,"r");

        fseek(f,0,2); len=ftell(f); fseek(f,0,0);
        ff =(char*) malloc(len+1); fread(ff,len,1,f); ff[len-1]=0; fclose(f);
        return ff;
}
/*******************************************************************/
int arrayindex(char *node)
{
        int i=1;
        while (*node&&*node!='[') { i++; node++; }
        if (*node) return i; else return 0;
}
/*******************************************************************/
int marrayindex(char *node)
{
        int i=1;
        while (*node&&*node!='[') { node++; i++; }
        if (*node==0) return 0;
        node++; while (*node&&*node!='[') node++;
        if (*node==0) return 0;
        return i;
}
/*******************************************************************/
int chanindex(char *node)
{
        int i;
        while (*node&&*node!='[') node++;
        if (*node==0) return 0;
        node++; i=atoi(node); while (*node&&isdigit(*node)) node++;
        if (*node==']') return i;
        return 0;
}
/*******************************************************************/
char charsearch(char a,char* s)
{
        while (*s&&*s!=a) s++; return *s;
}
/*******************************************************************/
void quicksort_ab(int *a,int *b,int l,int r ) /* decreasing */
{
        int v,i,j,t;
        if (r>l) {
                v=a[r]; i=l-1;j=r;
                for (;;) {
                        for (i++;(a[i]>v);i++);
                        for (j--;j>=l&&(a[j]<v);j--); if (j<l) j=1;
/*
                        while (a[++i] > v); while (a[--j] < v);
*/
                        if (i>=j) break;
                        t=a[i]; a[i]=a[j]; a[j]=t; t=b[i]; b[i]=b[j]; b[j]=t;
                }
                t=a[i]; a[i]=a[r]; a[r]=t;
                t=b[i]; b[i]=b[r]; b[r]=t;
                quicksort_ab(a,b,l,i-1); quicksort_ab(a,b,i+1,r);
        }
}
/**********************************************************************/
void quicksort_abf(float *a,int *b,int l,int r) /* decreasing */
{
        float v,t0;
        int i,j,t1;
        if (r>l) {
                v=a[r]; i=l-1;j=r;
                for (;;) {
                        for (i++;(a[i]>v);i++);
                        for (j--;j>=l&&(a[j]<v);j--); if (j<l) j=1;
/*
                        while (a[++i] > v); while (a[--j] < v);
*/
                        if (i>=j) break;
                        t0=a[i]; a[i]=a[j];a[j]=t0; t1=b[i]; b[i]=b[j];b[j]=t1;
                }
                t0=a[i]; a[i]=a[r];a[r]=t0;
                t1=b[i]; b[i]=b[r];b[r]=t1;
                quicksort_abf(a,b,l,i-1); quicksort_abf(a,b,i+1,r);
        }
}
/*******************************************************************/

/*******************************************************************/
void treewrite(tree *t,FILE *f)
{
	debug();
        if (t==NULL) return;
        if (t->left==NULL) { fprintf(f,"%s",t->node); treewrite(t->right,f); }
        else {
                fprintf(f,"("); treewrite(t->left,f);
                fprintf(f,"%s",t->node);
                if (t->node[0]==';'||t->node[0]=='#') fprintf(f,"\n");
                treewrite(t->right,f); fprintf(f,")");
        }
}
/*******************************************************************/
void prettyintwrite(int *ss,int a,int b,FILE *g)
{
        int i,j;
	debug();
        for (j=0,i=a;i<=b;i++) {
		fprintf(g,"%i ",ss[i]); 
		if (++j%10==0) fprintf(g,"\n");
	}
	fprintf(g,"\n");
}
/*******************************************************************/
void intwrite(int *ss,int a,int b,FILE *g)
{
        int i,j;
	debug();
        for (j=0,i=a;i<=b;i++) { fprintf(g,"%i ",ss[i]); }
	fprintf(g,"\n");
}
/*******************************************************************/
void charwrite(char *ss,int a,int b,FILE *g)
{
        int i;
	debug();
        for (i=a;i<=b;i++) fprintf(g,"%i ",ss[i]); fprintf(g,"\n");
}
/*******************************************************************/
void stringwrite(char *ss,int a,int b,FILE *g)
{
        int i;
	debug();
        for (i=a;i<=b;i++) fprintf(g,"%c",ss[i]); fprintf(g,"\n");
}
/*******************************************************************/
void modulewrite(module *mm,FILE *f)
{
	debug();
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
void cnfstringwrite(int *mstring,int mstringc,FILE *g)
{
        int i;
	debug();
        for (i=0;i<mstringc;i++)
        if (mstring[i]) fprintf(g," %i ",mstring[i]);
        else fprintf(g," %i\n",mstring[i]);
}
/*******************************************************************/
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
void cnfmodelwrite(model *mo,FILE *g)
{
        int i;
        int *mstring=mo->mstring;
        int mstringc=mo->l;

        fprintf(g,"p cnf %i %i\n",mo->n,mo->m);
        for (i=0;i<mstringc;i++)
        if (mstring[i]) fprintf(g," %i ",mstring[i]);
        else fprintf(g," %i\n",mstring[i]);
}
/*******************************************************************/
void cnfmstructwrite(model *mo,mstruct *cc,char *lcl,int k,FILE *g)
{
        int i;
	int *p;

        fprintf(g,"p cnf %i %i\n",mo->n,k);
        for (i=0;i<mo->m;i++) if (lcl[i]) {
		p=cc->Clause[i];
		while (*p) fprintf(g," %i ",*p++); fprintf(g," %i\n",*p);
	}
}
void cnfmodelappendwrite(model *mo,int *literals,FILE *g)
{
        int i;
        int *mstring=mo->mstring;
        int mstringc=mo->l;

        fprintf(g,"p cnf %i %i\n",mo->n,mo->m+mo->n);
	while (*literals) { fprintf(g," %i 0\n",*literals); literals++; }
        for (i=0;i<mstringc;i++)
        if (mstring[i]) fprintf(g," %i ",mstring[i]);
        else fprintf(g," %i\n",mstring[i]);
}
/*******************************************************************/
