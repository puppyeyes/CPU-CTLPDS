// RemoplaGotoStmt.java
// Author: Matthew Hague
//
// A class for goto statements

package jimpletomoped.representation;

import java.lang.StringBuffer;
import java.util.Collection;
import java.util.HashSet;
import java.io.PrintStream;


public class RemoplaGotoStmt extends RemoplaStmt {

    private Collection<String> targets;

    /** Construct a goto statement
      *  @param target the target label, non-null
      */
    public RemoplaGotoStmt(String target) {
        assert target != null;
        this.targets = new HashSet<String>();
        this.targets.add(target);
    }

    public Collection<String> getTargets() {
        return targets;
    }


   /** Writes the body of a statement to a string buffer.
     * That is, the statement without labels or other paraphernalia.        
     *  @param out the buffer
     *  @return the buffer
     */
    public StringBuffer toStringBody(StringBuffer out) {
        out.append("goto ");
        for (String target : targets) {
            out.append(target).append(", ");
        }
        return out.append(";");
    }

   /** Writes the body of a statement to a stream
     * That is, the statement without labels or other paraphernalia.        
     *  @param out the buffer
     *  @return the buffer
     */
    public void toStringBody(PrintStream out) {
        out.print("goto ");
        for (String target : targets) {
            out.print(target);
            out.print(", ");
        }
        out.print(";");
    }


    public void accept(RemoplaStmtVisitor visitor) {
        visitor.visit(this);
    }

}
