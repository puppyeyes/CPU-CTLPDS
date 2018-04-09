

#include "common.h"

extern int sbl_verbose;

// -----------------------------------------------------------------------------------



JNIEXPORT void JNICALL JNI_PREFIX (reorder_1setMethod) (JNIEnv * env, jobject this_, jint method) {
// not implemented
}

JNIEXPORT void JNICALL JNI_PREFIX (reorder_1now) (JNIEnv * env, jobject this_) {
 // not implemented
}

JNIEXPORT void JNICALL JNI_PREFIX (reorder_1enableDyanamic) (JNIEnv * env, jobject this_, jboolean enable) {
	// not implemented
}


JNIEXPORT void JNICALL JNI_PREFIX (reorder_1createVariableGroup) (JNIEnv * env, jobject this_, jint first, jint last, jboolean fix){
	// not implemented
}


// ---------------------------

JNIEXPORT jstring JNICALL JNI_PREFIX( getPackageName) (JNIEnv * env, jclass klass) {
	return (*env)->NewStringUTF(env, "SBL");
}


JNIEXPORT jint JNICALL JNI_PREFIX( getPackageCapabilities)  (JNIEnv *env, jclass klass) {
	return jbdd_JBDD_CAP_NONE;
}


// --------------------------------------------------------------

JNIEXPORT void JNICALL Java_JBDD_verbose (JNIEnv *env, jobject this_, jboolean verb_) {
	sbl_verbose = verb_;
}

// --------------------------------------------------------------
JNIEXPORT jint JNICALL JNI_PREFIX (internal_1index) (JNIEnv * env, jobject this_, jint bdd) {
  CHECK_BDD (bdd);

  return (bdd < varcount * 2) ? (bdd / 2 - 1) : LEVEL (bdd);
}


JNIEXPORT void JNICALL JNI_PREFIX (printSet (JNIEnv * env, jobject this_, jint bdd)) {
	bdd_printset(bdd);
}


