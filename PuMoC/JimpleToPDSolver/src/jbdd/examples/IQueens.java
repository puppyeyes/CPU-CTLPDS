// JBDD example code
package jbdd.examples;
import jbdd.*;



import java.awt.*;
import java.awt.event.*;

/**
 * 8-Queens, interactive
 * influenced by the work of H.R. Andersen and J. Lind-Nielsen in BuDDy
 * coded by Arash Vahidi (vahidi (a) s2.chalmers.se)
 */


public class IQueens extends Queens2 {


	private ChessBoard chessboard;

	public IQueens() {
		super(8); // --> Queens2(8);
		chessboard= new ChessBoard(this);
	}
	public void update(int [] positions, boolean [] sat) {
		int method = chessboard.getMethod();

		chessboard.setMessage("");
		if(method == 0)  { /* NONE */
			for(int i = 0; i < 64; i++) sat[i] = true;
			// Give some stats:
			int res = c; ref(c);
			for(int x = 0; x < 8; x++)
				if(positions[x] != -1) {
					int px = position(x, positions[x]);
					res = andTo(res,px);
				}
			int s = (int)satCount(res);
			deref(res);
			if(s == 0) chessboard.setMessage("DEAD END!!");
			else chessboard.setMessage("Active solutions: "  + s);

		} else if(method == 1) { /* SIMPLE */
			for(int i = 0; i < 64; i++) sat[i] = true;
			for(int x = 0; x < 8; x++)
				if(positions[x] != -1)
					reomvePositions(sat, x, positions[x]);

		} else if(method == 2) { /* ONE */
			for(int i = 0; i < 64; i++) sat[i] = true;
			for(int x = 0; x < 8; x++)
				if(positions[x] != -1) {
					int px = position(x, positions[x]);
					int res = and(px, c);
					filFalse(res, sat);
					deref(res);
				}
		} else if(method == 3) { /* FULL */
			for(int i = 0; i < 64; i++) sat[i] = true;


			int res = c; ref(c);
			for(int x = 0; x < 8; x++)
				if(positions[x] != -1) {
					int px = position(x, positions[x]);
					res = andTo(res,px);
				}
			filFalse(res, sat);
			deref(res);

		}

	}
	// ----------------------------------------------------------

	private void reomvePositions(boolean [] sat, int x, int y) {
		for(int i = 0; i < 8; i++) {
			if(x != i) sat[i + y * 8] = false; // SAME ROW
			if(y != i) sat[x + i * 8] = false; // SAME COL
		}

		// AND THE DIAGONALS
		for(int i = 0; i < 8; i++)
			for(int j = 0; j < 8; j++)
				if(i != x && j != y)
					if((i+y == j+x)  || (i+j==y+x) )
						sat[i + j * N] = false;

	}
	// ----------------------------------------------------------
	public void onQuit() {
		cleanup();
		System.exit(0);
	}

	// ----------------------------------------------------------
	public static void main(String [] args) {
		new IQueens();
	}
}


class ChessBoard extends Frame implements ActionListener, ItemListener {
	private IQueens owner;
	private ChessCanvas canvas;
	private Button bQuit;
	private Choice method;
	private Label msg;

	public ChessBoard(IQueens owner_) {
		super("[Interactive Queens] by Arash Vahidi (vahidi (a) s2.chalmers.se)");
		setBackground(new Color(0xFFFFB0));

		owner = owner_;

		add(canvas = new ChessCanvas(owner), BorderLayout.CENTER);

		Panel pNorth = new Panel(new FlowLayout(FlowLayout.LEFT));
		pNorth.add( bQuit = new Button("Quit"));
		pNorth.add( new Label("   assist method:"));
		pNorth.add( method = new Choice());

		bQuit.addActionListener(this);
		method.addItemListener(this);
		method.add("none");
		method.add("simple");
		method.add("one");
		method.add("full");
		method.select(1); // ONE

		add(pNorth, BorderLayout.NORTH);
		add(msg= new Label(""), BorderLayout.SOUTH);
		pack();
		show();
	}
	// -------------------------------------------------
	public int getMethod() {
		return method.getSelectedIndex();
	}
	public void setMessage(String str) {
		msg.setText(str);
	}
	// -------------------------------------------------
	public void actionPerformed(ActionEvent e){
		if(e.getSource() == bQuit) onQuit();
	}
	public void itemStateChanged(ItemEvent e) {
		if(e.getSource() == method) canvas.updateModel();
	}
 	// ---------------------------------------------
	private void onQuit() {
		dispose();
		owner.onQuit();
	}

}

class ChessCanvas extends Canvas implements MouseListener {
	private static final int size = 60;
	private Dimension dims;
	private int [] positions;
	private boolean [] sat;
	private IQueens owner;

	public ChessCanvas (IQueens owner_) {
		owner = owner_;
		dims = new Dimension(size * 8, size * 8);
		positions = new int[8];
		sat = new boolean[ 8 * 8];

		for(int i = 0; i < 8; i++) positions[i] = -1;
		for(int i = 0; i < 64; i++) sat[i] = true;

		addMouseListener(this);
	}

	// Access methods:
	public int [] getPositions() { return positions; }
	public boolean [] getSat() { return sat; }

	public Dimension getPreferredSize()  { return dims; }

	public void paint(Graphics g) {
		// Draw the boatd
		g.setColor( Color.black);
		for(int x = 0; x < 8; x++)
			for(int y = 0; y < 8; y++)
				if(((x + y) % 2) == 0)
					g.fillRect(x * size, y * size,size, size);



		// Draw the queens
		g.setColor(Color.orange);
		for(int x = 0; x < 8; x++)
			if(positions[x] != -1)
				g.fillOval(x * size+size/8, positions[x] * size+size/8,size-size/4, size-size/4);

		// Draw places we can set a queen:
		g.setColor(Color.red);
		for(int x = 0; x < 8; x++)
			for(int y = 0; y < 8; y++) if(!sat[x + y * 8]) disable(x,y,g);
	}
	private void disable(int x, int y, Graphics g) {
		int x0 = x * size + size/4;
		int x1 = (x+1) * size - size/4;
		int y0 = y * size +size/4;
		int y1 = (y+1) * size -size/4;

		g.drawLine(x0,y0,x1,y1);
		g.drawLine(x1,y0,x0,y1);
	}

	public void updateModel() {
		owner.update(positions, sat);
		repaint();
	}
	private void click(int x, int y) {
		if(!sat[x + y * 8] && positions[x] != y) return ; // CANT DO THAT MOVE
		if(positions[x] == y) positions[x] = -1;
		else positions[x] = y;
		updateModel();

	}
	// ------------------------------------ MouseListener interface
	public void mouseClicked(MouseEvent e)  { }
	public void mouseExited(MouseEvent e)  { }
	public void mouseEntered(MouseEvent e)  { }
	public void mouseReleased(MouseEvent e)  { }
	public void mousePressed(MouseEvent e)  { click(e.getX() / size, e.getY() / size); }

}
