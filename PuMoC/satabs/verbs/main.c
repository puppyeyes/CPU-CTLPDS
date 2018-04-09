#include "main.h"


/*******************************************************************/
#define pspecs	prog->right->right->right
#define ptrans	prog->right->right->left
#define pinit	prog->right->left
#define pvars	prog->left
/*******************************************************************/
model *cnfcreate(tree *prog,tree *spec,tree *cs,int booln,int k,int pv,int ptt)
{
	if (ptt==0) return cnfcreatet0(prog,cs,booln,k);
	else return cnfcreateall(prog,spec,cs,booln,k,pv);
}
model *cnfcreatet0(tree *prog,tree *cs,int booln,int k)
{
	model *mo,*mo2;
	tree *ts;
	
	mo=convmodel(pinit,ptrans,NULL,booln,k,1); 
	mo2=conv2model(pinit,ptrans,NULL,booln,k,mo->n); 
	concatmodel(mo,mo2);

	ts=spec2f1(cs,booln,k,pvars,ptrans); 
	mo2=convmodelspec(ts,mo->n); free(ts);
	concatmodel(mo,mo2);

	return mo;
}
model *cnfcreateall(tree *prog,tree *spec,tree *cs,int booln,int k,int pv)
{
	model *mo,*mo2;
	tree *ts;
	int npath;

	if (pv) {
		npath=fkx(spec,k); if (npath==0) npath=1;
		ts=spec2f1(spec,booln,k,pvars,ptrans); 
	} else {
		ts=negate(treecopy(spec));
		npath=gkx(ts,k); if (npath==0) npath=1; free(ts);
		ts=spec2f0(spec,booln,k,pvars,ptrans); 
	}

	mo=convmodel(pinit,ptrans,NULL,booln,k,npath); 
	mo2=convmodelspec(ts,mo->n); treefree(ts); 
	concatmodel(mo,mo2);

	ts=spec2f1(cs,booln,k,pvars,ptrans); 
	mo2=convmodelspec(ts,mo->n); free(ts);
	concatmodel(mo,mo2);

	reverse(mo->mstring,mo->l,booln,(k+1)*npath,mo->n);


	return mo;
}
/*
int sat(tree *prog,tree *spec,tree *cs,int booln,int k,int pv,int ptt)
{
	int i,r;
	model *mo;
        for (i=0;i<=k;i++) {
        	mo=cnfcreate(prog,spec,cs,booln,i,pv,ptt);
		modelclean(mo);
        	if ((r=issat(mo))==0) { printf("i=%i r=%i\n",i,r); break; }
        	free(mo->mstring); free(mo);
        }
}
*/
int cnfsat(tree *prog,tree *spec,tree *cs,int booln,int k,int pv,int ptt,char *s)
{
	int r;
	char fn[256];
	model *mo;
	FILE *g;

	mo=cnfcreate(prog,spec,cs,booln,k,pv,ptt);
	if (s) {
		r=applysolver(NULL,mo,s); 
              	printf("v=%i i=%i r=%i\n",pv,k,r);
		if (pv&&r==0) return 1;
		if (!pv&&!r==0) return -1;
        } else {
		sprintf(fn,"verbs_%i.%3i",pv,k);
		stringreplacechar(fn,' ','0');
              	g=fopen(fn,"w"); cnfwritemodel(mo,g); fclose(g);
	}
        free(mo->mstring); free(mo);
	return 0;
}


/*******************************************************************/
#define version() printf("verbs version 1.27 - 27 MAY 2009\n")
/*******************************************************************/
LOCAL time_t t0,t1;
LOCAL int PAR_tm=0;
/*******************************************************************/
LOCAL int PAR_test=-1;
LOCAL int PAR_deb=0;
LOCAL int PAR_avfs=7;
LOCAL int PAR_k=100;
LOCAL int PAR_m=0;
LOCAL int PAR_p=1;
/********************************************************************/
#ifdef ASATSOLVER 
LOCAL int PAR_solver='a';
#else
LOCAL int PAR_solver='i';
#endif
/********************************************************************/
pginfo gp;
/********************************************************************/
#define gpvars	 	gp.prog->left
#define gpinit 		gp.prog->right->left
#define gptrans 	gp.prog->right->right->left
#define gpspecs 	gp.prog->right->right->right
/********************************************************************/
LOCAL int main(int argc, char *argv[])
{
	tree *ts,*tspec;
	int i,temp,r;
	char fn[256],cmd[256],solver[256];
	module *mm;
	model *lmo;
	char *ll;
	FILE *g;

	if (argc==1) { version(); exit(0); }
	if (argc==2&&strcmp(argv[1],"-v")==0) { version(); exit(0); }
	version(); 

  	for(i=1;i<=argc-2;i++)
	if (argv[i][0] == '-') {
      		if (isdigit(*(argv[i]+2))) temp=atoi(argv[i]+2); else temp=1;
      		if      (argv[i][1]=='h') PAR_tm =temp*60;
      		else if (argv[i][1]=='t') PAR_test =temp;
      		else if (argv[i][1]=='d') PAR_deb =1;
      		else if (argv[i][1]=='v') PAR_avfs &=3;
      		else if (argv[i][1]=='f') PAR_avfs &=5;
      		else if (argv[i][1]=='s') {
      			if (*(argv[i]+2)==0) PAR_avfs &=6; else 
			PAR_solver=*(argv[i]+2);
		}
      		else if (argv[i][1]=='k') PAR_k =temp;
      		else if (argv[i][1]=='m') PAR_m =temp;
      		else if (argv[i][1]=='p') PAR_p =temp;
	}
	strcpy(fn,argv[i]); time(&t0); utiltime(0);
	if (PAR_avfs==0) return 0; 
	if ((PAR_avfs&0x1)==0) PAR_solver=0;

	switch (PAR_solver) {
#ifdef ASATSOLVER
		case 'a': 
                if (solverok(ASATSOLVER,"The specified SAT-solver not found"))
                strcpy(solver,ASATSOLVER); break;
#endif
#ifdef BSATSOLVER 
		case 'b': 
                if (solverok(BSATSOLVER,"The specified SAT-solver not found"))
		strcpy(solver,BSATSOLVER); break;
#endif
#ifdef CSATSOLVER 
		case 'c': 
                if (solverok(CSATSOLVER,"The specified SAT-solver not found"))
		strcpy(solver,CSATSOLVER); break;
#endif
		default:  *solver=0;
	}
	
	if (PAR_avfs==0x1) {
		r=applysolver(fn,NULL,solver); 
		if (i==1) printf("CONCLUSION: SAT\n");
		if (i==0) printf("CONCLUSION: UNSAT\n");

		printf("Diff		= %f\n",(float)utiltime(1));
		printf("Diff		= %i\n",(int)difftime(time(&t1),t0));
		return i;
	}

	gp.prog=file2prog(fn);
	gp.prog=progreconstruct(gp.prog);
	gp.ndlock=ndlockconstruct(gp.prog);
	gp.cs=csconstruct(gp.prog);

        gp.booln=vlistcons(gpvars,0); 
/**/
	gp.prog=prog2bool(gp.prog,gp.booln);
	gp.cs=cs2bool(gp.cs,gp.prog,gp.booln);
	gp.ndlock=cs2bool(gp.ndlock,gp.prog,gp.booln);

	gp.spec=specs(gp.prog,&gp,PAR_test,PAR_p);

	for (i=PAR_m;i<=PAR_k;i++) {
		if (PAR_tm&&(int)difftime(time(&t1),t0)>PAR_tm) break;
		if (PAR_avfs&0x4) {
			r=cnfsat(gp.prog,gp.spec,gp.cs,gp.booln,
				i,1,PAR_test,solver);
			if (r) break;
			printf("Time	= %i\n",(int)difftime(time(&t1),t0));
		}
		if (PAR_test==-1&&PAR_avfs&0x2) {
			r=cnfsat(gp.prog,gp.spec,gp.cs,gp.booln,
				i,0,PAR_test,solver);
			if (r) break;
			printf("Time	= %i\n",(int)difftime(time(&t1),t0));
		}
	}
	printf("Diff		= %f\n",(float)utiltime(1));
	printf("Diff		= %i\n",temp=(int)difftime(time(&t1),t0));
	if (r==1) printf("CONCLUSION: TRUE");
	if (r==-1) printf("CONCLUSION: FALSE");
	if (r==0) { printf("CONCLUSION: UNKNOWN"); i--; }
	printf(" (k=%i time=%i)\n",i,temp);
	return r;
}

/*******************************************************************/
tree *agTRUE()
{
	tree *t=treeconst(1);
	return treecompose("A",NULL,treecompose("G",NULL,t));
}
tree *afagTRUE()
{
	tree *t=agTRUE();
	return treecompose("A",NULL,treecompose("F",NULL,t));
}
tree *agagTRUE()
{
	tree *t=agTRUE();
	return treecompose("A",NULL,treecompose("G",NULL,t));
}
int applysolver(char *fn,model *mo,char *mnsat)
{
	FILE *g;
	char cmd[256];
	char *ll;
	int r;

	if (*mnsat==0) {
		if (fn!=NULL) mo=file2model(fn); else modelclean(mo); 
		r=issat(mo);
		return r;
	}

	if (fn!=NULL) sprintf(cmd,"%s %s > tmp_1",mnsat,fn);
	else {
		sprintf(cmd,"verbs.tmp");
        	g=fopen(cmd,"w"); cnfwritemodel(mo,g); fclose(g);
		sprintf(cmd,"%s verbs.tmp > tmp_1",mnsat);
	}
	system(cmd);
	g=fopen("tmp_1","r"); ll=file2string(g); fclose(g);
	r=(stringsearch(ll,"UNSAT")==NULL); free(ll);
	return r;
}

/*******************************************************************/
#define pspecs	prog->right->right->right
#define ptrans	prog->right->right->left
#define pinit	prog->right->left
#define pvars	prog->left
/*******************************************************************/
extern int PAR_deb;
/*******************************************************************/
tree *file2prog(char *fn)
{
	tree *prog;
	char *string;

	string=file2string(fopen(fn,"r"));
	if (PAR_deb) { stringwrite(string,stdout); nl }

	cleanstring(string); 
	prog=merge(string2module(string));
	listtraverse(ptrans,cleancolon);
	treet1(pinit,replacesemiand);
	if (PAR_deb) { treewrite(prog,stdout); nl }

	replacechan(prog->right,pvars);
	if (PAR_deb) { treewrite(prog,stdout); nl }

	replacemarray(prog->right,pvars);
	if (PAR_deb) { treewrite(prog,stdout); nl }
/*
	prog=string2tree(string); 
*/
	free(string); return prog;
}
tree *progreconstruct(tree *prog)
{
	listtraverse(pspecs,nnftemp); 
	ptrans=treeadddead(ptrans); 
	if (PAR_deb) { treewrite(prog,stdout); nl }

	treeaddeq(ptrans,pvars); 
        ptrans=treetraverse(ptrans,NULL,NULL,cleanarray); 
	if (PAR_deb) { treewrite(prog,stdout); nl }

	treeaddx(ptrans); 
	treeaddvc(prog->right,pvars); 
	if (PAR_deb) { treewrite(prog,stdout); nl }

        pvars=replacearray(prog->right,pvars); 
	treeaddvc(prog->right,pvars); 
	if (PAR_deb) { treewrite(prog,stdout); nl }

	simplifyarrays(prog->right);
	if (PAR_deb) { treewrite(prog,stdout); nl }

	return prog;
}
tree *csconstruct(tree *prog)
{
	tree *t;
        t=treecompose("F",NULL,negate(constr(pvars))); 
	treeaddvc(t,pvars); 
	return t;
}
tree *ndlockconstruct(tree *prog)
{
	tree *t;

	t=treecopy(ptrans->left->left); 
	t->node[0]='G'; t=treecompose("A",NULL,t); 
	return t;
}
tree *prog2bool(tree *prog,int booln)
{
	replaceconstants(prog); 
	if (PAR_deb) { treewrite(prog,stdout); nl }

        replacevarconst(prog->right,pvars,booln); V102
	if (PAR_deb) { treewrite(prog,stdout); nl }

	ptrans=treetraverse(ptrans,NULL,NULL,cleanbool); 
	prog->right=doall(prog->right,forsimplify); V102
	ptrans=doall(ptrans,nnf); 
	return prog;
}
tree *cs2bool(tree *spec,tree *prog,int booln)
{
	replaceconstants(spec); 
        replacevarconst(spec,pvars,booln); 
	spec=doall(spec,forsimplify); 
	spec=doall(spec,nnf); 
	return spec;
}
tree *specs(tree *prog,pginfo *pgi,int pt,int pp)
{
	int i;
	tree *t;

	if (pt==1) return agTRUE(); 
	if (pt==2) return agagTRUE(); 
	if (pt==3) return afagTRUE(); 
	if (pp==0) return pgi->ndlock;
	if (pspecs==NULL) return pgi->ndlock;
	t=pspecs;
	for (i=1;i<pp;i++) t=t->right; if (t->node[0]==';') t=t->left; 
	if (PAR_deb) { treewrite(t,stdout); nl }
	return t;
}


/*******************************************************************/
int reverse_1(int x,int bn,int m)
{
	int s;

	if (x>0) s=1; else { s=-1; x=-x; }
	x=((m-((x-1)/bn)*2)-1)*bn+x;
	return x*s;
}

int reverse_2(int x,int aux1a,int n)
{
	int s;

	if (x>0) s=1; else { s=-1; x=-x; }
	if (x>aux1a) x-=aux1a; else x+=(n-aux1a);
	return x*s;
}

void reverse(int *s1,int n1,int bn,int m,int n)
{
	int j;
	int aux1a=m*bn;


 return;
	for (j=0;j<n1;j++) 
/*
	if (s1[j]!=0&&s1[j]<=aux1a&&s1[j]>=-aux1a) s1[j]=reverse_1(s1[j],bn,m);
*/
	if (s1[j]!=0) s1[j]=reverse_2(s1[j],aux1a,n);
}
/*******************************************************************/
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
/********************************************************************/
