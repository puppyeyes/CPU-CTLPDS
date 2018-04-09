%{

#include <string.h>

#include "nfa.h"
#include "common.h"
#include "name.h"

int regexplex();
void regexperror(char*);
void parse_regexp(char*);
//int pdslineno = 1;
NFA* mc_nfa =NULL;
%}

%union 
{
	int integer;
	NFA* nfa;
	char *string;
}

%token<integer> NUMBER
%token PLUS
%token DOT
%token CAT
%token STAR
%token R_LB R_RB

%right R_RB
%left PLUS
%left CAT
%left STAR

%type <nfa> term

%%

NonFinAuto : term
	{
		mc_nfa = $1;
	};

term : 	NUMBER
	{
		$$ = productnfaforNUMBER($1);
	}		
	| DOT
	{
		$$ = productnfaforDOT();
	}
	| R_LB term R_RB  
	{
		$$ = $2;
	}
	|	term PLUS term
	{
		$$ = productnfaforPlus($1,$3);
	}
	| term CAT term
	{
		$$ = productnfaforCancat($1,$3);
	}
	| term STAR
	{
		$$ = productnfaforStar($1);
	};	


%%
#include "regexplex.c"

void regexperror (char *info)
{ 
	common_error("syntax error in regular expression: %s.\n", info);
}

void parse_regexp(char *formula)
{
	mc_nfa = NULL;
	
  regexp = formula;
	regexpparse();
}


