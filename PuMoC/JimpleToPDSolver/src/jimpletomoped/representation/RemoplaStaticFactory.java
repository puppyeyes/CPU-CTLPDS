// RemoplaStaticFactory.java
// Author: Matthew Hague
//
// A collection of static constructors for Remopla code.  For convenience.


package jimpletomoped.representation;

import java.util.ArrayList;
import java.util.List;


public class RemoplaStaticFactory {

    public static RemoplaBranchStmt rbranch(RemoplaStmt... branches) {
        RemoplaBranchStmt branch = new RemoplaBranchStmt();
        for (int i = 0; i < branches.length; i++) {
            branch.addBranch(branches[i]);
        }
        return branch;
    }

    public static RemoplaGotoStmt rgoto(String label) {
        return new RemoplaGotoStmt(label);
    }

    /** Convenience method for constructor **/
    public static RemoplaMethod method(String methodName) {
        return new RemoplaMethod(methodName);
    }

    public static RemoplaCallStmt call(String callString) {
        return new RemoplaCallStmt(callString);
    }

    static public RemoplaReturnStmt rreturn() {
        return new RemoplaReturnStmt();
    }

    /** @param guard the guard of the skip statement
     * @return a new RemoplaSkipStmt with the given guard
     */
    static public RemoplaSkipStmt rskip() {
        return new RemoplaSkipStmt();
    }


    static public RemoplaNullStmt rnull(String warning) {
        return new RemoplaNullStmt(warning);
    }

    static public List<RemoplaStmt> block(RemoplaStmt... stmts) {
        List<RemoplaStmt> block = new ArrayList<RemoplaStmt>(stmts.length);
        for (int i = 0; i < stmts.length; i++) {
            block.add(stmts[i]);
        }
        return block;
    }
}
