tree *cleanbool(tree *t);
tree *constructliteral(int i,int pos);
tree *constructbool(int start,int j,int k);
void replaceconstants(tree *t) ;
tree *replacevariable_eq(int a1,int b1,int a2,int b2);
void replacevariable(tree *t,tree *vlist,int bn) ;
