// StmtToRule.java
// Author: Matthew Hague
//
// A class for converting RemoplaStmt objects to Pds rules.  See
// RemoplaToPDS.java for the translation specifics.


// TODO: Refactor the handling of semiRingGuard or clarify it's use.  All a bit
// global var at the moment.

package jimpletomoped.remoplatopds;

import java.util.Collection;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
                    
import jimpletomoped.representation.*;

import org.apache.log4j.Logger;

import static jimpletomoped.representation.RemoplaStaticFactory.*;

public class StmtToRules extends AbstractRemoplaStmtVisitor {

    static Logger logger = Logger.getLogger(StmtToRules.class);

    Remopla currentRemopla;
    Pds currentPds;

    String nextStmtLabel = null;
    String breakLabel = null;


    public void addInitialCall(Pds pds, Remopla remopla) {
        RemoplaMethod init = remopla.getInitialMethod();
        String qinit = Namer.getInitControl();
        String qend = Namer.getEndControl();
        String q = Namer.getControlState();
        String init_stack = Namer.getInitStack();

        // We're going to the first statement in the method
        List<RemoplaStmt> methodStmts = init.getStmts();
        if (methodStmts.size() > 0) {
            String methodLabel = getAddLabel(methodStmts.get(0));
            pds.addRule(new Rule(qinit, init_stack, q, methodLabel, init_stack));
            pds.addRule(new Rule(q, init_stack, qend, init_stack));
        }
    }


    /** Translates a list of RemoplaStmt into a collection of pushdown rules
     * labelled with an ExprSemiring object
     *   @param stmts the list of statements to translate.  Adds a default label
     *   to the statement if it doesn't have one.
     *   @param nextStmt the statement to be executed next (provided stmt doesn't
     *   jump, &c.).  Null if there is no next statement (assumed to return from
     *   current method).  Adds a label to the statement if it does not have
     *   one.
     *   @param currentRemopla the current remopla instance
     *   @param pds the pds to add rules and props to
     */
    public void translateStmtBlock(List<RemoplaStmt> stmts, 
                                   RemoplaStmt nextStmt,
                                   Remopla currentRemopla,
                                   Pds pds) {
        assert currentRemopla != null && pds != null;
        this.currentRemopla = currentRemopla;
        this.currentPds = pds;

        subTranslateStmtBlock(stmts, getAddLabel(nextStmt));
    }

    /** Translates a given RemoplaStmt into a collection of pushdown rules
     * labelled with an ExprSemiring object
     *   @param stmt the statement to translate, non-null.
     *   Adds a label to the statement if it does not have one.
     *   @param nextStmt the statement to be executed next (provided stmt doesn't
     *   jump, &c.).  Null if there is no next statement (assumed to return from
     *   current method).  Adds a label to the statement if it does not have
     *   one.
     *   @param currentRemopla the current Remopla instance
     *   @param pds the pds to add new rules and propositions to
     */
    public void translateStmt(RemoplaStmt stmt, 
                              RemoplaStmt nextStmt,
                              Remopla currentRemopla,
                              Pds pds) {
        assert currentRemopla != null && stmt != null && pds != null;
        this.currentRemopla = currentRemopla;
        this.currentPds = pds;

        subTranslateStmt(stmt, getAddLabel(nextStmt));
    }



    public void visit(RemoplaGotoStmt gs) {
        for (String target : gs.getTargets()) {
            makePlainRule(getAddLabel(gs), target);
        }
    }




    public void visit(RemoplaBranchStmt branch) {
        int i = 0;
        for (RemoplaBranchStmt.Branch b : branch.getBranches()) {
            List<RemoplaStmt> stmts = b.getStmts();
            makePlainRule(getAddLabel(branch), getAddLabel(stmts.get(0)));
            subTranslateStmtBlock(stmts, nextStmtLabel);
        }
    }



    public void visit(RemoplaCallStmt call) {
        RemoplaMethod m = currentRemopla.getMethod(call.getCallString());
        makePushRule(getAddLabel(call), m, nextStmtLabel);
    }


    public void visit(RemoplaNullStmt obj) {
        // Null: do nothing
    }


    public void visit(RemoplaReturnStmt rs) {
        makePopRule(getAddLabel(rs));
    }


    public void visit(RemoplaSkipStmt ss) {
        makePlainRule(getAddLabel(ss), nextStmtLabel);
    }


    public void defaultCase(RemoplaStmt stmt) {
        logger.error("Unhandled Remopla structure in StmtToRules: " + stmt);
    }


    /////////////////////////////////////////////////////////////////////////////////////
    // Private methods
    //


    /** String stmtLabel is the label of the current top stack **/
    private void makePopRule(String stmtLabel) {
        String q = Namer.getControlState();
        currentPds.addRule(new Rule(q, stmtLabel, q));
    }


    /** Assumes that if the next statement label is null, we're doing a pop with
     * no return value
     */
    private void makePlainRule(String stmtLabel, String nextLabel) {
        if (nextLabel != null) {
            String q = Namer.getControlState();
            currentPds.addRule(new Rule(q, stmtLabel, q, nextLabel));
        } else {
            makePopRule(stmtLabel);
        }
    }

    /** If there is no next label, we're really doing a rewrite (since the
     * return would go to a state that would return.  That is for a stack "a w" we
     * normally push to "b a' w" where a' is the return site.  But, if a' is null we
     * just have "b w".
     *
     *  If the method has no statements, just a plain rule to the return site is
     *  created
     *
     *   @param stmt the remopla statement on the rule
     *   @param stmtLabel the statement calling
     *   @param m the method to be called
     *   @param nextLabel the return site
     */
    private void makePushRule(String stmtLabel, RemoplaMethod m, String nextLabel) {
        // We're going to the first statement in the method
        List<RemoplaStmt> methodStmts = m.getStmts();
        if (methodStmts.size() == 0) {
            makePlainRule(stmtLabel, nextLabel);
        } else {
            String methodLabel = getAddLabel(methodStmts.get(0));
            if (nextLabel == null) {
                makePlainRule(stmtLabel, methodLabel);
            } else {
                String q = Namer.getControlState();
                currentPds.addRule(new Rule(q, stmtLabel, q, methodLabel, nextLabel));
             }
        }
    }

   


    /** like translateStmtBlock, but does not new or free the result set, just
     * adds to it, and takes a string next statement label rather than a
     * statement
     */
    private void subTranslateStmtBlock(List<RemoplaStmt> stmts, String nextStmtLabel) {
        assert stmts != null; 

        String onsl = this.nextStmtLabel;

        String blockExitLabel = nextStmtLabel;

        Iterator i = stmts.iterator();
        RemoplaStmt blockNext = i.hasNext() ? (RemoplaStmt)i.next() : null;
        while (blockNext != null) {
            RemoplaStmt stmt = blockNext;
            blockNext = i.hasNext() ? (RemoplaStmt)i.next() : null;

            addProps(stmt);

            this.nextStmtLabel = blockNext != null ? getAddLabel(blockNext) : blockExitLabel;
            
            stmt.accept(this);
        }

        this.nextStmtLabel = onsl;
    }


    /** like translateStmt, but does not free or create the result set, just
     * adds to it, but takes a string next statement label rather than a stmt
     */
    private void subTranslateStmt(RemoplaStmt stmt, String nextStmtLabel) {
        assert stmt != null;

        String onsl = this.nextStmtLabel;
    
        this.nextStmtLabel = nextStmtLabel;

        addProps(stmt);

        stmt.accept(this);

        this.nextStmtLabel = onsl;
    }

    private String getAddLabel(RemoplaStmt s) {
        String l;
        if (s == null) {
            l = null;
        } else {
            l = s.getLabel();
            if (l == null) {
                l = Namer.getNewLabel();
                s.setLabel(l);
            }
        }
        return l;
    }


    private void addProps(RemoplaStmt s) {
        String l = getAddLabel(s);
        String q = Namer.getControlState();

        Collection<String> uses = s.getUses();
        Collection<String> defs = s.getDefs();

        for (String var : uses) {
            if (defs.contains(var)) {
                currentPds.addProp(q, l, usedefProp(var));
            } else {
                currentPds.addProp(q, l, useProp(var));
            }
        }
        
        for (String var : s.getDefs()) {
            if (!uses.contains(var)) 
                currentPds.addProp(q, l, defProp(var));
        }
    }

    private String useProp(String var) {
        return "use_" + var;
    }

    private String usedefProp(String var) {
        return "usedef_" + var;
    }

    private String defProp(String var) {
        return "def_" + var;
    }


}
