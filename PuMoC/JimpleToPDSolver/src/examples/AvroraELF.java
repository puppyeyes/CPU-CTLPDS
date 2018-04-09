
package examples;

import avrora.actions.ELFDumpAction;

public class AvroraELF {

    public static void main(String[] args) {
        try {
            ELFDumpAction action = new ELFDumpAction();
            action.run(args);
        } catch (Exception e) {
            // do nothing
        }
    }

}
