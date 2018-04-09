
package examples;

import avrora.actions.RegisterTestAction;

public class AvroraReg {

    public static void main(String[] args) {
        try {
            RegisterTestAction action = new RegisterTestAction();
            action.run(args);
        } catch (Exception e) {
            // do nothing
        }
    }

}
