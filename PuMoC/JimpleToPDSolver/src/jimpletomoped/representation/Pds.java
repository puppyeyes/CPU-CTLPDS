
package jimpletomoped.representation;

import java.util.Collection;
import java.util.HashSet;
import java.util.Set;
import java.util.HashMap;
import java.util.Map;

import jimpletomoped.remoplatopds.Namer;

import java.io.PrintStream;

import org.apache.log4j.Logger;


public class Pds {

    static Logger logger = Logger.getLogger(Pds.class);

    private Collection<Rule> rules = new HashSet<Rule>();

    private class Head {
        public String q;
        public String a;

        public Head(String q, String a) {
            this.q = q;
            this.a = a;
        }

        public boolean equals(Object h) {
            if (h instanceof Head) {
                Head hh = (Head)h;
                return q.equals(hh.q) && a.equals(hh.a);
            }
            return false;
        }

        public int hashCode() {
            return 2*q.hashCode() + 3*a.hashCode();
        }
    }

    private Map<Head, Collection<String>> props = new HashMap<Head, Collection<String>>();

    public Pds() {
        // do nothing.
    }

    public void addRule(Rule r) {
        assert r != null;
        rules.add(r);
    }

    public Collection<Rule> getRules() {
        return rules;
    }


    public void addProp(String q, String a, String prop) {
        Head h = new Head(q, a);
        if (props.containsKey(h)) {
            Collection<String> hprops = props.get(h);
            hprops.add(prop);
            props.put(h, hprops);
        } else {
            Collection<String> hprops = new HashSet<String>();
            hprops.add(prop);
            props.put(h, hprops);
        }
    }


    // Outputs Pds in pdsolver format, just the control flow
    public String toString() {
        StringBuilder out = new StringBuilder();
        doRules(out);
        doProps(out);
        return out.toString();
    }


    private void doRules(StringBuilder out) {
        out.append("\nRules:\n\n");
        for(Rule rule : rules) {
            rule.toString(out);
            out.append(";\n");
        }
    }

    private void doProps(StringBuilder out) {
        Set<String> controls = getControls();
        Set<String> chars = getChars();

        out.append("\nPropositions:\n\n");
        for (String p : controls) {
            String np = normState(p);
            for (String a : chars) {
                String na = normLabel(a);
                out.append(np + " " + na + " " + np + " " + na);

                Head h = new Head(p, a);
                if (props.containsKey(h)) {
                    for (String prop : props.get(h)) {
                        out.append(" " + prop);
                    }
                }
                out.append(";\n");
            }
        }
        out.append("\n");
    }

    // Outputs Pds in pdsolver format, just the control flow
    public void toString(PrintStream out) {
        doRules(out);
        doProps(out);
    }


    private void doRules(PrintStream out) {
        out.print("\nRules:\n\n");
        for(Rule rule : rules) {
            rule.toString(out);
            out.print(";\n");
        }
    }

    private void doProps(PrintStream out) {
        Set<String> controls = getControls();
        Set<String> chars = getChars();

        out.print("\nPropositions:\n\n");
        for (String p : controls) {
            String np = normState(p);
            for (String a : chars) {
                String na = normLabel(a);
                out.print(np + " " + na + " " + np + " " + na);

                Head h = new Head(p, a);
                if (props.containsKey(h)) {
                    for (String prop : props.get(h)) {
                        out.print(" " + prop);
                    }
                }
                out.print(";\n");
            }
        }
        out.print("\n");
    }


    private Set<String> getControls() {
        HashSet<String> controls = new HashSet();
        for (Rule rule : rules) {
            controls.add(rule.getq());
            controls.add(rule.getq_());
        }
        return controls;
    }

    private Set<String> getChars() {
        HashSet<String> chars = new HashSet();
        for (Rule rule : rules) {
            chars.add(rule.geta());
            for (String a : rule.getw()) {
                chars.add(a);
            }
        }
        return chars;
    }

    private String normLabel(String label) {
        if (label.equals(Namer.getInitStack())) 
            return label;
        else
            return "cp" + label;
    }

    private String normState(String state) {
        return "cs" + state;
    }
}
