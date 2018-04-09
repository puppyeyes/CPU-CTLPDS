
/**
 * this file contains the code common for SBL and buddy
 */

// XXX: we havent considred reordered variables yet!
// stuff like var2level() may be needed in some places when dynamic
// reordering is active!

#include <jni.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// BuDDy headers
#include <bdd.h>
#include <kernel.h>
// #include <cache.h>

#include "jbdd.h"

// best to use C++ keywords (?)
enum e_bool { false = 0, true = 1 };
#define bool int


// see if the __func__ macro is available??
static void dummy_function() {
#ifndef __func__
 #define __func__ "?"
#endif
}

#define IGNORE_CALL fprintf(stderr,"(this function (%s, %s/%d) is not implemented)\n",  __func__, __FILE__, __LINE__)


// ----------------------------------------------------------------------

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



// ----------------------------------------------------------------------

static int has_bdd = 0;

static int varnum = 0;		// max vars
static int varcount = 0;	// current var count!


#define MAX_NODES 8000000
#define MIN_NODES 1000


#define MAX_NODE_INCREASE 200000	/* default value is 50000 */





// --- [ JBDD internal refcount checks ]
// -------------------------------------
#ifdef _DEBUG
#define CHECK_BDD(bdd) debug_check_bdd(env, bdd, __LINE__, __func__ )

static char *lastfunc;
static void
throw_exception (JNIEnv * env)
{
  jclass cls;
  (*env)->ExceptionDescribe (env);
  (*env)->ExceptionClear (env);

  cls = (*env)->FindClass (env, "java/lang/Exception");
  if (cls == 0)
    {
      fprintf (stderr,
	       "Fatal error, could not load java.lang.Exception in the native code.\n");
      getchar ();
      exit (20);
    }

  (*env)->ThrowNew (env, cls, "CHECK_BDD failed in the native code");
}

static void
debug_check_bdd (JNIEnv * env, int bdd, int line, char *func)
{
  if (bdd < 0 || bdd > bddnodesize)
    {
      fprintf (stderr,
	       "Invalid BDD (%d) at line %d function %s, last call to %s\n",
	       bdd, line, func, lastfunc);
      throw_exception (env);
    }

  if (bddnodes[bdd].refcou <= 0)
    {
      fprintf (stderr,
	       "Zero refcount for BDD (%d) at line %d function %s, last call to %s\n",
	       bdd, line, func, lastfunc);
      throw_exception (env);
    }
  lastfunc = func;
}
#else
#define CHECK_BDD(bdd)		/* do nothing */
#endif


// --------------------------------------------------------------------------------
JNIEXPORT void JNICALL JNI_PREFIX (init) (JNIEnv * env, jobject this_,
					  jint varnum_, jint node_count)
{
  int ok;
  long nodenum, cachesize;

  if (node_count < MIN_NODES)
    node_count = MIN_NODES;
  else if (node_count > MAX_NODES)
    {
      fprintf (stderr,
	       "[JBDD:init()] Number of nodes should be between %d and %d	nodes\n",
	       MIN_NODES, MAX_NODES);
      exit (20);
    }

  nodenum = node_count;
  cachesize = nodenum / 8;	// WAS: 10


  if (has_bdd)
    {
      fprintf (stderr,
	       "A BDD package exist while you are trying to create another one\n"
	       "The BDD package is SINGLETON!\n");

      exit (20);
    }

  ok = bdd_init (nodenum, cachesize);
  if (ok == 0)
    {
      varnum = varnum_;
      varcount = 0;

      bdd_setmaxincrease (MAX_NODE_INCREASE);
      bdd_setvarnum (2 + 2 * varnum);
      has_bdd = 1;


      // check if true / false falues are same as in JBDD.java !!
      if (bdd_false () != 0 || bdd_true () != 1)
	{
	  fprintf (stderr, " INTERNAL ERROR : false = %d, true = %d\n",
		   bdd_false (), bdd_true ());
	  exit (20);
	}

    }
  else
    {
      fprintf (stderr, "bdd_init(%ld,%ld) Failed\n (error code %d)\n",
	       nodenum, cachesize, ok);
      exit (20);
    }

}

JNIEXPORT void JNICALL JNI_PREFIX (kill) (JNIEnv * env, jobject obj)
{
  if (has_bdd)
    {
      bdd_done ();
      has_bdd = 0;
    }
  else
    fprintf (stderr, "Killing already dead BDD class :(\n");

}


JNIEXPORT jint JNICALL JNI_PREFIX (getOne) (JNIEnv * env, jobject obj)
{
  BDD ret = bdd_true ();

  CHECK_BDD (ret);

  return (jint) ret;
}

JNIEXPORT jint JNICALL JNI_PREFIX (getZero) (JNIEnv * env, jobject obj)
{
  BDD ret = bdd_false ();

  CHECK_BDD (ret);

  return (jint) ret;
}


JNIEXPORT jint JNICALL JNI_PREFIX (createBDD) (JNIEnv * env, jobject obj)
{
  BDD ret;

  if (varcount >= varnum)
    {
      fprintf (stderr, "Maximum var count (%d) reached!!\n", varnum);
      exit (20);
    }

  ret = bdd_ithvar (varcount++);
  // bddnodes[ret].refcou = 1;	// why does BuDDy sets the initial
  // refcount to MAXREF (0x3FF) ?

  return ret;
}




JNIEXPORT jint JNICALL JNI_PREFIX (getVarCount) (JNIEnv * env, jobject obj)
{
  return varcount;
}



JNIEXPORT jint JNICALL JNI_PREFIX (getBDD) (JNIEnv * env, jobject obj,
					    jint index)
{
  if (index < 0 || index >= varcount)
    {
      fprintf (stderr, "[JBUDDY.getBDD] requested bad BDD: %d\n", index);
      return (jint) bdd_false;
    }
  return bdd_ithvar (index);
}




JNIEXPORT jint JNICALL JNI_PREFIX (ref) (JNIEnv * env, jobject obj, jint bdd)
{
  bdd_addref (bdd);
  return bdd;
}
JNIEXPORT void JNICALL JNI_PREFIX (localDeref) (JNIEnv * env, jobject obj,
						jint bdd)
{
  CHECK_BDD (bdd);

  bdd_delref (bdd);
}
JNIEXPORT void JNICALL JNI_PREFIX (deref) (JNIEnv * env, jobject obj,
					   jint bdd)
{

  CHECK_BDD (bdd);

  // ok, there is no recursive deref in BuDDy, as it is in CUDD.
  // I don't know how to fix this. I don't even know if this is needed
  // at all :)
  bdd_delref (bdd);
}





JNIEXPORT jint JNICALL JNI_PREFIX (and) (JNIEnv * env, jobject obj, jint l,
					 jint r)
{
  int bdd;

  CHECK_BDD (l);
  CHECK_BDD (r);

  bdd = bdd_apply (l, r, bddop_and);
  bdd_addref (bdd);
  return bdd;
}
JNIEXPORT jint JNICALL JNI_PREFIX (or) (JNIEnv * env, jobject obj, jint l,
					jint r)
{
  int bdd;

  CHECK_BDD (l);
  CHECK_BDD (r);

  bdd = bdd_apply (l, r, bddop_or);
  bdd_addref (bdd);
  return bdd;
}
JNIEXPORT jint JNICALL JNI_PREFIX (andTo) (JNIEnv * env, jobject obj,
					   jint l, jint r)
{
  BDD ret;

  CHECK_BDD (l);
  CHECK_BDD (r);

  ret = bdd_apply (l, r, bddop_and);
  bdd_addref (ret);
  bdd_delref (l);
  return ret;
}
JNIEXPORT jint JNICALL JNI_PREFIX (orTo) (JNIEnv * env, jobject obj, jint l,
					  jint r)
{
  BDD ret;

  CHECK_BDD (r);
  CHECK_BDD (l);

  ret = bdd_apply (l, r, bddop_or);
  bdd_addref (ret);
  bdd_delref (l);
  return ret;
}





JNIEXPORT jint JNICALL JNI_PREFIX (nand) (JNIEnv * env, jobject this_,
					  jint l, jint r)
{
  BDD bdd;

  CHECK_BDD (l);
  CHECK_BDD (r);

  bdd = bdd_apply (l, r, bddop_nand);
  bdd_addref (bdd);
  return bdd;
}

JNIEXPORT jint JNICALL JNI_PREFIX (nor) (JNIEnv * env, jobject this_,
					 jint l, jint r)
{
  BDD bdd;

  CHECK_BDD (l);
  CHECK_BDD (r);


  bdd = bdd_apply (l, r, bddop_nor);
  bdd_addref (bdd);
  return bdd;
}

JNIEXPORT jint JNICALL JNI_PREFIX (xor) (JNIEnv * env, jobject this_,
					 jint l, jint r)
{
  BDD bdd;

  CHECK_BDD (l);
  CHECK_BDD (r);

  bdd = bdd_apply (l, r, bddop_xor);
  bdd_addref (bdd);
  return bdd;
}

JNIEXPORT jint JNICALL JNI_PREFIX (ite) (JNIEnv * env, jobject this_,
					 jint if_, jint then_, jint else_)
{

  BDD bdd;

  CHECK_BDD (if_);
  CHECK_BDD (then_);
  CHECK_BDD (else_);

  bdd = bdd_ite (if_, then_, else_);
  bdd_addref (bdd);
  return bdd;
}

JNIEXPORT jint JNICALL JNI_PREFIX (imp) (JNIEnv * env, jobject this_,
					 jint l, jint r)
{
  BDD bdd;

  CHECK_BDD (l);
  CHECK_BDD (r);

  bdd = bdd_apply (l, r, bddop_imp);
  bdd_addref (bdd);
  return bdd;
}

JNIEXPORT jint JNICALL JNI_PREFIX (biimp) (JNIEnv * env, jobject this_,
					   jint l, jint r)
{
  BDD bdd;

  CHECK_BDD (l);
  CHECK_BDD (r);

  bdd = bdd_apply (l, r, bddop_biimp);
  bdd_addref (bdd);
  return bdd;
}

JNIEXPORT jint JNICALL JNI_PREFIX (not) (JNIEnv * env, jobject this_,
					 jint bdd)
{
  BDD tmp;

  CHECK_BDD (bdd);

  tmp = bdd_not (bdd);
  bdd_addref (tmp);
  return tmp;
}

JNIEXPORT jint JNICALL JNI_PREFIX (exists) (JNIEnv * env, jobject this_,
					    jint bdd, jint cube)
{
  BDD tmp;

  CHECK_BDD (bdd);
  CHECK_BDD (cube);

  tmp = bdd_exist (bdd, cube);
  bdd_addref (tmp);
  return tmp;
}

JNIEXPORT jint JNICALL JNI_PREFIX (forall) (JNIEnv * env, jobject this_,
					    jint bdd, jint cube)
{
  BDD tmp;

  CHECK_BDD (bdd);
  CHECK_BDD (cube);

  tmp = bdd_forall (bdd, cube);
  bdd_addref (tmp);
  return tmp;
}

JNIEXPORT jint JNICALL JNI_PREFIX (relProd) (JNIEnv * env, jobject this_,
					     jint l, jint r, jint cube)
{

  BDD bdd;

  CHECK_BDD (l);
  CHECK_BDD (r);

  bdd = bdd_appex (l, r, bddop_and, cube);
  bdd_addref (bdd);
  return bdd;
}


JNIEXPORT jint JNICALL JNI_PREFIX (restrict) (JNIEnv * env, jobject this_,
					      jint r, jint var)
{
  BDD bdd;

  CHECK_BDD (r);
  CHECK_BDD (var);

  bdd = bdd_restrict (r, var);
  bdd_addref (bdd);
  return bdd;
}

JNIEXPORT jint JNICALL JNI_PREFIX (constrain) (JNIEnv * env, jobject this_,
					       jint f, jint c)
{
  BDD bdd;

  CHECK_BDD (f);
  CHECK_BDD (c);

  bdd = bdd_constrain (f, c);
  bdd_addref (bdd);
  return bdd;
}


JNIEXPORT jint JNICALL JNI_PREFIX (makeSet___3III)
  (JNIEnv * env, jobject this_, jintArray vars, jint size, jint offset)
{
  BDD tmp, ret = 1;
  int i;
  jsize len = (*env)->GetArrayLength (env, vars);
  jint *body = (*env)->GetIntArrayElements (env, vars, 0);

  if (len - offset < size)
    {
      fprintf (stderr,
	       "Accessing vector element outside range 0-%d (%d+%d)\n",
	       len, size, offset);
      exit (20);
    }

  // DONT USE ´ret = bdd_makeset(body + offset, size);´ (it uses index
  // not variables)

  bdd_addref (ret);
  for (i = size - 1; i >= 0; i--)
    {

      CHECK_BDD (body[offset + i]);

      tmp = bdd_apply (ret, body[offset + i], bddop_and);
      bdd_addref (tmp);
      bdd_delref (ret);
      ret = tmp;
    }

  (*env)->ReleaseIntArrayElements (env, vars, body, 0);

  return ret;

}

JNIEXPORT jint JNICALL JNI_PREFIX (makeSet___3II)
  (JNIEnv * env, jobject this_, jintArray vars, jint size)
{
  return JNI_PREFIX (makeSet___3III (env, this_, vars, size, 0));
}


JNIEXPORT jint JNICALL JNI_PREFIX (createPair)
  (JNIEnv * env, jobject this_, jintArray old, jintArray new_)
{
  bddPair *pair = bdd_newpair ();

  jsize i, len = (*env)->GetArrayLength (env, old);
  jint *old_vars = (*env)->GetIntArrayElements (env, old, 0);
  jint *new_vars = (*env)->GetIntArrayElements (env, new_, 0);

  for (i = 0; i < len; i++)
    {

      /*
       * XXX - not bdd_var() call for new_vars[i] ??
       */
      bdd_setbddpair (pair, bdd_var (old_vars[i]), new_vars[i]);

    }

  (*env)->ReleaseIntArrayElements (env, new_, new_vars, 0);
  (*env)->ReleaseIntArrayElements (env, old, old_vars, 0);

  return (jint) pair;

}

JNIEXPORT void JNICALL
JNI_PREFIX (deletePair (JNIEnv * env, jobject this_, jint pair))
{

  bdd_freepair ((bddPair *) pair);
}

JNIEXPORT jint JNICALL
JNI_PREFIX (replace (JNIEnv * env, jobject this_, jint bdd, jint pair))
{
  BDD tmp;

  CHECK_BDD (bdd);

  tmp = bdd_replace (bdd, (bddPair *) pair);
  bdd_addref (tmp);
  return tmp;
}

JNIEXPORT void JNICALL
JNI_PREFIX (showPair (JNIEnv * env, jobject this_, jint pair))
{
  printf ("(function not supported, yet)\n");
}





JNIEXPORT jint JNICALL
JNI_PREFIX (support (JNIEnv * env, jobject this_, jint bdd))
{
  BDD tmp;

  CHECK_BDD (bdd);

  tmp = bdd_support (bdd);
  bdd_addref (tmp);
  return tmp;
}

JNIEXPORT jint JNICALL
JNI_PREFIX (nodeCount (JNIEnv * env, jobject this_, jint bdd))
{
  CHECK_BDD (bdd);

  return bdd_nodecount (bdd);
}

JNIEXPORT jint JNICALL
JNI_PREFIX (satOne (JNIEnv * env, jobject this_, jint bdd))
{
  BDD tmp;
  CHECK_BDD (bdd);

  tmp = bdd_satone (bdd);
  bdd_addref (tmp);
  return tmp;
}

/*
 * --------------------------------------------------------------------------
 */



JNIEXPORT jdouble JNICALL JNI_PREFIX (satCount__I (JNIEnv * env, jobject this_, jint bdd)) {
  double div, sat;

  CHECK_BDD (bdd);

  // See init for a explaination about 2 + varnum...
  div = pow (2, 2 + varnum);
  sat = bdd_satcount (bdd);
  // fprintf(stderr, "sat = %lf, div = %lf or 2^%ld\n", sat, div, ( 2 +
  // varnum));
  sat /= div;

  return sat;

}

JNIEXPORT jdouble JNICALL
JNI_PREFIX (satCount__II (JNIEnv * env, jobject this_,
			  jint bdd, jint vars_ignored))
{

  CHECK_BDD (bdd);

  // see init ...
  return (jdouble) bdd_satcount (bdd) / pow (2,
					     2 + varnum + 2 * vars_ignored);
}


/*
 * --------------------------------------------------------------------------
 */




JNIEXPORT void JNICALL JNI_PREFIX (gc) (JNIEnv * env, jobject this_)
{
  bdd_gbc ();
}

JNIEXPORT void JNICALL JNI_PREFIX (printDot__I) (JNIEnv * env,
						 jobject this_, jint bdd)
{

  CHECK_BDD (bdd);

  bdd_printdot (bdd);
  printf ("\n");
}

JNIEXPORT void JNICALL
JNI_PREFIX (printDot__ILjava_lang_String_2
	    (JNIEnv * env, jobject this_, jint bdd, jstring filename))
{


  char *str = (char *) (*env)->GetStringUTFChars (env, filename, 0);

  CHECK_BDD (bdd);

  bdd_fnprintdot (str, bdd);
  (*env)->ReleaseStringUTFChars (env, filename, str);
}


JNIEXPORT void JNICALL
JNI_PREFIX (print (JNIEnv * env, jobject this_, jint bdd))
{

  CHECK_BDD (bdd);

  bdd_printtable (bdd);
  printf ("\n");
  fflush (stdout);
}





JNIEXPORT void JNICALL JNI_PREFIX (printStats) (JNIEnv * env, jobject this_)
{
  bdd_printstat ();
}



void
dumpBDD_info (int bdd)
{
  BddNode *node;

  node = &bddnodes[bdd];
  fprintf (stderr,
	   "BDD %d:	RefCount %d, Level %-3d, IF %-3d, ELSE %-3d, HASH %d\n",
	   bdd, node->refcou, node->level, node->high, node->low, node->hash);
	fflush(stderr);
}


int
checkBDD (int bdd)
{
  if (!bddrunning)
    {
      bdd_error (BDD_RUNNING);
      return 0;
    }
  if (bdd < 0 || (bdd) >= bddnodesize)
    {
      bdd_error (BDD_ILLBDD);
      return 0;
    }
  if (bdd >= 2 && LOW (bdd) == -1)
    {
      bdd_error (BDD_ILLBDD);
      return 0;
    }

  if (bddnodes[bdd].refcou == 0)
    {
      fprintf (stderr, "ERROR: refcount is zero\n");
      return 0;
    }
  // refcount for TRUE and FALSE is saturated by default, ignore them!
  if (bdd > 2 && bddnodes[bdd].refcou == MAXREF)
    {
      fprintf (stderr, "ERROR: refcount is saturated\n");
      return 0;
    }

  return 1;
}

int
checkBuddy ()
{
  int i;

  if (!checkBDD (0))
    {
      fprintf (stderr, "bdd FALSE failed sanity check\n");
      dumpBDD_info (0);
      return 0;
    }

  if (!checkBDD (1))
    {
      fprintf (stderr, "bdd TRUE failed sanity check\n");
      dumpBDD_info (1);
      return 0;
    }

  for (i = bddvarnum; i < bddvarnum; i++)
    {
      if (!checkBDD (2 * i + 2))
	{
	  dumpBDD_info (2 * i + 2);
	  printf ("bdd variable #%d failed sanity check\n", i);
	  return 0;
	}

      if (!checkBDD (2 * i + 3))
	{
	  printf ("shadow of bdd variable #%d failed sanity check\n", i);
	  dumpBDD_info (2 * i + 3);
	  return 0;
	}
    }

  return 1;
}

JNIEXPORT jboolean JNICALL JNI_PREFIX (checkPackage) (JNIEnv * env,
						      jobject this_)
{
  return checkBuddy ()? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL JNI_PREFIX (debugPackage) (JNIEnv * env, jobject this_)
{
  IGNORE_CALL;
}

JNIEXPORT jboolean JNICALL JNI_PREFIX (debugBDD) (JNIEnv * env,
						  jobject this_, jint bdd)
{

  CHECK_BDD (bdd);

  dumpBDD_info (bdd);
  return checkBDD (bdd) ? JNI_TRUE : JNI_FALSE;
}


JNIEXPORT jint JNICALL JNI_PREFIX (internal_1refcount) (JNIEnv * env,
							jobject this_,
							jint bdd)
{

  CHECK_BDD (bdd);

  return bddnodes[bdd].refcou;
}

JNIEXPORT jboolean JNICALL JNI_PREFIX (internal_1isconst) (JNIEnv * env,
							   jobject this_,
							   jint bdd)
{

  CHECK_BDD (bdd);

  return (bdd == bddfalse) || (bdd == bddtrue);

}

JNIEXPORT jboolean JNICALL JNI_PREFIX (internal_1constvalue) (JNIEnv * env,
							      jobject this_,
							      jint bdd)
{

  CHECK_BDD (bdd);

  if (bdd == bddfalse)
    return JNI_FALSE;
  else
    return JNI_TRUE;

}

JNIEXPORT jboolean JNICALL JNI_PREFIX (internal_1iscomplemented) (JNIEnv *
								  env,
								  jobject
								  this_,
								  jint bdd)
{
  CHECK_BDD (bdd);

  return JNI_FALSE;		// no CE in BuDDy
}

JNIEXPORT jint JNICALL JNI_PREFIX (internal_1then) (JNIEnv * env,
						    jobject this_, jint bdd)
{
  CHECK_BDD (bdd);

  return bdd_high (bdd);
}

JNIEXPORT jint JNICALL JNI_PREFIX (internal_1else) (JNIEnv * env,
						    jobject this_, jint bdd)
{
  CHECK_BDD (bdd);

  return bdd_low (bdd);
}



// -------------------------------------------------------------------
JNIEXPORT jstring JNICALL JNI_PREFIX( getPackageVersion)   (JNIEnv *env, jclass klass) {
	char tmp[32];
	int v = bdd_versionnum();
	sprintf(tmp, "%d.%d", v/10, v%10 );
	return (*env)->NewStringUTF(env, tmp);
}
