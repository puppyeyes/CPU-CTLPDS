package ABPDS;

public class Configuration {
	public String controlLocation;
	public String stack1;
	public String stack2;
	public String getControlLocation() {
		return controlLocation;
	}
	public void setControlLocation(String controlLocation) {
		this.controlLocation = controlLocation;
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
	public Configuration(String controlLocation, String stack1, String stack2) {
		super();
		this.controlLocation = controlLocation;
		this.stack1 = stack1;
		this.stack2 = stack2;
	}
	public Configuration(String controlLocation, String stack1) {
		super();
		this.controlLocation = controlLocation;
		this.stack1 = stack1;
	}
}
