/* =========================================================================
   Extend Moped: afa.c
   	
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
   
   This file implemtent the functions of afa.h

   ====================================================================== */


#include "afa.h"
#include "common.h"
#include <assert.h>

extern void CTLPrintFormula(FormulaTree);
extern int mc_verbose;
extern int isdebug;
extern AlternatingFiniteAutomata afa;
int isomorphicChecking = 0;
int preStarChecking =0;
extern AFATransList* waiting_trans ;  
extern int waiting_num;
extern int waiting_num_Max;
extern const int WATIING_NUM_STEP;
extern AFATransList base;
extern ParamList ctlQuantVarList;
extern int mc_CTPL;
extern int mc_CTPL;

int mc_geq=0;
/* --------------------------   Functions   --------------------------------- */

AlternatingFiniteAutomata AFAInit()
{
	AlternatingFiniteAutomata afa;
	char* f = "AFAFinalState";
	afa.finalState = name_create(f);
	afa.stack= NULL;
	afa.transList= AFA_head_table_empty();	
	return afa;
}

AFATransTable AFA_head_table_empty()
{
	AFATransTable new_table=common_malloc(sizeof(struct AFATransTableStruct));
	int i;

	new_table->data = common_malloc(AFA_HASH_MIN * sizeof(AFATransHead*));
	new_table->size = AFA_HASH_MIN;
	new_table->mask = AFA_HASH_MIN - 1;
	new_table->fill = 0;

	for (i = 0; i < new_table->size; i++) new_table->data[i] = NULL;
	return new_table;
}


AFATransHead afatrans_head_create(Name state, Name stack)
{
	AFATransHead newhead = common_malloc(sizeof(struct AFATransHeadStruct));
	newhead->state  = state;
	newhead->stack   = stack;

	newhead->rules  = NULL;	/* contains list of neighbouring heads */
	newhead->next   = NULL;	/* for use inside hash table */

	newhead->hash  = compute_hash_value(state,stack);

	return newhead;
}

void  afatrans_head_delete(AFATransHead head)
{
	AFATransition tmp;
	while(head->rules)
	{
		tmp=head->rules->next;
		AFADeleteTrans(head->rules);
		head->rules =tmp;
	}
	common_free(head,sizeof(struct AFATransHeadStruct));
}

int   afatrans_head_compare (AFATransHead h1, AFATransHead h2)
{
	int cmp;
	if ((cmp = name_compare(h1->state,h2->state))) return cmp;  // list is an descend chain
	return name_compare(h1->stack,h2->stack);
}

void  afa_head_add_rule(AFATransHead head, AFATransition trans)
{
	trans->next=head->rules;
  head->rules =trans;
}

AFATransHead  afatrans_head_list_insert (AFATransHead* list, AFATransHead newhead, int *newentry)
{
	AFATransHead last_entry = NULL;
	AFATransHead next_entry = *list;
	int cmp = 1;

	while (next_entry)
	{
		cmp = afatrans_head_compare(next_entry,newhead);
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
		afatrans_head_delete(newhead);
		return next_entry;
	}
}

void  afatrans_head_table_delete (AFATransTable table)
{
	long i;

	for (i = 0; i < table->size; i++)
	{
		AFATransHead list = table->data[i];
		while (list)
		{
			AFATransHead nexthead = list->next;
			afatrans_head_delete(list);
			list = nexthead;
		}
	}
	common_free(table->data,table->size * sizeof(AFATransHead*));
	common_free(table,sizeof(struct AFATransTableStruct));
}


AFATransHead  afatrans_head_table_insert (AFATransTable table, AFATransHead head)
{
	int hash = head->hash & table->mask;
	int newtrans, i;

	head = afatrans_head_list_insert(&(table->data[hash]), head, &newtrans);

	if (!newtrans) return head;

	//head->list = table->list;
	//table->list = head;

	if (table->fill++ < table->size * 3/4) return head;

	table->data = common_realloc(table->data, table->size*2 * sizeof(AFATransHead*),
				     table->size * sizeof(AFATransHead*));

	table->mask = table->size * 2 - 1;

	for (i = 0; i < table->size; i++)
	{
		AFATransHead l1 = NULL, l2 = NULL;
		AFATransHead last_entry, next_entry;

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

AFATransHead  afatrans_head_table_find (AFATransTable table, Name state, Name stack)
{
	AFATransHead head = afatrans_head_create(state,stack);
	AFATransHead list = table->data[head->hash & table->mask];

	while (list)
	{
		int cmp = afatrans_head_compare(list,head);              // list is an descend chain
		if (cmp > 0) list = NULL;
		if (cmp >= 0) break;
		list = list->next;
	}

	afatrans_head_delete(head);
	return list;
}


/*AFATransition AFAInitTrans(AlternatingFiniteAutomata* afa, ABPDSStackList stacks)
{
	assert(afa); 
	AFARightSideofTransition rightSide = AFAInitRightSideofTrans(afa->finalState,0,NULL,NULL);

	AFATransition trans = NULL;
	
	if (isdebug) printf("****** Begin printing the initial transtion rule of AFA.\n");
	for (;stacks; stacks = stacks->next)
	{
		trans = AFACreateTrans(afa->finalState, NULL, stacks->stack, 0, rightSide);
    AFATransHead head =afatrans_head_create(trans->fromState,trans->inputStack);
    head =afatrans_head_table_insert(afa->transList, head);
    trans->next = head->rules;
    head->rules=trans;
    trans->fwdStack =afa->stack;
    afa->stack=trans;
    trans->bwdStack=NULL;
	}
	if (isdebug) printf("****** End of print the initial transtion rul of AFA.\n");
	return transList;
}*/

void AFADelete(AlternatingFiniteAutomata* afa)
{
	afatrans_head_table_delete(afa->transList);
	afa->transList = NULL;	
	afa->stack=NULL;
}


void AFAPrint(AlternatingFiniteAutomata* afa)
{
	if (!afa) return;
	printf("***************The transitions of AFA are: *******************\n");
	AFATransition tmp = NULL;
	int i,count=0;
  for (i = 0; i < afa->transList->size; i++) 
  {
  	AFATransHead list = afa->transList->data[i];
  	while(list)
  	{
  		for(tmp=list->rules; tmp; tmp=tmp->next)
  		{
  			AFAPrintTrans(tmp);
  			count++;
  		}
  		list=list->next;
  	}
  }	
	printf("\n The final state of AFA is: %s\n The number of AFA transitions: %d\n", name_cstring(afa->finalState), count);
}



AFATransition AFACreateTrans(Name state, DdNode* bdd, Name stack, int index, AFARightSideofTransition rightSide)
{
		AFATransition newTrans = common_malloc(sizeof(struct AFATransitionStruct));
		newTrans->fromState = state;
    newTrans->rightSide = rightSide;
    newTrans->next = NULL;
    newTrans->inputStack = stack;
    newTrans->index = index;
    newTrans->boolflag = 0;
    newTrans->Binding = bdd;
		newTrans->fwdStack=NULL;
		newTrans->bwdStack=NULL;
    return newTrans;    
}

void  AFADeleteTrans(AFATransition trans)
{
  AFADeleteRightSide(trans->rightSide);
  if(trans->Binding) Cudd_RecursiveDeref(CTLBDDMgr,trans->Binding);
  common_free(trans, sizeof(struct AFATransitionStruct) );
}

int  AFACompareTrans(AFATransition trans1, AFATransition trans2)
{
	if ( trans1 == trans2 ) return 1;
	if ((!trans1 && trans2)||(!trans2 && trans1)) return 0;
	if (trans1->fromState == trans2->fromState && trans1->inputStack == trans2->inputStack  \
		 && trans1->index == trans2->index && trans1->Binding==trans2->Binding )
	{
		if ( AFACompareRightSide(trans1->rightSide ,trans2->rightSide)) return 1; 
	}
	return 0;
}

int  AFACompareTransList(AFATransList transList1, AFATransList transList2)
{
	if ( transList1 == transList2 ) return 1;
	if ((transList1 == NULL && transList2)||(transList2 ==NULL && transList1)) return 0;	

	isomorphicChecking = 1;
	AFATransition tmp = transList1;
	while(tmp)
	{
		if (tmp->fromState == afa.finalState) break;
		if (!AFACheckTransInclusion(transList2, tmp)) 
		{
			isomorphicChecking = 0;
			return 0;
		} 
		tmp = tmp->next;
	}
	
	tmp = transList2;
	while(tmp)
	{
		if (tmp->fromState == afa.finalState) break;
		if (!AFACheckTransInclusion(transList1, tmp))
		{
			isomorphicChecking = 0; 
			return 0;
		} 
		tmp = tmp->next;
	}	
	isomorphicChecking = 0;
	return 1;
}	
	
int AFA_head_table_compare(AFATransTable table1, AFATransTable table2)
{
	if ( !table1->size && !table2->size ) return 1;
	if ((!table1->size && table2->size)||(!table2->size && table1->size)) return 0;
	int i;
	for(i=0;i<table1->size; i++)
	{
		AFATransHead list1 = table1->data[i];
		AFATransHead list2 = table2->data[i];
		while(list1 && list2)
		{
			if(!AFACompareTransList(list1->rules,list2->rules)) return 0;
			list1=list1->next;
			list2=list2->next;
		}
		if((list1 && !list2) || (list2 && !list1)) return 0;
	}
	return 1;	
}	
	
int  AFACheckTransInclusion (AFATransList transList, AFATransition trans)
{
	assert(trans!=NULL);
	while(transList)
	{
		if(trans==transList) {transList=transList->next; continue;  }
		if (trans->fromState ==  transList->fromState && trans->inputStack == transList->inputStack &&\
			  trans->Binding==transList->Binding)
		{
			if (AFACompareRightSide(trans->rightSide, transList->rightSide)) return 1; 
		}
		transList = transList->next;
	}
	return 0;
}


int	AFAAppend2TransList(AFATransTable table, AFATransition trans)    
{ 
	if (mc_verbose > 2) AFAPrintTrans(trans); 
	AFATransHead head=afatrans_head_create(trans->fromState,trans->inputStack);
	//AFAPrintTrans(trans); 
	head=afatrans_head_table_insert(table,head);
	AFATransition transList=head->rules;
  AFATransition preTrans=NULL;
  AFATransition tmpTrans=NULL;	

	while(transList)
	{		
		if(trans->Binding==transList->Binding)
		{
			if(transList->rightSide->state==afa.finalState)	
			{ 	AFADeleteTrans(trans);  return 0;  }
			
			if (trans->rightSide->state == afa.finalState)
			{
				AFARightSideofTransition tmp =AFAInitRightSideofTrans(afa.finalState, 0,NULL,NULL);
				AFADeleteRightSide(transList->rightSide);
				transList->rightSide =tmp;

				tmpTrans = preTrans =transList;
				transList = transList->next;		
				while(transList)
				{
					if (trans->Binding==transList->Binding)  	
			  	{
			  		preTrans->next = transList->next;
						if(transList->fwdStack) transList->fwdStack->bwdStack=transList->bwdStack;
						if(transList->bwdStack) transList->bwdStack->fwdStack=transList->fwdStack;
  		      if(afa.stack==transList) afa.stack=transList->fwdStack;
  		      	 
						if(transList==base) base = tmpTrans;
					
			  		AFADeleteTrans(transList);
			  		transList = preTrans->next;
			  	}
			  	else
			  	{
			  		preTrans =transList;
			  		transList = transList->next;
			  	}					
				}
				if(!tmpTrans->fwdStack && !tmpTrans->bwdStack)
				{
					tmpTrans->fwdStack=afa.stack;
					if(afa.stack) afa.stack->bwdStack=tmpTrans;
					afa.stack=tmpTrans;
				}	  			 				
				AFADeleteTrans(trans);
				return 0;				   				
			}
		}	
		if (AFACompareRightSide(trans->rightSide, transList->rightSide))
		{
			if(trans->Binding==transList->Binding	)
         { AFADeleteTrans(trans);  return 0;  }
			else
			{
				if(transList->Binding) 
				{	
					DdNode* tmp;
					Cudd_Ref(tmp=Cudd_bddOr(CTLBDDMgr,trans->Binding,transList->Binding));
					Cudd_RecursiveDeref(CTLBDDMgr,transList->Binding);
					transList->Binding=tmp;
				}
				if(!transList->fwdStack && !transList->bwdStack)
				{
					transList->fwdStack=afa.stack;
					if(afa.stack) afa.stack->bwdStack=transList;
					afa.stack=transList;
				}														  			 
				AFADeleteTrans(trans); 
				return 0;	  					
			}
		}
		if(trans->Binding==transList->Binding)
		{
			AFARightSideofTransition tmp_rs;
		//	mc_geq=1;
			for(tmp_rs =transList->rightSide; tmp_rs; tmp_rs=tmp_rs->next)
			{
				if(!AFACheckRightSideInclusion(trans->rightSide, tmp_rs->state, tmp_rs->Binding, tmp_rs->index))
					break;
			}
		//	mc_geq=0;
			if(tmp_rs)
			{
			//	mc_geq=1;
				for(tmp_rs =trans->rightSide; tmp_rs;tmp_rs=tmp_rs->next)
				{
					if(!AFACheckRightSideInclusion(transList->rightSide, tmp_rs->state, tmp_rs->Binding, tmp_rs->index))
						break;
				}
				//mc_geq=0;
				if(!tmp_rs)
				{
					if(preTrans)
					{
						preTrans->next = transList->next;		
						
						if(transList->fwdStack) transList->fwdStack->bwdStack=transList->bwdStack;
						if(transList->bwdStack) transList->bwdStack->fwdStack=transList->fwdStack;
  		      if(afa.stack==transList) afa.stack=transList->fwdStack;
  		      	
						if(transList==base) base = tmpTrans;
						AFADeleteTrans(transList); 
						transList = preTrans->next;
												
			  		if(!transList) return 0;
					}
					else
					{
						preTrans = transList;
						transList = transList->next;
					}
				}
				else 
				{ 
					if( trans->rightSide->next==transList->rightSide->next&& \
						 trans->rightSide->state==transList->rightSide->state && \
						 trans->rightSide->Binding==transList->rightSide->Binding )
					{
						if(trans->rightSide->index >= transList->rightSide->index)	AFADeleteTrans(trans); 							
						else
						{ 
							transList->rightSide->index=trans->rightSide->index;
							AFADeleteTrans(trans);
							if(!transList->fwdStack &&!transList->bwdStack)
      				{
      					transList->fwdStack=afa.stack;
      					if(afa.stack) afa.stack->bwdStack=transList;
      					afa.stack=transList;
      				}	
						}
						return 0;
					}
					else
					{
						preTrans = transList;
						transList = transList->next;
					}
				}
			}
			else 
			{				
				AFADeleteTrans(trans);  
				return 0; 										
    	} 	
    }
    else
    {
			preTrans = transList;
			transList = transList->next;        	
    } 		
	}	
	
	trans->fwdStack=afa.stack;
	trans->bwdStack=NULL;
	if(afa.stack) afa.stack->bwdStack=trans;
	afa.stack=trans;
  trans->next=head->rules;
  head->rules=trans;	
 // printf("\nadd to stack\n");
  //AFAPrintTrans(afa.stack);
  return 1;
}

void	AFAPrintTrans(AFATransition trans)
{
//	FILE* str=fopen("out.txt", "a");
	AFARightSideofTransition rightSide = trans->rightSide;
	printf("[%s]%d - %s -> { ", name_cstring(trans->fromState), trans->index,name_cstring(trans->inputStack));

	while(rightSide)
	{
		printf("[%s]%d ", name_cstring(rightSide->state),rightSide->index);	
		
		rightSide = rightSide->next;
	}
	printf("}\n");
	//fprintf(str, "     %d\n", trans->boolflag);
//	fclose(str);
}

AFARightSideofTransition AFAInitRightSideofTrans(Name state, int index, DdNode* bdd, AFARightSideofTransition next)
{
	AFARightSideofTransition rightSide = common_malloc(sizeof(struct AFARightSideofTransitionStruct));
	rightSide->state =state;
	rightSide->next =next;
	rightSide->index = index;	
	rightSide->Binding=bdd;
	return rightSide;
}	


void 	AFADeleteRightSide(AFARightSideofTransition rs)
{
	AFARightSideofTransition tmp;
	int size =  sizeof(struct AFARightSideofTransitionStruct);
	while(rs)
	{
		tmp = rs->next;
		if(rs->Binding&&mc_CTPL) Cudd_RecursiveDeref(CTLBDDMgr,rs->Binding);
		common_free(rs,size);
		rs = tmp;
	}
}

int	AFACompareRightSide(AFARightSideofTransition rightSide1, AFARightSideofTransition rightSide2)
{
	AFARightSideofTransition tmp=rightSide1;
	while(tmp)
	{		
		if (!AFACheckRightSideInclusion(rightSide2, tmp->state, tmp->Binding, tmp->index)) return 0; 
		tmp = tmp->next;
	}
	
	tmp = rightSide2;
	while(tmp)
	{
		if (!AFACheckRightSideInclusion(rightSide1, tmp->state, tmp->Binding, tmp->index)) return 0; 
		tmp = tmp->next;
	}	
	return 1;
}

int AFAAppend2RightSide(AFARightSideofTransition* rightSide, Name state, DdNode* bdd, int index)
{
	if (AFACheckRightSideInclusion(*rightSide,state, bdd, index)) return 0;
	AFARightSideofTransition tmp = AFAInitRightSideofTrans(state, index, bdd, *rightSide);
  (*rightSide) = tmp;
  return 1;
}

int	AFACheckRightSideInclusion(AFARightSideofTransition rightSide, Name state, DdNode* bdd, int index)
{
	for ( ; rightSide; rightSide= rightSide->next)
	{
		if(isomorphicChecking)
		{
			if (rightSide->state == state && rightSide->Binding==bdd) return 1;
		}
		else
		{
			if(mc_geq)
			{
        if (rightSide->state == state && rightSide->index >= index && rightSide->Binding==bdd) return 1;
			}
			else if (rightSide->state == state && rightSide->index == index && rightSide->Binding==bdd)
			{
				return 1;
			}
		}
	}
  return 0;
}


int AFACheckMembership(AlternatingFiniteAutomata* afa, Name state, FormulaTree ftree, int faState, DdNode* bdd, NameStack stacks)
{
	Name name= ftree? create_product_name(state,ftree,faState) : state;
//	printf("\n%s\n", name_cstring(name));
	NameStack tmp1 = NULL;
	NameStack tmp2= NULL;
	for(tmp1 = stacks; tmp1; tmp1 = tmp1->next)
	{
		name_stack_push(&tmp2, tmp1->name, NULL);
	}
	AFARightSideofTransition tmp3 = AFAInitRightSideofTrans(afa->finalState,0,NULL,NULL);
	
	AFARightSideofTransition tmp6 = NULL;
	AFATransition tmp4 = NULL;
	AFARightSideofTransition tmp5 = NULL;
	int flag;
  isomorphicChecking =1;
	for(; tmp2; tmp2 = tmp2->next)
	{
		tmp6 = NULL;
		flag = 1;
		while(flag)
		{	
			flag =0;
			for(tmp4 = afa->stack; tmp4; tmp4 = tmp4->fwdStack)
			{
				if(tmp4->inputStack==tmp2->name)
				{
					for(tmp5 = tmp4->rightSide; tmp5; tmp5 = tmp5->next)
					{
						if(!AFACheckRightSideInclusion(tmp3, tmp5->state,tmp5->Binding, tmp5->index)) break;
					}
					if(!tmp5)
					{
						if(AFAAppend2RightSide(&tmp6, tmp4->fromState, tmp4->Binding, tmp4->index))
							flag = flag+1;	
					}
				}
			}
		}
		AFADeleteRightSide(tmp3);
		tmp3 = tmp6;
	}
	isomorphicChecking =0;
	if (!tmp6) return 0;
	int index;
	if (tmp6->state == afa->finalState && !(tmp6->next)) return 0;
	else index = (tmp6->state == afa->finalState)? tmp6->next->index: tmp6->index;
  
  index = AFACheckRightSideInclusion(tmp6, name, bdd, index);
  AFADeleteRightSide(tmp6);
	return index;
}



DdNode*  Rel2BDD(ABPDSTransition ABPDS_trans, AFATransition AFA_trans)
{
	if(!AFA_trans->Binding) return NULL;
	DdNode* tmp=NULL;

	switch(ABPDS_trans->rel)
	{
		case EQUAL:
			Cudd_Ref(tmp= ABPDS_trans->bdd? ABPDS_trans->bdd : AFA_trans->Binding);
			break;
		case CONJ:
			if(ABPDS_trans->bdd)
				Cudd_Ref(tmp=Cudd_bddAnd(CTLBDDMgr,AFA_trans->Binding,ABPDS_trans->bdd));	
			else
				Cudd_Ref(tmp=AFA_trans->Binding);
			break;			
		case TOP:
		{
			DdNode* bdd=NULL;
			bdd = build_var_bdd(ABPDS_trans->var, ABPDS_trans->rightSide->aux.rel);
			/*printf("Vari Name: %s, value: %s\n",name_cstring(((ParamList) ABPDS_trans->formula->right)->name),name_cstring(ABPDS_trans->rightSide->aux.rel));
			printf("The var bdd:");
			Cudd_PrintMinterm(CTLBDDMgr,bdd);
			printf("AFA_trans->Binding");
			Cudd_PrintMinterm(CTLBDDMgr,AFA_trans->Binding);*/
			Cudd_Ref(tmp=Cudd_bddAnd(CTLBDDMgr,AFA_trans->Binding,bdd));	
			/*printf("var bdd && AFA_trans->Binding:");
			Cudd_PrintMinterm(CTLBDDMgr,tmp);*/
			Cudd_RecursiveDeref(CTLBDDMgr,bdd);
			if(tmp==CTLEmptyset) { Cudd_RecursiveDeref(CTLBDDMgr,tmp); return NULL; } 
			
			DdNode* tmp1 =NULL;
			DdNode* cube = NULL;
			int* cubeArray= malloc(step_length*sizeof(int)); 
			int index =findVarIndex(ctlQuantVarList,ABPDS_trans->var);
			int i;
			for(i=0;i<step_length;i++)
			{
				cubeArray[i]=index*step_length+i;
			}
			Cudd_Ref(cube = Cudd_IndicesToCube(CTLBDDMgr,cubeArray,step_length));
			/*printf("var Cube");
			Cudd_PrintMinterm(CTLBDDMgr,cube);*/
			free(cubeArray);
			Cudd_Ref(tmp1=Cudd_bddExistAbstract(CTLBDDMgr,tmp,cube));
			Cudd_RecursiveDeref(CTLBDDMgr,tmp);			
			Cudd_RecursiveDeref(CTLBDDMgr,cube);
			/*printf("After Exist Abstract");
			Cudd_PrintMinterm(CTLBDDMgr,tmp1);*/
			
			if(ABPDS_trans->bdd)
			{
				//printf("ABPDS_trans->bdd");
				//Cudd_PrintMinterm(CTLBDDMgr,ABPDS_trans->bdd);
				Cudd_Ref(tmp = Cudd_bddAnd(CTLBDDMgr,ABPDS_trans->bdd,tmp1));
				Cudd_RecursiveDeref(CTLBDDMgr,tmp1);
			}
			else tmp =tmp1;
			break;		
		}	
		case VAREQU:
		{
			DdNode* bdd=NULL;
			bdd = build_var_bdd( ABPDS_trans->rightSide->aux.rel, ABPDS_trans->fromStack);		
			Cudd_Ref(tmp=Cudd_bddAnd(CTLBDDMgr,AFA_trans->Binding,bdd));		
			Cudd_RecursiveDeref(CTLBDDMgr,bdd);
			if(tmp==CTLEmptyset) { Cudd_RecursiveDeref(CTLBDDMgr,tmp); return NULL; } 
			if(ABPDS_trans->bdd)
			{
				Cudd_Ref(bdd = Cudd_bddAnd(CTLBDDMgr,ABPDS_trans->bdd,tmp));
				Cudd_RecursiveDeref(CTLBDDMgr,tmp);
				tmp=bdd;
			}
			break;				
		}
	  case VARNEQU:
	  {
			DdNode* bdd=NULL;
			bdd = build_var_bdd( ABPDS_trans->rightSide->aux.rel, ABPDS_trans->fromStack);		
			bdd=Cudd_Not(bdd);
			Cudd_Ref(tmp=Cudd_bddAnd(CTLBDDMgr,AFA_trans->Binding,bdd));		
			Cudd_RecursiveDeref(CTLBDDMgr,bdd);
			if(tmp==CTLEmptyset) { Cudd_RecursiveDeref(CTLBDDMgr,tmp); return NULL; } 
			if(ABPDS_trans->bdd)
			{
				Cudd_Ref(bdd = Cudd_bddAnd(CTLBDDMgr,ABPDS_trans->bdd,tmp));
				Cudd_RecursiveDeref(CTLBDDMgr,tmp);
				tmp=bdd;
			}
			break;	  	
	  }
		case EMPTY:
			Cudd_Ref(tmp=ABPDS_trans->bdd);
			break;			
		default:
			break;		
	}
	if (tmp==CTLEmptyset)
	{
		Cudd_RecursiveDeref(CTLBDDMgr,tmp);
		return NULL;
	}
	return tmp; 
}
