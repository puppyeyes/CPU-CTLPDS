// ClassManager.java
// Author: Matthew Hague
//
// Deals with naming of classes, static subtyping tests, class ids &c. &c.


package jimpletomoped.translation;

import java.util.HashMap;
import java.util.Iterator;

import jimpletomoped.main.CmdOptions;

import jimpletomoped.representation.Remopla;

import org.apache.log4j.Logger;

import soot.Scene;
import soot.SootClass;
import soot.SootField;
import soot.SootMethod;

import static jimpletomoped.representation.RemoplaStaticFactory.*;

public class ClassManager {

    private static Logger logger = Logger.getLogger(ClassManager.class);

    private static ClassInfo classObjectInfo = null;
    // Class names to class info objects
    private static HashMap<String, ClassInfo> classInfo = new HashMap<String, ClassInfo>();

    /** @param sc the soot class to test
     * @return true if the method has a stub
     */
    static public boolean hasStub(SootClass sc) {
        assert sc != null;
        String className = sc.getName();
        return hasStub(className);
    }

    static public boolean hasStub(String className) {
        return CmdOptions.getStubList().contains(className);
    }

    /** @param class the class to test
     * @return true if the class belongs to an ignored package
     */
    public static boolean isIgnored(SootClass sc) {
        assert sc != null;
        String className = sc.getName();
        boolean ignored = false;

        Iterator<String> ignoredPrefixes = CmdOptions.getIgnoredClassPrefixes().iterator();
        while(ignoredPrefixes.hasNext() && !ignored) {
            ignored = className.startsWith(ignoredPrefixes.next());
        }

        return ignored; 
    }

    /** @param method the method to test
     * @return true if the method belongs to an ignored package
     */
    public static boolean isIgnored(SootMethod method) {
        assert method != null;
        return ClassManager.isIgnored(method.getDeclaringClass());
    }
        

    public static void loadStubs() {
        for (String stubName : CmdOptions.getStubList()) {
            loadClass(Namer.getConcreteClassName(stubName));
        }
    }

    public static SootClass getClassStub(SootClass sc) {
        assert sc != null;
        String className = Namer.getConcreteClassName(sc);

        return loadClass(className);
    }

    public static SootClass loadClass(String className) {
        assert className != null;
        SootClass sc = Scene.v().loadClassAndSupport(className);
        if (sc != null)
            sc.setApplicationClass();
        return sc;
    }

    public static SootClass loadClassWithSlashes(String className) {
        assert className != null;
        return loadClass(className.replaceAll("/","."));
    }


    /** @param method gets the stubbed version of this method, if
     * hasStub(method.getDeclaringClass()) holds.  Else returns null.
     * @return the stubbed version of method, or null.
     */
    public static SootMethod getMethodStub(SootMethod method) {
        assert method != null;
        SootClass stubClass = getClassStub(method.getDeclaringClass());
        
        // find the method
        SootMethod stubMethod = null;
        Iterator i = stubClass.getMethods().iterator();
        while (stubMethod == null && i.hasNext()) {
            SootMethod candidate = (SootMethod)i.next();
            if (Namer.getMethodName(candidate).equals(Namer.getMethodName(method))) {
                stubMethod = candidate;
            }
        }

        return stubMethod;
    }

    /** Loads the named method and all of its supporting classes and methods
     *  @param methodSig the method signature as described in
     *  Namer.makeSignature
     *  @return the corresponding SootMethod, null if it could not be found
     *  (classes may still be loaded)
     */
    public static SootMethod loadMethod(String methodSig) {
        assert methodSig != null;
        String methodClass = Namer.getClassPartFromSig(methodSig);
        SootMethod loadedMethod = null;

        if (methodClass != null) {
            SootClass c = loadClass(methodClass);

            Iterator mi = c.getMethods().iterator();
            while (mi.hasNext()) {
                SootMethod sm = (SootMethod)mi.next();
                if (sm.getSignature().equals(methodSig)) {
                    loadedMethod = sm;
                }
            }

            if (loadedMethod != null && loadedMethod.isConcrete()) {
                loadedMethod.retrieveActiveBody();
            } else {
                loadedMethod = null;
            }
        }

        return loadedMethod;
    }





    /** returns -1 if sub is not a subclass (or equal) of sub, an integer
     * representing the subclass difference otherwise
     *   @param sub the soot class to be a subclass
     *   @param sup the soot class to be a superclass
     *   @return -1 if not a subclass, 0 if equal, 1 if direct subclass, &c.
     */
    static public int subClassDist(SootClass sub, SootClass sup) {
        int distance = 0;
        SootClass subSup = sub;

        while (subSup.hasSuperclass() && !subSup.equals(sup)) {
            subSup = subSup.getSuperclass();        
            distance++;
        }

        return subSup.equals(sup) ? distance : -1;
    }


    static public ClassInfo getAddClassInfo(SootClass sc, Remopla remopla, boolean used) {
        String univeralName = Namer.getUniversalClassName(sc);

        ClassInfo info = classInfo.get(univeralName);

        if (info == null) {
            ClassInfo superClassInfo = null;
            if (sc.hasSuperclass()) {
                superClassInfo = getAddClassInfo(sc.getSuperclass(), remopla, false);
            } 

            info = new ClassInfo(sc, superClassInfo, remopla);

            classInfo.put(univeralName, info);

            if (!sc.hasSuperclass()) {
                classObjectInfo = info;
            } else {
                superClassInfo.addSubClass(info);
            }
        } else {
            info.setUsed( info.getUsed() || used);
        }

        return info;
    }


    /** Call this method after translation -- does nothing at the moment...
     */
    static public void finaliseClassData() {
        // do nothing      
    }



    /** Returns a method from sc that implements the method passed (e.g. if sc
     * is a subtype of the declaring class of sm)
     *  @param sc the class, not null
     *  @param sm the method (can be declared by another class/interface)
     *  @return a (concrete) method from sc implementing sm, null if none exists
     */
    static public SootMethod getImplementation(SootClass sc, SootMethod sm) {
        assert sc != null && sm != null;

        SootMethod implementation = null;

        if (subClassOrImplements(sc, sm.getDeclaringClass())) {
            String sig = sm.getSubSignature();
            if (sc.declaresMethod(sig)) {
                implementation = sc.getMethod(sig);
                if (!implementation.isConcrete()) {
                    implementation = null;
                }
            } else {
                if (sc.hasSuperclass()) {
                    implementation = getImplementation(sc.getSuperclass(), sm);
                } 
            }
        }

        return implementation;
    }


    /** @return true if sub is a subclass of sup, or sub is a subclass of a
     * class that implements sup, if sup is an interface.
     */
    static public boolean subClassOrImplements(SootClass sub, SootClass sup) {
        SootClass subSup = sub;

        boolean found = subSup.equals(sup) || subSup.getInterfaces().contains(sup);

        while (subSup.hasSuperclass() && !found) {
            subSup = subSup.getSuperclass();        
            found = subSup.equals(sup) || subSup.getInterfaces().contains(sup);
        }

        return found;
    }

 
}

