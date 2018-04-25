
public class Rule {
	FromConfig from;
	ToConfig to;
	public FromConfig getFrom() {
		return from;
	}
	public void setFrom(FromConfig from) {
		this.from = from;
	}
	public ToConfig getTo() {
		return to;
	}
	public void setTo(ToConfig to) {
		this.to = to;
	}
	public Rule(FromConfig from, ToConfig to) {
		super();
		this.from = from;
		this.to = to;
	}
	public Rule() {
		super();
	}
	public String toString() {
		String s;
		s=this.from.toString()+" --> "+this.to.toString();
		ToConfig tmp=this.to;
		while(tmp.next!=null) {
			s+=" & "+tmp.toString();
			tmp=tmp.next;
		}
		s+="\n";
		return s;
		
	}
}
