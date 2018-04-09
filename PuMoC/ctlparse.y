%{

#include <stdio.h>
#include <stdlib.h>

#include "ftree.h"
#include "string.h"
#include "common.h"

int ctllex();
void ctlerror(char*);

FormulaTree pushNegation(FormulaTree);
void CTLPrintFormula(FormulaTree);
FormulaTree ctlTree=NULL;

FormulaTree* treeBuffer;
int treeBufferLength =0;
int treeBufferLengthMax =0;
int treeBufferStep =10;
int paramLength =0;
extern int mc_pds_line_num;
 
//int ctllineno=0;
extern int mc_ctl_read_from_file;
extern int mc_CTPL;
%}

%union 
{
	char*  string;
	FormulaTree  ftree;
	ParamList param;
	Name name;
}


%token <name> T_ATOM 
%token <string> T_Regexpr
%token T_EX T_AX T_EF T_AF T_EG T_AG T_E T_A T_U T_W T_FALSEEXP T_TRUEEXP T_LB T_RB T_SQ
%type <ftree> form
%type <param> PARAM parameters parameter

%right ')'
%left  T_IMP
%left  T_IFF
%left  T_EX T_AX T_EF T_AF T_EG T_AG T_E T_A T_U T_W 
%left  T_OR T_AND
%right  T_NOT


%%

start	: form	{ ctlTree = $1; } ;

form	: T_ATOM PARAM
	  {
	  	Name tmp = $1;
	   
    	int i =0;
    	for (;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_IDENT && treeBuffer[i]->ident == tmp )
				{
					if (compareParams((ParamList)(treeBuffer[i]->right),$2))
					{ 
						$$ = treeBuffer[i];
						break;
					}
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				$$ = ftree_create(F_IDENT, tmp, (FormulaTree)paramLength, (FormulaTree)$2);
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = $$;
			}
	  }
  | T_Regexpr
 	  {
	  	Name tmp = name_create($1);
    	int i =0;
    	for (;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_IDENT)
				{
					if (treeBuffer[i]->ident == tmp)
					{ 
						$$ = treeBuffer[i];
						break;
					}
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				$$ = ftree_create(F_IDENT,tmp,NULL,(FormulaTree)1);
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = $$;
			}
	  }	  
   | T_TRUEEXP	
		{	
    	int i =0;
    	for (;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_TRUEEXP)
				{
					$$ = treeBuffer[i];
					treeBuffer[i]->ident++;
					break;
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				$$ = ftree_create(F_TRUEEXP,1,NULL,NULL);  
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = $$;
			}
	  } 
	| T_FALSEEXP			
		{	
    	int i =0;
    	for (;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_FALSEEXP)
				{
					$$ = treeBuffer[i];
					treeBuffer[i]->ident++;
					break;
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				$$ = ftree_create(F_FALSEEXP,1,NULL,NULL);  
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = $$;
			}
	  }	
	| '(' form ')'
		{ $$=$2; }

	| T_NOT form		
		{	
    	int i =0;
    	for (;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_NOT)
				{
					if ($2 == treeBuffer[i]->left)
					{
					  $$ = treeBuffer[i];
					  treeBuffer[i]->ident++;
					  if ($2->token!=F_IDENT) $2->ident--;
					  break;
					}
				}    	
    	}
    	if (i == treeBufferLength)
    	{
    	  if ($2->token == F_IDENT)	
    	  {
    	  	$$ = ftree_create(F_NOT,1,$2,NULL);
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = $$;  	  
    	  }	
    	  else 
    	  {
     	  	$$ = pushNegation(ftree_create(F_NOT,1,$2,NULL));
    	  }	
			}
	  }			
	| form T_AND form	
		{	
    	int i =0;
    	for (;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_AND)
				{
					if (($1 == treeBuffer[i]->left && $3 == treeBuffer[i]->right) ||($3 == treeBuffer[i]->left && $1 == treeBuffer[i]->right))
					{
					  $$ = treeBuffer[i];
					  treeBuffer[i]->ident++;
					  if ($1->token!=F_IDENT) $1->ident--;
					  if ($3->token!=F_IDENT) $3->ident--;					  
					  break;
					}
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				$$  = ftree_create(F_AND,1,$1,$3);
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = $$;
			}
	  }		
	| form T_OR form	
		{	
    	int i =0;
    	for (;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_OR)
				{
					if (($1 == treeBuffer[i]->left && $3 == treeBuffer[i]->right) ||($3 == treeBuffer[i]->left && $1 == treeBuffer[i]->right))
					{
						treeBuffer[i]->ident++;
					  $$ = treeBuffer[i];
					  if ($1->token!=F_IDENT) $1->ident--;
					  if ($3->token!=F_IDENT) $3->ident--;						  
					  break;
					}
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				$$ = ftree_create(F_OR,1,$1,$3);
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = $$;
			}
	  }	
	| form T_IFF form	
	{  
		if($1->token!=F_IDENT) $1->ident--;
		if($3->token!=F_IDENT) $3->ident--;		
		FormulaTree ctlTree1 = NULL;
		FormulaTree ctlTree2 = NULL;
		FormulaTree ctlTree3 = NULL;
		FormulaTree ctlTree4 = NULL;
  	int i =0;
  	for (;i<treeBufferLength;i++)
  	{
  		if (treeBuffer[i] == NULL ) continue;
			if (treeBuffer[i]->token == F_AND)
			{
				if (($1 == treeBuffer[i]->left && $3 == treeBuffer[i]->right) ||($3 == treeBuffer[i]->left && $1 == treeBuffer[i]->right))
				{
					treeBuffer[i]->ident++;
				  ctlTree1 = treeBuffer[i];
				  break;
				}
			}    	
  	}
  	if (i == treeBufferLength)
  	{
			ctlTree1 =  ftree_create(F_AND,1,$1,$3);
			if($1->token!=F_IDENT) $1->ident++;
			if($3->token!=F_IDENT) $3->ident++;					
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = ctlTree1;
		}
		
		

  	for (i=0;i<treeBufferLength;i++)
  	{
  		if (treeBuffer[i] == NULL ) continue;
			if (treeBuffer[i]->token == F_NOT)
			{
				if ($1 == treeBuffer[i]->left)
				{
					treeBuffer[i]->ident++;
				  ctlTree2 = treeBuffer[i];
				  break;
				}
			} 
		}   	
  	if (i == treeBufferLength)
  	{
  		if($1->token ==F_IDENT ) 
  		{
  			ctlTree2 =  ftree_create(F_NOT,1,$1,NULL);
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = ctlTree2;  		
  		}
  		else
  		{
  			$1->ident++;
  			ctlTree2 =  pushNegation(ftree_create(F_NOT,1,$1,NULL));
  		}

		}
	
 
  	for (i =0 ;i<treeBufferLength;i++)
  	{
  		if (treeBuffer[i] == NULL ) continue;
			if (treeBuffer[i]->token == F_NOT)
			{
				if ($3 == treeBuffer[i]->left)
				{
					treeBuffer[i]->ident++;
				  ctlTree3 = treeBuffer[i];
				  break;
				}
			}    	
  	}
  	if (i == treeBufferLength)
  	{
  		if($3->token ==F_IDENT )
  		{
  			ctlTree3 =  ftree_create(F_NOT,1,$3,NULL);
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = ctlTree3;		
  		}
  		else
  		{
  			$3->ident++;
  			ctlTree3 =  pushNegation(ftree_create(F_NOT,1,$3,NULL));
  		}  		
		}
	
  	for (i =0;i<treeBufferLength;i++)
  	{
  		if (treeBuffer[i] == NULL ) continue;
			if (treeBuffer[i]->token == F_AND)
			{
				if ((ctlTree2 == treeBuffer[i]->left && ctlTree3 ==treeBuffer[i]->right)||(ctlTree3 == treeBuffer[i]->left && ctlTree2 ==treeBuffer[i]->right))
				{
					treeBuffer[i]->ident++;
				  ctlTree4 = treeBuffer[i];
				  if(ctlTree2->token!=F_IDENT) 	ctlTree2->ident--;
				  if(ctlTree3->token!=F_IDENT) 	ctlTree3->ident--;				  
				  break;
				}
			}    	
  	}
  	if (i == treeBufferLength)
  	{
			ctlTree4 =  ftree_create(F_AND,1,ctlTree2,ctlTree3);
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = ctlTree4;
		}	
  	
  	for (i=0;i<treeBufferLength;i++)
  	{
  		if (treeBuffer[i] == NULL ) continue;
			if (treeBuffer[i]->token == F_OR)
			{
				if ((ctlTree1 == treeBuffer[i]->left && ctlTree4== treeBuffer[i]->right) ||(ctlTree4 == treeBuffer[i]->left && ctlTree1== treeBuffer[i]->right))
				{
					treeBuffer[i]->ident++;
				  $$ = treeBuffer[i];
					ctlTree1->ident--;
					ctlTree4->ident--;
			  				  
				  break;
				}
			}    	
  	}
  	if (i == treeBufferLength)
  	{
			$$ = ftree_create(F_OR,1,ctlTree1,ctlTree4);	
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = $$;
		}			
	 }
	| form T_IMP form	
	{ 
		if($1->token!=F_IDENT) $1->ident--;	
		if($3->token!=F_IDENT) $3->ident--;	
		FormulaTree ctlTree1 = NULL;	
  	int i =0;
  	for (;i<treeBufferLength;i++)
  	{
  		if (treeBuffer[i] == NULL ) continue;
			if (treeBuffer[i]->token == F_NOT)
			{
				if ($1 == treeBuffer[i]->left )
				{
					treeBuffer[i]->ident++;
				  ctlTree1 = treeBuffer[i];
				  break;
				}
			}    	
  	}
  	if (i == treeBufferLength)
  	{	
  	  if($1->token == F_IDENT)
  	  { 
  	  	ctlTree1 =  ftree_create(F_NOT,1,$1,NULL);
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] =  ctlTree1;  	
  	  }
  		else	
  		{	
  			$1->ident++;	
  			ctlTree1 =  pushNegation(ftree_create(F_NOT,1,$1,NULL));
			}
			
			$$ = ftree_create(F_OR,1,ctlTree1,$3);	
			if($3->token!=F_IDENT) $3->ident++;	
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = $$;							
		}
		else
		{
    	for (i=0;i<treeBufferLength;i++)
    	{ 		
    		if (treeBuffer[i] == NULL ) continue;
  			if (treeBuffer[i]->token == F_OR)
  			{
  				if ((ctlTree1 == treeBuffer[i]->left && $3== treeBuffer[i]->right)||(ctlTree1 == treeBuffer[i]->right && $3== treeBuffer[i]->left))
  				{
  				  $$ = treeBuffer[i];
  				  treeBuffer[i]->ident++;
  				  ctlTree1->ident--;
  				  break;
  				}
  			}    	
    	}
    	if (i == treeBufferLength)
    	{
  			$$ = ftree_create(F_OR,1,ctlTree1,$3);	
  			if($3->token!=F_IDENT) $3->ident++;	
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = $$;
  		}				
		}
	}
	| T_E T_ATOM form
	{
		if(!mc_CTPL) mc_CTPL++;
		Name tmp = $2;
   	int i =0;
  	for (;i<treeBufferLength;i++)
  	{
  		if (treeBuffer[i] == NULL ) continue;
			if (treeBuffer[i]->token == F_E)
			{
				if (((ParamList)(treeBuffer[i]->right))->name == tmp)
				{
					if ($3 == treeBuffer[i]->left)
					{
				 		$$ = treeBuffer[i];
				 		treeBuffer[i]->ident++;
				 		break;
				 	}
				}    	
  		}
  	}
  	if (i == treeBufferLength)
  	{
			$$ = ftree_create(F_E,1,$3, (FormulaTree)createParam(tmp,0, NULL));
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = $$;
		}			
	}
	| T_A T_ATOM form
	{
		if(!mc_CTPL) mc_CTPL++;
		Name tmp =$2;
   	int i =0;
  	for (;i<treeBufferLength;i++)
  	{
  		if (treeBuffer[i] == NULL ) continue;
			if (treeBuffer[i]->token == F_A)
			{
				if (((ParamList)(treeBuffer[i]->right))->name == tmp)
				{
					if ($3 == treeBuffer[i]->left)
					{
				 		$$ = treeBuffer[i];
				 		treeBuffer[i]->ident++;
				 		break;
				 	}
				}    	
  		}
  	}
  	if (i == treeBufferLength)
  	{
			$$ = ftree_create(F_A,1,$3,(FormulaTree)createParam(tmp,0,NULL));
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = $$;
		}			
	}
	| T_EX form 
	{
   	int i =0;
  	for (;i<treeBufferLength;i++)
  	{
  		if (treeBuffer[i] == NULL ) continue;
			if (treeBuffer[i]->token == F_EX)
			{
				if ($2 == treeBuffer[i]->left )
				{
				  $$ = treeBuffer[i];
				  treeBuffer[i]->ident++;
				  if ($2->token!=F_IDENT) $2->ident--;
				  break;
				}
			}    	
  	}
  	if (i == treeBufferLength)
  	{
			$$ = ftree_create(F_EX,1,$2,NULL);
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = $$;
		}
	}		
  | T_AX form 
	{
   	int i =0;
  	for (;i<treeBufferLength;i++)
  	{
  		if (treeBuffer[i] == NULL ) continue;
			if (treeBuffer[i]->token == F_AX)
			{
				if ($2 == treeBuffer[i]->left )
				{
				  $$ = treeBuffer[i];
				  treeBuffer[i]->ident++;
				  if ($2->token!=F_IDENT) $2->ident--;
				  break;
				}
			}    	
  	}
  	if (i == treeBufferLength)
  	{
			$$ = ftree_create(F_AX,1,$2,NULL);
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = $$;
		}
	}
  | T_EF form 
  	{
  		FormulaTree ctlTree1 = NULL;
  	  int i;
    	for (i =0;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_TRUEEXP)
				{
					ctlTree1 = treeBuffer[i];
					ctlTree1->ident++;
					break;
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				ctlTree1 = ftree_create(F_TRUEEXP,1,NULL,NULL); 
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = ctlTree1;
			}
    	for (i=0;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_EU)
				{
					if (ctlTree1 ==treeBuffer[i]->left && $2 ==treeBuffer[i]->right)
					{
						$$ = treeBuffer[i];
						treeBuffer[i]->ident++;
						ctlTree1->ident--;
						if($2->token!=F_IDENT) $2->ident--;						
						break;
					}
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				$$ = ftree_create(F_EU,1,ctlTree1,$2);
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = $$;
			}			
  	}
  | T_AF form 
  	{
  		FormulaTree ctlTree1 = NULL;
  	  int i =0;
    	for (;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_TRUEEXP)
				{
					ctlTree1 = treeBuffer[i];
					ctlTree1->ident++;
					break;
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				ctlTree1 = ftree_create(F_TRUEEXP,1,NULL,NULL); 
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] =  ctlTree1;
			}
  	 
    	for (i=0;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_AU)
				{
					if (treeBuffer[i] == NULL ) continue;
					if (ctlTree1 ==treeBuffer[i]->left && $2 ==treeBuffer[i]->right)
					{
						$$ = treeBuffer[i];
						treeBuffer[i]->ident++;
						ctlTree1->ident--;
						if($2->token!=F_IDENT) $2->ident--;						
						break;
					}
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				$$ = ftree_create(F_AU,1,ctlTree1,$2);
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = $$;
			} 	  		
  	}
  | T_EG form 
  	{ 
  		FormulaTree ctlTree1 = NULL;
  	  int i =0;
    	for (;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_FALSEEXP)
				{
					ctlTree1 = treeBuffer[i];
					ctlTree1->ident++;
					break;
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				ctlTree1 = ftree_create(F_FALSEEXP,1,NULL,NULL); 
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = ctlTree1;
			}

    	for (i=0;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_EW)
				{
					if (ctlTree1 ==treeBuffer[i]->left && $2 ==treeBuffer[i]->right)
					{
						$$ = treeBuffer[i];
						$$->ident++;
						ctlTree1->ident--;
						if($2->token!=F_IDENT) $2->ident--;						
						break;
					}
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				$$ = ftree_create(F_EW,1,ctlTree1,$2); 
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = $$;
			} 	  		
  	}
  | T_AG form 
 	 {
  		FormulaTree ctlTree1 = NULL;
  	  int i =0;
    	for (;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_FALSEEXP)
				{
					ctlTree1 = treeBuffer[i];
					ctlTree1->ident++;
					break;
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				ctlTree1 = ftree_create(F_FALSEEXP,1,NULL,NULL); 
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = ctlTree1;
			}
 
    	for (i=0;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_AW)
				{
					if (ctlTree1 ==treeBuffer[i]->left && $2 ==treeBuffer[i]->right)
					{	
						$$ = treeBuffer[i];
						$$->ident++;
						ctlTree1->ident--;
						if($2->token!=F_IDENT) $2->ident--;						
						break;
					}
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				$$ = ftree_create(F_AW,1,ctlTree1,$2); 
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = $$;
			} 	  		
  	}
  | T_A T_LB form T_U form T_RB 
 	 {
  	  int i =0;
    	for (;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_AU)
				{
					if ($3 ==treeBuffer[i]->left && $5 ==treeBuffer[i]->right)
					{
						$$ = treeBuffer[i];
						treeBuffer[i]->ident++;
						if ($3->token!=F_IDENT) $3->ident--;
						if ($5->token!=F_IDENT) $5->ident--;						
						break;
					}
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				$$ = ftree_create(F_AU,1,$3,$5); 
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = $$;
			} 	  		
  	}  
  | T_E T_LB form T_U form T_RB
 	 {
  	  int i =0;
    	for (;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_EU)
				{
					if ($3 ==treeBuffer[i]->left && $5 ==treeBuffer[i]->right)
					{
						$$ = treeBuffer[i];
						treeBuffer[i]->ident++;
						if ($3->token!=F_IDENT) $3->ident--;
						if ($5->token!=F_IDENT) $5->ident--;							
						break;
					}
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				$$ = ftree_create(F_EU,1,$3,$5);
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = $$;
			} 	  		
  	} 
  | T_A T_LB form T_W form T_RB
 	 {
  	  int i =0;
    	for (;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_AW)
				{	
					if ($3 ==treeBuffer[i]->left && $5 ==treeBuffer[i]->right)
					{
						$$ = treeBuffer[i];
						treeBuffer[i]->ident++;
						if ($3->token!=F_IDENT) $3->ident--;
						if ($5->token!=F_IDENT) $5->ident--;							
						break;
					}
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				$$ = ftree_create(F_AW,1,$3,$5);
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = $$;
			} 	  		
  	} 
  | T_E T_LB form T_W form T_RB
 	 {
	  int i =0;
  	for (;i<treeBufferLength;i++)
  	{
  		if (treeBuffer[i] == NULL ) continue;
			if (treeBuffer[i]->token == F_EW)
			{
				if ($3 ==treeBuffer[i]->left && $5 ==treeBuffer[i]->right)
				{
					$$ = treeBuffer[i];
					treeBuffer[i]->ident++;
					if ($3->token!=F_IDENT) $3->ident--;
					if ($5->token!=F_IDENT) $5->ident--;						
					break;
				}
			}    	
  	}
  	if (i == treeBufferLength)
  	{
			$$ = ftree_create(F_EW,1,$3,$5); 
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = $$;
		} 	  		
  };
	  
PARAM: 
  { paramLength =0; $$ =NULL; }
  | '(' parameters ')'
  {
  	$$=$2;
  };
	  
  
parameters: parameter
	{
		paramLength=1;
		$$ = $1;
	}
	| parameter ',' parameters
	{
		paramLength++;
		$1->next = $3;
		$$=$1;
	};
parameter : T_ATOM
		{
			if(!mc_CTPL) mc_CTPL++;
			$$ = createParam($1,0, NULL);
		}
		| T_SQ T_ATOM
		{
			if(!mc_CTPL) mc_CTPL++;
			$$ = createParam($2,1, NULL);
		}

%%

#define YY_ALWAYS_INTERACTIVE 1
#include "ctllex.c"

void ctlerror (char *info)
{ 
	common_error("CTL syntax error in formula: %s", info);
}

void CTLPrintFormula(FormulaTree tree)
{
	char *op = NULL;
	char *aux1 = NULL;
	char *aux2 = NULL;
	if (!tree) return;
	switch (tree->token)
	{
	    case F_FALSEEXP:
		common_addbuf("false"); return;
	    case F_TRUEEXP: 
		common_addbuf("true"); return;
	    case F_NOT:	if (!op) op = "!";
	    case F_EX:	if (!op) op = "EX";
	    case F_AX:	if (!op) op = "AX";
		common_addbuf(op);
		common_addbuf("(");
		CTLPrintFormula(tree->left);
		common_addbuf(")");	    
	  return;
	    case F_A:   if (!op) op = "A ";
	    case F_E:   if (!op) op = "E ";
		common_addbuf(op);
		common_addbuf(name_cstring(((ParamList)(tree->right))->name));
		common_addbuf("(");
		CTLPrintFormula(tree->left);
		common_addbuf(")");	    
	  return;	    
	    case F_AND:	if (!op) op = "&&";
	    case F_OR:	if (!op) op = "||";
		common_addbuf("(");
		CTLPrintFormula(tree->left);
		common_addbuf(op);
		CTLPrintFormula(tree->right);
		common_addbuf(")");
		return;	    
	    case F_EU:	if (!aux1 && !aux2) { aux1 = "E[("; aux2 = ")U("; }
	    case F_AU:	if (!aux1 && !aux2) {mc_pds_line_num++; aux1 = "A[("; aux2 = ")U("; }
	    case F_EW:	if (!aux1 && !aux2) { aux1 = "E[("; aux2 = ")W("; }
	    case F_AW:	if (!aux1 && !aux2) {mc_pds_line_num++; aux1 = "A[("; aux2 = ")W("; }
				common_addbuf(aux1);
				CTLPrintFormula(tree->left);
				common_addbuf(aux2);
				CTLPrintFormula(tree->right);
				common_addbuf(")]");
				return;	    
	    case F_IDENT:
	    {
	      if(tree->right)
	      {
	        if(tree->left)
	        {
	          common_addbuf(name_cstring(tree->ident));
  	    		common_addbuf("(");
  	    		ParamList tmp =(ParamList)tree->right;
  	    		while(tmp->next)
  	    		{
  	    			if(tmp->type) common_addbuf("'");
  	    			common_addbuf(name_cstring(tmp->name));
  	    			common_addbuf(",");
  	    			tmp = tmp->next;
  	    		}
  	    		if(tmp->type) common_addbuf("'");
  	    		common_addbuf(name_cstring(tmp->name));
  	    		common_addbuf(")");		          
	        }
	        else
	        {
	    		  common_addbuf("$");	
					  common_addbuf(name_cstring(tree->ident));
					  common_addbuf("$");		          
	        }
	      }
	      else common_addbuf(name_cstring(tree->ident));			
				return;
			}
	    default:
				common_error("unknown token (%i)",tree->token);
		return;
	}
}

FormulaTree pushNegation(FormulaTree ftree)
{
	FormulaTree tmp1 = NULL;
	FormulaTree tmp2 = NULL;  
	char dualToken = 0;
	int i, j;
  switch(ftree->left->token)	
  {
		case F_FALSEEXP:	if (!dualToken) dualToken = F_TRUEEXP;
		case F_TRUEEXP:  if (!dualToken) dualToken = F_FALSEEXP;
    	for (i =0;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL) continue;
				if (treeBuffer[i]->token == dualToken)
				{			
					if (ftree->left->ident ==1 )
					{
						for (j =0;j<treeBufferLength;j++)
						{
							if (treeBuffer[j] == NULL) continue;
							if (treeBuffer[j]==ftree->left)
							{
								treeBuffer[j] =NULL;
								common_free(ftree->left,sizeof(FormulaTree));
								break;
							}
						}
					}
					else	ftree->left->ident--;
					ftree->left = NULL;
					common_free(ftree, sizeof(FormulaTree));
					treeBuffer[i]->ident++;
					return treeBuffer[i];
				} 
			}
			if (i == treeBufferLength)  	
			{
				ftree->token = dualToken;
				if (ftree->left->ident ==1 )
				{
					for (j=0;j<treeBufferLength;j++)
					{
						if (treeBuffer[j] == NULL) continue;
						if (treeBuffer[j] == ftree->left)
						{
							treeBuffer[j] =NULL;
							common_free(ftree->left,sizeof(FormulaTree));
							break;
						}
					}  				
				}
				else ftree->left->ident--;
				ftree->left = NULL;	
				ftree->ident =1;
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = ftree;	
				return ftree;
			}
		case F_IDENT: 
		{ 
		  for (j=0;j<treeBufferLength; j++)
		  {
		  	if (treeBuffer[j] ==NULL) continue;
		  	if (treeBuffer[j]->token == F_NOT)
		  	{
		  		if (ftree->left == treeBuffer[j]->left)
		  		{
		  			common_free(ftree,sizeof(FormulaTree));
		  			treeBuffer[j]->ident++;		  			
		  			return treeBuffer[j];
		  		}
		  	}
		  }
		  if (j == treeBufferLength)
		  {
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = ftree;
		  }
			return ftree;
		}
		case F_NOT: 
			tmp1 = ftree->left->left;

			if (ftree->left->ident == 1)
			{
				for (j=0;j<treeBufferLength;j++)
				{
					if (treeBuffer[j] ==NULL) continue;
					if (treeBuffer[j] == ftree->left)
					{
						treeBuffer[j] = NULL;				
						//common_free(ftree->left, sizeof(FormulaTree));
						ftree->left =NULL;
						break;
					}
				}
			}
			else	ftree->left->ident--;
			common_free(ftree, sizeof(FormulaTree));
	   	return tmp1;
	  case F_EX:  if (!dualToken)  dualToken=F_AX;
	  case F_AX:	if (!dualToken)  dualToken=F_EX;
	  	ftree->token = dualToken;
	  	if (ftree->left->ident ==1)
	  	{
				ftree->left->token = F_NOT;
				for(j=0;j<treeBufferLength;j++)
				{
					if(treeBuffer[j]==ftree->left) 
					{
						treeBuffer[j] = NULL;
						break;
					}
				}
				ftree->left = pushNegation(ftree->left);
			}
			else
			{
				ftree->left->ident--;
				for(j=0;j<treeBufferLength;j++)
				{
					if(treeBuffer[j] == NULL) continue;
					if(treeBuffer[j]->token==F_NOT && treeBuffer[j]->left==ftree->left->left) 
					{
						tmp1 = treeBuffer[j];
						tmp1->ident++;
						break;
					}
				}
				if(j==treeBufferLength) 	
				{		
			  	tmp1 = ftree_create(F_NOT,1,ftree->left->left,NULL);
			  }
			  
			  if(tmp1->left->token==F_IDENT)
			  {
			  	ftree->left = tmp1;
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = tmp1;				  	
			  }
			  else
			  {
			  	if(ftree->left->left->token!=F_IDENT) ftree->left->left->ident++;
			  	ftree->left = pushNegation(tmp1);	
			  }
			}
			for (j=0;j<treeBufferLength;j++)
			{
			  if (treeBuffer[j] == NULL) continue;
				if (treeBuffer[j]->token == dualToken && treeBuffer[j]->left ==ftree->left )
				{
					common_free(ftree, sizeof(FormulaTree));
					ftree = treeBuffer[j];
					if(treeBuffer[j]->left->token!=F_IDENT) treeBuffer[j]->left->ident--;				
					break;
				}
			}
			if (j == treeBufferLength)
			{
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = ftree;			
			}
	  	return ftree;	
	  case F_E:  if (!dualToken)  dualToken=F_A;
	  case F_A:	 if (!dualToken)  dualToken=F_E;
	  	ftree->token = dualToken;
	  	if (ftree->left->ident ==1)
	  	{
	  		ftree->right = ftree->left->right;
				ftree->left->token = F_NOT;
				ftree->left->right = NULL;
				for(j=0;j<treeBufferLength;j++)
				{
					if(treeBuffer[j]==ftree->left) 
					{
						treeBuffer[j] = NULL;
						break;
					}
				}
				ftree->left = pushNegation(ftree->left);
			}
			else
			{
				ftree->right = (FormulaTree)paramsCopy((ParamList)ftree->left->right);
				ftree->left->ident--;
				for(j=0;j<treeBufferLength;j++)
				{
					if(treeBuffer[j] == NULL) continue;
					if(treeBuffer[j]->token==F_NOT && treeBuffer[j]->left==ftree->left->left) 
					{
						tmp1 = treeBuffer[j];
						tmp1->ident++;
						break;
					}
				}
				if(j==treeBufferLength) 	
				{		
			  	tmp1 = ftree_create(F_NOT,1,ftree->left->left,NULL);

  			  if(tmp1->left->token==F_IDENT)
  			  {
  			   	ftree->left = tmp1;
  					treeBufferLength++;
  					if(treeBufferLength>treeBufferLengthMax)
  					{
  						treeBufferLengthMax+=treeBufferStep;
  						if(treeBufferLengthMax>treeBufferStep)
  						{
  							treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
  						}
  						else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
  					}
  					treeBuffer[treeBufferLength-1] =  tmp1;				  	
  			  }
  			  else
  			  {
  			  	ftree->left->left->ident++;
  			  	ftree->left = pushNegation(tmp1);	
  			  }
  			}
			}
			for (j=0;j<treeBufferLength;j++)
			{
			  if (treeBuffer[j] == NULL) continue;
				if (treeBuffer[j]->token == dualToken && treeBuffer[j]->left ==ftree->left) 
				{
				  if( ((ParamList)(treeBuffer[j]->right))->name == ((ParamList)(ftree->right))->name )
					deleteParams((ParamList)(ftree->right));
					common_free(ftree, sizeof(FormulaTree));
					ftree = treeBuffer[j];
					treeBuffer[j]->left->ident--;				
					break;
				}
			}
			if (j == treeBufferLength)
			{
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = ftree;			
			}
	  	return ftree;	
	  case F_AND: if (!dualToken)  dualToken=F_OR;
	  case F_OR:  if (!dualToken)  dualToken=F_AND;
	  	ftree->token = dualToken;
	  	tmp1 = ftree->left->left;
	  	tmp2 = ftree->left->right;
	  	if( ftree->left->ident == 1 )
	  	{
	  		for (j=0;j<treeBufferLength;j++)
	  		{
	  			if (treeBuffer[j] == NULL) continue;
					if (treeBuffer[j] == ftree->left)
					{
					 	treeBuffer[j] = NULL;
					 	if(tmp1->token!=F_IDENT) tmp1->ident--;
					 	if(tmp2->token!=F_IDENT) tmp2->ident--;
					 	
						common_free(ftree->left, sizeof(FormulaTree));				
						break;
					}	  			
	  		}
	  	}
	  	else
	  	{
	  		ftree->left->ident--;  		
	  	} 	
	  	if(tmp1==tmp2)
	  	{
  	  	for (j=0;j<treeBufferLength;j++)
  	  	{
  	  		if (treeBuffer[j] == NULL) continue;
  	  		if (treeBuffer[j]->token == F_NOT && treeBuffer[j]->left == tmp1)
  	  		{
  	  			ftree->left = treeBuffer[j];
  	  			treeBuffer[j]->ident++;
  	  		}
  	  	}
  	  	if(j==treeBufferLength)
  	  	{
  	  		ftree->left = ftree_create(F_NOT,1,tmp1,NULL);
  	  		if(tmp1->token==F_IDENT)
  	  		{
  					treeBufferLength++;
  					if(treeBufferLength>treeBufferLengthMax)
  					{
  						treeBufferLengthMax+=treeBufferStep;
  						if(treeBufferLengthMax>treeBufferStep)
  						{
  							treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
  						}
  						else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
  					}
  					treeBuffer[treeBufferLength-1] = ftree->left;	 
  	  		}
  	  		else
  	  		{
  	  			tmp1->ident++;
  	  			ftree->left = pushNegation(ftree->left);
  	  		}
  	  	}
  	  	ftree->right=ftree->left;
  	  	if(ftree->left->token!=F_IDENT) ftree->left->ident++;	   		
	  	}
	  	else
	  	{	  	
  	  	for (j=0;j<treeBufferLength;j++)
  	  	{
  	  		if (treeBuffer[j] == NULL) continue;
  	  		if (treeBuffer[j]->token == F_NOT && treeBuffer[j]->left == tmp1)
  	  		{
  	  			ftree->left = treeBuffer[j];
  	  			treeBuffer[j]->ident++;
  	  		}
  	  	}
  	  	if(j==treeBufferLength)
  	  	{
  	  		ftree->left = ftree_create(F_NOT,1,tmp1,NULL);
  	  		if(tmp1->token==F_IDENT)
  	  		{
  				treeBufferLength++;
  				if(treeBufferLength>treeBufferLengthMax)
  				{
  					treeBufferLengthMax+=treeBufferStep;
  					if(treeBufferLengthMax>treeBufferStep)
  					{
  						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
  					}
  					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
  				}
  				treeBuffer[treeBufferLength-1] = ftree->left;	 
  	  		}
  	  		else
  	  		{
  	  			tmp1->ident++;
  	  			ftree->left = pushNegation(ftree->left);
  	  		}
  	  	}
  
  	  	for (j=0;j<treeBufferLength;j++)
  	  	{
  	  		if (treeBuffer[j] == NULL) continue;
  	  		if (treeBuffer[j]->token == F_NOT && treeBuffer[j]->left == tmp2)
  	  		{
  	  			ftree->right = treeBuffer[j];
  	  			treeBuffer[j]->ident++;
  	  		}
  	  	}
  	  	if(j==treeBufferLength)
  	  	{
  	  		ftree->right = ftree_create(F_NOT,1,tmp2,NULL);
  	  		if(tmp2->token==F_IDENT)
  	  		{
  				treeBufferLength++;
  				if(treeBufferLength>treeBufferLengthMax)
  				{
  					treeBufferLengthMax+=treeBufferStep;
  					if(treeBufferLengthMax>treeBufferStep)
  					{
  						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
  					}
  					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
  				}
  				treeBuffer[treeBufferLength-1] = ftree->right;	 
  	  		}
  	  		else
  	  		{
  	  			tmp2->ident++;	
  	  			ftree->right = pushNegation(ftree->right);	
  	  		}
  	  	}	  		
  		}
			for (j=0;j<treeBufferLength;j++)
			{
			  if (treeBuffer[j] == NULL) continue;
				if (treeBuffer[j]->token == dualToken && ( ( treeBuffer[j]->left ==ftree->left && treeBuffer[j]->right == ftree->right)\
				                                      || ( treeBuffer[j]->left ==ftree->right && treeBuffer[j]->right == ftree->left)))
				{
					common_free(ftree, sizeof(FormulaTree));
					ftree = treeBuffer[j];
					if(treeBuffer[j]->left->token!=F_IDENT) treeBuffer[j]->left->ident--;
					if(treeBuffer[j]->right->token!=F_IDENT) treeBuffer[j]->right->ident--;				
					break;
				}
			}	
			if (j == treeBufferLength)
			{
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] =  ftree;	
			}  	
	  	return ftree;		
		case F_EU:	 if (!dualToken)  dualToken=F_AW;	    		  
		case F_AU:	 if (!dualToken)  dualToken=F_EW;	  	
	  case F_EW:	 if (!dualToken)  dualToken=F_AU;	  
		case F_AW:	 if (!dualToken)  dualToken=F_EU;	  			  		  				  		  
	  	ftree->token = dualToken;
	  	tmp1 = ftree->left->left;
	  	tmp2 = ftree->left->right;
	  	
	  	if( ftree->left->ident == 1 )
	  	{
	  		for (j=0;j<treeBufferLength;j++)
	  		{
	  			if (treeBuffer[j] == NULL) continue;
					if (treeBuffer[j] == ftree->left)
					{
					 	treeBuffer[j] = NULL;
						common_free(ftree->left, sizeof(FormulaTree));	
						if(tmp1->token!=F_IDENT) tmp1->ident--;		
						if(tmp2->token!=F_IDENT) tmp2->ident--;	
						break;
					}	  			
	  		}
	  	}
	  	else
	  	{
	  		ftree->left->ident--;		
	  	} 	
	  	
	  	if(tmp1==tmp2)
	  	{
  	  	for (j=0;j<treeBufferLength;j++)
  	  	{
  	  		if (treeBuffer[j] == NULL) continue;
  	  		if (treeBuffer[j]->token == F_NOT && treeBuffer[j]->left == tmp1)
  	  		{
  	  			ftree->left = treeBuffer[j];
  	  			ftree->left->ident++;
  	  			break;
  	  		}
  	  	}
  	  	if(j==treeBufferLength)
  	  	{
  	  		if(tmp1->token==F_IDENT)
  	  		{
  	  			ftree->left = ftree_create(F_NOT,1,tmp1,NULL);
  				treeBufferLength++;
  				if(treeBufferLength>treeBufferLengthMax)
  				{
  					treeBufferLengthMax+=treeBufferStep;
  					if(treeBufferLengthMax>treeBufferStep)
  					{
  						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
  					}
  					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
  				}
  				treeBuffer[treeBufferLength-1] =  ftree->left;	  			
  	  		}
  	  		else
  	  		{
  	  			tmp1->ident++;
  	  			ftree->left = pushNegation(ftree_create(F_NOT,1,tmp1,NULL));
  	  		}
  	  	}
  	  	ftree->right =ftree->left;
  	  	if(ftree->left->token!=F_IDENT) ftree->left->ident++;	  	  		  	
	  	}
	  	else
	  	{	  	
  	  	for (j=0;j<treeBufferLength;j++)
  	  	{
  	  		if (treeBuffer[j] == NULL) continue;
  	  		if (treeBuffer[j]->token == F_NOT && treeBuffer[j]->left == tmp1)
  	  		{
  	  			ftree->left = treeBuffer[j];
  	  			ftree->left->ident++;
  	  			break;
  	  		}
  	  	}
  	  	if(j==treeBufferLength)
  	  	{
  	  		if(tmp1->token==F_IDENT)
  	  		{
  	  			ftree->left = ftree_create(F_NOT,1,tmp1,NULL);
  				treeBufferLength++;
  				if(treeBufferLength>treeBufferLengthMax)
  				{
  					treeBufferLengthMax+=treeBufferStep;
  					if(treeBufferLengthMax>treeBufferStep)
  					{
  						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
  					}
  					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
  				}
  				treeBuffer[treeBufferLength-1] =  ftree->left;	  			
  	  		}
  	  		else
  	  		{
  	  			tmp1->ident++;
  	  			ftree->left = pushNegation(ftree_create(F_NOT,1,tmp1,NULL));
  	  		}
  	  	}
  	  	
  	  	
  	  	for (j=0;j<treeBufferLength;j++)
  	  	{
  	  		if (treeBuffer[j] == NULL) continue;
  	  		if (treeBuffer[j]->token == F_NOT && treeBuffer[j]->left == tmp2)
  	  		{
  	  			ftree->right = treeBuffer[j];
  	  			ftree->right->ident++;
  	  			break;
  	  		}
  	  	}
  	  	if(j==treeBufferLength)
  	  	{
  	  		if(tmp2->token==F_IDENT)
  	  		{
  	  			ftree->right = ftree_create(F_NOT,1,tmp2,NULL);
  				treeBufferLength++;
  				if(treeBufferLength>treeBufferLengthMax)
  				{
  					treeBufferLengthMax+=treeBufferStep;
  					if(treeBufferLengthMax>treeBufferStep)
  					{
  						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
  					}
  					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
  				}
  				treeBuffer[treeBufferLength-1] =  ftree->right;	  			
  	  		}
  	  		else
  	  		{
  	  			tmp2->ident++;
  	  			ftree->right = pushNegation(ftree_create(F_NOT,1,tmp2,NULL));
  	  		}
  	  	}	  	
  		}	
			for (j=0;j<treeBufferLength;j++)
			{
			  if (treeBuffer[j] == NULL) continue;
				if (treeBuffer[j]->token == dualToken && treeBuffer[j]->left ==ftree->left && treeBuffer[j]->right == ftree->right)
				{
					common_free(ftree, sizeof(FormulaTree));
					ftree = treeBuffer[j];
					if(ftree->left->token!=F_IDENT) ftree->left->ident--;
					if(ftree->right->token!=F_IDENT) ftree->right->ident--;			
					break;
				}
			}	
			if (j == treeBufferLength)
			{
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = ftree;	
			}  	
	  	return ftree;    		
		default: 
			common_error("unknown token (%i) in Positive Normal Form",ftree->left->token);
			return NULL;	
	}
	return NULL;
}

void build_ctl_formula (char *formula)
{
	if (mc_ctl_read_from_file)
	{
		if (!(ctlin = fopen(formula,"r")))
			common_error("CTL formula file %s not found", formula);		
		ctlparse();
		fclose(ctlin);  
	}
	else 
	{	
		ctlstr = formula;
		ctlparse();
	}

  free(treeBuffer);
	treeBuffer = NULL;
  treeBufferLength =treeBufferLengthMax =0;
	common_strbuf_new();
	CTLPrintFormula(ctlTree);
	printf("This is the CTL formula: %s\n",common_strbuf);
}
