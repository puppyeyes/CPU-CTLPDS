// Namer.java
// Author: Matthew Hague
//
// Does the naming duties for the remople to pds translation

package jimpletomoped.remoplatopds;

public class Namer {

    private static final String DEFAULT_LABEL = "DEFAULT_";
    private static final String CONTROL_STATE = "q";
    private static final String INIT_STATE = "init";
    private static final String END_STATE = "end";
    private static final String RETURN_HANDLER = "RETURN_HANDLER_";
    private static final String INIT_STACK = "#";

    private static int nextLabel = 0;
    private static int nextReturn = 0;

    public static String getNewLabel() {
        return DEFAULT_LABEL + (nextLabel++);
    }


    public static String getControlState() {
        return CONTROL_STATE;
    }


    public static String getNewReturnHandler() {
        return RETURN_HANDLER + (nextReturn++);
    }

    public static String getInitStack() {
        return INIT_STACK;
    }

    public static String getInitControl() {
        return INIT_STATE;
    }


    public static String getEndControl() {
        return END_STATE;
    }
}
