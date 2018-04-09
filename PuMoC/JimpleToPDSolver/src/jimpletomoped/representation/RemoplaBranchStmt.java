// RemoplaGuardedStmt.java
// Author: Matthew Hague

package jimpletomoped.representation;

import java.util.Set;
import java.util.HashSet;
import java.util.Collection;
import java.util.List;
import java.util.ArrayList;

import java.lang.StringBuffer;

import java.io.PrintStream;

public class RemoplaBranchStmt extends RemoplaStmt {

    public class Branch {
        List<RemoplaStmt> stmts;

        public Branch(List<RemoplaStmt> stmts) { 
            this.stmts = stmts;
        }

        public StringBuffer toString(StringBuffer out) {
            out.append(" -> ");
            for (RemoplaStmt stmt : stmts) {
                stmt.toString(out).append(" ");
            }
            return out;
        }
 
        public void toString(PrintStream out) {
            out.print(" -> ");
            for (RemoplaStmt stmt : stmts) {
                stmt.toString(out);
                out.print(" ");
            }
        }
        
        public List<RemoplaStmt> getStmts() {
            return stmts;
        }
    }

    private Collection<Branch> branches = new HashSet<Branch>();

    public RemoplaBranchStmt() {
    }


    /** Add a new branch to the branch statement 
      *  @param stmt non-null statement 
      *  @return the branch 
      */
    public Branch addBranch(RemoplaStmt stmt) {
        assert stmt != null;
        List<RemoplaStmt> stmts = new ArrayList<RemoplaStmt>(1);
        stmts.add(stmt);
        Branch branch = new Branch(stmts);
        branches.add(branch);
        return branch;
    }


    /** Add a new clause to the branch statement 
      *  @param stmt non-null statement 
      *  @return the branch 
      */
    public Branch addBranch(List<RemoplaStmt> stmts) {
        assert stmts != null && !stmts.isEmpty();
        for (RemoplaStmt stmt : stmts) {
            assert stmt != null;
        }
        Branch branch = new Branch(stmts);
        branches.add(branch);
        return branch;
    }

    /** @return the collection of statements with branches **/
    public Collection<Branch> getBranches() {
        return branches;
    }

    /** Writes the body of a statement to a string buffer.
      * That is, the statement without labels or other paraphernalia.        
      *  @param out the buffer
      *  @return the buffer
      */
    public StringBuffer toStringBody(StringBuffer out) {
        writeStart(out).append("\n");

        for(Branch branch : branches) {
            out.append(":: ");
            branch.toString(out).append("\n");
        }

        return writeEnd(out).append(";");
    }

    /** Writes the body of a statement to a stream
      * That is, the statement without labels or other paraphernalia.        
      *  @param out the buffer
      *  @return the buffer
      */
    public void toStringBody(PrintStream out) {
        writeStart(out);
        out.append("\n");

        for(Branch branch : branches) {
            out.append(":: ");
            branch.toString(out);
            out.append("\n");
        }

        writeEnd(out);
        out.append(";");
    }


    public void flatten(List<RemoplaStmt> stmts) {
        super.flatten(stmts);
        for (Branch b : branches) {
            for (RemoplaStmt stmt : b.getStmts()) {
                stmt.flatten(stmts);
            }
        }
    }


    public StringBuffer writeStart(StringBuffer out) {
        return out.append("branch");
    }

    public StringBuffer writeEnd(StringBuffer out) {
        return out.append("branch");
    }

    public void writeStart(PrintStream out) {
        out.print("branch");
    }

    public void writeEnd(PrintStream out) {
        out.print("branch");
    }


    public void accept(RemoplaStmtVisitor visitor) {
        visitor.visit(this);
    }

}
