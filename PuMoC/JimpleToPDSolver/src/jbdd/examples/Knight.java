// JBDD example code
package jbdd.examples;
import jbdd.*;



/**
 * The Knights tour problem with BDDs.
 *
 * Having a chessboard, we would like to know if a knight at position p1
 * can reach a square at position p2, and in that case, what is the minimum
 * number of steps.
 *
 * To do this we use a BDD for the relation  (x,y) T (x',y') [see variable t]
 * We use the ?? variable ordering in this example
 *
 * Note: the size of the board is fixed to 8*8
 *
 * influenced by the lecture notes of H.R. Andersen at IT-C, Denmark
 * coded by Arash Vahidi (vahidi (a) s2.chalmers.se)
 */

public class Knight
	extends JBDD {

	private int vars;
	private int t; // Transition relation
	private int cube; // cube of S
	private int cubep; // cube of S'
	private int sp2s; // permutation for S' -> S
	private int [] variables, not_variables; // BDD vars
	private int [] S, Sp; // S and S' variables (current/next state)
	private int [] board, not_board; // board and its complement
	private int [] next_board, next_not_board; // next board and its complement
	private int [] keep_row; // row x does not move

	// valid knight moves
	private int positions[][] = {{1,2}, {-1,2}, {1,-2},{-1,-2},{2,1}, {-2,1}, {2,-1},{-2,-1}};
	// --------------------------------------------------------------------------

	public Knight() {
		// 8 rows, 8 columns, times 2 for current and next state
		super(2 * 8 * 8, 2 * 8 * 8 *800);

		int vars_column = 8; 		// number of variables needed per column:
		vars = vars_column * 8 * 2;

		// allocate BDD vars
		variables = new int[vars];
		not_variables = new int[vars];
		for(int i = 0; i < vars; i++) {
			int v = createBDD();
			variables[i] =
			not_variables[i] = not(v);

		}


		// get current/next state variables
		S = new int[vars/2];
		Sp = new int[vars/2];
		for(int i = 0; i < vars/2; i++) {
			S[i] = variables[i*2];
			Sp[i] = variables[i*2+1];
		}



		// S and S' cube:
		cube = makeSet(S, S.length);
		cubep = makeSet(Sp, Sp.length);


		// S' -> S permutation
		sp2s = createPair(Sp,S);


		// create the compressed board
		board = new int[8*8];
		next_board = new int[8*8];
		for(int row = 0; row < 8; row++)
			for(int column = 0; column < 8; column ++) {
				board[ column + row * 8] = getNumber(row, column);
				next_board[ column + row * 8] = getNumberNext(row, column);
				// System.out.println("-->" + row + "," + column);
				// printSet(board[ column + row * 8] );
				// System.exit(0);
			}


		// create the board negation
		not_board = new int[8*8];
		next_not_board = new int[8*8];
		for(int i = 0; i < 8 * 8; i++) {
			not_board[i] = not(board[i]);
			next_not_board[i] = not(next_board[i]);
		}



		// keep row
		keep_row = new int[8];
		for(int i = 0; i < 8; i++) {
			keep_row[i] = getOne(); ref(keep_row[i]);
			for(int j = 0; j < 8; j++) {
				int offset = i * 16;
				int tmp = biimp( variables[ offset + j * 2], variables[ offset + j * 2+ 1]);
				keep_row[i] = andTo(keep_row[i], tmp);
				deref(tmp);
			}
		}



		// build the transition relation
		t = getZero(); ref(t);
		for(int i = 0; i < 8; i ++) {
			for(int j = 0; j < 8; j++) {
				t = M(t,i,j);
			}
		}


		/*
		// Reorder T first:
		System.out.println("\nBefore reordering, |T| = " + nodeCount(t)+"\n(reordering...)");
		reorder_setMethod(REORDER_SIFT);
		reorder_now();
		*/

		System.out.println("\nInitialization done, |T| = " + nodeCount(t) + " |SAT(T)| = " + satCount(t));
		printSet(t);
	}

	public void check(int x0,int y0, int x1, int y1) {
		int s0 = current_position(x0,y0);
		int s1 = current_position(x1,y1);
		int steps = forward_with_constraint(s0,s1);

		if(steps == -1)
			System.out.println("("+x1+","+y1+") is not reachable from position ("+x0+","+y0+").");
		else
			System.out.println("A knight at ("+x1+","+y1+") reaches ("+x0+","+y0+") in " + steps + " steps.");



	}

	// -----------------------------------------------------------------------

/*
	// return the states reachble from the initial state in "start"
	private int forward(int start) {
		int rp, r = start, front = start;
		ref(front); // for frontier set
		ref(front); // for R0 (front = r here)
		do {
			rp = r;
			int tmp = relProd( t, r, cube);
			int tmp2 = replace(tmp, sp2s);
			deref(tmp);
			r = orTo(r, tmp2);
			deref(front);
			front = tmp2;
		} while(rp != r);

		deref(front);
		return r;
	}
*/

	// return number of states before reachables from "start" satisfy "constraint"
	// return -1 if none
	private int forward_with_constraint(int start,int constraint) {
		int rp, r = start, front = start;
		int steps = 0;
		ref(front); // for frontier set
		ref(front); // for R0 (front = r here)

		do {
			rp = r;

			int intersection = and( front, constraint);
			if(intersection != getZero()) {
				deref(front);
				deref(r);
				deref(intersection);
				return steps;
			} else {
				deref(intersection);
				steps++;
			}

			int tmp = relProd( t, r, cube);
			int tmp2 = replace(tmp, sp2s);
			deref(tmp);

			r = orTo(r, tmp2);
			deref(front);
			front = tmp2;
		} while(rp != r); // no more new states

		deref(front);
		deref(r);
		return -1;
	}



	// -----------------------------------------------------------------------
	public void cleanup() {

		deletePair(sp2s);
		deref(cube);
		deref(t);

		for(int i = 0; i < 8; i++)
			deref(keep_row[i]);

		for(int i = 0; i < 8*8; i++) {
			deref(board[i]);
			deref(not_board[i]);
			deref(next_board[i]);
			deref(next_not_board[i]);
		}

		for(int i = 0; i < vars; i++)
			deref(not_variables[i]);

		kill();
	}
	// -----------------------------------------------------------------------

	// returns true if the point is on the board
	private boolean ok(int p) {		return p >= 0 && p < 8;	}

	private int current_position(int x, int y) {	return board[x + y * 8];	}
	private int current_position_not(int x, int y) {	return not_board[x + y * 8];	}
	private int next_position(int x, int y) {	return next_board[x + y * 8];	}
	private int next_position_not(int x, int y) {	return next_not_board[x + y * 8];	}



	// -----------------------------------------------------------------------
	private int M(int t, int x, int y) {
		for(int i = 0; i < 8; i++) {
			int xp = x + positions[i][0];
			int yp = y + positions[i][1];
			if(ok(xp) && ok(yp)) t = move(t,x,y,xp,yp);
		}
		return t;
	}

	// -----------------------------------------------------------------------
	private int move(int t, int x, int y, int xp, int yp) {
		// move from (x,y) ---> (xp,yp)  and add it to t
		System.out.print("building ("+x+","+y+")-->("+xp+","+yp+")    \r");
		int all = getOne();
		for(int i = 0; i < 8; i++) if(i != y && i != yp) all = andTo(all, keep_row[i]);

		all = andTo(all, current_position(x,y));
		all = andTo(all, next_position(xp,yp));

		t = orTo(t,all);
		deref(all);

		return t;
	}


	// -----------------------------------------------------------------------
	private int getNumber(int row, int number) {
		int ret = getOne(); ref(ret);

		for(int x = 0; x < 8; x++) {
			for(int y = 0; y < 8; y++) {
				int offset = (x + y * 8) * 2;
				if(row != y || number != x) {
					ret = andTo(ret, not_variables[offset]);
				} else {
					ret = andTo(ret, variables[offset]);
				}
			}
		}

		return ret;
	}

	private int getNumberNext(int row, int number) {
		int ret = getOne(); ref(ret);

		for(int x = 0; x < 8; x++) {
			for(int y = 0; y < 8; y++) {
				int offset = (x + y * 8) * 2 + 1;
				if(row != y || number != x) {
					ret = andTo(ret, not_variables[offset]);
				} else {
					ret = andTo(ret, variables[offset]);
				}
			}
		}

		return ret;
		}
	// -----------------------------------------------------------------------
	public static void main(String [] args) {

		Knight k = new Knight();
		k.check(0,0,2,1);
		k.check(0,0,7,7);
		k.check(7,0,0,7);
		k.cleanup();

	}
}
