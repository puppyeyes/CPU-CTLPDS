/* =========================================================================

   ABPDS Reachability algorithm
   
   ====================================================================== */

#include <stdio.h>
#include "common.h"
#include "variables.h"
#include "abpds.h"
#include "afa.h"

extern int isdebug;

extern AlternatingFiniteAutomata afa;
extern void CTLPrintFormula(FormulaTree);
extern int isomorphicChecking;
extern int mc_CTPL;
extern int mc_pds_line_num;
AFATransList base;
extern int preStarChecking;
extern int   mc_check_emptiness;

AFATransTable preStarTable;
int afa_trans_num_max=0;

/* For reachability, compute all configurations backwards reachable
 * from the target. */

void compute_abpds_prestar(AlternatingBuchiPushdownSystem* abpds, AFATransTable baseTable, int level)
{
	base = NULL;

	//AFATransList prestar = *prestar_trans_list;    // Store the transitions of prestar, the added transition is appended at the end of abpds_mid_trans..

	ABPDSAcceptingStateList accepting = abpds->acceptingStateList;    //acceptint states of ABPDS
	ABPDSTransition abpds_trans;                  
	ABPDSTransition abpds_mid_trans = NULL;          // store the middle transitions when compute the prestar of <p,r>->{<p1,r1>,...,<pn,rn>}
	                                             // and <p,r>-><p1,r1r2>, the added transition is appended at the end of abpds_mid_trans.

	DdNode* bdd=NULL;
	ABPDSHeadTable abpds_mid_table=ABPDS_head_table_empty();
	preStarTable=AFA_head_table_empty();	
	AFATransHead AFAhead;
	ABPDSHead head;
	ABPDSRule rule;

  ABPDSTransition abpds_mid_trans_last=NULL;
	
	preStarChecking =1;   // set flag used to fix the state space exploding.
	int i;	
	if (isdebug)
	{
		printf("********************Begin printing the base transitions.***********\n\n");
		for(i=0 ;i<baseTable->size; i++)
		{
			AFATransHead list =baseTable->data[i];
			while(list)
			{
				for(base=list->rules; base; base=base->next)
				{
					AFAPrintTrans(base);
				}
				list=list->next;
			} 
		}
		printf("********************End of print the base transitions.***********\n\n");
	
	}
  int num_transition=0;
  if(!mc_CTPL)
  {
    for (abpds_trans = abpds->trans; abpds_trans; abpds_trans = abpds_trans->next)
    {
    	if (level == 1) num_transition++;
    	// deal with <p,r>-><p > 
    	if (abpds_trans->rightSide->stack1==0 && abpds_trans->rightSide->next==NULL)
    	{ 
    		AFARightSideofTransition rightSide = AFAInitRightSideofTrans(abpds_trans->rightSide->state, level, NULL,NULL);
  
    		AFATransition added_trans = AFACreateTrans(abpds_trans->fromState, NULL, abpds_trans->fromStack, level, rightSide);
  			AFAAppend2TransList(preStarTable, added_trans);	
    		
    		if (ABPDSCheckStateAcceptiness(accepting, abpds_trans->rightSide->state))
    		{
    			if (level == 1)	rightSide= AFAInitRightSideofTrans(afa.finalState,0,NULL,NULL);
    			else rightSide= AFAInitRightSideofTrans(abpds_trans->rightSide->state,level-1,NULL,NULL);
    			
      		AFATransition added_trans = AFACreateTrans(abpds_trans->fromState,  NULL, abpds_trans->fromStack, level, rightSide);
    			AFAAppend2TransList(preStarTable, added_trans);	     			
    		}
    	}
    }  	
	}	
  if (level == 1)
  {  
  	DdNode* accept =NULL;
  	if(isdebug) printf("******************** I am in %d level.***********\n\n", 1);	

  	for (abpds_trans = abpds->trans; abpds_trans; abpds_trans = abpds_trans->next)
  	{
  		if (abpds_trans->rightSide->next == NULL )  // deal with <p,r>-><p1,w1>
  		{
  			if ((accept = ABPDSCheckStateAcceptiness(accepting, abpds_trans->rightSide->state)) && ( mc_check_emptiness||ABPDSCheckStateAcceptiness(accepting, abpds_trans->fromState)))
  			{
  				bdd=NULL;
  				if(mc_CTPL)
  				{
    				if(abpds_trans->rel==VAREQU ||abpds_trans->rel== VARNEQU)  
    				{
              DdNode* tmp_bdd;
        			tmp_bdd = build_var_bdd( abpds_trans->rightSide->aux.rel, abpds_trans->fromStack);	
        			if(abpds_trans->rel== VARNEQU) tmp_bdd=Cudd_Not(tmp_bdd);
        			Cudd_Ref(bdd=Cudd_bddAnd(CTLBDDMgr, accept, tmp_bdd));		
        			Cudd_RecursiveDeref(CTLBDDMgr,tmp_bdd);   					
    				}
  
  					if(abpds_trans->rel==EQUAL ||abpds_trans->rel==CONJ) Cudd_Ref(bdd=accept);
  					if(abpds_trans->rel==TOP)
  					{ 	
  						DdNode* tmp_bdd, *varBdd;
  						varBdd = build_var_bdd(abpds_trans->var, abpds_trans->rightSide->aux.rel);
  						Cudd_Ref(tmp_bdd = Cudd_bddAnd(CTLBDDMgr,accept,varBdd));
  						Cudd_RecursiveDeref(CTLBDDMgr,varBdd);
  						if(tmp_bdd==CTLEmptyset) { Cudd_RecursiveDeref(CTLBDDMgr,tmp_bdd); continue;}
        			bdd=varExistAbstract(tmp_bdd,abpds_trans->var);	
        			Cudd_RecursiveDeref(CTLBDDMgr,tmp_bdd);				
  					}
  					if(bdd==CTLEmptyset) { Cudd_RecursiveDeref(CTLBDDMgr,bdd); continue; } 
  				}		
					AFARightSideofTransition tmp2 = AFAInitRightSideofTrans(afa.finalState,0,NULL,NULL);
          
					AFATransition added_trans = AFACreateTrans(abpds_trans->fromState, bdd,abpds_trans->fromStack, 1, tmp2);
					AFAAppend2TransList(preStarTable, added_trans);				
  			}
  		}
  		else         // deal with <p,r>->{<p1,w1>,...,<pn,wn>}
  		{
  			ABPDSRightSideofTransition  tmp2;
  			for( tmp2 = abpds_trans->rightSide; tmp2; tmp2 = tmp2->next)
  			{
  				if ((accept =ABPDSCheckStateAcceptiness(accepting, tmp2->state))&& ABPDSCheckStateAcceptiness(accepting, abpds_trans->fromState))
  				{
  					bdd=NULL;
  					if(mc_CTPL)
  					{
  						if(abpds_trans->rel==TOP)
  						{
  							DdNode* tmp = build_var_bdd(abpds_trans->var, tmp2->aux.rel);
  							Cudd_Ref(bdd=Cudd_bddAnd(CTLBDDMgr,accept,tmp));	
  							Cudd_RecursiveDeref(CTLBDDMgr,tmp);
  							if(bdd==CTLEmptyset) { Cudd_RecursiveDeref(CTLBDDMgr,bdd); continue; }   							
  							tmp=varExistAbstract(bdd,abpds_trans->var);
  							Cudd_RecursiveDeref(CTLBDDMgr,bdd);
  							bdd =tmp;
  							//printf("<p,r>->{<p1,w1>,...,<pn,wn>}:"); Cudd_PrintMinterm(CTLBDDMgr,bdd);
  						}
  						if(abpds_trans->rel==EQUAL ||abpds_trans->rel==CONJ) Cudd_Ref(bdd =accept);
      				if(abpds_trans->rel==VAREQU ||abpds_trans->rel== VARNEQU)  
      				{
                DdNode* tmp_bdd;
          			tmp_bdd = build_var_bdd( abpds_trans->rightSide->aux.rel, abpds_trans->fromStack);	
          			if(abpds_trans->rel== VARNEQU) tmp_bdd=Cudd_Not(tmp_bdd);
          			Cudd_Ref(bdd=Cudd_bddAnd(CTLBDDMgr, accept, tmp_bdd));		
          			Cudd_RecursiveDeref(CTLBDDMgr,tmp_bdd);   					
      				}  				 
  				    if(bdd==CTLEmptyset) { Cudd_RecursiveDeref(CTLBDDMgr,bdd); continue; } 
  				  } 	
  					ABPDSTransition tmp3 = ABPDSCreateTransition(abpds_trans->fromState, abpds_trans->rel, abpds_trans->fromStack, NULL);
    				tmp3->bdd = bdd;   
    				tmp3->var= abpds_trans->var;
						ABPDSRightSideofTransition tmp4,tmp5;
						tmp4=abpds_trans->rightSide; 
						while(tmp4)
						{
							if(tmp4== tmp2) { tmp4=tmp4->next; continue; }
							if ((accept =ABPDSCheckStateAcceptiness(accepting, tmp4->state)))
							{
  							if(mc_CTPL)
  							{		
      						if(abpds_trans->rel==TOP)
      						{
      							DdNode* tmp = build_var_bdd(abpds_trans->var, tmp4->aux.rel);
      							Cudd_Ref(bdd=Cudd_bddAnd(CTLBDDMgr,accept,tmp));	
      							Cudd_RecursiveDeref(CTLBDDMgr,tmp);
      							if(bdd!=CTLEmptyset)
      							{
      								tmp = varExistAbstract(bdd,abpds_trans->var);
      								Cudd_RecursiveDeref(CTLBDDMgr,bdd);
      								Cudd_Ref(bdd =Cudd_bddAnd(CTLBDDMgr,tmp3->bdd,tmp));
      								Cudd_RecursiveDeref(CTLBDDMgr,tmp);
      							}      							
      						}
      						if(abpds_trans->rel==CONJ) Cudd_Ref(bdd =Cudd_bddAnd(CTLBDDMgr,tmp3->bdd,accept)); 
      						if(abpds_trans->rel==VAREQU || abpds_trans->rel==VARNEQU ) 
      						{
                    DdNode* tmp_bdd;
              			tmp_bdd = build_var_bdd(tmp4->aux.rel, abpds_trans->fromStack);		
              			if(abpds_trans->rel== VARNEQU) tmp_bdd=Cudd_Not(tmp_bdd);
              			Cudd_Ref(bdd=Cudd_bddAnd(CTLBDDMgr, accept, tmp_bdd));		
              			Cudd_RecursiveDeref(CTLBDDMgr,tmp_bdd);
              			if(bdd==CTLEmptyset) 
              			{ 
              				Cudd_RecursiveDeref(CTLBDDMgr,bdd); 
              				ABPDSDestoryTransition(tmp3);
              				goto L1;
              			} 
              			if(tmp3->bdd)
              			{
              				Cudd_Ref(tmp_bdd = Cudd_bddAnd(CTLBDDMgr,tmp3->bdd,bdd));
              				Cudd_RecursiveDeref(CTLBDDMgr,bdd);
              				bdd=tmp_bdd;
              			}
      						}  							
      						if(bdd==CTLEmptyset) 
      						{ Cudd_RecursiveDeref(CTLBDDMgr,bdd);  ABPDSDestoryTransition(tmp3); goto L1; } 
      						else 
      						{ tmp3->bdd=bdd; 	Cudd_RecursiveDeref(CTLBDDMgr,bdd);							}
  							}
  							tmp4 = tmp4->next; 							
							}
							else
							{
      					tmp5 = ABPDSInitRightSideofTransition(tmp4->state,0,tmp4->stack1,0);
              	tmp5->next = tmp3->rightSide;
              	tmp5->aux.rel =tmp4->aux.rel;
              	tmp3->rightSide = tmp5;	 
              	tmp4 = tmp4->next; 							
              }
						}
						if(tmp3->rightSide)
						{
							if(ABPDSStrictAppend2TransList(abpds_mid_table,tmp3))
							{
    						if(!abpds_mid_trans) {	abpds_mid_trans_last=abpds_mid_trans=tmp3; }
    						else	{ abpds_mid_trans_last->next=tmp3; abpds_mid_trans_last =tmp3; }
							}
            }
            else
            {
							AFARightSideofTransition tmp_rs = AFAInitRightSideofTrans(afa.finalState,0,NULL,NULL);
              if(tmp3->bdd) Cudd_Ref(tmp3->bdd);
							AFATransition added_trans = AFACreateTrans(abpds_trans->fromState, tmp3->bdd, abpds_trans->fromStack, 1, tmp_rs);
							AFAAppend2TransList(preStarTable, added_trans);		 
							ABPDSDestoryTransition(tmp3);           	
            }
L1:					break;  											
  				}
  			}
  		}	

  	}
/*
		for(abpds_trans = abpds_mid_trans; abpds_trans; abpds_trans =  abpds_trans->next) 
		{	
			if (abpds_trans->rightSide->next == NULL )  // deal with <p,r>-><p1,w1>
			{
				if ((accept = ABPDSCheckStateAcceptiness(accepting, abpds_trans->rightSide->state)))
				{
					if(mc_CTPL)
  				{
  					if(abpds_trans->rel==TOP)
  					{
  						DdNode* tmp = build_var_bdd(abpds_trans->var, abpds_trans->rightSide->aux.rel);
  						Cudd_Ref(bdd=Cudd_bddAnd(CTLBDDMgr,accept,tmp));	
  						Cudd_RecursiveDeref(CTLBDDMgr,tmp);
  						if(bdd!=CTLEmptyset)
  						{
  							tmp = varExistAbstract(bdd,abpds_trans->var);
  							Cudd_RecursiveDeref(CTLBDDMgr,bdd);
  							Cudd_Ref(bdd =Cudd_bddAnd(CTLBDDMgr,abpds_trans->bdd,tmp));
  							Cudd_RecursiveDeref(CTLBDDMgr,tmp);
  						}      							
  					}
  					if(abpds_trans->rel==CONJ) Cudd_Ref(bdd =Cudd_bddAnd(CTLBDDMgr,abpds_trans->bdd,accept));
  					if(abpds_trans->rel==VAREQU || abpds_trans->rel==VARNEQU ) 
  					{
              DdNode* tmp_bdd;
        			tmp_bdd = build_var_bdd( abpds_trans->rightSide->aux.rel, abpds_trans->fromStack);	
        			if(abpds_trans->rel== VARNEQU) tmp_bdd=Cudd_Not(tmp_bdd);
        			Cudd_Ref(bdd=Cudd_bddAnd(CTLBDDMgr, accept, tmp_bdd));		
        			Cudd_RecursiveDeref(CTLBDDMgr,tmp_bdd);
        			if(bdd==CTLEmptyset) { Cudd_RecursiveDeref(CTLBDDMgr,bdd); continue; } 
        			if(abpds_trans->bdd)
        			{
        				Cudd_Ref(tmp_bdd = Cudd_bddAnd(CTLBDDMgr,abpds_trans->bdd,bdd));
        				Cudd_RecursiveDeref(CTLBDDMgr,bdd);
        				bdd=tmp_bdd;
        			}
  					}
  					if(bdd==CTLEmptyset) {Cudd_RecursiveDeref(CTLBDDMgr,bdd);  continue; }
  			     //deal with  case <p,r>-><p1,r1>
					}
					AFARightSideofTransition tmp2 = AFAInitRightSideofTrans(afa.finalState, 0, NULL,NULL);

					AFATransition added_trans = AFACreateTrans(abpds_trans->fromState, bdd, abpds_trans->fromStack, 1, tmp2);
					AFAAppend2TransList(preStarTable, added_trans);			
					RemoveHeadTable(abpds_mid_table,abpds_trans )	;	  				 																		
				}
				
				
			}
			else         // deal with <p,r>->{<p1,w1>,...,<pn,wn>}
  		{
  			ABPDSRightSideofTransition  tmp2;
  			//int accept_flag=0;
  			ABPDSTransition tmp3=NULL;
  			for( tmp2 = abpds_trans->rightSide; tmp2; tmp2 = tmp2->next)
  			{
  				if ((accept =ABPDSCheckStateAcceptiness(accepting, tmp2->state)))
  				{
  					//accept_flag++;
  					if(mc_CTPL)
    				{
  						if(abpds_trans->rel==TOP)
  						{
  							DdNode* tmp = build_var_bdd(abpds_trans->var, tmp2->aux.rel);
  							Cudd_Ref(bdd=Cudd_bddAnd(CTLBDDMgr,accept,tmp));	
  							Cudd_RecursiveDeref(CTLBDDMgr,tmp);
  							if(bdd!=CTLEmptyset)
  							{
  								tmp = varExistAbstract(bdd,abpds_trans->var);
  								Cudd_RecursiveDeref(CTLBDDMgr,bdd);
  								Cudd_Ref(bdd =Cudd_bddAnd(CTLBDDMgr,abpds_trans->bdd,tmp));
  								Cudd_RecursiveDeref(CTLBDDMgr,tmp);
  							}      							
  						}
  						if(abpds_trans->rel==CONJ) Cudd_Ref(bdd =Cudd_bddAnd(CTLBDDMgr,abpds_trans->bdd,accept)); 
  						if(abpds_trans->rel==VAREQU || abpds_trans->rel==VARNEQU ) 
  						{
                DdNode* tmp_bdd;
          			tmp_bdd = build_var_bdd(tmp2->aux.rel, abpds_trans->fromStack);		
          			if(abpds_trans->rel== VARNEQU) tmp_bdd=Cudd_Not(tmp_bdd);
          			Cudd_Ref(bdd=Cudd_bddAnd(CTLBDDMgr, accept, tmp_bdd));		
          			Cudd_RecursiveDeref(CTLBDDMgr,tmp_bdd);
          			if(bdd==CTLEmptyset) { Cudd_RecursiveDeref(CTLBDDMgr,bdd); continue; } 
          			if(abpds_trans->bdd)
          			{
          				Cudd_Ref(tmp_bdd = Cudd_bddAnd(CTLBDDMgr,abpds_trans->bdd,bdd));
          				Cudd_RecursiveDeref(CTLBDDMgr,bdd);
          				bdd=tmp_bdd;
          			}
  						}  							
  						if(bdd==CTLEmptyset) { Cudd_RecursiveDeref(CTLBDDMgr,bdd);  continue; }
  				  } 
  					tmp3 = ABPDSCreateTransition(abpds_trans->fromState,abpds_trans->rel,abpds_trans->fromStack,NULL);
    				tmp3->bdd = bdd;
    				tmp3->var=	abpds_trans->var;				
						ABPDSRightSideofTransition tmp4,tmp5;
						for (tmp4=abpds_trans->rightSide; tmp4 ; tmp4 = tmp4->next)
						{ 
							  if(tmp4==tmp2) continue;
      					tmp5 = ABPDSInitRightSideofTransition(tmp4->state, 0,tmp4->stack1,0);
              	tmp5->next = tmp3->rightSide;
              	tmp5->aux.rel =tmp4->aux.rel;
              	tmp3->rightSide = tmp5;	  							             
						} 					
    				
    				if(!abpds_mid_trans) { abpds_mid_trans_last=abpds_mid_trans=tmp3;  }
    				else	{ abpds_mid_trans_last->next=tmp3; abpds_mid_trans_last =tmp3; }  
    				ABPDSStrictAppend2TransList(abpds_mid_table,tmp3); 
    				RemoveHeadTable(abpds_mid_table,abpds_trans);	 	
						break;  											
  				}
  			}
  		
  		}
		} 	
*/
    
  	while((base=afa.stack))              //waiting_trans is stack, LIFO
  	{
  		afa.stack=base->fwdStack;
  		base->fwdStack=base->bwdStack=NULL;
  		if(afa.stack) afa.stack->bwdStack=NULL;
  		//AFAPrintTrans(base);
  		//printf("     %d\n", base->boolflag);
			if (!(base->boolflag))
			{
				//AFAPrintTrans(base);
				head=ABPDS_head_find(abpds->transList, base->fromState,base->inputStack);
				rule =head? head->rules: NULL;
    		
    		for( ;rule; rule = rule->next)
    		{
    			//printf("111\n");
    			abpds_trans =rule->rule;
    			if (abpds_trans->rightSide->next == NULL) // deal with <p,r>-><p1,w1>
    			{
    				bdd=NULL;
    				if (mc_CTPL)	{		if(! (bdd = Rel2BDD(abpds_trans, base))) continue; }
    					
						if (abpds_trans->rightSide->stack2 == 0 )      //deal with  case <p,r>-><p1,r1>
						{
							AFARightSideofTransition tmp2, tmp3, tmp4 = NULL;
							for(tmp2 = base->rightSide; tmp2; tmp2 = tmp2->next )
							{
								tmp3 = AFAInitRightSideofTrans(tmp2->state ,tmp2->index,NULL,tmp4);
								if(tmp2->Binding&&mc_CTPL) Cudd_Ref(tmp3->Binding =tmp2->Binding);
								tmp4 = tmp3;
							}
							AFATransition added_trans = AFACreateTrans(abpds_trans->fromState, bdd, abpds_trans->fromStack, level, tmp4);
							AFAAppend2TransList(preStarTable, added_trans);	
						}
						else              //deal with  case <p,r>-><p1,r1r2>
						{
							ABPDSTransition tmp1 = ABPDSCreateTransition(abpds_trans->fromState,abpds_trans->rel,abpds_trans->fromStack,NULL);
  				     // The rel should be EQUAL.
							tmp1->bdd = bdd;
							tmp1->var=abpds_trans->var;
							
							AFARightSideofTransition tmp2 = NULL;
							ABPDSRightSideofTransition tmp3 = NULL; 
							for (tmp2 = base->rightSide; tmp2 ; tmp2 = tmp2->next)
							{
								if (ABPDSCheckStateAcceptiness(accepting, tmp2->state)) continue;
								if (tmp2->state==afa.finalState) continue;
									
								tmp3 = ABPDSInitRightSideofTransition(tmp2->state,1,abpds_trans->rightSide->stack2,0);
								if(tmp2->Binding&&mc_CTPL) Cudd_Ref(tmp3->aux.Binding = tmp2->Binding);
								tmp3->next = tmp1->rightSide;    								
								tmp1->rightSide =tmp3;
							}
							if (tmp1->rightSide)
							{
								if(ABPDSStrictAppend2TransList(abpds_mid_table,tmp1))
								{		     					
    							if(!abpds_mid_trans) {	 abpds_mid_trans_last=abpds_mid_trans=tmp1; }
    							else	{ abpds_mid_trans_last->next=tmp1; abpds_mid_trans_last =tmp1; }
    			
  								AFATransition tmp4 = NULL;
  								for(tmp3=tmp1->rightSide; tmp3; tmp3=tmp3->next)
  								{
  									AFAhead = afatrans_head_table_find(preStarTable,tmp3->state,tmp3->stack1);
  									if(AFAhead) tmp4=AFAhead->rules;
  									while(tmp4)
  									{
  										if(!tmp4->fwdStack && !tmp4->bwdStack)
  										{
  											tmp4->fwdStack=afa.stack;
  											if(afa.stack) afa.stack->bwdStack=tmp4;
  											tmp4->boolflag=1;
  											afa.stack =tmp4;
  										}
  										tmp4=tmp4->next;
  									}   
    							}									
  							}	
							}
							else
							{
								ABPDSDestoryTransition(tmp1);
								AFARightSideofTransition tmp4 =AFAInitRightSideofTrans(afa.finalState,0,NULL,NULL);
								bdd=NULL;
								if(base->Binding&&mc_CTPL) Cudd_Ref(bdd =base->Binding);
								AFATransition added_trans = AFACreateTrans(abpds_trans->fromState,  bdd, abpds_trans->fromStack, level, tmp4);
								AFAAppend2TransList(preStarTable,added_trans);  								
							}
						}
    			}
    			else         // deal with <p,r>->{<p1,w1>,...,<pn,wn>}
    			{
    				ABPDSRightSideofTransition tmp2;
    				for (tmp2 = abpds_trans->rightSide ; tmp2; tmp2 = tmp2->next )
    				{
    					if (tmp2->state == base->fromState && tmp2->stack1 == base->inputStack)
							{  						
								if(mc_CTPL)
								{
      						if(abpds_trans->rel==TOP)
      						{
      							DdNode* tmp = build_var_bdd(abpds_trans->var, tmp2->aux.rel);
      							Cudd_Ref(bdd=Cudd_bddAnd(CTLBDDMgr,base->Binding,tmp));	
      							Cudd_RecursiveDeref(CTLBDDMgr,tmp);
      							if(bdd==CTLEmptyset) { Cudd_RecursiveDeref(CTLBDDMgr,bdd); continue; } 
      							tmp=varExistAbstract(bdd,abpds_trans->var);
      							Cudd_RecursiveDeref(CTLBDDMgr,bdd);
      							bdd =tmp;      							
      						}
      						if(abpds_trans->rel==EQUAL || abpds_trans->rel==CONJ) Cudd_Ref(bdd =base->Binding);      						 
      						if(abpds_trans->rel==VAREQU || abpds_trans->rel==VARNEQU)
      						{
              			DdNode* tmp = build_var_bdd( tmp2->aux.rel, abpds_trans->fromStack);	
              			if(abpds_trans->rel==VARNEQU) tmp=Cudd_Not(tmp);
              			Cudd_Ref(bdd=Cudd_bddAnd(CTLBDDMgr,base->Binding,tmp));		
              			Cudd_RecursiveDeref(CTLBDDMgr,tmp); 
              		}
      						if(bdd==CTLEmptyset) { Cudd_RecursiveDeref(CTLBDDMgr,bdd);  continue; }
      					}
    					  break;
    					}
    				}
    				if (tmp2)
    				{    				
    					ABPDSTransition tmp3 = ABPDSCreateTransition(abpds_trans->fromState, abpds_trans->rel,abpds_trans->fromStack,NULL);
  						tmp3->bdd = bdd;
  						tmp3->var =abpds_trans->var;
        			ABPDSRightSideofTransition tmp4,tmp5;
        			for ( tmp4= abpds_trans->rightSide; tmp4 ; tmp4 = tmp4->next)
        			{
        				if(tmp4==tmp2) continue;
        				tmp5 = ABPDSInitRightSideofTransition(tmp4->state, 0,tmp4->stack1,0);
                tmp5->aux.rel =tmp4->aux.rel;
                tmp5->next = tmp3->rightSide;
                tmp3->rightSide = tmp5;		
        			}
        			AFARightSideofTransition tmp6;
        			ABPDSTransLabel tmp7;
        			for (tmp6 = base->rightSide ;tmp6; tmp6 = tmp6->next) 
        			{
        				if (tmp6->state==afa.finalState) continue;
      					tmp7 = CreateABPDSTransLabel( tmp6->state,NULL,tmp6->index);
    						if(tmp6->Binding&&mc_CTPL) Cudd_Ref(tmp7->Binding =tmp6->Binding);
    						tmp7->next = tmp3->label;
    						tmp3->label = tmp7;
      				}   
      				if(ABPDSStrictAppend2TransList(abpds_mid_table,tmp3))
							{     					
    						if(!abpds_mid_trans) {abpds_mid_trans_last=abpds_mid_trans=tmp3; }
    						else	{ abpds_mid_trans_last->next=tmp3; abpds_mid_trans_last =tmp3; }
    					}
    				}
    			}
    		}
				if (mc_CTPL)
				{
					head=ABPDS_head_find(abpds->transList, base->fromState, 0);
					if(head) rule =head->rules;
    			else rule=NULL;
    			for(; rule; rule = rule->next)
    		  {					
    		  	abpds_trans =rule->rule;
						if((bdd=Rel2BDD(abpds_trans,base)))
						{
  						AFARightSideofTransition tmp2;
  						tmp2 = AFAInitRightSideofTrans(base->fromState,base->index,NULL,NULL);
  						Cudd_Ref(tmp2->Binding =base->Binding);
  						AFATransition added_trans = AFACreateTrans(abpds_trans->fromState, bdd, abpds_trans->fromStack, level, tmp2);
  					  AFAAppend2TransList(preStarTable,added_trans);    								
  					}
  				}
				}				    		
    	}
          	
			head=ABPDS_head_find(abpds_mid_table, base->fromState,base->inputStack);
			if(head) rule =head->rules;
    	else rule=NULL;    
    			
  		for(; rule; rule = rule->next)
  		{ 
  			
  			abpds_trans= rule->rule;
  			if(abpds_trans==NULL) continue;
  			if (abpds_trans->rightSide->index==0 && base->boolflag) continue; // AFA transition with boolflag =1 comes 
  																																				// from p,r->p,r1 r2, so the index should great than 0.
  			//printf("222 \n");
  			//ABPDSPrintTransition(abpds_trans);
  			int elm_flag=0;
  			if (abpds_trans->rightSide->next == NULL) // deal with <p,r>-><p1,w1>
  			{
  				if (!abpds_trans->rightSide->index || abpds_trans->rightSide->aux.Binding==base->Binding ) 
  				{
  					bdd=NULL;
					  if(mc_CTPL) { if(! (bdd = Rel2BDD(abpds_trans,base))) continue; }
				
						if (abpds_trans->rightSide->stack2 == 0 )      //deal with  case <p,r>-><p1,r1>
						{
							AFARightSideofTransition tmp2, tmp3, tmp4 = NULL;
							for(tmp2 = base->rightSide; tmp2; tmp2 = tmp2->next )
							{
								if (tmp2->state == afa.finalState) continue;
								tmp3 = AFAInitRightSideofTrans(tmp2->state,tmp2->index,NULL,tmp4);
								if(tmp2->Binding&&mc_CTPL) Cudd_Ref(tmp3->Binding =tmp2->Binding);
								tmp4 = tmp3;
							}
							AFARightSideofTransition tmp5 = NULL;
							ABPDSTransLabel tmp6=NULL;
							for(tmp6 =abpds_trans->label; tmp6; tmp6 = tmp6->next)   // union of the label states and the right-hand side of the AFA transition 
							{
								if (tmp6->state==afa.finalState) continue;
								for (tmp5 = tmp4; tmp5; tmp5= tmp5->next )
								{
									if (tmp5->state==tmp6->state && tmp5->Binding ==tmp6->Binding) 
									{
										if( tmp5->index>=tmp6->index)	break;
										else { tmp5->index=tmp6->index;	break;	}
									}
								}
								if(!tmp5)
								{
									tmp3 = AFAInitRightSideofTrans(tmp6->state,tmp6->index,NULL,tmp4 );
									if(tmp6->Binding&&mc_CTPL)  Cudd_Ref(tmp3->Binding =tmp6->Binding);
									tmp4 = tmp3;
									elm_flag++;
								}  								
							}
							if(!tmp4)
							{
								tmp4 = AFAInitRightSideofTrans(afa.finalState,0,NULL,NULL);
							}
							AFATransition added_trans = AFACreateTrans(abpds_trans->fromState, bdd, abpds_trans->fromStack, level, tmp4);
							AFAAppend2TransList(preStarTable, added_trans);	 	
  						if(!elm_flag) 
  						{ 
  							if(mc_CTPL)
  							{
  								if(bdd==abpds_trans->bdd) RemoveHeadTable(abpds_mid_table,abpds_trans);
  							}
  							else 	RemoveHeadTable(abpds_mid_table,abpds_trans);
  						}												 						
  					}
  				}           // mid ABPDS transition rule does not exist case <p,r>-><p1,r1 r2> 
  			}
  			else         // deal with <p,r>->{<p1,w1>,...,<pn,wn>}
  			{
  				ABPDSRightSideofTransition tmp2;
  				for (tmp2 = abpds_trans->rightSide ; tmp2; tmp2 = tmp2->next )
  				{
  					if (tmp2->state == base->fromState && tmp2->stack1 == base->inputStack && \
  						 (!abpds_trans->rightSide->index ||tmp2->aux.Binding==base->Binding ))
  					{  
  						if(mc_CTPL)
    					{		 
    						if(abpds_trans->rel==TOP)
    						{
    							DdNode* tmp = build_var_bdd(abpds_trans->var, tmp2->aux.rel);
    							Cudd_Ref(bdd=Cudd_bddAnd(CTLBDDMgr,base->Binding,tmp));	
    							Cudd_RecursiveDeref(CTLBDDMgr,tmp);
    							if(bdd!=CTLEmptyset)
    							{
    								tmp = varExistAbstract(bdd,abpds_trans->var);
    								Cudd_RecursiveDeref(CTLBDDMgr,bdd);
    								Cudd_Ref(bdd = Cudd_bddAnd(CTLBDDMgr,abpds_trans->bdd ,tmp));	
    								Cudd_RecursiveDeref(CTLBDDMgr,tmp);
    							}      							
    						}
    						if(abpds_trans->rel==EQUAL) 
    							Cudd_Ref(bdd = abpds_trans->bdd); 
    						if(abpds_trans->rel==CONJ) 
    							Cudd_Ref(bdd =Cudd_bddAnd(CTLBDDMgr,abpds_trans->bdd,base->Binding));
    						if(abpds_trans->rel==VAREQU || abpds_trans->rel==VARNEQU)
    						{
            			DdNode* tmp = build_var_bdd( tmp2->aux.rel, abpds_trans->fromStack);		
            			if(abpds_trans->rel==VARNEQU) tmp=Cudd_Not(tmp);
            			Cudd_Ref(bdd=Cudd_bddAnd(CTLBDDMgr,base->Binding,tmp));		
            			Cudd_RecursiveDeref(CTLBDDMgr,tmp); 
            			if(bdd!=CTLEmptyset)
            			{
    								Cudd_Ref(tmp = Cudd_bddAnd(CTLBDDMgr,abpds_trans->bdd, bdd));	
    								Cudd_RecursiveDeref(CTLBDDMgr,bdd);     
    								bdd=tmp;       				
            			}
            		}    	               
    						if(bdd==CTLEmptyset) { Cudd_RecursiveDeref(CTLBDDMgr,bdd); continue; }
    					}
    					break;    						
  					}
  				}
  				if (tmp2)
  				{    				
  					ABPDSTransition tmp3 = ABPDSCreateTransition(abpds_trans->fromState,abpds_trans->rel,abpds_trans->fromStack,NULL);
						tmp3->bdd =bdd;
						tmp3->var=abpds_trans->var;
					
      			ABPDSRightSideofTransition tmp4,tmp5;
      			for (tmp4 = abpds_trans->rightSide; tmp4; tmp4 = tmp4->next)
      			{
      				if(tmp4 == tmp2) continue;
      				tmp5 = ABPDSInitRightSideofTransition(tmp4->state,tmp4->index,tmp4->stack1,0);                
              if(tmp4->index&& tmp4->aux.Binding) Cudd_Ref(tmp5->aux.Binding =tmp4->aux.Binding);
              else tmp5->aux.rel =tmp4->aux.rel;
              tmp5->next = tmp3->rightSide;
              tmp3->rightSide = tmp5;		
              if(mc_pds_line_num>=2) break;
      			}
    				      			
      			AFARightSideofTransition tmp6;
      			ABPDSTransLabel tmp7,tmp9;
      			for (tmp9 = abpds_trans->label; tmp9; tmp9 = tmp9->next)
      			{
     					tmp7 = CreateABPDSTransLabel(tmp9->state, NULL,tmp9->index);
  						if(tmp9->Binding&&mc_CTPL) Cudd_Ref(tmp7->Binding = tmp9->Binding);
  						tmp7->next = tmp3->label;
  						tmp3->label = tmp7;     				
      			}	
      			int elm_flag=0;
      			for (tmp6 = base->rightSide ;tmp6; tmp6 = tmp6->next)         // union of the states.
      			{
      				if (tmp6->state==afa.finalState) continue;
      				ABPDSTransLabel tmp8;
      				for(tmp8 =tmp3->label; tmp8; tmp8 = tmp8->next)
      				{
      					if(tmp8->state == tmp6->state&& tmp8->Binding== tmp6->Binding) 
								{
									if( tmp8->index>=tmp6->index)	break;
									else { tmp8->index=tmp6->index; break; }
								}
      				}
      				if (!tmp8)
      				{
      					tmp7 =CreateABPDSTransLabel( tmp6->state, NULL,tmp6->index);
    						if(tmp6->Binding&&mc_CTPL) Cudd_Ref(tmp7->Binding = tmp6->Binding);
    						tmp7->next = tmp3->label;
    						tmp3->label = tmp7;
    						elm_flag++;
    					}      				
      			}     					      			

    				if(ABPDSStrictAppend2TransList(abpds_mid_table,tmp3))
						{     					
  						if(!abpds_mid_trans) {	 abpds_mid_trans_last=abpds_mid_trans=tmp3; }
  						else	{ abpds_mid_trans_last->next=tmp3; abpds_mid_trans_last =tmp3; }
	         //   ABPDSRightSideofTransition tmpsss=tmp3->rightSide;
	         //   while(tmpsss)  	{printf("22222222\n"); if(tmpsss->aux.Binding&& tmpsss->index) printf("22222222333333333333\n"); tmpsss=tmpsss->next;}
	            		
							if(!elm_flag) 
							{ 
								if(mc_CTPL)
								{
									if(bdd==abpds_trans->bdd) RemoveHeadTable(abpds_mid_table,abpds_trans);
								}
								else 	RemoveHeadTable(abpds_mid_table,abpds_trans);
							}    					
  					}
  				}
  			}
  		}
  	}
  //	printf("*1100000000000\n");	
  }  
  else
  {
  	if(isdebug) printf("******************** I am in %d level.***********\n\n", level);
    for(i=0; i<baseTable->size; i++)
    {
    	AFATransHead list =baseTable->data[i];
    	while(list)
    	{
    		if (!ABPDSCheckStateAcceptiness(accepting, list->state)) 	{ list=list->next; continue; }
    	  base= list->rules;
   			for (; base; base = base->next)
      	{
      		//if(!base->fromState||!base->inputStack)  printf("NULL          NULL\n");
  				head = ABPDS_head_find(abpds->transList, base->fromState,base->inputStack);
  				if(head) rule =head->rules;
      		else rule=NULL;
      			
      		for (; rule; rule = rule->next)
      		{
      			abpds_trans = rule->rule;
      			if (abpds_trans->rightSide->next == NULL)  // deal with <p,r>-><p1,w1>
      			{
    					if(mc_CTPL)	{ if(! (bdd =Rel2BDD(abpds_trans, base))) continue; }
    						
    					if (abpds_trans->rightSide->stack2 ==0)   //deal with  case <p,r>-><p1,r1>
    					{ 
    						//if (isdebug) printf("The type of the above transition is: <p,r>-><p1,r1>\n\n");    
  							AFARightSideofTransition tmp2, tmp3, tmp4 = NULL;
  							for(tmp2 = base->rightSide; tmp2; tmp2 = tmp2->next )
  							{
  								tmp3 = AFAInitRightSideofTrans(tmp2->state,tmp2->index, NULL,tmp4);
  								if(tmp2->Binding&&mc_CTPL) Cudd_Ref(tmp3->Binding =tmp2->Binding);
  								tmp4 = tmp3;
  							}  							 
  					  	AFATransition added_trans = AFACreateTrans(abpds_trans->fromState, bdd, abpds_trans->fromStack, level, tmp4);
  							AFAAppend2TransList(preStarTable, added_trans);	
  						}
  						else   //deal with case <p,r>-><p1,r1r2> 
  						{ 
  							//if (isdebug) printf("The type of the above transition is: <p,r>-><p1,r1r2> \n\n");  
 								AFARightSideofTransition tmp1;
  							ABPDSRightSideofTransition tmp2,tmp3 = NULL;
  							
								for (tmp1 = base->rightSide; tmp1; tmp1 = tmp1->next)
								{        					
									if (tmp1->state == afa.finalState) continue;
        					tmp2 = ABPDSInitRightSideofTransition(tmp1->state,tmp1->index,abpds_trans->rightSide->stack2,0);
        					if(tmp1->Binding&&mc_CTPL) Cudd_Ref(tmp2->aux.Binding =tmp1->Binding);
        					tmp2->next = tmp3;
									tmp3 = tmp2;
																
        				}
        				if (tmp3)
        				{
  								ABPDSTransition tmp4 = ABPDSCreateTransition(abpds_trans->fromState, EQUAL,abpds_trans->fromStack, tmp3);
  								tmp4->bdd =bdd;
  								tmp4->var=abpds_trans->var;
    							if(ABPDSStrictAppend2TransList(abpds_mid_table,tmp4))
    							{     					
        						if(!abpds_mid_trans) {abpds_mid_trans_last=abpds_mid_trans=tmp4; }
        						else	{ abpds_mid_trans_last->next=tmp4; abpds_mid_trans_last =tmp4; }
        					}
								}
								else
								{
  								AFARightSideofTransition tmp4 =AFAInitRightSideofTrans(afa.finalState,0,NULL,NULL);
  								AFATransition added_trans = AFACreateTrans(abpds_trans->fromState, bdd, abpds_trans->fromStack, level, tmp4);
  								AFAAppend2TransList(preStarTable, added_trans);	 									
								}	
  						}      			
      			}
      			else         // deal with <p,r>->{<p1,w1>,...,<pn,wn>}
      			{
      				if (isdebug) 
      				{	
      					printf("The type of the above transition is 2222: {<p1,w1>,...,<pn,wn>} \n\n");  
      				}
     					ABPDSRightSideofTransition  tmp;
          		for (tmp = abpds_trans->rightSide; tmp; tmp = tmp->next)
          		{
          			if (tmp->stack1 == base->inputStack && tmp->state == base->fromState )
          			{
          				if(mc_CTPL)
          				{
        						if(abpds_trans->rel==TOP)
        						{
        							DdNode *tmp1;
        							bdd = build_var_bdd(abpds_trans->var, tmp->aux.rel);
        							Cudd_Ref(tmp1=Cudd_bddAnd(CTLBDDMgr,base->Binding,bdd));	
        							Cudd_RecursiveDeref(CTLBDDMgr,bdd);
        							if(tmp1==CTLEmptyset) { Cudd_RecursiveDeref(CTLBDDMgr,tmp1); continue; } 
        							bdd=varExistAbstract(tmp1,abpds_trans->var);
        							Cudd_RecursiveDeref(CTLBDDMgr,tmp1);
        						}
        						if(abpds_trans->rel==EQUAL ||abpds_trans->rel==CONJ) Cudd_Ref(bdd =base->Binding);     
        						if(abpds_trans->rel==VAREQU || abpds_trans->rel==VARNEQU)
        						{
                			DdNode* tmp1 = build_var_bdd( tmp->aux.rel, abpds_trans->fromStack);		
                			if(abpds_trans->rel==VARNEQU) tmp1=Cudd_Not(tmp1);
                			Cudd_Ref(bdd=Cudd_bddAnd(CTLBDDMgr,base->Binding,tmp1));		
                			Cudd_RecursiveDeref(CTLBDDMgr,tmp1); 
                		}
        						if(bdd==CTLEmptyset) { Cudd_RecursiveDeref(CTLBDDMgr,bdd);  continue; }
        					}
      					  break;
          			} 
          		}
          		if (tmp)
          		{
          			ABPDSTransition tmp3 = ABPDSCreateTransition(abpds_trans->fromState,abpds_trans->rel,abpds_trans->fromStack,NULL);
        				tmp3->bdd =bdd;
        				tmp3->var=abpds_trans->var;
          			ABPDSRightSideofTransition tmp2,tmp4;
          			for (tmp2 = abpds_trans->rightSide; tmp2 ; tmp2 = tmp2->next)
          			{
          				if(tmp2==tmp) continue;
          				tmp4 = ABPDSInitRightSideofTransition(tmp2->state,0,tmp2->stack1,0);
          				tmp4->aux.rel =tmp2->aux.rel;
                  tmp4->next = tmp3->rightSide;
                  tmp3->rightSide = tmp4;	
          			}
        				          			
          			ABPDSTransLabel tmp5; 
          			AFARightSideofTransition tmp6;
          			for (tmp6 = base->rightSide ;tmp6; tmp6 = tmp6->next)
          			{
          				if (tmp6->state==afa.finalState) continue;
          				tmp5 = CreateABPDSTransLabel( tmp6->state,NULL,tmp6->index);
        					if(tmp6->Binding&&mc_CTPL) Cudd_Ref(tmp5->Binding =tmp6->Binding);
        					tmp5->next = tmp3->label;
        					tmp3->label = tmp5;      				
          			}			
          			

    						if(ABPDSStrictAppend2TransList(abpds_mid_table,tmp3))
  							{     					
      						if(!abpds_mid_trans) { abpds_mid_trans_last=abpds_mid_trans=tmp3; }
      						else	{ abpds_mid_trans_last->next=tmp3; abpds_mid_trans_last =tmp3; }
      					}
          		}
      			}
      		}
      	  if(mc_CTPL)
      	  {
  					head = ABPDS_head_find(abpds->transList, base->fromState,0);
  					if(head) rule =head->rules;
      			else rule=NULL;
      			for (; rule; rule = rule->next)
      			{      		
      				abpds_trans=rule->rule;
    					if((bdd=Rel2BDD(abpds_trans,base)))
							{
  							AFARightSideofTransition tmp2;
								tmp2 = AFAInitRightSideofTrans(base->fromState, base->index,NULL,NULL);
								Cudd_Ref(tmp2->Binding =base->Binding);
								AFATransition added_trans = AFACreateTrans(abpds_trans->fromState, bdd, abpds_trans->fromStack, level, tmp2);
							  AFAAppend2TransList(preStarTable,added_trans);    								
							}															
						}
					}      		
      	}  	   
    	  list=list->next;	
    	}
    }
    for(i=0; i<baseTable->size; i++)
    {
    	AFATransHead list =baseTable->data[i];
    	while(list)
    	{
    	  base= list->rules;
   			for (; base; base = base->next)
      	{
  				head = ABPDS_head_find(abpds_mid_table, base->fromState,base->inputStack);
  				if(head) rule =head->rules;
      		else rule=NULL;
      		for (; rule; rule = rule->next)
      		{
      			abpds_trans = rule->rule;   
      			if(!abpds_trans) continue;
  					if ((abpds_trans->rightSide->index==0) &&!ABPDSCheckStateAcceptiness(accepting,base->fromState)) continue;
    				if (abpds_trans->rightSide->next == NULL) // deal with <p,r>-><p1,w1>
      			{
      				if(!abpds_trans->rightSide->index ||abpds_trans->rightSide->aux.Binding==base->Binding )
      				{
      					if(mc_CTPL) { if(! (bdd =Rel2BDD(abpds_trans, base))) continue;   }
    						 //deal with case <p,r>-><p1,r1>
    						AFARightSideofTransition tmp2, tmp3, tmp4 = NULL;
                
    						AFARightSideofTransition tmp5 = NULL;
    						ABPDSTransLabel tmp6=NULL;
    						for(tmp6 =abpds_trans->label; tmp6; tmp6 = tmp6->next)   // union of the label states and the right-hand side of the AFA transition 
    						{
    							if (tmp6->state==afa.finalState) continue;
    							tmp3 = AFAInitRightSideofTrans(tmp6->state,tmp6->index,NULL,tmp4);
    							if(tmp6->Binding&&mc_CTPL) Cudd_Ref(tmp3->Binding =tmp6->Binding);
    							tmp4 = tmp3;
    						}   								
                int elm_flag=0;
    						for(tmp2 = base->rightSide; tmp2; tmp2 = tmp2->next )
    						{
    							if (tmp2->state==afa.finalState) continue;
    							for (tmp5 = tmp4; tmp5; tmp5= tmp5->next )
    							{
    								if (tmp5->state == tmp2->state && tmp5->Binding == tmp2->Binding) 
    								{
    										if( tmp5->index>=tmp2->index)	break;
    										else	{ tmp5->index=tmp2->index; break; }	
    								}
    							}
    							if(!tmp5)
    							{
    								tmp3 = AFAInitRightSideofTrans(tmp2->state,tmp2->index,NULL,tmp4);
    								if(tmp2->Binding&&mc_CTPL) Cudd_Ref(tmp3->Binding =tmp2->Binding);
    								tmp4 = tmp3;
    								elm_flag++;
    							}   
    						}
    						    						
    						if (!tmp4)
    						{
    							tmp4 = AFAInitRightSideofTrans(afa.finalState,0,NULL,NULL);
    						}
    						AFATransition added_trans = AFACreateTrans(abpds_trans->fromState, bdd, abpds_trans->fromStack, level, tmp4);
    						AFAAppend2TransList(preStarTable, added_trans);	
  							if(!elm_flag) 
  							{ 
  								if(mc_CTPL)
  								{
  									if(bdd==abpds_trans->bdd) RemoveHeadTable(abpds_mid_table,abpds_trans);
  								}
  								else 	RemoveHeadTable(abpds_mid_table,abpds_trans);
  							}   	 
    					}						 						
      			}
      			else         // deal with <p,r>->{<p1,w1>,...,<pn,wn>}
      			{
      				ABPDSRightSideofTransition tmp2;
      				for (tmp2 = abpds_trans->rightSide ; tmp2; tmp2 = tmp2->next )
      				{
      					if (tmp2->state == base->fromState && tmp2->stack1 == base->inputStack &&\
      						 (!tmp2->index ||tmp2->aux.Binding==base->Binding ))
      					{
      						if(mc_CTPL)
      						{
        						if(abpds_trans->rel==TOP)
        						{
        							DdNode* tmp1 = build_var_bdd(abpds_trans->var, tmp2->aux.rel);
        							Cudd_Ref(bdd=Cudd_bddAnd(CTLBDDMgr,base->Binding,tmp1));	
        							Cudd_RecursiveDeref(CTLBDDMgr,tmp1);
        							if(bdd==CTLEmptyset) { Cudd_RecursiveDeref(CTLBDDMgr,bdd); continue; } 
        							tmp1=varExistAbstract(bdd,abpds_trans->var);
        							Cudd_RecursiveDeref(CTLBDDMgr,bdd);
        							Cudd_Ref(bdd=Cudd_bddAnd(CTLBDDMgr,abpds_trans->bdd,tmp1));	
        							Cudd_RecursiveDeref(CTLBDDMgr,tmp1);
        						}
        						if(abpds_trans->rel==EQUAL) Cudd_Ref(bdd = abpds_trans->bdd);
        						if(abpds_trans->rel==CONJ)	Cudd_Ref(bdd=Cudd_bddAnd(CTLBDDMgr,base->Binding, abpds_trans->bdd));
        							
        						if(abpds_trans->rel==VAREQU || abpds_trans->rel==VARNEQU)
        						{
                			DdNode* tmp1 = build_var_bdd( tmp2->aux.rel, abpds_trans->fromStack);		
                			if(abpds_trans->rel==VARNEQU) tmp1=Cudd_Not(tmp1);
                			Cudd_Ref(bdd=Cudd_bddAnd(CTLBDDMgr,base->Binding,tmp1));		
                			Cudd_RecursiveDeref(CTLBDDMgr,tmp1); 
                			if(bdd!=CTLEmptyset)
                			{
        								Cudd_Ref(tmp1 = Cudd_bddAnd(CTLBDDMgr,abpds_trans->bdd, bdd));	
        								Cudd_RecursiveDeref(CTLBDDMgr,bdd);     
        								bdd=tmp1;       				
                			}
                		}     										
        						if(bdd==CTLEmptyset) { Cudd_RecursiveDeref(CTLBDDMgr,bdd); continue; }   
        					}     						 
      						break;    						    						
      					} 
      				}
      				if (tmp2)
      				{    				
      					ABPDSTransition tmp3 = ABPDSCreateTransition(abpds_trans->fromState,abpds_trans->rel,abpds_trans->fromStack,NULL);
    						tmp3->bdd =bdd;
    						tmp3->var=abpds_trans->var;
          			ABPDSRightSideofTransition tmp4,tmp5;
          			for (tmp4 = abpds_trans->rightSide; tmp4 != tmp2; tmp4 = tmp4->next)
          			{
          				tmp5 = ABPDSInitRightSideofTransition(tmp4->state,tmp4->index,tmp4->stack1,0);
                  if(abpds_trans->rightSide->index && tmp4->aux.Binding) Cudd_Ref(tmp5->aux.Binding = tmp4->aux.Binding);
                  else tmp5->aux.rel = tmp4->aux.rel;
                  tmp5->next = tmp3->rightSide;
                  tmp3->rightSide = tmp5;		
                   if(mc_pds_line_num>=2) break;
          			}
          			AFARightSideofTransition tmp6;
          			ABPDSTransLabel tmp7,tmp9;
          			for (tmp9 = abpds_trans->label; tmp9; tmp9 = tmp9->next)
          			{
          				if (tmp9->state==afa.finalState) continue;
         					tmp7 = CreateABPDSTransLabel( tmp9->state,NULL,tmp9->index);
      						if(tmp9->Binding&&mc_CTPL) Cudd_Ref(tmp7->Binding=tmp9->Binding);
      						tmp7->next = tmp3->label;
      						tmp3->label = tmp7;     				
          			}	
          			int elm_flag=0;
          			for (tmp6 = base->rightSide ;tmp6; tmp6 = tmp6->next)
          			{
          				if (tmp6->state==afa.finalState) continue;
          				ABPDSTransLabel tmp8 = tmp3->label;
          				for(;tmp8;tmp8 =tmp8->next)
          				{
          					if(tmp8->state ==tmp6->state && tmp8->Binding ==tmp6->Binding)
          					{
    									if( tmp8->index>=tmp6->index)	break;
    									else 	{	tmp8->index=tmp6->index;	break;	}  
          					}
          				}
          				if (!tmp8)
          				{ 
          					
          					tmp7 = CreateABPDSTransLabel( tmp6->state,NULL,tmp6->index);
        						if(tmp6->Binding&& mc_CTPL) Cudd_Ref(tmp7->Binding =tmp6->Binding);
        						tmp7->next = tmp3->label;
        						tmp3->label = tmp7;
        						elm_flag++;
        					}      				
          			}
          					      			
        				for (tmp2 = tmp2->next; tmp2; tmp2 = tmp2->next)
        				{
          				tmp5 = ABPDSInitRightSideofTransition(tmp2->state, tmp2->index,tmp2->stack1,0);
                  if(abpds_trans->rightSide->index && tmp2->aux.Binding) Cudd_Ref(tmp5->aux.Binding = tmp2->aux.Binding);
                  else tmp5->aux.rel = tmp2->aux.rel; 
                  tmp5->next = tmp3->rightSide;
                  tmp3->rightSide = tmp5;	
                   if(mc_pds_line_num>=2) break;
        				}
    						if(ABPDSStrictAppend2TransList(abpds_mid_table,tmp3))
  							{     					
      						if(!abpds_mid_trans) {abpds_mid_trans_last=abpds_mid_trans=tmp3; }
      						else	{ abpds_mid_trans_last->next=tmp3; abpds_mid_trans_last =tmp3; }
    							if(!elm_flag) 
    							{ 
    								if(mc_CTPL)
    								{
    									if(tmp3->bdd==abpds_trans->bdd) RemoveHeadTable(abpds_mid_table,abpds_trans);
    								}
    								else 	RemoveHeadTable(abpds_mid_table,abpds_trans);     
    							} 							
    						}    					
      				}
      			}
      		}
      	}
        list=list->next;
    	}
  	}
  
  	while((base=afa.stack))             //waiting_trans is a stack, LIFO
  	{
  		afa.stack=base->fwdStack;
  		base->fwdStack=base->bwdStack=NULL;
  		if(afa.stack) afa.stack->bwdStack=NULL;
  			
  		/*printf("\nStack rule\n");
  		
  		AFATransition afatrans=afa.stack;
  		for(; afatrans; afatrans=afatrans->fwdStack)
  		{
  			AFAPrintTrans(afatrans);
  		}
  		printf("\nStack rule                end!!!!!!!!!!!!!!\n");*/
  		if (!(base->boolflag))
   		{
 				head=ABPDS_head_find(abpds->transList, base->fromState,base->inputStack);
				if(head) rule =head->rules;
    		else rule=NULL;
    		for( ;rule; rule = rule->next)
    		{
    			abpds_trans =rule->rule;
    			if (abpds_trans->rightSide->next == NULL) // deal with <p,r>-><p1,w1>
    			{
    				if(isdebug)
    				{
    					printf("The transition type is : deal with <p,r>-><p1,w1>\n");
    					ABPDSPrintTransition(abpds_trans);
    				}
    				if (mc_CTPL) {		if(! (bdd =Rel2BDD(abpds_trans, base))) continue;}
						if (abpds_trans->rightSide->stack2 == 0 )      //deal with  case <p,r>-><p1,r1>
						{
							AFARightSideofTransition tmp2, tmp3, tmp4 = NULL;
							for(tmp2 = base->rightSide; tmp2; tmp2 = tmp2->next )
							{
								tmp3 = AFAInitRightSideofTrans(tmp2->state,tmp2->index,NULL,tmp4);
								if(tmp2->Binding&&mc_CTPL) Cudd_Ref(tmp3->Binding =tmp2->Binding);
								tmp4 = tmp3;
							}
							AFATransition added_trans = AFACreateTrans(abpds_trans->fromState,bdd, abpds_trans->fromStack, level, tmp4);
							AFAAppend2TransList(preStarTable, added_trans);														
						}
						else              //deal with  case <p,r>-><p1,r1r2>
						{
							if(isdebug)
  						{
  							printf("The transition type is : deal with <p,r>-><p1,r1r2>\n");
  							ABPDSPrintTransition(abpds_trans);
  						}
  				
							ABPDSTransition tmp1 = ABPDSCreateTransition(abpds_trans->fromState,abpds_trans->rel,abpds_trans->fromStack,NULL);
							tmp1->bdd =bdd;
							tmp1->var=abpds_trans->var;
							AFARightSideofTransition tmp2 = NULL;
							ABPDSRightSideofTransition tmp3 = NULL; 
							for (tmp2 = base->rightSide; tmp2 ; tmp2 = tmp2->next)
							{
								if (tmp2->state == afa.finalState) continue;
								tmp3 = ABPDSInitRightSideofTransition(tmp2->state,tmp2->index,abpds_trans->rightSide->stack2,0);
								if(tmp2->Binding&&mc_CTPL) Cudd_Ref(tmp3->aux.Binding =tmp2->Binding);
								tmp3->next = tmp1->rightSide;
								tmp1->rightSide =tmp3;
							}
							if (tmp1->rightSide)
							{
								AFATransHead AFAHead=NULL;
  							if(ABPDSStrictAppend2TransList(abpds_mid_table, tmp1))
  							{
    							if(!abpds_mid_trans) { abpds_mid_trans_last=abpds_mid_trans=tmp1; }
    							else	{ abpds_mid_trans_last->next=tmp1; abpds_mid_trans_last =tmp1; }

  								for(tmp3=tmp1->rightSide; tmp3; tmp3=tmp3->next)
  								{		
    								AFATransition tmp4 = NULL;
    								if(tmp3->index == level)
    								{
    									AFAHead=afatrans_head_table_find(preStarTable, tmp3->state,tmp3->stack1);
											if(AFAHead) tmp4=AFAHead->rules;
											while(tmp4)
											{
												if(!tmp4->fwdStack &&! tmp4->bwdStack)
												{
													tmp4->fwdStack=afa.stack;
													if(afa.stack) afa.stack->bwdStack=tmp4;
													tmp4->boolflag=1;
													afa.stack=tmp4;
												}
												tmp4=tmp4->next;
											}
											tmp4 = NULL;
      								if (ABPDSCheckStateAcceptiness(accepting, tmp3->state))
      								{
      									AFAHead=afatrans_head_table_find(baseTable, tmp3->state,tmp3->stack1);
  											if(AFAHead) tmp4=AFAHead->rules;
  											while(tmp4)
  											{
  												if(!tmp4->fwdStack &&! tmp4->bwdStack)
  												{
  													tmp4->fwdStack=afa.stack;
  													if(afa.stack)  afa.stack->bwdStack=tmp4;
  													afa.stack=tmp4;
  													tmp4->boolflag=1;
  												}
  												tmp4=tmp4->next;
  											}      									
      								}
      							}
      							else
      							{
      								tmp4 = NULL;
    									AFAHead=afatrans_head_table_find(baseTable, tmp3->state,tmp3->stack1);
											if(AFAHead) tmp4=AFAHead->rules;
											while(tmp4)
											{
												if(!tmp4->fwdStack &&! tmp4->bwdStack)
												{
													tmp4->fwdStack=afa.stack;
													if(afa.stack)  afa.stack->bwdStack=tmp4;
													afa.stack=tmp4;
													tmp4->boolflag=1;
												}
												tmp4=tmp4->next;
											}  								
      							}    
      						}
      					}  									
							}
							else
							{
								ABPDSDestoryTransition(tmp1);
								AFARightSideofTransition tmp4 =AFAInitRightSideofTrans(afa.finalState,0,NULL,NULL);
								AFATransition added_trans = AFACreateTrans(abpds_trans->fromState,bdd, abpds_trans->fromStack, level, tmp4);
								AFAAppend2TransList(preStarTable,added_trans);    							
							}
						}
    			}
    			else         // deal with <p,r>->{<p1,w1>,...,<pn,wn>}
    			{
    				if(isdebug)
    				{
    					printf("The transition type is : deal with <p,r>->{<p1,w1>,...,<pn,wn>}\n");
    					ABPDSPrintTransition(abpds_trans);
    				}
    				ABPDSRightSideofTransition tmp2;
    				for (tmp2 = abpds_trans->rightSide ; tmp2; tmp2 = tmp2->next )
    				{
    					if (tmp2->state == base->fromState && tmp2->stack1 == base->inputStack) 
    					{
								if(mc_CTPL)
  							{
      						if(abpds_trans->rel==TOP)
      						{
      							DdNode* tmp1 = build_var_bdd(abpds_trans->var, tmp2->aux.rel);
      							Cudd_Ref(bdd=Cudd_bddAnd(CTLBDDMgr,base->Binding,tmp1));	
      							Cudd_RecursiveDeref(CTLBDDMgr,tmp1);
      							if(bdd==CTLEmptyset) { Cudd_RecursiveDeref(CTLBDDMgr,bdd); continue; } 
      							tmp1=varExistAbstract(bdd,abpds_trans->var);
      							Cudd_RecursiveDeref(CTLBDDMgr,bdd);
      							bdd =tmp1;
      						}
      						if(abpds_trans->rel==EQUAL || abpds_trans->rel==CONJ) Cudd_Ref(bdd =base->Binding);
      						if(abpds_trans->rel==VAREQU || abpds_trans->rel==VARNEQU)
      						{
              			DdNode* tmp1 = build_var_bdd( tmp2->aux.rel, abpds_trans->fromStack);		
              			if(abpds_trans->rel==VARNEQU) tmp1=Cudd_Not(tmp1);
              			Cudd_Ref(bdd=Cudd_bddAnd(CTLBDDMgr,base->Binding,tmp1));		
              			Cudd_RecursiveDeref(CTLBDDMgr,tmp1); 
              		}     										
      						if(bdd==CTLEmptyset) { Cudd_RecursiveDeref(CTLBDDMgr,bdd); continue; }   
      					}     						 
    						break;      						
    					}
    				}
    				if (tmp2)
    				{    				
    					ABPDSTransition tmp3 =ABPDSCreateTransition(abpds_trans->fromState, abpds_trans->rel,abpds_trans->fromStack,NULL);
  						tmp3->bdd =bdd;
  						tmp3->var=abpds_trans->var;
        			ABPDSRightSideofTransition tmp4,tmp5;
        			for (tmp4 = abpds_trans->rightSide; tmp4 != tmp2; tmp4 = tmp4->next)
        			{
        				tmp5 = ABPDSInitRightSideofTransition(tmp4->state,0,tmp4->stack1,0);
                tmp5->aux.rel =tmp4->aux.rel;
                tmp5->next = tmp3->rightSide;
                tmp3->rightSide = tmp5;		
        			}
        			AFARightSideofTransition tmp6;
        			ABPDSTransLabel tmp7;
        			for (tmp6 = base->rightSide ;tmp6; tmp6 = tmp6->next)
        			{
        				if (tmp6->state==afa.finalState) continue;
      					tmp7 = CreateABPDSTransLabel( tmp6->state,NULL,tmp6->index);
    						if(tmp6->Binding&& mc_CTPL) Cudd_Ref(tmp7->Binding =tmp6->Binding);
    						tmp7->next = tmp3->label;
    						tmp3->label = tmp7;
      				}      						      			
      				for (tmp2 = tmp2->next; tmp2; tmp2 = tmp2->next)
      				{
        				tmp5 =ABPDSInitRightSideofTransition(tmp2->state,0,tmp2->stack1,0);
                tmp5->aux.rel = tmp2->aux.rel;
                tmp5->next = tmp3->rightSide;
                tmp3->rightSide = tmp5;	
      				}
      				if(ABPDSStrictAppend2TransList(abpds_mid_table, tmp3))
							{     					
    						if(!abpds_mid_trans) { abpds_mid_trans_last=abpds_mid_trans=tmp3; }
    						else	{ abpds_mid_trans_last->next=tmp3; abpds_mid_trans_last =tmp3; }
    					}
    				}
    			}
    		}
    		if(mc_CTPL)
    		{
 					head=ABPDS_head_find(abpds->transList, base->fromState,0);
					if(head) rule =head->rules;
    			else rule=NULL;
    			for( ;rule; rule = rule->next)
    			{
    				abpds_trans =rule->rule;    				    
						if((bdd=Rel2BDD(abpds_trans,base)))
						{
							AFARightSideofTransition tmp2;
							tmp2 = AFAInitRightSideofTrans(base->fromState,base->index,NULL, NULL);
							Cudd_Ref(tmp2->Binding =base->Binding);
							AFATransition added_trans = AFACreateTrans(abpds_trans->fromState, bdd, abpds_trans->fromStack, level, tmp2);
						  AFAAppend2TransList(preStarTable,added_trans);   								
						}													
					}   
				}  		
    	}
			head=ABPDS_head_find(abpds_mid_table, base->fromState,base->inputStack);
			if(head) rule =head->rules;
			else rule=NULL;
			
			for( ;rule; rule = rule->next)
			{
				abpds_trans =rule->rule;      
				if(!abpds_trans) 	continue;
 				if (abpds_trans->rightSide->index==0 && base->boolflag) continue; // AFA transition with boolflag =1 comes 
  																																				// from p,r->p,r1 r2, so the index should great than 0.
  			  			
  			if (abpds_trans->rightSide->next == NULL) // deal with <p,r>-><p1,w1>
  			{
  				if(abpds_trans->rightSide->index==0||\
  					(abpds_trans->rightSide->index==base->index&&abpds_trans->rightSide->aux.Binding == base->Binding)||\
  					  (abpds_trans->rightSide->index==base->index+1&&\
  					   abpds_trans->rightSide->aux.Binding == base->Binding&&\
  					   ABPDSCheckStateAcceptiness(accepting,base->fromState)))
  				{
						 //deal with  case <p,r>-><p1,r1>
						if(mc_CTPL) {if(! (bdd =Rel2BDD(abpds_trans,base))) continue;}
						AFARightSideofTransition tmp2, tmp3, tmp4 = NULL;
						AFARightSideofTransition tmp5 = NULL;
						ABPDSTransLabel tmp6=NULL;	
						for(tmp6 =abpds_trans->label; tmp6; tmp6 = tmp6->next)   // union of the label states and the right-hand side of the AFA transition 
						{
							if (tmp6->state==afa.finalState) continue;
							tmp3 = AFAInitRightSideofTrans(tmp6->state,tmp6->index,NULL,tmp4);
							if(tmp6->Binding&&mc_CTPL) Cudd_Ref(tmp3->Binding = tmp6->Binding);
							tmp4 = tmp3;
						}			
						int elm_flag=0;		
						for(tmp2 = base->rightSide; tmp2; tmp2 = tmp2->next )
						{
							if (tmp2->state==afa.finalState) continue;
							for (tmp5 = tmp4; tmp5; tmp5= tmp5->next )
							{
								if (tmp5->state == tmp2->state && tmp5->Binding == tmp2->Binding) 
      					{
										if (tmp5->index>=tmp2->index)	break;
										else { tmp5->index=tmp2->index;	break; }    
      					}
							}
							if(!tmp5)
							{
								tmp3 = AFAInitRightSideofTrans(tmp2->state,tmp2->index,NULL,tmp4);
								if(tmp2->Binding&& mc_CTPL) Cudd_Ref(tmp3->Binding = tmp2->Binding);
								tmp4 = tmp3;
								elm_flag++;
							} 
						}
												
						if (!tmp4)
						{
							tmp4 = AFAInitRightSideofTrans(afa.finalState,0,NULL,NULL);
						}
						AFATransition added_trans = AFACreateTrans(abpds_trans->fromState, bdd, abpds_trans->fromStack, level, tmp4);
						AFAAppend2TransList(preStarTable, added_trans);	
						if(!elm_flag) 
						{ 
							if(mc_CTPL)
							{
								if(bdd==abpds_trans->bdd) RemoveHeadTable(abpds_mid_table,abpds_trans);
							}
							else 	RemoveHeadTable(abpds_mid_table,abpds_trans);     
						}		
					}
  			}
  			else         // deal with <p,r>->{<p1,w1>,...,<pn,wn>}
  			{
  				ABPDSRightSideofTransition tmp2;
  				for (tmp2 = abpds_trans->rightSide ; tmp2; tmp2 = tmp2->next ) 
  				{
  					if (tmp2->state == base->fromState && tmp2->stack1 == base->inputStack && \
  						  (tmp2->index==0 || (tmp2->aux.Binding == base->Binding &&tmp2->index==base->index)||\
  						  (tmp2->aux.Binding == base->Binding && \
  						  tmp2->index==base->index+1 &&ABPDSCheckStateAcceptiness(accepting,base->fromState) )) )  //?????????????????????
  					{
  						if(mc_CTPL)
  						{	
    						if(abpds_trans->rel==TOP)
    						{
    							DdNode* tmp1 = build_var_bdd(abpds_trans->var, tmp2->aux.rel);
    							Cudd_Ref(bdd=Cudd_bddAnd(CTLBDDMgr,base->Binding,tmp1));	
    							Cudd_RecursiveDeref(CTLBDDMgr,tmp1);
    							if(bdd==CTLEmptyset) { Cudd_RecursiveDeref(CTLBDDMgr,bdd); continue; } 
    							tmp1=varExistAbstract(bdd,abpds_trans->var);
    							Cudd_RecursiveDeref(CTLBDDMgr,bdd);
    							Cudd_Ref(bdd=Cudd_bddAnd(CTLBDDMgr,abpds_trans->bdd,tmp1));	
    							Cudd_RecursiveDeref(CTLBDDMgr,tmp1);
    						}
    						if(abpds_trans->rel==EQUAL) Cudd_Ref(bdd = abpds_trans->bdd);
    						if(abpds_trans->rel==CONJ) Cudd_Ref(bdd=Cudd_bddAnd(CTLBDDMgr,base->Binding, abpds_trans->bdd));	    							
    						
    						if(abpds_trans->rel==VAREQU || abpds_trans->rel==VARNEQU)
    						{
            			DdNode* tmp1 = build_var_bdd( tmp2->aux.rel, abpds_trans->fromStack);		
            			if(abpds_trans->rel==VARNEQU) tmp1=Cudd_Not(tmp1);
            			Cudd_Ref(bdd=Cudd_bddAnd(CTLBDDMgr,base->Binding,tmp1));		
            			Cudd_RecursiveDeref(CTLBDDMgr,tmp1); 
            			if(bdd!=CTLEmptyset)
            			{
    								Cudd_Ref(tmp1 = Cudd_bddAnd(CTLBDDMgr,abpds_trans->bdd, bdd));	
    								Cudd_RecursiveDeref(CTLBDDMgr,bdd);     
    								bdd=tmp1;       				
            			}
            		}     										
    						if(bdd==CTLEmptyset) { Cudd_RecursiveDeref(CTLBDDMgr,bdd); continue; }   
    					}     						 
  						break;      						
  					}
  				}
  				if (tmp2)
  				{    				
  					ABPDSTransition tmp3 = ABPDSCreateTransition(abpds_trans->fromState,abpds_trans->rel,abpds_trans->fromStack,NULL);
						tmp3->bdd =bdd;
						tmp3->var=abpds_trans->var;
      			ABPDSRightSideofTransition tmp4,tmp5;
      			for (tmp4= abpds_trans->rightSide; tmp4 != tmp2; tmp4 = tmp4->next)
      			{
      				tmp5 = ABPDSInitRightSideofTransition(tmp4->state,tmp4->index,tmp4->stack1,0);
              if(tmp4->index&&tmp4->aux.Binding) Cudd_Ref(tmp5->aux.Binding =tmp4->aux.Binding) ;
              else tmp5->aux.rel =tmp4->aux.rel;
              tmp5->next = tmp3->rightSide;
              tmp3->rightSide = tmp5;		
               if(mc_pds_line_num>=2) break;
      			}
      			AFARightSideofTransition tmp6;
      			ABPDSTransLabel tmp7,tmp9;
      			for (tmp9 = abpds_trans->label; tmp9; tmp9 = tmp9->next)
      			{
      				if (tmp9->state==afa.finalState) continue;
     					tmp7 = CreateABPDSTransLabel( tmp9->state,NULL,tmp9->index);
  						if(tmp9->Binding&& mc_CTPL) Cudd_Ref(tmp7->Binding =tmp9->Binding);
  						tmp7->next = tmp3->label;
  						tmp3->label = tmp7;     				
      			}	
      			int elm_flag=0;
      			for (tmp6 = base->rightSide ;tmp6; tmp6 = tmp6->next)
      			{
      				if (tmp6->state==afa.finalState) continue;
      				ABPDSTransLabel tmp8 = tmp3->label;
      				for(;tmp8;tmp8 =tmp8->next)
      				{
      					if(tmp8->state ==tmp6->state &&	tmp8->Binding ==tmp6->Binding)  
      					{
										if( tmp8->index>=tmp6->index)	break;
										else	{ tmp8->index=tmp6->index; break;	}   
      					}      						
      				}
      				if (!tmp8)
      				{
      					tmp7 = CreateABPDSTransLabel( tmp6->state, NULL,tmp6->index);
      					if(tmp6->Binding&&mc_CTPL) Cudd_Ref(tmp7->Binding =tmp6->Binding);
    						tmp7->next = tmp3->label;
    						tmp3->label = tmp7;
    						elm_flag++;
    					}      				
      			}
      					      			
    				for (tmp2 = tmp2->next; tmp2; tmp2 = tmp2->next)
    				{
      				tmp5 = ABPDSInitRightSideofTransition(tmp2->state,tmp2->index,tmp2->stack1,0);
              if(tmp2->index&&tmp2->aux.Binding) Cudd_Ref(tmp5->aux.Binding =tmp2->aux.Binding) ;
              else tmp5->aux.rel =tmp2->aux.rel;                
              tmp5->next = tmp3->rightSide;
              tmp3->rightSide = tmp5;	
              if(mc_pds_line_num>=2) break;
    				}
    				if(ABPDSStrictAppend2TransList(abpds_mid_table,tmp3))
						{     					
  						if(!abpds_mid_trans) {	abpds_mid_trans_last=abpds_mid_trans=tmp3; }
  						else	{ abpds_mid_trans_last->next=tmp3; abpds_mid_trans_last =tmp3; }
   						if(!elm_flag) 
  						{ 
  							if(mc_CTPL)
  							{
  								if(tmp3->bdd==abpds_trans->bdd) RemoveHeadTable(abpds_mid_table,abpds_trans);
  							}
  							else 	RemoveHeadTable(abpds_mid_table,abpds_trans);     
  						}		 							
  					}
  				}
  			}
  		}
  		base->boolflag = 0;
  	}
  	if(isdebug) printf("$$$$$$$$$$$End of while waiting_num, level %d.$$$$$$$$$\n\n", level);
	}     	
  
  ABPDS_head_table_delete(abpds_mid_table);	
  abpds_mid_table=NULL;
  ABPDSDestoryTransList(abpds_mid_trans);
	abpds_mid_trans=NULL;
	preStarChecking =0;   // set flag used to fix the state space exploding.
	if(isdebug) printf("********************Finished the %d level.************\n\n", level);
}

void substitution(AFATransTable table, int level)
{
	int afa_trans_num=0;
  int i,flag;	
	if (isdebug)
	{
		AFATransition base;
		printf("********************Begin printing the base transitions.*111111111\n\n");
		for(i=0 ;i<table->size; i++)
		{
			AFATransHead list =table->data[i];
			while(list)
			{
				for(base=list->rules; base; base=base->next)
				{
					AFAPrintTrans(base);
					afa_trans_num++;
				}
				list=list->next;
			} 
		}
		printf("********************End of print the base transitions.11111111111111***********\n\n");
	  afa_trans_num_max= afa_trans_num>afa_trans_num_max? afa_trans_num: afa_trans_num_max; 
	}		
	
	AFATransition tmp2 = NULL;
	for(i=0; i<table->size; i++)
	{
		AFATransHead list=table->data[i];
		AFATransHead lastlist=NULL; 
		while(list)
		{
			AFATransition tmp1=list->rules;
    	while(tmp1)
    	{
    		if(tmp1->fwdStack || tmp1->bwdStack) 
    		{
    			printf("\n.......................\n");
    			AFAPrintTrans(tmp1);
    		}
    		AFARightSideofTransition tmp3 = tmp1->rightSide;
    		AFARightSideofTransition tmp4 = NULL;
    		flag = 0;
    		while(tmp3)
    		{
    			isomorphicChecking =1;
    			if(AFACheckRightSideInclusion(tmp3->next, tmp3->state, tmp3->Binding, 0))
    			{
    				if(tmp3 == tmp1->rightSide)
    				{
    					tmp1->rightSide = tmp3->next;
    				//	AFADeleteRightSide(tmp3);
    					tmp3 = tmp1->rightSide;
    				}
    				else
    				{
    					tmp4->next = tmp3->next;
    				//	AFADeleteRightSide(tmp3); 
    					tmp3 =tmp4->next;
    				}
    				flag = 1;
    			}
    			else
    			{
    				if(tmp3->index==level-1) 
    				{	
    					flag = 1;
    					tmp3->index = level;	
    				}
    				tmp4 =tmp3;
    				tmp3=tmp3->next;		
    			}
    			isomorphicChecking =0;
    		}
    		if(flag)                 // if flag=1, there are some state in right-hand side such that the subscript is level-1.
    		{
    			if(list->rules == tmp1)       // the first transition of the transList need be modified.
    			{	
    				tmp2= tmp1->next;
    				if(AFACheckTransInclusion(list->rules, tmp1))  // if the modified transition exists in the transList, we will delelte it, otherwise not. 
    				{
    					AFADeleteTrans(tmp1);
    					list->rules = tmp1 = tmp2;
    				}
    				else
    				{
    					tmp2=tmp1;
    					tmp1 = tmp1->next;
    				}
    			}
    			else  
    			{
    				//tmp2->next = tmp1->next;
    				if(AFACheckTransInclusion(list->rules, tmp1))
    				{
    					tmp2->next=tmp1->next;
    					AFADeleteTrans(tmp1);
    					tmp1 = tmp2->next;
    				}
    				else
    				{
    					tmp2 = tmp1;
    					tmp1= tmp1->next;
    				}
    			}
    		}
    		else
    		{
    			tmp2 = tmp1;
    			tmp1 = tmp1->next;
    		}
    	}		
  		if(!list->rules) 
  		{
  			if(lastlist) 
  			{
  				lastlist->next=list->next;
  				afatrans_head_delete(list);
  				list=lastlist->next;
  			}
  			else
  			{
  				table->data[i]=list->next;
  				afatrans_head_delete(list);
  				list=table->data[i];  				
  			}
  		}    	
  		else	
  		{
  			lastlist=list;
				list=list->next;
			}
		}
	}
	if (isdebug)
	{
		AFATransition base;
		printf("********************Begin printing the base transitions.2222222222222\n\n");
		for(i=0 ;i<table->size; i++)
		{
			AFATransHead list =table->data[i];
			while(list)
			{
				//printf("state: %s, stack: %s\n", name_cstring(list->state), name_cstring(list->stack));
				for(base=list->rules; base; base=base->next)
				{		
					AFAPrintTrans(base);
				}
				list=list->next;
			} 
		}
		printf("********************End of print the base transitions.222222222222***********\n\n");
	}		
}

void compute_fixpoint(AlternatingBuchiPushdownSystem* abpds)
{  				
/*	int count=0;
	ABPDSTransition printtrans=NULL;
	for(printtrans=abpds->trans; printtrans;printtrans=printtrans->next)
	{
		ABPDSPrintTransition(printtrans);
		count++;
	}
	
	printf("This is the accepting state list!\n");

	printAcceptingState(abpds->acceptingStateList);
	

  ABPDSStackList stack;
  for(stack=abpds->stackList; stack;stack=stack->next)
  {
  	printf("%s, ", name_cstring(stack->stack));
  }
	exit(1);*/
	printf("This is the accepting state list!\n");
	printAcceptingState(abpds->acceptingStateList);
	
	int recursion_level =0;
	afa = AFAInit();


	recursion_level++;
	printf("Level:      %d\n", recursion_level);
	compute_abpds_prestar(abpds, afa.transList, recursion_level);

	do
	{
		afatrans_head_table_delete(afa.transList);
		afa.transList = preStarTable;

		recursion_level++;
		printf("Level:         %d\n", recursion_level);
    
    afa.stack=NULL;
		compute_abpds_prestar(abpds, afa.transList, recursion_level);

		substitution(preStarTable, recursion_level);
		//printf("Level:         %d%d%d finished!\n",recursion_level,recursion_level,recursion_level);
					
	} while (!AFA_head_table_compare(preStarTable, afa.transList));
//  printf("The recusive times: %d\n\n", recursion_level);
  
	afatrans_head_table_delete(afa.transList);
	afa.transList = preStarTable;
	int i;
	if (isdebug)
	{
		printf("\n********************Begin printing the transitions.***********\n\n");
		AFATransition base  =NULL;
		
		for(i=0 ;i<preStarTable->size; i++)
		{
			AFATransHead list =preStarTable->data[i];
			while(list)
			{
				for(base=list->rules; base; base=base->next)
				{
					AFAPrintTrans(base);
				}
				list=list->next;
			} 
		}
		printf("\n*******************End of print the transitions.***********\n\n");
	}
	AFATransition base  =NULL;
	afa.stack =NULL;
	for(i=0 ;i<preStarTable->size; i++)
	{
		AFATransHead list =preStarTable->data[i];
		while(list)
		{
			for(base=list->rules; base; base=base->next)
			{
					base->fwdStack=afa.stack;
					afa.stack=base;
			}
			list=list->next;
		} 
	}  
	AFARightSideofTransition rightSide = AFAInitRightSideofTrans(afa.finalState,0,NULL,NULL);

	base = NULL;
	ABPDSStackList stacks;
	for (stacks=abpds->stackList ; stacks; stacks = stacks->next)
	{
		base = AFACreateTrans(afa.finalState, NULL, stacks->stack, 0, rightSide);
    base->fwdStack =afa.stack;
    afa.stack=base;
	}	
}
