// RemoplaMethod.java
// Author: Matthew Hague
//
// RemoplaMethod contains info on methods for the translation

package jimpletomoped.representation;

import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;
import java.util.Iterator;
import java.lang.Exception;
import java.lang.StringBuffer;
import java.util.Collection;
import java.io.PrintStream;

import org.apache.log4j.Logger;

public class RemoplaMethod {

    static Logger logger = Logger.getLogger(RemoplaMethod.class);

    String methodName;
    List<RemoplaStmt> stmts = new ArrayList<RemoplaStmt>();

    /** Create a RemoplaMethod object.  
      *  @param methodName the name of the method, non-null
      */
    public RemoplaMethod(String methodName) {
        assert methodName != null;
        this.methodName = methodName;
    }
 
     /** Writes the method to a printstream
      *  @param out the buffer to write to
      *  @return the buffer
      */
    public void toString(PrintStream out) {
        assert out != null;
        writeHeader(out);
        for (RemoplaStmt stmt : stmts) {
            stmt.toString(out);
            out.print("\n");
        }
        writeFooter(out);
    }
  
    /** Writes the method to a string buffer
      *  @param out the buffer to write to
      *  @return the buffer
      */
    public StringBuffer toString(StringBuffer out) {
        assert out != null;
        writeHeader(out);
        for (RemoplaStmt stmt : stmts) {
            stmt.toString(out);
            out.append("\n");
        }
        writeFooter(out);
        return out;
    }

    public String toString() {
        return toString(new StringBuffer()).toString(); 
    }

    public String getName() {
        return methodName;
    }

    /** @return the string to use when referencing the method in a call
      */
    public String getCallString() {
        return methodName;
    }




    /** append a statement to the method
      * @param stmt the statement to add
      */
    public void appendStatement(RemoplaStmt stmt) {
        assert stmt != null;
        stmts.add(stmt); 
    }

    /** append a statement to the method
      * @param stmt the statement to add
      */
    public void appendStatements(List<RemoplaStmt> stmts) {
        assert stmts != null;
        for (RemoplaStmt s : stmts) {
            assert s != null;
        }
        this.stmts.addAll(stmts); 
    }


    /** @return the declaration string of the method 
     * That is, returnType name(args)
     * with no characters after the ).  
     */
    public String getDeclaration() {
        return getDeclaration(new StringBuffer()).toString();
    }

    /** Writes the declaration string of the method to the string buffer.
     * That is, returnType name(args)
     * with no characters after the ).  
     *  @param out the string buffer
     *  @return the string buffer
     */

    public StringBuffer getDeclaration(StringBuffer out) {
        out.append("module ").append(methodName).append(" (...)");
        return out;
    }


    public List<RemoplaStmt> getStmts() {
        return stmts;
    }

    /** Statements such as if have sub statements.  This method returns all
     * substatements too.
     *   @param stmts the list to store the statements into
     */
    public void getFlattenedStmts(List<RemoplaStmt> stmts) {
        for(RemoplaStmt s : getStmts()) {
            s.flatten(stmts);
        }
    }
   



    ////////////////////////////////////////////////////////////////////////////
    // Private methods
    //
    

    private void writeHeader(StringBuffer out) {
        getDeclaration(out);
        out.append(" {\n");
    }

    private void writeFooter(StringBuffer out) {
        out.append("}\n");
    }


    private void writeHeader(PrintStream out) {
        out.print(getDeclaration());
        out.print(" {\n");
    }

    private void writeFooter(PrintStream out) {
        out.print("}\n");
    }



}
