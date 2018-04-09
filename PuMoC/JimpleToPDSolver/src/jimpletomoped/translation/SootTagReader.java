// SootTagReader.java
// Author: Matthew Hague
//
// Provides static methods for extracting information from a soot Tag


package jimpletomoped.translation;

import java.util.Collection;

import org.apache.log4j.Logger;

import soot.Local;

import soot.jimple.Stmt;

import soot.tagkit.Tag;


public class SootTagReader {
    static Logger logger = Logger.getLogger(SootTagReader.class);

    private static final String SOOT_LINE_NUMBER_TAG = "LineNumberTag";


    public static String getLineNumber(Stmt stmt) {
        Tag lineNo = (Tag)stmt.getTag(SOOT_LINE_NUMBER_TAG);
        String lineNoString = null;
        if (lineNo != null) {
            lineNoString = lineNo.toString();
        }
        return lineNoString;
    }

 }
