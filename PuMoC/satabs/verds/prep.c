#include "keywords.h"
#include "prep.h"

/*******************************************************************/
char *PREPfile2txt(char *fn)
{
        char *txt;

        txt=file2string(fn);
	txt=PREPreplaceENUM(txt);
        PREPreplaceEQ(txt);
        stringcompress(txt,' ',' ');
        PREPreplaceSTEP(txt);
        stringed(txt,";goto","&"); stringed(txt,":goto",":");
/*
 infos(txt);
*/
        return txt;
}
/*******************************************************************/
void PREPreplaceSTEP(char *txt)
{
	char *t;

        for (t=txt;1;) {
                while (*txt&&*txt!='S') *t++=*txt++;
                if (*txt==0) { *t=0; break; }
                if (strncmp(txt,"STEP",4)==0) { 
			if (isalnum(*(txt+4))) { 
				strncpy(t,"s_p=",4); 
				t+=4; txt+=4;
			}
			else { strncpy(t,"s_p",3); t+=3; txt+=4; }
                } else { *t++=*txt++; }
        }
}
/*******************************************************************/
void PREPreplaceEQ(char *txt)
{
	int i;
	char *t0a,*t0,*t1,*t2;
	tree *tt;
	char *s0=txt;

        while (1) {
		for (i=1;1;txt++) {
                	while (*txt&&*txt!=';') { if (*txt=='=') i=0; txt++; } 
			if (i==0||*txt==0) break;
		}
		if (*txt==0) break;
		t2=txt; i=1;
                while (*t2&&*t2!='=') { if (*t2==',') i=0; t2--; }
		if (i==1) continue; 
		t0=t2-1; t1=t2+1; 
                while (*t0&&isws(*t0)) t0--; while (*t1&&isws(*t1)) t1++;
		if (*t0!=')') continue;
		t0a=PREPmatch0(t0); t2=PREPmatch1(t1);

		tt=PREPcreatevlist(t0a,t0);
		PREPwritevlist(tt,t0a,t1+1,t2); treefree(tt);
        }
}
/*******************************************************************/
char *PREPmatch0(char *t0)
{
	int i=0;
	for (;1;t0--) {
		if (*t0==')') i++; else if (*t0=='(') i--; if (i==0) return t0;
	}
}
char *PREPmatch1(char *t0)
{
	int i=0;
	for (;1;t0++) {
		if (*t0=='(') i++; else if (*t0==')') i--; if (i==0) return t0;
	}
}
tree *PREPcreatevlist(char *t0,char *t1)
{
	char *t2=t0+1;
	while (1) {
		t0++; while (t0<t1) if (*t0!=',') t0++; else break; *t0=0; 
		if (t0<t1) return treecompose(t2,NULL,PREPcreatevlist(t0,t1));
		return treecompose(t2,NULL,NULL);
	}
}
void PREPwritevlist(tree *tt,char *t0,char *t1,char *t2)
{
	char *t;
	t=tt->node; while (*t) *t0++=*t++; *t0++='=';
	while (*t1!=','&&t1<t2) *t0++=*t1++;
	if (t1==t2) { while (t0<=t2) *t0++=' '; return; }
	*t0++='&'; PREPwritevlist(tt->right,t0,t1+1,t2);
}
/*******************************************************************/


/*******************************************************************/
#define MI 	1000
#define MIL 	1000
/*******************************************************************/
LOCAL char **cleft;
LOCAL char **cright;
LOCAL int *clen;
LOCAL int cc=0;
/*******************************************************************/
#define isdchar(c)	(isalpha(c)||isachar(c))
/*******************************************************************/
char *PREPreplaceENUM(char *s)
{
	char *t;
	int i,k,x;
	char *s0=s;
	char *s1=s;
	char *s2;

	repinit(); kwall(); 

    	for (;*s;s++) 
    	if (strncmp(s,"DEFINE",6)==0||strncmp(s,"VAR",3)==0) break;
    	if (strncmp(s,"VAR",3)!=0) {
		s+=6;
    		for (;*s&&strncmp(s,"VAR",3)!=0;) {
			while (*s&&!isdchar(*s)) s++; if (*s==0) break;
			if (k=repgetdef(s,cc)) { s+=k; cc++; } 
			while (*s&&isws(*s)) { s++; }
		}
	}
		
    	for (s=s0;*s;s++) 
	if (strncmp(s,"VVM",3)==0) { 
		s+=3; 
		while (strncmp(s,"VAR",3)!=0) *s++=' '; break; 
	}

        for (;*s;s++) {
	    	for (;*s;s++) if (strncmp(s,"VAR",3)==0) break;
	    	for (;*s;s++) {
			if (*s=='I') break;
			if (*s!='{') continue; t=s; s++; i=0;
			while (k=repgetw(s,cc,i)) { s+=k; cc++; i++; }
			sprintf(t,"%i..%i",0,i-1);
			while (*t) t++; *t=' ';
			while (*t!='}') *t++=' '; *t=' ';
            	}
	}

	repdefinternal(); 
	x=reprewriteok(cleft,cright,clen,cc); 
	if (x==0) s1=(char *)malloc(MAXSTRING*sizeof(char));;
	stringkstring(s0,cleft,cright,clen,cc,s1); 
	if (x==0) free(s0); 

	cc=0; repproc(s1); 
	if (cc) { 
		s2=(char *)malloc(MAXSTRING*sizeof(char));;
		stringkstring(s1,cleft,cright,clen,cc,s2); free(s1); s1=s2;
	}
	repclose(); return realloc(s1,strlen(s1)+1); 
}
/*******************************************************************/
void repinit()
{
	int i;
	cleft=(char **)malloc(MI*sizeof(char *));
	cright=(char **)malloc(MI*sizeof(char *));
	clen=(int *)malloc(MI*sizeof(int));
	for (i=0;i<MI;i++) {
		cleft[i]=(char *)malloc(MIL*sizeof(char));
		cright[i]=(char *)malloc(MIL*sizeof(char));
	}
/*
	infos(cleft[1]); infos(cright[1]);
*/
}
void repclose()
{
	int i;
	for (i=0;i<MI;i++) { free(cleft[i]); free(cright[i]); }
	free(cleft); free(cright); free(clen);
}
/*******************************************************************/
int repgetw(char *s,int cc1,int i)
{
	char *s0=s;
	char *t=cleft[cc1];

	if (*s=='}') return 0;
	clen[cc1]=0;
	while (isws(*s)) { s++; }
	while (*s!=','&&*s!='}'&&!isws(*s)) { *t++=*s++; clen[cc1]++; }
	while (isws(*s)||*s==',') { s++; }
	sprintf(cright[cc1],"%i",i); return s-s0;
}
int repgetdef(char *s,int cc1)
{
	char *s0=s;
	char *t=cleft[cc1];

	clen[cc1]=0;
	while (isdchar(*s)) { *t++=*s++; clen[cc1]++; }
	while (*s!='=') { s++; } 

	s++; t=cright[cc1]; 
	while (*s) { if (*s=='\\') s+=2; if (*s=='\n') break; *t++=*s++; }
	*t=0; return s-s0;
}
void stringkstring(char *s,char **a,char **b,int *c,int k,char *s1)
{
	int i,j;
	int ok;
	char *bk;

	while (*s) {
	  	while (*s&&isws(*s)) *s1++=*s++;
	  	for (ok=0,i=0;i<k;i++) 
       	    	if (strncmp(s,a[i],c[i])==0) {
			if (isalpha(*s)&&isdchar(*(s+c[i]))) { continue; }
			ok=1;
			bk=b[i];
			j=strlen(bk); /* if (c[i]<j)error("PREPreplaceENUM"); */
			memcpy(s1,&(b[i][0]),j); s1+=j; 
			for (;j<c[i];j++) { *s1++=' '; }
			s+=c[i]; break;
	    	}
	  	if (ok==0) { 
			if (isalpha(*s)) while (isdchar(*s)) *s1++=*s++; else 
			*s1++=*s++; 
	  	}
	}
	*s1=*s;
}
/*******************************************************************/
int reprewriteok(char **a,char **b,int *c,int k)
{
	int i;
/*
	for (i=0;i<k;i++ ) { infos(cleft[i]); infos(cright[i]); infoi(c[i]); }
*/
	return 0;
}
/*******************************************************************/
void repproc(char *s)
{
	char *s0,*t0;

/*
    	for (;*s;s++) if (strncmp(s,"PROC",4)==0) break; 
	if (*s==0) return; s+=4;
*/
    	for (;*s;s++) if (*s=='#') break; if (*s==0) return; s++;
    	for (;*s;s++) if (*s=='#') break; if (*s==0) return; s++;
	while (1) {
		while (*s&&isws(*s)) { s++; } 
/*
    		if (strncmp(s,"FAIRNESS",8)==0) break; 
    		if (strncmp(s,"SPEC",4)==0) break; 
    		if (strncmp(s,"MODULE",6)==0) break; 
*/
		if (*s=='#') break; 

		s0=s;
		while (*s&&(*s!=':')) { s++; }
		t0=s-1; while (t0>s0&&isws(*t0)) t0--;
		if (*t0==']') { s+=repgetproc(s0); }
		while (*s&&(*s!=';')) { s++; } if (*s==0) break; s++;
	}
}
int repgetproc(char *s)
{
	int i,k=0,n=0;
	char *t;
	char pname[64];
	char pn1[64];
	int cc1;
	int low,high;

	cc1=cc;
	t=cleft[cc1]; while (*s!=';') { *t++=*s++; n++; } clen[cc1]=n;
	s=cleft[cc1];
	while (isws(*s)) s++;
	while (*s!='['&&!isws(*s)) { pname[k]=*s++; k++; } pname[k]=0;
	while (*s!='[') *s++; s++; while (isws(*s)) s++; low=atoi(s);
	while (*s!='.') *s++; s+=2; while (isws(*s)) s++; high=atoi(s);
	while (*s!=':') *s++; s+=1;
	for (t=cright[cc1],i=low;i<=high;i++) {
		sprintf(pn1,"%s_x_%i:%s;",pname,i,s); 
		strcpy(t,pn1); t+=strlen(pn1);
	}
	*(t-1)=0;
	cc1++;
	for (i=low;i<=high;i++) {
		sprintf(cleft[cc1],"%s[%i]",pname,i); 
		sprintf(cright[cc1],"%s_x_%i",pname,i); 
		clen[cc1]=strlen(cleft[cc1]); cc1++; 
	}
	cc=cc1; return n;
}
/*******************************************************************/
int repdefinternal()
{
	int i;
	char *s0,*s1;
	s1=(char *)malloc(1000*sizeof(char));;
	for (i=1;i<cc;i++) {
		s0=cright[i];
		stringkstring(s0,cleft,cright,clen,i,s1);
		cright[i]=s1; s1=s0;
	}
	free(s1);
}
/*******************************************************************/
