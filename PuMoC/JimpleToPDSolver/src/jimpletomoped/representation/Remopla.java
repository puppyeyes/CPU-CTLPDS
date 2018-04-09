// Remopla.java
// Author: Matthew Hague
//
// The Remopla class hierarchy is based on the PDS hierarchy in jMoped by 
// Dejvuth Suwimonteerabuth

// The Remopla class is the umbrella class for the Remopla representation
// Maintains a list of methods and variables &c. 
// Provides to<WHAT> output method

package jimpletomoped.representation;

import org.apache.log4j.Logger;

import java.util.Collection;
import java.util.LinkedHashSet;
import java.lang.StringBuffer;
import java.util.Map;
import java.util.HashMap;
import java.util.HashSet;

import java.io.PrintStream;

import jimpletomoped.main.CmdOptions;
import java.util.Set;
import jimpletomoped.representation.RemoplaStmt;
import java.util.List;
import java.util.ArrayList;

public class Remopla {
    private static final String INIT = "init";
    private static final String DEF_DEFAULT_INT_BITS = "define DEFAULT_INT_BITS";

    static Logger logger = Logger.getLogger(Remopla.class);

    // map names to objects
    Map<String, RemoplaMethod> methods = new HashMap<String, RemoplaMethod>();
    Collection<RemoplaStmt> floating = new HashSet<RemoplaStmt>();

    RemoplaMethod initialMethod;

    public Remopla() {
    }

    public void addMethod(RemoplaMethod method) {
        methods.put(method.getCallString(), method);
    }

    public Collection<RemoplaMethod> getMethods() {
        return methods.values();
    }

    /** Designates the initial method.
     *   @param initialMethod must be non-null, take no arguments and already added
     *   to the remopla instance as a method
     */
    public void setInitialMethod(RemoplaMethod initialMethod) {
        assert initialMethod != null;
        assert getMethods().contains(initialMethod);
        this.initialMethod = initialMethod;
    }


    /** Returns the initial method, null if one hasn't been set **/
    public RemoplaMethod getInitialMethod() {
        return initialMethod;
    }


    /** Writes the Remopla program to a 
      *  @param out the buffer to write to
      *  @return the buffer
      */
    public StringBuffer toString(StringBuffer out) {
        assert out != null;
        writeHeader(out);
        for(RemoplaMethod method : getMethods()) {
            method.toString(out);
            out.append("\n\n");
        }
        writeFooter(out);
        return out;
    }



    /** Writes the Remopla program to a 
      *  @param out the buffer to write to
      *  @return the buffer
      */
    public PrintStream toString(PrintStream out) {
        assert out != null;
        writeHeader(out);
        for(RemoplaMethod method : getMethods()) {
            method.toString(out);
            out.print("\n\n");
        }
        writeFooter(out);
        return out;
    }


    public String toString() {
        return toString(new StringBuffer()).toString(); 
    }

    /** Add a floating statement (these are generally of the form label: goto
     * label and constitute dead end error states)
     *  @param stmt non-null statement
     */
    public void addFloatingStmt(RemoplaStmt stmt) {
        assert stmt != null;
        floating.add(stmt);
    }

    public Collection<RemoplaStmt> getFloatingStmts() {
        return floating;
    }


    public RemoplaMethod getMethod(String callString) {
        return methods.get(callString);
    }

    /** @return a map from call strings to methods **/
    public Map<String, RemoplaMethod> getMethodMap() {
        return methods;
    }


    /** @return all the labels defined in the program **/
    public Set<String> getLabels() {
        Set<String> labels = new HashSet();
        //int count=0;
        List<RemoplaStmt> stmts = new ArrayList();
        for (RemoplaMethod method : methods.values()) {
            stmts.clear();
            method.getFlattenedStmts(stmts);
            for (RemoplaStmt s : stmts) {
                if (s.getLabel() != null)
                { 
                	 // count++;
                    labels.add(s.getLabel());
                }
            }
        }
       // System.out.printf("\n\n number: %d \n\n",count);//
        return labels;
    }

    /////////////////////////////////////////////////////////////////////////////////////
    // Private methods
    //


    
    private StringBuffer writeHeader(StringBuffer out) {
        for (RemoplaMethod method : getMethods()) {
            method.getDeclaration(out).append(";\n");
        }


        out.append("\n"); 

        if (initialMethod != null) {
            out.append(INIT).append(" ").append(initialMethod.getCallString()).append(";");
        }

        return out.append("\n\n");
    }

    private StringBuffer writeFooter(StringBuffer out) {
        out.append("\n");

        for (RemoplaStmt stmt : floating) {
            stmt.toString(out).append("\n");
        }
        return out;
    }

    private void writeHeader(PrintStream out) {
        for (RemoplaMethod method : getMethods()) {
            out.print(method.getDeclaration() + ";\n");
        }

        out.print("\n"); 

        if (initialMethod != null) {
            out.print(INIT);
            out.print(" ");
            out.print(initialMethod.getCallString());
            out.print(";");
        }

        out.print("\n\n");
    }

    private void writeFooter(PrintStream out) {
        out.print("\n");

        for (RemoplaStmt stmt : floating) {
            stmt.toString(out);
            out.print("\n");
        }
    }

}

