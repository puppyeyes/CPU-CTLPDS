/* Main class for Jimple To Moped conversion
 * Written by: Matthew Hague 
 * 
 * Heavily based on the fooanalasys example from
 * 
 * 		A Survivor's Guide to Java Program Analysis with Soot
 * 		A. Einarsson and J. D. Nielsen
 */


package jimpletomoped.main;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Set;

import java.io.FileOutputStream;
import java.io.PrintStream;

import jimpletomoped.representation.Remopla;
import jimpletomoped.representation.RemoplaStmt;
import jimpletomoped.representation.Pds;

import jimpletomoped.remoplatopds.RemoplaToPDS;

import jimpletomoped.tomopedanalysis.ToMopedAnalysis;

import jimpletomoped.translation.Namer;

import org.apache.log4j.Logger;



public class Main {

    static Logger logger = Logger.getLogger(Main.class);

	/** 
	 * 
	 * @param args 
	 * @throws Exception 
	 */
	public static void main(String[] args) throws Exception {		
			String program_name = null;

            if (doProcessCommandLine(args)) {
                doJimpleToPDSolverTranslate();
            } else {
                logger.error(CmdOptions.usage());
            }
	}


    ///////////////////////////////////////////////////////////////
    // Private Methods
    //

    private static void doJimpleToPDSolverTranslate() {
        Remopla remopla = doJimpleToRemopla();

        try{

            PrintStream out = new PrintStream(new FileOutputStream(CmdOptions.getOutfileName()));

            if (remopla != null) {
                if (CmdOptions.getOutputRemopla()) {
                    remopla.toString(out);
                }

                Pds pds = getPds(remopla);
                pds.toString(out);
            } else {
                logger.error("An error occurred");
            }

            out.close();
        } catch (Exception e) {
            logger.error("An exception occurred: " + e);
        }
    }




    /** Assumes CmdOptions has been set up correctly (doProcessCommandLine).  Performs the translation
     * to a Remopla object 
     */
    public static Remopla doJimpleToRemopla() {
        String methodSig = CmdOptions.getMethodSignature();
        ToMopedAnalysis tm = new ToMopedAnalysis(methodSig);
        return tm.getRemopla();
    }


    /** Given the command line (broken up a-la java), sets up CmdOptions,
     * returns false if an error occured
     */
    public static boolean doProcessCommandLine(String[] args) {
        return CmdOptions.readCommandLine(args);
    }


    public static Pds getPds(Remopla remopla) {
        return RemoplaToPDS.translate(remopla);
    }
}




