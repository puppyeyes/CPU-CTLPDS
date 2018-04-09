// RemoplaSkipStmt.java
// Author: Matthew Hague
//
// The class for simple skips

package jimpletomoped.representation;

import java.io.PrintStream;

public class RemoplaSkipStmt extends RemoplaStmt {
        
    public RemoplaSkipStmt() {
        // do nothing
    }

    /** Writes the skip statement to a string buffer without adorning labels &c.
      *  @param out the buffer
      *  @return the buffer
      */
    public StringBuffer toStringBody(StringBuffer out) {
        out.append("skip");
        return out.append(";");
    }

    public void toStringBody(PrintStream out) {
        out.print("skip");
        out.print(";");
    }

    public void accept(RemoplaStmtVisitor visitor) {
        visitor.visit(this);
    }

}
