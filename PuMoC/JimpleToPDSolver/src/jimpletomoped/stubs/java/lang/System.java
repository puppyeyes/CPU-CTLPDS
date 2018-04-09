package jimpletomoped.stubs.java.lang;

import jimpletomoped.stubs.java.io.PrintStream;

public class System {

	public static final PrintStream err = new PrintStream();
	public static final PrintStream out = err;
	
	public static void arraycopy(Object src, int srcPos, 
			Object dest, int destPos, int length) {
		
		Object[] a = (Object[]) src;
		Object[] b = (Object[]) dest;
		
		for (int i = 0; i < length; i++)
			b[destPos + i] = a[srcPos + i];
	}
}
