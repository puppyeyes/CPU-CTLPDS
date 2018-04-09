// JBDD example code
package jbdd.examples;
import jbdd.*;


/**
 * This is another example of solving the N-Queens problem with BDDs.
 *
 * In this example each row in the board is coded as a number, saving
 * a lot of BDD variables
 *
 * influenced by the work of H.R. Andersen and J. Lind-Nielsen in BuDDy
 * coded by Arash Vahidi (vahidi (a) s2.chalmers.se)
 */

public class Queens2
	extends JBDD {

	protected int N; // number
	protected int c; // BDD tree
	protected int vars, vars_column; // number of variables [per column]
	protected int [] variables, not_variables; // BDD variables and their inverse
	protected int [] board, not_board; // the boardand the inverted board [positions]
	protected int sp2s;


	protected int position(int x, int y) {
		return board[x + y * N];
	}

	protected int position_not(int x, int y) {
		return not_board[x + y * N];
	}

	private int build(int x, int y) {
		// not that same_column is removed (its already in the model with numbers per column)

		int same_row, same_diag1, same_diag2;

		same_row = getOne(); ref(same_row);
		same_diag1 = getOne(); ref(same_diag1);
		same_diag2 = getOne(); ref(same_diag2);



		for(int i = 0; i < N; i++) {

			if(i != y) {
				int y_and_not_i = imp(position(x,y), position_not(x,i));
				same_row = andTo(same_row, y_and_not_i);
				deref(y_and_not_i);
			}


			int l1 = i - x + y;
			if(l1 >= 0 && l1 < N && i != x) {
				int diag_up = imp(position(x,y), position_not(i,l1));
				same_diag1 = andTo(same_diag1, diag_up);
				deref(diag_up);
			}


			int l2 = x+y-i;
			if(l2 >= 0 && l2 <N && i != x) {
				int diag_down = imp(position(x,y), position_not(i,l2));
				same_diag2 = andTo(same_diag2, diag_down);
				deref(diag_down);
			}
		}

		same_row = andTo(same_row, same_diag1);
		same_row = andTo(same_row, same_diag2);

		deref(same_diag1);
		deref(same_diag2);
		return same_row;
	}

	private int getNumber(int row, int number) {
		int offset = row * vars_column;

		int ret = getOne(); ref(ret);

		for(int i = 0; i < vars_column; i++) {
			if( (number & (1L << i)) == 0)  ret = andTo(ret, not_variables[offset]);
			else ret = andTo(ret, variables[offset]);
			offset++;
		}
		return ret;
	}

	private static int getWidth(int N) {
		return (int)Math.ceil( Math.log(N) /  Math.log(2));
	}
	// ---------------------------------------------------------------------------
	public Queens2(int N) {
		super(N*getWidth(N), N*getWidth(N)*400);
		this.N = N;


		// number of variables need per comlumn:
		vars_column = getWidth(N);
		vars = vars_column * N;

		if(1L << vars_column != N) {
			System.out.println("N must be a power of 2");
			// System.exit(0);
		}


		variables = new int[vars];
		not_variables = new int[vars];

		// allocate BDD vars
		for(int i = 0; i < vars; i++) {
			variables[i] = createBDD();
			not_variables[i] = not(variables[i]);
		}



		// build the board
		board = new int[N*N];
		not_board = new int[N*N];


		// create the compressed board
		for(int row = 0; row < N; row++)
			for(int column = 0; column < N; column ++)
				board[ column + row * N] = getNumber(row, column);


		// create the board negation
		for(int i = 0; i < N *N; i++)
			not_board[i] = not(board[i]);


		// the initial constraint:
		c = getOne(); ref(c);



		// there MUST be one in each row (and not in the DONT-CARE area)
		for(int i = 0; i < N; i ++) {
			int cr = getZero(); ref(cr);
			for(int j = 0; j < N; j++)
				cr = orTo(cr, position(i,j));
			c = andTo(c, cr);
			deref(cr);
		}

		// the rest (the constrints)
		for(int i = 0; i < N; i ++)
			for(int j = 0; j < N; j++) {
				int c2 = build(i,j);
				c = andTo(c,c2);
				deref(c2);
			}
	}


	protected void filTrue(int bdd, boolean [] out) {
		for(int y = 0; y < N; y++) {
			for(int x = 0; x < N; x++) {
				int check = and(bdd, position(x,y));
				if(check != getZero()) out[x + y * N] = true;
				deref(check);
			}
		}
	}
	protected void filFalse(int bdd, boolean [] out) {
		for(int y = 0; y < N; y++) {
			for(int x = 0; x < N; x++) {
				int check = and(bdd, position(x,y));
				if(check == getZero()) out[x + y * N] = false;
				deref(check);
			}
		}
	}

	public void show() {
		System.out.println("There are " + (int) satCount(c)  + " solutions.");
		int sat = satOne(c);
		if(sat != getZero()) { // THIS IS PARTIALLY DUE TO INCOMPLETE CUDD INTERFACE

			boolean [] ret = new boolean[N * N];
			for(int i = 0; i < N * N; i++) ret[i] = false;
			filTrue(sat, ret);

			for(int y = 0; y < N; y++) {
				for(int x = 0; x < N; x++)
					System.out.print(ret[x + N * y] ? "*" : ".");
				System.out.println();
			}
			/*
			for(int y = 0; y < N; y++) {
				for(int x = 0; x < N; x++) {
					int check = and(sat, position(x,y));
					System.out.print(check == getZero() ? "." : "*");
				}
				System.out.println();
			}
			*/
			deref(sat);
		}
	}
	public void cleanup() {
		deref(c);

		for(int i = 0; i < N*N; i++) {
			deref(board[i]);
			deref(not_board[i]);
		}

		for(int i = 0; i < vars; i++)
			deref(not_variables[i]);

		kill();
	}

	public static void main(String [] args) {

		int N = args.length != 1 ? 4 : Integer.parseInt(args[0]);

		if(N < 4) N = 4;
		Queens2 q = new Queens2(N);
		q.show();
		q.cleanup();

	}
}
