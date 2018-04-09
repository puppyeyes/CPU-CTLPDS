/* =========================================================================
   Extend Moped: afa.h 
   	
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
   
   This file defines the data structures and functions for Alternating
   Finite Automata.

   ====================================================================== */
#ifndef AFA_H
#define AFA_H

#include "name.h"
#include "abpds.h"
#include "bdd.h"
#include "ftree.h"

#define AFA_HASH_MIN 1024


/* --------------------------   Datatypes   --------------------------------- */
typedef struct AlternatingFiniteAutomataStruct AlternatingFiniteAutomata;
typedef struct AFATransitionStruct *AFATransition;
typedef struct AFATransitionStruct *AFATransList;
typedef struct AFARightSideofTransitionStruct  *AFARightSideofTransition;

typedef struct AFATransTableStruct*   AFATransTable;
typedef struct AFATransHeadStruct* AFATransHead;  

FILE *file; 

struct AlternatingFiniteAutomataStruct
{
	Name finalState;
	AFATransTable transList;
	AFATransition stack;
};

struct AFATransitionStruct
{
	Name fromState;
	DdNode* Binding;
	Name inputStack;
	AFARightSideofTransition rightSide;
  int index;
  int boolflag;
	AFATransition next;
	AFATransition fwdStack;
	AFATransition bwdStack;
};

struct AFARightSideofTransitionStruct
{
	Name state;
	DdNode* Binding;
	int index; 
	AFARightSideofTransition next;
};

struct AFATransTableStruct
{
	AFATransHead *data;
	long size;
	long mask;
	long fill;
};

struct AFATransHeadStruct
{
	Name state;
	Name stack;
	unsigned long hash;
	AFATransHead next;
	AFATransition rules;
};

/* --------------------------   Functions   --------------------------------- */

AlternatingFiniteAutomata AFAInit();

void          AFAPrint(AlternatingFiniteAutomata*);
void          AFADelete(AlternatingFiniteAutomata*);

//AFATransition AFAInitTrans(AlternatingFiniteAutomata*, ABPDSStackList);

AFATransition AFACreateTrans(Name, DdNode*, Name, int, AFARightSideofTransition);

void					AFAPrintTrans(AFATransition);
void         	AFADeleteTrans(AFATransition);
int          	AFACompareTrans(AFATransition, AFATransition);
void          AFADeleteTransList(AFATransList);
int				    AFAAppend2TransList(AFATransTable, AFATransition);

int          	AFACompareTransList(AFATransList, AFATransList);
int           AFACheckTransInclusion (AFATransList, AFATransition);

AFARightSideofTransition AFAInitRightSideofTrans(Name, int, DdNode* , AFARightSideofTransition);

void 					AFADeleteRightSide(AFARightSideofTransition);
int 					AFAAppend2RightSide(AFARightSideofTransition*, Name, DdNode*, int);
int					  AFACompareRightSide(AFARightSideofTransition, AFARightSideofTransition);
int						AFACheckRightSideInclusion(AFARightSideofTransition, Name, DdNode*, int);

int           AFACheckMembership(AlternatingFiniteAutomata*, Name, FormulaTree, int, DdNode*, NameStack);

DdNode*       Rel2BDD(ABPDSTransition,AFATransition);

AFATransTable AFA_head_table_empty();

AFATransHead afatrans_head_create(Name, Name);

void         afatrans_head_delete(AFATransHead);
int          afatrans_head_compare (AFATransHead, AFATransHead);
void         afa_head_add_rule(AFATransHead, AFATransition);

//void          afatrans_head_list_delete (AFATransHead*);
AFATransHead  afatrans_head_list_insert (AFATransHead*, AFATransHead, int*);

void          afatrans_head_table_delete (AFATransTable);
AFATransHead  afatrans_head_table_insert (AFATransTable, AFATransHead);
AFATransHead  afatrans_head_table_find (AFATransTable, Name,Name);

int AFA_head_table_compare(AFATransTable, AFATransTable);





#endif
