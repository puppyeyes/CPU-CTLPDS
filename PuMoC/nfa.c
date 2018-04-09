/* =========================================================================

   Implement the nondeterministic finita automata

   ====================================================================== */

#include <stdio.h>
#include "abpds.h"
#include "nfa.h"
#include "name.h"

int state_count =0;
extern AlternatingBuchiPushdownSystem	mc_abpds;

NFA* initNFA()
{
	NFA* tmp = malloc(sizeof(struct NondeterministicFiniteAutomata));
	tmp->initial_states =NULL;
	tmp->final_states=NULL;
	tmp->trans=NULL;
	tmp->lastTran = NULL;
	tmp->lastInitState = NULL;
	tmp->lastFinalState= NULL;
	return tmp;
}

void print_nfa(NFA* nfa)
{
	printf("\nThe initial states are: \n");
	StateList tmp1 = nfa->initial_states;
	for(; tmp1; tmp1=tmp1->next)
	{
		printf(" %d, ", tmp1->state);
	}
	printf("\nEnd of print the initial states.\n");
	
	
	printf("\nThe final states are: \n");
  tmp1 = nfa->final_states;
	for(; tmp1; tmp1=tmp1->next)
	{
		printf(" %d, ", tmp1->state);
	}
	printf("\nEnd print the final states.\n");	
	
	
	printf("\nPrint the transition rules.\n");
  NFATrans tmp2;
  for(tmp2=nfa->trans; tmp2; tmp2=tmp2->next)
  {
  	printf("%d -%s-> %d, ", tmp2->from_state, name_cstring(tmp2->input),tmp2->to_state);
  }	
  printf("\nEnd of print the transition rules.\n");
}

void print_cnfa(CNFA* cnfa)
{
	printf("The initial states are: ");
	StateList tmp1 = cnfa->initial_states;
	for(; tmp1; tmp1=tmp1->next)
	{
		printf(" %d, ", tmp1->state);
	}
	printf("\nEnd of print the initial states.\n");
	
	
	printf("\nThe final states are: \n");
  tmp1 = cnfa->final_states;
	for(; tmp1; tmp1=tmp1->next)
	{
		printf(" %d, ", tmp1->state);
	}
	printf("\nEnd print the final state.\n");	
	
	
	printf("\nPrint the transition rules.\n");
  CNFATrans tmp2;
  for(tmp2=cnfa->trans; tmp2; tmp2=tmp2->next)
  {
  	printf("%d --{",tmp2->from_state);
  	for(tmp1=tmp2->input; tmp1->next; tmp1=tmp1->next)
  	{
  		printf("%s,",name_cstring(tmp1->state) );
  	}
  	printf("%s}-->%d{",name_cstring(tmp1->state),tmp2->type);
  	
  	
  	StateList tmp3;
  	for(tmp3 = tmp2->rightSide; tmp3->next; tmp3=tmp3->next)
  	{
  		printf("%d,", tmp3->state);
  	}
  	printf("%d}, ", tmp3->state);
  }	
  printf("\nEnd of print the transition rule.\n");
}

NFA* productnfaforPlus(NFA* nfa1,NFA* nfa2)
{
	nfa1->lastInitState->next = nfa2->initial_states;	
	nfa1->lastInitState = nfa2->lastInitState;


	nfa1->lastFinalState->next = nfa2->final_states;	
	nfa1->lastFinalState = nfa2->lastFinalState;

	if(nfa1->trans) nfa1->lastTran->next = nfa2->trans;
	else nfa1->trans = nfa2->trans;
	
	nfa1->lastTran = nfa2->lastTran;
	deleteNFA(nfa2,1);
	return nfa1;
}

NFA* productnfaforCancat(NFA* nfa1,NFA* nfa2)
{
	//StateList tmp3;				
	NFATrans tmp =nfa2->trans;
	int flag=0;
	for(; tmp; tmp = tmp->next)
	{
		if(CheckMemberShip(nfa2->initial_states, tmp->from_state))
		{
			StateList tmp2 = nfa1->final_states;
			NFATrans new_trans =NULL;
			for(; tmp2->next; tmp2 = tmp2->next)
			{
				new_trans = malloc(sizeof(struct NFATransitionListStruct));
				new_trans->flag = 0;
				new_trans->from_state = tmp2->state;
				new_trans->input = tmp->input;
				new_trans->to_state = tmp->to_state;
				nfa1->lastTran->next = new_trans;
				nfa1->lastTran =new_trans;
			}
			StateList tmp_state=nfa2->final_states;
		//	StateList pre_state=NULL;
		//	StateList cache_state=NULL;
			while(tmp_state && !flag)
			{
				if(tmp_state->state == tmp->from_state)
				{
				/*	if(pre_state) 
					{
						pre_state->next = tmp_state->next;
						printf("\ndelete state:%d\n", tmp_state->state);
						free(tmp_state);
					}
					else
					{
						nfa2->final_states = tmp_state->next;
						free(tmp_state);
					} */
					flag=1;
					break;
				}
				else 
				{ 
					//pre_state = tmp_state;
					tmp_state =tmp_state->next;
				}
			}
			if(tmp->from_state==tmp->to_state)
			{
				new_trans = malloc(sizeof(struct NFATransitionListStruct));
				new_trans->from_state = tmp2->state;
				new_trans->input = tmp->input;
				new_trans->to_state = tmp->to_state;
				if(nfa1->lastTran)
				{
					nfa1->lastTran->next = new_trans;
					nfa1->lastTran =new_trans;				
				}
				else
				{
					nfa1->trans=nfa1->lastTran=new_trans;
				}		
			}
			else tmp->from_state = tmp2->state;	
		}
	}

	nfa1->lastTran->next = nfa2->trans;

	nfa1->lastTran = nfa2->lastTran;
	if(flag)
	{  	
		nfa1->lastFinalState->next = nfa2->final_states;
	}
	else
	{
		deleteStateList(nfa1->final_states);
		nfa1->final_states = nfa2->final_states;
	}
	nfa1->lastFinalState = nfa2->lastFinalState;
	deleteNFA(nfa2,2);

	return nfa1;
}
NFA* productnfaforStar(NFA* nfa1)
{
	StateList tmp =nfa1->initial_states;
	StateList tmp1 =NULL;
	NFATrans tmp_trans =NULL;
	NFATrans newTrans =NULL;
	NFATrans Guard = NULL;
	NFATrans LastGuard =NULL;
	
	for(tmp_trans =nfa1->trans; tmp_trans; tmp_trans =tmp_trans->next)
	{
		if(CheckMemberShip(tmp, tmp_trans->from_state))
		{
			for(tmp1 = nfa1->final_states; tmp1->next; tmp1 = tmp1->next)
			{
				newTrans =  malloc(sizeof(struct NFATransitionListStruct));
				newTrans->flag=0;
				newTrans->from_state = tmp1->state;
				newTrans->input = tmp_trans->input;
				newTrans->to_state = tmp_trans->to_state;
				newTrans->next = NULL;
				if(Guard)
				{
					LastGuard->next =newTrans;
					LastGuard = newTrans;
				}
				else
				{
					Guard = LastGuard = newTrans;
				}
			}
			tmp_trans->from_state = tmp1->state;	
		}
	}
	if(LastGuard)
	{	
		nfa1->lastTran->next = Guard;
		nfa1->lastTran = LastGuard;
	}
	deleteStateList(nfa1->initial_states);
	nfa1->initial_states = NULL;
	for(tmp =nfa1->final_states; tmp; tmp=tmp->next)
	{
		tmp1 =copyState(tmp);
		if(nfa1->initial_states)
		{
			nfa1->lastInitState->next =tmp1;
			nfa1->lastInitState =tmp1;
		}
		else
		{
			nfa1->initial_states = nfa1->lastInitState =tmp1;
		}
	}
	return nfa1;
}

NFA* productnfaforNUMBER(int input)
{	
	NFA* tmp =initNFA();
	NFATrans trans = malloc(sizeof(struct NFATransitionListStruct));
	trans->flag=0;
	state_count++;
	trans->from_state=state_count;
	
	StateList init_state = malloc(sizeof(struct NFAStateListStruct));
	init_state->state=state_count;
	init_state->next=NULL;	
	
	tmp->initial_states = tmp->lastInitState = init_state;
		
	trans->input = input;
	
	state_count++;
	trans->to_state = state_count;
	StateList final_state = malloc(sizeof(struct NFAStateListStruct));
	final_state->state = state_count;
	final_state->next = NULL;	
	
	tmp->final_states = tmp->lastFinalState = final_state;
	
	trans->next = NULL;
	tmp->trans =tmp->lastTran= trans;

	return tmp;
}
NFA* productnfaforDOT()
{
	NFA* tmp =initNFA();
	
	state_count++;
	StateList init_state = malloc(sizeof(struct NFAStateListStruct));
	init_state->state=state_count;
	init_state->next=NULL;	
	
	tmp->initial_states = tmp->lastInitState = init_state;	

	state_count++;
	StateList final_state = malloc(sizeof(struct NFAStateListStruct));
	final_state->state=state_count;
	final_state->next=NULL;	
	
	tmp->final_states = tmp->lastFinalState = final_state;	
	
	ABPDSStackList input;
	for (input=mc_abpds.stackList ;input->next; input=input->next)
	{
		NFATrans trans = malloc(sizeof(struct NFATransitionListStruct));
		trans->next =NULL;
		trans->flag=0;
		trans->from_state = init_state->state;
		trans->to_state = final_state->state;
		trans->input = input->stack;
		if(tmp->lastTran) 
		{
			tmp->lastTran->next = trans;
			tmp->lastTran = trans;
		}
		else
		{
			tmp->lastTran = tmp->trans=trans;
		}
	}
	return tmp;
}

void deleteNFA(NFA* nfa, int flag)
{
	switch(flag)
	{
		case 1: free(nfa); return;  //plus and star
		case 2: deleteStateList(nfa->initial_states); free(nfa); return;      //CAT
		default: 
			deleteStateList(nfa->initial_states); 
			deleteStateList(nfa->final_states);
			deleteTransList(nfa->trans);
			free(nfa);
	}
}


StateList copyState(StateList list)
{
	StateList tmp = malloc(sizeof(struct NFAStateListStruct));
	tmp->state=list->state;
	tmp->next=NULL;
	return tmp;
}

void deleteStateList(StateList list)
{
	StateList tmp =NULL;
	while(list)
	{
		tmp = list->next;
		free(list);
		list=tmp;
	}
}
void deleteTransList(NFATrans list)
{
	NFATrans tmp =NULL;
	while(list)
	{
		tmp = list->next;
		free(list);
		list=tmp;
	}
}

int CheckMemberShipWithVar(StateList list, int state)
{
	for(; list; list =list->next)
	{
		if(list->state == state && !IsVariable(list->state)) return 1;
	}
	return 0;
}


int CheckMemberShip(StateList list, int state)
{
	for(; list; list =list->next)
	{
		if(list->state == state) return 1;
	}
	return 0;
}
void Normalize(NFA* nfa)
{
	/* non */
}

CNFA* ComplementNFA(NFA* nfa)
{
	CNFA* cnfa = CNFAInit();
	cnfa->initial_states = nfa->initial_states;
	NFATrans tmp = nfa->trans;
	CNFATrans tmp4 =NULL;
	state_count++;
	StateList rightSide; 
		
	while(tmp)
	{		
		if(tmp->flag)
		{
			tmp =tmp->next;
			continue;
		}
		
		if(!CheckMemberShip(nfa->final_states, tmp->from_state))
			Append2StateList(&(cnfa->final_states), tmp->from_state);
		
		if(!CheckMemberShip(nfa->final_states, tmp->to_state))
			Append2StateList(&(cnfa->final_states), tmp->to_state);
		
		/*compute alternating transition from non-deterministic transitions*/
		rightSide = malloc(sizeof(struct NFAStateListStruct));
		rightSide->state = tmp->to_state;
		rightSide->next = NULL;		
		tmp->flag=1;
		
		NFATrans tmp2 =NULL;
		/*compute rightside of CNFA transition*/			
		for(tmp2=tmp->next; tmp2; tmp2=tmp2->next)
		{
			if(tmp2->from_state==tmp->from_state && tmp2->input==tmp->input) 
			{
				StateList tmp3 = malloc(sizeof(struct NFAStateListStruct));
				tmp3->state = tmp2->to_state;
				tmp3->next = rightSide;
				rightSide =tmp3;
				tmp2->flag=1;
				
				/*exchange the final states with non-final states*/
				if(!CheckMemberShip(nfa->final_states, tmp2->to_state))
					Append2StateList(&(cnfa->final_states), tmp2->to_state);				
			}
		}
		/*create CNFA transition*/
		InputList in=malloc(sizeof(struct NFAStateListStruct));
		in->state=tmp->input;
		in->next=NULL;
		if(IsVariable(tmp->input))
			tmp4 = CNFACreateTrans(tmp->from_state,in, 1, rightSide);		
		else
  		tmp4 = CNFACreateTrans(tmp->from_state,in, 0, rightSide);
  	
  	tmp4->next = cnfa->trans;
  	cnfa->trans =tmp4; 		
  	tmp =tmp->next;
	}
	
	StateList tmp_state = NULL;
	ABPDSStackList tmp_stack = NULL;


  /* add self cycle for q_f -r-> q_f for all r\in \Gamma*/
	for (tmp_stack=mc_abpds.stackList; tmp_stack->next; tmp_stack=tmp_stack->next)   // note that the last stack symbol is sharp.
	{
		StateList rightSide = malloc(sizeof(struct NFAStateListStruct));
		rightSide->state =state_count;
		rightSide->next= NULL;
			
		InputList in=malloc(sizeof(struct NFAStateListStruct));
		in->state=tmp_stack->stack;
		in->next=NULL;		
		CNFATrans trans = CNFACreateTrans(state_count, in,0, rightSide);
		trans->next = cnfa->trans;
		cnfa->trans = trans;
	}	
	
	/*add state_count to final states*/
	tmp_state = malloc(sizeof(struct NFAStateListStruct));    
	tmp_state->state = state_count;
	tmp_state->next = cnfa->final_states;
	cnfa->final_states =tmp_state;
	
	/*complementation for p-r->q \not\in\delta*/
	for(tmp_state = nfa->final_states; tmp_state; tmp_state=tmp_state->next)
	{
		if(checkAssumption(cnfa->trans,tmp_state->state)) continue;
		for(tmp_stack= mc_abpds.stackList; tmp_stack->next; tmp_stack = tmp_stack->next)
		{
			if(!CheckExistance(cnfa->trans,tmp_state->state, tmp_stack->stack))
			{
				StateList rightSide = malloc(sizeof(struct NFAStateListStruct));
				rightSide->state=state_count;
				rightSide->next=NULL;
				InputList in=malloc(sizeof(struct NFAStateListStruct));
				in->state=tmp_stack->stack;
				in->next=NULL;					
				CNFATrans trans = CNFACreateTrans(tmp_state->state, in, 0, rightSide);
				trans->next = cnfa->trans;
				cnfa->trans = trans;
			}
		}
	}
	for( tmp_state = cnfa->final_states->next; tmp_state; tmp_state=tmp_state->next)
	{
		if(checkAssumption(cnfa->trans, tmp_state->state)) continue;
		for(tmp_stack= mc_abpds.stackList; tmp_stack->next; tmp_stack = tmp_stack->next)
		{
			if(!CheckExistance(cnfa->trans,tmp_state->state, tmp_stack->stack))
			{
				StateList rightSide = malloc(sizeof(struct NFAStateListStruct));
				rightSide->state=state_count;
				rightSide->next=NULL;
				InputList in=malloc(sizeof(struct NFAStateListStruct));
				in->state=tmp_stack->stack;
				in->next=NULL;							
				CNFATrans trans = CNFACreateTrans(tmp_state->state, in,0, rightSide);
				trans->next = cnfa->trans;
				cnfa->trans = trans;
			}
		}
	}
	
	/* compute transition type 2 for parametric transition*/
	NFATrans tmp5,tmp6;
	for(tmp5=nfa->trans; tmp5; tmp5=tmp5->next)
	{
		tmp_state=NULL;
		rightSide=NULL;
		if(tmp5->flag!=1) continue;
		if(IsVariable(tmp5->input))
		{
			InputList inlist=malloc(sizeof(struct NFAStateListStruct));
			inlist->state=tmp5->input;
			inlist->next=NULL;
			tmp5->flag=2;
			
			InputList input=NULL;
			for(tmp6=nfa->trans; tmp6; tmp6=tmp6->next)
			{
				if(tmp6->from_state==tmp5->from_state && tmp6->input!=tmp5->input)
				{
					tmp6->flag=2;
					if(!CheckMemberShip(input,tmp6->input))
					{
						input = malloc(sizeof(struct NFAStateListStruct));
						input->state=tmp6->input;
						input->next=inlist;
						inlist=input;
					}
				}
			} 
			rightSide =malloc(sizeof(struct NFAStateListStruct));
  		rightSide->state = state_count;
 			rightSide->next =NULL;
 			
			tmp4 = CNFACreateTrans(tmp5->from_state, inlist, 2, rightSide);
  		tmp4->next = cnfa->trans;
  		cnfa->trans =tmp4;
  	}  			
	}
	return cnfa;
}

CNFA* CNFAInit()
{
	CNFA* tmp = malloc(sizeof(struct ComplementationNFAStruct));
	tmp->initial_states =NULL;
	tmp->final_states=NULL;
	tmp->trans=NULL;
	tmp->lastTran = NULL;
	tmp->lastInitState = NULL;
	tmp->lastFinalState= NULL;
	return tmp;
}
void CNFADelete(CNFA* cnfa)
{
	//deleteStateList(cnfa->initial_states);
	deleteStateList(cnfa->final_states);
	CNFADeleteTransList(cnfa->trans);
	free(cnfa);
}
CNFATrans CNFACreateTrans(int state, InputList input, int type, StateList rightSide)
{
	CNFATrans tmp = malloc(sizeof(struct ComplementationDFATransStruct));
	tmp->from_state = state;
	tmp->input = input;
	tmp->rightSide = rightSide;
	tmp->next = NULL;
	tmp->type = type;
	return tmp;
}
void  CNFADeleteTrans(CNFATrans trans)
{
	deleteStateList(trans->input);
	deleteStateList(trans->rightSide);
	free(trans);
}

void  CNFADeleteTransList(CNFATrans trans)
{
	CNFATrans tmp = NULL;
	while(trans)
	{
		tmp = trans->next;
		CNFADeleteTrans(trans);
		trans=tmp;
	}
}

void  Append2StateList(StateList* list, int state)
{
	StateList tmp=(*list);
	for(; tmp; tmp=tmp->next)
	{
		if(tmp->state==state) break;
	}
	if(!tmp) 
	{
		StateList tmp2 = malloc(sizeof(struct NFAStateListStruct));
		tmp2->state = state;
		tmp2->next=(*list);		
		(*list) =tmp2;
	}
}

int CheckExistance(CNFATrans transList,int state, int stack)
{
	for(; transList; transList = transList->next)
	{
		if (transList->from_state==state)
		{ 
			if(transList->input->state==stack) return 1;
		  
		}
	}
	return 0;
}


int checkAssumption(CNFATrans trans, int state)
{
	CNFATrans tmp;
	for(tmp=trans; tmp; tmp=tmp->next)
	{
		if(tmp->from_state==state && tmp->type)
		{
			return 1;
		}
	}
	return 0;
}