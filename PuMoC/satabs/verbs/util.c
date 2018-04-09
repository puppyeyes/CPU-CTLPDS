#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/times.h>
#include <time.h>
#include "trop.h"
#include "util.h"





#define CLK_TCK 100



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
char *file2string(FILE *f)
{
        int len;
        char *ff;

        fseek(f,0,2); len=ftell(f); fseek(f,0,0);
        ff =(char*) malloc(len+1); fread(ff,len,1,f); ff[len-1]=0;
        return ff;
}
/*******************************************************************/
void writeintlist(int *ss,int a,int b,FILE *g)
{
        int i,j;
        for (j=0,i=a;i<=b;i++) {
		fprintf(g,"%i ",ss[i]); 
		if (++j%10==0) fprintf(g,"\n");
	}
	fprintf(g,"\n");
}
/*******************************************************************/
void writecharlist(char *ss,int a,int b,FILE *g)
{
        int i;
        for (i=a;i<=b;i++) fprintf(g,"%i ",ss[i]); fprintf(g,"\n");
}
/*******************************************************************/
void quicksort_ab(int *a,int *b,int l,int r ) /* decreasing */
{
        int v,i,j,t;
        if (r>l) {
                v=a[r]; i=l-1;j=r;
                for (;;) {
                        while (a[++i] > v); while (a[--j] < v);
                        if (i>=j) break;
                        t=a[i]; a[i]=a[j];a[j]=t;
                        t=b[i]; b[i]=b[j];b[j]=t;
                }
                t=a[i]; a[i]=a[r];a[r]=t;
                t=b[i]; b[i]=b[r];b[r]=t;
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
                        while (a[++i] > v); while (a[--j] < v);
                        if (i>=j) break;
                        t0=a[i]; a[i]=a[j];a[j]=t0;
                        t1=b[i]; b[i]=b[j];b[j]=t1;
                }
                t0=a[i]; a[i]=a[r];a[r]=t0;
                t1=b[i]; b[i]=b[r];b[r]=t1;
                quicksort_abf(a,b,l,i-1); quicksort_abf(a,b,i+1,r);
        }
}
/*******************************************************************/

/*******************************************************************/
#define isLiteral(c)     (isdigit(c)||(c=='-'))
#define isNumbers(ll)	 ((strncmp(ll,"p cnf",5)==0))
#define moveLiteral(ll)  while (isLiteral(*ll)) ll++; 
#define moveNLiteral(ll) while (*ll&&!isLiteral(*ll)&&*ll!='\n') ll++;
/*******************************************************************/
void cnfwritemodel(model *mo,FILE *g)
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
void modelcompress(model *mm,mstruct *cc,mdata *wc,int *cl)
{
        int i,k;
        char *gim=wc->InactiveMatrix;
	int *p0=mm->mstring;
	int *p2=mm->mstring;

	for (k=i=0;i<mm->m;i++) if (cl[i]) {
		p2=cc->Clause[i];
		while (*p2) if (gim[*p2]&1) p2++; else *p0++=*p2++;
		*p0++=*p2++; k++;
	}
	mm->m=k;
	mm->l=p0-mm->mstring;
	mm->mstring=realloc(mm->mstring,mm->l*sizeof(int));
}
/************************************************************************/
model *file2model(char *fn)
{
	FILE *f;
	char *ll; 
        model *mm=malloc(sizeof(model)); 

	f=fopen(fn,"r"); ll=file2string(f); fclose(f);
	read_numbers(ll,mm); 
	read_clauses(ll,mm); free(ll);
	return mm;
}
void read_numbers(char *ll,model *mm)
{
	int n,m,l;

	m=n=l=0;
	while (*ll) {
		if (*ll=='c') { while (*ll&&*ll!='\n') ll++; }
		else if (isNumbers(ll)) { break; }
		else ll++;
	}
	if (*ll) {
		moveNLiteral(ll); n=atoi(ll); moveLiteral(ll); 
		moveNLiteral(ll); m=atoi(ll); moveLiteral(ll); 
		moveNLiteral(ll); if (isLiteral(*ll)) l=atoi(ll); 
	}
	if (!(n>0&&m>0)) error("numbers required.");

	if (l==0) l=2*strlen(ll); 
	mm->n=n; mm->m=m; mm->l=l;
        mm->mstring=malloc(l*sizeof(int)); 
}
void read_clauses(char *ll,model *mm)
{
        int i=0,j=0,k=0,len,n;
        int clen,c;
	int modm=mm->m;
	int modl=mm->l;
	int *p0=mm->mstring;
	
	while (*ll) {
		if (*ll=='c') { while (*ll&&*ll!='\n') ll++; }
		else if (isNumbers(ll)) { while (*ll&&*ll!='\n') ll++; break; }
		else ll++;
	}
	for (;;) {
		while (*ll&&(!isLiteral(*ll)||*ll=='0')) ll++;
		if (*ll==0) break;
                if (j==modm) error("too many clauses");
                for (clen=0,i=0;i<modl;i++) {
			c=atoi(ll); moveLiteral(ll); moveNLiteral(ll);
			if (!c) {
				n=remove_red(p0-clen,clen); 
				p0-=n; k-=n;
				if (clen>n) *p0++=c; else j--; break;
			}
        		if (k==modl) error("too many literals"); 
			k++; *p0++=c; clen++;
                }
                j++;
        }
	mm->m=j; mm->l=k+j; 
	mm->mstring=realloc(mm->mstring,mm->l*sizeof(int)); 
}
/************************************************************************/
int remove_red(int *p0,int k)
{
	int i,j;

	for (i=0;i<k;i++)
	for (j=i+1;j<k;j++) {
		if (p0[i]==p0[j]) { 
			p0[i]=p0[k-1]; p0[k-1]=0;
			return remove_red(p0,k-1)+1; 
		}
		if (p0[i]==-p0[j]) { return k; }
	}
	return 0;
}
int remove_red2(int *p0)
{
	int *p1=p0;
	int k;
	
	while (*p1) p1++; k=p1-p0;
	if (k==remove_red(p0,k)) return 0; return 1;
}
/************************************************************************/
void modelclean(model *mm)
{
        int i;
	int *p0=mm->mstring;
	int *p2=mm->mstring;
	int m=0;
	int n,k;

	for (i=0;i<mm->m;i++) {
		k=0;
		while (*p0) { *p2++=*p0++; k++; }
		n=remove_red(p2-k,k);
		p2-=n; k-=n; if (k>0) { *p2++=0; m++; } 
		p0++;	/*zwh*/
	}
	mm->m=m;
	mm->l=p2-mm->mstring;
	mm->mstring=realloc(mm->mstring,mm->l*sizeof(int));
}
/************************************************************************/
void modelcompresseq(model *mm,mstruct *cc,mdata *wc,int *cl,int *n1,int *n2)
{
        int i,j,k;
        char *gim=wc->InactiveMatrix;
	int *p0=mm->mstring;
	int *p2=mm->mstring;

	for (k=i=0;i<mm->m;i++) if (cl[i]) {
		p2=cc->Clause[i];
		while (*p2) if (gim[*p2]&1) p2++; else {
		    	if (j=intin(*p2,n1,1,*n1)) *p0++=n2[j]; 
		    	else if (j=intin(-*p2,n1,1,*n1)) *p0++=-n2[j]; 
			else *p0++=*p2;
			p2++;
		}
		*p0++=*p2++; k++;
	}
	mm->m=k;
	mm->l=p0-mm->mstring;
	mm->mstring=realloc(mm->mstring,mm->l*sizeof(int));
	modelclean(mm);
}
/************************************************************************/
int intin(int a,int *b,int x,int y)
{
	int i;
	for (i=x;i<=y;i++) if (a==b[i]) return i; return 0;
}
int eqcycle(int *a,int *b)
{
	int i,j;
	for (i=1;i<=*b;i++) {
		if (j=intin(b[i],a,1,i-1)) b[i]=b[j];
		if (j=intin(-b[i],a,1,i-1)) b[i]=-b[j];
		if (a[i]==-b[i]) { return 0; }
		if (a[i]==b[i]) {
			for (j=i+1;j<=*b;j++) {
				a[j-1]=a[j]; b[j-1]=b[j]; 
			}
			a[0]--; b[0]--; i--; continue;
		}
		j=1;
		while (j) {
			if (j=intin(a[i],b,1,i-1)) b[j]=b[i];
			else if (j=intin(-a[i],b,1,i-1)) b[j]=-b[i];
		}
	}
	return 1;
}
/************************************************************************/

/*******************************************************************/
char *stringsearch(char *s,char *a)
{
	int i=strlen(a);

	while (1) { 
		while (*s&&*s!=*a) s++;
		if (*s==0) { return NULL; }
		if (strncmp(s,a,i)==0) { return s; }
		s++;
	}
}

void stringedone(char *s,char *a,char *b)
{
        int la=strlen(a);
        int lb=strlen(b);
        char tmp[MAXVARLEN];

        if (strncmp(s,a,la)==0) {
                strcpy(tmp+lb,s+la); strncpy(tmp,b,lb);
                strcpy(s,tmp);
        }
}

void stringreplace(char *s,char *a,char *b)
{
	if (strcmp(s,a)==0) strcpy(s,b); 
}

