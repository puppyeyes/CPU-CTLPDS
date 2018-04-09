// RemoplaStmt.java
// Author: Matthew Hague
//
// The information concerning a particular Remopla translation of a Jimple
// statement

package jimpletomoped.representation;

import java.lang.StringBuffer;

import java.util.List;
import java.util.Collection;
import java.util.HashSet;

import java.io.PrintStream;

import org.apache.log4j.Logger;

public abstract class RemoplaStmt {

    static Logger logger = Logger.getLogger(RemoplaStmt.class);

    private String label = null;
    private String comment = null;

    private Collection<String> uses = new HashSet<String>();
    private Collection<String> defs = new HashSet<String>();


    /** Writes the statement to stream (including terminating semi-colon)
      *  @param out the buffer to write to
      *  @return the buffer
      */
    public void toString(PrintStream out) {
        if (label != null) {
            out.print(label);
            out.print(": ");
        }

        toStringBody(out);

        if (comment != null) {
            out.print("   ## ");
            out.print(comment);
            out.print(" ##");
        }

        if (!uses.isEmpty()) {
            out.print("   UU ");
            for (String var : uses) {
                out.print(var + "  ");
            }
            out.print(" UU");
        }

        if (!defs.isEmpty()) {
            out.print("   DD ");
            for (String var : defs) {
                out.print(var + "  ");
            }
            out.print(" DD");
        }
    }




    /** Writes the statement to a string (including terminating semi-colon)
      *  @param out the buffer to write to
      *  @return the buffer
      */
    public StringBuffer toString(StringBuffer out) {
        if (label != null)
            out.append(label).append(": ");

        toStringBody(out);

        if (comment != null) 
            out.append("   ## ").append(comment).append(" ##");

        if (!uses.isEmpty()) {
            out.append("   UU ");
            for (String var : uses) {
                out.append(var + "  ");
            }
            out.append(" UU");
        }

        if (!defs.isEmpty()) {
            out.append("   DD ");
            for (String var : defs) {
                out.append(var + "  ");
            }
            out.append(" DD");
        }

        return out;
    }

    public void setComment(String comment) {
        this.comment = comment;
    }

    public String getComment() {
        return comment;
    }

    public String toString() {
        return toString(new StringBuffer()).toString();
    }


    /** @return The label associated with the statement, null if none exists
      */
    public String getLabel() {
        return label;
    }

    public void setLabel(String label) {
        this.label = label;
    }

    /** puts statement and all substatements into stmts **/
    public void flatten(List<RemoplaStmt> stmts) {
        assert stmts != null;
        stmts.add(this);
    }


    public void addUses(Collection<String> uses) {
        this.uses.addAll(uses);
    }

    public void addDefs(Collection<String> defs) {
        this.defs.addAll(defs);
    }


    public Collection<String> getUses() {
        return uses;
    }

    public Collection<String> getDefs() {
        return defs;
    }

    /** Writes the body of a statement to a string buffer.
      * That is, the statement without labels or other paraphernalia.        
      *  @param out the buffer
      *  @return the buffer
      */
    public abstract StringBuffer toStringBody(StringBuffer out);
    public abstract void toStringBody(PrintStream out);

    public abstract void accept(RemoplaStmtVisitor visitor);
}
