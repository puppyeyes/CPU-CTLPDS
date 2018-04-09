#include "main.h"
#include "main.h"

/********************************************************************/
#define mreturn(r)	{ PAR_bound=k; return r; }
#define prverdstmp()    { sprintf(fn,".verds_%i.%3i",bv,k); }
/********************************************************************/
#define usr_msg2(r)	\
 if (bv==1) printf("bound =%3i  time = %i\n",k,(int)difftime(time(&t1),t0)); \
 if (bv==0) printf("----------  time = %i\n",(int)difftime(time(&t1),t0)); \
/********************************************************************/
extern char *PAR_qbfsolver;
extern char *PAR_satsolver;
/********************************************************************/
extern int PAR_ifness;
extern int PAR_m;
extern int PAR_bound;
/********************************************************************/
extern int PAR_tm;
extern time_t t0,t1;
/********************************************************************/
int qbfbmc(workspace *ws2,tree *spec)
{
	int k,r,bv;
	qmdata *qm;
	qinfo *qi;
	char fn[100];
	FILE *g;
	int i;
	model *mm0;

	if (PAR_qbfsolver==NULL) return 0; 

	for (r=0,k=PAR_m;k<=PAR_bound;k++) {
	    for (bv=1;bv>=0;bv--) {
		if (PAR_verify==1&&bv==0) continue;
		if (PAR_tm&&(int)difftime(time(&t1),t0)>PAR_tm) mreturn(r);
		qi=(qinfo *)calloc(1,sizeof(qinfo));
		qm=bp2q(qi,ws2->prog,spec,ws2->booln,k,bv);
        	if (strcmp(PAR_qbfsolver,"_")!=0) {
			i=md2treetype(qm);
			if (i==1&&PAR_bmc==4) {
				mm0=qmd2model(mdnegate(qm),qi);
				if (bv==1) r=SOLVEapply(PAR_satsolver,mm0,bv); 
				else r=-SOLVEapply(PAR_satsolver,mm0,!bv); 
				free(mm0->mstring); free(mm0); 
			} else if (i==2&&PAR_bmc==4) {
				mm0=qmd2model(qm,qi);
				if (bv==0) r=SOLVEapply(PAR_satsolver,mm0,bv); 
				else r=-SOLVEapply(PAR_satsolver,mm0,!bv); 
				free(mm0->mstring); free(mm0); 
			} else { 
				r=SOLVEapplyq(PAR_qbfsolver,qm,qi,bv); 
			}
		} else {
                	prverdstmp(); stringreplacechar(fn,' ','0');
                	g=fopen(fn,"w"); qmdwrite(qm,qi,g); fclose(g);
			i=md2treetype(qm);
			if (i==1) {
                		prverdstmp(); stringreplacechar(fn,' ','a');
                		g=fopen(fn,"w"); qmdwritecnf(mdnegate(qm),qi,g);
				fclose(g);
			} else if (i==2) {
                		prverdstmp(); stringreplacechar(fn,' ','e');
                		g=fopen(fn,"w"); qmdwritecnf(qm,qi,g);
				fclose(g);
			} 
			freeqmdata(qm); free(qi);
		}
		usr_msg2(r);
		if (r) mreturn(r);
	    }
	}
	mreturn(r);
}
/********************************************************************/
int satbmc(workspace *ws2,tree *spec)
{
	int k,r,bv;
	model *mm0;
	char fn[100];
	FILE *g;

	if (PAR_satsolver==NULL) return 0; 

	for (r=0,k=PAR_m;k<=PAR_bound;k++) {
	    for (bv=1;bv>=0;bv--) {
		if (PAR_verify==1&&bv==0) continue;
		if (PAR_tm&&(int)difftime(time(&t1),t0)>PAR_tm) mreturn(r);
		mm0=bp2s(ws2->prog,ws2->varconstr,spec,
				ws2->booln,k,bv,PAR_option); 
        	if (strcmp(PAR_satsolver,"_")!=0) {
			r=SOLVEapply(PAR_satsolver,mm0,bv); 
		} else {
                	prverdstmp(); stringreplacechar(fn,' ','0');
                	g=fopen(fn,"w"); cnfwritemodel(mm0,g); fclose(g);
		}
		free(mm0->mstring); free(mm0); usr_msg2(r);
		if (r) mreturn(r);
	    }
	}
	mreturn(r);
}
/********************************************************************/
void qmdwritecnf(qmdata *qm,qinfo *qi,FILE *g)
{
	model *mo;
	tree *t=md2tree(qm);
	t=treedopost(t,TLFnnf);
	t=treedopost(t,TLFnnfsimp);
	mo=CNFspec2model(t,qi->n); cnfwritemodel(mo,g); 
	treefree(t); free(mo->mstring); free(mo); 
}
/********************************************************************/
model *qmd2model(qmdata *qm,qinfo *qi)
{
	model *mo;
	tree *t=md2tree(qm);
	t=treedopost(t,TLFnnf);
	t=treedopost(t,TLFnnfsimp);
	mo=CNFspec2model(t,qi->n); treefree(t);
	return mo;
}
/********************************************************************/

/********************************************************************/
#define version() printf("VERSION:    verds 1.32 - JAN 2011\n")
/********************************************************************/
extern O1npath;
extern char **ORDv1;
extern char **ORDv2;
extern int *ORDv0;
/********************************************************************/
LOCAL FILE *gLOG;
LOCAL workspace *ws1,*ws2;
LOCAL qmdata *qm;
LOCAL qinfo *qi;
/*******************************************************************/
#define max(a,b) (a>b?a:b)
/*******************************************************************/
extern int PAR_tm;
extern int PAR_ag;
extern int PAR_verify;
extern int PAR_test;
extern int PAR_m;
extern int PAR_option;
/********************************************************************/
extern int PAR_p;
extern int PAR_ck;
extern char *PAR_ifn;
extern char *PAR_qbfsolver;
extern char *PAR_satsolver;
extern char *PAR_v;
extern time_t t0,t1;
/********************************************************************/
extern int PAR_bmc;
extern int PAR_bound;
/********************************************************************/
LOCAL char *mainparameters(int argc, char *argv[]);
LOCAL void mainchecksolver(char *solver,int s);
/********************************************************************/
LOCAL int main(int argc, char *argv[])
{
	FILE *f=fopen("test.tmp","w");
	int k,r,bv,npath,q;
	tree *prog;
	tree *spec,*bspec,*spectype;
	model *mm0;
	char solver[128];
	int type=0;
        tree *processes=treecompose(";",NULL,NULL);
	tree *trs;
	tree *vl0;
	tree *fness;

	version();
	PAR_ifn=mainparameters(argc,argv); 
	if (PAR_ifn==NULL) { return 0; }
	time(&t0); utiltime(0);

	checkfiles();
	prog=f2p0(PAR_ifn,processes); vl0=treecopy(prog->left);
	fnessaddprog(prog,processes); 
	
	if (report(1,PAR_ifn,prog->right->right->right->right,0,0)==0) return 0;

	prog=f2p1(prog);

	ws1=p2ws(prog);
	if (PAR_v) var_order(ws1->prog->left,vl0,ws1->booln); treefree(vl0);

	ws2=p2bp(ws1); 
	ws2->fness=fnessget(ws2->prog); if (PAR_ifness) ws2->fness=NULL;
	spec=crspec(ws2,PAR_ck);

	if (PAR_bmc==0) {
		trs=ws2->prog->right->right->left;
		if (PAR_ck<0) { PAR_ag=PAR_ag|(-PAR_ck); trs=trs->right; }
		r=m2start(ws2->prog->right->left,trs,ws2->fness,spec,ws2->booln,
			PAR_ag,ORDv1,ORDv2,ORDv0); 
		return report(0,NULL,NULL,2*r-1,k);
	}

	q=SOLVEcheck(PAR_satsolver,PAR_qbfsolver,isTLFtypeAEorE(spec),PAR_bmc);
	switch (PAR_bmc) {
    		case 1: r=qbfbmc(ws2,spec); break;
    		case 2: r=satbmc(ws2,spec); break;
    		case 3: if (q==0) r=satbmc(ws2,spec); else 
			r=qbfbmc(ws2,spec); break;
		case 4: r=qbfbmc(ws2,spec); break;
	}
	return report(0,NULL,NULL,r,PAR_bound);
	
	if ((spectype=TLFtype1(spec))!=NULL) { treefree(spectype); type=1; }
	if ((spectype=TLFtype3(spec))!=NULL) { treefree(spectype); type=3; }


	for (k=PAR_m;k<=PAR_bound;k++) {
	    for (bv=1;bv>=0;bv--) {
		if (PAR_verify==1&&bv==0) continue;
		if (PAR_tm&&(int)difftime(time(&t1),t0)>PAR_tm) goto res;

		if (PAR_option&8)
		if (O3repformula(type,bv,solver,ws2,spec,k)) type=0;

		if (spec->node[0]==TRUE&&bv==1) mm0=createnonrepmm0(ws2,k); 
		else {
			mm0=bp2s(ws2->prog,
				ws2->varconstr,spec,ws2->booln,k,bv,PAR_option); 
		}

		if (PAR_option&2)
		{
			O1npath=SPECnpath(spec,bv,k,PAR_option);
        		r=O1SOLVEapply(solver,mm0,spec,k,ws2->booln,0,bv); 
		} else {
			r=SOLVEapply(solver,mm0,bv); 
		}

		free(mm0->mstring); free(mm0); 
 r1:
		printf("v=%i i=%i r=%i\n",bv,k,(r==1?0:(r==-1?1:bv)));
		printf("Time	= %i\n",(int)difftime(time(&t1),t0));
		if (r) goto res;
	    }
	}
 res:
	printf("File	= %s\n",PAR_ifn);
	printf("Diff	= %f\n",(float)utiltime(1));
	printf("Diff	= %i\n",bv=(int)difftime(time(&t1),t0));
	if (r==1) printf("CONCLUSION: TRUE");
	if (r==-1) printf("CONCLUSION: FALSE");
	if (r==0) { printf("CONCLUSION: UNKNOWN"); k--; }
	printf(" (k=%i time=%i)\n",k,bv);
		
	fclose(f); return r;
}
/********************************************************************/

/*******************************************************************/
#define pspec	prog->right->right->right
/*******************************************************************/
void fnesscheck(tree *prog)
{
	tree *fness;

	if (pspec->node[0]!='#') { error("checkfness"); } 
	fness=pspec->left; reprunning2(fness,-1); 
/*
	return listrmstring("running",fness);
*/
}
/*******************************************************************/
tree *fnessget(tree *prog)
{
	tree *p0;
	tree *fness;

	if (pspec->node[0]!='#') { error("checkfness"); }
	fness=pspec->left; p0=pspec; pspec=pspec->right; free(p0);

	return fness;
}
/*******************************************************************/
tree *fnessaddvlist(tree *list,int n)
{
	tree *t0;
	char v0[64],v1[64];
	
	sprintf(v0,PIDs); sprintf(v1,"%i..%i",0,n-1); 
	t0=treecompose(":",
		treecompose(v0,NULL,NULL),treecompose(v1,NULL,NULL));
	return treecompose(";",t0,list);
}
/*******************************************************************/
tree *fnessaddilist(tree *list,int n)
{
	tree *t0;
	char v1[64];
	
	sprintf(v1,"%i",0); 
	t0=treecompose("=",
		treecompose(PIDs,NULL,NULL),treecompose(v1,NULL,NULL));
	return treecompose("&",t0,list);
}
/*******************************************************************/
void fnessaddtlist(int n,tree *dbpr)
{
	int i,lc;
	tree *start,*middle,*next,*tr1;

        for (i=0;dbpr->left!=NULL;dbpr=dbpr->right,i++) {
		middle=dbpr->left; next=dbpr->right->left;
        	for (lc=0,start=middle;start!=next;start=start->right) {
                	tr1=listgetone(start,lc++); if (tr1==NULL) continue;
			fnessaddt(tr1,i); if (start->node[0]!=';') break;
                }
        }
}
/*******************************************************************/
void fnessaddt(tree *list,int n)
{
	tree *t0;
	char v1[64];
	
	sprintf(v1,"%i",n); 
	t0=treecompose("=",
		treecompose(PIDs,NULL,NULL),treecompose(v1,NULL,NULL));
	list->right=treecompose("&",t0,list->right);
}
/*******************************************************************/
#define pspec 	prog->right->right->right
/*******************************************************************/
void *fnessaddprog(tree *prog,tree *processes)
{
	int n;
	tree *tn;
	tree *dbpr=processes;

	fnesscheck(prog);
	tn=treesearch(PIDs,0,pspec->left); if (tn==NULL) return;
	for (n=0;dbpr->left!=NULL;dbpr=dbpr->right,n++);
	prog->left=fnessaddvlist(prog->left,n);
	prog->right->left=fnessaddilist(prog->right->left,n);
	fnessaddtlist(n,processes);
}
/*******************************************************************/

/*******************************************************************/
#define pbody   ws2->prog->right
#define pspecs  ws2->prog->right->right->right
#define ptrans  ws2->prog->right->right->left
#define pinit   ws2->prog->right->left
#define pvars   ws2->prog->left
/*******************************************************************/
model *createnonrepmm0(workspace *ws2,int k)
{
        model *mo,*mo2;
        tree *ts1,*ts2;

        prts("nonrep models");
        mo=CNFsys2model(pinit,ptrans,ws2->booln,k,1);
        mo2=CNFnonrepetition(pinit,ptrans,ws2->booln,k,mo->n);
        CNFconcatmodel(mo,mo2);

	mo=bp2sADDconstr(mo,ws2->prog,ws2->varconstr,ws2->booln,k);
        return mo;
}
/*******************************************************************/
model *addnonrep(workspace *ws2,int k,model *mo)
{
        model *mo2;
        tree *ts1,*ts2;

        mo2=CNFnonrepetition(pinit,ptrans,ws2->booln,k,mo->n);
        CNFconcatmodel(mo,mo2);

        return mo;
}
/*******************************************************************/

/*******************************************************************/
#define tmp1file "satlog.tmp"
#define tmp2file "satlom.tmp"
#define tmp0file "verds.tmp"
/*******************************************************************/
LOCAL int O1npath;
/*******************************************************************/
int O1SOLVEapply(char *solver,model *mo,tree *spec,int k,int low,int h,int bv)
{
	FILE *g;
	char cmd[256];
	char *ll,*l0;
	int r;
	int k1,k2;

/*
*/
	if (bv==1) 
	return SOLVEapply(solver,mo,bv); 

	if (*solver==0) r=SATapply(mo); 
	else {

        	O1prepare(mo,0,0,NULL,0); 
 pprts("booln");
 pprti(low);
 pprti((k+1)*O1npath);
		O1prepareliterals(spec,(k+1)*O1npath,low);
        	while (1) {
/*
 getc(stdin);
*/
       			g=fopen(tmp0file,"w"); 
			k2=O1prepare(mo,0,0,g,1); fclose(g);
			if (k2==k1) { 
/*
				continue;
*/
 pprts("k1=k2");
 pprti(k1);
 pprti(mo->m);
       				O1prepare(mo,0,0,NULL,2); 
				return SOLVEapply(solver,mo,bv); 
			}
			k1=k2;
 pprti(k1);
 pprti(mo->m);
			r=SOLVEapply_0(solver); if (r==-1) break;
                	if (r==0) break; if (k1==mo->m) break;
/*cost too much, no gain.
       			g=fopen(tmp0file,"w"); 
                	O1rmodelappend(mo,tmp2file,low,g); fclose(g);
 pprti(k1);
 pprti(mo->m);
			r=SOLVEapply_0(solver); if (r==-1) break;
			if (r==1) break;
*/
		}
	}
       	O1prepare(mo,0,0,NULL,2); 
	if (bv&&r==0) return 1;
	if (!bv&&r==1) return -1;
	return 0;
}
/*******************************************************************/

/********************************************************************/
#define OTT	1
#define ODF	2
#define ORDM	20
/********************************************************************/
#define err_msg1 "error: no properties have been specified"
#define err_msg2 "error: the selection is not valid"
#define err_msg3 "error: the selected property does not exist" 
/********************************************************************/
#define max(a,b) (a>b?a:b)
/*******************************************************************/
LOCAL int PAR_ag=0;
LOCAL int PAR_test=0;
LOCAL int PAR_option=0;
/*******************************************************************/
LOCAL int PAR_verify=-1;
LOCAL int PAR_tm=0;
LOCAL time_t t0,t1;
/*******************************************************************/
LOCAL int PAR_p=0;
LOCAL int PAR_ck=1;
LOCAL char *PAR_v=NULL;
/*******************************************************************/
LOCAL int PAR_bmc=0;
LOCAL int PAR_ifness=0;
LOCAL int PAR_m=0;
LOCAL int PAR_bound=10000;
LOCAL char *PAR_satsolver=NULL;
LOCAL char *PAR_qbfsolver=NULL;
LOCAL char *PAR_ifn=NULL;
/********************************************************************/
GLOBAL char **ORDv1=NULL;
GLOBAL char **ORDv2=NULL;
GLOBAL int *ORDv0=NULL;
/********************************************************************/
#define help() \
fprintf(stdout,"%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",\
"-p                : Prints the list of the properties in the model",\
"-ck i             : Checks the i-th property (default: -ck 1)",\
"-ck tt            : Checks whether it is a total transition relation",\
"-help (-h) (-?)   : Prints the command line help",\
"-v varfile        : Uses the variable ordering from file varfile",\
"-ignore fairness  : Ignore Fairness Specifications",\
"-bs               : Uses bsmc (qbf-based/sat-based bounded semantics)",\
"-QBF              : Uses bsmc (qbf-based bounded semantics)",\
"-SAT              : Uses bsmc (sat-based bsmc for ACTL properties)",\
"-bound k          : Sets the bound variable, used by bsmc",\
"-qbfsolver solver : Sets the qbfsolver variable, used by bsmc",\
"-satsolver solver : Sets the satsolver variable, used by bsmc",\
"\n")
/********************************************************************/
LOCAL char *mainparameters(int argc, char *argv[])
{
	int i,temp;

	if (argc==1) return NULL;
  	for(i=1;i<=argc-2;i++)
	if (argv[i][0]=='-') {
      		temp=atoi(argv[i]+2);
      		if      (argv[i][1]=='T') PAR_tm =temp*60;
      		else if (argv[i][1]=='V') PAR_verify =temp;
      		else if (argv[i][1]=='M') PAR_m =temp;
      		else if (argv[i][1]=='O') PAR_option =temp;
		else i+=param(argv,i);
	}
	if (argv[i][0]=='-') { i+=param(argv,i); i++; }
	if (PAR_bmc) getsolvers(); return argv[i];
}
/********************************************************************/
int param(char **argv, int i)
{
	int k=0;
	if (!strcmp(argv[i]+1,"p")) { PAR_p=1; return 0; }
	if (!strcmp(argv[i]+1,"ck")) { 
	   if (!strcmp(argv[i+1],"tt")) { PAR_ck=-1; return 1; }
	   if (!strcmp(argv[i+1],"df")) { PAR_ck=-2; return 1; }
	   if (isdigit(*argv[i+1])) { PAR_ck=atoi(argv[i+1]);  return 1; }
	} 
	if (!strcmp(argv[i]+1,"ignore")) { 
	   if (!strcmp(argv[i+1],"fairness")) { PAR_ifness=1; return 1; }
	} 
	if (!strcmp(argv[i]+1,"?")) { help(); exit(0);}
	if (!strcmp(argv[i]+1,"h")) { help(); exit(0);}
	if (!strcmp(argv[i]+1,"help")) { help(); return 0;}
	if (!strcmp(argv[i]+1,"v")) { PAR_v=argv[i+1]; return 1; }
        if (!strcmp(argv[i]+1,"tbd")) { PAR_bmc=0; return 0; }
        if (!strcmp(argv[i]+1,"QBF")) { PAR_bmc=1; return 0; }
        if (!strcmp(argv[i]+1,"SAT")) { PAR_bmc=2; return 0; }
        if (!strcmp(argv[i]+1,"bound")) {
                PAR_bound=atoi(argv[i+1]);
                PAR_bmc=max(1,PAR_bmc);
                return 1;
        }
        if (!strcmp(argv[i]+1,"bs")) { 
                PAR_bmc=3;
                return 0;
        }
        if (!strcmp(argv[i]+1,"satsolver")) { 
                PAR_satsolver=argv[i+1];
                PAR_bmc=max(2,PAR_bmc);
                return 1;
        }
        if (!strcmp(argv[i]+1,"qbfsolver")) { 
                PAR_qbfsolver=argv[i+1]; 
                PAR_bmc=max(1,PAR_bmc);
                return 1;
        }
        fprintf(stdout,"warning: invalid option, ignored\n"); return 0;
}
/********************************************************************/
void checkfiles()
{
	if (PAR_v!=NULL&&!fileok(PAR_v)) exit(0);
	if (PAR_ifn!=NULL&&!fileok(PAR_ifn)) exit(0);
}
/********************************************************************/
int param_p(tree *plist)
{
	char c;
	tree *t;
	int k=0,i=0;

        while ((t=listgetone(plist,i++))!=NULL) {
		fprintf(stdout,"%i: ",i); treewrite(t,stdout); 
		fprintf(stdout,"\n");
	}
	if (i==1) { fprintf(stdout,"%s\n",err_msg1); return 0; }
       	fprintf(stdout,"select a property (1-%i): ",i-1);
	while ((c=getc(stdin))&&isdigit(c)) k=k*10+c-'0';
	if (!(k>=1&&k<i)) { fprintf(stdout,"%s\n",err_msg2); return 0; }
	time(&t0); PAR_ck=k; return 1;
}
/********************************************************************/
int message_ck(tree *plist)
{
	int i=1;
	tree *p;

	if (PAR_p==1) if (param_p(plist)==0) return 0;

	fprintf(stdout,"PROPERTY:   "); 
	switch (-PAR_ck) {
	    case OTT: fprintf(stdout,"total transition relation\n"); break;
	    case ODF: fprintf(stdout,"deadlock-freeness\n"); break;
	    default:  p=listget1n(plist,PAR_ck);
		      if (p==NULL) { fprintf(stdout,"%s\n",err_msg3); return 0;}
		      treewrite(p,stdout); fprintf(stdout,"\n"); break;
	}
	return 1;
}
/********************************************************************/
void var_order(tree *vlist,tree *vlist_old,int n)
{
	FILE *g=fopen(PAR_v,"r");
	char line[1000],vn[1000];
	tree *v;
	int low,high;
	int i,j;
	int internal=1;

        ORDv1=(char **)malloc((2*n+1)*sizeof(char *));
        ORDv2=(char **)malloc((2*n+1)*sizeof(char *));
        ORDv0=(int *)malloc((2*n+1)*sizeof(int));

        for (i=0;i<2*n;i++) ORDv1[i]=(char *)malloc(ORDM*sizeof(char));
        for (i=0;i<2*n;i++) ORDv2[i]=(char *)malloc(ORDM*sizeof(char));
        ORDv1[2*n]=NULL; ORDv2[2*n]=NULL;

	i=0;
	while (1) {
		if (internal>0) {
			internal--; 
			v=treesearchvarlist("p_id_",0,vlist);
			if (v==NULL) { continue; }
		} else {
			fgets(line,1000,g); if (feof(g)) { fclose(g); break; }
			var_order_vn(line,vn,vlist_old);
			v=treesearchvarlist(vn,0,vlist); 
			if (v==NULL) { infos(vn); error("varfile 0"); }
		} 
		low=(int)atoi(v->left->left->node); 
		high=(int)atoi(v->left->right->node);
/*
		i=2*(low-1);
*/
/*
		for (j=i+high-low+1;low<=high;low++) {
        		sprintf(ORDv1[i],"%i",low);
        		sprintf(ORDv1[j],"%i",n+low);
			sprintf(ORDv2[i],"%i",n+low);
			sprintf(ORDv2[j],"%i",low);
        		ORDv0[low]=++i;
			ORDv0[n+low]=++j;
		} i+=(j-i);
*/
		for (;low<=high;low++) {
        		sprintf(ORDv1[i],"%i",low);
        		sprintf(ORDv1[i+1],"%i",n+low);
			sprintf(ORDv2[i],"%i",n+low);
			sprintf(ORDv2[i+1],"%i",low);
        		ORDv0[low]=i+1;
			ORDv0[n+low]=i+1+1;
			i+=2;
		}
	}
/*
 infos("SS"); for (i=1;i<=2*n;i++) { infos(ORDv1[i-1]); }
 infos("SS"); for (i=1;i<=2*n;i++) { infos(ORDv2[i-1]); }
 infos("SS"); for (i=1;i<=2*n;i++) { infoi(ORDv0[i]); }
*/
}
void var_order_vn(char *l1,char *l2,tree *vlist_old)
{
        int i=0;
        char *l0=l1;
        char *l2a=l2;
        char *l1a,*l0a;
        int a0=0;
        int a1=1;
        tree *v;

        while (*l1&&*l1!='\n'&&*l1!='[') { *l2++=*l1++; i++; }
        if (*l1==0||*l1=='\n') { *l2=0; return; }
        v=treesearchvarlist(l0,i+1,vlist_old);
        if (v==NULL&&var_order_process(l0,l2a)) {
                return var_order_vn(l0,l2a,vlist_old);
        }
        if (v==NULL) { infos(l0); error("varfile 1"); }
        l0=v->left->node;
        l1a=l1; l0a=l0; while (*l1) { l1++; } while (*l0) { l0++; }
        while (l1>l1a&&*l1!='[') { l1--; }

	while (1) {
		a0+=atoi(l1+1)*a1; l1--; while (l1>l1a&&*l1!='[') { l1--; }
		if (*l1!='[') { sprintf(l2,"_%i",a0); return; }
		while (l0>l0a&&*l0!='.') { l0--; } a1*=atoi(l0+1)+1; l0-=2;
	}
}
/********************************************************************/
int var_order_process(char *l1,char *l2)
{
        char *l1a=l1;
        char *l2a=l2;

        for (l1a=l1;*l1a&&*l1a!='[';l1a++); if (*l1a==0) return 0;
        for (l1a=l1;*l1a!='[';) *l2a++=*l1a++; strcpy(l2a,"_x_");
        for (l2a+=3,l1a++;*l1a!=']';) *l2a++=*l1a++;
        for (l1a++;*l1a!=0;) *l2a++=*l1a++; *l2a=0; strcpy(l1,l2); return 1;
}
/********************************************************************/
void getsolvers()
{
	if (PAR_satsolver==NULL) {
		PAR_satsolver=(char *)calloc(256,sizeof(char));
#ifdef SATSOLVER
		strcpy(PAR_satsolver,SATSOLVER);
#endif
	}
	if (PAR_qbfsolver==NULL) {
		PAR_qbfsolver=(char *)calloc(256,sizeof(char));
#ifdef QBFSOLVER
		strcpy(PAR_qbfsolver,QBFSOLVER);
#endif
	}
}
/********************************************************************/
int report(int start,char *fn,tree *p,int r,int k)
{
	int bv;

	if (start) {
        	printf("FILE:       %s\n",fn);
        	if (message_ck(p)==0) return 0;
		return 1;
	} else {
		bv=(int)difftime(time(&t1),t0);
/*
		if (PAR_bmc) 
        	printf("TIME (verds)	= %f\n",(float)utiltime(1));
*/
        	if (r==1) printf("CONCLUSION: TRUE");
        	if (r==-1) printf("CONCLUSION: FALSE");
        	if (r==0) { printf("CONCLUSION: UNKNOWN"); k--; }
		if (PAR_bmc) printf(" (time=%i bound=%i)\n",bv,k); else 
			     printf(" (time=%i)\n",bv);
		return r;
	}
}
/********************************************************************/

/*******************************************************************/
int SOLVEapplyq(char *solver,qmdata *qm,qinfo *qi,int bv)
{
        FILE *g;
        int r;

        if (*solver==0) r=QBFapply(qi,qm); 
	else {
                g=fopen(tmp0file,"w"); 
		qmdwrite(qm,qi,g); fclose(g); freeqmdata(qm); free(qi);
                r=SOLVEapply_0(solver); 
		if (r==-1) return 0;
        }

	if (bv&r==1) return 1;
	if (!bv&r==1) return -1;
        return 0;
}
/*******************************************************************/
int qm2file(qmdata *qm,qinfo *qi,int bv,int k)
{
        FILE *f;
	char fn[100];
	char *f1;

	sprintf(fn,"qbf.%i%2i",bv,k); 
	f1=fn; for (;*f1;f1++) if (*f1==' ') *f1='0'; 
	f=fopen(fn,"w"); qmdwrite(qm,qi,f); fprintf(f,"\n"); fclose(f);
}

/*******************************************************************/
#define tt(t,i)	if (i&t) return (128)|t; else return i|t;
/*******************************************************************/
char TLFnnftype(tree *fml)
{
	int t;
	if (fml==NULL) return 0;
	t=TLFnnftype(fml->left)|TLFnnftype(fml->right); 
	switch (fml->node[0]) {
	  case 'A': tt(t,1);
	  case 'E': tt(t,2);
	  case 'F': tt(t,4);
	  case 'G': tt(t,8);
	  case 'R': tt(t,16);
	  case 'U': tt(t,32);
	  case 'X': tt(t,64);
	}
	return t;
}
/*******************************************************************/
tree *TLFtype1(tree *fml)
{
	char a=TLFnnftype(fml);

	if (a&0xf6) return NULL;
	if (fml->node[0]=='A') fml=fml->right;
	if (fml->node[0]=='G') return treecopy(fml->right); else return NULL;
}
/*******************************************************************/
tree *TLFtype2(tree *fml)
{
	char a=TLFnnftype(fml);

	if (a&0xfa) return NULL;
	if (fml->node[0]=='A') fml=fml->right;
	if (fml->node[0]=='F') return treecopy(fml->right); else return NULL;
}

/*******************************************************************/
tree *TLFtype3(tree *fml)
{
	char a=TLFnnftype(fml);

	if ((a&0x09)==0) return NULL;
	if (fml->node[0]=='A') fml=fml->right;
	if (fml->node[0]=='G') return treecopy(fml->right); else return NULL;
}
/*******************************************************************/
int isTLFtypeAEorE(tree *fml)
{
	char a=TLFnnftype(fml);
	if ((a&0x3)==0x3||(a&0x2)==0x2) return 1; return 0;
}
/*******************************************************************/
