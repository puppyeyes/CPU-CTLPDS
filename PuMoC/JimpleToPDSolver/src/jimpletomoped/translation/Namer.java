// Namer.java
// Author: Matthew Hague
//
// The namer class provides the naming conventions used in the translations.


package jimpletomoped.translation;

import org.apache.log4j.Logger;

import soot.jimple.Stmt;
import soot.SootMethod;
import soot.Local;
import soot.SootField;
import soot.SootClass;
import soot.jimple.ParameterRef;
import soot.jimple.StaticFieldRef;

import soot.tagkit.Tag;
   
import jimpletomoped.main.CmdOptions;

import jimpletomoped.representation.RemoplaVar;

public class Namer {
       
    static Logger logger = Logger.getLogger(Namer.class);

    private static final String PARAM_PREFIX = "arg";
    private static final String THIS = "_this";
    private static final String GLOBAL_EXCEPTION = "_gexception";
    private static final String HEAP_ARRAY = "_heap";
    private static final String HEAP_POINTER = "_heapPtr";
    private static final String DEF_RETURN_NAME = "_ret";

    private static final String OUT_OF_HEAP = "LoutOfHeap";
    private static final String INDEX_OUT_OF_BOUNDS = "LindexOutOfBounds";
    private static final String NULL_POINTER = "LnullPointer";
    private static final String UNHANDLED_EXCEPTION = "LunhandledException";
    private static final String ERROR = "Lerror";

    private static final String CLINIT_SUBSIG = "void <clinit>()";
    private static final String REMOPLA_INIT_METHOD = "init00";
    private static final String ARRAY_LEN_SUFFIX = "_len_";

    private static final String GENERAL_VARIABLE_PREFIX = "_var";

    private static final String INT = "int";
    private static final String VOID = "void";
    private static final String INT_ARRAY = "int[]";

    private static final String INT_REGEX = "int";
    private static final String VOID_REGEX = "void";
    private static final String INT_ARRAY_REGEX = "int\\[\\]";


    private static final String D_INT = "I";
    private static final String D_VOID = "V";
    private static final String D_INT_ARRAY = "[I";

    private static final String CLASS_ID_SUFFIX = "_classId";
    private static final String CLASS_RANGE_MIN_SUFFIX = "_rangeMin";
    private static final String CLASS_RANGE_MAX_SUFFIX = "_rangeMax";
    private static final String CLASS_HEAP_SIZE_SUFFIX = "_size";

    private static final String NO_LINE_NUMBER = "NoLineNumber";

    private static final String NPE_MARKER = "_NPE";
    private static final String IOOB_MARKER = "_IOOB";
    private static final String OOH_MARKER = "_OOH";
    private static final String ASSERT_MARKER = "_ASSERT";
    private static final String ARITH_MARKER = "_ARITH";

    private static int nextLabel = 0;
    private static int nextVariableId = 0;

    private static int NULL = 0;

    static public String getMethodName(SootMethod sm) {
		return cleanString(sm.getBytecodeSignature());
    }

    static public String getLocalVariableName(Local l, SootMethod sm) {
        String methodName = getMethodName(sm);
        return cleanString(l.getName()) + "__in__" + methodName;
    }


    static public String getParameterName(ParameterRef par) {
        return getParameterName(par.getIndex());
    }

    static public String getDefaultReturnVarName() {
        return DEF_RETURN_NAME;
    }

    static public String getGlobalVariableName(StaticFieldRef field) {
        return cleanString(field.getField().getSignature());    
    }

    /** Returns the name of a parameter given its index in the method
      *  @param index the index of the parameter in the method
      *  @return the name of the parameter
      */
    static public String getParameterName(int index) {
        return PARAM_PREFIX + Integer.toString(index);
    }


    /** One name is always used for this references.  
     *  @return the name of the this variable
     */
    static public String getThisVariableName() {
        return THIS;
    }


    /** When throwing an exception, the global exception variable is used to store the
     * reference.  
     *   @return the name of the exception variable used to store the exception
     *   reference when one is thrown.
     */
    static public String getGlobalExceptionVariableName() {
        return GLOBAL_EXCEPTION;
    }


    /** 
     * @param userContent Part of the label defined by the user, to contain
     * their own information (for ease of reading, &c.)
     * @return a new label for a statement in method sm 
     **/
    static public String getNewLabel(String userContent) {
        return "L" + userContent + Integer.toString(nextLabel++);
    }

    /** 
     * @param stmt the Jimple statement the label is associated to
     * @param userContent Part of the label defined by the user, to contain
     * their own information (for ease of reading, &c.)
     * @return a new label for a statement in method sm 
     **/
    static public String getNewStmtLabel(Stmt stmt, String userContent) {
        assert stmt != null;
        String lineNoString = SootTagReader.getLineNumber(stmt);
        if (lineNoString == null) {
            lineNoString = NO_LINE_NUMBER;
        }
        return getNewLabel(userContent + "_" + lineNoString + "_");
    }

    static public String getErrorLabel() {
        return ERROR;
    }

    static public int getNull() {
        return NULL;
    }

   
    /** The class name used to identify the class in the Remopla (i.e. without
     * stub package prefix
     */ 
    static public String getUniversalClassName(SootClass sc) {
        return sc.getName().replaceAll(CmdOptions.getStubPackagePrefix(), "");
    }

    /** The concrete class name (i.e. has a stub prefix if the class has a stub)
     */
    static public String getConcreteClassName(SootClass sc) {
        return getConcreteClassName(sc.getName());
    }

    static public String getConcreteClassName(String className) {
        return !ClassManager.hasStub(className) ? className :
                                           CmdOptions.getStubPackagePrefix() + className;
    }


    /** Takes a signature (as used by Soot and the command line) and returns the
     * class part.  That is <package.Class: retType methodName(argTypes)> returns
     * package.Class.
     *  @param sig the signature to extract from
     *  @return the class part, null if the string cannot be split.
     */
    static public String getClassPartFromSig(String sig) {
        if (!isSignature(sig)) {
            return null;
        } else {
            return sig.substring(1, sig.indexOf(":")).trim();
        }
    }


    /** Takes a signature (as used by Soot and the command line) and returns the
     * method part.  That is <package.Class: retType methodName(argTypes)> returns
     * retType methodName(argTypes)
     *  @param sig the signature to extract from
     *  @return the method part, null if the string cannot be split.
     */
    static public String getMethodPartFromSig(String sig) {
        if (!isSignature(sig)) {
            return null;
        } else {
            return sig.substring(sig.indexOf(": ") + 1, sig.length() - 1).trim();
        }
    }

 
    /** Returns the method name from a given signature.  Null if not a
     * signature.
     */
    static public String getMethodNameFromSig(String methodSig) {
        String methodName = null;

        if (isSignature(methodSig)) {
            String methodPart = getMethodPartFromSig(methodSig);
            int space = methodPart.indexOf(" ");
            int bracket = methodPart.indexOf("(");
            methodName = methodPart.substring(space + 1, bracket);
        }
        
        return methodName;
    }

    
    /** Translates a soot style signature into a bytecode like descriptor.
     * That is, <package.Class: void method(int,int[])> becomes
     * packages/Class.method(I[I)V
     *   @return the descriptor, null if not a signature, or uses unhandled
     *   types
     */
    static public String getMethodDescFromSig(String methodSig) {
        String methodDesc = null; 

        if (isSignature(methodSig)) {
            String methodPart = getMethodPartFromSig(methodSig);

            int retTypeEnd = methodPart.indexOf(" ");
            String retType = methodPart.substring(0, retTypeEnd);
            String methodRest = methodPart.substring(retTypeEnd + 1, methodPart.length());

            // check args ok
            retType = sootTypeToDescType(retType);
            if (retType != null) {
                int argsStart = methodPart.indexOf("(");
                int argsEnd = methodPart.indexOf(")", argsStart);
   
                String argsPart = methodPart.substring(argsStart + 1, argsEnd);
                String[] args = argsPart.split(",");
                boolean argsOk = true;
                int i = 0;
                while (i < args.length && argsOk) {
                    argsOk = (args[i].length() == 0 || sootTypeToDescType(args[i]) != null);
                    i++;
                }

                if (argsOk) {
                    methodDesc = methodPart.substring(argsStart, argsEnd + 1);
                    methodDesc = methodDesc.replaceAll(INT_ARRAY_REGEX, D_INT_ARRAY);
                    methodDesc = methodDesc.replaceAll(INT_REGEX, D_INT);
                    methodDesc = methodDesc.replaceAll(VOID_REGEX, D_VOID);
                    methodDesc = methodDesc.replaceAll(",", "");
                    methodDesc += retType;
                } 
            }
        }

        return methodDesc; 
    }


   
    /** Signatures are of the form <package.Class: retType methodName(argTypes)>
     * where package.Class is the classPart, "retType methodName(argTypes)" is the
     * methodPart.  
     *   @param classPart the classPart
     *   @param methodPart the methodPart
     *   @return the signature constructed from the arguments
     */ 
    static public String makeSignature(String classPart, String methodPart) {
        return "<" + classPart + ": " + methodPart + ">";
    }


    /** Signatures are of the form <package.Class: retType methodName(argType,...,argType)>
     */
    static public boolean isSignature(String s) {
        if (s.length() < 3) return false;
        if (s.charAt(0) != '<') return false;
        if (s.charAt(s.length() - 1) != '>') return false;
        
        int split = s.indexOf(": ");
        if (split < 0) return false;

        int returnTypeStart = s.indexOf(" ", split + 1);
        if (returnTypeStart < 0) return false;

        int methodNameEnd = s.indexOf("(", returnTypeStart);
        if (methodNameEnd < 0) return false;

        int argsEnd = s.indexOf(")", methodNameEnd);
        if (argsEnd < 0) return false;
        return true;
    }


    /** @return the sub signature of a class's clinit method -- as returned by
     * SootClass.getSubSignature()
     */
    static public String getClinitSubSig() {
        return CLINIT_SUBSIG;
    }

    /** @return the name of the initial remopla method that does the set up
     * before calling the translated method
     */
    static public String getRemoplaInitMethod() {
        return REMOPLA_INIT_METHOD;
    }


    /** Return the name of an array length variable for use by the initial
     * method
     *  @param arrayVar the RemoplaVar representing the array
     *  @return the name of the length variable
     */
    static public String getArrayLengthVarName(RemoplaVar arrayVar) {
        return arrayVar + ARRAY_LEN_SUFFIX;
    }


    /** Returns a fresh variable name to use for a local quantified variable
     */
    static public String getNewVariableName() {
        return GENERAL_VARIABLE_PREFIX + (nextVariableId++);
    }    


    /** The name of a constant used to store the class id **/
    static public String getClassIdConstName(SootClass sc) {
        return cleanString(sc.getName() + CLASS_ID_SUFFIX);
    }

    /** The name of a constant used to store the min sub class id **/
    static public String getClassRangeMinConstName(SootClass sc) {
        return cleanString(sc.getName() + CLASS_RANGE_MIN_SUFFIX);
    }

    /** The name of a constant used to store the max sub class id **/
    static public String getClassRangeMaxConstName(SootClass sc) {
        return cleanString(sc.getName() + CLASS_RANGE_MAX_SUFFIX);
    }

    /** The name of a constant used to store the size of a class object on the
     * heap
     */
    static public String getClassHeapSizeConstName(SootClass sc) {
        return cleanString(sc.getName() + CLASS_HEAP_SIZE_SUFFIX);
    }

    /** Name of a constant storing the offset of a field in an object **/
    static public String getClassFieldName(SootField sf) {
        return cleanString(sf.getSignature());
    }


    ///////////////////////////////////////////////////////////////////////////////
    // Private methods
    //
    
    /** Returns the string according to our naming convention, taking cues
      * (and some code) from jMoped. 
      *  @param s the string
      */

    static private String cleanString(String s) {
        // removes the stub package prefix if it's in there
        String regex = CmdOptions.getStubPackagePrefix().replaceAll("\\.", "\\\\\\.");
        String newS = s.replaceAll(regex, "");
        regex = CmdOptions.getStubPackagePrefix().replaceAll("\\.", "\\/");
        newS = newS.replaceAll(regex, "");
        s = newS;



		s = s.replaceAll("[()/;$]", "_");
		s = s.replace('[', 'A');
        s = s.replace(']', 'A');
		s = s.replace('.', '_');
		s = s.replaceAll("_{2,}", "_");
		s = s.replaceAll("[<>]", "_");
        
        // Only change from moped -- needed for class name/method name
        // separation in bytecode signature
        s = s.replaceAll(": ", "__");
        // Remove any lagging spaces
        s = s.replaceAll(" ", "_");

//		if (s.endsWith("_"))
//			s = s.substring(0, s.length()-1);
//		if (s.startsWith("_"))
//			s = s.substring(1, s.length());
        
        return s;
    }


    /** Converts int to I, int[] to [I, void to V, &c.  Null if case not
     * handled.
     */
    static private String sootTypeToDescType(String sType) {
        String dType = null;

        if (sType.equals(VOID)) {
            dType = D_VOID;
        } else if (sType.equals(INT_ARRAY)) {
            dType = D_INT_ARRAY;
        } else if (sType.equals(INT)) {
            dType = INT; 
        }

        return dType;
    }
}
