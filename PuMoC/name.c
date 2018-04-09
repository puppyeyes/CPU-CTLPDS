/* =========================================================================

   Definition der Datenstrukturen und Funktionen
   zur Verwaltung von Namen

   ====================================================================== */

#include <stdio.h>

#include "common.h"
#include "name.h"
#include "variables.h"

char **name_globals_array;
long name_globals_alloc;
long name_globals_used;
long name_current_size;

Name sharp;

ParamList ctlQuantVarList = NULL;
int ctlQuantVarNum =0;
ValList abpdsValList = NULL;
int abpdsValNum =0;

NameSet *name_globals_hash;

/* ----------------------------------------------------------------------- */
/* ------------------------------  Name  --------------------------------- */
/* ----------------------------------------------------------------------- */

int name_create(char *new_name)
{
	int cmp = 1, ret;
	NameListElement last_entry;
	NameListElement next_entry;
	NameListElement new_entry;
	unsigned long hash = 1;
	char *c;

	for (c = new_name; *c; c++) hash = (hash + *c) << 1;
	hash = hash % 200003;

	/* Einfuegeposition suchen */

	last_entry = NULL;
	next_entry = name_globals_hash[hash % name_current_size];

	while (next_entry)
	{
		cmp = strcmp(new_name, name_globals_array[next_entry->name]);
		if (cmp >= 0) break;
		last_entry = next_entry;
		next_entry = next_entry->next;
	}

	/* new_name bereits vorhanden => diesen zurueckliefern */
	if (!cmp) return next_entry->name;

	/* new_name noch nicht vorhanden */
	ret = name_globals_used;

	if (ret == name_globals_alloc)
	{
		/* neuen Speicher reservieren falls noetig */

		name_globals_array = common_realloc(name_globals_array,
		    (name_globals_alloc += NAME_GLOBALS_DELTA) * sizeof(char*),
		    NAME_GLOBALS_DELTA * sizeof(char*));
	}

	name_globals_array[ret] = common_strdup(new_name);
	name_globals_used++;

	/* Rehashing? */

	if (ret >= name_current_size * 3/4)
	{
		NameListElement l1 = NULL, l2 = NULL;
		long newsizemask = name_current_size * 2 - 1;
		int i;

		name_globals_hash = common_realloc(name_globals_hash,
		    name_current_size * 2 * sizeof(void*),
		    name_current_size * sizeof(void*));

		for (i = 0; i < name_current_size; i++)
		{
		    l1 = l2 = NULL;
		    last_entry = name_globals_hash[i];
		    while (last_entry)
		    {
			next_entry = last_entry->next;
			if ((last_entry->hash & newsizemask)
					== (unsigned long)i)
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

		    name_globals_hash[i] = last_entry;

		    last_entry = NULL;
		    while (l2)
		    {
			next_entry = l2->next;
			l2->next = last_entry;
			last_entry = l2;
			l2 = next_entry;
		    }

		    name_globals_hash[i + name_current_size] = last_entry;
		}

		name_current_size *= 2;

		last_entry = NULL;
		next_entry = name_globals_hash[hash % name_current_size];

		/* Position neu bestimmen */
		while (next_entry)
		{
		    cmp = strcmp(new_name,name_globals_array[next_entry->name]);
		    if (cmp >= 0) break;
		    last_entry = next_entry;
		    next_entry = next_entry->next;
		}
	}

	/* => Neuen Eintrag in Hash-Liste erzeugen */
 
	new_entry = common_malloc(sizeof(struct NameListElementStruct));
	new_entry->name = ret;
	new_entry->hash = hash;

	hash = hash % name_current_size;

	if (!last_entry)
	{
		/* ganz vorne in Liste einfuegen */
		new_entry->next = name_globals_hash[hash];
		name_globals_hash[hash] = new_entry;
	}
	else
	{
		/* sonst mittlere oder Endposition */
		new_entry->next  = next_entry;
		last_entry->next = new_entry;
	}

	return ret;
}


/* ----------------------------------------------------------------------- */
/* ----------------------------  NameSet  -------------------------------- */
/* ----------------------------------------------------------------------- */

/* NameSets werden absteigend sortiert, hoehere Namen zuerst! */

Name name_set_insert (NameSet *name_set, int new_name)
{
	NameListElement last_entry;
	NameListElement next_entry;

	int cmp = 1;

	/* Einfuegeposition suchen */

	last_entry = NULL;
	next_entry = *name_set;

	while (next_entry)
	{
		cmp = name_compare(new_name,next_entry->name);
		if (cmp <= 0) break;
		last_entry = next_entry;
		next_entry = next_entry->next;
	}

	if (cmp)
	{
		/* wenn noch nicht vorhanden, neuen Eintrag erzeugen */
		NameListElement  new_entry;
		new_entry = common_malloc(sizeof(struct NameListElementStruct));
		new_entry->name = new_name;

		if( last_entry == NULL )
		{
			/* ganz vorne in Liste einfuegen */
			new_entry->next = *name_set;
			*name_set       = new_entry;
		}
		else
		{
			/* sonst mittlere oder Endposition */
			new_entry->next  = next_entry;
			last_entry->next = new_entry;
		}
	}

	return new_name;
}


Name name_set_find (NameSet set, int name)
{
	int cmp = 1;

	while (set)
	{
		cmp = name_compare(name, set->name);
		if (cmp <= 0) break;
		set = set->next;
	}
  
	return !cmp;
}


void name_set_delete (NameSet *set)
{
	NameListElement entry;

	while (*set)
	{
		entry = *set;
		*set  = (*set)->next;
		common_free(entry,sizeof(struct NameListElementStruct));
	}
}


/* ---------------------------------------------------------------------- */
/* ----------------------------  NameStack  ----------------------------- */
/* ---------------------------------------------------------------------- */

void name_stack_push (NameStack *stack, int name, DdNode *bdd)
{
	NameStackElement new_element;

	new_element = common_malloc(sizeof(struct NameStackElementStruct));
	new_element->name = name;
	new_element->bdd = bdd;
	new_element->next = *stack;
	*stack = new_element;
}


void name_stack_delete (NameStack *stack)
{
	NameStack tmp;

	while (*stack)
	{
		tmp = *stack;
		*stack = (*stack)->next;
		common_free(tmp,sizeof(struct NameStackElementStruct));
	}
}


/* ----------------------------------------------------------------------- */
/* -----------------------------  NameTree  ------------------------------ */
/* ----------------------------------------------------------------------- */

#define NAME_TREE_ALLOC 256

NameNode name_tree_mknode (Name name, Name refname, block_t block)
{
	NameNode new = malloc(sizeof(struct NameNodeStruct));
	new->name = name;
	new->refname = refname;
	new->block = block;
	return new;
}

void name_tree_insert (NameTree *treeptr, NameNode node)
{
	int uc, oc, low, up, mid;
	NameTree tree;

	if (!*treeptr)
	{
		tree = *treeptr = common_malloc(sizeof(struct NameTreeStruct));
		tree->alloc = NAME_TREE_ALLOC;
		tree->used = 1;
		tree->data = malloc(tree->alloc*sizeof(void*));
		tree->data[0] = node;
		return;
	}

	tree = *treeptr;
	if (tree->alloc <= tree->used)
	{
		tree->alloc += NAME_TREE_ALLOC;
		tree->data = realloc(tree->data,tree->alloc*sizeof(void*));
	}

	uc = 0; oc = tree->used-1;

	while (uc <= oc)
	{
		low = tree->data[uc]->name;
		up = tree->data[oc]->name;

		if (node->name > up) { uc = oc+1; break; }
		if (node->name < low) { oc = uc-1; break; }
		mid = (uc==oc)? uc : uc + (node->name-low)*(oc-uc)/(up-low);

		if (tree->data[mid]->name < node->name) uc = mid+1;
		else if (tree->data[mid]->name > node->name) oc = mid-1;
		else if (tree->data[mid]->name == node->name) 
		    common_error("name %s used twice",name_cstring(node->name));
	}

	for (oc = tree->used; oc > uc; oc--)
		tree->data[oc] = tree->data[oc-1];

	tree->data[uc] = node;
	tree->used++;
}

NameNode name_tree_find (NameTree tree, Name name)
{
	int uc, oc;

	if (!tree) return NULL;

	uc = 0; oc = tree->used-1;

	while (uc <= oc)
	{
		int low = tree->data[uc]->name;
		int up = tree->data[oc]->name;
		int mid;
	       
		if (name < low || name > up) return NULL;

		mid = (uc==oc)? uc : uc + (name-low)*(oc-uc)/(up-low);
		if (tree->data[mid]->name < name) uc = mid+1;
		else if (tree->data[mid]->name > name) oc = mid-1;
		else if (tree->data[mid]->name == name) return tree->data[mid];
	}

	return NULL;
}

void name_tree_delete (NameTree *tree)
{
	int i;

	if (!*tree) return;
	for (i = 0; i < (*tree)->used; i++) free((*tree)->data[i]);
	free((*tree)->data);
	free(*tree);
	*tree = NULL;
}

/* ----------------------------------------------------------------------- */
/* ----------------------------  NameGlobal  ----------------------------- */
/* ----------------------------------------------------------------------- */

void name_global_empty()
{
	name_globals_hash = common_malloc(NAME_HASH_MIN * sizeof(NameSet));
	memset(name_globals_hash, 0, NAME_HASH_MIN * sizeof(NameSet));
	name_current_size = NAME_HASH_MIN;

	name_globals_array = NULL;
	name_globals_alloc = 0;
	name_globals_used  = 0;

	/* Name mit nummer 0 ist leer */
	name_create("*");
}


void name_global_delete()
{
	int i;

	/* name_globals_hash loeschen */

	for (i = 0; i< name_current_size; i++)
		name_set_delete(&(name_globals_hash[i]));

	common_free(name_globals_hash, name_current_size * sizeof(NameSet));

	/* name_globals_array loeschen */

	if (!name_globals_array) return;

	for (i = 0; i < name_globals_used; i++)
		common_free(name_globals_array[i],
				strlen(name_globals_array[i])+1);

	common_free(name_globals_array, name_globals_alloc * sizeof(char*));
}

/* ----------------------------------------------------------------------- */
/* ----------------------------  NameBalTree  ---------------------------- */
/* ----------------------------------------------------------------------- */

signed char name_bal_insert(NameBalTree *tree, Name name)
{
	signed char obal, diff, nbal;

	if (!*tree)
	{
		*tree = common_malloc(sizeof(struct NameBalTreeStruct));
		(*tree)->name = name;
		(*tree)->bal = 0;
		(*tree)->left = (*tree)->right = NULL;
		return 1;
	}

	if ((*tree)->name == name) return 0;

	obal = (*tree)->bal;

	if ((*tree)->name > name)
		diff = -name_bal_insert(&((*tree)->left),name);
	else
		diff = name_bal_insert(&((*tree)->right),name);

	nbal = ((*tree)->bal += diff);

	if (nbal == 0) return 0;
	if (nbal == 1 || nbal == -1) return !!diff;

	if ((*tree)->bal == -2)
	{
		NameBalTree a = *tree;
		NameBalTree b = a->left;
		NameBalTree c = b->right;

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
			NameBalTree f = c->left;
			NameBalTree g = c->right;

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
		NameBalTree a = *tree;
		NameBalTree b = a->right;
		NameBalTree c = b->left;

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
			NameBalTree f = c->left;
			NameBalTree g = c->right;

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

int name_bal_find (NameBalTree tree, Name name)
{
	while (tree)
	{
		if (tree->name == name) return 1;
		tree = (tree->name > name)? tree->left : tree->right;
	}
	return 0;
}

void name_bal_print (NameBalTree tree)
{
	if (!tree) { printf("*"); return; } 
	printf("%s",name_cstring(tree->name));
	if (tree->bal < 0) printf("-");
	if (tree->bal > 0) printf("+");
	if (!tree->left && !tree->right) return;
	printf("(");
	name_bal_print(tree->left);
	printf(",");
	name_bal_print(tree->right);
	printf(")");
}

void name_bal_delete (NameBalTree tree)
{
	NameBalTree left = NULL;
	NameBalTree right = NULL;
	if(tree)
	{
		left = tree->left;
		right = tree->right;
		common_free(tree, sizeof(struct NameBalTreeStruct));
		name_bal_delete(left);
		name_bal_delete(right);
	}
}
ParamList createParam(Name param,int type, ParamList paramlist)
{
	ParamList tmp = common_malloc(sizeof(struct ParamListStruct));
	tmp->name =param;
	tmp->next = paramlist;
	tmp->type = type;
	if(type) return tmp;  // type=1 represents this param is constant rather othan variable.
		
	ParamList tmp2 = ctlQuantVarList;
	for(; tmp2; tmp2=tmp2->next)
	{
		if(tmp2->name==param) break;
	}
	if(!tmp2) 
	{
		ParamList tmp1 = common_malloc(sizeof(struct ParamListStruct));
		tmp1->name =param;
		tmp1->type =0;
		tmp1->next = ctlQuantVarList;
		ctlQuantVarList =tmp1;
		ctlQuantVarNum++;               // when the parm is variable, inc the count. 
	}
	
	return tmp;
}
int findVarIndex(ParamList list, Name name)
{
	int r=0;
	for(;list;list=list->next)
	{
		if(list->name==name) return r;            //type: 0=variable, 1=constant
		r++;
	}
	printf("Error in findVarIndex!\n");
	return -1;
}

int findValIndex(ValList list, Name name)
{
	int r=0;
	for(;list;list=list->next)
	{
		if(list->name==name ) return r;         
		r++;
	}
	printf("Error in findValIndex: %d,%s!\n", name,name_cstring(name));
	return -1;
}

int compareParams(ParamList paramList1,ParamList paramList2)
{
	if (!paramList1 && !paramList2 ) return 1;
	if (paramList1 && !paramList2 ) return 0;
	if (!paramList1 && paramList2 ) return 0;	
	if (paramList1->name == paramList2->name && paramList1->type == paramList2->type)
		return compareParams(paramList1->next,paramList2->next);
	return 0;
}
void deleteParams(ParamList paramList)
{
	ParamList tmp = NULL;
	while(paramList)
	{
		tmp = paramList->next;
		common_free(paramList,sizeof(struct ParamListStruct));
		paramList =tmp;
	}
}

ParamList paramsCopy(ParamList paramlist)
{
	ParamList tmp = NULL;
	if(paramlist)
	{
		tmp = createParam(paramlist->name,paramlist->type, paramsCopy(paramlist->next));
	}
	return tmp;
}
ValList createVal(Name value, ValList list)
{
	ValList tmp = common_malloc(sizeof(struct ValueListStruct));
	tmp->name =value;
	tmp->next = list;
	ValList tmp2 = abpdsValList;
	for(; tmp2; tmp2=tmp2->next)
	{
		if(tmp2->name==value) break;
	}
	if(!tmp2) 
	{
		ValList tmp1 = common_malloc(sizeof(struct ValueListStruct));
		tmp1->name =value;
		tmp1->next = abpdsValList;
		abpdsValList =tmp1;
		abpdsValNum++;
	}
	return tmp;
}

int compareVal(ValList paramList1,ValList paramList2)
{
	if (!paramList1 && !paramList2 ) return 1;
	if (paramList1 && !paramList2 ) return 0;
	if (!paramList1 && paramList2 ) return 0;	
	if (paramList1->name == paramList2->name)
	{
		return compareVal(paramList1->next,paramList2->next);
	}
	return 0;
}

void deleteVal(ValList paramList)
{
	ValList tmp = NULL;
	while(paramList)
	{
		tmp = paramList->next;
		common_free(paramList,sizeof(struct ValueListStruct));
		paramList =tmp;
	}
}

int getbddVarIndex(ParamList list, Name var)
{
	int index=0;
	while(list)
	{
		if(list->name==var) break;
		list=list->next;
		index++;
	}
	if(!list)
	{
		printf("The variable %s does not exist in the variables set.\n", name_cstring(var));
		exit(1);	
	}
	return index;
}


DdNode* build_conj_bdd(ParamList list1,ValList list2)
{
	DdNode* bdd, *tmp1, *tmp2;
	Cudd_Ref(bdd = CTLFullset);
	int var_index=0;
	int val_index=0; 
	int i;

	for(;list1; list1=list1->next)
	{
		if(list1->type) 
		{
			if(list1->name!= list2->name) {	Cudd_RecursiveDeref(CTLBDDMgr,bdd);	return NULL; }
			else list2=list2->next;
		}
		else
  	{
  		var_index= findVarIndex(ctlQuantVarList,list1->name);
  		//printf("name: %s\n", name_cstring(list1->name));
  		val_index= findValIndex(abpdsValList,list2->name);
  
  		for(i=step_length-1; i>=0; i--)
  		{
  			tmp1 = bddValVar[var_index*step_length+i];
  			if( !(val_index & 1)) tmp1 = Cudd_Not(tmp1);
  			Cudd_Ref(tmp2 = Cudd_bddAnd(CTLBDDMgr,bdd,tmp1));	
  			Cudd_RecursiveDeref(CTLBDDMgr,bdd);	
  			bdd = tmp2;
  			val_index>>=1;	
  			//Cudd_PrintMinterm(CTLBDDMgr,bdd);		
  		}
  		list2=list2->next;
  	}
	}
	
	return bdd;
}


DdNode* build_disj_bdd(ParamList list1,ValList list2)
{
	DdNode* tmp1, *tmp2, *tmp3, *bdd;
	
	Cudd_Ref(bdd = CTLEmptyset);
	int var_index=0;
	int val_index=0; 
	int i;

	while(list1)
	{
		if(list1->type) 
		{
			if(list1->name == list2->name) { list1=list1->next; list2=list2->next; }
			else { Cudd_RecursiveDeref(CTLBDDMgr,bdd); Cudd_Ref(bdd = CTLFullset);	return bdd; }
		}
		else
  	{
  		var_index= findVarIndex(ctlQuantVarList,list1->name);
  		
  		val_index= findValIndex(abpdsValList,list2->name);
  	  //printf("Var name: %s,index: %d,Value name: %s,index:%d\n", name_cstring(list1->name),var_index, name_cstring(list2->name),val_index );
  		Cudd_Ref(tmp3 = CTLFullset);
  		for(i=step_length-1; i>=0; i--)                    // compute x_i=c_i
  		{
  			tmp1 = bddValVar[var_index*step_length+i];
  			if( !(val_index & 1)) tmp1 = Cudd_Not(tmp1);
  			Cudd_Ref(tmp2 = Cudd_bddAnd(CTLBDDMgr,tmp3,tmp1));	
  			Cudd_RecursiveDeref(CTLBDDMgr,tmp3);	
  			tmp3 = tmp2;
  			val_index>>=1;			
  		}
  		tmp3 =Cudd_Not(tmp3);                 // get bdd of x_i != c_i
  		//printf("Atom:");Cudd_PrintMinterm(CTLBDDMgr,tmp3);	
  		Cudd_Ref(tmp2 = Cudd_bddOr(CTLBDDMgr,bdd,tmp3));	// x_1 !=c_1 || ,...,|| x_n!=c_n 
  		//printf("Disjunction:");Cudd_PrintMinterm(CTLBDDMgr,tmp2);	
  		Cudd_RecursiveDeref(CTLBDDMgr,bdd);	
  		bdd = tmp2;
  		list1=list1->next;		
  		list2=list2->next;
  	}
	}
	return bdd;
}
			
DdNode* build_var_bdd(Name variable, Name value)
{	
	DdNode* tmp1,*tmp2,*bdd;
	Cudd_Ref(bdd = CTLFullset);
	int var_index=findVarIndex(ctlQuantVarList,variable);
	int val_index=findValIndex(abpdsValList,value);
	int i;
	for(i=step_length-1;i>=0;i--)
	{
		tmp1 = bddValVar[var_index*step_length+i];
		if( !(val_index & 1)) tmp1 = Cudd_Not(tmp1);
		Cudd_Ref(tmp2 = Cudd_bddAnd(CTLBDDMgr,bdd,tmp1));	
		Cudd_RecursiveDeref(CTLBDDMgr,bdd);
		bdd=tmp2;
		val_index>>=1;			
	}
	return bdd;
}
			
DdNode* varExistAbstract(DdNode* bdd, Name var)
{
	//printf("Var name: %s\n",name_cstring(var));
	//Cudd_PrintMinterm(CTLBDDMgr,bdd);
	DdNode* cube=NULL;
	DdNode* r=NULL;
	int* cubeArray= malloc(step_length*sizeof(int)); 
	int index =findVarIndex(ctlQuantVarList,var);
	int i;
	for(i=0;i<step_length;i++)	cubeArray[i]=index*step_length+i;
	
	Cudd_Ref(cube = Cudd_IndicesToCube(CTLBDDMgr,cubeArray,step_length));
	free(cubeArray);
	Cudd_Ref(r=Cudd_bddExistAbstract(CTLBDDMgr,bdd,cube)); 	
	//Cudd_PrintMinterm(CTLBDDMgr,r);
	return r;
}			
			
int IsVariable(Name input)
{
	ParamList tmp;
	for(tmp=ctlQuantVarList; tmp; tmp=tmp->next)
	{
		if (tmp->name==input && tmp->type==0)
			return 1;
	}
	return 0;
}					
			
/*
void test_bal ()
{
	NameBalTree tree = NULL;

	for (;;)
	{
		int i;
		scanf("%d",&i);
		if (!i) exit(0);
		name_bal_insert(&tree,i);
		name_bal_print(tree);
		printf("\n");
	}
}
*/

