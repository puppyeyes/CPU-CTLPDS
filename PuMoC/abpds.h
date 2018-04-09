/* =========================================================================
   Extend Moped: abpds.h 
   	
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
   
   This file defines the data structures and functions for Alternating Buchi
   Pushdown System (ABPDS)
   a) Assumption: Each transition rule (p,r)->R of the ABPDS,
   		  If |R|={(p',w')}, then |w'|<3
   		  If |R|>1, then for every (p',w') in R, |w'|=1.
   			
   Notes that any ABPDS can equivalently translate to another ABPDS 
   which satisfies this assumption. These task is done in the product procedure.   

   ====================================================================== */

#ifndef ABPDS_H
#define ABPDS_H

#include "name.h"
#include "ftree.h"
#include "bdd.h"
#define ABPDS_HEAD_HASH_MIN 1024

/* -------------------------- Definitions  Datatypes   --------------------------------- */
               
typedef struct AlternatingBuChiPushdownSystemStruct AlternatingBuchiPushdownSystem;
typedef struct ABPDSTransitionStruct* ABPDSTransition;
typedef struct ABPDSRightSideofTransitionStruct* ABPDSRightSideofTransition;
typedef struct ABPDSAcceptingStateListStruct* ABPDSAcceptingStateList;
typedef struct ABPDSControlLocationStruct* ABPDSCtrlLocation;
typedef struct ABPDSStackListStruct* ABPDSStackList;
typedef struct ABPDSTransitionLabelStruct* ABPDSTransLabel;

typedef struct ABPDSHeadStruct* ABPDSHead;
typedef struct ABPDSRuleStruct* ABPDSRule;
typedef struct ABPDSHeadTableStruct* ABPDSHeadTable;
FILE *file;

typedef enum
{
	EQUAL,
	CONJ,
	TOP,
	EMPTY,
	VAREQU,
	VARNEQU,
} Relation;


struct ABPDSHeadStruct
{
	Name state;
	Name stack;
	unsigned long hash;
	ABPDSHead next;
	ABPDSRule rules;
};

struct ABPDSHeadTableStruct
{
	ABPDSHead *data;
	unsigned long size;
	unsigned long mask;
	unsigned long fill;
};

struct ABPDSRuleStruct
{
	ABPDSTransition rule;
	ABPDSRule next;
};


struct AlternatingBuChiPushdownSystemStruct
{
	Name         		startState;
	NameStack     	startStack;

	ABPDSCtrlLocation ctrlList;
	ABPDSStackList  stackList;
	
	ABPDSAcceptingStateList acceptingStateList;
	
	ABPDSHeadTable 		transList;
	ABPDSTransition trans;
};

struct ABPDSStackListStruct
{
	Name stack;
	ABPDSStackList next;
};

struct ABPDSControlLocationStruct
{
	Name         		state;
	ABPDSCtrlLocation next;
};

struct ABPDSTransitionStruct           // The transition rule is ([from_state,formula],from_stack)->righthandside;
{
	Name fromState;                        
	Name fromStack;

  Relation rel;
  Name var;
  DdNode* bdd;

  ABPDSRightSideofTransition rightSide;
  ABPDSTransLabel label;
  ABPDSTransition next;
};

struct ABPDSRightSideofTransitionStruct
{
	Name state;
	int index;
	union{
		Name rel;
		DdNode* Binding;
	}aux;
	Name stack1;
	Name stack2;
	ABPDSRightSideofTransition next;
};

struct ABPDSAcceptingStateListStruct
{
	Name state;
	DdNode* Binding;
	signed char bal;
  ABPDSAcceptingStateList right,left;
};

struct ABPDSTransitionLabelStruct
{
	Name state;
	DdNode* Binding;
	int index; 
	ABPDSTransLabel next;
};


/* --------------------------Definitions of Function   --------------------------------- */
    

/*  Function ABPDSInitial constructs an initial ABPDS.
    Output: An ABPDS*/
AlternatingBuchiPushdownSystem ABPDSInitial();



/* Function ABPDSDestory deletes the ABPDS.
   Input: An ABPDS. */
   	
void ABPDSDestory(AlternatingBuchiPushdownSystem*);




/* Function ABPDSDisplay prints the ABPDS.
   Input: An ABPDS. */
void	ABPDSDisplay(AlternatingBuchiPushdownSystem*);




/* Function ABPDSInsertTransition adds an transition into the ABPDS.
   Input: An transition list, 
   				The state of the left-hand side of the transition rule,
   	      FormulaTree of the left-hand side of the transition rule,
   	      The stack of the left-hand side of the transition rule,
   	      Name of the state of the right-hand side.
   	      Name of the topmost stack of the right-hand side.
   	      Name of the second stack of the right-hand side*/          
void	ABPDSInsertTransition(AlternatingBuchiPushdownSystem*, Name, FormulaTree, Name,Name,Name,Name);
						   
			
/* Function ABPDSPrintTransition prints a transition rule.
   Input: A transition of ABPDS. */
void	ABPDSPrintTransition(ABPDSTransition);


/*int CheckInclusion(ABPDSTransition, ABPDSTransition);

 Function ABPDSCreateTransition builds an transition rule.
   Input: FormulaTree of the left-hand side of the transition rule,
   	      The stack of the left-hand side of the transition rule,
   	      The right-hand side of ABPDS transition rule.*/  
ABPDSTransition	ABPDSCreateTransition(Name, Relation, Name, ABPDSRightSideofTransition);





/* Function ABPDSFindTransition loop up an transition rule whose 
	 left-hand side is <[Name,FormulaTree],Name>.
   Input: An transition list.
   				The state name of the left-hand side.
   				FormulaTree of left-hand side.
   				Stack Name of left-hand side.
   Output: the Transition pointer if found, otherwise NULL
ABPDSTransition	ABPDSFindTransition(ABPDSHeadTable, Name, Relation, Name);*/   




/* Function ABPDSDestoryTransition deletes an transition rule.
   Input: A transition rule of ABPDS*/     	   
void	ABPDSDestoryTransition(ABPDSTransition);


/* Function ABPDSDestoryTransList deletes the transition list of ABPDS.
   Input: A transition list of ABPDS*/  
void	ABPDSDestoryTransList(ABPDSTransition);

void HandleInitialConfiguration(AlternatingBuchiPushdownSystem*, FormulaTree);


/* Function ABPDSAppend2TransList inserts a transition rule into the transition list of ABPDS.
   Input: An transition list
   	      A transition rule*/ 
void	ABPDSAppend2TransList(ABPDSHeadTable, ABPDSTransition);
void	ABPDSAppend2TransListFWD(ABPDSHeadTable, ABPDSTransition);

int	ABPDSStrictAppend2TransList(ABPDSHeadTable, ABPDSTransition);

void RemoveHeadTable(ABPDSHeadTable,ABPDSTransition);
/* Function ABPDSInitRightSideofTransition creats a initial right-hand side of a transition.
   Output:  A APBDSRightSideofTransition.*/ 
ABPDSRightSideofTransition ABPDSInitRightSideofTransition(Name, int,Name,Name);

 
/* Function ABPDSDestoryRightSideofTransition deletes the right-hand side of a transition.
   Input:  A ABPDSRightSideofTransition.*/ 
void 	ABPDSDestoryRightSideofTransition(ABPDSRightSideofTransition);



/* Function ABPDSInitAcceptingStateList creats a initial accepting state list of the ABPDS.
   Output:  An ABPDSAcceptingStateList.*/ 
ABPDSAcceptingStateList ABPDSInitAcceptingStateList();
 
 
 
 
 
/* Function ABPDSDestoryAcceptingStateList deletes the accepting state list of the ABPDS.
   Input:  An ABPDSAcceptingStateList.*/ 
void 	ABPDSDestoryAcceptingStateList(ABPDSAcceptingStateList);




/* Function ABPDSCheckStateAcceptiness checks a state of ABPDS is accepting or not.
   Input:  A ABPDSAcceptingStateList.
   	 			 The name of a state.
   	 			 A FormulaTree a part of the state of ABPDS.
   Output: True if the state is accepint, otherwise false*/
DdNode* ABPDSCheckStateAcceptiness(ABPDSAcceptingStateList, Name);



/* Function ABPDSAppend2AcceptingStateList sets a state of ABPDS to be acceping.
   Input:  An accepting state list of the ABPDS.
   	 			 The name of a state.
   	 			 A FormulaTree a part of the state of ABPDS.*/ 
signed char 	ABPDSAppend2AcceptingStateList(ABPDSAcceptingStateList*, Name, DdNode*);




/* Function ABPDSFindRightSide checks whether there exists a transition rule
   from ([Name,FormulaTree],Name) or not. If exists, ruturn the right-hand side of 
   the transition rule, otherwise return 0.
   Input:  An ABPDSTransList.
   	 			 The name of a state.
   	 			 The name of the stack.
   	 			 A FormulaTree a part of the state of ABPDS.
   Output: 0 or a array of the address of  ABPDSRightSideofTransition.*/ 
ABPDSRightSideofTransition* ABPDSFindRightSide(ABPDSHeadTable, Name, Name);

void ABPDSFindOneRightSide(ABPDSHeadTable, Name,Name);


/* The function ABPDSPrepareForModelChecking generates the transition of ABPDS 
   use the information obtained by call ABPDSComputeAtomicSubformula.
   Input: An ABPDS.*/
void ABPDSPrepareForModelChecking(AlternatingBuchiPushdownSystem*);



/* The ABPDSComputeAtomicSubformula first task is Computing all the true, 
   atomic proposition or negation of atomic propositon
   subformula  of the Formula, which is used to create the transitions of ABPDS 
   when the subformula is one of these. Second is create the transition of ABPDS 
   when the token of suboformula is AND or OR, and recusivly invoke the function 
   ABPDSComputeAtomicSubformula. 
   Input: An ABPDS and a formuula.*/
void ABPDSComputeAtomicSubformula(AlternatingBuchiPushdownSystem* , FormulaTree );



/* Function ABPDSInsert2Stack adds a new stack into the Stack List.
   Input: A stack list and the name of new stack.*/
void ABPDSInsert2StackList(ABPDSStackList*, Name);


/* Function ABPDSDestoryStackList deletes the Stack List.
   Input: A stack list.*/
void ABPDSDestoryStackList(ABPDSStackList);


/* Function ABPDSInsert2StateList adds a new control location into the State List.
   Input: A state list and the name of new state.*/
void ABPDSInsert2StateList(ABPDSCtrlLocation*, Name);


/* Function ABPDSDestoryStateList deletes State List.
   Input: A state list.*/
void ABPDSDestoryStateList(ABPDSCtrlLocation);

ABPDSTransLabel CreateABPDSTransLabel(Name, DdNode*,int);

//void DeleteABPDSTransLabel(ABPDSTransLabel);


ABPDSHead ABPDS_head_create(Name, Name);

void ABPDS_head_delete (ABPDSHead);

int  ABPDS_head_compare (ABPDSHead, ABPDSHead);

void ABPDS_head_add_rule (ABPDSHead, ABPDSTransition);

ABPDSHeadTable ABPDS_head_table_empty ();

void ABPDS_head_table_delete (ABPDSHeadTable);

ABPDSHead ABPDS_head_table_insert(ABPDSHeadTable, ABPDSHead);

ABPDSHead ABPDS_head_find (ABPDSHeadTable, Name, Name);

unsigned long compute_hash_value(Name,Name);

ABPDSHead ABPDS_head_list_insert(ABPDSHead*, ABPDSHead, int*);

ABPDSRule create_rule(ABPDSTransition, ABPDSRule);

Name create_product_name(Name, FormulaTree, int);

void printAcceptingState(ABPDSAcceptingStateList);
#endif
