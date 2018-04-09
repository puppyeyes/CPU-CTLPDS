// RemoplaMethodCall.java
// Author: Matthew Hague
//

package jimpletomoped.representation;

import java.util.List;
import java.util.ArrayList;
import java.util.Iterator;
import java.lang.StringBuffer;
import java.io.PrintStream;

public class RemoplaCallStmt extends RemoplaStmt {

    String callString;

    /** Construct a method call 
      *  @param callString the string to call the method with (non-null)
      *  @param parameters a list of parameter expressions (non-null)
      *  Currently it is assumed that the number of
      *  parameters is correct and that they are non-null.
      */
    public RemoplaCallStmt(String callString) {
        assert callString != null;
        this.callString = callString;
    } 

    public String getCallString() {
        return callString;
    }

    /** Write the method call to a string buffer 
      *  @param out the buffer
      *  @return the buffer
      */
    public StringBuffer toStringBody(StringBuffer out) {
        out.append(callString).append("(...)");
        return out;
    } 

    /** Write the method call to a stream
      *  @param out the buffer
      *  @return the buffer
      */
    public void toStringBody(PrintStream out) {
        out.print(callString);
        out.print("(...)");
    } 


    public String toString() {
        return toString(new StringBuffer()).toString();
    }


    public void accept(RemoplaStmtVisitor visitor) {
        visitor.visit(this);
    }


}
