
public class ToConfig {
	String state;
	String stack1;
	String stack2;
	ToConfig next;
	public String getState() {
		return state;
	}
	public void setState(String state) {
		this.state = state;
	}
	public String getStack1() {
		return stack1;
	}
	public void setStack1(String stack1) {
		this.stack1 = stack1;
	}
	public String getStack2() {
		return stack2;
	}
	public void setStack2(String stack2) {
		this.stack2 = stack2;
	}
	public ToConfig getNext() {
		return next;
	}
	public void setNext(ToConfig next) {
		this.next = next;
	}
	public ToConfig(String state, String stack1, String stack2, ToConfig next) {
		super();
		this.state = state;
		this.stack1 = stack1;
		this.stack2 = stack2;
		this.next = next;
	}
	public ToConfig(String state, String stack1) {
		super();
		this.state = state;
		this.stack1 = stack1;
	}
	public ToConfig(String state, String stack1, ToConfig next) {
		super();
		this.state = state;
		this.stack1 = stack1;
		this.next = next;
	}
	public ToConfig(String state, String stack1, String stack2) {
		super();
		this.state = state;
		this.stack1 = stack1;
		this.stack2 = stack2;
	}
	public String toString() {
		String s;
		if(this.stack2==null)
		{
			s=this.state+" "+"<"+this.stack1+">";
		}else {
			s=this.state+" "+"<"+this.stack1+" "+this.stack2+">";
		}		
		return s;
		
	}
}
