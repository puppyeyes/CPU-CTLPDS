int O1prepare(model *mm,int low,int high,FILE *g,int stage);
void O1rmodelappend(model *mm,char *fn,int max,FILE *g);
void O1prepareliterals(tree *spec,int nstates,int booln);
int O1calculate1(mstruct *cc,int n);
int O3repformula(int type,int bv,char *solver,workspace *ws2,tree *sp,int k);
tree *axnonrep(tree *spec,int k);
tree *agnonrep(tree *spec,int k);
