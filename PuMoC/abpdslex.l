%{

#include <string.h>
#define YY_NO_UNPUT

%}

%option noyywrap

%%

"Accepting" return ACCEPTING; 
"ACCEPTING"  return ACCEPTING; 
"accepting" return ACCEPTING; 
"true"  { abpdslval.name = name_create(abpdstext); return T_TRUE; } 
"TRUE"  { abpdslval.name = name_create(abpdstext); return T_TRUE; } 
"True"   { abpdslval.name = name_create(abpdstext); return T_TRUE; } 

[A-Za-z_][A-Za-z0-9_]* {
			abpdslval.name = name_create(abpdstext);
			return IDENT;
		}

"-->"		return ARROW; 
"&"		 return ABPDS_AND; 

[][<>,;\(\)]	{  return abpdstext[0]; }

[{}\%#](.)*\n	{ abpdslineno++; }
[ 	]	;
\n		{ abpdslineno++; }

