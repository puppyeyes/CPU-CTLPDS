int TLFdualopr(char a);
tree *TLFnegate(tree *t);
tree *TLFnnf(tree *t);
tree *TLFnnfsimpATF(tree *t);
tree *TLFnnfsimp(tree *t);
tree *TLFnnfsimpall(tree *t);
int iscclause(tree *t) ;
tree *lcnfcombine(tree *t1,tree *t2);
int TLFlcnf(tree *t);
