%{

#include <string.h>
#include "abpds.h"
#include "common.h"

extern int abpdslex();
extern int innerStateCount;
extern int innerStackCount;
void abpdserror(char*);

extern AlternatingBuchiPushdownSystem mc_abpds;
Name trueState = 0;
extern Name sharp;
extern char* product_buffer;
//int abpdslineno=0;

%}

%union 
{
	NameStack stack;
	Name name;
	ABPDSAcceptingStateList state;
	ABPDSRightSideofTransition rightside;
}

%token <name> IDENT
%token ABPDS_AND
%token ARROW 
%token ACCEPTING
%token <name> T_TRUE

%type <stack> stacklist
%type <state> statelist
%type <rightside> RightSides RightSide
%right ')'
%left ABPDS_AND


%%

start : startconfig accepting transitionlist 
				| accepting startconfig transitionlist;




startconfig : 
   | '(' IDENT '<' stacklist '>' ')'
	  {
	  	mc_abpds.startState = $2;
	  	ABPDSInsert2StateList(&(mc_abpds.ctrlList), $2);
	  	mc_abpds.startStack = $4;
	  } ;

stacklist: 
  IDENT 
  	{ 
  		ABPDSInsert2StackList(&(mc_abpds.stackList), $1);
  		$$ = name_stack_empty(); 
  		name_stack_push(&$$,sharp,NULL);
  		name_stack_push(&$$,$1,NULL);
  	}
	| IDENT stacklist
	  {
	  	ABPDSInsert2StackList(&(mc_abpds.stackList), $1);
	  	name_stack_push(&$2,$1,NULL);
	  	$$=$2;
	  };

accepting : 
	| ACCEPTING statelist
	{
		mc_abpds.acceptingStateList = $2;
	};

statelist : IDENT
	{
		ABPDSAcceptingStateList tmp1 = NULL;
		ABPDSAppend2AcceptingStateList(&tmp1, $1, NULL);
		$$=tmp1;
	}
	| IDENT ',' statelist
	{
		ABPDSAppend2AcceptingStateList(&($3), $1, NULL);
		$$=$3;
	};
 

transitionlist:
	| transitionlist transition ;

transition : IDENT '<' IDENT '>' ARROW RightSides
	  { 
    ABPDSInsert2StackList(&(mc_abpds.stackList), $3);
		/* ABPDSInsert2StateList(&(mc_abpds.ctrlList), $1); */
	
		ABPDSTransition added_trans = ABPDSCreateTransition($1, EQUAL, $3, $6 );
		ABPDSAppend2TransList(mc_abpds.transList, added_trans);
		added_trans->next = mc_abpds.trans;
		mc_abpds.trans=added_trans;
	  }
	 | IDENT '<' IDENT '>' ARROW T_TRUE
	 {
			ABPDSRightSideofTransition tmp1 = ABPDSInitRightSideofTransition($6,0,0,0);
			ABPDSTransition added_trans = ABPDSCreateTransition($1, EQUAL, $3, tmp1 );
		  ABPDSAppend2TransList(mc_abpds.transList, added_trans);
			added_trans->next = mc_abpds.trans;
			mc_abpds.trans=added_trans;		  	  
		  if(!trueState) trueState = $6;
	 };
	  
RightSides : RightSide
    {
    	$$ = $1;
    };
   | RightSide ABPDS_AND RightSides
		{
			if ($1->stack1 == 0)
			{
				innerStackCount++;
				sprintf(product_buffer,"%s.%d", "epsilon",innerStackCount);
				Name innerName = name_create(product_buffer);	
			
				innerStateCount++;	
				sprintf(product_buffer,"%s.%d", name_cstring($1->state), innerStateCount);  
  			Name innerStateName = name_create(product_buffer);			
		
  			ABPDSRightSideofTransition tmp1 = ABPDSInitRightSideofTransition($1->state,0,0,0);
						
				ABPDSTransition abpdsTrans = ABPDSCreateTransition(innerStateName, EQUAL, innerName, tmp1);
				ABPDSAppend2TransList(mc_abpds.transList, abpdsTrans);				
				abpdsTrans->next = mc_abpds.trans;
				mc_abpds.trans=abpdsTrans;				
				$$=$1;  
				$1->stack1 = innerName;
				$1->next = $3; 
				$1->state =innerStateName;
			}
			else
			{
				if ($1->stack2 == 0)
				{
					$1->next =$3;
					$$ = $1;
				}
				else
				{ 
  				innerStateCount++;
  				sprintf(product_buffer,"%s.%d", name_cstring($1->state),innerStateCount);  	   
  				Name innerName = name_create(product_buffer);			
  	
  				ABPDSRightSideofTransition tmp1 =  ABPDSInitRightSideofTransition($1->state,0,$1->stack1,$1->stack2);
				 	ABPDSTransition abpdsTrans = ABPDSCreateTransition(innerName, EQUAL, $1->stack1, tmp1);
  				ABPDSAppend2TransList(mc_abpds.transList, abpdsTrans);
  				abpdsTrans->next = mc_abpds.trans;
					mc_abpds.trans=abpdsTrans;						
  				$1->state = innerName; 
  				$1->stack2 =0;
  				$1->next = $3;
  				$$ = $1;						
				}
			}
			if ($3->next==NULL)
			{
  			if ($3->stack1 == 0)
  			{
  				innerStackCount++;
  				sprintf(product_buffer,"%s.%d", "epsilon",innerStackCount);
  				Name innerName = name_create(product_buffer);	 
  				
  				innerStateCount++;	
  				sprintf(product_buffer,"%s.%d", name_cstring($3->state),innerStateCount);  
    			Name innerStateName = name_create(product_buffer);			
  				
    			ABPDSRightSideofTransition tmp1 =  ABPDSInitRightSideofTransition($3->state,0,0,0);  						
  				ABPDSTransition abpdsTrans = ABPDSCreateTransition(innerStateName, EQUAL,innerName, tmp1);
  				ABPDSAppend2TransList(mc_abpds.transList, abpdsTrans);				
					abpdsTrans->next = mc_abpds.trans;
					mc_abpds.trans=abpdsTrans;  				
  				$3->stack1 = innerName;
  				$3->state=innerStateName;  				
  			}
  			else
  			{
  				if ($3->stack2)
  				{
    				innerStateCount++;
    				sprintf(product_buffer,"%s.%d", name_cstring($1->state),innerStateCount);  	   
    				Name innerName = name_create(product_buffer);			
    		 
    				ABPDSRightSideofTransition tmp1 = ABPDSInitRightSideofTransition($3->state, 0, $3->stack1, $3->stack2);
  				 	ABPDSTransition abpdsTrans = ABPDSCreateTransition(innerName, EQUAL, $3->stack1, tmp1);
    				ABPDSAppend2TransList(mc_abpds.transList, abpdsTrans);
						abpdsTrans->next = mc_abpds.trans;
						mc_abpds.trans=abpdsTrans;    						
    				$3->state = innerName; 
    				$3->stack2 =0;						
  				}
  			}				
			}
		}; 

RightSide : IDENT '<' IDENT IDENT '>'
	  { 
	  /* ABPDSInsert2StateList(&(mc_abpds.ctrlList), $1);*/
    ABPDSInsert2StackList(&(mc_abpds.stackList), $3);
    ABPDSInsert2StackList(&(mc_abpds.stackList), $4);
		$$ =  ABPDSInitRightSideofTransition($1,0, $3, $4);
	  }
	| IDENT '<' IDENT '>'
	  {
	  /* ABPDSInsert2StateList(&(mc_abpds.ctrlList), $1);*/
    ABPDSInsert2StackList(&(mc_abpds.stackList), $3); 
		$$ = ABPDSInitRightSideofTransition($1,0, $3, 0);
	  }
	| IDENT '<' '>'
	  { 
	  /*ABPDSInsert2StateList(&(mc_abpds.ctrlList), $1);*/
	  
		$$ = ABPDSInitRightSideofTransition($1,0, 0, 0);
	  } ;

%%

#include "abpdslex.c"

void abpdserror(char *info)
{ 
	common_error("syntax error in Alternating Buchi pushdown system: %s, line %d",
			info,abpdslineno);
}

void parse_abpds(char *filename)
{
	mc_abpds = ABPDSInitial();

	if (!(abpdsin = fopen(filename,"r")))
		common_error("modelfile %s not found", filename);

	if (abpdsparse())
	{
		fclose(abpdsin);
		common_error("parse errors encountered while reading abpds");
	}
/*	abpdsin = fopen(filename,"r");
	abpdsparse();*/
	if(trueState)
	{
		ABPDSStackList tmp = NULL;
		for(tmp = mc_abpds.stackList; tmp; tmp = tmp->next)
		{
			ABPDSRightSideofTransition tmp1 = ABPDSInitRightSideofTransition(trueState,0, tmp->stack, 0);			
		 	ABPDSTransition abpdsTrans = ABPDSCreateTransition(trueState, EQUAL, tmp->stack, tmp1);
			ABPDSAppend2TransList(mc_abpds.transList, abpdsTrans);	
			abpdsTrans->next = mc_abpds.trans;
			mc_abpds.trans=abpdsTrans;					
		}
		ABPDSAppend2AcceptingStateList(&(mc_abpds.acceptingStateList), trueState, NULL);
	}

	fclose(abpdsin);
}


