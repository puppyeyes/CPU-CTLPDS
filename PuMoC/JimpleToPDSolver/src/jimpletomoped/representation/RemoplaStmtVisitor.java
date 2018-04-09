// RemoplaVisitor.java
// Author: Matthew Hague
//
// An interface for a visitor to RemoplaStmts

package jimpletomoped.representation;

public interface RemoplaStmtVisitor {
    public void visit(RemoplaGotoStmt obj);
    public void visit(RemoplaNullStmt obj);
    public void visit(RemoplaReturnStmt obj);
    public void visit(RemoplaSkipStmt obj);
    public void visit(RemoplaCallStmt obj);
    public void visit(RemoplaBranchStmt obj);
}
