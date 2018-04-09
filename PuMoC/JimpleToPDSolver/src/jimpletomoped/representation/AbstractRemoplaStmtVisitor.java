// AbstractRemoplaStmtVisitor.java
// Author: Matthew Hague
//
// Implements a remople statement visitor that will call
// "defaultCase(RemoplaStmt)" on all unhandled statement cases

package jimpletomoped.representation;

public abstract class AbstractRemoplaStmtVisitor implements RemoplaStmtVisitor {
    public void visit(RemoplaGotoStmt obj) { defaultCase(obj); }
    public void visit(RemoplaCallStmt obj) { defaultCase(obj); }
    public void visit(RemoplaNullStmt obj) { defaultCase(obj); }
    public void visit(RemoplaReturnStmt obj) { defaultCase(obj); }
    public void visit(RemoplaSkipStmt obj) { defaultCase(obj); }
    public void visit(RemoplaBranchStmt obj) { defaultCase(obj); }

    public abstract void defaultCase(RemoplaStmt obj);
}


