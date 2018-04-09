
package examples;

import avrora.actions.DisassembleAction;

public class AvroraDisassemble {

    public static void main(String[] args) {
        try {
            DisassembleAction action = new DisassembleAction();
            action.run(args);
        } catch (Exception e) {
            // do nothing
        }
    }

}
