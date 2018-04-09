%{

#include <string.h>

#include "pds.h"
#include "abpds.h"
#include "common.h"
#include "variables.h"
#include "reorder.h"
#include "ftree.h"
#include "expr.h"
#include "bdd.h"


//int pdslineno = 0;
int pds_p1, pds_y1, pds_p2, pds_y2, pds_y3;
int pp_current_label;
int length=0;
int mc_pds_line_num=0;
extern int pdslex();
extern FormulaTree ctlTree;

void pdserror (char*);

extern PushdownSystem mc_pds;
extern AlternatingBuchiPushdownSystem mc_abpds;
extern int mc_verbose;
extern int mc_restrict;
extern int mc_logic_lang_CTL;
extern NameTreeList mc_atoms;
extern Name sharp;
extern ValList abpdsValList;
extern int abpdsValNum;
extern int mc_CTPL;

%}

%union 
{
	char *string;
	int integer;
	NameStack stack;
	NameSet set;
	Name name;
	DdNode *bdd;
	DdNode **varptr;
	ident_t ident;
	identref_t identref;
	FormulaTree etree;
	block_t block;
	const_t quant;
	ValList valList;
}

%token <string> LABEL
%token <name> IDENT
%token <integer> NUMBER
%token <quant> QUANT
%token PDS_OR PDS_AND PDS_NOT
%token PDS_EQV PDS_XOR
%token PDS_PLUS PDS_MINUS
%token PDS_MULT PDS_DIV PDS_SHL
%token PDS_LE PDS_EQ PDS_NE PDS_GE
%token PDS_FORALL PDS_EXISTS
%token ARROW PRIME
%token GLOBAL LOCAL BOOL INT
%token DEFINE
%token ATOMS RANGE BEGIN_RULE BEGIN_PROPOSITION
%token CSQ CSEND CSINIT 


%type <stack> stacklist
%type <ident> globals varlist decl
%type <ident> boollist boolident
%type <ident> intlist intident
%type <ident> optdim
%type <name> optproc 
%type <block> locals
%type <set> symblist symbols
%type <string> label
%type <name> startident stackident  ctrl_state
%type <bdd> expression
%type <etree> expr intexpr boolvar
%type <etree> intterm indexvar
%type <name> varident  value
%type <identref> variable
%type <integer> comp_op
%type <integer> constexp 
%type <valList> valdecl 

%right ')'
%left PDS_EQV
%left PDS_XOR
%left PDS_OR
%left PDS_AND
%right PDS_NOT

%left PDS_PLUS PDS_MINUS
%left PDS_MULT PDS_DIV
%left PDS_SHL

%%

automaton: defines universal vardecl startconfig atoms transitionlist 
   | defines universal vardecl atoms startconfig transitionlist
   | defines universal vardecl atoms transitionlist
   | vardecl rules propositions
   {
       	if (!mc_logic_lang_CTL)  
      	{ 	
	    		mc_pds.start_stack= name_stack_empty();
	      	name_stack_push(&mc_pds.start_stack,sharp,fullset);	   	  
	    		mc_pds.start_state = name_create("csinit");
	    		mc_pds.start_bdd = fullset;
				}
      };
   
      
      
propositions: BEGIN_PROPOSITION  props ;

props:  | props prop ;

prop: CSQ IDENT CSQ IDENT {pp_current_label = $2;} IDENTS ';' 
    | CSEND IDENT CSEND IDENT {pp_current_label = 0;} IDENTS ';' 
    | CSINIT IDENT CSINIT IDENT {pp_current_label = 0;} IDENTS ';' 
    | CSQ IDENT CSQ IDENT  ';' 
    | CSEND IDENT CSEND IDENT  ';' 
    | CSINIT IDENT CSINIT IDENT ';' 
    | CSQ '#' CSQ '#'  ';' 
    | CSEND '#' CSEND '#'  ';' 
    | CSINIT '#' CSINIT '#' ';'     ;
    
IDENTS: IDENT 
		{
		 if(pp_current_label)
		 {
  		int propos = $1;
  		NameTreeList tmp = mc_atoms;
  
  		while (tmp)
  		{
  			if (tmp->name == propos && !(tmp->valList)) break;
  			tmp = tmp->next;
  		}
  		if (!tmp)
  		{
  			tmp = common_malloc(sizeof(struct NameTreeListStruct));
  			tmp->name = propos;
  			tmp->tree = NULL;
  			tmp->valList =NULL;
  			tmp->length = 0;
  			tmp->next = mc_atoms;
  			mc_atoms = tmp;
  			}
  		name_bal_insert(&(tmp->tree),pp_current_label);		 	
  		}
		}
	| 	IDENT PRIME IDENT PRIME IDENT
	{
	 if(pp_current_label)
		 {
			char* str=malloc( strlen(name_cstring($1)) + strlen(name_cstring($3)) + strlen(name_cstring($5))+3);
		 	sprintf(str, "%s'%s'%s",name_cstring($1),name_cstring($3),name_cstring($5));		
		 		 
  		int propos = name_create(str);
  		NameTreeList tmp = mc_atoms;
      free(str);
  		while (tmp)
  		{
  			if (tmp->name == propos && !(tmp->valList)) break;
  			tmp = tmp->next;
  		}
  		if (!tmp)
  		{
  			tmp = common_malloc(sizeof(struct NameTreeListStruct));
  			tmp->name = propos;
  			tmp->tree = NULL;
  			tmp->valList =NULL;
  			tmp->length = 0;
  			tmp->next = mc_atoms;
  			mc_atoms = tmp;
  			}
  		name_bal_insert(&(tmp->tree),pp_current_label);		 	
  		}
		}
	| IDENTS IDENT 
		{
		 if(pp_current_label)
		 {
  		int propos = $2;
  		NameTreeList tmp = mc_atoms;
  
  		while (tmp)
  		{
  			if (tmp->name == propos && !(tmp->valList)) break;
  			tmp = tmp->next;
  		}
  		if (!tmp)
  		{
  			tmp = common_malloc(sizeof(struct NameTreeListStruct));
  			tmp->name = propos;
  			tmp->tree = NULL;
  			tmp->valList =NULL;
  			tmp->length = 0;
  			tmp->next = mc_atoms;
  			mc_atoms = tmp;
  			}
  		name_bal_insert(&(tmp->tree),pp_current_label);		 	
  		}
		}
	| IDENTS IDENT PRIME IDENT PRIME IDENT
		{ 
		 if(pp_current_label)
		 {
		  char* str=malloc( strlen(name_cstring($2))+strlen(name_cstring($4))+strlen(name_cstring($6))+3);
		  sprintf(str, "%s'%s'%s",name_cstring($2),name_cstring($4),name_cstring($6));		 
  		int propos = name_create(str);
  		free(str);
  		NameTreeList tmp = mc_atoms;
  
  		while (tmp)
  		{
  			if (tmp->name == propos && !(tmp->valList)) break;
  			tmp = tmp->next;
  		}
  		if (!tmp)
  		{
  			tmp = common_malloc(sizeof(struct NameTreeListStruct));
  			tmp->name = propos;
  			tmp->tree = NULL;
  			tmp->valList =NULL;
  			tmp->length = 0;
  			tmp->next = mc_atoms;
  			mc_atoms = tmp;
  			}
  		name_bal_insert(&(tmp->tree),pp_current_label);		 	
  		}
		}		;
     
      
rules : BEGIN_RULE rule_list;

rule_list:      | rule_list rule;
       
rule : ctrl_state  IDENT  ARROW ctrl_state IDENT IDENT ';'
	  { 
		if((mc_logic_lang_CTL))
		{
			ABPDSInsertTransition(&mc_abpds,$1, ctlTree, $2, $4, $5, $6);
    	ABPDSInsert2StackList(&(mc_abpds.stackList), $2);
    	ABPDSInsert2StackList(&(mc_abpds.stackList), $5);
    	ABPDSInsert2StackList(&(mc_abpds.stackList), $6);
			ABPDSInsert2StateList(&(mc_abpds.ctrlList), $1);
			ABPDSInsert2StateList(&(mc_abpds.ctrlList), $4);	
		}
		else
		 	pds_insert_trans(&mc_pds, $1, $2, $4, $5, $6, fullset, 0, 0);	
	  }
	| ctrl_state  IDENT  ARROW ctrl_state IDENT ';'
	  { 
	  if(name_create("_")!=$5)
		{
  		if(mc_logic_lang_CTL)
  		{
  			ABPDSInsertTransition(&mc_abpds,$1, ctlTree, $2, $4, $5, 0);
      	ABPDSInsert2StackList(&(mc_abpds.stackList), $5);
  			ABPDSInsert2StackList(&(mc_abpds.stackList), $2);
    		ABPDSInsert2StateList(&(mc_abpds.ctrlList), $1);
  			ABPDSInsert2StateList(&(mc_abpds.ctrlList), $4);	      	
      }
      else               
        pds_insert_trans(&mc_pds, $1, $2, $4, $5, 0, fullset, 0, 0);	
  	}
  	else
  	{
  	  if(mc_logic_lang_CTL)
  	  {
  	    	ABPDSInsert2StackList(&(mc_abpds.stackList), $2);
    			ABPDSInsert2StateList(&(mc_abpds.ctrlList), $1);
  				ABPDSInsert2StateList(&(mc_abpds.ctrlList), $4);	
  		   	ABPDSInsertTransition(&mc_abpds,$1, ctlTree, $2, $4, 0, 0); 	
  		}
  		else pds_insert_trans(&mc_pds, $1, $2, $4, 0, 0, fullset, 0, 0);
  	}	

	}
	| ctrl_state  '#'  ARROW ctrl_state '#' ';'
	 {
	  if(mc_logic_lang_CTL)
	  {
   	ABPDSInsertTransition(&mc_abpds,$1, ctlTree, sharp, $4, sharp, 0); 		
    ABPDSInsert2StateList(&(mc_abpds.ctrlList), $1);
  	ABPDSInsert2StateList(&(mc_abpds.ctrlList), $4);	
  	}
  	else pds_insert_trans(&mc_pds, $1, sharp, $4, sharp, 0, fullset, 0, 0);
	}
	| ctrl_state  '#'  ARROW ctrl_state IDENT  '#' ';'
	 {
	  if(mc_logic_lang_CTL)
{   	ABPDSInsertTransition(&mc_abpds,$1, ctlTree, sharp, $4, $5, sharp); 		
    ABPDSInsert2StateList(&(mc_abpds.ctrlList), $1);
  	ABPDSInsert2StateList(&(mc_abpds.ctrlList), $4);
  	ABPDSInsert2StackList(&(mc_abpds.stackList), $5);	
  	}
  	else pds_insert_trans(&mc_pds, $1, sharp, $4, $5, 0, fullset, 0, 0);
	};
	
ctrl_state: IDENT {$$=$1;}
        | CSQ  {$$=name_create("csq");}
        | CSINIT  {$$=name_create("csinit");}
        | CSEND	 {$$=name_create("csend");};

defines	: defines DEFINE IDENT constexp
	  {
		const_t con = common_malloc(sizeof(struct const_element));
		con->name = $3;
		con->value = $4;
		con->next = constlist;
		constlist = con;
	  }
	| defines DEFINE NUMBER constexp
	| ;
	
universal	: 
  | RANGE vallist;

vallist : IDENT
  {
		abpdsValList = common_malloc(sizeof(struct ValueListStruct));
		abpdsValList->name =$1;
		abpdsValList->next = NULL;
		abpdsValNum++;
  }
	| IDENT ',' vallist  
	{
		ValList tmp;
		for(tmp=abpdsValList; tmp; tmp=tmp->next)
		{
			if(tmp->name==$1) break;
		}
		if(!tmp)
		{	
			tmp = common_malloc(sizeof(struct ValueListStruct));
			tmp->name =$1;
			tmp->next = abpdsValList;
			abpdsValNum++;
			abpdsValList = tmp;
		}
	}
valdecl : value
  {
  	length++;
		$$ = createVal($1,NULL);
  }
	| value ',' valdecl  
	{
		$$ = createVal($1,$3);
		length++;
	}

value :	IDENT
    {
      $$ =$1;
    }
    | NUMBER
    {
    	char* tmp =malloc(sizeof(int)+ 1);
      sprintf(tmp,"%d", $1);
      $$=name_create(tmp);
      free(tmp);
    }

vardecl	: globals locals
	  {
		block_t tmp;
		mc_blocks = common_malloc(sizeof(struct block_element));
		mc_blocks->procname = 0;
		mc_blocks->names = NULL;
		mc_blocks->idents = $1;
		mc_blocks->next = mc_maxblock = tmp = $2;

		global_vars = $1? $1->index + ($1->type? $1->type : 1)
					    * ($1->dim? $1->dim : 1) : 0;
		local_vars = 0;
		while (tmp)
		{
			ident_t id = tmp->idents;
			int x = id? id->index + (id->type? id->type : 1)
						* (id->dim? id->dim : 1) : 0;
			if (x>local_vars) { mc_maxblock = tmp; local_vars = x; }
			tmp = tmp->next;
		}

		for (tmp = mc_blocks; tmp; tmp = tmp->next)
		{
			ident_t id = tmp->idents, nl = NULL, tmpid;
			
			while (id)
			{
				tmpid = id->next;
				id->next = nl;
				nl = id;
				id = tmpid;
			}
			tmp->idents = nl;
		}

		pds_p1 = pds_y1 = pds_p2 = pds_y2 = pds_y3 = 0;
		idchain = NULL;
		init_cudd();

		for (tmp = mc_blocks; tmp; tmp = tmp->next)
			build_properties(tmp);
	  } ;

globals	: GLOBAL varlist { $$ = $2;   }
	|		 { $$ = NULL; } ;

locals	: { $$ = NULL; local_vars = 0; }
	| locals LOCAL optproc symblist varlist
	  {
		NameSet set;
		$$ = common_malloc(sizeof(struct block_element));
		$$->procname = $3;
		$$->names = $4;
		$$->idents = $5;
		$$->next = $1;
		for (set = $$->names; set; set = set->next)
			name_tree_insert(&mc_localtree,
				name_tree_mknode(set->name,0,$$));
		local_vars = 0;
	  } ;

optproc	: IDENT
	| { $$ = 0; } ;

symblist: '(' symbols ')'
	  { $$ = $2; } ;

symbols	: IDENT
	  {
		NameSet set = NULL;
		name_set_insert(&set,$1);
		$$ = set;
	  }
	| symbols ',' IDENT
	  {
		name_set_insert(&$1,$3);
		$$ = $1;
	  } ;

varlist	: decl
	| varlist decl 
	  {
		$$ = $2;
		while ($2->next) $2 = $2->next;
		$2->next = $1;
	  } ;

decl	: BOOL boollist ';'	{ $$ = $2; }
	| INT intlist ';'	{ $$ = $2; }  ;

boollist: boolident		  { ($$ = $1)->next = NULL; }
	| boollist ',' boolident  { ($$ = $3)->next = $1; } ;

boolident: IDENT optdim
	   {
		$$ = $2;
		$$->varname = $1;
		$$->type = 0;
		$$->index = local_vars;
		local_vars += $$->dim? $$->dim : 1;
	   } ;

intident: IDENT optdim '(' constexp ')'
	  {
		$$ = $2;
		$$->varname = $1;
		if ($4 <= 0) pdserror("positive integer expected");
		$$->index = local_vars;
		local_vars += ($$->type = $4) * ($$->dim? $$->dim : 1);
	  } ;

optdim	: {
		($$ = common_malloc(sizeof(struct ident_element)))->dim = 0; 
	  }
	| '[' constexp ']'
	  {
		if ($2 <= 0) pdserror("positive integer expected");
		$$ = common_malloc(sizeof(struct ident_element));
		$$->dim = $2; $$->lo = 0; $$->hi = $2-1;
	  }
	| '[' constexp ',' constexp ']'
	  {
		if ($2 < 0) pdserror("nonnegative integer expected");
		$$ = common_malloc(sizeof(struct ident_element));
		$$->dim = $4-$2+1; $$->lo = $2; $$->hi = $4;
	  } ;

intlist : intident		{ ($$ = $1)->next = NULL; }
	| intlist ',' intident	{ ($$ = $3)->next = $1; } ;


startconfig 
	: '(' startstate '<' stacklist '>' ')'
	  {
	  	if (mc_logic_lang_CTL)
	  	{
	  		mc_abpds.startStack = $4;
	  	}
	  	else
	  	{
				mc_pds.start_stack = $4;
			}
	  } 
	  | 
	  {
	    mc_pds.start_stack =0;
	    mc_abpds.startState=0;
	    mc_abpds.startStack=0;
	    mc_pds.start_state = 0; 
	  }
	  ;

startstate
	: startident
	  {
	  	if (mc_logic_lang_CTL)
	  	{
	  		mc_abpds.startState = $1;
	  	}
	  	else
	  	{
				mc_pds.start_state = $1;
				mc_pds.start_bdd = fullset;
	  	}
	  }
	| startident '(' expression ')'
	  {
	  	if (mc_logic_lang_CTL)
	  	{
	  		mc_abpds.startState = $1;
	  	}
	  	else
	  	{
				mc_pds.start_state = $1;
				mc_pds.start_bdd = $3;
			}
	  }
	  ;

startident: IDENT
	    {
	  if (mc_logic_lang_CTL) ABPDSInsert2StateList(&(mc_abpds.ctrlList), $1);
		pds_p1 = $$ = $1;
		pds_y1 = pds_p2 = pds_y2 = pds_y3 = 0;
	    } 
	  | CSINIT 
	  {pds_p1= $$=name_create("csinit");
		pds_y1 = pds_p2 = pds_y2 = pds_y3 = 0; ABPDSInsert2StateList(&(mc_abpds.ctrlList), $$);}
	  | CSQ
	  {pds_p1= $$=name_create("csq");
		pds_y1 = pds_p2 = pds_y2 = pds_y3 = 0; ABPDSInsert2StateList(&(mc_abpds.ctrlList), $$);}	  
	  | CSEND
	  {pds_p1= $$=name_create("csend");
		pds_y1 = pds_p2 = pds_y2 = pds_y3 = 0; ABPDSInsert2StateList(&(mc_abpds.ctrlList), $$);}	  
	  ;

stackident: IDENT
	    {
    if (mc_logic_lang_CTL) ABPDSInsert2StackList(&(mc_abpds.stackList), $1);
		pds_y1 = $$ = $1;
		pds_p1 = pds_p2 = pds_y2 = pds_y3 = 0;
	    } 
	  
	 | '#' 
	  {
	  pds_y1 = $$ = sharp;		
		pds_p1 = pds_p2 = pds_y2 = pds_y3 = 0;	    
	  };

stacklist
	: stackident 
	  {
	    $$ = name_stack_empty();
	    if (mc_logic_lang_CTL)  
	    {	
	    	name_stack_push(&$$,sharp,NULL);
	    	name_stack_push(&$$,$1,NULL);
	    }
	    else name_stack_push(&$$,$1,fullset);
	  }
	| stackident '(' expression ')'
	  {
	    $$ = name_stack_empty();
	  	if (mc_logic_lang_CTL)  name_stack_push(&$$,$1,NULL);
	  	else name_stack_push(&$$,$1,$3);
	  } 
	| stackident stacklist
	  {
	  	if (mc_logic_lang_CTL) name_stack_push(&$2,$1,NULL);
	  	else name_stack_push(&$2,$1,fullset);
	  	$$=$2;
	  }
	| stackident '(' expression ')' stacklist
	  {
	  	if (mc_logic_lang_CTL) name_stack_push(&$5,$1,NULL);
	  	else name_stack_push(&$5,$1,$3);
	  	$$=$5;
	  } 
	  ;

atoms	:
	| atoms atom ;

atom	: ATOMS IDENT { pp_current_label = $2; } proplist ;

proplist: propident
	| proplist ',' propident ;

propident: IDENT
	{
		int propos = $1;
		NameTreeList tmp = mc_atoms;

		while (tmp)
		{
			if (tmp->name == propos && !(tmp->valList)) break;
			tmp = tmp->next;
		}
		if (!tmp)
		{
			tmp = common_malloc(sizeof(struct NameTreeListStruct));
			tmp->name = propos;
			tmp->tree = NULL;
			tmp->valList =NULL;
			tmp->length = 0;
			tmp->next = mc_atoms;
			mc_atoms = tmp;
		}
		name_bal_insert(&(tmp->tree),pp_current_label);
	}
	| IDENT '(' valdecl ')'
	{
		if(!$3) pdserror("The value of atomic proposition with parentheses can no be empty!");
		int propos = $1;
		NameTreeList tmp = mc_atoms;

		while (tmp)
		{
			if (tmp->name == propos && compareVal(tmp->valList,$3))
			{
				deleteVal($3);
				break;
			}
			tmp = tmp->next;
		}
		if (!tmp)
		{
			tmp = common_malloc(sizeof(struct NameTreeListStruct));
			tmp->name = propos;
			tmp->tree = NULL;
			tmp->valList =$3;
			tmp->length = length;
			tmp->next = mc_atoms;
			mc_atoms = tmp;
		}
		length=0;
		name_bal_insert(&(tmp->tree),pp_current_label);
	};

transitionlist: 
	{ 
		if(mc_CTPL) init_CTL_BDD_Manager(); 
	}
	| transitionlist transition ;

transition
	: simp_trans_wrap label
	  {
	  	if (mc_logic_lang_CTL)
	  	{
	  		ABPDSInsertTransition(&mc_abpds,pds_p1,ctlTree, pds_y1, pds_p2, pds_y2, pds_y3);
	  	}
	  	else
	  	{
				pds_insert_trans(&mc_pds, pds_p1, pds_y1, pds_p2, pds_y2, pds_y3, fullset, $2, 0);
	  	}
	  	if ($2) free($2);
	  }
	| simp_trans_wrap label '(' expression ')'
	  {
	  	if (mc_logic_lang_CTL)
	  	{
	  		ABPDSInsertTransition(&mc_abpds,pds_p1, ctlTree, pds_y1, pds_p2, pds_y2, pds_y3);
	  	}
	  	else
	  	{
				pds_insert_trans(&mc_pds, pds_p1, pds_y1, pds_p2, pds_y2, pds_y3, $4, $2, 0);
				Cudd_RecursiveDeref(mc_mgr,$4);
			}
			if ($2) free($2);
	  } ;

simp_trans_wrap
	: simple_transition
	  {
		if (mc_verbose > 1) fprintf(stdout,"-## %s %s %s %s %s\n",
			name_cstring(pds_p1),name_cstring(pds_y1),
			name_cstring(pds_p2),name_cstring(pds_y2),
			name_cstring(pds_y3));
	  } ;

simple_transition
	: IDENT '<' IDENT '>' ARROW IDENT '<' IDENT IDENT '>'
	  { 
		pds_p1 = $1; pds_y1 = $3;
		pds_p2 = $6; pds_y2 = $8;
		pds_y3 = $9;
    if(mc_logic_lang_CTL)
    {
    	ABPDSInsert2StackList(&(mc_abpds.stackList), $3);
    	ABPDSInsert2StackList(&(mc_abpds.stackList), $8);
    	ABPDSInsert2StackList(&(mc_abpds.stackList), $9);
			ABPDSInsert2StateList(&(mc_abpds.ctrlList), $1);
			ABPDSInsert2StateList(&(mc_abpds.ctrlList), $6);		
	  }
	 }
	| IDENT '<' IDENT '>' ARROW IDENT '<' IDENT '>'
	  {
			pds_p1 = $1; pds_y1 = $3;
			pds_p2 = $6; pds_y2 = $8; pds_y3 = 0;
    if(mc_logic_lang_CTL)
    {
    	ABPDSInsert2StackList(&(mc_abpds.stackList), $3);
    	ABPDSInsert2StackList(&(mc_abpds.stackList), $8);
			ABPDSInsert2StateList(&(mc_abpds.ctrlList), $1);
			ABPDSInsert2StateList(&(mc_abpds.ctrlList), $6);	
	  }
	 }
	| IDENT '<' IDENT '>' ARROW IDENT '<' '>'
	  { 
		pds_p1 = $1; pds_y1 = $3;
		pds_p2 = $6; pds_y2 = pds_y3 = 0;
		if(mc_logic_lang_CTL)
		{ABPDSInsert2StackList(&(mc_abpds.stackList), $3);
    ABPDSInsert2StateList(&(mc_abpds.ctrlList), $1);
		ABPDSInsert2StateList(&(mc_abpds.ctrlList), $6);	
	  }} ;

label	: { $$ = NULL; }
	| LABEL ;

expr	: boolvar
	| intexpr
	| '(' expr ')'		{ $$ = $2; }
	| expr PDS_OR expr	{ $$ = etree_create(E_OR,0,$1,$3); }
	| expr PDS_AND expr	{ $$ = etree_create(E_AND,0,$1,$3); }
	| expr PDS_EQV expr	{ $$ = etree_create(E_EQV,0,$1,$3); }
	| expr PDS_XOR expr	{ $$ = etree_create(E_XOR,0,$1,$3); }
	| PDS_NOT expr { $$ = etree_create(E_NOT,0,$2,NULL); }
	| PDS_FORALL quant '(' constexp ',' constexp ')' expr
	  {
		quantlist->lower = $4;
		quantlist->upper = $6;
		$$ = etree_create(E_FORALL,(identref_t)quantlist,$8,NULL);
		quantlist = quantlist->next;
	  }
	| PDS_EXISTS quant '(' constexp ',' constexp ')' expr
	  {
		quantlist->lower = $4;
		quantlist->upper = $6;
		$$ = etree_create(E_EXISTS,(identref_t)quantlist,$8,NULL);
		quantlist = quantlist->next;
	  } ;

quant	: IDENT
	  {
		const_t q;
		q = common_malloc(sizeof(struct const_element));
		q->name = $1;
		q->next = quantlist;
		quantlist = q;
	  } ;

expression: expr
	  {
		$$ = build_expr($1);
		free_expr_tree($1);
	  } ;

boolvar	: variable
	  {
		if ($1->ident->type) pdserror("boolean variable expected");
		if ($1->ident->dim) pdserror("missing subscript");
		$$ = etree_create(E_BVAR,$1,NULL,NULL);
	  }
	| indexvar
	  {
		if ($1->idref->ident->type)
			pdserror("boolean variable expected");
		$$ = $1;
		$1->token = E_BVAR;
		$1->left = (FormulaTree) idchain;
		idchain = NULL;
	  } ;

indexvar: variable '[' intterm ']'
	  {
		if (!$1->ident->dim) pdserror("unexpected subscript");
		$$ = etree_create(0,$1,NULL,$3);
	  } ;

intexpr	: intterm comp_op intterm
	  {
		FormulaTree tree = etree_create($2,0,$1,$3);
		$$ = etree_create(E_IEXPR,0,idchain,tree);
		idchain = NULL;
	  } ;

intterm	: intterm PDS_PLUS intterm	{ $$ = etree_create(E_PLUS,0,$1,$3); }
	| intterm PDS_MINUS intterm	{ $$ = etree_create(E_MINUS,0,$1,$3); }
	| intterm PDS_MULT intterm	{ $$ = etree_create(E_MULT,0,$1,$3); }
	| intterm PDS_DIV intterm	{ $$ = etree_create(E_DIV,0,$1,$3); }
	| intterm PDS_SHL intterm	{ $$ = etree_create(E_SHL,0,$1,$3); }
	| variable
	  {
		if (!$1->ident->type) pdserror("integer variable expected");
		$1 = add_to_idchain($1);
		$$ = etree_create(E_IDENT,$1,NULL,NULL);
		if ($1->ident->dim) pdserror("missing subscript");
	  }
	| indexvar
	  {
		if (!$1->idref->ident->type)
			pdserror("integer variable expected");
		$$ = $1;
		$$->token = E_IDENT;
		$1->idref->sub = $1->right;
		$1->idref->next = idchain;
		idchain = $1->idref;
		$1->right = NULL;
	  }
	| QUANT	 { $$ = etree_create(E_QUANT,(identref_t)$1,NULL,NULL); }
	| NUMBER { $$ = etree_create(E_NUMBER,(identref_t)$1,NULL,NULL); }
	| '{' intterm '}' { $$ = $2; } ;

comp_op	: '<'	  { $$ = E_LT; }
	| PDS_LE  { $$ = E_LE; }
	| PDS_EQ  { $$ = E_EQ; }
	| PDS_NE  { $$ = E_NE; }
	| PDS_GE  { $$ = E_GE; }
	| '>'	  { $$ = E_GT; } ;

variable: varident			/* (x,y) -> (p',y') */
	  {				/* in init cfg: (x,y) -> (p,y) */
		if (pds_p1 * pds_y1 == 0)
		    $$ = find_ident(pds_y1, $1, pds_p1? pvars : NULL, yvars);
		else
		    $$ = find_ident(pds_y1, $1, ppvars, ypvars);
		if (!$$) pdserror("invalid identifier");
	  }
	| varident PRIME		/* (x',y') -> (p,y) */
	  {
		if (pds_p1 * pds_y1 == 0) pdserror("invalid identifier");
		$$ = find_ident(pds_y2, $1, pvars, yvars);
		if (!$$) pdserror("invalid identifier");
	  }
	| varident PRIME PRIME		/*   (y'') -> (y'') */
	  {
		$$ = find_ident(mc_restrict? 0 : pds_y3, $1, NULL, yppvars);
		if (!$$) pdserror("invalid identifier");
	  } ;

varident: IDENT ;

constexp: constexp PDS_PLUS constexp	{ $$ = $1 + $3; }
	| constexp PDS_MINUS constexp	{ $$ = $1 - $3; }
	| constexp PDS_MULT constexp	{ $$ = $1 * $3; }
	| constexp PDS_DIV constexp	{ $$ = $1 / $3; }
	| constexp PDS_SHL constexp	{ $$ = $1 << $3; }
	| '(' constexp ')'		{ $$ = $2; }
	| NUMBER			{ $$ = $1; } ;

%%

#include "pdslex.c"

void pdserror (char *info)
{ 
	common_error("syntax error in pushdown system: %s, line %d",
			info,pdslineno);
}

void parse_pds (char *filename)
{
	mc_pds = pds_empty();
	quantlist = NULL;
	mc_localtree = NULL;
	mc_atoms = NULL;

	if (!(pdsin = fopen(filename,"r")))
		common_error("modelfile %s not found", filename);

	if (pdsparse())
		common_error("parse errors encountered while reading pds: token: %s", pdstext);

	fclose(pdsin);
}

void build_abpds(char *filename)
{
  abpdsValList = NULL;
  abpdsValNum =0;
	quantlist = NULL;
	mc_localtree = NULL;
	mc_atoms = NULL;

  mc_abpds = ABPDSInitial();
  sharp=sharp? sharp: name_create("sharp");
  
	if (!(pdsin = fopen(filename,"r")))
		common_error("modelfile %s not found", filename);

	if (pdsparse())
		common_error("parse errors encountered while reading pds");
	fclose(pdsin);  
	 
//	if(pdslineno>50000) mc_pds_line_num++;
	
}

