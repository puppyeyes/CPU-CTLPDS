/* ToMopedAnalysis class for Jimple To Moped conversion
 * Written by: Matthew Hague 
 * 
 * Basic structure based on the fooanalasys example from
 * 
 * 		A Survivor's Guide to Java Program Analysis with Soot
 * 		A. Einarsson and J. D. Nielsen
 */

package jimpletomoped.translation; 

import java.io.OutputStreamWriter;
import java.io.PrintWriter;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Stack;

import jimpletomoped.main.CmdOptions;

import jimpletomoped.representation.*;

import org.apache.log4j.Logger;

import soot.ArrayType;
import soot.Body;
import soot.BooleanType;
import soot.ByteType;
import soot.IntType;
import soot.JimpleBodyPack;
import soot.LongType;
import soot.NullType;
import soot.Pack;
import soot.PackManager;
import soot.Printer;
import soot.RefType;
import soot.Scene;
import soot.ShortType;
import soot.SootClass;
import soot.SootMethod;
import soot.Type;
import soot.VoidType;

import soot.jimple.Stmt;

import soot.toolkits.graph.ExceptionalUnitGraph;

import static jimpletomoped.representation.RemoplaStaticFactory.*;

public class ToMopedTranslator {	

    static Logger logger = Logger.getLogger(ToMopedTranslator.class);
    public int count=0;
	
	private StmtTranslator st;

    private static StmtLabeller l = new StmtLabeller() {
        public RemoplaStmt label(RemoplaStmt stmt) {
            // Do nothing -- we label non-java methods using labelmethod after
            // construction, java methods are labelled by the statement
            // translator
            return stmt;
        }
    };

    // The signatures of methods already translated
    private Collection<SootMethod> translatedMethods = new HashSet<SootMethod>();
    private Stack<SootMethod> worklist = new Stack<SootMethod>();
    // if we use a class, we need to run its static init method at the
    // beginning, collect them here
    private Collection<SootMethod> classStaticInits = new HashSet<SootMethod>();

    
    /** Translates the methods in the worklist into remopla.  This translates
     * everything required by the method too, returning a complete program
     * simulating the method call.  (Does not remember what methods were translated
     * during a previous call.)
     *  @param initMethod the initial method of the translation 
     *  @return the Remopla object corresponding to the method -- the main
     *  function will call the method appropriately. 
     */
    public Remopla translateMethods(SootMethod initMethod) {
		st = new StmtTranslator();
        translatedMethods.clear();
        classStaticInits.clear();        
        Remopla remopla;

        if (methodTranslatableAsInit(initMethod)) {
            addToWorklist(initMethod);
            st.addClass(initMethod.getDeclaringClass());

            remopla = createRemopla();

            while (!worklist.empty()) {
                SootMethod method = worklist.pop();
                if (!translatedMethods.contains(method)) {
                    if (ClassManager.isIgnored(method)) {
                        translateIgnoredMethod(method, remopla);
                    } else if (ClassManager.hasStub(method.getDeclaringClass())) {
                        translateStubbedMethod(method, remopla);
                    } else {
                        translateIncludedMethod(method, remopla);
                    }
                }            
            }

            completeRemopla(initMethod, remopla);
            tidyRemopla(remopla);
        } else {
            logger.error("Initial method is not suitable.");
            remopla = null;
        }

        // tidy up
        System.out.printf("\n\n number: %d \n\n",count);//
        st = null;
        translatedMethods.clear();
        classStaticInits.clear();

        return remopla;
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Private methods
    //

    /** Translate a user method, loading any methods it calls into the work
     * list, if they haven't been translated already.  If a problem occurs, the
     * method is translated as an ignored method.
     *  @param method the method to translate.
     *  @param remopla the remopla instance to add the code to
     */
    private void translateIncludedMethod(SootMethod method, Remopla remopla) {
        // make sure method/class is loaded
        loadClass(method.getDeclaringClass());

        // translate and record
        RemoplaMethod rMethod = translateMethod(method, remopla);
        if (rMethod != null) {
            remopla.addMethod(rMethod);
        } else {
            translateIgnoredMethod(method, remopla);
        }
        translatedMethods.add(method);

        // add required methods to worklist and reset
        for (SootMethod requiredMethod : st.getCalledMethods()) {
            // this check is made twice, perhaps more efficient if only
            // made once (i figure contains is quicker than an
            // unnecessary add).
            if (!translatedMethods.contains(requiredMethod)) {
                addToWorklist(requiredMethod);
            }
        }
        st.clearCalledMethods();

        for (SootClass sc : st.getStaticAccesses()) {
            loadClass(sc);
        }
        st.clearStaticAccesses();
    }
        

    /** Translate a method that has a stub -- that is, translate the stub
     * instead.  If a stub can't be found, treat the method as ignored.
     *   @param method the method to translate.
     *   @param remopla the current Remopla isntance
     */
    private void translateStubbedMethod(SootMethod method, Remopla remopla) {
        SootClass stubClass = loadClass(method.getDeclaringClass());
        
        // find the method
        SootMethod stubMethod = null;
        Iterator i = stubClass.getMethods().iterator();
        while (stubMethod == null && i.hasNext()) {
            SootMethod candidate = (SootMethod)i.next();
            if (Namer.getMethodName(candidate).equals(Namer.getMethodName(method))) {
                stubMethod = candidate;
            }
        }

        if (stubMethod != null) {
            translateIncludedMethod(stubMethod, remopla);
            // the stub method is not the same as the method being translated
            // so we record that method has been translated too
            translatedMethods.add(method);
        } else {
            translateIgnoredMethod(method, remopla);
        }
    }

    /** Translate an ignored method -- create a stub.  Currently the stub is
     * just a hardcoded return undef.
     *  @param method the method to translate.
     *  @param remopla the remopla instance to add the code to
     */
    private void translateIgnoredMethod(SootMethod method, Remopla remopla) {
        translatedMethods.add(method);

        RemoplaMethod rMethod = new RemoplaMethod(Namer.getMethodName(method));
        rMethod.appendStatement(new RemoplaReturnStmt());
        labelMethod(rMethod);
        remopla.addMethod(rMethod);

    }


    /** Translate the method.
     *   @param sm the method
     *   @param remopla the current remopla instance
     *   @return a RemoplaMethod representing the method, or null if it's not
     *   translatable (not concrete, doesn't have an active body, ...)
     */
    private RemoplaMethod translateMethod(SootMethod sm, Remopla remopla) {
        RemoplaMethod rMethod = null;

        if (sm.isConcrete()) {
            Body body = getMethodBody(sm);

            if (body != null) { 
                rMethod = new RemoplaMethod(Namer.getMethodName(sm));

                ExceptionalUnitGraph cug = new ExceptionalUnitGraph(body);
                Iterator si = cug.iterator();
                while (si.hasNext()) {
                    Stmt stmt = (Stmt)si.next();
                    st.translateStmt(stmt, rMethod, remopla, sm, cug);
                    List<RemoplaStmt> rstmts = (List<RemoplaStmt>)st.getResult();
                    for (RemoplaStmt rstmt : rstmts) {
                        if (rstmt != null) {
                        	count=count+1;
                            rMethod.appendStatement(rstmt);
                        }
                    }
                }
            }
		}
        
        return rMethod;
    }


    /** Returns the method body of a SootMethod.  This is where we do Jimple
     * optimisations to get a better method body than a straight call to
     * sm.getActiveBody would provide.
     *   @param method the method to get the body for
     *   @return the (optimised) method body, null if the method doesn't have a
     *   body
     */
    private Body getMethodBody(SootMethod sm) {
        Body body = null;

        if (sm.isConcrete()) {
            body = sm.retrieveActiveBody();
//            runSootBodyPacks(body);
        }

        return body;
    }


    /** Creates a remopla instance with various additionals required for the
     * translation from Jimple.
     *   @return the remopla instance
     */
    private Remopla createRemopla() {
        Remopla remopla = new Remopla();
        return remopla;
    }

    /** Completes the remopla class by adding the init00 method that calls all
     * clinits and non-dets the input.  Then assigns class ids as appropriate.
     *   3) initialise the classes with static init methods
     *   4) call the initial method
     */
    private void completeRemopla(SootMethod initMethod, Remopla remopla) {
        RemoplaMethod rinit = new RemoplaMethod(Namer.getRemoplaInitMethod());
        // initialise classes    
        for (SootMethod clinit : classStaticInits) {
            RemoplaCallStmt rcall = new RemoplaCallStmt(Namer.getMethodName(clinit));
            rinit.appendStatement(rcall);
        }

        addInitialMethodCall(initMethod, rinit, remopla);

        remopla.addMethod(rinit);
        remopla.setInitialMethod(rinit);

        labelMethod(rinit);

        ClassManager.finaliseClassData();
    }
    
    /** load a class and add it's static init to the worklist
     *   @return the loaded class (if a stub is available, the return will be the
     *   stub class rather than the given class)
     */ 
    private SootClass loadClass(SootClass sc) {
        String className = Namer.getConcreteClassName(sc);

        sc = Scene.v().loadClassAndSupport(className);
        sc.setApplicationClass();
        
//        PackManager.v().runPacks();

        // search for static init, add to worklist if required
        for (Object mobj : sc.getMethods()) {
            SootMethod method = (SootMethod)mobj;
            if (method.getSubSignature().equals(Namer.getClinitSubSig())) {
                if (!classStaticInits.contains(method)) {
                    addToWorklist(method);
                    classStaticInits.add(method);
                }
            }
        }
    
        return sc;
    }


    /** Add a method to the worklist.  Call translateMethods() to get the
     * Remopla code corresponding to the translations of the methods in the
     * worklist.
     *   @param sm the method to add, non-null.
     */
    private void addToWorklist(SootMethod sm) {
        assert sm != null;
        worklist.push(sm);
    }

    private void clearWorklist() {
        worklist.clear();
    }

    /** @return true if method is suitable for an initial method -- that is,
     * always...
     */
    private boolean methodTranslatableAsInit(SootMethod method) {
        boolean translatable = true;
        return translatable;
    }


    /** Adds the code for the initial method call 
     *  @param initMethod the initial method to call
     *  @param rinit the initial remopla method to add statements to
     *  @param remopla the current remopla instance
     */
    private void addInitialMethodCall(SootMethod initMethod, 
                                             RemoplaMethod rinit, 
                                             Remopla remopla) {
        RemoplaCallStmt rcall = new RemoplaCallStmt(Namer.getMethodName(initMethod));
        rinit.appendStatement(rcall);
    }


    private void labelMethod(RemoplaMethod method) {
        List<RemoplaStmt> stmts = new ArrayList();
        method.getFlattenedStmts(stmts);
        for(RemoplaStmt stmt : stmts) {
            if (stmt.getLabel() == null) {
                stmt.setLabel(Namer.getNewLabel(method.getName()));
            }
           
        }
    }


    private void tidyRemopla(Remopla remopla) {
        RemoplaCleaner cleaner = new RemoplaCleaner();
        cleaner.cleanRemopla(remopla);
    }


    /** A bit of a mess: ideally Soot would do this at startup, but it doesn't
     * know which classes are application classes, so we either have to do a
     * pre-run to determine all classes that will need to be optimised and
     * tagged, or we can do it as we load each body.  
     *
     * TODO: Ultimately we want to apply whole program optimisations, so we need
     * to sort out the problem about not having a main method.  We will probably
     * have to do a pre-run to get all application classes.  Basically a mess.
     * Great.
     */
    private void runSootBodyPacks(Body body) {
        PackManager.v().getPack("jtp").apply(body);
        PackManager.v().getPack("jop").apply(body);
        PackManager.v().getPack("jap").apply(body);
    }

}
