/* ToMopedAnalysis class for Jimple To Moped conversion
 * Written by: Matthew Hague 
 * 
 * Basic structure based on the fooanalasys example from
 * 
 * 		A Survivor's Guide to Java Program Analysis with Soot
 * 		A. Einarsson and J. D. Nielsen
 *
 * use getResult() to get the result of the translation -- if this is null, then
 * no extra Remopla statement is necessary.
 */

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
import jimpletomoped.representation.RemoplaVar;

import org.apache.log4j.Logger;

import soot.ArrayType;
import soot.IntType;
import soot.Local;
import soot.RefType;
import soot.SootClass;
import soot.SootMethod;
import soot.Trap;
import soot.Type;
import soot.Unit;
import soot.Value;

import soot.jimple.AbstractStmtSwitch;
import soot.jimple.AssignStmt;
import soot.jimple.DefinitionStmt;
import soot.jimple.GotoStmt;
import soot.jimple.IdentityStmt;
import soot.jimple.IfStmt;
import soot.jimple.InvokeExpr;
import soot.jimple.InvokeStmt;
import soot.jimple.LookupSwitchStmt;
import soot.jimple.NewArrayExpr;
import soot.jimple.NewExpr;
import soot.jimple.NopStmt;
import soot.jimple.ParameterRef;
import soot.jimple.RetStmt;
import soot.jimple.ReturnStmt; 
import soot.jimple.ReturnVoidStmt;
import soot.jimple.Stmt;
import soot.jimple.TableSwitchStmt;
import soot.jimple.ThrowStmt;

import soot.tagkit.Tag;

import soot.toolkits.graph.ExceptionalUnitGraph.ExceptionDest;

import soot.toolkits.graph.ExceptionalUnitGraph;

import static jimpletomoped.representation.RemoplaStaticFactory.*;
import jimpletomoped.translation.ExprTranslator.DefUses;

public class StmtTranslator extends AbstractStmtSwitch {

    private static final String NOT_IMPLEMENTED = " has not been implemented.";

    static Logger logger = Logger.getLogger(StmtTranslator.class);

	private ExprTranslator et;
    private InvokeTranslator it;

    private RemoplaMethod currentMethod = null;
    private Remopla currentRemopla = null;   
    private SootMethod currentSootMethod = null;
    private Stmt currentStmt = null;
    private ExceptionalUnitGraph currentCug = null;


    private StmtTransLabeller l = new StmtTransLabeller();

    // Map statements to their Remopla translations
    // This allows us to add labels to statements as required
    // (A Jimple jump statement only contains references to the target Stmt
    // not any label).  If a label is required before the statement is
    // translated, we create a RemoplaNullStmt as a placeholder.
    // Object is used rather than Stmt because soot sometimes passes statements 
    // as Units or Objects
    private Map<Object, RemoplaStmt> translatedStmts = new HashMap<Object, RemoplaStmt>();

    // Store called methods so that we can translate them all eventually
    private Collection<SootMethod> calledMethods = new HashSet<SootMethod>();

	public StmtTranslator() {
		et = new ExprTranslator();
        it = new InvokeTranslator();
	}



    /** Translate a statement to Remopla.  If the statement refers to local
      * variables &c., pass the method it refers to.  During translation, local
      * or global variable may be added to the parameters, as required.
      *  @param stmt statement to translate
      *  @param method method any local references are with respect to
      *  @param remopla Remopla instance the code will belong to (for global
      *  vars)
      *  @param sootMethod the SootMethod object holding the statement
      *  @param cug The ExceptionalUnitGraph object of the program being
      *  @param label Whether to automatically label the translated statement (default
      *  true).  An if statement, for example, will result in several
      *  statements, all labelled if label is true.
      *  translated
      */
	public void translateStmt(Stmt stmt, 
                              RemoplaMethod method, Remopla remopla, 
                              SootMethod sootMethod, ExceptionalUnitGraph cug,
                              boolean label) {
        assert method != null && 
               remopla != null && 
               cug != null && 
               sootMethod != null;

        currentMethod = method;
        currentRemopla = remopla;
        currentSootMethod = sootMethod;
        currentCug = cug;
        currentStmt = stmt;
        l.setLabel(label);
        l.setMethodStmt(method, stmt);

		stmt.apply(this);
	}

	public void translateStmt(Stmt stmt, 
                              RemoplaMethod method, Remopla remopla, 
                              SootMethod sootMethod, ExceptionalUnitGraph cug) {
        translateStmt(stmt, method, remopla, sootMethod, cug, true);
    }

    /** The methods called in some way since the statement translator has been
     * translating statements.  Call clearCalledMethods() to prevent aggregation.
     */
    public Collection<SootMethod> getCalledMethods() { 
        return calledMethods;
    }

    /** The statement translated collates the signatures of the methods that
     * translated statements have called.  This clears the record.
     */
    public void clearCalledMethods() {
        calledMethods.clear();
    }

    /** The SootClasses who've had static fields accessed since the statement translator has been
     * translating statements.  Call clearStaticAccesses() to prevent aggregation.
     */
    public Collection<SootClass> getStaticAccesses() { 
        return et.getStaticAccesses();
    }

    /** The statement translated collates the classes who've had static members
     * referenced.  This clears the record.
     */
    public void clearStaticAccesses() {
        et.clearStaticAccesses();
    }


    /** To translate invoke assignments properly, the translator must know which
     * classes are being used by the program.  These can be deduced from the
     * explicit "new" statements in the program, but if a class may appear without
     * an explicit new, this method should be used to keep the translator informed.
     */
    public void addClass(SootClass sc) {
        it.addClass(sc);
    }

    public Remopla getCurrentRemopla() {
        return currentRemopla;
    }

    //////////////////////////////////////////////////////////////////////////////////////
    // Call back case handlers
    //

    public void caseAssignStmt(AssignStmt stmt) {
        registerResultList(stmt, getDefinitionStmts(stmt));
    }	


    public void caseIdentityStmt(IdentityStmt id) {
        if (id.getLeftOp() instanceof Local && 
            id.getRightOp() instanceof ParameterRef) {
            registerResult(id, null);
        } else {
            registerResultList(id, getDefinitionStmts(id));
        }
    }


    public void caseIfStmt(IfStmt stmt) {
        String targetLabel = getAddStatementLabel(stmt.getTarget());

        RemoplaBranchStmt rb = rbranch(l.label(rgoto(targetLabel)), 
                                       l.label(rskip()));
        l.label(rb);
        addDefUse(rb, doExpr(stmt.getCondition(), false));

        registerResult(stmt, rb);
    }


    // Because i don't know what this is...
    public void caseRetStmt(RetStmt ret) {
        logger.warn("StmtTranslator encountered a RetStmt -- this is not supposed to happen.");
        registerResult(ret, l.label(rnull("Unexpected RetStmt -- return from a JSR.")));
    }

    public void caseReturnStmt(ReturnStmt ret) {
        List<RemoplaStmt> rstmts = new ArrayList<RemoplaStmt>(2);

        RemoplaStmt r = l.label(rreturn());
        addDefUse(r, doExpr(ret.getOp(), false));
        rstmts.add(r);

        registerResultList(ret, rstmts);
    }

    public void caseReturnVoidStmt(ReturnVoidStmt ret) {
        List<RemoplaStmt> rstmts = new ArrayList<RemoplaStmt>(2);
        rstmts.add(l.label(rreturn()));
        registerResultList(ret, rstmts);
    }


    public void caseGotoStmt(GotoStmt stmt) {
        String gotoLabel = getAddStatementLabel((Stmt)stmt.getTarget());
        registerResult(stmt, l.label(rgoto(gotoLabel)));
    }


    public void caseNopStmt(NopStmt stmt) {
        registerResult(stmt, l.label(rskip()));
    }


    /** A lookup switch behaves like a java case statement with the added bonus
      * that the cases are distinct (n == 1) or (n == 2) &c.  This means we do
      * not have to worry about the non-determinism between overlapping cases
      * in Remopla's if statements
      */
    public void caseLookupSwitchStmt(LookupSwitchStmt stmt) {
        RemoplaBranchStmt rswitch = (RemoplaBranchStmt)l.label(rbranch());

        // if the key is undefined, we could choose any case
        for(int i = 0; i < stmt.getTargetCount(); i++) {
            String dest = getAddStatementLabel((Stmt)stmt.getTarget(i));
            rswitch.addBranch(l.label(rgoto(dest)));
        }      
        if (stmt.getDefaultTarget() != null) {
            String dest = getAddStatementLabel((Stmt)stmt.getDefaultTarget());
            rswitch.addBranch(l.label(rgoto(dest)));
        }

        addDefUse(rswitch, doExpr(stmt.getKey(), false));

        registerResult(stmt, rswitch);
    }


    /** A table switch is of the form switch (n) low high { case 1: case 2:  default }
      * where case 1 fires if n = low + 1, case 2 if n = low + 2, &c.  Default
      * if no matches
      */
    public void caseTableSwitchStmt(TableSwitchStmt stmt) {
        int low = stmt.getLowIndex();
        int ncases = stmt.getHighIndex() - low + 1;

        RemoplaBranchStmt rswitch = (RemoplaBranchStmt)l.label(rbranch());

        for(int i = 0; i < ncases; i++) {
            String dest = getAddStatementLabel((Stmt)stmt.getTarget(i));
            rswitch.addBranch(l.label(rgoto(dest)));
        }      
        if (stmt.getDefaultTarget() != null) {
            String dest = getAddStatementLabel((Stmt)stmt.getDefaultTarget());
            rswitch.addBranch(l.label(rgoto(dest)));
        }

        addDefUse(rswitch, doExpr(stmt.getKey(), false));

        registerResult(stmt, rswitch);
    }


    /** When an invoke calls a method that may throw an exception, we need to
     * check, on returning, whether an exception has been raised.  This will be the
     * case if the exception variable is not null.
     *
     * Plan:
     *   do the invoke
     *   check the global, if exception, propagate
     */
    public void caseInvokeStmt(InvokeStmt invoke) {
        List<RemoplaStmt> rstmts = new ArrayList<RemoplaStmt>();

        RemoplaStmt rinvoke = getInvokeExpression(invoke.getInvokeExpr());
        if (rinvoke == null) {
            rstmts.add(l.label(rnull("[Invoke undefined : " + invoke + "]")));
        } else {
            l.label(rinvoke);
            // Call the method
            rstmts.add(rinvoke);
            rstmts.add(getExceptionBranchStmt(invoke));
        }

        registerResultList(invoke, rstmts);
    }


    /** Handles throw statements.  When labelling the statement, we check
     * whether this line is throwing an assertion error.  If so, we add a marker
     * to the label to indicate something bad has happened.
     */
    public void caseThrowStmt(ThrowStmt stmt) {
        List<RemoplaStmt> rstmts = new ArrayList<RemoplaStmt>();
        // first add a statement to mark the usedefs
        RemoplaStmt thrower = l.label(rskip());
        addDefUse(thrower, doExpr(stmt.getOp(), false));
        rstmts.add(thrower);
        rstmts.add(getExceptionBranchStmt(stmt)); 
        registerResultList(stmt, rstmts);
    }

       

    public void defaultCase(Object obj) {
        logger.warn("Statement feature [" + obj + " : " + obj.getClass() + "]" + NOT_IMPLEMENTED);
        if (obj instanceof Stmt) {
            RemoplaStmt rstmt = l.label(rnull("[Statement class " + obj.getClass() + " not translated]"));
            registerResult((Stmt)obj, rstmt);
        }
    }



    //////////////////////////////////////////////////////////////////////////////////////
    // Private definitions
    //
    

    private RemoplaStmt getInvokeExpression(Value expr) {
        it.translateInvoke(expr, currentSootMethod, currentMethod, currentRemopla, l);        

        calledMethods.addAll(it.getCalledMethods());
        it.clearCalledMethods();

        return (RemoplaStmt)it.getResult();
    }

    /** @param def definition to translate
     * @return the list of statements required to handle to definition
     */
    private List<RemoplaStmt> getDefinitionStmts(DefinitionStmt def) {
        Value lhs = def.getLeftOp();
        Value rhs = def.getRightOp();

        List<RemoplaStmt> rstmts = new ArrayList<RemoplaStmt>();

        if (rhs instanceof NewExpr) {
            rstmts = getNewObjectAllocation(lhs, (NewExpr)rhs);
        } else {
            if (rhs instanceof InvokeExpr) {
                RemoplaStmt rrhs = getInvokeExpression(rhs);
                if (rrhs != null) {
                    // do invoke/assign
                    rstmts.add(l.label(rrhs));
                    // Check for any exceptions and switch call and nullException
                    // labels (so goto statements jump to the right place)
                    rstmts.add(getExceptionBranchStmt(def));
                    // final assign
                    RemoplaStmt assign = l.label(rskip());
                    DefUses dus = doExpr(lhs, true);
                    addDefUse(assign, dus);
                    rstmts.add(assign);
                } else {
                    logger.error("InvokeExpr not translated to a method call in StmtTranslator.getDefinitionStmts");
                    rstmts.add(l.label(rskip()));
                }
            } else {
                RemoplaStmt assign = l.label(rskip());
                addDefUse(assign, doExpr(lhs, true));
                addDefUse(assign, doExpr(rhs, false));
                rstmts.add(assign);
            }
        }

        return rstmts;
    }

    /** Returns the statements required to allocate a new object and any
     * additional bookkeeping required for new objects.  Ultimately,
     * this will be a heap allocation, but for now it's just array fields.
     *   @param rlhs the remopla variable to assign to
     *   @param rhs the new object expression to translate
     *   @return the list of statements performing the allocation
     */
    private List<RemoplaStmt> getNewObjectAllocation(Value lhs, NewExpr rhs) {
        SootClass sType = rhs.getBaseType().getSootClass();

        // First inform the invoke translator, so it can update its polymorphic
        // calls to handle the new class
        it.addClass(sType);
        calledMethods.addAll(it.getCalledMethods());
        it.clearCalledMethods();

        List<RemoplaStmt> rstmts = new ArrayList();
        RemoplaStmt assign = l.label(rskip());
        addDefUse(assign, doExpr(lhs, true));
        addDefUse(assign, doExpr(rhs, false));
        rstmts.add(assign);

        return rstmts;
    }





    /** Looks up the current statement and returns its label, creating one if
      * needed.  The created label assumes that the statement will appear in the
      * current method being translated, so will add the current method name to
      * aid readability.
      *  @param stmt the soot statement in question (may be a Unit or Object)
      *  @return the statement label
      */
    private String getAddStatementLabel(Stmt stmt) {
        RemoplaStmt rstmt = translatedStmts.get(stmt);

        if (rstmt == null) {
            rstmt = rnull("Label placeholder.  Created by ToMopedTranslator.getAddStatementLabel().");
            translatedStmts.put(stmt, rstmt);
        }

        if (rstmt.getLabel() == null) {
            rstmt.setLabel(Namer.getNewStmtLabel(stmt, currentMethod.getName()));
        }

        return rstmt.getLabel();
    }


    /** After a statement has been translated, register it with the translator
      * This allows labels to be added before and after the fact.  If before,
      * there will be a RemoplaNullStmt mapped to stmt holding its place. 
      *
      * The result of a statement translation is a list of statements.  This
      * function sets the result to a list containing a single statement.
      *
      *  @param stmt the soot statement
      *  @param rstmt the remopla statement
      */
    private void registerResult(Stmt stmt, RemoplaStmt rstmt) {
        List<RemoplaStmt> rstmts = new ArrayList<RemoplaStmt>();
        rstmts.add(rstmt);
    
        registerResultList(stmt, rstmts);
    }

    /** After a statement has been translated, register it with the translator
      * This allows labels to be added before and after the fact.  If before,
      * there will be a RemoplaNullStmt mapped to stmt holding its place. 
      *
      * The result of a statement translation is a list of statements.  This
      * function sets the result to the given list (the first statement in the
      * list will have the label for the statement, if required). 
      *
      *  @param stmt the soot statement
      *  @param rstmt the remopla statement
      */
    private void registerResultList(Stmt stmt, List<RemoplaStmt> rstmts) {
        if (rstmts.size() > 0) {
            RemoplaStmt firstStatement = rstmts.get(0);
            RemoplaStmt lastStatement = rstmts.get(rstmts.size() - 1);

            if (firstStatement != null) {
                registerStatement(stmt, firstStatement);
            }
        }

        setResult(rstmts);
    }


    /** Register the first Remopla statement corresponding to a Jimple statement.
     * Assign this statement a label if it is required (that is, if stmt needs
     * its corresponding rstmt to be labelled).  Add live variable information
     * to optimisations if enabled.
     *   @param stmt the jimple statement
     *   @param rstmt its corresponding remopla statement
     */
    private void registerStatement(Stmt stmt, RemoplaStmt rstmt) {
        // Assign label
        RemoplaStmt existingStmt = translatedStmts.get(stmt);
        if(existingStmt != null) {
            if(existingStmt instanceof RemoplaNullStmt) {
                rstmt.setLabel(existingStmt.getLabel());
            } else {
                logger.warn("StmtTranslator: Tried to register the same statement translation twice.");
                logger.warn("              : " + stmt);
                logger.warn("             -> " + rstmt);
            }
        }


        translatedStmts.put(stmt, rstmt);
    }

    
    /** Finds the best exception trap for the given value from the given
     * program statement.  Takes the type of Value exception and returns the 
     * handling code associated with getExceptionDest that will accept the Value
     * exception.
     *   @param throwStmt the statement the exception is thrown from
     *   @param exception the type of exception thrown
     *   @return a statement handling the control flow -- either goto trap, or
     *   return
     */
    private RemoplaStmt getStaticExceptionHandler(Stmt throwStmt, SootClass exception) {
        Trap nextTrap = null;

        Iterator traps = currentCug.getExceptionDests(throwStmt).iterator();
        while (traps.hasNext() && nextTrap == null) {
            ExceptionDest eDest = (ExceptionDest)traps.next();
            Trap trap = eDest.getTrap();

            if (trap != null) {
                SootClass caughtClass = trap.getException();
                int dist = ClassManager.subClassDist(exception, caughtClass);
                if (dist > -1) {
                    nextTrap = trap;
                }
            }
        }

        RemoplaStmt handler = null;
        if (nextTrap != null) {
            handler = l.label(rgoto(getAddStatementLabel((Stmt)nextTrap.getHandlerUnit())));
        } else {
            logger.warn("Might have to implement exception handling better...");
            handler = l.label(rreturn());
        }

        return handler;
    }

   


    /** @param stmt the statement the exception branching is relative to
     * @param skpIfNull If true, then if the exception is null, then skip.
     * Otherwise raise a null pointer exception (null is used to indicate no
     * exception, but when throwing, the exception must not be null)
     * @return an if statement that branches to the exception handlers for stmt,
     * or returns if the exception is not caught
     */
    private RemoplaBranchStmt getExceptionBranchStmt(Stmt stmt) {
        RemoplaBranchStmt branch = (RemoplaBranchStmt)l.label(rbranch());

        // it's possible we don't have an exception...
        branch.addBranch(l.label(rskip()));

        // check local traps
        for (ExceptionDest eDest : currentCug.getExceptionDests((Unit)stmt)) {
            Trap trap = eDest.getTrap();
            if (trap != null) {
                RemoplaStmt gotoCatch = rgoto(getAddStatementLabel((Stmt)trap.getHandlerUnit()));
                branch.addBranch(l.label(gotoCatch));
            }
        }
 
        // else pop
        branch.addBranch(l.label(rreturn()));

        return branch;
    }


    private DefUses doExpr(Value expr, boolean defining) {
        et.analyseExpr(expr, currentSootMethod, defining);
        it.addClasses(et.getStaticAccesses());
        return (DefUses)et.getResult();
    }


    private void addDefUse(RemoplaStmt s, DefUses du) {
        s.addUses(du.getUses());
        s.addDefs(du.getDefs());
    }



    //////////////////////////////////////////////////////////////////////////////////////////////////////
    // Private classes
    //

    private class StmtTransLabeller extends StmtLabeller {
        private boolean label = false;
        private Stmt stmt = null;
        private RemoplaMethod method = null;

        public void setLabel(boolean label) {
            this.label = label;
        }

        public void setMethodStmt(RemoplaMethod method, Stmt stmt) {
            this.stmt = stmt;
            this.method = method;
        }

        /** Labels a statement if label is true, returns the statement for
         * convenience.  Labels as if rstmt is translated from currentStmt.
         */
        public RemoplaStmt label(RemoplaStmt rstmt) {
            return label(rstmt, stmt);
        }

        public RemoplaStmt forcelabel(RemoplaStmt rstmt) {
            return forcelabel(rstmt, stmt);
        }

        /** labels a statement, with reference to stmt **/
        public RemoplaStmt label(RemoplaStmt rstmt, Stmt stmt) {
            if (label) {   
                forcelabel(rstmt);
            }
            return rstmt;
        }

        public List<RemoplaStmt> labelStmts(List<RemoplaStmt> stmts) {
            if (label) {
                for (RemoplaStmt stmt : stmts) {
                    label(stmt);
                }
            }
            return stmts;
        }

        /** labels a statement, with reference to stmt **/
        public RemoplaStmt forcelabel(RemoplaStmt rstmt, Stmt stmt) {
            if (rstmt.getLabel() == null) {
                String userContent = "";
                if (currentMethod != null) {
                    userContent = currentMethod.getName();
                }

                if (stmt == null) {
                    rstmt.setLabel(Namer.getNewLabel(userContent));
                } else {
                    rstmt.setLabel(Namer.getNewStmtLabel(stmt, userContent));
                }
            }
            return rstmt;
        }

        /** Swap two labels **/
        public void swaplabels(RemoplaStmt s1, RemoplaStmt s2) {
            if (s1.getLabel() != null && s2.getLabel() != null) {
                String tmp = s1.getLabel();
                s1.setLabel(s2.getLabel());
                s2.setLabel(tmp);
            }
        }
    }

}
