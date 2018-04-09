// ExprTranslator.java
// Author: Matthew Hague
//
// Translates expressions.  Based on soot example code.


package jimpletomoped.translation;

import java.util.List;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Collection;
import java.util.HashSet;

import org.apache.log4j.Logger;

import soot.Value;
import soot.Local;
import soot.SootClass;
import soot.SootField;
import soot.ArrayType;
import soot.RefType;
import soot.SootMethod;

import soot.jimple.AbstractJimpleValueSwitch;
import soot.jimple.AddExpr;
import soot.jimple.SubExpr;
import soot.jimple.MulExpr;
import soot.jimple.DivExpr;
import soot.jimple.NegExpr;
import soot.jimple.AndExpr;
import soot.jimple.OrExpr;
import soot.jimple.XorExpr;
import soot.jimple.BinopExpr;
import soot.jimple.UnopExpr;
import soot.jimple.DoubleConstant;
import soot.jimple.FloatConstant;
import soot.jimple.IntConstant;
import soot.jimple.LongConstant;
import soot.jimple.EqExpr;
import soot.jimple.GeExpr;
import soot.jimple.GtExpr;
import soot.jimple.LeExpr;
import soot.jimple.LtExpr;
import soot.jimple.NeExpr;
import soot.jimple.ParameterRef;
import soot.jimple.ShlExpr;
import soot.jimple.ShrExpr;
import soot.jimple.UshrExpr;
import soot.jimple.StaticFieldRef;
import soot.jimple.RemExpr;
import soot.jimple.NewArrayExpr;
import soot.jimple.ArrayRef;
import soot.jimple.NewExpr;
import soot.jimple.InstanceFieldRef;
import soot.jimple.InstanceOfExpr;
import soot.jimple.InvokeExpr;
import soot.jimple.StaticInvokeExpr;
import soot.jimple.InstanceInvokeExpr;
import soot.jimple.SpecialInvokeExpr;
import soot.jimple.VirtualInvokeExpr;
import soot.jimple.InterfaceInvokeExpr;
import soot.jimple.ThisRef;
import soot.jimple.LengthExpr;
import soot.jimple.NullConstant;
import soot.jimple.StringConstant;
import soot.jimple.CaughtExceptionRef;
import soot.jimple.ClassConstant;
import soot.jimple.CastExpr;
import soot.jimple.CmpExpr;           
import soot.jimple.CmpgExpr;
import soot.jimple.CmplExpr;
     

import jimpletomoped.representation.*;
import static jimpletomoped.representation.RemoplaStaticFactory.*;

import jimpletomoped.main.CmdOptions;

public class ExprTranslator extends AbstractJimpleValueSwitch {

    static Logger logger = Logger.getLogger(ExprTranslator.class);

    private String NOT_IMPLEMENTED = " not implemented."; 


    // if a static field is accessed, the initialisation method needs to handle
    // it correctly (that is, set it to an appropriate value)
    private Collection<SootClass> staticAccesses = new HashSet<SootClass>();

    private SootMethod currentMethod = null;


    // Policy of defs vs. uses: all variable occurrences are considered a use
    // unless explicitly a def.  E.g. a method call a.f() constitutes an
    // explicit def since f() may modify a.
    //
    // If defining is true, then it means the current expression is being
    // assigned to, hence the outermost part is a def.  E.g. a[i] = x defines to
    // a but not i.

    private boolean defining = false;

    public class DefUses {
        private Collection<String> uses = new HashSet<String>();
        private Collection<String> defs = new HashSet<String>();

        public DefUses() {
            // do nothing
        }

        public DefUses(DefUses du1, DefUses du2) {
            assert du1 != null && du2 != null;
            uses.addAll(du1.getUses());
            uses.addAll(du2.getUses());
            defs.addAll(du1.getDefs());
            defs.addAll(du2.getDefs());
        }

        public void addUse(String var) {
            assert var != null;
            uses.add(var);
        }

        public void addDef(String var) {
            assert var != null;
            defs.add(var);
        }

        public Collection<String> getUses() {
            return uses;
        }

        public Collection<String> getDefs() {
            return defs;
        }

        public void addDefUses(DefUses du) {
            assert du != null;
            uses.addAll(du.getUses());
            defs.addAll(du.getDefs());
        }
    }



	public ExprTranslator() {
	}

    /** translates an expression to Moped.  If recordArrayAccesses or
      * recordObjectAccesses are set, then the corresponding information is recorded
      * and can be obtained by getArrayAccesses and getObjectAccesses.
      *  @param expr the expression to analyze
      *  @param currentMethod
      *  @param defining the expression is being assigned to (so a[i] should
      *  could array a as a def)
      *  @return none -- stored in getObject as a Collection<String> of used ids
      */
	public void analyseExpr(Value expr, SootMethod currentMethod, boolean defining) {
        boolean old_def = this.defining;
        SootMethod old_method = this.currentMethod;
        this.defining = defining;
        this.currentMethod = currentMethod;
		expr.apply(this);
        this.defining = old_def;
        this.currentMethod = old_method;
	}

    public DefUses getEmptyDefUses() { 
        return new DefUses();
    }



    /** The classes who had a static field accessed in some way since the expression translator has been
     * translating statements.  Call clearStaticAccesses() to prevent aggregation.
     */
    public Collection<SootClass> getStaticAccesses() { 
        return staticAccesses;
    }

    /** The statement translated collates the signatures of the methods that
     * translated statements have called.  This clears the record.
     */
    public void clearStaticAccesses() {
        staticAccesses.clear();
    }



    ///////////////////////////////////////////////////////////////////////////////////////
    // Atoms
    //

    /** Doubles are not supported and will be converted to integers
      */
    public void caseDoubleConstant(DoubleConstant dbl) {
        registerResult(null);
    }

     /** Floats are not supported and will be converted to integers
      */
    public void caseFloatConstant(FloatConstant flt) {
        registerResult(null); 
    }

    public void caseIntConstant(IntConstant i) {
        registerResult(null);
    }

    public void caseLongConstant(LongConstant i) {
        registerResult(null);
    }


	public void caseLocal(Local var) {
        registerResultSingle(Namer.getLocalVariableName(var, currentMethod));
	}

    // since parameters are just indexes later assigned to a variable, they
    // don't count as use or def
    public void caseParameterRef(ParameterRef par) {
        registerResult(null);        
    }

    /** Static fields are stored as global variables */
    public void caseStaticFieldRef(StaticFieldRef field) {
        staticAccesses.add(field.getField().getDeclaringClass());
        String name = Namer.getGlobalVariableName(field);
        registerResultSingle(name);
    }


    public void caseArrayRef(ArrayRef aref) {
        DefUses base = doExpr(aref.getBase(), defining);
        DefUses index = doExpr(aref.getIndex(), false);

        registerResult(new DefUses(base, index));
    }


    public void caseInstanceOfExpr(InstanceOfExpr io) {
        registerResult(doExpr(io.getOp(), false));
    }
      

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Arithmetic 
    //

    public void caseAddExpr(AddExpr add) {
        caseBinOp(add);
    }

    public void caseSubExpr(SubExpr sub) {
        caseBinOp(sub);
    }
   
    public void caseMulExpr(MulExpr mul) {
        caseBinOp(mul);
    }

    public void caseDivExpr(DivExpr div) {
        caseBinOp(div);
    }


    public void caseShlExpr(ShlExpr shl) {
        caseBinOp(shl);
    }

    public void caseShrExpr(ShrExpr shr) {
        caseBinOp(shr);
    }

    public void caseUshrExpr(UshrExpr ushr) {
        caseBinOp(ushr);
    }

    public void caseRemExpr(RemExpr rem) {
        caseBinOp(rem);
    }


    //////////////////////////////////////////////////////////////////////////////////////////
    // Logic
    //

    public void caseNegExpr(NegExpr neg) {
        caseUnaryOp(neg);
    }

    public void caseAndExpr(AndExpr and) {
        caseBinOp(and);
    }

    public void caseOrExpr(OrExpr or) {
        caseBinOp(or);
    }

    public void caseXorExpr(XorExpr xor) {
        caseBinOp(xor);
    }

    public void caseEqExpr(EqExpr eq) {
        caseBinOp(eq);
    }

    public void caseGeExpr(GeExpr ge) {
        caseBinOp(ge);
    }

    public void caseGtExpr(GtExpr gt) {
        caseBinOp(gt);
    }

    public void caseLeExpr(LeExpr le) {
        caseBinOp(le);
    }

    public void caseLtExpr(LtExpr lt) {
        caseBinOp(lt);
    }

    public void caseNeExpr(NeExpr ne) {
        caseBinOp(ne);
    }

    public void caseCmpExpr(CmpExpr v) {
        caseBinOp(v);
    }
           
    public void caseCmpgExpr(CmpgExpr v) {
        caseBinOp(v);
    }
           
    public void caseCmplExpr(CmplExpr v) {
        caseBinOp(v);
    }
    

    /////////////////////////////////////////////////////////////////////////////////////
    // Methods, classes, calls, arrays &c.
    //

    public void caseThisRef(ThisRef expr) {
        registerResultSingle(Namer.getThisVariableName());
    }



    public void caseNewArrayExpr(NewArrayExpr newarr) {
        registerResult(doExpr(newarr.getSize(), false));
    }

    public void caseNewExpr(NewExpr expr) {
        // a new expr just allocates space, i think
        registerResult(null);
    }

    public void caseInstanceFieldRef(InstanceFieldRef field) {
        DefUses base = doExpr(field.getBase(), defining);
        String name = Namer.getClassFieldName(field.getField());
        if (defining)
            base.addUse(name);
        else
            base.addDef(name);
        registerResult(base);
    }


    public void caseNullConstant(NullConstant expr) {
        registerResult(null);
    }


    public void caseStringConstant(StringConstant str) {
        registerResult(null);
    }


    // these are like parameter refs, i think -- should be ignored.
    public void caseCaughtExceptionRef(CaughtExceptionRef e) {
        registerResult(null);
    }


    public void caseLengthExpr(LengthExpr len) {
        registerResult(doExpr(len.getOp(), false));
    }


    public void caseClassConstant(ClassConstant expr) {
        String className = expr.getValue();
        if (!className.startsWith("[")) {
            SootClass c = ClassManager.loadClassWithSlashes(expr.getValue());
            staticAccesses.add(c);
        }
        registerResult(null);
    }


    public void caseCastExpr(CastExpr cast) {
        registerResult(doExpr(cast.getOp(), defining));
    }

    /////////////////////////////////////////////////////////////////////////////////////
    // Default
    //

    public void defaultCase(Object expr) {
        logger.warn("Expression feature [" + expr + " : " + expr.getClass() + "]" + NOT_IMPLEMENTED);
        registerResult(null);
    }



    ////////////////////////////////////////////////////////////////////////////////////
    // Private methods
    //

    public void caseBinOp(BinopExpr expr) {
        registerResult(new DefUses(doExpr(expr.getOp1(), false), 
                                   doExpr(expr.getOp2(), false)));
    }

    public void caseUnaryOp(UnopExpr expr) {
        registerResult(doExpr(expr.getOp(), false));
    }

    private DefUses doExpr(Value expr, boolean defining) {
        analyseExpr(expr, currentMethod, defining);        
        return (DefUses)getResult();
    }


    private void registerResultSingle(String obj) {
        if (defining) {
            registerResultDef(obj);
        } else {
            registerResultUse(obj);
        }
    }

    private void registerResultUse(String obj) {
        if (obj == null) {
            setResult(new DefUses());
        } else {
            DefUses one = new DefUses();
            one.addUse(obj);
            setResult(one);
        }
    }

    private void registerResultDef(String obj) {
        if (obj == null) {
            setResult(new DefUses());
        } else {
            DefUses one = new DefUses();
            one.addDef(obj);
            setResult(one);
        }
    }


    private void registerResult(DefUses dus) {
        if (dus == null) {
            setResult(new DefUses());
        } else {
            setResult(dus);
        }
    }
}

