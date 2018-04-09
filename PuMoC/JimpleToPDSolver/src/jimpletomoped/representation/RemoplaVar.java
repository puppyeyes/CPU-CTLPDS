// RemoplaVar.java Author: Matthew Hague
//
// Variables in Remopla 

package jimpletomoped.representation;

import java.lang.StringBuffer;

public class RemoplaVar {
    private String name;

    /** @param name the variable name
     */
    public RemoplaVar(String name) {
        assert name != null;
        this.name = name;
    }


    /** Writes the variable to out.  For use within code.  
      * Use getDeclaration for the full declaration.
      *  @param out string buffer to write to
      *  @return the given string buffer
      */
    public StringBuffer toString(StringBuffer out) {
        out.append(name);
        return out;
    }

    /** Returns the variable as a string for use in within normal code.  Use
      * getDeclaration for the declaration.
      *  @return variable as a string
      */
    public String toString() {
        return name;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        assert name != null;
        this.name = name; 
    }

    /** Write the declaration to a string buffer, without a trailing ";" or ","
      *  @param out the buffer
      *  @return the buffer
      */
    public StringBuffer getDeclaration(StringBuffer out) {
        out.append("var ").append(getName()).append(";");
        return out;
    }

    public String getDeclaration() {
        return getDeclaration(new StringBuffer()).toString();
    }
}
