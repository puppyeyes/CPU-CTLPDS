// InvokeTranslator.java
// Author: Matthew Hague
//
// A class for converting Value to RemoplaMethodCall
// Checks that Value is an InvokeExpr



package jimpletomoped.translation;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import jimpletomoped.main.CmdOptions;

import jimpletomoped.representation.*;
import jimpletomoped.translation.StmtLabeller;

import org.apache.log4j.Logger;

import soot.ArrayType;
import soot.Local;
import soot.RefType;
import soot.SootClass;
import soot.SootField;
import soot.SootMethod;
import soot.Value;

import soot.jimple.AbstractJimpleValueSwitch;
import soot.jimple.InstanceInvokeExpr;
import soot.jimple.InterfaceInvokeExpr;
import soot.jimple.InvokeExpr;
import soot.jimple.ParameterRef;
import soot.jimple.SpecialInvokeExpr;
import soot.jimple.StaticInvokeExpr;
import soot.jimple.VirtualInvokeExpr;

import static jimpletomoped.representation.RemoplaStaticFactory.*;
import jimpletomoped.translation.ExprTranslator.DefUses;


public class InvokeTranslator extends AbstractJimpleValueSwitch {

    static Logger logger = Logger.getLogger(InvokeTranslator.class);

	private ExprTranslator et;

    private class MethodCall {
        private RemoplaMutableStmt callStmt;
        private RemoplaBranchStmt branchStmt;
        private DefUses dus;
        private SootMethod method;
        private boolean implemented;
        private StmtLabeller l;

        // Create a method call, creates and manages its own call statement that
        // should be used by the translator
        // Clones the StmtLabeller so that labelling is consistent for
        // statements added later
        public MethodCall(InvokeExpr invoke, StmtLabeller l) {
            assert invoke != null && l != null;
            this.method = invoke.getMethod();
            this.branchStmt = null;
            RemoplaStmt call = getCallStatement(null);
            this.dus = getParamDefUses(invoke);
            addDefUse(call, dus);
            this.callStmt = new RemoplaMutableStmt(call);
            this.implemented = false;
            this.l = (StmtLabeller)l.clone();
        }

        public RemoplaStmt getCallStmt() {
            return callStmt;
        }

        public SootMethod getMethod() {
            return method;
        }

        /** @param sc the class with the new implementing method
         * @param m the implementing method (due to inheritance, this may not have
         * m.getDeclaringClass().equals(sc)
         */
        public void addImplementingMethod(SootClass sc, SootMethod m) {
            String callString = Namer.getMethodName(m);
            RemoplaStmt call = getCallStatement(callString);
            addDefUse(call, dus);

            if (!implemented) {
                call.setLabel(callStmt.getLabel());
                callStmt.setStmt(call);
                implemented = true;
            } else if (branchStmt == null) {
                l.label(call);

                RemoplaStmt elseStmt = callStmt.getStmt();

                branchStmt = new RemoplaBranchStmt();
                branchStmt.setLabel(elseStmt.getLabel());
                elseStmt.setLabel(null);
                l.label(elseStmt);
                branchStmt.addBranch(call);
                branchStmt.addBranch(elseStmt);

                callStmt.setStmt(branchStmt);
            } else {
                l.label(call);
                branchStmt.addBranch(call);
            }
        }
    }

    private SootMethod currentSootMethod;
    private RemoplaMethod currentMethod;
    private Remopla currentRemopla;	
    private String ignoredComment;
    private StmtLabeller l;

    private Collection<SootMethod> calledMethods = new HashSet<SootMethod>();

    private Map<SootMethod, List<MethodCall>> polyCallStatements = new HashMap<SootMethod, List<MethodCall>>();
    private Collection<SootClass> presentClasses = new HashSet<SootClass>();

    public InvokeTranslator() {
        et = new ExprTranslator();
	}

    /** translates an invoke/assignment expression to Moped.  If recordArrayAccesses or
      * recordObjectAccesses are set, then the corresponding information is recorded
      * and can be obtained by getArrayAccesses and getObjectAccesses.
      *  @param expr the expression to translate, must be an invoke expr
      *  @param method the method the expression is located in (for local vars)
      *  @param remopla the program the expression is in (for globals)
      *  @param labelBody during processing, we may have to add new statements.
      *  To add a label to these, pass a non-null label body (calls
      *  Namer.getNewLabel(labelBody)
      *  @return none -- RemoplaStmt handling polymorphism.  Note: does not handle exceptions.
      */ 
	public void translateInvoke(Value expr, 
                                SootMethod currentSootMethod,
                                RemoplaMethod method, 
                                Remopla remopla,
                                StmtLabeller l) {
        assert expr != null && method != null && remopla != null && l != null;
        currentMethod = method;
        currentRemopla = remopla;
        this.currentSootMethod = currentSootMethod;
        this.l = l;

		expr.apply(this);
	}

    public Collection<SootMethod> getCalledMethods() {
        return calledMethods;
    }

    public void clearCalledMethods() {
        calledMethods.clear();
    }


    /////////////////////////////////////////////////////////////////////////////////////
    // Translate the call
    //

    public void caseStaticInvokeExpr(StaticInvokeExpr invoke) {
        ignoredComment = "Ignored Static Invoke: ";
        translateStaticallyBoundInvoke(invoke);
    }

    public void caseSpecialInvokeExpr(SpecialInvokeExpr invoke) {
        ignoredComment = "Ignored Special Invoke: ";
        translateStaticallyBoundInvoke(invoke);
    }

    public void caseVirtualInvokeExpr(VirtualInvokeExpr invoke) {
        ignoredComment = "Ignored Virtual Invoke: ";
        translateDynamicallyBoundInvoke(invoke);
    }

    public void caseInterfaceInvokeExpr(InterfaceInvokeExpr invoke) {
        ignoredComment = "Ignored Interface Invoke: ";
        translateDynamicallyBoundInvoke(invoke);
    }


    /** Whenever a new class is brought into use, make sure that all the
     * previous call statements know that the call may polymorphically go to the new
     * class
     */
    public void addClass(SootClass sc) {
        if (!presentClasses.contains(sc)) {
            for (SootMethod sm : polyCallStatements.keySet()) {
                SootMethod implementation = ClassManager.getImplementation(sc, sm);
                if (implementation != null) {
                    for (MethodCall mc : polyCallStatements.get(sm)) {
                        addPolymophicCall(mc, sc, implementation);
                    }
                }
            }
            presentClasses.add(sc);
        }
    }

    public void addClasses(Collection<SootClass> scs) {
        for (SootClass sc : scs) {
            addClass(sc);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////
    // Default
    //

    public void defaultCase(Object expr) {
        logger.error("Expression feature [" + expr + " : " + expr.getClass() + "] is not a method call");
        registerResult(null);
    }



    ////////////////////////////////////////////////////////////////////////////////////
    // Private methods
    //



    // Pass a null thisRef if static method
    private void translateStaticallyBoundInvoke(InvokeExpr invoke) {
        // record the method call
        recordMethodCall(invoke.getMethod());

        // translate
        String callString = Namer.getMethodName(invoke.getMethod());
        RemoplaStmt call = getCallStatement(callString);
        addDefUse(call, getParamDefUses(invoke));

        registerResult(call);
    }



    private void translateDynamicallyBoundInvoke(InstanceInvokeExpr invoke) {
        ignoredComment += Namer.getMethodName(invoke.getMethod());

        MethodCall call = new MethodCall(invoke, l);
        addPresentPolymorphicCalls(call);
        addPolyCall(call);

        registerResult(call.getCallStmt());
    }





    private void recordMethodCall(SootMethod sm) {
        calledMethods.add(sm);
    }




    private RemoplaStmt getCallStatement(String callString) {
        RemoplaStmt callStatement = null;
        if (callString == null) {
            callStatement = new RemoplaSkipStmt();
            callStatement.setComment(ignoredComment);
        } else {
            callStatement = new RemoplaCallStmt(callString);
        }
        return l.label(callStatement);
    }

    private void addPresentPolymorphicCalls(MethodCall mc) {
        for (SootClass c : presentClasses) {
            SootMethod implementation = ClassManager.getImplementation(c, mc.getMethod());
            if (implementation != null) {
                addPolymophicCall(mc, c, implementation);
            }
        }
    }

    /** @param mc the method call object being added to
     * @param sc the class with the implementation (may not be the same as
     * sm.getDeclaringClass -- inheritance)
     * @param sm the implementing method
     */
    private void addPolymophicCall(MethodCall mc, SootClass sc, SootMethod sm) {
        recordMethodCall(sm);
        mc.addImplementingMethod(sc, sm);
    }



    private void registerResult(RemoplaStmt callStmt) {
        setResult(callStmt);
    }

    private void addPolyCall(MethodCall mc) {
        SootMethod m = mc.getMethod();
        
        List<MethodCall> calls = polyCallStatements.get(m);

        if (calls == null) {
            calls = new ArrayList<MethodCall>();
            polyCallStatements.put(m, calls);
        }

        calls.add(mc);
    }


    private DefUses doExpr(Value expr, boolean defining) {
        et.analyseExpr(expr, currentSootMethod, defining);
        addClasses(et.getStaticAccesses());
        return (DefUses)et.getResult();
    }

    private void addDefUse(RemoplaStmt s, DefUses du) {
        s.addUses(du.getUses());
        s.addDefs(du.getDefs());
    }

    private DefUses getParamDefUses(InvokeExpr invoke) {
        DefUses du = et.getEmptyDefUses();
        for (int i = 0; i < invoke.getArgCount(); i++) {
            du.addDefUses(doExpr(invoke.getArg(i), false));
        }
        return du;
    }
}

