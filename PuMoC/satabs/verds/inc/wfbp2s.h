model *bp2s(tree *bp,tree *vconstr,tree *bs,int booln,int k,int bv,int opt);
model *bp2s0(tree *bp,tree *bs,int booln,int k,int bv,int opt);
model *bp2sLTL(tree *bp,tree *bs,int booln,int k,int bv,int opt);
model *bp2sCTL(tree *bp,tree *bs,int booln,int k,int bv,int opt);
model *bp2sADDconstr(model *mo,tree *bp,tree *varconstr,int booln,int k);
tree *bpLTLspec2s(tree *bp,tree *bs,int booln,int k,int bv,int opt);
model *CNFnonrepetition(tree *init,tree *trans,int bn,int k,int auxp);
model *CNFsys2model(tree *init,tree *trans,int bn,int k,int npath);
model *CNFspec2model(tree *spec,int auxp);
model *CNFconcatmodel(model *m1,model *m2);
void CNFaddstring(int *mstring,int mstringc,model *mo);
tree *crspec(workspace *ws2,int pp);
tree *agTRUE();
tree *agagTRUE();
tree *afagTRUE();
tree *afFALSE();
tree *agCompose(tree *prop);
tree *SPECactl2f0(tree *t,int k,int pn);
tree *SPECactl2f1(tree *t,int k,int pn);
tree *eqpath(int pa,int k);
tree *makeform_ctl(tree *f(),tree *t,int k,int pn,int pa,int x,int y,int j);
tree *actl2f1a(tree *t,int k,int pn,int pa,int x,int y);
tree *actl2f0a(tree *t,int k,int pn,int pa,int x,int y);
int fkx(tree *t,int k);
int gkx(tree *t,int k);
tree *actl2f1aOO(tree *t,int k,int pn,int pa,int x,int y);
tree *SPECactl2f0CTLR(tree *t,int k,int pn);
tree *SPECactl2f1CTLR(tree *t,int k,int pn);
tree *eqpathj(int pa,int k,int j);
tree *CTLRactl2f1a(tree *t,int k,int pn,int pa,int x,int y);
tree *CTLRactl2f0a(tree *t,int k,int pn,int pa,int x,int y);
int CTLRfkx(tree *t,int k);
int CTLRgkx(tree *t,int k);
int CNFnnf2cnf_1(tree *t,char c,int s0,int s,int *mstring);
int CNFnnf2cnf_2(tree *t,char c,int s,int *mstring);
int CNFnnf2cnf(tree *t,char c,int s0,int auxp,int *mstring);
int *CNFrotate(int *s1,int *s2,int n,int a1,int a2,int b1,int b2,int bn,int au);
tree *SPECltl2f0(tree *t,int k,int lstart,int pn,tree *tr);
tree *SPECltl2f1(tree *t,int k,int i,int pn);
tree *makeform_ll(tree *f(),tree *t,int k,int i,int j,char c,int pn);
tree *makeform_l0(tree *f(),tree *t,int k,int i,int j,char c,int pn,int ll);
tree *ltl2f0b(tree *t,int k,int i,int pn,int ll,int j);
tree *SPECreplcolon(tree *t);
tree *SPECreplcolonneg(tree *t);
int SPECnpath(tree *spec,int bv,int k,int ctl_opt);
