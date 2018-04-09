/* ToMopedAnalysis class for Jimple To Moped conversion
 * Written by: Matthew Hague 
 * 
 * Heavily based on the fooanalasys example from
 * 
 * 		A Survivor's Guide to Java Program Analysis with Soot 
 * 		A. Einarsson and J. D. Nielsen
 */


package jimpletomoped.tomopedanalysis;

import java.io.IOException;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import jimpletomoped.main.CmdOptions;

import jimpletomoped.translation.ClassManager;
import jimpletomoped.translation.SootClassPreloader;
import jimpletomoped.translation.SootMainDummy;
import jimpletomoped.translation.SootMainDummyHelpers;
import jimpletomoped.translation.ToMopedTranslator;

import jimpletomoped.representation.Remopla;

import org.apache.log4j.Logger;

import soot.ArrayType;
import soot.Body;
import soot.IntType;
import soot.Local;
import soot.PackManager;
import soot.PhaseOptions;
import soot.Scene;
import soot.SootClass;
import soot.SootMethod;
import soot.Transform;
import soot.Type;
import soot.Unit;
import soot.ValueBox;

import soot.jimple.Jimple;
import soot.jimple.StaticInvokeExpr;

import soot.options.Options;

import soot.util.Chain;

public class ToMopedAnalysis {

    static Logger logger = Logger.getLogger(ToMopedAnalysis.class);

    private ToMopedTranslator mt;
    private Map/*<ValueBox,String>*/  sourcefile_map;
    private Map/*<ValueBox,String>*/  class_map;
    private Map/*<ValueBox,String>*/  method_map;
    private Map/*<ValueBox,Integer>*/ line_map;

    private Remopla remopla = null;

   
    /** Performs a translation to Moped on the current application classes,
     * The translated Remopla can be retrieved by getRemopla().  This will be
     * null if there was a problem.
     * @param methodSig the method signature as described in
     * Namer.makeSignature
     * @throws IOException 
     */
    public ToMopedAnalysis(String methodSig) {
        initialiseSoot(methodSig);
    	mt = new ToMopedTranslator();
        SootMethod sm = ClassManager.loadMethod(methodSig);
        if (sm != null) {
            remopla = mt.translateMethods(sm);
        } else {
            logger.error("Could not load " + methodSig + "\nCheck signature against example usage.  Spaces are important.");
        }
    }

    /** Returns the Remopla object representing the analysed classes
      * @return the Remopla object	
      */
    public Remopla getRemopla() {
        return remopla;
    }


    
    /** Returns the name of the source file containing the given expression.
     *  @param box the expression.
     *  @return the source file name.
     */
    public final String getSourceFile(ValueBox box) {
    	return (String)sourcefile_map.get(box);
    }

    /** Returns the name of the class containing the given expression.
     *  @param box the expression.
     *  @return the fully qualified class name.
     */
    public final String getClassName(ValueBox box) {
    	return (String)class_map.get(box);
    }

    /** Returns the name of the method containing the given expression.
     *  @param box the expression.
     *  @return the method name.
     */
    public final String getMethodName(ValueBox box) {
    	return (String)method_map.get(box);
    }

    /** Returns the source line number of the given expression.
     *  @param box the expression.
     *  @return the line number.
     */
    public final int getLineNumber(ValueBox box) {
    	return ((Integer)line_map.get(box)).intValue();
    }


    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Private methods
    //

    private void initialiseSoot(String initMethodSig) {
        initialiseOptions();
        initialisePhaseOptions();
        Scene.v().loadBasicClasses();
        // TODO: experiment with soot -app option. (Or whichever option treats
        // all reference classes as application classes.)
        (new SootClassPreloader()).preloadMethodAndSupport(initMethodSig);
        initialiseMainDummy(initMethodSig);
        Scene.v().loadNecessaryClasses();
        PackManager.v().runPacks();
    }

    private void initialiseOptions() {
        List<String> optionsList = CmdOptions.getSootOptions();
        String[] optionsArr = new String[optionsList.size()];
        optionsList.toArray(optionsArr);
        Options.v().parse(optionsArr);

        Options.v().set_keep_line_number(true);
        Options.v().set_output_format(Options.output_format_jimple);

        Options.v().set_main_class(SootMainDummy.class.getName());

    }

    private void initialisePhaseOptions() {
        PhaseOptions.v().setPhaseOption("jb", "use-original-names:true");
    }

    private void initialiseMainDummy(String initMethodSig) {
        SootClass dummyMain = Scene.v().loadClassAndSupport(SootMainDummy.class.getName());
        SootClass dummyHelpers = Scene.v().loadClassAndSupport(SootMainDummyHelpers.class.getName());

        Scene.v().setMainClass(dummyMain);  
 
        SootMethod main = Scene.v().getMainMethod();
        SootMethod init = ClassManager.loadMethod(initMethodSig);
        SootMethod randInt = dummyHelpers.getMethodByName(SootMainDummyHelpers.RAND_INT);
        SootMethod randArr = dummyHelpers.getMethodByName(SootMainDummyHelpers.RAND_ARRAY);

        Body mainBody = main.retrieveActiveBody();
        Chain<Unit> units = mainBody.getUnits();

        List emptyArgs = new ArrayList(0);
        List args = new ArrayList(init.getParameterCount());
        for (int i = 0; i < init.getParameterCount(); i++) {
            Type paramType = init.getParameterType(i);
            if (paramType instanceof ArrayType) {
                StaticInvokeExpr argExpr = Jimple.v().newStaticInvokeExpr(randArr.makeRef(), emptyArgs);
                Local arg = Jimple.v().newLocal("arg" + i, ArrayType.v(IntType.v(), 1));
                mainBody.getLocals().add(arg);
                units.add(Jimple.v().newAssignStmt(arg, argExpr));
                args.add(arg);
            } else if (paramType instanceof IntType) {
                StaticInvokeExpr argExpr = Jimple.v().newStaticInvokeExpr(randInt.makeRef(), emptyArgs);
                Local arg = Jimple.v().newLocal("arg" + i, IntType.v());
                mainBody.getLocals().add(arg);
                units.add(Jimple.v().newAssignStmt(arg, argExpr));
                args.add(arg);
            } else {
                logger.error("ToMopedAnalysis.initialiseMainDummy found parameter of unsupported type. Adding null...");
                args.add(null);
            }
        }

        units.add(Jimple.v().newInvokeStmt(Jimple.v().newStaticInvokeExpr(init.makeRef(), args)));
    }
}
