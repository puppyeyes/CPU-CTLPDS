// ClassInfo.java
// Author: Matthew Hague
//
// Used by Variable Factory to implement objects

package jimpletomoped.translation;

import java.util.Set;
import java.util.HashSet;
import java.util.Map;
import java.util.HashMap;

import soot.SootClass;
import soot.SootField;

import jimpletomoped.representation.Remopla;

import org.apache.log4j.Logger;

public class ClassInfo {

    private static final int UNDEFINED = -1;

    Logger logger = Logger.getLogger(ClassInfo.class);

    SootClass sc;
    ClassInfo superClass;
    Set<ClassInfo> subClasses = new HashSet<ClassInfo>();

    // store whether the class is actually used in the program (no point
    // assigning ids to classes that never get referenced)
    boolean used;

    /** @param sc the soot class
     *  @param superClass the info of the super class
     *  @param used true if the class is referenced in the program (has an
     *  instance or appears in instanceof, &c.
     */
    public ClassInfo(SootClass sc, ClassInfo superClass, Remopla remopla) {
        assert sc != null;
        this.sc = sc;
        this.superClass = superClass;
        this.used = false;
    }

    public void addSubClass(ClassInfo subClass) {
        assert subClass != null;
        subClasses.add(subClass);
    }

    public Set<ClassInfo> getSubClasses() {
        return subClasses;
    }

    /** Set whether the class is used by the program (unused classes don't get
     * an id in numberHierarchy)
     */
    public void setUsed(boolean used) {
        this.used = used;
    }

    public boolean getUsed() {
        return used;
    }
}


