// JBDD example code
package jbdd.examples;
import jbdd.*;


/**
 * This is an example of solving the N-Queens problem with BDDs.
 *
 * In this example each position in the N*N board is coded with its own variable
 *
 * influenced by the work of H.R. Andersen and J. Lind-Nielsen in BuDDy
 * coded by Arash Vahidi (vahidi (a) s2.chalmers.se)
 */

public class Queens
	extends JBDD {

	private int N; // number
	private int c; // BDD tree
	private int [] board; // BDD variables
	private int [] not_board; // inverted BDD variables


	// OK, here is something funky:
	// if you swap x and y, the performance changes DRASTICALLY.
	// reason? the first constraint (must be a queen in each row)
	// produces a huge BDD and then you are dead already :(
	//
	// now, this shows how important operator-scheduling is
	// (not to mention good variable ordering)
	private int position(int x, int y) {
		return board[y + x * N];
	}

	private int position_not(int x, int y) {
		return not_board[y + x * N];
	}

	private int build(int x, int y) {
		int same_column, same_row, same_diag1, same_diag2;

		same_column = getOne(); ref(same_column);
		same_row = getOne(); ref(same_row);
		same_diag1 = getOne(); ref(same_diag1);
		same_diag2 = getOne(); ref(same_diag2);



		for(int i = 0; i < N; i++) {
			if(i != x) {
				int x_and_not_i = imp(position(x,y), position_not(i,y));
				same_column = andTo(same_column, x_and_not_i);
				deref(x_and_not_i);
			}

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


		same_column = andTo(same_column, same_row);
		same_column = andTo(same_column, same_diag1);
		same_column = andTo(same_column, same_diag2);

		deref(same_row);
		deref(same_diag1);
		deref(same_diag2);
		return same_column;
	}

	public Queens(int N) {
		super(N*N, N*N*400); // <-- number of nodes is way to low, i know :)
		this.N = N;


		// build the board
		board = new int[N*N];
		not_board = new int[N*N];

		for(int i = 0; i < N * N; i++) {
			board[i] = createBDD();
			not_board[i] = not(board[i]);
		}


		// the initial constraint:
		c = getOne(); ref(c);



		// there MUST be one in each row
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

	public void show() {
		System.out.println("There are " + (int) satCount(c)  + " solutions.");

		int sat = satOne(c);
		if(sat != getZero()) { // THIS IS PARTIALLY DUE TO INCOMPLETE CUDD INTERFACE
			for(int y = 0; y < N; y++) {
				for(int x = 0; x < N; x++) {
					int check = and(sat, position(x,y));
					System.out.print(check == getZero() ? "." : "*");
				}
				System.out.println();
			}
			deref(sat);
		}
	}
	public void cleanup() {
		deref(c);
		for(int i = 0; i < N*N; i++) deref(not_board[i]);
		kill();
	}

	public static void main(String [] args) {

		int N = args.length != 1 ? 4 : Integer.parseInt(args[0]);

		if(N < 4) N = 4;
		Queens q = new Queens(N);
		q.show();
		q.cleanup();

	}
}
