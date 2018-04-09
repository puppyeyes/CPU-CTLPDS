/* =========================================================================

	Nondeterministic finite automata.

   ====================================================================== */

#ifndef NFA_H
#define NFA_H

typedef struct NondeterministicFiniteAutomata NFA;
typedef struct NFAStateListStruct *StateList;
typedef struct NFAStateListStruct *InputList;

typedef struct NFATransitionListStruct * NFATrans;

typedef struct ComplementationNFAStruct CNFA;
typedef struct ComplementationDFATransStruct *CNFATrans;


/* --------------------------   Datatypes   --------------------------------- */
struct NondeterministicFiniteAutomata
{
	StateList initial_states;
	StateList lastInitState;
	StateList final_states;
	StateList lastFinalState;
	NFATrans trans;
	NFATrans lastTran;
};

struct NFAStateListStruct
{
	int state;
	StateList next;
};

struct NFATransitionListStruct
{
	int from_state;
	int input;
	int to_state;
	int flag;
	NFATrans next;
};

struct ComplementationNFAStruct
{
	StateList initial_states;
	StateList lastInitState;
	StateList final_states;
	StateList lastFinalState;
	CNFATrans trans;
	CNFATrans lastTran;
};

struct ComplementationDFATransStruct
{
	int from_state;
	InputList input;
	int type;                // transiton type 0  input\in\Gamma; transiton type 1 means B(x) =\gamma; transiton type 2 means B(x) !=\gamma;
	StateList rightSide;
 	CNFATrans next;
};


/* --------------------------   Functions   --------------------------------- */



NFA* initNFA();
void print_nfa(NFA*);
NFA* productnfaforPlus(NFA*,NFA*);
NFA* productnfaforCancat(NFA*,NFA*);
NFA* productnfaforStar(NFA*);
NFA* productnfaforNUMBER(int);
NFA* productnfaforDOT();
void deleteNFA(NFA*, int);
void Normalize(NFA*);
StateList copyState(StateList);
void deleteStateList(StateList);
int CheckMemberShip(StateList, int);
int CheckMemberShipWithVar(StateList, int);
void Append2StateList(StateList*, int);
void deleteTransList(NFATrans);

CNFA* ComplementNFA(NFA*);

CNFA* CNFAInit();
void print_cnfa(CNFA*);
void CNFADelete(CNFA*);
CNFATrans CNFACreateTrans(int, InputList, int, StateList );
void  CNFADeleteTrans(CNFATrans);
void  CNFADeleteTransList(CNFATrans);
int CheckExistance(CNFATrans,int, int);
int checkAssumption(CNFATrans, int);
#endif
	