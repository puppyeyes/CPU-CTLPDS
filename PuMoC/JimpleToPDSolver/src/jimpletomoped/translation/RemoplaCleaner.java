// RemoplaCleaner.java
// Author: Matthew Hague
//
// Applies any final changes to a remopla translation to make it tidier
// For example, runs all expressions through an expression normaliser.

package jimpletomoped.translation;

import java.util.Iterator;

import jimpletomoped.representation.*;

import org.apache.log4j.Logger;

public class RemoplaCleaner extends AbstractRemoplaStmtVisitor {
    static Logger logger = Logger.getLogger(RemoplaCleaner.class);

    static final private String DELETED_LABEL = "<!XXX#DELETED#XXX!>";

    private RemoplaStmt nextStmt = null;

    /**
     * @param remopla
     *            changes given remopla object, replacing untidiness with
     *            tidiness... Initial implementation simply replaces all
     *            expressions with their normalised versions.
     */
    public void cleanRemopla(Remopla remopla) {
        for (RemoplaMethod method : remopla.getMethods()) {
            nextStmt = null;

            Iterator i = method.getStmts().iterator();
            if (i.hasNext()) {
                nextStmt = (RemoplaStmt)i.next();
            }
            while (nextStmt != null) {
                RemoplaStmt stmt = nextStmt;
                nextStmt = i.hasNext() ? (RemoplaStmt)i.next() : null;
                cleanStatement(stmt);
            }
        }
    }

    /**
     * {@inheritDoc}
     * 
     * @see AbstractRemoplaStmtVisitor#defaultCase(RemoplaStmt)
     */
    public void defaultCase(RemoplaStmt obj) {
        // do nothing
    }

    /**
     * {@inheritDoc}
     * 
     * @see RemoplaStmtVisitor#visit(RemoplaIfStmt)
     */
    public void visit(RemoplaBranchStmt obj) {
        for (RemoplaBranchStmt.Branch b : obj.getBranches()) {
            for (RemoplaStmt stmt : b.getStmts()) {
                cleanStatement(stmt);
            }
        }
    }

    /**
     * {@inheritDoc}
     * 
     * @see RemoplaStmtVisitor#visit(RemoplaCallStmt)
     */
    public void visit(RemoplaCallStmt obj) {
        // Do nothing -- we don't expect parameters to be of a particularly
        // normalisable form (usually just variables)
    }

    /**
     * {@inheritDoc}
     * 
     * @see RemoplaStmtVisitor#visit(RemoplaReturnStmt)
     */
    public void visit(RemoplaReturnStmt obj) {
        // do nothing
    }

    /**
     * {@inheritDoc}
     * 
     * @see RemoplaStmtVisitor#visit(RemoplaSkipStmt)
     */
    public void visit(RemoplaSkipStmt obj) {
//        if (nextStmt != null && nextStmt instanceof RemoplaSkipStmt) {
//            Collection<String> ourUses = obj.getUses();
//            Collection<String> theirUses = nextStmt.getUses();
//            Collection<String> ourDefs = obj.getDefs();
//            Collection<String> theirDefs = nextStmt.getDefs();

//            if (ourUses.equals(theirUses) && ourDefs.equals(theirDefs)) {
//                logger.info("HITHITHITHITTTTT");
//                String ourLabel = obj.getLabel();
//                String theirLabel = nextStmt.getLabel();

//                obj.setLabel(DELETED_LABEL);
//                replaceAllLabels(ourLabel, theirLabel);
//            }
//        }
    }


    public void cleanStatement(RemoplaStmt stmt) {
        stmt.accept(this);
    }


    private void replaceAllLabels(String oldLabel, String newLabel) {

    }
}
