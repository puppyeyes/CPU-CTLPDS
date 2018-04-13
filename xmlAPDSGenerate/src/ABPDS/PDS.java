package ABPDS;

import java.util.List;

public class PDS {
	List<Rule> rulelist;
	List<String> finalState;
	int stateSize;
	int stackSize;
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
	
	public int getStateSize() {
		return stateSize;
	}
	public void setStateSize(int stateSize) {
		this.stateSize = stateSize;
	}
	public int getStackSize() {
		return stackSize;
	}
	public void setStackSize(int stackSize) {
		this.stackSize = stackSize;
	}
	public PDS() {
		super();
	}
	
}
