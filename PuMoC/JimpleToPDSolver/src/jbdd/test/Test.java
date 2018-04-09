package jbdd.test;
import jbdd.*;
import jbdd.examples.*;

/**
 * This file will - when we have more time :( - conatin a testbed for the interface.
 *
 */

public class Test{
	// TEST BED
	public static void main( String [] args) {

		try {
			// load the specified library
			if(args.length != 1) {
				System.err.println("Usage: java Test <JBDD native DLL>");
				System.exit(20);
			}

			// show some debug messages
			System.out.println();
			System.out.println("Attempting to load " + args[0]);
			System.load(args[0]);

			// show some more debug messages
			System.out.println("JBDD v" + JBDD.getJBDDVersion() + ", package " + JBDD.getPackageName() + " v" + JBDD.getPackageVersion() + ", capabilities = " + JBDD.getPackageCapabilities()  );
			System.out.println();


			JBDD jbdd = new JBDD(10,1);
			int v1 = jbdd.createBDD();
			int v2 = jbdd.createBDD();
			int v3 = jbdd.createBDD();
			int v4 = jbdd.createBDD();

			jbdd.gc();
			jbdd.checkPackage();

			int v1andv2 = jbdd.and(v1,v2);
			int v1orv2 = jbdd.or(v1,v2);

			jbdd.printSet(v1andv2);


			jbdd.gc();
			jbdd.checkPackage();


			int [] set = new int[2];
			set[0] = v1;
			set[1] = jbdd.getOne() ;
			int x = jbdd.makeSet(set,2);
			jbdd.debugBDD(x);
			jbdd.debugBDD( jbdd.getZero() );
			jbdd.debugBDD( jbdd.getOne() );

			jbdd.deref(x);

			jbdd.gc();
			jbdd.checkPackage();
			jbdd.kill();


			// external examples
			Queens.main( new String[] { "8" });
			Equivalence.main(null);

		} catch(Exception exx) {
			System.err.println("ERROR: " + exx);
			exx.printStackTrace();
			System.exit(20);
		}

	}
}

