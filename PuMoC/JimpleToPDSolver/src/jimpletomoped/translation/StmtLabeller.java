// StmtLabeller.java
// Author: Matthew Hague


package jimpletomoped.translation;

import java.lang.CloneNotSupportedException;

import org.apache.log4j.Logger;

import jimpletomoped.representation.RemoplaStmt;

import jimpletomoped.main.CmdOptions;

public abstract class StmtLabeller implements Cloneable {

    static Logger logger = Logger.getLogger(StmtLabeller.class);

    public StmtLabeller clone() {
        StmtLabeller s = null;
        try {
            s = (StmtLabeller)super.clone();
        } catch (CloneNotSupportedException e) {
            logger.error("CloneNotSupportedException when cloning instance of StmtLabeller!");
        }

        return s;
    } 

    /** Adds a label to the current stmt 
     *  @return the stmt object, for convenience
     */
    public abstract RemoplaStmt label(RemoplaStmt stmt);

}
