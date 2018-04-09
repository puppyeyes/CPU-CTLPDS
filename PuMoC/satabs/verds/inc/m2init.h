#ifndef m2init_h
#define m2init_h
/***********************************************************************/
#define MAXVARLEN 0x00000040
/***********************************************************************/
typedef struct _tree {
        char node[MAXVARLEN];
        struct _tree *left;
        struct _tree *right;
} tree;
/***********************************************************************/
typedef struct _m2data {
        int n;
        int n0;
        int nr;
        TBD ***fw;
        TBD ***bw;
        TBD ***nfw;
        TBD ***nbw;
        int **vfw;
        int **vbw;
        int *m2oi;
        int *m2order;
        int *ltable;
} m2data;
/***********************************************************************/
#endif  
