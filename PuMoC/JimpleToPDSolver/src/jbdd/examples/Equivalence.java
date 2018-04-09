// JBDD example code
package jbdd.examples;
import jbdd.*;



/**
 * This is an extremely simple example on equivalence checking
 * of combinatorial circuits.
 *
 * we check whether the two circuits
 *       c1 = v0 AND v1      and     c2 = NOT(NOT va OR NOT vb)
 * are "equal", in the sense that for the same input, they will
 * generate equal outputs.
 *
 * coded by Arash Vahidi (vahidi (a) s2.chalmers.se)
 */



public class Equivalence extends JBDD {

	private int c1; // first circuit
	private int v0, v1; // inputs for the first circuit

	private int c2; // second circuit
	private int va, vb; // inputs fir the second circuit


	public Equivalence() {
		super(4);

		v0 = createBDD();
		v1 = createBDD();

		va = createBDD();
		vb = createBDD();

		// C1 = v0 AND v1
		c1 = and(v0,v1);

		// C2 = NOT(NOT va OR NOT vb)
		int tmp1 = not(va);
		int tmp2 = not(vb);
		int tmp3 = or(tmp1,tmp2);
		c2 = not(tmp3);
		deref(tmp1);deref(tmp2);deref(tmp3);

	}

	public void cleanup() {
		deref(c1);
		deref(v0);
		deref(v1);
		deref(c2);
		deref(va);
		deref(vb);
	}

	public void show() {
		System.out.println("These two (DAGs/formulas) should be isomorphic if 0 <==> 2 and 1 <==> 3 :");
		print(c1);
		print(c2);

		// if the inputs is the same
		int tmp1 = biimp(v0,va);
		int tmp2 = biimp(v1,vb);
		int inputs_equal = and(tmp1,tmp2);
		deref(tmp1); deref(tmp2);

		// ... the outputs are equal:
		int outputs_equal = biimp(c1,c2);

		// same inputs --> same outputs
		int equality = imp(inputs_equal, outputs_equal);

	System.out.println();

		// check the results
		if(equality == getOne()) { // ==> ALWAYS OK
			System.out.println("Two circuits are equal!");
		} else { // ==> THERE IS AT LEAST ONE CASE WHERE THEY ARE NOT EQUAL
			int not_equal = not(equality);
			System.out.println("Two circuits are NOT equal, here are all the counterexamples:");
			printSet(not_equal);
			deref(not_equal);
		}

		// cleanup:
		deref(equality);
		deref(outputs_equal);
		deref(inputs_equal);


	}
	public static void main(String [] args) {

		Equivalence e = new Equivalence();
		e.show();
		e.cleanup();

	}
}
