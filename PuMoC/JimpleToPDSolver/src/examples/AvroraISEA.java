
package examples;

import avrora.actions.ISEAAction;

public class AvroraISEA {

    public static void main(String[] args) {
        try {
            ISEAAction action = new ISEAAction();
            action.run(args);
        } catch (Exception e) {
            // do nothing
        }
    }

}
