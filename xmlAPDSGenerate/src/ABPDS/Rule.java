package ABPDS;

import java.util.List;

public class Rule {
	Configuration from;
	List<Configuration> to;
	public Configuration getFrom() {
		return from;
	}
	public void setFrom(Configuration from) {
		this.from = from;
	}
	public List<Configuration> getTo() {
		return to;
	}
	public void setTo(List<Configuration> to) {
		this.to = to;
	}
	public Rule(Configuration from, List<Configuration> to) {
		super();
		this.from = from;
		this.to = to;
	}
	
}
