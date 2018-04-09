// CmdOptions.java
// Author: Matthew Hague        usage    25236                    read 22880
//
// A static class that reads command line arguments and makes them available to
// the rest of the program

package jimpletomoped.main;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Formatter;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Locale;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import jimpletomoped.translation.Namer;

import org.apache.log4j.Logger;

public class CmdOptions {

    static Logger logger = Logger.getLogger(CmdOptions.class);

    private static String[] classPaths = {};
    private static boolean noLabel = false;
    private static boolean outputRemopla = false;
    private static boolean outputPDS = false;
    private static List<String> sootOptions = new ArrayList<String>();
    private static String outfile_name = "j2p.out";


    private static Collection<String> acceptedSootOpts = new HashSet<String>();
    static {
        acceptedSootOpts.add("-O");
        acceptedSootOpts.add("-optimize");
        acceptedSootOpts.add("-W");
        acceptedSootOpts.add("-whole-optimize");
        acceptedSootOpts.add("-p");
        acceptedSootOpts.add("-verbose");
        acceptedSootOpts.add("-w");
    }
    private static Collection<String> ignoredClassPrefixes = new HashSet<String>();
    static {
        ignoredClassPrefixes.add("java.io");
        ignoredClassPrefixes.add("org.gjt.jclasslib");
        ignoredClassPrefixes.add("sun"); 
    }
    private static Collection<String> hasStub = new HashSet<String>();
    static {
		hasStub.add("java.lang.AssertionError");
		hasStub.add("java.lang.Character");
		hasStub.add("java.lang.Class");
		hasStub.add("java.lang.Enum");
		hasStub.add("java.lang.Error");
		hasStub.add("java.lang.Float");
		hasStub.add("java.lang.Double");
		hasStub.add("java.lang.IllegalArgumentException");
		hasStub.add("java.lang.Integer");
		hasStub.add("java.lang.InterruptedException");
		hasStub.add("java.lang.Long");
		hasStub.add("java.lang.Number");
		hasStub.add("java.lang.RuntimeException");
		hasStub.add("java.lang.String");
		hasStub.add("java.lang.StringBuilder");
		hasStub.add("java.lang.System");
		hasStub.add("java.lang.Thread");
		hasStub.add("java.lang.Throwable");
		hasStub.add("java.math.BigInteger");
		hasStub.add("java.util.Random");
        hasStub.add("java.io.ObjectStreamField");
    }
    private static String stubPackagePrefix = "jimpletomoped.stubs.";
    
    private static String methodSig = null;

    
    public static String usage() {
        String usage = "Usage: ./JimpleToMoped [options] methodSig\n";
        usage += "\n";
        usage += "Example: ./JimpleToMoped \"<examples.Flensburg: void quicksort(int[],int,int)>\"\n";
        usage += "Example: ./JimpleToMoped \"<examples.Factorial: void main(java.lang.String[])>\"\n";
        usage += "\n";
        usage += "Options:\n";
        usage += "\n";
        usage += formatOpt("-O, -optimize", 
                           "Perform intraprocedural optimisations.  This option is passed " +
                           "directly to Soot. See the Soot documentation for more details.");
        usage += formatOpt("-W, -whole-optimize",
                           "Perform whole program optimisations.  This option is passed " +
                           "directly to Soot.  See the Soot documentation for more details. " +
                           "Warning: whole program transformations require Soot to analyse " +
                           "the Java library; this takes some time.");
        usage += formatOpt("-p phaseOpt:val",
                           "Passes the given phase option and value to Soot.  There are " +
                           "many options.  See the Soot documentation for details.");
        usage += formatOpt("-verbose",
                           "Provides detailed information about the Soot processing. This " +
                           "option is passed directly to Soot.  See the Soot documentation for " +
                           "more details.");
        usage += formatOpt("-remopla",
                           "Output the Remopla translation generated to the file.");
        usage += formatOpt("-pds",
                           "Output the pushdown system constructed from the Remopla to the file.");
        usage += formatOpt("-f",
                           "Write the output to the given file, default: j2p.out");
        return usage;
    }

    
    public static List<String> getSootOptions() {
        return sootOptions;
    }

    public static boolean getOutputPDS() {
        return outputPDS;
    }
        
    /** @return the passed method signature to check (null if there isn't one)
     */
    public static String getMethodSignature() {
        return methodSig;
    }

    /** @return the list of class prefixes to ignore
     */
    public static Collection<String> getIgnoredClassPrefixes() {
        return ignoredClassPrefixes;
    }

    /** @return the collection of classes that have defined stubs
     */
    public static Collection<String> getStubList() {
        return hasStub;
    }

    /** @return the package prefix of the stubs package
     */
    public static String getStubPackagePrefix() {
        return stubPackagePrefix;
    }

    public static String[] getClassPaths() {
        return classPaths;
    }

    public static boolean getOutputRemopla() {
        return outputRemopla;
    }


    public static String getOutfileName() {
        return outfile_name;
    }

    /** Takes the command line args and processes them.  Results available
     * through get methods.
     *   @params args the String[] passed to main
     *   @return true if success, false otherwise
     */
    public static boolean readCommandLine(String[] args) {
        boolean hasSig = false;
        boolean ok = true;

        if (args.length < 1) 
            return false;

        for (int i = 0; i < args.length && ok; i++) {
        	if (acceptedSootOpts.contains(args[i])) {
                i = processSootArg(args, i);
                if (i < 0) {
                    ok = false;
                }
        	} else {
                if (args[i].equals("-remopla")) {
                    outputRemopla = true;
                } else if (args[i].equals("-pds")) {
                    outputPDS = true;
                } else if (args[i].equals("-f")) {
                    i++;
                    if (i >= args.length) 
                        logger.error("No output file specified after -f!");
                    else
                        outfile_name = args[i];
                } else if (!hasSig && Namer.isSignature(args[i])) {
                    hasSig = true;
                    methodSig = args[i];
                } else {
                    logger.error("Unexpected argument: " + args[i]);
                    ok = false;
                }
        	}
        }

        return ok && hasSig;
    }




    /////////////////////////////////////////////////////////////////////
    // Private Methods
    //

    // returns the last index of consumed args (for example -O will just return
    // i, but "-p pack option" will return (i+2))
    //
    // returns -1 in case of error
    private static int processSootArg(String[] args, int i) {
        if (args[i].equals("-p")) {
            if (i + 2 >= args.length) {
                logger.error("Soot option -p requires two arguments.");
                i = -1;
            } else {
                sootOptions.add(args[i]);
                sootOptions.add(args[i+1]);
                sootOptions.add(args[i+2]);
                i += 2;
            }
        } else {
            sootOptions.add(args[i]);
        }
        return i;
    }



    // formats the option
    private static String tab = "    ";

    private static String formatOpt(String option, String description) {
        String res = tab + option + "\n\n";
        return res + wordWrap(description, tab + tab, "") + "\n";
    }


    // Word wrapping code stolen from Keith at Joustlog, then modded
    private static final Pattern wrapRE = Pattern.compile(".{0,63}(?:\\S(?:-| |$)|$)");

    private static String wordWrap(String str, String prefix, String postfix) {
        String s = "";
        Matcher m = wrapRE.matcher(str);
        while (m.find())
            s += prefix + m.group() + postfix + "\n";
        return s;
    }



    
    
}
