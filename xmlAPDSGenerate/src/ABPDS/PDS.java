package ABPDS;

import java.util.List;

public class PDS {
	List<Rule> rulelist;
	List<String> finalState;
	public List<Rule> getRulelist() {
		return rulelist;
	}
	public void setRulelist(List<Rule> rulelist) {
		this.rulelist = rulelist;
	}
	public List<String> getFinalState() {
		return finalState;
	}
	public void setFinalState(List<String> finalState) {
		this.finalState = finalState;
	}
	public PDS(List<Rule> rulelist, List<String> finalState) {
		super();
		this.rulelist = rulelist;
		this.finalState = finalState;
	}
	public PDS() {
		super();
	}
	
}
