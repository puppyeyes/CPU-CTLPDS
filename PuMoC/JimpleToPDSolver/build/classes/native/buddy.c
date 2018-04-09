

#include "common.h"



// XXX: we havent considred reordered variables yet!
// stuff like var2level() may be needed in some places when dynamic
// reordering is active!


// --- [ dynamic reordering ]
// --------------------------------------------------
#define MAX_REORDERING_METHODS		5
int reordering_method_table[MAX_REORDERING_METHODS] = {
  BDD_REORDER_NONE,		// JBDD_REORDER_NONE
  BDD_REORDER_WIN2,		// JBDD_REORDER_WIN2
  BDD_REORDER_WIN3,		// JBDD_REORDER_WIN3
  BDD_REORDER_SIFT,		// JBDD_REORDER_SIFT
  BDD_REORDER_RANDOM		// JBDD_REORDER_RANDOM
};


// -----------------------------------------------------------------------------------


static int current_reordering_method = BDD_REORDER_NONE;
JNIEXPORT void JNICALL JNI_PREFIX (reorder_1setMethod) (JNIEnv * env,
							jobject this_,
							jint method)
{
  if (method >= 0 && method < MAX_REORDERING_METHODS)
    current_reordering_method = reordering_method_table[method];
}

JNIEXPORT void JNICALL JNI_PREFIX (reorder_1now) (JNIEnv * env, jobject this_)
{
  bdd_reorder (current_reordering_method);
}

JNIEXPORT void JNICALL JNI_PREFIX (reorder_1enableDyanamic) (JNIEnv * env,
							     jobject this_,
							     jboolean enable)
{
  if (enable == JNI_TRUE)
    bdd_enable_reorder ();
  else
    bdd_disable_reorder ();
}


JNIEXPORT void JNICALL JNI_PREFIX (reorder_1createVariableGroup)
  (JNIEnv * env, jobject this_, jint first, jint last, jboolean fix)
{
  bdd_intaddvarblock (first, last,
		      fix == JNI_TRUE ? BDD_REORDER_FIXED : BDD_REORDER_FREE);
}


// ------------------------------------------------------------------------
JNIEXPORT jstring JNICALL JNI_PREFIX( getPackageName) (JNIEnv * env, jclass klass) {
	return (*env)->NewStringUTF(env, bdd_versionstr() );
}


JNIEXPORT jint JNICALL JNI_PREFIX( getPackageCapabilities)  (JNIEnv *env, jclass klass) {
	return jbdd_JBDD_CAP_DVO | jbdd_JBDD_CAP_DVO_GROUP;
}


// --------------------------------------------------------------

JNIEXPORT void JNICALL Java_JBDD_verbose (JNIEnv *env, jobject this_, jboolean verb_) {
	// NOT IMPLEMENTED!
}


// -------------------------------------------------------------------------



JNIEXPORT jint JNICALL JNI_PREFIX (internal_1index) (JNIEnv * env, jobject this_, jint bdd) {
  CHECK_BDD (bdd);

  return (bdd < varcount * 2) ? (bdd / 2 - 1) : bddlevel2var[LEVEL (bdd)];
}

void printSet_rec (char *txt, int level, BDD bdd) {

  if (bdd == 0)
    {
      /*
       * do nothing
       */
      return;
    }
  else if (bdd == 1)
    {
      printf ("%s	1\n", txt);
      return;
    }
  else
    {
      BDD low = LOW (bdd);
      BDD high = HIGH (bdd);
      int l;
      char save;


      // printf("BDD=%d/%d, LOW=%d, HIGH=%d\n", bdd,
      // bddlevel2var[LEVEL(bdd)], low, high);
      l = bdd < varcount * 2 ? bdd / 2 - 1 : bddlevel2var[LEVEL (bdd)];

      save = txt[l];
      txt[l] = '0';
      // printf("*L_low=%d\n", l);
      printSet_rec (txt, level + 1, low);
      txt[l] = save;


      save = txt[l];
      txt[l] = '1';
      // printf("*L_high=%d (%d)\n", l, varcount);
      printSet_rec (txt, level + 1, high);
      txt[l] = save;
    }
}

JNIEXPORT void JNICALL
JNI_PREFIX (printSet (JNIEnv * env, jobject this_, jint bdd))
{

  char *txt;
  int i;


  CHECK_BDD (bdd);


  if (bdd < 2)
    {
      printf ("\n%s\n", bdd == 0 ? "False" : "True");
      return;
    }

  txt = (char *) malloc (bddvarnum + 2);
  if (!txt)
    {
      bdd_error (BDD_MEMORY);
      return;
    }

  for (i = 0; i < varcount; i++)
    txt[i] = '-';
  txt[i] = '\0';

  printf ("\n");
  printSet_rec (txt, 0, bdd);

  free (txt);
  fflush(stdout);
}


