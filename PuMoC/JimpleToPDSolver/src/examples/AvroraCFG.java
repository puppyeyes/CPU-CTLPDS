
package examples;

import avrora.actions.CFGAction;

public class AvroraCFG {

    public static void main(String[] args) {
        try {
            CFGAction action = new CFGAction();
            action.run(args);
        } catch (Exception e) {
            // do nothing
        }
    }

}
