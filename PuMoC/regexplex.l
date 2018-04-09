%{

#include <string.h>
#define YY_NO_UNPUT
#include "name.h"

const char *regexp;

#define YY_INPUT(buf,result,max_size) \
	if ( YY_CURRENT_BUFFER_LVALUE->yy_is_interactive ) \
	{ \
		int c = '*'; \
		int n; \
		for ( n = 0; n < max_size && (c = *regexp) && (regexp += !!*regexp) && c != '\n'; ++n ) \
		buf[n] = (char) c; \
		if ( c == '\n' ) buf[n++] = (char) c; \
		if ( c == EOF && ferror( regexpin ) ) YY_FATAL_ERROR( "input in flex scanner failed" ); \
		result = n; \
	} \
	else \
	{ \
		errno=0; \
		while ( (result = fread(buf, 1, max_size, regexpin))==0 && ferror(regexpin)) \
		{ \
			if( errno != EINTR) \
			{ \
				YY_FATAL_ERROR( "input in flex scanner failed" ); \
				break; \
			} \
			errno=0; \
			clearerr(regexpin); \
		} \
	} \

%}

%option noyywrap

%%

"*"		{ return STAR; }
"+"		{ return PLUS; }
"."		{ return DOT; }
"|"   { return CAT; }
"("   {return R_LB;}
")"   {return R_RB;}


[A-Za-z0-9_][A-Za-z0-9_:\$]* {
     
			regexplval.integer = name_create(regexptext);
			return NUMBER;
		}

\n		;
[ 	]	;

%%