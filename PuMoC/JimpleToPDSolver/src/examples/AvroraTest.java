
package examples;

import avrora.actions.TestAction;

public class AvroraTest {

    public static void main(String[] args) {
        try {
            TestAction action = new TestAction();
            action.run(args);
        } catch (Exception e) {
            // do nothing
        }
    }

}
