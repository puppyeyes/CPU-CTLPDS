// RemoplaReturnStmt.java
// Author: Matthew Hague
//
// A class for representing return statements

package jimpletomoped.representation;

import java.lang.StringBuffer;

import java.io.PrintStream;

public class RemoplaReturnStmt extends RemoplaStmt {
    
    /** Construct a return object.
      */
    public RemoplaReturnStmt() {
    }


    /** Print statement body to a string buffer.
      * That is, ignore labels and other paraphernalia.
      *  @param out the buffer
      *  @return the buffer
      */
    public StringBuffer toStringBody(StringBuffer out) {
        out.append("return");
        return out.append(";");
    }

    public void toStringBody(PrintStream out) {
        out.print("return");
        out.print(";");
    }

    public void accept(RemoplaStmtVisitor visitor) {
        visitor.visit(this);
    }

}
