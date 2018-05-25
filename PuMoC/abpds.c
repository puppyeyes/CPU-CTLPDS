/* =========================================================================
   Extend Moped: abpds.c
   	
   Written by SONG Fu, LIAFA, France 
   Copyright (c) 2010  SONG Fu 

   This program is free software; you can redistribute it and/or modify   
   it under the terms of the GNU General Public License as published by   
   the Free Software Foundation; either version 2 of the License, or      
   (at your option) any later version.                                    
                                                                        
   This program is distributed in the hope that it will be useful,        
   but WITHOUT ANY WARRANTY; without even the implied warranty of         
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          
   GNU General Public License for more details.                           
                                                                        
   You should have received a copy of the GNU General Public License      
   along with this program; if not, write to the Free Software            
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
                                                                     
   Send bug-reports and/or questions to song@liafa.jussieu.fr.                                                                                                                             
   
   This file carry out functions defined in abpds.h

   ====================================================================== */

#include <stdio.h>
#include <string.h>
#include "common.h"
#include "mcheck.h"
#include "abpds.h"
#include "ftree.h"
#include "name.h"
#include "nfa.h"
#include <assert.h>

#define DynamicArrayStep 20

int innerStateCount = 0;
int innerStackCount = 0;
short int rightSideListLength = 0;
short int rightSideListLengthMax = 0;

FormulaTree trueSubformula = NULL;

FormulaTree* atomPropTree = NULL;
short int atomPropTreeNum = 0;
short int atomPropTreeNumMax = 0;

FormulaTree* negAtomPropTree = NULL;
short int negAtomPropTreeNum = 0;
short int negAtomPropTreeNumMax = 0;

extern void CTLPrintFormula(FormulaTree);
extern NameTreeList	mc_atoms;             //Defined by Moped  
extern int isdebug;
extern Name sharp;
extern int mc_CTPL;
extern ValList abpdsValList;
extern int abpdsValNum;
extern FormulaTree ctlTree;
//extern ParamList ctlQuantVarList;

ABPDSHeadTable global_head_table=NULL;

int state_num=0;
int stack_num=0;
int abpds_trans_num=0;

extern void parse_regexp(char*);
extern NFA* mc_nfa;
char* product_buffer=NULL;
/* --------------------------Definitions of Function   --------------------------------- */


/*  Function abpds_empty constructs an initial ABPDS.
    Output: A ABPDS*/
AlternatingBuchiPushdownSystem ABPDSInitial()
{
	AlternatingBuchiPushdownSystem abpds;
	abpds.startState = 0;
	abpds.startStack = NULL;
	
	abpds.acceptingStateList = NULL;
	abpds.transList = ABPDS_head_table_empty();
	abpds.ctrlList = NULL;
	abpds.stackList = NULL;
	abpds.trans=NULL;
	char* tmp = "sharp";
  sharp = name_create(tmp);
	ABPDSInsert2StackList(&(abpds.stackList), sharp);
	return abpds;	
}


/* Function ABPDSDestory deletes the ABPDS.
   Input: A ABPDS. */
   	
void	ABPDSDestory (AlternatingBuchiPushdownSystem* abpds)
{
	name_stack_delete(&(abpds->startStack));
	ABPDSDestoryAcceptingStateList(abpds->acceptingStateList);
	ABPDSDestoryStateList(abpds->ctrlList);
  ABPDSDestoryStackList(abpds->stackList);
  ABPDSDestoryTransList(abpds->trans);
  ABPDS_head_table_delete(abpds->transList);
}

void ABPDSComputeAtomicSubformula(AlternatingBuchiPushdownSystem* abpds, FormulaTree ftree)
{		
	/*delete head table, construct an new head table*/
  ABPDSHead head=NULL;
  ABPDSRule rule=NULL;
  
	if (!ftree) return;	
	switch(ftree->token)
	{
		case F_FALSEEXP: 
			return;
    case F_TRUEEXP: 
    	if (!trueSubformula)
    		trueSubformula = ftree;
    	return;
    case F_IDENT:	
		{
    	short int i=0;
    	for (;i<atomPropTreeNum; i++)
    	{
    		if (atomPropTree[i] == ftree ) return;
    	}
    	atomPropTreeNum++;
    	if(atomPropTreeNum>atomPropTreeNumMax)
    	{	
    		atomPropTreeNumMax+=DynamicArrayStep;	 
				if(atomPropTreeNumMax==DynamicArrayStep)
					atomPropTree = common_malloc(atomPropTreeNumMax*sizeof(FormulaTree));
    		else 	
    			atomPropTree = common_realloc(atomPropTree, atomPropTreeNumMax*sizeof(FormulaTree), DynamicArrayStep*sizeof(FormulaTree));
    	}
    	atomPropTree[atomPropTreeNum-1] = ftree;
			return;
		}    	  	
		case F_NOT:	
		{
    	int i;
    	for (i=0;i<negAtomPropTreeNum; i++)
    	{
    		if (negAtomPropTree[i] == ftree ) return;
    	}
    	negAtomPropTreeNum++;
    	if(negAtomPropTreeNum>negAtomPropTreeNumMax)
    	{
    		negAtomPropTreeNumMax+=DynamicArrayStep;
    		if(negAtomPropTreeNumMax==DynamicArrayStep) 
    			negAtomPropTree = common_malloc(negAtomPropTreeNumMax*sizeof(FormulaTree));
    		else 
    			negAtomPropTree = common_realloc(negAtomPropTree, negAtomPropTreeNumMax*sizeof(FormulaTree),DynamicArrayStep*sizeof(FormulaTree));
    	}
    	negAtomPropTree[negAtomPropTreeNum-1] = ftree;
			return;
		}
		case F_AND:         // for subformula  \psi = \psi_1&&\psi_2,  create transition rule 
		{                   // <[p,\psi],r>-->{<[p,\psi_1],r>,<[p,\psi_2],r>} for all p\in P and r\in \Gamma.
			if (ftree->right->token==F_FALSEEXP || ftree->left->token==F_FALSEEXP ) return;
			ABPDSCtrlLocation tmp1 = NULL;
			ABPDSStackList tmp2 = NULL;
			Name name=create_product_name(abpds->ctrlList->state,ftree,0);
			ABPDSFindOneRightSide(abpds->transList,name,abpds->stackList->stack);
			if (rightSideListLength) return;
			

   	 	if (ftree->right->token==F_TRUEEXP || ftree->right==ftree->left) 
			{
  			for(tmp1 = abpds->ctrlList; tmp1; tmp1 = tmp1->next)
      	{
      		name=create_product_name(tmp1->state,ftree,0);
      		Name name2=create_product_name(tmp1->state,ftree->left,0);
      		for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
      		{	  			  
      			ABPDSRightSideofTransition rightSide = ABPDSInitRightSideofTransition(name2,0,tmp2->stack,0);
      			rightSide->next = NULL;		    			
         		ABPDSTransition abpdsTrans = ABPDSCreateTransition(name, EQUAL, tmp2->stack, rightSide);
          	abpdsTrans->next = abpds->trans;	
          	abpds->trans = abpdsTrans;
          	ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);
          	ABPDSAppend2TransList(global_head_table, abpdsTrans); 
      		}
      	}
        ABPDSComputeAtomicSubformula(abpds, ftree->left);
        return;
   	 	}  	
   	 	if (ftree->left->token==F_TRUEEXP) 
			{
  			for(tmp1 = abpds->ctrlList; tmp1; tmp1 = tmp1->next)
      	{
      		name=create_product_name(tmp1->state,ftree,0);
      		Name name2=create_product_name(tmp1->state,ftree->right,0);
      		for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
      		{	  			  
      			ABPDSRightSideofTransition rightSide = ABPDSInitRightSideofTransition(name2,0,tmp2->stack,0);
      			rightSide->next = NULL;		    			
         		ABPDSTransition abpdsTrans = ABPDSCreateTransition(name, EQUAL, tmp2->stack, rightSide);
          	abpdsTrans->next = abpds->trans;	
          	abpds->trans = abpdsTrans;
          	ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);
          	ABPDSAppend2TransList(global_head_table, abpdsTrans); 
      		}
      	}
        ABPDSComputeAtomicSubformula(abpds, ftree->right);
        return;
   	 	}  	   	 	
		
  			for(tmp1 = abpds->ctrlList; tmp1; tmp1 = tmp1->next)
      	{
      		name=create_product_name(tmp1->state,ftree,0);
      		Name name2=create_product_name(tmp1->state,ftree->left,0);
      		Name name3=create_product_name(tmp1->state,ftree->right,0);
      		for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
      		{	  			  
      			ABPDSRightSideofTransition rightSide = ABPDSInitRightSideofTransition(name2,0,tmp2->stack,0);
      			ABPDSRightSideofTransition rightSideNext = ABPDSInitRightSideofTransition(name3,0,tmp2->stack,0);
      			rightSide->next = rightSideNext;		    			
         		ABPDSTransition abpdsTrans = ABPDSCreateTransition(name, CONJ, tmp2->stack, rightSide);
          	abpdsTrans->next = abpds->trans;	
          	abpds->trans = abpdsTrans;
          	ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);
          	ABPDSAppend2TransList(global_head_table, abpdsTrans); 
      		}
      	}
        ABPDSComputeAtomicSubformula(abpds, ftree->left);
        ABPDSComputeAtomicSubformula(abpds, ftree->right);  
        return;
  	
    };	
		case F_OR:  // for subformula  \psi = \psi_1||\psi_2,  create two transition rule 
		{           // <[p,\psi],r>--><[p,\psi_1],r> and <[p,\psi_1],r>--><[p,\psi_2],r>} for all p\in P , r\in\Gamma
 		
			if (ftree->right->token==F_FALSEEXP && ftree->left->token==F_FALSEEXP ) return;
			ABPDSCtrlLocation tmp1 = NULL;
			ABPDSStackList tmp2 = NULL;
			Name name=create_product_name(abpds->ctrlList->state,ftree,0);
			ABPDSFindOneRightSide(abpds->transList,name,abpds->stackList->stack);
			if (rightSideListLength) return;
			
					
			if(ftree->right->token==F_TRUEEXP || ftree->right==ftree->left )
			{
		
  			for(tmp1 = abpds->ctrlList; tmp1; tmp1 = tmp1->next)
      	{
      		name=create_product_name(tmp1->state,ftree,0);
      		Name name3=create_product_name(tmp1->state,ftree->right,0);
      		for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
      		{           	
           	//procedure the right part
      			ABPDSRightSideofTransition rightSide = ABPDSInitRightSideofTransition(name3,0,tmp2->stack,0);
           	ABPDSTransition abpdsTrans = ABPDSCreateTransition(name,EQUAL,tmp2->stack,rightSide);
          	abpdsTrans->next = abpds->trans;	
          	abpds->trans = abpdsTrans;
          	ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);	 
          	ABPDSAppend2TransList(global_head_table, abpdsTrans); 		   			
      		}
      	}
        ABPDSComputeAtomicSubformula(abpds, ftree->right);    	
      	return;				
			}		
			if(ftree->left->token==F_TRUEEXP )
			{
  			for(tmp1 = abpds->ctrlList; tmp1; tmp1 = tmp1->next)
      	{
      		name=create_product_name(tmp1->state,ftree,0);
      		Name name3=create_product_name(tmp1->state,ftree->left,0);
      		for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
      		{           	
           	//procedure the left part
      			ABPDSRightSideofTransition rightSide = ABPDSInitRightSideofTransition(name3,0,tmp2->stack,0);
           	ABPDSTransition abpdsTrans = ABPDSCreateTransition(name,EQUAL,tmp2->stack,rightSide);
          	abpdsTrans->next = abpds->trans;	
          	abpds->trans = abpdsTrans;
          	ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);	
          	ABPDSAppend2TransList(global_head_table, abpdsTrans);  		   			
      		}
      	}
      	ABPDSComputeAtomicSubformula(abpds, ftree->left);   	
      	return;				
			}				
			for(tmp1 = abpds->ctrlList; tmp1; tmp1 = tmp1->next)
    	{
    		name=create_product_name(tmp1->state,ftree,0);
    		Name name2=ftree->left->token==F_FALSEEXP? 0: create_product_name(tmp1->state,ftree->left,0);
    		Name name3=ftree->right->token==F_FALSEEXP? 0:create_product_name(tmp1->state,ftree->right,0);
    		for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
    		{
    			//procedure the left part
    			if(name2)
    			{
    				ABPDSRightSideofTransition rightSide = ABPDSInitRightSideofTransition(name2,0,tmp2->stack,0);
         		ABPDSTransition abpdsTrans = ABPDSCreateTransition(name,EQUAL,tmp2->stack,rightSide);
        		abpdsTrans->next = abpds->trans;	
        		abpds->trans = abpdsTrans;
        		ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);	
        		ABPDSAppend2TransList(global_head_table, abpdsTrans); 
          }
         	
         	//procedure the right part
    			if(name3)
    			{         	
    				ABPDSRightSideofTransition rightSide = ABPDSInitRightSideofTransition(name3,0,tmp2->stack,0);
         		ABPDSTransition abpdsTrans = ABPDSCreateTransition(name,EQUAL,tmp2->stack,rightSide);
        		abpdsTrans->next = abpds->trans;	
        		abpds->trans = abpdsTrans;
        		ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);
        		ABPDSAppend2TransList(global_head_table, abpdsTrans); 	 		
        	}   			
    		}
    	}
      ABPDSComputeAtomicSubformula(abpds, ftree->left);
      ABPDSComputeAtomicSubformula(abpds, ftree->right);     	
    	return;
    };
		case F_EX:
		case F_AX:
		{
			if(ftree->left->token==F_FALSEEXP) return;
			
			ABPDSCtrlLocation tmp1 = NULL;
			ABPDSStackList tmp2 = NULL;

			for(tmp1 = abpds->ctrlList; tmp1; tmp1 = tmp1->next)
    	{
    		Name name=create_product_name(tmp1->state,ftree,0);
    		Name name2=create_product_name(tmp1->state,ftree->left,0);
     		head=ABPDS_head_find(global_head_table, name,0);
    		rule =head? head->rules: NULL;
    		if(rule || ftree==ctlTree)
    		{  
      		for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
      		{
      			ABPDSFindOneRightSide(abpds->transList, name, tmp2->stack);
        		if (!rightSideListLength) 
        		{    			
        			ABPDSRightSideofTransition rightSide = ABPDSInitRightSideofTransition(name2,0,tmp2->stack,0);
           		ABPDSTransition abpdsTrans = ABPDSCreateTransition(name,EQUAL, tmp2->stack, rightSide);
          		abpdsTrans->next = abpds->trans;	
          		abpds->trans = abpdsTrans;
          		ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);	 
          		ABPDSAppend2TransList(global_head_table, abpdsTrans); 
        		}
      		}    			    		
    		}
    		else
    		{
      		for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
      		{
         		head=ABPDS_head_find(global_head_table, name,tmp2->stack);
        		rule =head? head->rules: NULL;
        		if(rule)
        		{        			
        			ABPDSFindOneRightSide(abpds->transList, name, tmp2->stack);
          		if (!rightSideListLength) 
          		{    			
          			ABPDSRightSideofTransition rightSide = ABPDSInitRightSideofTransition(name2,0,tmp2->stack,0);
             		ABPDSTransition abpdsTrans = ABPDSCreateTransition(name,EQUAL, tmp2->stack, rightSide);
            		abpdsTrans->next = abpds->trans;	
            		abpds->trans = abpdsTrans;
            		ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);	 
            		ABPDSAppend2TransList(global_head_table, abpdsTrans); 
          		}
          	}
      		}      			
    		}		
    	}									
      ABPDSComputeAtomicSubformula(abpds, ftree->left);	
      return;	
    };       
		case F_E:
		{          		
			if(ftree->left->token==F_FALSEEXP) return;
			ABPDSCtrlLocation tmp1 = NULL;
			ABPDSStackList tmp2 = NULL;
			Name name=create_product_name(abpds->ctrlList->state,ftree,0);
			
			ABPDSFindOneRightSide(abpds->transList, name, abpds->stackList->stack);			
			if (rightSideListLength)
			{
      		ABPDSComputeAtomicSubformula(abpds, ftree->left);
				 return;
			}
			for(tmp1 = abpds->ctrlList; tmp1; tmp1 = tmp1->next)
    	{
    		name=create_product_name(tmp1->state,ftree,0);
    		Name name2=create_product_name(tmp1->state,ftree->left,0);
     		head=ABPDS_head_find(global_head_table, name,0);
    		rule =head? head->rules: NULL;
    		if(rule|| ftree==ctlTree)
      	{   
      		for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
      		{
      			/*procedure the left part*/
      			ABPDSRightSideofTransition rightSide = NULL; 
      			ValList value; 
      			if(ftree->left->token==F_TRUEEXP)
      			{
        				rightSide = ABPDSInitRightSideofTransition(name2,0,tmp2->stack,0);
             		ABPDSTransition abpdsTrans = ABPDSCreateTransition(name,EQUAL,tmp2->stack,rightSide);
          			abpdsTrans->next = abpds->trans;	
          			abpds->trans = abpdsTrans;
          			ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);	 
          			ABPDSAppend2TransList(global_head_table, abpdsTrans);  		   				
      			}
      			else
      			{
        			for(value=abpdsValList; value; value = value->next)
        			{	
        				rightSide = ABPDSInitRightSideofTransition(name2,0,tmp2->stack,0);
        				rightSide->aux.rel = value->name ;
        				
             		ABPDSTransition abpdsTrans =ABPDSCreateTransition(name,TOP,tmp2->stack,rightSide);
          			abpdsTrans->next = abpds->trans;	
          			abpdsTrans->var = ((ParamList) ftree->right)->name;
          			abpds->trans = abpdsTrans;
          			ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);	
          			ABPDSAppend2TransList(global_head_table, abpdsTrans);  		
             	}	
            } 	
          }
        }
        else
        {
        	for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
        	{
         		head=ABPDS_head_find(global_head_table, name,tmp2->stack);
        		rule =head? head->rules: NULL;
        		if(rule)
          	{
        			ABPDSRightSideofTransition rightSide = NULL; 
        			ValList value; 
        			if(ftree->left->token==F_TRUEEXP)
        			{
          				rightSide = ABPDSInitRightSideofTransition(name2,0,tmp2->stack,0);
               		ABPDSTransition abpdsTrans = ABPDSCreateTransition(name,EQUAL,tmp2->stack,rightSide);
            			abpdsTrans->next = abpds->trans;	
            			abpds->trans = abpdsTrans;
            			ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);	 
            			ABPDSAppend2TransList(global_head_table, abpdsTrans);  		   				
        			}
        			else
        			{
          			for(value=abpdsValList; value; value = value->next)
          			{	
          				rightSide = ABPDSInitRightSideofTransition(name2,0,tmp2->stack,0);
          				rightSide->aux.rel = value->name ;
          				
               		ABPDSTransition abpdsTrans =ABPDSCreateTransition(name,TOP,tmp2->stack,rightSide);
            			abpdsTrans->next = abpds->trans;	
            			abpdsTrans->var = ((ParamList) ftree->right)->name;
            			abpds->trans = abpdsTrans;
            			ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);	
            			ABPDSAppend2TransList(global_head_table, abpdsTrans);  		
               	}	
              }          		      
          	}     		
        	}
        }	     		   			
    	}
      ABPDSComputeAtomicSubformula(abpds, ftree->left);
    	return;
    };	
		case F_A:
		{                  
			if(ftree->left->token==F_FALSEEXP) return;
			ABPDSCtrlLocation tmp1 = NULL;
			ABPDSStackList tmp2 = NULL;
			Name name=create_product_name(abpds->ctrlList->state,ftree,0);
			
    	ABPDSFindOneRightSide(abpds->transList,name, abpds->stackList->stack);
			if (rightSideListLength)
			{
      ABPDSComputeAtomicSubformula(abpds, ftree->left);
				 return;
				}
			for(tmp1 = abpds->ctrlList; tmp1; tmp1 = tmp1->next)
    	{
      	name=create_product_name(tmp1->state,ftree,0);
      	Name name2=create_product_name(tmp1->state,ftree->left,0);    		
     		head=ABPDS_head_find(global_head_table, name,0);
    		rule =head? head->rules: NULL;
    		if(rule || ftree==ctlTree)
    		{    		
      		for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
      		{
      			ValList value; 
      			ABPDSRightSideofTransition rightSide =NULL;
      			ABPDSRightSideofTransition rightSideNext;
      			if(ftree->left->token==F_TRUEEXP)
      			{
        				rightSide = ABPDSInitRightSideofTransition(name2,0,tmp2->stack,0);
             		ABPDSTransition abpdsTrans = ABPDSCreateTransition(name,EQUAL,tmp2->stack,rightSide);
          			abpdsTrans->next = abpds->trans;	
          			abpds->trans = abpdsTrans;
          			ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);	 
          			ABPDSAppend2TransList(global_head_table, abpdsTrans);    				
      			}
      			else
      			{
        			for(value=abpdsValList; value; value = value->next)
        			{	      			        			
          			rightSideNext = ABPDSInitRightSideofTransition(name2,0,tmp2->stack,0);
          			rightSideNext->next = rightSide;
          			rightSideNext->aux.rel =value->name;
          			rightSide = rightSideNext;
              }
          	
              if (rightSide)
             	{
             		ABPDSTransition abpdsTrans = ABPDSCreateTransition(name, TOP, tmp2->stack, rightSide);
        				abpdsTrans->next = abpds->trans;	
        				abpds->trans = abpdsTrans;
        				abpdsTrans->var = ((ParamList) ftree->right)->name;
        				ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);	
        				ABPDSAppend2TransList(global_head_table, abpdsTrans); 
              }   
            }                	 		   			
      		}
      	}
      	else
      	{
      		for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
      		{      		
         		head=ABPDS_head_find(global_head_table, name,tmp2->stack);
        		rule =head? head->rules: NULL;
        		if(rule)
        		{      
         			ValList value; 
        			ABPDSRightSideofTransition rightSide =NULL;
        			ABPDSRightSideofTransition rightSideNext;
        			if(ftree->left->token==F_TRUEEXP)
        			{
          				rightSide = ABPDSInitRightSideofTransition(name2,0,tmp2->stack,0);
               		ABPDSTransition abpdsTrans = ABPDSCreateTransition(name,EQUAL,tmp2->stack,rightSide);
            			abpdsTrans->next = abpds->trans;	
            			abpds->trans = abpdsTrans;
            			ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);	 
            			ABPDSAppend2TransList(global_head_table, abpdsTrans);    				
        			}
        			else
        			{
          			for(value=abpdsValList; value; value = value->next)
          			{	      			        			
            			rightSideNext = ABPDSInitRightSideofTransition(name2,0,tmp2->stack,0);
            			rightSideNext->next = rightSide;
            			rightSideNext->aux.rel =value->name;
            			rightSide = rightSideNext;
                }
            	
                if (rightSide)
               	{
               		ABPDSTransition abpdsTrans = ABPDSCreateTransition(name, TOP, tmp2->stack, rightSide);
          				abpdsTrans->next = abpds->trans;	
          				abpds->trans = abpdsTrans;
          				abpdsTrans->var = ((ParamList) ftree->right)->name;
          				ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);	
          				ABPDSAppend2TransList(global_head_table, abpdsTrans); 
                }   
              }       			  			
          	}
          }
         }
    	}
  
    	ABPDSComputeAtomicSubformula(abpds, ftree->left);
    	return;
    };	    
		case F_EU:
		case F_AU:
		{
			ABPDSCtrlLocation tmp1 = NULL;
			ABPDSStackList tmp2 = NULL;			
			if(ftree->right->token==F_FALSEEXP) return;
				
			for(tmp1 = abpds->ctrlList; tmp1; tmp1 = tmp1->next)
    	{
				Name name=create_product_name(tmp1->state,ftree,0);

    		Name name2=create_product_name(tmp1->state,ftree->right,0);
     		head=ABPDS_head_find(global_head_table, name,0);
    		rule =head? head->rules: NULL;
    		if(rule|| ftree==ctlTree)
    		{      		
      		for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
      		{		
      			ABPDSFindOneRightSide(abpds->transList,name,tmp2->stack);
  	    		if (!rightSideListLength) 
  	    		{	
  	         	//procedure the right part
  	    			ABPDSRightSideofTransition rightSide = ABPDSInitRightSideofTransition(name2,0,tmp2->stack,0);
  	         	ABPDSTransition abpdsTrans = ABPDSCreateTransition(name, EQUAL, tmp2->stack,rightSide);
      				abpdsTrans->next = abpds->trans;	
      				abpds->trans = abpdsTrans;
      				ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);	
      				ABPDSAppend2TransList(global_head_table, abpdsTrans); 
      				//if(ftree->token==F_EU) ABPDSPrintTransition(abpdsTrans);
  	    		}   			
      		}  
      	}
      	else
      	{
      		for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
      		{      		
         		head=ABPDS_head_find(global_head_table, name,tmp2->stack);
        		rule =head? head->rules: NULL;
        		if(rule)
        		{  
        			ABPDSFindOneRightSide(abpds->transList,name,tmp2->stack);
    	    		if (!rightSideListLength) 
    	    		{	
    	         	//procedure the right part
    	    			ABPDSRightSideofTransition rightSide = ABPDSInitRightSideofTransition(name2,0,tmp2->stack,0);
    	         	ABPDSTransition abpdsTrans = ABPDSCreateTransition(name, EQUAL, tmp2->stack,rightSide);
        				abpdsTrans->next = abpds->trans;	
        				abpds->trans = abpdsTrans;
        				ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);	
        				ABPDSAppend2TransList(global_head_table, abpdsTrans); 
        				//if(ftree->token==F_EU) ABPDSPrintTransition(abpdsTrans);
    	    		}         			
        		}
        	}      		
      	}  		
    	}			
    	ABPDSComputeAtomicSubformula(abpds, ftree->left);	
			ABPDSComputeAtomicSubformula(abpds, ftree->right);
			return;				
		};
		case F_AW:
		case F_EW:
		{
			ABPDSCtrlLocation tmp1 = NULL;
			ABPDSStackList tmp2 = NULL;
			if(ftree->right->token==F_FALSEEXP) return;
				
			for(tmp1 = abpds->ctrlList; tmp1; tmp1 = tmp1->next)
    	{
    		Name name=create_product_name(tmp1->state,ftree,0);
    		Name name2=create_product_name(tmp1->state,ftree->right,0);
     		head=ABPDS_head_find(global_head_table, name,0);
    		rule =head? head->rules: NULL;
    		if(rule || ftree==ctlTree)
    		{     		
      		for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
      		{
      			ABPDSFindOneRightSide(abpds->transList,name, tmp2->stack);
        		if (!rightSideListLength) 
        		{
        			ABPDSRightSideofTransition rightSide = ABPDSInitRightSideofTransition(name2,0,tmp2->stack,0);
           		ABPDSTransition abpdsTrans = ABPDSCreateTransition(name,EQUAL, tmp2->stack, rightSide);
      				abpdsTrans->next = abpds->trans;	
      				abpds->trans = abpdsTrans;
      				ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);	
      				ABPDSAppend2TransList(global_head_table, abpdsTrans); 
        		}
      		}  
      	}
      	else
      	{
      		for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
      		{      		
         		head=ABPDS_head_find(global_head_table, name,tmp2->stack);
        		rule =head? head->rules: NULL;
        		if(rule)
        		{  
        			ABPDSFindOneRightSide(abpds->transList,name, tmp2->stack);
          		if (!rightSideListLength) 
          		{
          			ABPDSRightSideofTransition rightSide = ABPDSInitRightSideofTransition(name2,0,tmp2->stack,0);
             		ABPDSTransition abpdsTrans = ABPDSCreateTransition(name,EQUAL, tmp2->stack, rightSide);
        				abpdsTrans->next = abpds->trans;	
        				abpds->trans = abpdsTrans;
        				ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);	
        				ABPDSAppend2TransList(global_head_table, abpdsTrans); 
          		}        			
        		}
        	}      		
      	}  
    		DdNode* bdd=NULL;
    		if(CTLFullset) Cudd_Ref(bdd=CTLFullset);
    		ABPDSAppend2AcceptingStateList(&(abpds->acceptingStateList), name, bdd);          			
    	}			
    	ABPDSComputeAtomicSubformula(abpds, ftree->left);	
			ABPDSComputeAtomicSubformula(abpds, ftree->right);
			return;				
		};
		default: return;
	}
}	

void HandleInitialConfiguration(AlternatingBuchiPushdownSystem* abpds, FormulaTree ftree)
{		
	/*delete head table, construct an new head table*/

	if (!ftree) return;	
	if(!abpds->startState) return;	
	switch(ftree->token)
	{			
		case F_EX:
		case F_AX:
		{
      if(ftree->left->token==F_FALSEEXP) return;

  		Name name=create_product_name(abpds->startState,ftree,0);
  		Name name2=create_product_name(abpds->startState,ftree->left,0);
  	//	ABPDSRightSideofTransition* rightSideList =NULL;
			ABPDSFindOneRightSide(abpds->transList, name, abpds->startStack->name);
  		if (!rightSideListLength) 
  		{    			
  			ABPDSRightSideofTransition rightSide = ABPDSInitRightSideofTransition(name2,0,abpds->startStack->name,0);
     		ABPDSTransition abpdsTrans = ABPDSCreateTransition(name,EQUAL, abpds->startStack->name, rightSide);
    		abpdsTrans->next = abpds->trans;	
    		abpds->trans = abpdsTrans;
    		ABPDSAppend2TransList(global_head_table, abpdsTrans);  
    		HandleInitialConfiguration(abpds,ftree->left); 
  		}
      return;	
    };
		case F_E:
		{          		
			Name name=create_product_name(abpds->startState,ftree,0);
			if(ftree->left->token==F_FALSEEXP) return;
			ABPDSFindOneRightSide(abpds->transList, name, abpds->startStack->name);
			if (rightSideListLength) return; 
			
  		Name name2=create_product_name(abpds->startState,ftree->left,0);
   	
			/*procedure the left part*/
			ABPDSRightSideofTransition rightSide = NULL; 
			ValList value; 
			if(ftree->left->token==F_TRUEEXP)
			{
  				rightSide = ABPDSInitRightSideofTransition(name2,0,abpds->startStack->name,0);
       		ABPDSTransition abpdsTrans = ABPDSCreateTransition(name,EQUAL,abpds->startStack->name,rightSide);
    			abpdsTrans->next = abpds->trans;	
    			abpds->trans = abpdsTrans;
    			ABPDSAppend2TransList(global_head_table, abpdsTrans); 	    				
			}
			else
  		{
  			for(value=abpdsValList; value; value = value->next)
  			{	
  				rightSide = ABPDSInitRightSideofTransition(name2,0,abpds->startStack->name,0);
  				rightSide->aux.rel = value->name ;
  				
       		ABPDSTransition abpdsTrans =ABPDSCreateTransition(name,TOP,abpds->startStack->name,rightSide);
    			abpdsTrans->next = abpds->trans;	
    			abpdsTrans->var = ((ParamList) ftree->right)->name;
    			abpds->trans = abpdsTrans;
    			ABPDSAppend2TransList(global_head_table, abpdsTrans);  	
       	}	
      }
      HandleInitialConfiguration(abpds,ftree->left);         
    	return;
    };	
		case F_A:
		{                  
			Name name=create_product_name(abpds->startState,ftree,0);
			if(ftree->left->token==F_FALSEEXP) return;
    	ABPDSFindOneRightSide(abpds->transList,name,abpds->startStack->name);
      if (rightSideListLength) 	return;		

    	Name name2=create_product_name(abpds->startState,ftree->left,0);    		
 
			ValList value; 
			ABPDSRightSideofTransition rightSide =NULL;
			ABPDSRightSideofTransition rightSideNext;
			if(ftree->left->token==F_TRUEEXP)
			{
  				rightSide = ABPDSInitRightSideofTransition(name2,0,abpds->startStack->name,0);
       		ABPDSTransition abpdsTrans = ABPDSCreateTransition(name,EQUAL,abpds->startStack->name,rightSide);
    			abpdsTrans->next = abpds->trans;	
    			abpds->trans = abpdsTrans;
    			ABPDSAppend2TransList(global_head_table, abpdsTrans); 	 			
			}
			else
			{
  			for(value=abpdsValList; value; value = value->next)
  			{	      			        			
    			rightSideNext = ABPDSInitRightSideofTransition(name2,0,abpds->startStack->name,0);
    			rightSideNext->next = rightSide;
    			rightSideNext->aux.rel =value->name;
    			rightSide = rightSideNext;
        }
    	
        if (rightSide)
       	{
       		ABPDSTransition abpdsTrans = ABPDSCreateTransition(name, TOP, abpds->startStack->name, rightSide);
  				abpdsTrans->next = abpds->trans;	
  				abpds->trans = abpdsTrans;
  				abpdsTrans->var = ((ParamList) ftree->right)->name;
  				ABPDSAppend2TransList(global_head_table, abpdsTrans); 
        }
       }   
      HandleInitialConfiguration(abpds,ftree->left);   
    	return;
    };	    
		case F_EU:
		case F_AU:
		{
			if(ftree->right->token==F_FALSEEXP) return;
				
			Name name=create_product_name(abpds->startState,ftree,0);
  		ABPDSFindOneRightSide(abpds->transList,name, abpds->startStack->name);
  		if (rightSideListLength) 	break;
  		Name name2=create_product_name(abpds->startState,ftree->right,0); 
	    
     	//procedure the right part
			ABPDSRightSideofTransition rightSide = ABPDSInitRightSideofTransition(name2,0,abpds->startStack->name,0);
     	ABPDSTransition abpdsTrans = ABPDSCreateTransition(name, EQUAL,abpds->startStack->name,rightSide);
			abpdsTrans->next = abpds->trans;	
			abpds->trans = abpdsTrans;
			ABPDSAppend2TransList(global_head_table, abpdsTrans); 	
	    HandleInitialConfiguration(abpds,ftree->right);  
			return;				
		};
		case F_AW:
		case F_EW:
		{
			if(ftree->right->token==F_FALSEEXP) return;
				
  		Name name=create_product_name(abpds->startState,ftree,0);
  		Name name2=create_product_name(abpds->startState,ftree->right,0);
   
			ABPDSFindOneRightSide(abpds->transList,name,abpds->startStack->name);
  		if (!rightSideListLength) 
  		{
  			ABPDSRightSideofTransition rightSide = ABPDSInitRightSideofTransition(name2,0,abpds->startStack->name,0);
     		ABPDSTransition abpdsTrans = ABPDSCreateTransition(name,EQUAL, abpds->startStack->name, rightSide);
				abpdsTrans->next = abpds->trans;	
				abpds->trans = abpdsTrans;
				ABPDSAppend2TransList(global_head_table, abpdsTrans); 
  		}
  		DdNode* bdd=NULL;
  		if(CTLFullset) Cudd_Ref(bdd=CTLFullset);
  		ABPDSAppend2AcceptingStateList(&(abpds->acceptingStateList), name, bdd);          			
  	  HandleInitialConfiguration(abpds,ftree->right);  
			return;				
		};
		case F_AND:
		case F_OR:
  	  HandleInitialConfiguration(abpds,ftree->right);  
  	  HandleInitialConfiguration(abpds,ftree->left);  
			return;				
		default: return;
	}
}	

void ABPDSPrepareForModelChecking(AlternatingBuchiPushdownSystem* abpds)
{
	//ABPDS_head_table_delete(abpds->transList);
  ABPDSHead head=NULL;
  ABPDSRule rule=NULL;	
  ABPDSRule rule2=NULL;	
	global_head_table=ABPDS_head_table_empty();
	
	ABPDSTransition t1=abpds->trans;
	
	for(;t1;t1=t1->next)
	{
		ABPDSAppend2TransList(global_head_table,t1);
	}
	
  ABPDSComputeAtomicSubformula(abpds,ctlTree);
  HandleInitialConfiguration(abpds,ctlTree);
	ABPDSCtrlLocation tmp1 = NULL;
	ABPDSStackList tmp2 = NULL;
	ABPDSRightSideofTransition* rightSideList =NULL;
	ABPDSTransition abpdsTrans;

	if (trueSubformula)
	{
		ABPDSRightSideofTransition rightSide;
		for(tmp1 = abpds->ctrlList; tmp1; tmp1 = tmp1->next)
		{
			Name name1=create_product_name(tmp1->state,trueSubformula,0);
     	head=ABPDS_head_find(global_head_table, name1,0);
    	rule =head? head->rules: NULL;
    	if(rule || trueSubformula==ctlTree)
    	{			
				for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
				{
  				rightSide = ABPDSInitRightSideofTransition(name1,0,tmp2->stack,0);
					abpdsTrans = ABPDSCreateTransition(name1, EQUAL , tmp2->stack, rightSide);
        	abpdsTrans->next = abpds->trans;	
        	abpds->trans = abpdsTrans;
        	ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);
        	ABPDSAppend2TransList(global_head_table,abpdsTrans); 
				}
			}
			else
    	{
    		for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
    		{      		
       		head=ABPDS_head_find(global_head_table, name1,tmp2->stack);
      		rule =head? head->rules: NULL;
      		if(rule)
      		{          		
    				rightSide = ABPDSInitRightSideofTransition(name1,0,tmp2->stack,0);
    				abpdsTrans = ABPDSCreateTransition(name1, EQUAL , tmp2->stack, rightSide);
          	abpdsTrans->next = abpds->trans;	
          	abpds->trans = abpdsTrans;
          	ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);
          	ABPDSAppend2TransList(global_head_table,abpdsTrans);       			
      		}
      	}      		
    	}  
  		DdNode* bdd=NULL;
  		if(CTLFullset) Cudd_Ref(bdd=CTLFullset);
  		ABPDSAppend2AcceptingStateList(&(abpds->acceptingStateList),name1, bdd);  		
		}
	}
	 	
	 		
	NameTreeList atoms;
	short int i;
	DdNode* bdd =NULL;
	for (i=0;i<atomPropTreeNum;i++)
	{
		//printf("atomic prop name111:%s\n", name_cstring(atomPropTree[i]->ident));
  	if((!(atomPropTree[i]->right) && !(atomPropTree[i]->left))||(atomPropTree[i]->right && atomPropTree[i]->left)) // seperate   atomic propostion between P(x1,...,x_) (normal) and $r1r2$ (regular proposition)
    {
    	for (atoms = mc_atoms; atoms; atoms = atoms->next)
    	{
    		//printf("atomic prop name222:%s\n", name_cstring(atoms->name));
    		if (atomPropTree[i]->ident == atoms->name && ((int)atomPropTree[i]->left) == atoms->length)
    		{
    			for(tmp1 = abpds->ctrlList; tmp1; tmp1 = tmp1->next)
    			{
    				if(isdebug) printf("State %s.\n", name_cstring(tmp1->state));
    				if(name_bal_find(atoms->tree,tmp1->state))
    				{
    					if(isdebug) printf("The atomic proposition is %s.\n", name_cstring(atomPropTree[i]->ident));    						
    					bdd = mc_CTPL? build_conj_bdd((ParamList)(atomPropTree[i]->right),atoms->valList): NULL;
    					if(!bdd && atoms->length) continue;		
    					ABPDSRightSideofTransition rightSide = NULL;
  						abpdsTrans =NULL;
  						Name name1=create_product_name(tmp1->state,atomPropTree[i],0);
     					
     					head=ABPDS_head_find(global_head_table, name1,0);
    					rule =head? head->rules: NULL;
    					if(rule|| atomPropTree[i]==ctlTree)
    					{     								
      					//ABPDSFindOneRightSide(abpds->transList,name1, abpds->stackList->stack);
        				//if (!rightSideListLength)     								
  					
  							for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
  							{
  								rightSide = ABPDSInitRightSideofTransition(name1,0,tmp2->stack,0);
  						 		abpdsTrans= ABPDSCreateTransition(name1,EQUAL,tmp2->stack,rightSide);
        					abpdsTrans->next = abpds->trans;	
       						abpds->trans = abpdsTrans;
                  ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);
                  ABPDSAppend2TransList(global_head_table,abpdsTrans);       							
      					}
  						}
  						else
  						{
  							for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
  							{
       						head=ABPDS_head_find(global_head_table, name1,tmp2->stack);
      						rule =head? head->rules: NULL;
      						if(rule)
      						{
    								rightSide = ABPDSInitRightSideofTransition(name1,0,tmp2->stack,0);
    						 		abpdsTrans= ABPDSCreateTransition(name1,EQUAL,tmp2->stack,rightSide);
          					abpdsTrans->next = abpds->trans;	
         						abpds->trans = abpdsTrans;
                    ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);
                    ABPDSAppend2TransList(global_head_table,abpdsTrans);         							   	
      						}							
  							}  							
  						}
  						//Cudd_PrintMinterm(CTLBDDMgr,bdd);
  						if(mc_CTPL)
  						{
  							DdNode* old=ABPDSCheckStateAcceptiness(abpds->acceptingStateList,name1);
  							if(old) 
  							{
  								DdNode* newbdd;
  								Cudd_Ref(newbdd = Cudd_bddOr(CTLBDDMgr,old,bdd));
  								Cudd_RecursiveDeref(CTLBDDMgr,bdd);	
  								bdd=newbdd;
  							}	
  						}
  						ABPDSAppend2AcceptingStateList(&(abpds->acceptingStateList), name1, bdd); 		
    				}
    			}
    			for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
    			{
    				if(isdebug) printf("Stack %s.\n", name_cstring(tmp2->stack));
    				if(name_bal_find(atoms->tree,tmp2->stack))
    				{
    					if(atoms->length) 
    					{
    						printf("Error: Parameterized atomic proposition can not project to stack symbol! ");
    						//exit(1);
    						continue;
    					}
    					bdd =NULL;		
    					if(isdebug) printf("The atomic proposition is %s.\n", name_cstring(atomPropTree[i]->ident));
    					ABPDSRightSideofTransition rightSide = NULL;
  						abpdsTrans =NULL;
  						for(tmp1 = abpds->ctrlList; tmp1; tmp1 = tmp1->next)
  						{				
  							Name name1=create_product_name(tmp1->state,atomPropTree[i],0);
       					head=ABPDS_head_find(global_head_table, name1,tmp2->stack);
      					rule =head? head->rules: NULL;
      					head=ABPDS_head_find(global_head_table, name1,0);
      					rule2 =head? head->rules: NULL;	
      					if(!rule && !rule2) continue;
      					  							
  							ABPDSFindOneRightSide(abpds->transList, name1, tmp2->stack);
  							if (rightSideListLength) continue;	           // this state has been proceeded, ignore it.
  							rightSide = ABPDSInitRightSideofTransition(name1,0,tmp2->stack,0);
  						 	abpdsTrans= ABPDSCreateTransition(name1, EQUAL, tmp2->stack, rightSide);
        				abpdsTrans->next = abpds->trans;	
       					abpds->trans = abpdsTrans;
                ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);
                ABPDSAppend2TransList(global_head_table,abpdsTrans); 
  							if(CTLFullset) Cudd_Ref(bdd=CTLFullset);
  							ABPDSAppend2AcceptingStateList(&(abpds->acceptingStateList), name1, bdd); 								
  						} 		  					
    				}
    			}
    			if(!mc_CTPL) break;
    		}
    	}
    	for(tmp1 = abpds->ctrlList; tmp1; tmp1 = tmp1->next)
    	{
    		if(tmp1->state == atomPropTree[i]->ident && !(atomPropTree[i]->left) )
    		{				
  				Name name1=create_product_name(tmp1->state,atomPropTree[i],0);
  				ABPDSRightSideofTransition rightSide = NULL;
  				abpdsTrans =NULL;
  				
					head=ABPDS_head_find(global_head_table, name1,0);
					rule2 =head? head->rules: NULL;					
  				
  				for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
  				{
 						if(!rule2)
 						{
 							head=ABPDS_head_find(global_head_table, name1,tmp2->stack);
							rule =head? head->rules: NULL;      					
							if(!rule) continue;  
						}  					
  					ABPDSFindOneRightSide(abpds->transList, name1, tmp2->stack);
  					if (rightSideListLength) continue;
  					rightSide = ABPDSInitRightSideofTransition(name1,0,tmp2->stack,0);
  				 	abpdsTrans= ABPDSCreateTransition(name1,EQUAL ,tmp2->stack,rightSide);
    				abpdsTrans->next = abpds->trans;	
   					abpds->trans = abpdsTrans;
        		ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);
       		  ABPDSAppend2TransList(global_head_table,abpdsTrans); 
  				}
  				if(CTLFullset) Cudd_Ref(bdd =CTLFullset);
  				ABPDSAppend2AcceptingStateList(&(abpds->acceptingStateList), name1, bdd);  
    		}
    	}
    	for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
    	{
    		if(tmp2->stack == atomPropTree[i]->ident && !(atomPropTree[i]->left) )
    		{			
  				ABPDSRightSideofTransition rightSide = NULL;
  				abpdsTrans =NULL;
  				bdd =NULL;
  				for(tmp1 = abpds->ctrlList; tmp1; tmp1 = tmp1->next)
  				{										
  					Name name1=create_product_name(tmp1->state,atomPropTree[i],0);
   				
   					head=ABPDS_head_find(global_head_table, name1,tmp2->stack);
  					rule =head? head->rules: NULL;
  					head=ABPDS_head_find(global_head_table, name1,0);
  					rule2 =head? head->rules: NULL;	
  					if(!rule && !rule2) continue;  					
  					
  					ABPDSFindOneRightSide(abpds->transList, name1, tmp2->stack);
  					if (rightSideListLength) continue;
  					rightSide = ABPDSInitRightSideofTransition(name1,0,tmp2->stack,0);
  				 	abpdsTrans= ABPDSCreateTransition(name1,EQUAL, tmp2->stack,rightSide);
    				abpdsTrans->next = abpds->trans;	
   					abpds->trans = abpdsTrans;
     			  ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);
      			ABPDSAppend2TransList(global_head_table,abpdsTrans); 
  				//	ABPDSPrintTransition(abpdsTrans);
  					if(CTLFullset) Cudd_Ref(bdd =CTLFullset);
  					ABPDSAppend2AcceptingStateList(&(abpds->acceptingStateList), name1,bdd); 					
  				}
    		}
    	} 
    }
    else      // regular proposition 
  	{
  		parse_regexp(name_cstring(atomPropTree[i]->ident));
      //print_nfa(mc_nfa);                                       //   debug
      //printf("\n-----------------\n");
  		//exit(1);
  		ABPDSRightSideofTransition rightSide=NULL;
  		ABPDSTransition abpdsTrans =NULL;
  		Name name1=0;
  		Name name2=0;
  		StateList nfaState;
  		for(tmp1 = abpds->ctrlList; tmp1; tmp1 = tmp1->next)
  		{
  			nfaState=mc_nfa->initial_states;
  			name1=create_product_name(tmp1->state,atomPropTree[i],0);
  			for(; nfaState; nfaState = nfaState->next)
  			{
  				for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
  				{
  					 //if(tmp2->stack==sharp) continue;       //sharp is the bottom symbol of the stack
  					name2=create_product_name(0, NULL, nfaState->state);
  					rightSide = ABPDSInitRightSideofTransition(name2,0,tmp2->stack,0);
  					abpdsTrans = ABPDSCreateTransition(name1,EQUAL, tmp2->stack,rightSide);
    				abpdsTrans->next = abpds->trans;	
   					abpds->trans = abpdsTrans;
        		ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);
        		ABPDSAppend2TransList(global_head_table,abpdsTrans); 
  				}
  			}
  		}
			NFATrans nfaTrans = mc_nfa->trans;
			for(; nfaTrans; nfaTrans = nfaTrans->next)
			{
				name1=create_product_name(0,NULL,nfaTrans->from_state);
				name2=create_product_name(0, NULL,nfaTrans->to_state);
				if(IsVariable(nfaTrans->input))
				{
  				for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
  				{
  					if(tmp2->stack==sharp) continue;       //sharp is the bottom symbol of the stack
  					rightSide = ABPDSInitRightSideofTransition(name2,0,0,0);
  					rightSide->aux.rel = nfaTrans->input;       // variable
  					abpdsTrans = ABPDSCreateTransition(name1, VAREQU, tmp2->stack, rightSide);
						abpdsTrans->next = abpds->trans;	
 						abpds->trans = abpdsTrans;    					
  		      ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);
      			ABPDSAppend2TransList(global_head_table,abpdsTrans); 
  				}
			  }
			  else
			  {
			  	rightSide = ABPDSInitRightSideofTransition(name2,0,0,0);
			  /*	if(mc_bp)
			  	{
      			char tmps[256];	// fixme
          	bp_fun_t fun;
          	Name stack=0;
      			for (fun = bp_functions; fun; fun = fun->next)
      			{
      				sprintf(tmps,"%s:%s",name_cstring(fun->funname),name_cstring(nfaTrans->input));
      				stack = name_create(tmps);
      				if (bp_lookup(bp_labeltree,stack,-2)) break;
      			}
      			if (!fun||!stack) common_error("The symbol %s in regular expression %s is not stack symbol",name_cstring(nfaTrans->input),name_cstring(atomPropTree[i]->ident));
						abpdsTrans = ABPDSCreateTransition(name1, EQUAL, stack, rightSide);
					}
					else */
					abpdsTrans = ABPDSCreateTransition(name1, EQUAL, nfaTrans->input, rightSide);
						
					abpdsTrans->next = abpds->trans;	
 					abpds->trans = abpdsTrans;  					
	        ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);
      		ABPDSAppend2TransList(global_head_table,abpdsTrans); 
				}				
			}
			
			for(nfaState = mc_nfa->final_states; nfaState; nfaState = nfaState->next)
			{
				name1=create_product_name(0,NULL,nfaState->state);
				rightSide = ABPDSInitRightSideofTransition(name1,0,sharp,0);
				abpdsTrans = ABPDSCreateTransition(name1,EQUAL, sharp,rightSide);
				abpdsTrans->next = abpds->trans;	
 				abpds->trans = abpdsTrans;
        ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);
      	ABPDSAppend2TransList(global_head_table,abpdsTrans); 
  			DdNode* bdd=NULL;
  			if(CTLFullset) Cudd_Ref(bdd=CTLFullset);
				ABPDSAppend2AcceptingStateList(&(abpds->acceptingStateList), name1,bdd);
			}  			
  		
  		deleteNFA(mc_nfa,0);
  	}
  }
  common_free(atomPropTree,atomPropTreeNumMax);
  atomPropTreeNumMax = atomPropTreeNum = 0;

	for (i=0;i<negAtomPropTreeNum;i++)
	{
		if(negAtomPropTree[i]->left->left)
		{
  		for (atoms = mc_atoms; atoms; atoms = atoms->next)
  		{
  			if (negAtomPropTree[i]->left->ident == atoms->name && (int)(negAtomPropTree[i]->left->left)==atoms->length)
  			{
  				for(tmp1 = abpds->ctrlList; tmp1; tmp1 = tmp1->next)
  				{
  					Name name1=create_product_name(tmp1->state,negAtomPropTree[i],0);
      			head=ABPDS_head_find(global_head_table, name1,0);
      			rule2 =head? head->rules: NULL;	
      				 					
  					
  					if(name_bal_find(atoms->tree,tmp1->state))
  					{
  						
    					ABPDSFindOneRightSide(abpds->transList,name1, abpds->stackList->stack);
      				if (!rightSideListLength)     								
  						{
    						ABPDSRightSideofTransition rightSide = NULL;
  							abpdsTrans =NULL;
  							for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
  							{
        					if(!rule2)
        					{
        						head=ABPDS_head_find(global_head_table, name1,tmp2->stack);
      							rule =head? head->rules: NULL;
      							if(!rule) continue; 	 								
      						}
  								rightSide = ABPDSInitRightSideofTransition(name1,0,tmp2->stack,0);
  						 		abpdsTrans= ABPDSCreateTransition(name1,EQUAL,tmp2->stack,rightSide);
  								abpdsTrans->next = abpds->trans;	
   								abpds->trans = abpdsTrans;
    				      ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);
        					ABPDSAppend2TransList(global_head_table,abpdsTrans); 
  							}
  						}
  						bdd = mc_CTPL? build_disj_bdd((ParamList)(negAtomPropTree[i]->left->right),atoms->valList): NULL;
  						
  						if(mc_CTPL)
  						{
  							DdNode* old=ABPDSCheckStateAcceptiness(abpds->acceptingStateList,name1);
  							if(old) 
  							{
  								DdNode* newbdd;
  								Cudd_Ref(newbdd = Cudd_bddAnd(CTLBDDMgr,old,bdd));
  								Cudd_RecursiveDeref(CTLBDDMgr,bdd);	
  								bdd=newbdd;
  							}	
  						}  							
  						if(bdd) ABPDSAppend2AcceptingStateList(&(abpds->acceptingStateList), name1,bdd);  	  						
  					}
  				}
  			}
  		}
			for(tmp1 = abpds->ctrlList; tmp1; tmp1 = tmp1->next)
			{
				bdd=NULL;
				Name name1=create_product_name(tmp1->state,negAtomPropTree[i],0);
				tmp2 = abpds->stackList;
				ABPDSFindOneRightSide(abpds->transList,name1, tmp2->stack);
				if (rightSideListLength) continue;	           // this state has been proceeded, ignore it.  
      	head=ABPDS_head_find(global_head_table, name1,0);
      	rule2 =head? head->rules: NULL;						  				
				for(; tmp2; tmp2 = tmp2->next)
				{
					if(!rule2)
					{
						head=ABPDS_head_find(global_head_table, name1,tmp2->stack);
						rule =head? head->rules: NULL;
						if(!rule) continue; 	 								
					}					
					ABPDSRightSideofTransition rightSide = ABPDSInitRightSideofTransition(name1,0,tmp2->stack,0);
				 	abpdsTrans= ABPDSCreateTransition(name1, EQUAL, tmp2->stack, rightSide);
  				abpdsTrans->next = abpds->trans;	
   				abpds->trans = abpdsTrans;
    			ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);
        	ABPDSAppend2TransList(global_head_table,abpdsTrans); 
				}
				if(CTLFullset) Cudd_Ref(bdd=CTLFullset);
				ABPDSAppend2AcceptingStateList(&(abpds->acceptingStateList), name1,bdd); 
			}    		
		}
		else
		{
			if(negAtomPropTree[i]->left->right)
    	{
    	//	printf("\nregexp:  %s  \n", name_cstring(negAtomPropTree[i]->left->ident));
    		parse_regexp(name_cstring(negAtomPropTree[i]->left->ident));  
    		CNFA* cnfa = ComplementNFA(mc_nfa);
    		//print_nfa(mc_nfa);
    		//printf("\n----------------------\n");    		
    		//print_cnfa(cnfa);
    		//printf("\n----------------------\n");
    		//exit(1);
    		
    		ABPDSRightSideofTransition rightSide = NULL;    		
    		ABPDSTransition abpdsTrans = NULL;  
    		ABPDSRightSideofTransition tmp_rightSide=NULL;		
  			Name name1=0;
  			Name name2=0;
  			StateList cnfaState;
    		for(tmp1 = abpds->ctrlList; tmp1; tmp1 = tmp1->next)
    		{
    			name1=create_product_name(tmp1->state,negAtomPropTree[i],0);
    			/*move to initial states of finite automata corresponding to Item 2 in paper.*/
    			for(cnfaState= cnfa->initial_states; cnfaState; cnfaState = cnfaState->next)
    			{
    				name2=create_product_name(0,NULL,cnfaState->state);
    				for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
    				{       					
    					//if(tmp2->stack==sharp) continue;
    					rightSide = ABPDSInitRightSideofTransition(name2,0,tmp2->stack,0);
    					abpdsTrans = ABPDSCreateTransition(name1,EQUAL, tmp2->stack,rightSide);
  						abpdsTrans->next = abpds->trans;	
   						abpds->trans = abpdsTrans;
    				  ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);
        			ABPDSAppend2TransList(global_head_table,abpdsTrans); 
    				}
    			}
    		}
  			CNFATrans cnfaTrans;
  			
  			for(cnfaTrans= cnfa->trans; cnfaTrans; cnfaTrans = cnfaTrans->next)
  			{
  				tmp_rightSide=NULL;
  				StateList tmp_state=cnfaTrans->rightSide;
  				switch(cnfaTrans->type)
  			  {
  			  	case 0:
  			  	{
  			  		name1=create_product_name(0,NULL,cnfaTrans->from_state);
  			  		name2=create_product_name(0,NULL,tmp_state->state);
  			  		if(cnfaTrans->rightSide->next)
  			  		{
        				for(tmp_state=cnfaTrans->rightSide; tmp_state; tmp_state = tmp_state->next )
        				{
          				innerStackCount++;	
          				
          				sprintf(product_buffer,"%s.%d", "epsilon",innerStackCount);
          				Name tmp_stack = name_create(product_buffer);
          				
          				name2=create_product_name(0,NULL,tmp_state->state);
          				
        					rightSide = ABPDSInitRightSideofTransition(name2,0,0,0);					
        					abpdsTrans = ABPDSCreateTransition(name2,EQUAL,tmp_stack,rightSide);
      						abpdsTrans->next = abpds->trans;	
       						abpds->trans = abpdsTrans;
        					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
      						ABPDSAppend2TransList(global_head_table,abpdsTrans); 
        					
        					rightSide = ABPDSInitRightSideofTransition(name2,0,tmp_stack,0);
        					rightSide->next = tmp_rightSide;
        		  		tmp_rightSide = rightSide;          		  		
        				}   
        			}
        			else	tmp_rightSide = ABPDSInitRightSideofTransition(name2,0,0,0);   
    			  	/*if(mc_bp)
    			  	{
          			char tmps[256];	// fixme
              	bp_fun_t fun;
              	Name stack=0;
          			for (fun = bp_functions; fun; fun = fun->next)
          			{
          				sprintf(tmps,"%s:%s",name_cstring(fun->funname),name_cstring(cnfaTrans->input->state));
          				stack = name_create(tmps);
          				if (bp_lookup(bp_labeltree,stack,-2)) break;
          			}
          			if (!fun||!stack) common_error("The symbol %s in regular expression %s is not stack symbol",name_cstring(cnfaTrans->input->state),name_cstring(negAtomPropTree[i]->left->ident));
    						abpdsTrans = ABPDSCreateTransition(name1, CONJ, stack, rightSide);
    					}
    					else */ 
    					abpdsTrans = ABPDSCreateTransition(name1, CONJ, cnfaTrans->input->state,tmp_rightSide);  
    						          				 					
      				abpdsTrans->next = abpds->trans;	
       				abpds->trans = abpdsTrans;
  				    ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);
      			  ABPDSAppend2TransList(global_head_table,abpdsTrans); 
        			break;  				  		
  			  	}	
  			  	case 1:
  			  	{
  			  		for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
  			  		{	  			
  			  			if(tmp2->stack==sharp) continue;
  			  			name1=create_product_name(0,NULL,cnfaTrans->from_state);
  			  			name2=create_product_name(0,NULL,tmp_state->state);
    			  		if(cnfaTrans->rightSide->next)
    			  		{
    			  			tmp_rightSide =NULL;
          				for(tmp_state=cnfaTrans->rightSide; tmp_state; tmp_state = tmp_state->next )
          				{
                				innerStackCount++;	
                				sprintf(product_buffer,"%s.%d", "epsilon",innerStackCount);
                				Name tmp_stack = name_create(product_buffer);
              					rightSide = ABPDSInitRightSideofTransition(name2,0,0,0);					
              					abpdsTrans = ABPDSCreateTransition(name2,EQUAL,tmp_stack,rightSide);
          							abpdsTrans->next = abpds->trans;	
           							abpds->trans = abpdsTrans;
            						ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
          							ABPDSAppend2TransList(global_head_table,abpdsTrans); 
              					
              					rightSide = ABPDSInitRightSideofTransition(name2,0,tmp_stack,0);
              					rightSide->next = tmp_rightSide;
              					rightSide->aux.rel= cnfaTrans->input->state;
              		  		tmp_rightSide = rightSide;            		  		
          				}   
          			}
          			else	
          			{
          				tmp_rightSide = ABPDSInitRightSideofTransition(name2,0,0,0);    				
          			  tmp_rightSide->aux.rel=cnfaTrans->input->state;
          			}	    	
    						abpdsTrans = ABPDSCreateTransition(name1, VAREQU,tmp2->stack,tmp_rightSide);		
  							abpdsTrans->next = abpds->trans;	
   							abpds->trans = abpdsTrans;
  				      ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);
      					ABPDSAppend2TransList(global_head_table,abpdsTrans);       		  		
              }        			
        			break;  				  		
  			  	}
   					case 2:
      			{
   			  		name1=create_product_name(0,NULL,cnfaTrans->from_state);
  			  		name2=create_product_name(0,NULL,tmp_state->state);         				
    					for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
    					{
    						if(tmp2->stack==sharp) continue;
    						if(CheckMemberShipWithVar(cnfaTrans->input,tmp2->stack)) continue;					
    						InputList input;
    						tmp_rightSide =NULL;    						
    						for(input=cnfaTrans->input; input; input=input->next)
    						{
    							if(IsVariable(input->state))
    							{
    								rightSide=ABPDSInitRightSideofTransition(name2,0,0,0);
    								rightSide->aux.rel=input->state;
    								rightSide->next=tmp_rightSide;
    								tmp_rightSide =rightSide;
    							}
    						}
    						if(tmp_rightSide==NULL) continue;
  							abpdsTrans = ABPDSCreateTransition(name1, VARNEQU,tmp2->stack,tmp_rightSide);		
  							abpdsTrans->next = abpds->trans;	
   							abpds->trans = abpdsTrans;
  				      ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);
      					ABPDSAppend2TransList(global_head_table,abpdsTrans);       					
    					}
    					break;
    				}       						
  					default:
  						printf("Warning in product of regular.\n");
  						break;
  			  } 			
  			}
  			/*Add loop on <[p,f,\B],#>  corresponding to Item 13 in paper.*/
  			for(cnfaState = cnfa->final_states; cnfaState; cnfaState = cnfaState->next)
  			{
  				name1=create_product_name(0,NULL,cnfaState->state);
  				rightSide = ABPDSInitRightSideofTransition(name1,0,sharp,0);
  				abpdsTrans = ABPDSCreateTransition(name1,EQUAL,sharp,rightSide);
					abpdsTrans->next = abpds->trans;	
					abpds->trans = abpdsTrans;
  				ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);
      		ABPDSAppend2TransList(global_head_table,abpdsTrans); 
  				bdd=NULL;
  				if(CTLFullset) Cudd_Ref(bdd=CTLFullset);
  				ABPDSAppend2AcceptingStateList(&(abpds->acceptingStateList),name1,bdd);
  			}  			
    		
    		deleteNFA(mc_nfa,0);  
    		CNFADelete(cnfa);		
    	}
			else
			{
      	for (atoms = mc_atoms; atoms; atoms = atoms->next)
      	{
      		if (negAtomPropTree[i]->left->ident == atoms->name && !atoms->length)
      		{
      			for(tmp1 = abpds->ctrlList; tmp1; tmp1 = tmp1->next)
      			{
      				if(!name_bal_find(atoms->tree,tmp1->state) && atoms->name !=tmp1->state)
      				{
                Name name1=create_product_name(tmp1->state,negAtomPropTree[i],0);
      					head=ABPDS_head_find(global_head_table, name1,0);
      					rule2 =head? head->rules: NULL;	      					
      					ABPDSRightSideofTransition rightSide = NULL;
    						abpdsTrans =NULL;
    						for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
    						{
    							if(name_bal_find(atoms->tree,tmp2->stack)||tmp2->stack==atoms->name) continue;
        					if(!rule2)
        					{
        						head=ABPDS_head_find(global_head_table, name1,tmp2->stack);
        						rule =head? head->rules: NULL;
        						if(!rule) continue; 	 								
        					}	
    							rightSide = ABPDSInitRightSideofTransition(name1,0,tmp2->stack,0);
    						 	abpdsTrans= ABPDSCreateTransition(name1, EQUAL ,tmp2->stack,rightSide);
      						abpdsTrans->next = abpds->trans;	
       						abpds->trans = abpdsTrans;
    				      ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);
        					ABPDSAppend2TransList(global_head_table,abpdsTrans); 
    						} 		
    						bdd= CTLFullset? CTLFullset: NULL;
    						if(bdd || !mc_CTPL) ABPDSAppend2AcceptingStateList(&(abpds->acceptingStateList), name1, bdd); 		
      				}
      			}  			
      			break;
      		}
      	}
      	if(!atoms)     // deal with some atomic proposition of the CTL formula which does not defined by the PDS.
      	{
    			for(tmp1 = abpds->ctrlList; tmp1; tmp1 = tmp1->next)
    			{
    				if (tmp1->state==negAtomPropTree[i]->left->ident) continue;
    				Name name1=create_product_name(tmp1->state,negAtomPropTree[i],0);
  					head=ABPDS_head_find(global_head_table, name1,0);
  					rule2 =head? head->rules: NULL;	     				
    				ABPDSRightSideofTransition rightSide = NULL;
    				abpdsTrans =NULL;
    				for(tmp2 = abpds->stackList; tmp2; tmp2 = tmp2->next)
    				{
      				if (tmp2->stack==negAtomPropTree[i]->left->ident) continue;
    					if(!rule2)
    					{
    						head=ABPDS_head_find(global_head_table, name1,tmp2->stack);
    						rule =head? head->rules: NULL;
    						if(!rule) continue; 	 								
    					}	    					
    					rightSide = ABPDSInitRightSideofTransition(name1,0,tmp2->stack,0);
    				 	abpdsTrans= ABPDSCreateTransition(name1, EQUAL, tmp2->stack,rightSide);
  						abpdsTrans->next = abpds->trans;	
   						abpds->trans = abpdsTrans;
    				  ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);
        			ABPDSAppend2TransList(global_head_table,abpdsTrans); 
    				} 
    				if(CTLFullset) Cudd_Ref(bdd=CTLFullset);
    				ABPDSAppend2AcceptingStateList(&(abpds->acceptingStateList), name1,bdd); 					
    			}  		
      	}
    	}
    }  			
	}
  common_free(negAtomPropTree,negAtomPropTreeNumMax);
  negAtomPropTreeNumMax = negAtomPropTreeNum =0;
  if (isdebug)
  {
    printf("The following is the accepting states of the ABPDS.\n");
    printAcceptingState(abpds->acceptingStateList);
    printf("End of print the accepting states of the ABPDS.\n");
  }
  common_free(rightSideList,rightSideListLengthMax);
	rightSideListLengthMax = rightSideListLength = 0;  
	ABPDS_head_table_delete(abpds->transList);
	abpds->transList=global_head_table;
}


void printAcceptingState(ABPDSAcceptingStateList list)
{
	if(list)
	{
		printf("%s", name_cstring(list->state));
		fprintf(file,"FinalState:%s", name_cstring(list->state));
		if(list->Binding) Cudd_PrintMinterm(CTLBDDMgr,list->Binding);
		printf(";");
		fprintf(file,";\n");
		printAcceptingState(list->left);
		printAcceptingState(list->right);
	}
}
/* Function ABPDSDisplay prints the ABPDS.
   Input: A ABPDS. */
void	ABPDSDisplay (AlternatingBuchiPushdownSystem* abpds)
{
	// It has not been used.
}

/* Function ABPDSInsertTransition adds an transition into the ABPDS.
   This function will achieve the product of PDS and CTL.
   Input: An ABPDS, the state of the left-hand side of the transition rule,
   	      FormulaTree of the left-hand side of the transition rule,
   	      The stack of the left-hand side of the transition rule,
   	      The right-hand side of ABPDS transition rule.*/          
void	ABPDSInsertTransition(AlternatingBuchiPushdownSystem* abpds, Name fromState, FormulaTree ftree, Name fromStack, Name toState, Name toStack1, Name toStack2)
{
	Name name1,name2,name3;
  if (ftree)
	{
	  switch(ftree->token)
    {
    	case F_A:
    	case F_E:
    		{
    			ABPDSInsertTransition(abpds, fromState, ftree->left, fromStack, toState, toStack1, toStack2);
    			return;
    		}
    	case F_AND:
    	case F_OR:
    		{
    			ABPDSInsertTransition(abpds, fromState, ftree->left, fromStack, toState, toStack1, toStack2);
    			ABPDSInsertTransition(abpds, fromState, ftree->right, fromStack, toState, toStack1, toStack2);
    			return;
    		}
	    case F_EX:	
	    {	
	    	if(ftree->left->token==F_FALSEEXP) return;
	    	name1=create_product_name(fromState,ftree,0);
	    	name2=create_product_name(toState,ftree->left,0);
	    	ABPDSRightSideofTransition* rightSideList = ABPDSFindRightSide(abpds->transList, name1, fromStack);
				ABPDSRightSideofTransition rightSide= NULL;	
	    	if (!rightSideListLength)
	    	{
	    		rightSide= ABPDSInitRightSideofTransition(name2,0,toStack1,toStack2);
	      	ABPDSTransition abpdsTrans= ABPDSCreateTransition(name1,EQUAL,fromStack,rightSide);
					abpdsTrans->next = abpds->trans;	
					abpds->trans = abpdsTrans;
					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
	      	ABPDSInsertTransition(abpds, fromState, ftree->left, fromStack, toState, toStack1, toStack2);
	      }
	      else
	      {
	      	if(ftree->left->token==F_TRUEEXP) return;
	      	short int i;
	      	for (i = 0; i<rightSideListLength;i++)
	      	{
	      		if (rightSideList[i]->state == name2 && rightSideList[i]->stack1 == toStack1&&rightSideList[i]->stack2 == toStack2  )
	      			break;
	      	}
	      	if (i==rightSideListLength)
	      	{
	      		rightSide= ABPDSInitRightSideofTransition(name2,0,toStack1,toStack2);
	      		ABPDSTransition abpdsTrans= ABPDSCreateTransition(name1,EQUAL,fromStack,rightSide);
						abpdsTrans->next = abpds->trans;	
						abpds->trans = abpdsTrans;
						ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
	      		ABPDSInsertTransition(abpds, fromState, ftree->left, fromStack, toState, toStack1, toStack2);  		
	      	}
	      }	    		
	    	return;
	    }
	    case F_AX:		
	    {
	    	if(ftree->left->token==F_FALSEEXP) return;
	    	name1=create_product_name(fromState,ftree,0);
	    	name2=create_product_name(toState,ftree->left,0);
	    		    	
	    	ABPDSRightSideofTransition* rightSideList = ABPDSFindRightSide(abpds->transList, name1, fromStack);
	    	if(rightSideListLength)
	    	{
	    		if(ftree->left->token==F_TRUEEXP) return;
	    		ABPDSRightSideofTransition rightSide = NULL;
	    		for (rightSide = rightSideList[0];rightSide; rightSide = rightSide->next) 
	    		{
	    			if (rightSide->state == name2 && rightSide->stack1== toStack1 && rightSide->stack2==toStack2)
	    				break;
	    		}
	    		if (!rightSide)
	    		{
	    			if (toStack1 == 0)
	    			{
	    				innerStackCount++;	
	    				sprintf(product_buffer,"%s.%d", "epsilon",innerStackCount);

		    			rightSide = ABPDSInitRightSideofTransition(name2,0,0,0);  
	    				Name innerName = name_create(product_buffer);		

      				innerStateCount++;	

      				sprintf(product_buffer,"%s.%d", name_cstring(toState),innerStateCount);  
        			
        			Name innerStateName = name_create(product_buffer);			
	    				ABPDSTransition abpdsTrans = ABPDSCreateTransition(innerStateName,EQUAL, innerName, rightSide);
							abpdsTrans->next = abpds->trans;	
							abpds->trans = abpdsTrans;
							ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
	    				
	    				rightSide = ABPDSInitRightSideofTransition(innerStateName,0,innerName,0);
	    				rightSide->next = rightSideList[0]->next;
	    				rightSideList[0]->next =rightSide; 			
	    				   				   				
	    			}
	    			else 
	    			{
	    				if (toStack2 == 0)
  	    			{
  	    				rightSide = ABPDSInitRightSideofTransition(name2,0,toStack1,0);
  	    				rightSide->next = rightSideList[0]->next;
  	    				rightSideList[0]->next =rightSide; 	
      				}
  	    			else
  	    			{
  	    				innerStateCount++; 

        				sprintf(product_buffer,"%s.%d", name_cstring(toState),innerStateCount);  
        				 	    				 			
  		    			rightSide =  ABPDSInitRightSideofTransition(name2,0,toStack1,toStack2); 
  	    				Name innerName = name_create(product_buffer);		
  	    				ABPDSTransition abpdsTrans = ABPDSCreateTransition(innerName, EQUAL, toStack1,rightSide);
								abpdsTrans->next = abpds->trans;	
								abpds->trans = abpdsTrans;
								ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
  	    			
  	    				rightSide = ABPDSInitRightSideofTransition(innerName,0,toStack1,0);
  	    				rightSide->next = rightSideList[0]->next;
  	    				rightSideList[0]->next =rightSide;	
  	    				
  	    			}
  	    		}
	    			ABPDSInsertTransition(abpds, fromState, ftree->left, fromStack, toState, toStack1, toStack2);
	    		}   			
	    	}
	    	else
	    	{
	    		ABPDSRightSideofTransition rightSide;
	    		if(ftree->left->token==F_TRUEEXP) 
	    		{
  	    			rightSide =  ABPDSInitRightSideofTransition(name2,0,toStack1,toStack2);
  	        	ABPDSTransition abpdsTrans= ABPDSCreateTransition(name1,EQUAL,fromStack,rightSide);
  						abpdsTrans->next = abpds->trans;	
  						abpds->trans = abpdsTrans;
  						ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
  						return;	    				
	    		}
	    		
    			if (toStack1 == 0)
    			{
    				innerStackCount++;	
    				sprintf(product_buffer,"%s.%d",  "epsilon" ,innerStackCount);
    				
		    		rightSide = ABPDSInitRightSideofTransition(name2,0,0,0); 
	    			Name innerName = name_create(product_buffer);	
 
    				innerStateCount++;	
    				sprintf(product_buffer,"%s.%d", name_cstring(toState),innerStateCount);  
    			    				
      			Name innerStateName = name_create(product_buffer);			
	    			ABPDSTransition abpdsTrans = ABPDSCreateTransition(innerStateName,EQUAL, innerName, rightSide);
						abpdsTrans->next = abpds->trans;	
						abpds->trans = abpdsTrans;
						ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
	    			
	    			rightSide = ABPDSInitRightSideofTransition(innerStateName, 0, innerName, 0);
	    			abpdsTrans = ABPDSCreateTransition(name1,CONJ, fromStack,rightSide);   	
						abpdsTrans->next = abpds->trans;	
						abpds->trans = abpdsTrans;
						ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
    			}
	    		else 
	    		{
	    			if (toStack2 ==0)
  	    		{
  	    			rightSide =  ABPDSInitRightSideofTransition(name2,0,toStack1,0);
  	        	ABPDSTransition abpdsTrans= ABPDSCreateTransition(name1,CONJ,fromStack,rightSide);
  						abpdsTrans->next = abpds->trans;	
  						abpds->trans = abpdsTrans;
  						ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 		
  	    		}
  	    		else
  	    		{

      				innerStateCount++;	

      				sprintf(product_buffer,"%s.%d",  name_cstring(toState),innerStateCount);    	    			
  	    			
  		    		rightSide =  ABPDSInitRightSideofTransition(name2,0,toStack1,toStack2);
  	    			Name innerName = name_create(product_buffer);		
  	    			ABPDSTransition abpdsTrans = ABPDSCreateTransition(innerName,EQUAL,toStack1,rightSide);
  						abpdsTrans->next = abpds->trans;	
  						abpds->trans = abpdsTrans;
  						ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
  	    			rightSide = ABPDSInitRightSideofTransition(innerName,0,toStack1,0);
  	    			abpdsTrans = ABPDSCreateTransition(name1,CONJ, fromStack,rightSide);   	
  						abpdsTrans->next = abpds->trans;	
  						abpds->trans = abpdsTrans;
  						ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
  	    				  
  	    		}  			
	    		}
	    		ABPDSInsertTransition(abpds, fromState, ftree->left, fromStack, toState, toStack1, toStack2);
	    	}
	    	return;
	    }
	    case F_EU:
	    {
	    	if(ftree->right->token==F_FALSEEXP)
	    	{ 
	    		ABPDSInsertTransition(abpds, fromState, ftree->left, fromStack, toState, toStack1, toStack2);
	    		return;
	    	}
	    	name1=create_product_name(fromState,ftree,0);
	    	name2=create_product_name(fromState,ftree->left,0);
	    	name3=create_product_name(toState,ftree,0);
	    	
	    	ABPDSRightSideofTransition* rightSideList = ABPDSFindRightSide(abpds->transList, name1, fromStack);
	    	if(rightSideListLength)
	    	{
	    		if(ftree->left->token==F_FALSEEXP)
	    		{
	    			 ABPDSInsertTransition(abpds, fromState, ftree->right, fromStack, toState, toStack1, toStack2);
	    			 return;
	    		}
	    		ABPDSRightSideofTransition rightSide = NULL;
	    		short int i;
	    		for (i =0; i<rightSideListLength; i++)
	    		{
	    			for (rightSide = rightSideList[i]; rightSide; rightSide=rightSide->next)
	    			{
	    				if (rightSide->state == name3 && rightSide->stack1== toStack1 && rightSide->stack2==toStack2)
	    				{
    						i = rightSideListLength;
    						break;
	    				}
	    			}
	    		}
	    		if (!rightSide)            // does not exist a transition from <[p,\psi],r>.
	    		{
  	    		ABPDSRightSideofTransition tmp2 = NULL;
  	    		ABPDSTransition abpdsTrans;	    			
	    			if(ftree->left->token==F_TRUEEXP)
    	    	{
    	    			rightSide = ABPDSInitRightSideofTransition(name3,0,toStack1,toStack2);						
    	    			
    	        	abpdsTrans= ABPDSCreateTransition(name1,EQUAL,fromStack,rightSide);
    						abpdsTrans->next = abpds->trans;	
    						abpds->trans = abpdsTrans;
    						ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
    		        ABPDSInsertTransition(abpds, fromState, ftree->right, fromStack, toState, toStack1, toStack2);
    		        return;
    				}	    			
	    			
	    			rightSide = ABPDSInitRightSideofTransition(name2,0,fromStack,0);
  	    		

    	    	if (toStack1 == 0)
      			{
      				innerStackCount++;	
      				sprintf(product_buffer,"%s.%d",  "epsilon" ,innerStackCount);
      				
  		    		tmp2 = ABPDSInitRightSideofTransition(name3,0,0,0);
  	    			Name innername = name_create(product_buffer);			

      				innerStateCount++;	

      				sprintf(product_buffer,"%s.%d", name_cstring(toState),innerStateCount);  
      				     				
        			Name innerStateName = name_create(product_buffer);			
  	    			abpdsTrans = ABPDSCreateTransition(innerStateName,EQUAL, innername, tmp2);
  						abpdsTrans->next = abpds->trans;	
  						abpds->trans = abpdsTrans;
  						ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
  	    			
  	    			tmp2 = ABPDSInitRightSideofTransition(innerStateName,0,innername,0);
  	    			rightSide->next = tmp2;
  	    			abpdsTrans = ABPDSCreateTransition(name1,CONJ, fromStack,rightSide);   	
   						abpdsTrans->next = abpds->trans;	
  						abpds->trans = abpdsTrans;
  						ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);
      			}
  	    		else 
  	    		{
  	    			if (toStack2 == 0)
    	    		{			
    	    			tmp2 = ABPDSInitRightSideofTransition(name3,0,toStack1,0);						
    	    			rightSide->next = tmp2;
    	        	abpdsTrans= ABPDSCreateTransition(name1,CONJ,fromStack,rightSide);
    						abpdsTrans->next = abpds->trans;	
    						abpds->trans = abpdsTrans;
    						ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
    	    		}
    	    		else
    	    		{
        				innerStateCount++;	
        				sprintf(product_buffer,"%s.%d", name_cstring(toState),innerStateCount);  
    		    		tmp2= ABPDSInitRightSideofTransition(name3,0,toStack1,toStack2);	  
    	    			Name innername = name_create(product_buffer);		
    	    			abpdsTrans = ABPDSCreateTransition(innername,EQUAL, toStack1,tmp2);
    						abpdsTrans->next = abpds->trans;	
    						abpds->trans = abpdsTrans;
    						ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
    	    			tmp2 = ABPDSInitRightSideofTransition(innername,0,toStack1,0);
    	    			rightSide->next = tmp2;	
    	    			abpdsTrans = ABPDSCreateTransition(name1,CONJ, fromStack, rightSide);   	
    						abpdsTrans->next = abpds->trans;	
    						abpds->trans = abpdsTrans;
    						ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
    	    		}	    			
  	    		}
  	    		ABPDSInsertTransition(abpds, fromState, ftree->left, fromStack, toState, toStack1, toStack2);
  	    		ABPDSInsertTransition(abpds, fromState, ftree->right, fromStack, toState, toStack1, toStack2);
  	    	}
	    	}
	    	else
	    	{
	    		ABPDSTransition abpdsTrans;
					ABPDSRightSideofTransition tmp2 = NULL;
						    		
	        Name name4=create_product_name(fromState,ftree->right,0);
	    		ABPDSRightSideofTransition tmp1 =  ABPDSInitRightSideofTransition(name4,0,fromStack,0);
	        abpdsTrans= ABPDSCreateTransition(name1,EQUAL,fromStack,tmp1);
					abpdsTrans->next = abpds->trans;	
					abpds->trans = abpdsTrans;
					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);		 
					if(ftree->right->token==F_TRUEEXP)
					{
						ABPDSInsertTransition(abpds, fromState, ftree->left, fromStack, toState, toStack1, toStack2);
						return;
					} 		
					   		
	    		if(ftree->left->token==F_TRUEEXP)
	    		{
	    				tmp1 = ABPDSInitRightSideofTransition(name3,0,toStack1,toStack2);					
	    		
	    				abpdsTrans= ABPDSCreateTransition(name1,EQUAL, fromStack,tmp1);   	
  						abpdsTrans->next = abpds->trans;	
  						abpds->trans = abpdsTrans;
  						ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 	    			
	    			  ABPDSInsertTransition(abpds, fromState, ftree->right, fromStack, toState, toStack1, toStack2); 
	    			  return;
    			}
	    			
	    		tmp1 = ABPDSInitRightSideofTransition(name2,0,fromStack,0);

				
  	    	if (toStack1 == 0)
    			{
    				innerStackCount++;	
    				sprintf(product_buffer,"%s.%d", "epsilon",innerStackCount);
		    		tmp2 = ABPDSInitRightSideofTransition(name3,0,0,0);
	    			Name innername = name_create(product_buffer);			
    				innerStateCount++;	
    				
    				sprintf(product_buffer,"%s.%d", name_cstring(toState),innerStateCount);  
    				 
      			Name innerStateName = name_create(product_buffer);			
	    			abpdsTrans= ABPDSCreateTransition(innerStateName,EQUAL, innername, tmp2);
						abpdsTrans->next = abpds->trans;	
						abpds->trans = abpdsTrans;
						ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
	    			
	    			tmp2 = ABPDSInitRightSideofTransition(innerStateName,0,innername,0);
	    			tmp1->next = tmp2;
	    			abpdsTrans= ABPDSCreateTransition(name1,CONJ, fromStack,tmp1);   	
						abpdsTrans->next = abpds->trans;	
						abpds->trans = abpdsTrans;
						ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
    			}
  	    	else
  	    	{ 
  	    		if (toStack2 == 0)
	    			{
	    				tmp2 = ABPDSInitRightSideofTransition(name3,0,toStack1,0);					
	    				tmp1->next = tmp2;	    		
	    				abpdsTrans= ABPDSCreateTransition(name1,CONJ, fromStack,tmp1);   	
  						abpdsTrans->next = abpds->trans;	
  						abpds->trans = abpdsTrans;
  						ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
	        	}
	        	else
	        	{
      				innerStateCount++;	
      				
      				sprintf(product_buffer,"%s.%d", name_cstring(toState),innerStateCount);  
      				
  	    			
  		    		tmp2 = ABPDSInitRightSideofTransition(name3,0,toStack1,toStack2);
  	    			Name innername = name_create(product_buffer);			
  	    			abpdsTrans = ABPDSCreateTransition(innername,EQUAL, toStack1,tmp2);
  						abpdsTrans->next = abpds->trans;	
  						abpds->trans = abpdsTrans;
  						ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
  						
  	    			tmp2 = ABPDSInitRightSideofTransition(innername,0,toStack1,0);
  	    			tmp1->next = tmp2;
  	    			abpdsTrans = ABPDSCreateTransition(name1, CONJ,fromStack, tmp1);   	
  						abpdsTrans->next = abpds->trans;	
  						abpds->trans = abpdsTrans;
  						ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
  	    		}    	
	        }
 
	        ABPDSInsertTransition(abpds, fromState, ftree->left, fromStack, toState, toStack1, toStack2);
	        ABPDSInsertTransition(abpds, fromState, ftree->right, fromStack, toState, toStack1, toStack2);       	
	    	}
	    	return;
	    }
	    case F_AU:	
	    {	
	    	if(ftree->right->token==F_FALSEEXP) 
	    	{
	    		ABPDSInsertTransition(abpds, fromState, ftree->left, fromStack, toState, toStack1, toStack2);   
	    		return;
	    	}
	    	name1=create_product_name(fromState,ftree,0);
	    	name2=create_product_name(fromState,ftree->left,0);
	    	name3=create_product_name(toState,ftree,0);
	    		    	
	    	ABPDSRightSideofTransition* rightSideList = ABPDSFindRightSide(abpds->transList, name1, fromStack);
	    	if(rightSideListLength)
	    	{
	    		if(ftree->left->token==F_FALSEEXP) 
	    		{ 
	    			ABPDSInsertTransition(abpds, fromState, ftree->right, fromStack, toState, toStack1, toStack2);  
	    			return;
	    		}
	    		if(ftree->right->token==F_TRUEEXP) 
	    		{ 
	    			ABPDSInsertTransition(abpds, fromState, ftree->left, fromStack, toState, toStack1, toStack2);  
	    			return;
	    		}	    		
	    		ABPDSRightSideofTransition tmp;
	    		short int i;
	    		for (i = 0; i<rightSideListLength; i++)
	    		{
	    			if (rightSideList[i]->next==NULL&&ftree->left->token!=F_TRUEEXP)	continue;
						for (tmp = rightSideList[i]; tmp; tmp = tmp->next)
						{
	    				if (tmp->state == name3 && tmp->stack1== toStack1 && tmp->stack2==toStack2 )	break;
	    			}
	    			if(!tmp)
	    			{
	    				ABPDSTransition abpdsTrans;
    					if (toStack1 == 0)
        			{
        				innerStackCount++;	
        				sprintf(product_buffer,"%s.%d", "epsilon",innerStackCount);
        				
    		    		tmp = ABPDSInitRightSideofTransition(name3,0,0,0); 
    	    			Name innername = name_create(product_buffer);		
        				innerStateCount++;	
        				
        				sprintf(product_buffer,"%s.%d", name_cstring(toState),innerStateCount);  
        				
          			Name innerStateName = name_create(product_buffer);			
    	    			abpdsTrans = ABPDSCreateTransition(innerStateName,EQUAL, innername, tmp);
    	    			
      					abpdsTrans->next = abpds->trans;	
      					abpds->trans = abpdsTrans;
      					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
    	    			tmp = ABPDSInitRightSideofTransition(innerStateName,0,innername,0);
  	    				ABPDSRightSideofTransition tmp2 = rightSideList[i]->next;
  	    				rightSideList[i]->next = tmp;
  	    				tmp->next =tmp2;	    					
        			}
  						else
  						{
  							if (toStack2 ==0 )
  							{
  								tmp =  ABPDSInitRightSideofTransition(name3,0,toStack1,0);
  	    					ABPDSRightSideofTransition tmp2 = rightSideList[i]->next;
  	    					rightSideList[i]->next = tmp;
  	    					tmp->next =tmp2;
  	        		}	
  	        		else
    	        	{
          				innerStateCount++;	
          				sprintf(product_buffer,"%s.%d", name_cstring(toState),innerStateCount);  
          				
    	    			
    		    			tmp = ABPDSInitRightSideofTransition(name3,0,toStack1,toStack2);
    	    				Name innername = name_create(product_buffer);		
    	    				abpdsTrans = ABPDSCreateTransition(innername,EQUAL, toStack1,tmp);
        					abpdsTrans->next = abpds->trans;	
        					abpds->trans = abpdsTrans;
        					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);
    	    			  tmp = ABPDSInitRightSideofTransition(innername,0,toStack1,0);
    	    				ABPDSRightSideofTransition tmp2 = rightSideList[i]->next;
    	    				rightSideList[i]->next = tmp;
    	    				tmp->next =tmp2;	    					
    	        	}
    	        }
  	        	ABPDSInsertTransition(abpds, fromState, ftree->left, fromStack, toState, toStack1, toStack2);
	    				ABPDSInsertTransition(abpds, fromState, ftree->right, fromStack, toState, toStack1, toStack2);   				
  	    		}
  	    		return;
  	    	}
	    	}
	    	else
	    	{
  	      Name name4=create_product_name(fromState,ftree->right,0);
	    		ABPDSRightSideofTransition tmp = ABPDSInitRightSideofTransition(name4,0,fromStack,0);    			
	        ABPDSTransition abpdsTrans = ABPDSCreateTransition(name1,EQUAL,fromStack,tmp);
					abpdsTrans->next = abpds->trans;	
					abpds->trans = abpdsTrans;
					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
					if(ftree->right->token==F_TRUEEXP)
					{
	       		ABPDSInsertTransition(abpds, fromState, ftree->left, fromStack, toState, toStack1, toStack2); 						
						return;    		
					}
					
					ABPDSRightSideofTransition tmp2= NULL ;
					if(ftree->left->token==F_FALSEEXP)
					{
	       		ABPDSInsertTransition(abpds, fromState, ftree->right, fromStack, toState, toStack1, toStack2); 						
						return;    		
					}

	    		tmp = ftree->left->token==F_TRUEEXP? NULL: ABPDSInitRightSideofTransition(name2,0,fromStack,0);
	    			    		
					if (toStack1 == 0)
    			{
    				innerStackCount++;	
    				sprintf(product_buffer,"%s.%d",  "epsilon",innerStackCount);
    				
						tmp2 = ABPDSInitRightSideofTransition(name3,0,0,0);
	    			Name innername = name_create(product_buffer);		
    				innerStateCount++;	
    				
    				sprintf(product_buffer,"%s.%d", name_cstring(toState),innerStateCount);  
    				
      			Name innerStateName = name_create(product_buffer);			
	    			abpdsTrans = ABPDSCreateTransition(innerStateName,EQUAL, innername, tmp2);
  					abpdsTrans->next = abpds->trans;	
  					abpds->trans = abpdsTrans;
  					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
	    			tmp2 = ABPDSInitRightSideofTransition(innerStateName,0,innername,0);
	    			if(tmp) tmp->next = tmp2;
	    			else tmp=tmp2;
	    			abpdsTrans= ABPDSCreateTransition(name1,CONJ,fromStack,tmp);
  					abpdsTrans->next = abpds->trans;	
  					abpds->trans = abpdsTrans;
  					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 					    			  				   				
    			}
  				else
  				{
  					if (toStack2 ==0 )
  	    		{
  	    			tmp2 = ABPDSInitRightSideofTransition(name3,0,toStack1,0);		
  	    			if(tmp)		tmp->next = tmp2;	    		
  	    			else tmp=tmp2;
  	        	abpdsTrans = ABPDSCreateTransition(name1,CONJ,fromStack,tmp);
    					abpdsTrans->next = abpds->trans;	
    					abpds->trans = abpdsTrans;
    					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
  	        }
  	        else
  	        {
      				innerStateCount++;	
      				sprintf(product_buffer,"%s.%d", name_cstring(toState),innerStateCount);  
	   
  	    			tmp2 = ABPDSInitRightSideofTransition(name3,0,toStack1,toStack2);    			
  	    			Name innername = name_create(product_buffer);	
  	    			abpdsTrans = ABPDSCreateTransition(innername,EQUAL,toStack1,tmp2);
    					abpdsTrans->next = abpds->trans;	
    					abpds->trans = abpdsTrans;
    					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
  	    			
  	    			tmp2 = ABPDSInitRightSideofTransition(innername,0,toStack1,0);    			  	    			
  	    			if(tmp) tmp->next = tmp2;
  	    			else tmp=tmp2;
  	    				  
  	    			abpdsTrans= ABPDSCreateTransition(name1,CONJ,fromStack,tmp);
    					abpdsTrans->next = abpds->trans;	
    					abpds->trans = abpdsTrans;
    					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
  	        }
  	      }
	   
	       	ABPDSInsertTransition(abpds, fromState, ftree->left, fromStack, toState, toStack1, toStack2);
	    		ABPDSInsertTransition(abpds, fromState, ftree->right, fromStack, toState, toStack1, toStack2);     	
	    	}
	    	return;
	    }	
	    case F_EW:	
	    { 
	    	if(ftree->right->token==F_FALSEEXP) 
	    	{
	    		ABPDSInsertTransition(abpds, fromState, ftree->left, fromStack, toState, toStack1, toStack2);
	    		return;
	    	}
	    	name1=create_product_name(fromState,ftree,0);
	    	name2=create_product_name(fromState,ftree->left,0);
	    	name3=create_product_name(toState,ftree,0);
	    	Name name4=create_product_name(fromState,ftree->right,0);
	    	ABPDSRightSideofTransition* rightSideList = ABPDSFindRightSide(abpds->transList, name1, fromStack);
	    	
	    	if(rightSideListLength)	    	
	    	{
	    		ABPDSRightSideofTransition tmp = NULL;
	    		short int i;	    		
	    		for (i =0; i<rightSideListLength; i++)
	    		{
	    			for (tmp = rightSideList[i]; tmp; tmp = tmp->next)
	    			{
	    				if (tmp->state == name3 && tmp->stack1== toStack1 && tmp->stack2==toStack2)
	    				{
	    					i =rightSideListLength;
	    					break;
	    				}
	    			}
	    		}
	    		if(!tmp)
	    		{ 
  	    		ABPDSRightSideofTransition tmp2 = NULL;
  	    		ABPDSTransition abpdsTrans; 
  	    			    			
  	    		if(ftree->right->token==F_TRUEEXP) 
  	    		{
  						tmp = ABPDSInitRightSideofTransition(name3,0,toStack1,toStack2); 
  	        	abpdsTrans= ABPDSCreateTransition(name1,EQUAL,fromStack,tmp);
    					abpdsTrans->next = abpds->trans;	
    					abpds->trans = abpdsTrans;
    					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);	    			
  	    			ABPDSInsertTransition(abpds, fromState, ftree->left, fromStack, toState, toStack1, toStack2);
  	    			return;
  	    		}
	    			
  	    		tmp= ABPDSInitRightSideofTransition(name4,0,fromStack,0);    			

  					if (toStack1 == 0)
      			{
      				innerStackCount++;	
      				sprintf(product_buffer,"%s.%d", "epsilon" ,innerStackCount);
      				tmp2 = ABPDSInitRightSideofTransition(name3,0,0,0);    			   		    		
  	    			Name innername = name_create(product_buffer);		
      				innerStateCount++;	
      				sprintf(product_buffer,"%s.%d", name_cstring(toState),innerStateCount);  
        			Name innerStateName = name_create(product_buffer);			
  	    			abpdsTrans = ABPDSCreateTransition(innerStateName,EQUAL, innername, tmp2);
    					abpdsTrans->next = abpds->trans;	
    					abpds->trans = abpdsTrans;
    					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
  	    			
  	    			tmp2 = ABPDSInitRightSideofTransition(innerStateName,0,innername,0); 
  	    			tmp->next = tmp2;
  	    				  
  	    			abpdsTrans= ABPDSCreateTransition(name1,CONJ, fromStack,tmp);
    					abpdsTrans->next = abpds->trans;	
    					abpds->trans = abpdsTrans;
    					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);	

      			}
						else
						{
							if (toStack2 ==0 )
    					{
    						tmp2 = ABPDSInitRightSideofTransition(name3,0,toStack1,0); 
    	    			tmp->next = tmp2;
    	        	abpdsTrans= ABPDSCreateTransition(name1,CONJ,fromStack,tmp);
      					abpdsTrans->next = abpds->trans;	
      					abpds->trans = abpdsTrans;
      					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);
    	        }
    	        else
    	        {	    		
        				innerStateCount++;	
        				sprintf(product_buffer,"%s.%d", name_cstring(toState),innerStateCount);  
    	    			 	    					    			
    	    			tmp2 = ABPDSInitRightSideofTransition(name3,0,toStack1,toStack2);  
    	    			Name innername = name_create(product_buffer);		
    	    			abpdsTrans = ABPDSCreateTransition(innername,EQUAL, toStack1,tmp2);
      					abpdsTrans->next = abpds->trans;	
      					abpds->trans = abpdsTrans;
      					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
    	    			tmp2 = ABPDSInitRightSideofTransition(innername,0,toStack1,0); 
    	    			tmp->next = tmp2;
    	    				  
    	    			abpdsTrans= ABPDSCreateTransition(name1,CONJ,fromStack,tmp);
      					abpdsTrans->next = abpds->trans;	
      					abpds->trans = abpdsTrans;
      					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
    	    		}	
  					}
	    			ABPDSInsertTransition(abpds, fromState, ftree->left, fromStack, toState, toStack1, toStack2);
	    			ABPDSInsertTransition(abpds, fromState, ftree->right, fromStack, toState, toStack1, toStack2);  					
  				}	 	
	    	}
	    	else
	    	{
	    		ABPDSRightSideofTransition tmp, tmp2;
	    		ABPDSTransition abpdsTrans;
	    		if(ftree->left->token==F_TRUEEXP)
	    		{
  	    		tmp = ABPDSInitRightSideofTransition(name4,0,fromStack,0);  	    		
  	        abpdsTrans= ABPDSCreateTransition(name1,EQUAL,fromStack,tmp);
  					abpdsTrans->next = abpds->trans;	
  					abpds->trans = abpdsTrans;
  					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 	    			
	    		}
	    		else 
	    		{	
	    			if(ftree->left->token!=F_FALSEEXP)
	    			{ 
	  	    		tmp = ABPDSInitRightSideofTransition(name4,0,fromStack,0);  	    		
  						tmp2= ABPDSInitRightSideofTransition(name2,0,fromStack,0);  					
  	    			tmp->next = tmp2;		    		
  	          abpdsTrans= ABPDSCreateTransition(name1,CONJ,fromStack,tmp);
  						abpdsTrans->next = abpds->trans;	
  						abpds->trans = abpdsTrans;
  						ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 	     		
  					}
	    		}
	    		if(ftree->right->token==F_TRUEEXP) 
  	    	{
  						tmp = ABPDSInitRightSideofTransition(name3,0,toStack1,toStack2); 
  	        	abpdsTrans= ABPDSCreateTransition(name1,EQUAL,fromStack,tmp);
    					abpdsTrans->next = abpds->trans;	
    					abpds->trans = abpdsTrans;
    					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans);	    			
  	    			ABPDSInsertTransition(abpds, fromState, ftree->left, fromStack, toState, toStack1, toStack2);
  	    			return;
  	    	}
	    		tmp = ABPDSInitRightSideofTransition(name4,0,fromStack,0); 
	    		tmp2 = NULL;
	    		
					if (toStack1 == 0)
    			{
    				innerStackCount++;	
    				sprintf(product_buffer,"%s.%d", "epsilon" ,innerStackCount);
    				 		 
    				tmp2 = ABPDSInitRightSideofTransition(name3,0,0,0); 
	    			Name innername = name_create(product_buffer);			
    				innerStateCount++;	

    				sprintf(product_buffer,"%s.%d", name_cstring(toState),innerStateCount);  
    				
      			Name innerStateName = name_create(product_buffer);			
	    			abpdsTrans= ABPDSCreateTransition(innerStateName, EQUAL, innername, tmp2);
  					abpdsTrans->next = abpds->trans;	
  					abpds->trans = abpdsTrans;
  					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
	    			
	    			tmp2 = ABPDSInitRightSideofTransition(innerStateName,0,innername,0); 
	    			tmp->next = tmp2;
	    				  
	    			abpdsTrans= ABPDSCreateTransition(name1,CONJ, fromStack,tmp);
  					abpdsTrans->next = abpds->trans;	
  					abpds->trans = abpdsTrans;
  					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 			
    			}
					else 
					{
						if (toStack2 ==0 )
  	    		{
  	    			tmp2 = ABPDSInitRightSideofTransition(name3,0,toStack1,0);  						
  	    			tmp->next = tmp2;	    		
  	        	abpdsTrans= ABPDSCreateTransition(name1,CONJ,fromStack,tmp);
    					abpdsTrans->next = abpds->trans;	
    					abpds->trans = abpdsTrans;
    					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
  	        }
  	        else
  	        {
       				innerStateCount++;	

      				sprintf(product_buffer,"%s.%d", name_cstring(toState),innerStateCount);  
	    		
  	    			tmp2 = ABPDSInitRightSideofTransition(name3,0,toStack1,toStack2);  	
  	    			Name innername = name_create(product_buffer);	
  	    			abpdsTrans= ABPDSCreateTransition(innername, EQUAL, toStack1,tmp2);
    					abpdsTrans->next = abpds->trans;	
    					abpds->trans = abpdsTrans;
    					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
  	    			
  	    			tmp2 = ABPDSInitRightSideofTransition(innername,0,toStack1,0);  
  	    			tmp->next = tmp2;
  	    				  
  	    			abpdsTrans= ABPDSCreateTransition(name1,CONJ,fromStack,tmp);
    					abpdsTrans->next = abpds->trans;	
    					abpds->trans = abpdsTrans;
    					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 		
  	    					
  	    		}        	
	        }   	
	        ABPDSInsertTransition(abpds, fromState, ftree->left, fromStack, toState, toStack1, toStack2);
	    	  ABPDSInsertTransition(abpds, fromState, ftree->right, fromStack, toState, toStack1, toStack2);	        	
	    	}
	    	return;
	    }
	    case F_AW:		      
	    {
	    	if(ftree->right->token==F_FALSEEXP) 
	    	{
	    		ABPDSInsertTransition(abpds, fromState, ftree->left, fromStack, toState, toStack1, toStack2);
	    		return;
	    	}	    	
	    	name1=create_product_name(fromState,ftree,0);
	    	name2=create_product_name(fromState,ftree->left,0);
	    	name3=create_product_name(toState,ftree,0);
	    	Name name4=create_product_name(fromState,ftree->right,0);	    	
	    	ABPDSRightSideofTransition* rightSideList = ABPDSFindRightSide(abpds->transList, name1, fromStack);
	    	if(rightSideListLength)
	    	{
	    		ABPDSRightSideofTransition tmp;
	    		short int i;
	    		//int index;
	    		if(ftree->left->token==F_TRUEEXP && ftree->right->token==F_TRUEEXP)	return;
	    		
	    		for (i = 0; i<rightSideListLength; i++)
	    		{
	    			if (rightSideList[i]->state==name2) continue;
	    			if( rightSideList[i]->next)	{ if(rightSideList[i]->next->state==name2) continue; }
						for (tmp = rightSideList[i]; tmp; tmp= tmp->next )
						{
	    				if (tmp->state == name3 && tmp->stack1== toStack1 && tmp->stack2==toStack2 ) break;
						}
						if (!tmp)
						{
							
  						ABPDSTransition abpdsTrans;
     					if (toStack1 == 0)
        			{
        				innerStackCount++;	
        				sprintf(product_buffer,"%s.%d", "epsilon",innerStackCount);
	 		    		
        				tmp = ABPDSInitRightSideofTransition(name3,0,0,0);  		
    	    			Name innername = name_create(product_buffer);
        				innerStateCount++;	
        				sprintf(product_buffer,"%s.%d", name_cstring(toState),innerStateCount);  

          			Name innerStateName = name_create(product_buffer);			
    	    			abpdsTrans = ABPDSCreateTransition(innerStateName, EQUAL, innername, tmp);

      					abpdsTrans->next = abpds->trans;	
      					abpds->trans = abpdsTrans;
      					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
    	    			tmp =ABPDSInitRightSideofTransition(innerStateName,0,innername,0);  
    	    			tmp->next = rightSideList[i]->next;
  	    				rightSideList[i]->next = tmp;   								
        			}
  						else 
  						{
  							if ( toStack2 == 0 )
    						{ 
    							tmp = ABPDSInitRightSideofTransition(name3,0,toStack1,0);  		
    	    				tmp->next = rightSideList[i]->next;
    	    				rightSideList[i]->next = tmp;
    	        	}	 
    	        	else
    	        	{
          				innerStateCount++;	
          				sprintf(product_buffer,"%s.%d", name_cstring(toState),innerStateCount);  
       			      tmp = ABPDSInitRightSideofTransition(name3,0,toStack1,toStack2);
    	    				Name innername = name_create(product_buffer);		
    	    				abpdsTrans = ABPDSCreateTransition(innername,EQUAL, toStack1,tmp);
        					abpdsTrans->next = abpds->trans;	
        					abpds->trans = abpdsTrans;
        					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
    	    				tmp = ABPDSInitRightSideofTransition(innername,0,toStack1,0);
    	    				tmp->next = rightSideList[i]->next;
    	    				rightSideList[i]->next = tmp;   								
    	    			}
  	    			}
  	    			ABPDSInsertTransition(abpds, fromState, ftree->left, fromStack, toState, toStack1, toStack2);
	    				ABPDSInsertTransition(abpds, fromState, ftree->right, fromStack, toState, toStack1, toStack2);        		
	        	}	
	    			break;
	    		}
	    	}
	    	else
	    	{
	    		ABPDSRightSideofTransition tmp, tmp2;
	    		if(ftree->left->token==F_TRUEEXP)
	    		{
  	    		tmp = ABPDSInitRightSideofTransition(name4,0,fromStack,0);  	    		
  	        ABPDSTransition abpdsTrans= ABPDSCreateTransition(name1,EQUAL,fromStack,tmp);
  					abpdsTrans->next = abpds->trans;	
  					abpds->trans = abpdsTrans;
  					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 	    			
	    		}
	    		else 
	    		{	
	    			if(ftree->left->token!=F_FALSEEXP)
	    			{ 
	  	    		tmp = ABPDSInitRightSideofTransition(name4,0,fromStack,0);  	    		
  						tmp2= ABPDSInitRightSideofTransition(name2,0,fromStack,0);  					
  	    			tmp->next = tmp2;		    		
  	        	ABPDSTransition abpdsTrans= ABPDSCreateTransition(name1,CONJ,fromStack,tmp);
  						abpdsTrans->next = abpds->trans;	
  						abpds->trans = abpdsTrans;
  						ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 	     		
  					}
	    		}   		
	    		
	    		tmp = ftree->right->token==F_TRUEEXP? NULL : ABPDSInitRightSideofTransition(name4,0,fromStack,0);
	    		tmp2= NULL;

 					if (toStack1 == 0)
    			{
    				innerStackCount++;	
    				sprintf(product_buffer,"%s.%d",  "epsilon" ,innerStackCount);	
    				 		    		
    				tmp2 =  ABPDSInitRightSideofTransition(name3,0,0,0);
	    			Name innername = name_create(product_buffer);
    				innerStateCount++;	

    				sprintf(product_buffer,"%s.%d", name_cstring(toState),innerStateCount);  
      			Name innerStateName = name_create(product_buffer);			
	    			ABPDSTransition abpdsTrans = ABPDSCreateTransition(innerStateName,EQUAL, innername, tmp2);
  					abpdsTrans->next = abpds->trans;	
  					abpds->trans = abpdsTrans;
  					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
	    			
	    			tmp2 = ABPDSInitRightSideofTransition(innerStateName,0,innername,0);
	    			if(tmp) tmp->next = tmp2;
	    			else tmp=tmp2;
	    			abpdsTrans = ABPDSCreateTransition(name1,CONJ,fromStack,tmp);
  					abpdsTrans->next = abpds->trans;	
  					abpds->trans = abpdsTrans;
  					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 					
    			}
					else 
					{
						if ( toStack2 == 0 )
  					{    
  						tmp2 =  ABPDSInitRightSideofTransition(name3,0,toStack1,0);		
  	    			if(tmp) tmp->next = tmp2;
  	    			else tmp=tmp2;
  	    			ABPDSTransition abpdsTrans = ABPDSCreateTransition(name1,CONJ,fromStack,tmp);
    					abpdsTrans->next = abpds->trans;	
    					abpds->trans = abpdsTrans;
    					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
  	        }	 
  	        else
  	        {
      				innerStateCount++;	

      				sprintf(product_buffer,"%s.%d", name_cstring(toState),innerStateCount);  
     					
     					tmp2 =  ABPDSInitRightSideofTransition(name3,0,toStack1,toStack2);
  	    			Name innername = name_create(product_buffer);			
  	    			ABPDSTransition abpdsTrans = ABPDSCreateTransition(innername,EQUAL,toStack1,tmp2);
    					abpdsTrans->next = abpds->trans;	
    					abpds->trans = abpdsTrans;
    					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
  	    			
  	    			tmp2 = ABPDSInitRightSideofTransition(innername,0,toStack1,0);
  	    			if(tmp) tmp->next = tmp2;
  	    			else tmp=tmp2;
  	    			abpdsTrans = ABPDSCreateTransition(name1,CONJ,fromStack,tmp);
    					abpdsTrans->next = abpds->trans;	
    					abpds->trans = abpdsTrans;
    					ABPDSAppend2TransListFWD(abpds->transList,abpdsTrans); 
  	    		}   		
	        }		           		 	
	        ABPDSInsertTransition(abpds, fromState, ftree->left, fromStack, toState, toStack1, toStack2);
	    		ABPDSInsertTransition(abpds, fromState, ftree->right, fromStack, toState, toStack1, toStack2);        	
	    	}
	    	return;
	    }
	    case F_IDENT:
	    case F_FALSEEXP:
	    case F_TRUEEXP:
	    case F_NOT:
				return; 
	    default:
				common_error("unknown token (%i)",ftree->token);
		return;
 		}
	}			
}			   
			
/* Function ABPDSPrintTransition prints a transition rule.
   Input: A transition rule. */
void	ABPDSPrintTransition(ABPDSTransition trans)
{
	ABPDSRightSideofTransition rightSide = trans->rightSide;

	if(mc_CTPL) 
		{
			printf("<%s,%s> --> {",name_cstring(trans->fromState),name_cstring(trans->fromStack));
			//fprintf(file,"<%s,%s> --> {",name_cstring(trans->fromState),name_cstring(trans->fromStack));
		}
	else {printf("<%s,%s> --> {",name_cstring(trans->fromState),name_cstring(trans->fromStack));
	//fprintf(file,"<%s,%s> --> {",name_cstring(trans->fromState),name_cstring(trans->fromStack));
	}

	while(rightSide)
	{
		if (rightSide->stack2)
		{
			if(rightSide->aux.rel) {
				printf("<[%s,%s],%s %s>", name_cstring(rightSide->state),name_cstring(rightSide->aux.rel), name_cstring(rightSide->stack1),name_cstring(rightSide->stack2));
				//fprintf(file,"<[%s,%s],%s %s>", name_cstring(rightSide->state),name_cstring(rightSide->aux.rel), name_cstring(rightSide->stack1),name_cstring(rightSide->stack2));
			}
			else{
				printf("<%s,%s %s>", name_cstring(rightSide->state),name_cstring(rightSide->stack1),name_cstring(rightSide->stack2));
				//fprintf(file,"<%s,%s %s>", name_cstring(rightSide->state),name_cstring(rightSide->stack1),name_cstring(rightSide->stack2));
			}			
		}
		else
		{
			if(rightSide->aux.rel) 
			{
				printf("<[%s,%s],%s>", name_cstring(rightSide->state),name_cstring(rightSide->aux.rel),name_cstring(rightSide->stack1));
				//fprintf(file,"<[%s,%s],%s>", name_cstring(rightSide->state),name_cstring(rightSide->aux.rel),name_cstring(rightSide->stack1));
			}	
			else {
				printf("<%s,%s>", name_cstring(rightSide->state),name_cstring(rightSide->stack1));
				//fprintf(file,"<%s,%s>", name_cstring(rightSide->state),name_cstring(rightSide->stack1));
			}
		}
		rightSide = rightSide->next;
	}
	printf("}\n");
	//fprintf(file,"}\n");
}

/* Function ABPDSCreateTransition builds an transition rule.
   Input: FormulaTree of the left-hand side of the transition rule,
   	      The stack of the left-hand side of the transition rule,
   	      The right-hand sideo f ABPDS transition rule.*/  
ABPDSTransition	ABPDSCreateTransition(Name fromState, Relation rel, Name fromStack, ABPDSRightSideofTransition rightSide)
{
		ABPDSTransition newTrans = common_malloc(sizeof(struct ABPDSTransitionStruct));
		newTrans->fromState = fromState;
		newTrans->fromStack = fromStack;
    newTrans->rightSide = rightSide;
    newTrans->label = NULL;
    newTrans->rel = rel;
    newTrans->bdd =NULL;
    newTrans->next =NULL;
    newTrans->var =0;
    
    return newTrans;
}

  
/*int CheckInclusion(ABPDSTransition list, ABPDSTransition trans)
{
	assert(list&&trans);
	for(; list ;list = list->next)
	{
		if(list->fromState==trans->fromState && list->fromStack==trans->fromStack \
			&& list->formula==trans->formula&& list->faState==trans->faState && list->bdd==trans->bdd)
			return 1;
	}
	return 0;
} */

/* Function ABPDSDestoryTransition deletes an transition rule.
   Input: A transition rule of ABPDS*/     	   
void	ABPDSDestoryTransition (ABPDSTransition trans)
{
	
	ABPDSDestoryRightSideofTransition(trans->rightSide);
	
	ABPDSTransLabel tmp;
	while(trans->label)
	{
		tmp = trans->label->next;
		if(trans->label->Binding) Cudd_RecursiveDeref(CTLBDDMgr,trans->label->Binding);
		common_free(trans->label, sizeof(struct ABPDSTransitionLabelStruct));
		trans->label =tmp;
	}
	if(trans->bdd) Cudd_RecursiveDeref(CTLBDDMgr,trans->bdd);
	common_free(trans,sizeof(struct ABPDSTransitionStruct));
}



/* Function ABPDSDestoryTransList deletes the transition list.
   Input: A transition list*/  

void  ABPDSDestoryTransList(ABPDSTransition transList)
{
	ABPDSTransition tmp;
	while(transList)
	{
		tmp = transList->next;
		ABPDSDestoryTransition(transList);
		transList = tmp;
	}
}



/* Function ABPDSAppend2TransList inserts a transition rule into the transition list and update the lasttrans.
   Input: An ABPDSTransList,
   	      A transition rule will be inserted,
   	      Last transition of the transition list.*/ 
void	ABPDSAppend2TransList(ABPDSHeadTable table, ABPDSTransition trans) 	      
{	
	if (!trans) return;
	if (mc_verbose > 2 || isdebug) 
	{
		ABPDSPrintTransition(trans);
		ABPDSRightSideofTransition rightSide = trans->rightSide;
	if(mc_CTPL) 
		{
			fprintf(file,"<%s,%s> --> {",name_cstring(trans->fromState),name_cstring(trans->fromStack));
		}
	else {
			fprintf(file,"<%s,%s> --> {",name_cstring(trans->fromState),name_cstring(trans->fromStack));
	}

	while(rightSide)
	{
		if (rightSide->stack2)
		{
			if(rightSide->aux.rel) {
				fprintf(file,"<[%s,%s],%s %s>", name_cstring(rightSide->state),name_cstring(rightSide->aux.rel), name_cstring(rightSide->stack1),name_cstring(rightSide->stack2));
			}
			else{
				fprintf(file,"<%s,%s %s>", name_cstring(rightSide->state),name_cstring(rightSide->stack1),name_cstring(rightSide->stack2));
			}			
		}
		else
		{
			if(rightSide->aux.rel) 
			{
				fprintf(file,"<[%s,%s],%s>", name_cstring(rightSide->state),name_cstring(rightSide->aux.rel),name_cstring(rightSide->stack1));
			}	
			else {
				fprintf(file,"<%s,%s>", name_cstring(rightSide->state),name_cstring(rightSide->stack1));
			}
		}
		rightSide = rightSide->next;
	}
	fprintf(file,"}\n");
		}

	ABPDSRightSideofTransition tmp=trans->rightSide;
	if(tmp->next)
	{
		if(tmp->state==tmp->next->state&& tmp->stack1==tmp->next->stack1)
		{
	    ABPDSHead head =ABPDS_head_create(tmp->state,tmp->stack1);
    	head =ABPDS_head_table_insert(table, head);
    	head->rules=create_rule(trans,head->rules);			
			return;
		}
	}			
	for(tmp=trans->rightSide; tmp; tmp=tmp->next)
	{
  	ABPDSHead head =ABPDS_head_create(tmp->state,tmp->stack1);
  	head =ABPDS_head_table_insert(table, head);
  	head->rules=create_rule(trans,head->rules);
  }
}

void	ABPDSAppend2TransListFWD(ABPDSHeadTable table, ABPDSTransition trans) 	      
{	
		if (!trans) return;
		if (mc_verbose > 2 || isdebug) ABPDSPrintTransition(trans);
		
    ABPDSHead head =ABPDS_head_create(trans->fromState,trans->fromStack);
    head =ABPDS_head_table_insert(table, head);
    head->rules=create_rule(trans,head->rules);
    abpds_trans_num++;
    
   // printf("abpds_trans_num: %d\n",abpds_trans_num);
}

ABPDSRule create_rule(ABPDSTransition trans, ABPDSRule rules)
{
	ABPDSRule rule=common_malloc(sizeof (struct ABPDSRuleStruct));
	rule->rule=trans;
	rule->next=rules;
	return rule;
}

int	ABPDSStrictAppend2TransList(ABPDSHeadTable table, ABPDSTransition trans)
{
	ABPDSRightSideofTransition tmp=trans->rightSide;
	ABPDSHead head =ABPDS_head_create(tmp->state,tmp->stack1);
	head =ABPDS_head_table_insert(table, head);
	ABPDSRule rule=head->rules;
	ABPDSTransition tmp1;
	
	for(; rule; rule=rule->next)
	{
		tmp1=rule->rule;
		if(!tmp1) continue;
		if(tmp1->fromState==trans->fromState && tmp1->fromStack==trans->fromStack \
			&&tmp1->rel==trans->rel && tmp1->bdd==trans->bdd )
			{
				ABPDSRightSideofTransition tmp3,tmp4;
				for(tmp3=trans->rightSide; tmp3; tmp3=tmp3->next)
				{
					for(tmp4=tmp1->rightSide; tmp4; tmp4=tmp4->next)
					{
						if(tmp3->state==tmp4->state && tmp3->index==tmp4->index &&tmp3->aux.Binding==tmp4->aux.Binding \
							&& tmp3->stack1==tmp4->stack1) break;
					}
					if(!tmp4) break;
				}
				if(tmp3) continue;

				for(tmp3=tmp1->rightSide; tmp3; tmp3=tmp3->next)
				{
					for(tmp4=trans->rightSide; tmp4; tmp4=tmp4->next)
					{
						if(tmp3->state==tmp4->state && tmp3->index==tmp4->index \
							&&tmp3->aux.Binding==tmp4->aux.Binding &&	tmp3->stack1==tmp4->stack1 ) break;
					}
					if(!tmp4) break;
				}
				if(tmp3) continue;	
			  
				ABPDSTransLabel tmp5,tmp6;
				for(tmp5=trans->label; tmp5; tmp5=tmp5->next)
				{
					for(tmp6=tmp1->label; tmp6; tmp6=tmp6->next)
					{
						if(tmp5->state==tmp5->state&&	tmp5->index==tmp6->index &&tmp5->Binding==tmp6->Binding) break;
					}
					if(!tmp6) break;
				}
				if(tmp5) continue;

				for(tmp5=tmp1->label; tmp5; tmp5=tmp5->next)
				{
					for(tmp6=trans->label; tmp6; tmp6=tmp6->next)
					{
						if(tmp5->state==tmp6->state &&tmp5->index==tmp6->index &&tmp5->Binding==tmp6->Binding) break;
					}
					if(!tmp6) break;
				}
				if(tmp5) continue;	
				ABPDSDestoryTransition(trans);	
				return 0;									
			}
	}
	ABPDSRightSideofTransition tmp3=trans->rightSide;
	if(tmp3->next)
	{
		if(tmp3->state==tmp3->next->state && tmp3->stack1==tmp3->next->stack1)
		{
	    ABPDSHead head =ABPDS_head_create(tmp3->state,tmp3->stack1);
    	head =ABPDS_head_table_insert(table, head);
    	if(!head->rules) head->rules=create_rule(trans,NULL);
    	else
    	{
    		rule=head->rules;
    		while(rule->next) rule=rule->next;
    		rule->next=create_rule(trans,NULL);
    	}			
			return 1;
		}
	}		
	for(; tmp3; tmp3=tmp3->next)
	{
  	ABPDSHead head =ABPDS_head_create(tmp3->state,tmp3->stack1);
  	head =ABPDS_head_table_insert(table,head);
  	if(!head->rules) head->rules=create_rule(trans,NULL);
  	else
  	{
  		rule=head->rules;
  		while(rule->next) rule=rule->next;
  		rule->next=create_rule(trans,NULL);	
  	}
	}
	return 1;
}

void RemoveHeadTable(ABPDSHeadTable table, ABPDSTransition trans)
{
	ABPDSHead head;
	ABPDSRule rule;
//	ABPDSTransition tmp1;
	
	ABPDSRightSideofTransition tmp3=trans->rightSide;
	for(; tmp3; tmp3=tmp3->next)
	{
    head =ABPDS_head_create(tmp3->state,tmp3->stack1);
  	head =ABPDS_head_table_insert(table,head);
  	if(head->rules) 
  	{
  		rule=head->rules;
  		while(rule)
  		{
  			if(rule->rule==trans)
  			{
  				rule->rule=NULL; 
  				break; 
  			}
  			rule=rule->next;
  		}
  	}
	}
}
/* Function ABPDSInitRightSideofTransition creats a initial right-hand side of a transition.
   Output:  A ABPDSRightSideofTransition .*/ 

ABPDSRightSideofTransition ABPDSInitRightSideofTransition(Name state, int index, Name stack1, Name stack2)
{
	 ABPDSRightSideofTransition  rightSide= common_malloc(sizeof(struct ABPDSRightSideofTransitionStruct));
	 rightSide->state = state;
	 rightSide->stack1 = stack1;
	 rightSide->stack2 = stack2;
	 rightSide->next = NULL;
	 rightSide->index = index;
	 rightSide->aux.Binding = NULL;
	 return rightSide;
}

/* Function ABPDSFindTransition look up an transition rule whose 
	 left-hand side is <[Name,FormulaTree],Name>. There maybe more than one transition 
	 from <[Name,FormulaTree],Name>, the function return the first found transition.
   Input: Address points to a transition list.
   				The state name of the left-hand side.
   				FormulaTree of left-hand side.
   				Stack Name of left-hand side.
   Output: the Transition pointer if found, otherwise NULL*/   
/*ABPDSTransition	ABPDSFindTransition (ABPDSTransList* transList, Name state, FormulaTree ftree, Relation rel, Name stack)
{
	ABPDSTransition tmp = *transList ; 
	
	for (; tmp; tmp = tmp->next)
	{	
		if (tmp->fromState == state && tmp->fromStack == stack && tmp->formula == ftree && rel == tmp->rel)
		{
			return tmp;
		}
	}
	return NULL;
} */ 
 
/* Function ABPDSDestoryRightSideofTransition deletes the right-hand side of a transition.
   Input:  A ABPDSRightSideofTransition.*/ 
void 	ABPDSDestoryRightSideofTransition(ABPDSRightSideofTransition rightSide)
{
	ABPDSRightSideofTransition tmp;
	while(rightSide)
	{
		tmp = rightSide->next;
		if(rightSide->aux.Binding&& rightSide->index&& mc_CTPL) Cudd_RecursiveDeref(CTLBDDMgr,rightSide->aux.Binding);
		common_free(rightSide, sizeof(struct ABPDSRightSideofTransitionStruct));
		rightSide = tmp;
	}
}



/* Function ABPDSInitAcceptingStateList creats a initial accepting state list.
   Output:  A ABPDSAcceptingStateList.*/ 
ABPDSAcceptingStateList  ABPDSInitAcceptingStateList()
{
	ABPDSAcceptingStateList stateList = common_malloc(sizeof(struct ABPDSAcceptingStateListStruct));
	stateList->state = 0;
	stateList->Binding = NULL;
	stateList->bal=0;
  stateList->right =stateList->left= NULL;
  return stateList;
}


 
/* Function ABPDSDestoryAcceptingStateList deletes a accepting state list.
   Input:  A ABPDSAcceptingStateList.*/ 
void 	ABPDSDestoryAcceptingStateList(ABPDSAcceptingStateList stateList)
{
	if(!stateList) return;
  ABPDSDestoryAcceptingStateList(stateList->left);
  ABPDSDestoryAcceptingStateList(stateList->right);
  if(stateList->Binding) Cudd_RecursiveDeref(CTLBDDMgr,stateList->Binding);
	common_free(stateList, sizeof(struct ABPDSAcceptingStateListStruct));
}

/* Function ABPDSCheckStateAcceptiness checks a state of ABPDS is accepting or not.
   Input:  A (ABPDSAcceptingStateList.
   	 			 The name of a state.
   	 			 FormulaTree a part of the state of ABPDS.
   Output: True if the state is acceping, otherwise false.*/
   	
DdNode* ABPDSCheckStateAcceptiness(ABPDSAcceptingStateList tree, Name state)
{
	while (tree)
	{
		if (tree->state == state) return (tree->Binding==NULL? (DdNode*) 1: tree->Binding);
		tree = (tree->state > state)? tree->left : tree->right;
	}
	return (DdNode*) 0;
}

/* Function ABPDSAppend2AcceptingStateList sets a state of ABPDS to be acceping.
   Input:  A state list.
   	 			 The name of a state.
   	 			 A FormulaTree.*/ 
   	 
signed char 	ABPDSAppend2AcceptingStateList(ABPDSAcceptingStateList* tree, Name state, DdNode* bdd)
{
	signed char obal, diff, nbal;

	if (!*tree)
	{
		*tree = ABPDSInitAcceptingStateList();
		(*tree)->state = state;
		(*tree)->Binding =bdd;
		(*tree)->bal = 0;
		(*tree)->left = (*tree)->right = NULL;
		return 1;
	}

	if ((*tree)->state == state) 
	{
		if((*tree)->Binding)	Cudd_RecursiveDeref(CTLBDDMgr,(*tree)->Binding);	
		(*tree)->Binding=bdd;
		return 0;
	}
	obal = (*tree)->bal;

	if ((*tree)->state > state)
		diff = -ABPDSAppend2AcceptingStateList(&((*tree)->left),state,bdd);
	else
		diff = ABPDSAppend2AcceptingStateList(&((*tree)->right),state,bdd);

	nbal = ((*tree)->bal += diff);

	if (nbal == 0) return 0;
	if (nbal == 1 || nbal == -1) return !!diff;

	if ((*tree)->bal == -2)
	{
		ABPDSAcceptingStateList a = *tree;
		ABPDSAcceptingStateList b = a->left;
		ABPDSAcceptingStateList c = b->right;

		if (b->bal < 0)
		{
/*printf("rebalancing on %d, case 1\n",(*tree)->name); */
			*tree = b;
			b->right = a;
			a->left = c;
			a->bal = b->bal = 0;
			return 0;
		}
		else
		{
			ABPDSAcceptingStateList f = c->left;
			ABPDSAcceptingStateList g = c->right;

/* printf("rebalancing on %d, case 2\n",(*tree)->name); */
			*tree = c;
			c->left = b;
			c->right = a;
			a->left = g;
			b->right = f;
			a->bal = (c->bal == -1)? 1 : 0;
			b->bal = (c->bal == 1)? -1 : 0;
			c->bal = 0;
			return 0;
		}
	}

	if ((*tree)->bal == 2)
	{
		ABPDSAcceptingStateList a = *tree;
		ABPDSAcceptingStateList b = a->right;
		ABPDSAcceptingStateList c = b->left;

		if (b->bal > 0)
		{
/* printf("rebalancing on %d, case 3\n",(*tree)->name); */
			*tree = b;
			b->left = a;
			a->right = c;
			a->bal = b->bal = 0;
			return 0;
		}
		else
		{
			ABPDSAcceptingStateList f = c->left;
			ABPDSAcceptingStateList g = c->right;

/* printf("rebalancing on %d, case 4\n",(*tree)->name); */
			*tree = c;
			c->left = a;
			c->right = b;
			a->right = f;
			b->left = g;
			a->bal = (c->bal == 1)? -1 : 0;
			b->bal = (c->bal == -1)? 1 : 0;
			c->bal = 0;
			return 0;
		}
	}

	common_error("name_bal_insert: strange bal value; can't happen??");
	return 0;
}



ABPDSRightSideofTransition* ABPDSFindRightSide(ABPDSHeadTable table, Name state, Name stack)
{
	ABPDSHead head=ABPDS_head_find(table,state,stack);
	static ABPDSRightSideofTransition* transArray;
	rightSideListLength = 0;
	if(!head) return NULL;
		
	ABPDSRule tmp=head->rules;
	while (tmp)
	{
		rightSideListLength++;
		if(rightSideListLengthMax==0)
		{
			rightSideListLengthMax+=DynamicArrayStep;
			transArray = common_malloc(rightSideListLengthMax*sizeof(ABPDSRightSideofTransition));
		}
  	else
  	{
  		if(rightSideListLength>rightSideListLengthMax)
  		{
				rightSideListLengthMax+=DynamicArrayStep;
  			transArray = common_realloc(transArray, rightSideListLengthMax*sizeof(ABPDSRightSideofTransition),DynamicArrayStep*sizeof(ABPDSRightSideofTransition)); 
			}
		} 
		transArray[rightSideListLength-1] = tmp->rule->rightSide;			
	 	tmp = tmp->next;	
	}
	return transArray;
}

void ABPDSFindOneRightSide(ABPDSHeadTable table, Name state, Name stack)
{
	ABPDSHead head=ABPDS_head_find(table,state,stack);
	if(!head) rightSideListLength=0;
	else
		rightSideListLength = head->rules? 1: 0;
	return;
}


void ABPDSInsert2StackList(ABPDSStackList* l, Name stack)
{
	
	ABPDSStackList tmp = *l;
	while(tmp)
	{
		if(tmp->stack == stack) return;
		tmp = tmp->next;
	}
	if (!tmp)
	{
		ABPDSStackList newStack = common_malloc(sizeof(struct ABPDSStackListStruct));
		newStack->stack = stack;
		newStack->next = (*l);
		(*l) = newStack;
		
		ValList tmp1;
		for(tmp1=abpdsValList; tmp1; tmp1=tmp1->next)
		{
			if(tmp1->name==stack) return;
		}
		if(!tmp1)
		{	
			tmp1 = common_malloc(sizeof(struct ValueListStruct));
			tmp1->name =stack;
			tmp1->next = abpdsValList;
			abpdsValNum++;
			abpdsValList = tmp1;
		}		
	}
	stack_num++;
}

void ABPDSInsert2StateList(ABPDSCtrlLocation* l, Name state)
{
	ABPDSCtrlLocation tmp = (*l);
	while(tmp)
	{
		if(tmp->state == state) return;
		tmp = tmp->next;
	}
	if (!tmp)
	{
		ABPDSCtrlLocation newState = common_malloc(sizeof(struct ABPDSControlLocationStruct));
		newState->state = state;
		newState->next =  (*l);
		 (*l) = newState;
	}	
	state_num++;
}


/* Function ABPDSDestoryStackList deletes the Stack List.
   Input: A stack list.*/
void ABPDSDestoryStackList(ABPDSStackList l)
{
	ABPDSStackList tmp =NULL;
	while (l)
	{
		tmp = l;
		l = l->next;
		common_free(tmp, sizeof(struct ABPDSStackListStruct ));
	}
}



/* Function ABPDSDestoryStateList deletes State List.
   Input: A state list.*/
void ABPDSDestoryStateList(ABPDSCtrlLocation l)
{
	ABPDSCtrlLocation tmp =NULL;
	while (l)
	{
		tmp = l;
		l = l->next;
		common_free(tmp, sizeof(struct ABPDSControlLocationStruct ));
	}
}


ABPDSTransLabel CreateABPDSTransLabel(Name state, DdNode* bdd, int index)
{
	ABPDSTransLabel r =common_malloc(sizeof(struct ABPDSTransitionLabelStruct));
	r->state =state;
	r->Binding =bdd;
	r->index =index; 
	r->next =NULL;	
	return r;
}

ABPDSHeadTable ABPDS_head_table_empty()
{
	ABPDSHeadTable new_table=common_malloc(sizeof(struct ABPDSHeadTableStruct));
	int i;

	new_table->data = common_malloc(ABPDS_HEAD_HASH_MIN * sizeof(ABPDSHead*));
	new_table->size = ABPDS_HEAD_HASH_MIN;
	new_table->mask = ABPDS_HEAD_HASH_MIN - 1;
	new_table->fill = 0;

	for (i = 0; i < new_table->size; i++) new_table->data[i] = NULL;
	return new_table;
}



ABPDSHead ABPDS_head_create(Name state, Name stack)
{
	ABPDSHead newhead = common_malloc(sizeof(struct ABPDSHeadStruct));

	newhead->state  = state;
	newhead->stack   = stack;

	newhead->rules  = NULL;	/* contains list of neighbouring heads */
	newhead->next   = NULL;	/* for use inside hash table */

	newhead->hash  = compute_hash_value(state,stack);

	return newhead;
}


void ABPDS_head_delete (ABPDSHead head)
{
	ABPDSRule tmp;
	while(head->rules)
	{
		tmp=head->rules->next;
		//if(preStarChecking) ABPDSDestoryTransition(tmp->rule);
		common_free(head->rules,sizeof(struct ABPDSRuleStruct));
		head->rules =tmp;
	}
	common_free(head,sizeof(struct ABPDSHeadStruct));
}

int  ABPDS_head_compare (ABPDSHead h1, ABPDSHead h2)
{
	int cmp;
	if ((cmp = name_compare(h1->state,h2->state))) return cmp;  // list is an descend chain
	return name_compare(h1->stack,h2->stack);
}


void ABPDS_head_add_rule (ABPDSHead head, ABPDSTransition trans)
{
	ABPDSRule rule=common_malloc(sizeof(struct ABPDSRuleStruct));
  rule->rule=trans;
  rule->next =head->rules;
  head->rules =rule;
}


void ABPDS_head_table_delete (ABPDSHeadTable table)
{
	long i;

	for (i = 0; i < table->size; i++)
	{
		ABPDSHead list = table->data[i];
		while (list)
		{
			ABPDSHead nexthead = list->next;
			ABPDS_head_delete(list);
			list = nexthead;
		}
	}

	common_free(table->data,table->size * sizeof(Head*));
	common_free(table,sizeof(struct ABPDSHeadTableStruct));
}


ABPDSHead ABPDS_head_list_insert (ABPDSHead *list, ABPDSHead newhead, int *newentry)
{
	ABPDSHead last_entry = NULL;
	ABPDSHead next_entry = *list;
	int cmp = 1;

	while (next_entry)
	{
		cmp = ABPDS_head_compare(next_entry,newhead);
		if (cmp >= 0) break;
		last_entry = next_entry;
		next_entry = next_entry->next;
	}

	if ((*newentry = cmp))
	{
		if (last_entry)
		{
			newhead->next = next_entry;
			last_entry->next = newhead;
		}
		else
		{
			newhead->next = *list;
			*list = newhead;
		}
		return newhead;
	}
	else
	{
		ABPDS_head_delete(newhead);
		return next_entry;
	}
}


ABPDSHead ABPDS_head_table_insert(ABPDSHeadTable table, ABPDSHead head)
{
	int hash = head->hash & table->mask;
	int newtrans, i;

	head = ABPDS_head_list_insert(&(table->data[hash]), head, &newtrans);

	if (!newtrans) return head;

	//head->list = table->list;
	//table->list = head;

	if (table->fill++ < table->size * 3/4) return head;

	table->data = common_realloc(table->data, table->size*2 * sizeof(ABPDSHead*),
				     table->size * sizeof(Head*));

	table->mask = table->size * 2 - 1;

	for (i = 0; i < table->size; i++)
	{
		ABPDSHead l1 = NULL, l2 = NULL;
		ABPDSHead last_entry, next_entry;

		last_entry = table->data[i];
		while (last_entry)
		{
			next_entry = last_entry->next;
			if ((last_entry->hash & table->mask) == (unsigned long)i)
			{
				last_entry->next = l1;
				l1 = last_entry;
			}
			else
			{
				last_entry->next = l2;
				l2 = last_entry;
			}
			last_entry = next_entry;
		}

		last_entry = NULL;
		while (l1)
		{
			next_entry = l1->next;
			l1->next = last_entry;
			last_entry = l1;
			l1 = next_entry;
		}
		table->data[i] = last_entry;

		last_entry = NULL;
		while (l2)
		{
			next_entry = l2->next;
			l2->next = last_entry;
			last_entry = l2;
			l2 = next_entry;
		}
		table->data[i + table->size] = last_entry;
	}

	table->size *= 2;
	return head;
}


ABPDSHead ABPDS_head_find(ABPDSHeadTable table, Name state, Name stack)
{
	ABPDSHead head = ABPDS_head_create(state,stack);
	ABPDSHead list = table->data[head->hash & table->mask];

	while (list)
	{
		int cmp = ABPDS_head_compare(list,head);              // list is an descend chain
		if (cmp > 0) list = NULL;
		if (cmp >= 0) break;
		list = list->next;
	}
	ABPDS_head_delete(head);
	return list;
}

unsigned long compute_hash_value(Name state,Name stack)
{
	return state * state + stack * stack * 2;
}


Name create_product_name(Name state, FormulaTree ftree , int faState)
{
	char *b1;
	int l1;
	char *c1=state? name_cstring(state): NULL;

	common_strbuf_new();
	CTLPrintFormula(ftree);
	
	b1 = common_malloc(l1 = (c1?strlen(c1):0) + (ftree?strlen(common_strbuf):0) + sizeof(int)+5);
	sprintf(b1,"[%s,%s,%d]",c1,common_strbuf, faState);
	Name ret= name_create(b1);
	common_free(b1,l1);
	return ret;
}
