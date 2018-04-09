// RemoplaMutableStmt.java
// Author: Matthew Hague
//
// A class for holding a statement that may change into another kind of
// statement.  This statement may appear in code, and an outsider can change it
// into something else.  Passes on visitor requests, label changes and tostring requests
// directly to the contained statement.  Changes label when setStmt is called.


package jimpletomoped.representation;


import java.util.List;
import java.io.PrintStream;

public class RemoplaMutableStmt extends RemoplaStmt {

    private RemoplaStmt stmt;

    public RemoplaMutableStmt(RemoplaStmt stmt) {
        assert stmt != null;
        this.stmt = stmt;
    }

    public RemoplaStmt getStmt() {
        return stmt;
    }

    public void setStmt(RemoplaStmt stmt) {
        assert stmt != null;
        this.stmt = stmt;
    }

    /** puts statement and all substatements into stmts **/
    public void flatten(List<RemoplaStmt> stmts) {
        assert stmts != null;
        stmts.add(stmt);
    }

    public void setLabel(String label) {
        stmt.setLabel(label);
    }

    public String getLabel() {
        return stmt.getLabel();
    }


    /** Writes the body of a statement to a string buffer.
      * That is, the statement without labels or other paraphernalia.        
      *  @param out the buffer
      *  @return the buffer
      */
    public StringBuffer toStringBody(StringBuffer out) {
        return stmt.toString(out);
    }

    public void toStringBody(PrintStream out) {
        stmt.toString(out);
    }


    public void accept(RemoplaStmtVisitor visitor) {
        stmt.accept(visitor);
    }
}
