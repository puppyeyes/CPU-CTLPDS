
public class FromConfig {
	String state;
	String stack;

	public String getState() {
		return state;
	}

	public void setState(String state) {
		this.state = state;
	}

	public String getStack() {
		return stack;
	}

	public void setStack(String stack) {
		this.stack = stack;
	}

	public FromConfig(String state, String stack) {
		super();
		this.state = state;
		this.stack = stack;
	}

	public String toString() {
		String s;
		s = this.state + " <" + this.stack + ">";
		return s;
	}
}
