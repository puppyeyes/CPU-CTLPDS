// SootClassPreloader.java
// Author: Matthew Hague
//
// Does some handy soot things that enable us to play nicely with the soot
// phases, to a limited extent...
//
// Currently: sets application classes and replaces libraries with stubs

package jimpletomoped.translation;

import java.util.Collection;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Map;
import java.util.Stack;

import org.apache.log4j.Logger;

import soot.Body;
import soot.BodyTransformer;
import soot.Scene;
import soot.SootClass;
import soot.SootMethod;
import soot.TypeSwitch;
import soot.RefType;
import soot.ValueBox;
import soot.Value;

import soot.jimple.AbstractExprSwitch;
import soot.jimple.AbstractStmtSwitch;
import soot.jimple.InterfaceInvokeExpr;
import soot.jimple.InvokeStmt;
import soot.jimple.SpecialInvokeExpr;
import soot.jimple.StaticInvokeExpr;
import soot.jimple.Stmt;
import soot.jimple.VirtualInvokeExpr;
import soot.jimple.StaticFieldRef;
import soot.jimple.ClassConstant;

import soot.toolkits.graph.ExceptionalUnitGraph;

public class SootClassPreloader {
    static Logger logger = Logger.getLogger(SootClassPreloader.class);

    private StmtHandler sHandler = new StmtHandler();
    private ExprHandler eHandler = new ExprHandler();
    private TypeHandler tHandler = new TypeHandler();

    // The signatures of methods already translated
    private Collection<SootMethod> loadedMethods = new HashSet<SootMethod>();
    private Collection<SootClass> loadedClasses = new HashSet<SootClass>();
    private Stack<SootMethod> worklist = new Stack<SootMethod>();
    private Collection<SootMethod> polymorphicMethods = new HashSet<SootMethod>();

    /**
     * @param methodSig
     *            the signature of the method to pre-load
     * @param scene
     *            the soot scene to load into
     * 
     *            Performs a pass over the code, from the given method, to make
     *            sure that all the classes that the model checker will use are
     *            loaded, and set to application classes, with due respect to
     *            stubs.
     * 
     *            Logs an error and returns if the method could not be found.
     */
    public void preloadMethodAndSupport(String methodSig) {
        loadStandardSupport();
        SootMethod sm = ClassManager.loadMethod(methodSig);
        if (sm == null) {
            logger.error("SootClassPreloader could not find method [" + methodSig + "]");
        } else {
            loadedMethods.clear();
            clearWorklist();
            addToWorklist(sm);

            depleteWorklist();

            loadedMethods.clear();
        }
    }

    // ////////////////////////////////////////////////////////////////////////////////////////////
    // Private methods
    //
    
    private void loadStandardSupport() {
        ClassManager.loadStubs();
    }

    private void depleteWorklist() {
        while (!worklist.empty()) {
            SootMethod method = worklist.pop();
            if (!loadedMethods.contains(method)) {
                if (ClassManager.isIgnored(method)) {
                    preloadIgnoredMethod(method);
                } else if (ClassManager.hasStub(method.getDeclaringClass())) {
                    preloadStubbedMethod(method);
                } else {
                    preloadIncludedMethod(method);
                }
            }
        }
    }

    // //////////////////////////////////////////////////////////////////////////////////
    // Private methods
    //

    private void preloadIncludedMethod(SootMethod method) {
        // make sure method/class is loaded
        loadClass(method.getDeclaringClass());

        // translate and record
        preloadMethod(method);
        loadedMethods.add(method);
    }

    private void preloadStubbedMethod(SootMethod method) {
        // find the method
        SootMethod stubMethod = ClassManager.getMethodStub(method);
        if (stubMethod != null) {
            preloadIncludedMethod(stubMethod);
            // the stub method is not the same as the method being translated
            // so we record that method has been translated too
            loadedMethods.add(method);
        } else {
            preloadIgnoredMethod(method);
        }
    }

    private void preloadIgnoredMethod(SootMethod method) {
        // make sure method/class is loaded
        loadedMethods.add(method);
    }

    private void preloadMethod(SootMethod sm) {
        if (sm.isConcrete()) {
            Body body = getMethodBody(sm);
            if (body != null) {
                ExceptionalUnitGraph cug = new ExceptionalUnitGraph(body);
                Iterator si = cug.iterator();
                while (si.hasNext()) {
                    Stmt stmt = (Stmt) si.next();
                    stmt.apply(sHandler);
                }
            }
        }
    }

    /**
     * Returns the method body of a SootMethod. This is where we do Jimple
     * optimisations to get a better method body than a straight call to
     * sm.getActiveBody would provide.
     * 
     * @param method
     *            the method to get the body for
     * @return the (optimised) method body, null if the method doesn't have a
     *         body
     */
    private Body getMethodBody(SootMethod sm) {
        Body body = null;

        if (sm.isConcrete()) {
            body = sm.retrieveActiveBody();
        }

        return body;
    }

    /**
     * Add a method to the worklist. Call translateMethods() to get the Remopla
     * code corresponding to the translations of the methods in the worklist.
     * 
     * @param sm
     *            the method to add, non-null.
     */
    private void addToWorklist(SootMethod sm) {
        assert sm != null;
        worklist.push(sm);
    }

    private void clearWorklist() {
        worklist.clear();
    }


    private void addPolymorphicMethod(SootMethod m) {
        polymorphicMethods.add(m);

        for (SootClass sc : loadedClasses) {
            SootMethod implementation = ClassManager.getImplementation(sc, m);
            if (implementation != null && !loadedMethods.contains(implementation)) {
                addToWorklist(implementation);
            }
        }
    }

    private void clearPolymorphicMethods() {
        polymorphicMethods.clear();
    }

    private void loadClass(SootClass sc) {
        if (!loadedClasses.contains(sc)) {
            ClassManager.loadClass(sc.getName());

            // if the class implements any polymorphic calls, we need to add the
            // implementing methods to the worklist.

            for (SootMethod sm : polymorphicMethods) {
                SootMethod implementation = ClassManager.getImplementation(sc, sm);
                if (implementation != null) {
                    addToWorklist(implementation);
                }
            }

            // search for static init, add to worklist if required
            for (Object mobj : sc.getMethods()) {
                SootMethod method = (SootMethod)mobj;
                if (method.getSubSignature().equals(Namer.getClinitSubSig())) {
                    if (!loadedMethods.contains(method)) {
                        addToWorklist(method);
                    }
                }
            }

            loadedClasses.add(sc);
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // Private methods
    //

    private class StmtHandler extends AbstractStmtSwitch {

        /**
         * {@inheritDoc}
         * 
         * @see soot.jimple.StmtSwitch#caseInvokeStmt(InvokeStmt)
         */
//        public void caseInvokeStmt(InvokeStmt is) {
//            is.getInvokeExpr().apply(eHandler);
//        }

        /**
         * {@inheritDoc}
         * @see AbstractStmtSwitch#defaultCase(Object)
         */
        public void defaultCase (Object obj) {
            if (obj instanceof Stmt) {
                Stmt s = (Stmt)obj;
                for (Object o : s.getUseAndDefBoxes()) {
                    Value v = ((ValueBox)o).getValue();
                    if (v instanceof ClassConstant) {
                        String className = ((ClassConstant)v).getValue();
                        if (!className.startsWith("[")) {
                            loadClass(ClassManager.loadClassWithSlashes(className));
                        }
                    }
                }
                if (s.containsInvokeExpr()) {
                    s.getInvokeExpr().apply(eHandler);
                } else if (s.containsFieldRef()) {
                    s.getFieldRef().getType().apply(tHandler);
                    loadClass(s.getFieldRef().getField().getDeclaringClass());
                }
            }
        }
    }

    private class ExprHandler extends AbstractExprSwitch {

        /**
         * {@inheritDoc}
         * 
         * @see soot.jimple.ExprSwitch#caseInterfaceInvokeExpr(InterfaceInvokeExpr)
         */
        public void caseInterfaceInvokeExpr(InterfaceInvokeExpr iie) {
            addPolymorphicMethod(iie.getMethod());
        }

        /**
         * {@inheritDoc}
         * 
         * @see soot.jimple.ExprSwitch#caseSpecialInvokeExpr(SpecialInvokeExpr)
         */
        public void caseSpecialInvokeExpr(SpecialInvokeExpr sie) {
            addToWorklist(sie.getMethod());
        }

        /**
         * {@inheritDoc}
         * 
         * @see soot.jimple.ExprSwitch#caseStaticInvokeExpr(StaticInvokeExpr)
         */
        public void caseStaticInvokeExpr(StaticInvokeExpr sie) {
            addToWorklist(sie.getMethod());
        }

        /**
         * {@inheritDoc}
         * 
         * @see soot.jimple.ExprSwitch#caseVirtualInvokeExpr(VirtualInvokeExpr)
         */
        public void caseVirtualInvokeExpr(VirtualInvokeExpr vie) {
            addToWorklist(vie.getMethod());
            addPolymorphicMethod(vie.getMethod());
        }

    }



    private class TypeHandler extends TypeSwitch {
        public void caseRefType(RefType ref) {
            loadClass(ref.getSootClass());
        }
    }

}
