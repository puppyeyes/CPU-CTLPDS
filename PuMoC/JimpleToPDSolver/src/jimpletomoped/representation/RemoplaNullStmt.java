// RemoplaNullStmt.java
// Author: Matthew Hague
//
// A statement class for unimplemented statements
// To string is simply a warning

package jimpletomoped.representation;

import java.lang.StringBuffer;

import java.io.PrintStream;

public class RemoplaNullStmt extends RemoplaStmt {

    String warning;

    public RemoplaNullStmt(String warning) {
        this.warning = warning;
    }

    /** Writes the body of the statement to a string buffer.
      * That is, without labels or other generic paraphernalia.
      *  @param out the buffer
      *  @return the buffer
      */
    public StringBuffer toStringBody(StringBuffer out) {
        out.append("skip; // " + warning);
        return out; 
    }

    public void toStringBody(PrintStream out) {
        out.print("skip; // " + warning);
    }

 
    public void accept(RemoplaStmtVisitor visitor) {
        visitor.visit(this);
    }
}
