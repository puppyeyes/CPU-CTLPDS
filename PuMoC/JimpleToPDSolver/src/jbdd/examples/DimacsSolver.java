// JBDD example code
package jbdd.examples;
import jbdd.*;

import java.io.*;
import java.util.*;

/**
 * This is another simple JBDD example on working with BDD:s.
 * <p>
 * a very simple 'SAT-solver' that reads a DIAMCS file and gives a minterm satisfying
 *  the CNF formula in the file if SAT, or UNSAT if no such minterm exists.
 * <p><b>Note:</b> "education purpose only", dont use this slooow solver on any real-world problems.
 * Speacking of which, here are two DIMACS examples that are just large enough for this solver :)
 * <p>here is a SAT one: <i>v1 AND NOT v2</i>
 * <pre>
 * p cnf 2 2     <-- 2 literals and two cluases
 * 1 0           <-- this is a clause with one literal, v1
 * -2 0          <-- each clause must end with a 0
 * </pre>
 *
 * <p>and a UNSAT: <i>(v1 or v2) AND (NOT v1 or b2) AND (NOT v2)</i>
 * <pre>
 * p cnf 2 3
 * 1 2 0
 * -1 2 0
 *  * -2 0
 * </pre>
 */

public class DimacsSolver {
	private JBDD jbdd = null;	/** the BDD interface */
	private int n_lits, n_cls, bdd_all; /* number of literals, clauses and the BDD for our CNF formula */
	private int [] vars, not_vars; /* vars and negated vars pre-computed and stored here */

	/** read ans solve the problem in this file, throw IOException if Math.random() > 0.5, no jk :)  */
	public DimacsSolver(String file) throws IOException {
		// ------------------- load dimacs file and build the formula
		BufferedReader br = new BufferedReader ( new FileReader( file) );

		// Read the first line
		StringTokenizer tmp = getLine(br);
		assure(tmp, "p");
		assure(tmp, "cnf");

		n_lits = getInt(tmp);
		n_cls  = getInt(tmp);

		// alloc BDD vars
		jbdd = new JBDD(n_lits);
		vars = new int[n_lits];
		not_vars = new int[n_lits];
		for(int i = 0; i < n_lits; i++) {
			vars [i] = jbdd.createBDD();
			not_vars[i] = jbdd.not( vars[i] );
		}

		// build the formula line by line:
		bdd_all = jbdd.ref( jbdd.getOne() );
		while( (tmp = getLine(br)) != null) {
			int lit, bdd = jbdd.ref( jbdd.getZero() );
			while((lit = getInt(tmp)) != 0 ) {
				bdd = jbdd.orTo(bdd,lit > 0 ? vars[lit-1] : not_vars[-lit-1]);
			}
			bdd_all = jbdd.andTo(bdd_all, bdd);
			jbdd.deref(bdd);
			if(bdd_all == jbdd.getZero()) break;; // no idea to continue
		}


		// ------------------- show the result:
		if(bdd_all == jbdd.getZero()) System.out.println("UNSAT");
		else {
			int sat1 = jbdd.satOne( bdd_all);
			System.out.print("SAT: ");
			jbdd.printSet(sat1);
			jbdd.deref(sat1);
		}
		jbdd.deref(bdd_all);
	}


	/** cleanup after the BDD package, _important_ ! */
	public void cleanup() { if(jbdd != null) jbdd.kill(); }

	/** assert next word in the line is the one we want */
	private void assure(StringTokenizer st, String str) throws IOException {
		if(!st.hasMoreTokens()) throw new IOException("pre-mature end of line when waiting for " + str);
		String got = st.nextToken();
		if(! got.equalsIgnoreCase( str) ) throw new IOException("Expected " + str + ", got " + got);
	}

	/** get the next integer in the line */
	private int getInt(StringTokenizer st) throws IOException {
		if(!st.hasMoreTokens()) throw new IOException("pre-mature end of line when waiting for a number ");
		return Integer.parseInt( st.nextToken() );
	}

	/** get next line, ignore commens (comments start with 'c', doesn't this remainds you of something?) */
	private StringTokenizer getLine(BufferedReader br) throws IOException	{
		String ret;
		for(;;) {
			ret = br.readLine();
			if(ret == null) return null;
			if(ret.charAt(0) != 'c') return new StringTokenizer(ret);
		}
	}


	// ----------------------------------------------------
	/** I have no idea what this function does ;) */
	public static void main(String args[]) {
		if(args.length == 0) System.err.println("Need DIMACS file as argument");
		else for(int i = 0; i < args.length;i++) {
			try {
				DimacsSolver ds = new DimacsSolver(args[i]);
				ds.cleanup();
			} catch(IOException exx) { exx.printStackTrace(); }
		}
	}
}

/** The author would like to thank Coldplay for moral support during this little project:) */
