// RemoplaToPDS.java
// Author: Matthew Hague
//
// A class for translating our Remopla representation to the PDS representation
// used by jwpds.  The strategy is similar to the strategy used by jMoped2, but
// not so tied to the particular translation (but tied to our "generic" Remopla
// representation).
//
//   RemoplaToPDS: Manages the translation.
//
//   StmtToRule: Translates a given statement to a PDS Rule.  The created rule
//   will have a weight that is an instance of ExprSemiring.
//
//   ExprSemiring: a simple class that is a "placeholder" semiring.  The
//   semiring operations do nothing.  It is more efficient to just add
//   placeholders at this stage.
//
//   BDDSemiring: the true semiring implementation.  When combining with another
//   semiring, it will check for the placeholders of type ExprSemiring and
//   update the BDD appropriately.  Like jMoped2, our BDDSemiring is tied to our
//   ExprSemiring class.


package jimpletomoped.remoplatopds;

import java.util.Iterator;
import java.util.Map;


import jimpletomoped.representation.Remopla;
import jimpletomoped.representation.RemoplaMethod;
import jimpletomoped.representation.RemoplaStmt;
import jimpletomoped.representation.Pds;
import jimpletomoped.representation.Rule;

public class RemoplaToPDS {

    /** Translates a jimpletomoped.representation.Remopla object into a
     * de.tum.in.wpds.Pds object with jimpletomoped.remoplatopds.ExprSemiring
     * weights.  The class jimpletoremople.BDDSemiring provides a concrete semiring
     * that can be combined with ExprSemiring weights to produce valid results.
     */
    static public Pds translate(Remopla remopla) {
        StmtToRules str = new StmtToRules();

        Pds pds = new Pds();
        str.addInitialCall(pds, remopla);
        addGlobalRules(pds, remopla, str);
        addMethods(pds, remopla, str);

        return pds;
    }


    //////////////////////////////////////////////////////////////////////////////
    // Private methods


    static private void addGlobalRules(Pds pds, Remopla remopla, 
                                       StmtToRules str) {
        for (RemoplaStmt floater : remopla.getFloatingStmts()) {
            str.translateStmt(floater, null, remopla, pds);
        }
    }

    static private void addMethods(Pds pds, Remopla remopla, 
                                   StmtToRules str) {
        for (RemoplaMethod method : remopla.getMethods()) {
            Iterator stmts = method.getStmts().iterator();
            RemoplaStmt nextStmt = stmts.hasNext() ? (RemoplaStmt)stmts.next() : null;
            while (nextStmt != null) {
                RemoplaStmt stmt = nextStmt;
                nextStmt = stmts.hasNext() ? (RemoplaStmt)stmts.next() : null;
                str.translateStmt(stmt, nextStmt, remopla, pds);
            }
        }
    }
}
