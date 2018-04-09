
package examples;

import avrora.actions.AnalyzeStackAction;

public class AvroraStack {

    public static void main(String[] args) {
        try {
            AnalyzeStackAction action = new AnalyzeStackAction();
            action.run(args);
        } catch (Exception e) {
            // do nothing
        }
    }

}
