// java.cpp : Defines the entry point for the DLL application.
//


#ifdef _WINDOWS
#include "stdafx.h"
#endif



#include <jni.h>

#include <stdio.h>
#include <stdlib.h>

#include <cudd.h>
#include <cuddInt.h>

#include "jbdd.h"


#define MAX_NODES 8000000
#define MIN_NODES 1000
#define DEFAULT_VARS 64




// see if the __func__ macro is available??
static void dummy_function() {
#ifndef __func__
 #define __func__ "?"
#endif
}

// must do the flush, even with stderr :(
#define IGNORE_CALL	fprintf(stderr,"(this function (%s,%s/%d) is not implemented)\n", \
																 __func__, __FILE__, __LINE__), fflush(stdout)

#define REF_COUNT(n)	(Cudd_Regular((DdNode *) (n))->ref)


// mingw uses the UNIX linkage method, so it will omit "-" even when it running under WINDOWS!
#ifdef __MINGW32__
 #ifdef SUPREMICA
  #define JNI_PREFIX(name) _Java_org_supremica_util_BDD_JBDD_##name
 #else
  #define JNI_PREFIX(name) _Java_jbdd_JBDD_ ## name
 #endif
#else
 #ifdef SUPREMICA
  #define JNI_PREFIX(name) Java_org_supremica_util_BDD_JBDD_##name
 #else
  #define JNI_PREFIX(name) Java_jbdd_JBDD_ ## name
 #endif
#endif





static DdManager *manager = 0;
static jint bdd_one, bdd_zero;
static int varcount, varnum;

// -------------------------------------------------------------------------------------------------
// internal variable table: for some odd reasons, manager->vars[] is
// currpted under UNIX (win32 ok)

static int max_vars, count_vars;
static jbdd_verbose = 0;
static DdNode **variables = 0;





#define MAX_REORDERING_METHODS		5

int reordering_method_table[MAX_REORDERING_METHODS] = {
  CUDD_REORDER_NONE,		// JBDD_REORDER_NONE
  CUDD_REORDER_WINDOW2,		// JBDD_REORDER_WIN2
  CUDD_REORDER_WINDOW3,		// JBDD_REORDER_WIN3
  CUDD_REORDER_SIFT,		// JBDD_REORDER_SIFT
  CUDD_REORDER_RANDOM		// JBDD_REORDER_RANDOM
};




#ifdef _DEBUG
#define CHECK_BDD(bdd) debug_check_bdd((DdNode*)bdd, __LINE__)
static int last_line = -1;
static void debug_check_bdd (DdNode * bdd, int from) {
	if( bdd == (DdNode *) 1) {
		fprintf (stderr, "BDD '1' is not the valid TRUE in CUDD. Error before lint %d in cudd.c\n", from);
    exit (20);
	}

  if (Cudd_Regular (bdd)->ref <= 0)
    {
      fprintf (stderr, "BAD refcount before %d, last call %d\n", from, last_line);
      exit (20);
    }

  /*
   * if(Cudd_DebugCheck(manager)) { fprintf(stderr, "Cudd_DebugCheck
   * failed at %d, last operation from %d\n", from, last_line);
   * exit(20); }
   *
   * if(Cudd_CheckKeys(manager)) { fprintf(stderr, "Cudd_CheckKeys
   * failed at %d, last call from %d\n", from, last_line); exit(20); }
   */

  last_line = from;
}
#else
#define CHECK_BDD(bdd)		/* ignored */
#endif






// --------------------------------------------------------------------------------------------


void
cleanup_variable_groups ()
{
  Cudd_FreeTree (manager);
}

void
cleanup_variables ()
{
  if (variables)
    {
      free (variables);
      variables = 0;
    }
}

void
realloc_variables ()
{
  int i, new_size;
  DdNode **tmp;

  if (variables)
    {
      new_size = max_vars * 2 + 1;
      tmp = (DdNode **) malloc (sizeof (DdNode *) * new_size);
      for (i = 0; i < count_vars; i++)
	tmp[i] = variables[i];
      free (variables);
      variables = tmp;
      max_vars = new_size;
    }
  else
    {				// first time:
      max_vars = DEFAULT_VARS;
      count_vars = 0;
      variables = (DdNode **) malloc (sizeof (DdNode *) * max_vars);
    }
}
void
addVariable (DdNode * nod)
{
  if (variables == 0 || count_vars >= max_vars)
    realloc_variables ();
  variables[count_vars++] = nod;
}

DdNode *
getVariable (int index)
{
  if (index < 0 || index >= count_vars)
    return 0;
  return variables[index];
}

// ------------------------------------------ some helper functions..
static int getNodeIndex (DdNode * node) {
  return (int) Cudd_NodeReadIndex (node);
}




void printBDDId (FILE * fp, jint bdd) {
  if (bdd == bdd_zero)    fprintf (fp, "False");
  else if (bdd == bdd_one)    fprintf (fp, "True ");
  else    {
		int index = getNodeIndex ((DdNode *) bdd);
		if (index < varcount)	// a variable
			fprintf (fp, "v%-4d", getNodeIndex ((DdNode *) bdd));
     else fprintf (fp, "%5d", getNodeIndex ((DdNode *) bdd));
	}
}



// -------------------------------------------- our CUDD GC HOOK

static long gc_counter = 0, gc_total_time = 0;

int gc_hook (DdManager * man, char *str, void *data) {
  long tmp = Cudd_ReadGarbageCollectionTime (manager);
  long time = tmp;
  gc_total_time = tmp;
  gc_counter++;

	if(jbdd_verbose) {
  	fprintf(stderr, "Garbage collection #%ld:	%2.1lf free / %ld / %ld total\n", gc_counter, Cudd_ReadNodesFreed (manager), time, gc_total_time);
  	fflush(stderr); // damn non-standard consoles...
	}
  return 1;
}

// --------------------------------------------------------- CUDD stuffs

JNIEXPORT void JNICALL JNI_PREFIX (init) (JNIEnv * env, jobject this_, jint vars, jint node_count) {
  long nodenum, cachesize;

  if (manager != 0) {
		fprintf (stderr, "JBDD.init called twice!\n");
		return;
	}


  if (node_count < MIN_NODES)    node_count = MIN_NODES;
  else if (node_count > MAX_NODES) {
		fprintf (stderr, "[JBDD:init()] Number of nodes should be between %d and %d	nodes\n", MIN_NODES, MAX_NODES);
		exit (EXIT_FAILURE);
	}



  nodenum = node_count / 10;	// that node_count was originaly computed
  // for BuDDy ...

  cachesize = nodenum / 20;	// WAS: 10

  manager = Cudd_Init (0 /* vars */ , 0, nodenum, cachesize, 0);
  if (!manager) {
		fprintf (stderr, "Unable to initilize BDD package\n");
		exit (EXIT_FAILURE);
	}

  Cudd_AddHook (manager, gc_hook, CUDD_POST_GC_HOOK);



	// the inverse of ReadZero is _NOT_ ReadOne in CUDD.
	// ReadOne seems to be totally unrelated to BDDs for that matter
  bdd_one = (jint) Cudd_ReadOne (manager);
  bdd_zero = (jint) Cudd_Not (Cudd_ReadOne (manager));
  Cudd_Ref ((DdNode *) bdd_one);	// just to be sure:)

  varcount = 0;
  varnum = (int) vars;
}

JNIEXPORT void JNICALL JNI_PREFIX (kill) (JNIEnv * env, jobject this_) {
  int bdds;

  if (manager == 0) {
		fprintf (stderr, "JBDD.kill called without a successful call to JBDD.init!\n");
		return;
   }

  bdds = Cudd_CheckZeroRef (manager);
  if (bdds > 0 && jbdd_verbose) {
  	fprintf (stderr, "Note: %d BDDs still in memory when terminating\n", bdds);
	}

  cleanup_variable_groups ();
  Cudd_Quit (manager);
  cleanup_variables ();
  manager = 0;

}



JNIEXPORT jint JNICALL JNI_PREFIX (getOne) (JNIEnv * env, jobject this_) {
  CHECK_BDD (bdd_one);
  return bdd_one;
}

JNIEXPORT jint JNICALL JNI_PREFIX (getZero) (JNIEnv * env, jobject this_) {
  CHECK_BDD (bdd_zero);
  return bdd_zero;
}



JNIEXPORT jint JNICALL JNI_PREFIX (createBDD) (JNIEnv * env, jobject this_) {
  DdNode *ret;

  if (varcount >= varnum) {
		fprintf (stderr, "Maximum var count (%d) reached!!\n", varnum);
		exit (EXIT_FAILURE);
	}

  ret = Cudd_bddNewVar (manager);
  addVariable (ret);
  varcount++;

  return (jint) ret;
}

JNIEXPORT jint JNICALL JNI_PREFIX (getVarCount) (JNIEnv * env, jobject this_) {
  return varcount;
}


JNIEXPORT jint JNICALL JNI_PREFIX (getBDD) (JNIEnv * env, jobject this_, jint index) {
  // return (jint) manager->vars[index]; /* DOESNT WORK UNDER UNIX ???
  // */
  DdNode *ret = getVariable (index);

  CHECK_BDD (ret);

  return (jint) ret;
}





// ---------------------------------------- reference counting
JNIEXPORT void JNICALL JNI_PREFIX (localDeref) (JNIEnv * env, jobject this_, jint bdd) {
  CHECK_BDD (bdd);
  Cudd_Deref ((DdNode *) bdd);
}

JNIEXPORT void JNICALL JNI_PREFIX (deref) (JNIEnv * env, jobject this_, jint bdd) {
  CHECK_BDD (bdd);
  Cudd_RecursiveDeref (manager, (DdNode *) bdd);
}



JNIEXPORT jint JNICALL JNI_PREFIX (ref) (JNIEnv * env, jobject this_, jint bdd) {
  Cudd_Ref ((DdNode *) bdd);
  return bdd;
}

// ----------------------------------------------------------- Operations




JNIEXPORT jint JNICALL JNI_PREFIX (and) (JNIEnv * env, jobject this_, jint l, jint r) {
  DdNode *ret;

  CHECK_BDD (l);
  CHECK_BDD (r);

  ret = Cudd_bddAnd (manager, (DdNode *) l, (DdNode *) r);
  Cudd_Ref (ret);
  return (jint) ret;
}

JNIEXPORT jint JNICALL JNI_PREFIX (or) (JNIEnv * env, jobject this_, jint l, jint r) {
  DdNode *ret;

  CHECK_BDD (l);
  CHECK_BDD (r);

  ret = Cudd_bddOr (manager, (DdNode *) l, (DdNode *) r);
  Cudd_Ref (ret);
  return (jint) ret;
}

JNIEXPORT jint JNICALL JNI_PREFIX (nand) (JNIEnv * env, jobject this_, jint l, jint r) {
  DdNode *ret;

  CHECK_BDD (l);
  CHECK_BDD (r);

  ret = Cudd_bddNand (manager, (DdNode *) l, (DdNode *) r);
  Cudd_Ref (ret);
  return (jint) ret;
}

JNIEXPORT jint JNICALL JNI_PREFIX (nor) (JNIEnv * env, jobject this_, jint l, jint r) {
  DdNode *ret;

  CHECK_BDD (l);
  CHECK_BDD (r);

  ret = Cudd_bddNor (manager, (DdNode *) l, (DdNode *) r);
  Cudd_Ref (ret);
  return (jint) ret;
}

JNIEXPORT jint JNICALL JNI_PREFIX (xor) (JNIEnv * env, jobject this_, jint l, jint r) {
  DdNode *ret;

  CHECK_BDD (l);
  CHECK_BDD (r);

  ret = Cudd_bddXor (manager, (DdNode *) l, (DdNode *) r);
  Cudd_Ref (ret);
  return (jint) ret;
}

JNIEXPORT jint JNICALL JNI_PREFIX (biimp) (JNIEnv * env, jobject this_, jint l, jint r) {
  DdNode *ret;

  CHECK_BDD (l);
  CHECK_BDD (r);

  ret = Cudd_bddXnor (manager, (DdNode *) l, (DdNode *) r);
  Cudd_Ref (ret);
  return (jint) ret;
}

JNIEXPORT jint JNICALL JNI_PREFIX (andTo) (JNIEnv * env, jobject this_, jint l, jint r) {
  DdNode *ret;

  CHECK_BDD (l);
  CHECK_BDD (r);

  ret = Cudd_bddAnd (manager, (DdNode *) l, (DdNode *) r);
  Cudd_Ref (ret);
  Cudd_RecursiveDeref (manager, (DdNode *) l);
  return (jint) ret;
}

JNIEXPORT jint JNICALL JNI_PREFIX (orTo) (JNIEnv * env, jobject this_, jint l, jint r) {
  DdNode *ret;

  CHECK_BDD (l);
  CHECK_BDD (r);

  ret = Cudd_bddOr (manager, (DdNode *) l, (DdNode *) r);
  Cudd_Ref (ret);
  Cudd_RecursiveDeref (manager, (DdNode *) l);

  return (jint) ret;
}


JNIEXPORT jint JNICALL JNI_PREFIX (ite) (JNIEnv * env, jobject this_, jint l, jint r, jint c) {
  DdNode *ret;

  CHECK_BDD (l);
  CHECK_BDD (r);
  CHECK_BDD (c);


  ret = Cudd_bddIte (manager, (DdNode *) l, (DdNode *) r, (DdNode *) c);
  Cudd_Ref (ret);
  return (jint) ret;
}


JNIEXPORT jint JNICALL JNI_PREFIX (imp) (JNIEnv * env, jobject this_, jint l, jint r) {
  DdNode *ret;
  DdNode *not_l;

  CHECK_BDD (l);
  CHECK_BDD (r);


  not_l = Cudd_Not ((DdNode *) l);
  Cudd_Ref (not_l);
  ret = Cudd_bddOr (manager, not_l, (DdNode *) r);
  Cudd_Ref (ret);
  Cudd_RecursiveDeref (manager, not_l);
  return (jint) ret;
}

JNIEXPORT jint JNICALL JNI_PREFIX (not) (JNIEnv * env, jobject this_, jint bdd) {
  DdNode *ret;

  CHECK_BDD (bdd);

  ret = Cudd_Not ((DdNode *) bdd);
  Cudd_Ref (ret);
  return (jint) ret;
}


JNIEXPORT jint JNICALL JNI_PREFIX (exists) (JNIEnv * env, jobject this_, jint bdd, jint cube) {
  DdNode *ret;

  CHECK_BDD (bdd);
  CHECK_BDD (cube);

  ret = Cudd_bddExistAbstract (manager, (DdNode *) bdd, (DdNode *) cube);
  Cudd_Ref (ret);
  return (jint) ret;
}

JNIEXPORT jint JNICALL JNI_PREFIX (forall) (JNIEnv * env, jobject this_, jint bdd, jint cube) {
  DdNode *ret;

  CHECK_BDD (bdd);
  CHECK_BDD (cube);

  ret = Cudd_bddUnivAbstract (manager, (DdNode *) bdd, (DdNode *) cube);
  Cudd_Ref (ret);
  return (jint) ret;
}


JNIEXPORT jint JNICALL JNI_PREFIX (relProd) (JNIEnv * env, jobject this_, jint l, jint r, jint cube) {
  DdNode *ret;

  CHECK_BDD (l);
  CHECK_BDD (r);
  CHECK_BDD (cube);

  ret = Cudd_bddAndAbstract (manager, (DdNode *) l, (DdNode *) r, (DdNode *) cube);
  Cudd_Ref (ret);
  return (jint) ret;
}


JNIEXPORT jint JNICALL JNI_PREFIX (restrict) (JNIEnv * env, jobject this_, jint f, jint c) {
  DdNode *ret;

  CHECK_BDD (f);
  CHECK_BDD (c);

  ret = Cudd_bddRestrict (manager, (DdNode *) f, (DdNode *) c);
  Cudd_Ref (ret);
  return (jint) ret;
}


JNIEXPORT jint JNICALL JNI_PREFIX (constrain) (JNIEnv * env, jobject this_, jint f, jint c) {
  DdNode *ret;

  CHECK_BDD (f);
  CHECK_BDD (c);

  ret = Cudd_bddConstrain (manager, (DdNode *) f, (DdNode *) c);
  Cudd_Ref (ret);
  return (jint) ret;
}



JNIEXPORT jint JNICALL JNI_PREFIX (makeSet___3III) (JNIEnv * env, jobject this_, jintArray vars, jint size, jint offset) {
  DdNode *tmp, *ret = (DdNode *) bdd_one;
  long i;


  jsize len = (*env)->GetArrayLength (env, vars);
  jint *body = (*env)->GetIntArrayElements (env, vars, 0);


  if (len - offset < size) {
		fprintf (stderr, "Accessing vector element outside range 0-%ld (%ld+%ld)\n", (long) len, (long) size, (long) offset);
		exit (EXIT_FAILURE);
	}



  Cudd_Ref (ret);
  for (i = size - 1; i >= 0; i--) {
		CHECK_BDD (body[offset + i]);

		tmp = Cudd_bddAnd (manager, ret, (DdNode *) body[offset + i]);

		Cudd_Ref (tmp);
		Cudd_RecursiveDeref (manager, ret);

		ret = tmp;
	}

  (*env)->ReleaseIntArrayElements (env, vars, body, 0);

  return (jint) ret;
}


JNIEXPORT jint JNICALL JNI_PREFIX (makeSet___3II) (JNIEnv * env, jobject this_, jintArray vars, jint size) {
  return JNI_PREFIX (makeSet___3III) (env, this_, vars, size, 0);
}


JNIEXPORT jint JNICALL JNI_PREFIX (createPair) (JNIEnv * env, jobject this_, jintArray old, jintArray new_) {

  int len = (int) (*env)->GetArrayLength (env, old);
  jint *old_vars = (*env)->GetIntArrayElements (env, old, 0);
  jint *new_vars = (*env)->GetIntArrayElements (env, new_, 0);
  int i, *permutation = (int *) malloc (sizeof (int) * varnum);
  int index1, index2;


  if (permutation == NULL) {
		fprintf (stderr, "FATAL: [createPair] unable to allocate permutation table\n");
		exit (EXIT_FAILURE);
	}

  for (i = 0; i < varnum; i++)
    permutation[i] = i;


  for (i = 0; i < len; i++) {
		index1 = getNodeIndex ((DdNode *) old_vars[i]);
		index2 = getNodeIndex ((DdNode *) new_vars[i]);
		permutation[index1] = index2;
	}

  (*env)->ReleaseIntArrayElements (env, new_, new_vars, 0);
  (*env)->ReleaseIntArrayElements (env, old, old_vars, 0);

  return (jint) permutation;
}


JNIEXPORT void JNICALL JNI_PREFIX (deletePair) (JNIEnv * env, jobject this_, jint permute) {
	// XXX: talk about security problems :(
  free ((int *) permute);
}


JNIEXPORT jint JNICALL JNI_PREFIX (replace) (JNIEnv * env, jobject this_, jint bdd, jint permute) {
  DdNode *ret;

  CHECK_BDD (bdd);

  ret = Cudd_bddPermute (manager, (DdNode *) bdd, (int *) permute);
  Cudd_Ref (ret);
  return (jint) ret;
}


JNIEXPORT void JNICALL JNI_PREFIX (showPair) (JNIEnv * env, jobject this_, jint permute) {
  int i, *p = (int *) permute;

  printf ("Permutation:\n");
  for (i = 0; i < varnum; i++)
    if (p[i] != i)
      printf ("\t %d -> %d\n", i, p[i]);

  fflush(stdout);
}

JNIEXPORT jint JNICALL JNI_PREFIX (support) (JNIEnv * env, jobject this_, jint bdd) {
  DdNode *ret;

  CHECK_BDD (bdd);

  ret = Cudd_Support (manager, (DdNode *) bdd);
  Cudd_Ref (ret);
  return (jint) ret;
}



JNIEXPORT jint JNICALL JNI_PREFIX (nodeCount) (JNIEnv * env, jobject this_, jint bdd) {
  CHECK_BDD (bdd);

  return Cudd_DagSize ((DdNode *) bdd);
}



JNIEXPORT jint JNICALL JNI_PREFIX (satOne) (JNIEnv * env, jobject this_, jint bdd) {
  CHECK_BDD (bdd);

  IGNORE_CALL;
  return bdd_zero;		// shoudl be refed firt, but what the
  // hell..
}

JNIEXPORT jdouble JNICALL JNI_PREFIX (satCount__I) (JNIEnv * env, jobject this_, jint bdd) {
  CHECK_BDD (bdd);

  return (jdouble) Cudd_CountMinterm (manager, (DdNode *) bdd, varcount);
}

JNIEXPORT jdouble JNICALL JNI_PREFIX (satCount__II) (JNIEnv * env, jobject this_, jint bdd, jint vars_ignored) {
  CHECK_BDD (bdd);
  CHECK_BDD (vars_ignored);

  IGNORE_CALL;
  return (jdouble) 0;
}

JNIEXPORT void JNICALL JNI_PREFIX (gc) (JNIEnv * env, jobject this_) {
  cuddGarbageCollect (manager, 1);
}


JNIEXPORT void JNICALL JNI_PREFIX (printDot__I) (JNIEnv * env, jobject this_, jint bdd) {
  DdNode *nod = (DdNode *) bdd;

  CHECK_BDD (bdd);

  Cudd_DumpDot (manager, 1, &nod, 0, 0, stdout);
  printf ("\n");
  fflush(stdout);
}

JNIEXPORT void JNICALL JNI_PREFIX (printDot__ILjava_lang_String_2) (JNIEnv *
								    env,
								    jobject
								    this_,
								    jint
								    bdd,
								    jstring
								    filename)
{

  char *str = (char *) (*env)->GetStringUTFChars (env, filename, 0);
  DdNode *nod = (DdNode *) bdd;
  FILE *fp = fopen (str, "w");

  CHECK_BDD (bdd);

  if (fp)
    {
      Cudd_DumpDot (manager, 1, &nod, 0, 0, fp);
      fclose (fp);
    }
  else
    fprintf (stderr, "Unable to open %s\n", str);

  (*env)->ReleaseStringUTFChars (env, filename, str);
}


JNIEXPORT void JNICALL JNI_PREFIX (print) (JNIEnv * env, jobject this_,
					   jint bdd)
{
  DdNode *nod = (DdNode *) bdd;

  CHECK_BDD (bdd);

  Cudd_DumpFactoredForm (manager, 1, &nod, 0, 0, stdout);
  printf ("\n");
  fflush (stdout);
}


JNIEXPORT void JNICALL JNI_PREFIX (printSet) (JNIEnv * env, jobject this_,
					      jint bdd)
{

  CHECK_BDD (bdd);

  if (bdd == bdd_one)
    printf ("True\n");
  else if (bdd == bdd_zero)
    printf ("False\n");
  else
    {
      printf ("\n");
      Cudd_PrintMinterm (manager, (DdNode *) bdd);
    }
  fflush (stdout);
}

JNIEXPORT void JNICALL JNI_PREFIX (printStats) (JNIEnv * env, jobject this_)
{
  Cudd_PrintInfo (manager, stdout);
}


JNIEXPORT jboolean JNICALL JNI_PREFIX (checkPackage) (JNIEnv * env,
						      jobject this_)
{
  // fast check of the variable and index tables
  int i, index;
  DdNode *node;
  for (i = 0; i < varcount; i++)
    {
      node = getVariable (i);
      index = getNodeIndex (node);
      if (i != index)
	{
	  printf ("ERROR: variable_%3i at 0x%8x has index %i\n", i,
		  (int) node, index);
	  return JNI_FALSE;
	}
    }


  return (jboolean) (Cudd_DebugCheck (manager) ? JNI_FALSE : JNI_TRUE);
}


JNIEXPORT void JNICALL JNI_PREFIX (debugPackage) (JNIEnv * env, jobject this_)
{

  if (Cudd_DebugCheck (manager))
    {
      fprintf (stderr, "Cudd_DebugCheck failed\n");
      exit (20);
    }

  if (Cudd_CheckKeys (manager))
    {
      fprintf (stderr, "Cudd_CheckKeys failed\n");
      exit (20);
    }
}


JNIEXPORT jboolean JNICALL JNI_PREFIX (debugBDD) (JNIEnv * env,
						  jobject this_, jint bdd)
{

  DdNode *tmp = (DdNode *) bdd;
  DdNode *node = Cudd_Regular (tmp);
  int inverted = (tmp != node);



  fprintf (stderr, "BDD ");
  printBDDId (stderr, bdd);
  fprintf (stderr, ": Refs %-3d, Nodes %-5d",
	   (int) Cudd_Regular (node)->ref, Cudd_DagSize (node));

  if (bdd != bdd_zero && bdd != bdd_one)
    {
      fprintf (stderr, " - if ");
      printBDDId (stderr, (jint) cuddT (node));
      fprintf (stderr, ", else ");
      printBDDId (stderr, (jint) cuddE (node));
      if (inverted)
	fprintf (stderr, "	(Inverted)");
    }
  fprintf (stderr, "\n");
  fflush(stderr);
  return (jboolean) (Cudd_Regular (node)->ref > 0 ? JNI_TRUE : JNI_FALSE);

}


JNIEXPORT jint JNICALL JNI_PREFIX (internal_1index) (JNIEnv * env,
						     jobject this_, jint bdd)
{
  CHECK_BDD (bdd);

  return getNodeIndex ((DdNode *) bdd);
}

JNIEXPORT jint JNICALL JNI_PREFIX (internal_1refcount) (JNIEnv * env,
							jobject this_,
							jint bdd)
{
  DdNode *tmp = (DdNode *) bdd;
  DdNode *node = Cudd_Regular (tmp);

  CHECK_BDD (bdd);

  return (jint) node->ref;

}


JNIEXPORT jboolean JNICALL JNI_PREFIX (internal_1isconst) (JNIEnv * env,
							   jobject this_,
							   jint bdd)
{
  CHECK_BDD (bdd);

  return (jboolean) (Cudd_IsConstant ((DdNode *) bdd) ? JNI_TRUE : JNI_FALSE);
}


JNIEXPORT jboolean JNICALL JNI_PREFIX (internal_1iscomplemented) (JNIEnv *
								  env,
								  jobject
								  this_,
								  jint bdd)
{
  CHECK_BDD (bdd);

  return (jboolean) (Cudd_Complement ((DdNode *) bdd) ? JNI_TRUE : JNI_FALSE);
}


JNIEXPORT jboolean JNICALL JNI_PREFIX (internal_1constvalue) (JNIEnv * env,
							      jobject this_,
							      jint bdd)
{
  CHECK_BDD (bdd);

  if (bdd == bdd_zero)
    return JNI_FALSE;
  else
    return JNI_TRUE;
}



JNIEXPORT jint JNICALL JNI_PREFIX (internal_1then) (JNIEnv * env,
						    jobject this_, jint bdd)
{
  DdNode *ret;

  CHECK_BDD (bdd);

  ret = cuddT ((DdNode *) bdd);
  Cudd_Ref (ret);
  return (jint) ret;
}

JNIEXPORT jint JNICALL JNI_PREFIX (internal_1else) (JNIEnv * env,
						    jobject this_, jint bdd)
{
  DdNode *ret;

  CHECK_BDD (bdd);

  ret = cuddE ((DdNode *) bdd);
  Cudd_Ref (ret);
  return (jint) ret;
}



// -----------------------------------------------------------------------------------


static int current_reordering_method = CUDD_REORDER_NONE;
JNIEXPORT void JNICALL JNI_PREFIX (reorder_1setMethod) (JNIEnv * env,
							jobject this_,
							jint method)
{
  if (method >= 0 && method < MAX_REORDERING_METHODS)
    current_reordering_method = reordering_method_table[method];
}

JNIEXPORT void JNICALL JNI_PREFIX (reorder_1now) (JNIEnv * env, jobject this_)
{
  Cudd_ReduceHeap (manager, current_reordering_method, 1);
}

JNIEXPORT void JNICALL JNI_PREFIX (reorder_1enableDyanamic) (JNIEnv * env,
							     jobject this_,
							     jboolean enable)
{

  if (enable == JNI_TRUE)
    Cudd_AutodynEnable (manager, current_reordering_method);
  else
    Cudd_AutodynDisable (manager);

}


JNIEXPORT void JNICALL JNI_PREFIX (reorder_1createVariableGroup)
  (JNIEnv * env, jobject this_, jint first, jint last, jboolean fix)
{

  MtrNode *group = Cudd_MakeTreeNode (manager, first, last - first + 1,
				      fix ==JNI_TRUE ? MTR_FIXED : MTR_DEFAULT);
}

// --------------------------------------------------------------
JNIEXPORT void JNICALL Java_JBDD_verbose (JNIEnv *env, jobject this_, jboolean verb_) {
	jbdd_verbose = verb_;
}

// --------------------------------------------------------------
JNIEXPORT jstring JNICALL JNI_PREFIX( getPackageName) (JNIEnv * env, jclass klass) {
	return (*env)->NewStringUTF(env, "CUDD");
}

JNIEXPORT jstring JNICALL JNI_PREFIX( getPackageVersion)   (JNIEnv *env, jclass klass) {
	return (*env)->NewStringUTF(env, CUDD_VERSION);
}

JNIEXPORT jint JNICALL JNI_PREFIX( getPackageCapabilities)  (JNIEnv *env, jclass klass) {
	return jbdd_JBDD_CAP_DVO | jbdd_JBDD_CAP_DVO_GROUP | jbdd_JBDD_CAP_CE;
}

