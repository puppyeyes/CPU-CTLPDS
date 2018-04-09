
package examples;

import avrora.actions.MediumTestAction;

public class AvroraMedTest {

    public static void main(String[] args) {
        try {
            MediumTestAction action = new MediumTestAction();
            action.run(args);
        } catch (Exception e) {
            // do nothing
        }
    }

}
