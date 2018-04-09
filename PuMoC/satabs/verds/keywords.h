/****************************************/
/*
#define KW28 	"MODULE"
#define KW38 	"MODULE"
#define KW29 	"VVM"
#define KW39 	"MAIN"
/****************************************/
#define KW01	"TRUE"
#define KW02	"FALSE"
#define KW03 	"VAR"
#define KW04 	"INIT"
#define KW05	"TRANS"
#define KW06	"SPEC"
#define KW07 	"PROC"
#define KW08 	"FAIRNESS"
#define OP01 	"!="
#define OP02 	"<="
#define OP03 	">="
#define OP04 	":="
/****************************************/
#define KW11	"1=1"
#define KW12	"1=0"
#define KW13 	"   "
#define KW14 	"#"
#define KW15	"#"
#define KW16	"#"
#define KW17 	"#"
#define KW18 	"#1=0;"
#define OP11 	"B"
#define OP12 	"{"
#define OP13 	"}"
#define OP14 	"="
/****************************************/
#define kw(a,b) strcpy(cleft[cc],a); strcpy(cright[cc],b); cc++;
/****************************************/
#define kwall() \
	clen[cc]=4; kw(KW01,KW11); \
	clen[cc]=5; kw(KW02,KW12); \
	clen[cc]=3; kw(KW03,KW13); \
	clen[cc]=4; kw(KW04,KW14); \
	clen[cc]=5; kw(KW05,KW15); \
	clen[cc]=4; kw(KW06,KW16); \
	clen[cc]=4; kw(KW07,KW17); \
	clen[cc]=8; kw(KW08,KW18); \
	clen[cc]=2; kw(OP01,OP11); \
	clen[cc]=2; kw(OP02,OP12); \
	clen[cc]=2; kw(OP03,OP13); \
	clen[cc]=2; kw(OP04,OP14); \
/****************************************/
