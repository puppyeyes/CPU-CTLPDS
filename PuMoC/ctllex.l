%{

#include <string.h>
#include "name.h"
#define YY_NO_UNPUT

extern int mc_ctl_read_from_file;
const char *ctlstr;


#define YY_INPUT(buf,result,max_size) \
	if ( YY_CURRENT_BUFFER_LVALUE->yy_is_interactive ) \
	{ \
		int c = '*'; \
		int n; \
		if (mc_ctl_read_from_file) \
		{	\
				for ( n = 0; n < max_size && (c = getc( ctlin )) != EOF && c != '\n'; ++n ) \
				buf[n] = (char) c; \
		} \
		else \
		{ \
				for ( n = 0; n < max_size && (c = *ctlstr) && (ctlstr += !!*ctlstr) && c != '\n'; ++n ) \
				buf[n] = (char) c; \
		} \
		if ( c == '\n' ) buf[n++] = (char) c; \
		if ( c == EOF && ferror( ctlin ) ) YY_FATAL_ERROR( "input in flex scanner failed" ); \
		result = n; \
	} \
	else \
	{ \
		errno=0; \
		while ( (result = fread(buf, 1, max_size, ctlin))==0 && ferror(ctlin)) \
		{ \
			if( errno != EINTR) \
			{ \
				YY_FATAL_ERROR( "input in flex scanner failed" ); \
				break; \
			} \
			errno=0; \
			clearerr(ctlin); \
		} \
	} \

%}

%option noyywrap
%X comment
%X regularexpr


%%

"!"		   return T_NOT; 
"||"		 return T_OR; 
"&&"		 return T_AND; 
"->"		 return T_IMP; 
"<->"		 return T_IFF;
"EX"     return T_EX;
"AX"     return T_AX;
"EF"     return T_EF;
"AF"     return T_AF;
"EG"  	 return T_EG;
"AG"		 return T_AG;
"E"			 return T_E ;
"A"			 return T_A;
"U"      return T_U;
"W"      return T_W ;
"true"	 return T_TRUEEXP;
"TRUE"   return T_TRUEEXP;
"True"   return T_TRUEEXP;
"false"	 return T_FALSEEXP;
"FALSE"	 return T_FALSEEXP;
"False"	 return T_FALSEEXP;
"["      return T_LB;
"]"      return T_RB;
"'"      return T_SQ;

[A-Za-z0-9_:\$]+	{
			ctllval.name= name_create(ctltext);
			return T_ATOM;
		}

"\""[^ }\n]+"\"" {
			ctllval.name = name_create(ctltext);
			return T_ATOM;
		}
   
[(),]		{ return *ctltext; }	  


[\#]  BEGIN(regularexpr);

<regularexpr>[A-Za-z0-9_(\.][A-Za-z0-9_:()\$ \.\|\+\*]* {
			ctllval.string = ctltext;
			return T_Regexpr; }
			
<regularexpr>[\#] BEGIN(INITIAL); 


"/*"  BEGIN(comment);
<comment>[^*\n]*  ;
<comment>"*"+[^*/\n]* ;
<comment>\n ctllineno++ ;
<comment>"*/" BEGIN(INITIAL); 

[\t\n]	;

.		;
%%