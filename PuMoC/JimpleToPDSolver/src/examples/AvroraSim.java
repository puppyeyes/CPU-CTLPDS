
package examples;

import avrora.actions.SimAction;

public class AvroraSim {

    public static void main(String[] args) {
        try {
            SimAction action = new SimAction();
            action.run(args);
        } catch (Exception e) {
            // do nothing
        }
    }

}
