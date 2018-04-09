
package jimpletomoped.representation;

import java.util.List;
import java.util.ArrayList;

import java.io.PrintStream;

import jimpletomoped.remoplatopds.Namer;

public class Rule {
    public String q;
    public String a;
    public String q_;
    public List<String> w;

    public Rule(String q, String a, String q_, List<String> w) {
        this.q = q;
        this.a = a;
        this.q_ = q_;
        this.w = w;
    }

    public Rule(String q, String a, String q_, String... w) {
        this.q = q;
        this.a = a;
        this.q_ = q_;
        this.w = new ArrayList<String>();
        for (int i = 0; i < w.length; i++) {
            this.w.add(w[i]);
        }
    }

    public String toString() {
        String s = (normState(q) + " " + 
                    normLabel(a) + " -> " + 
                    normState(q_));
        for (String b : w) {
            s += " " + normLabel(b);
        }
        return s;
    }

    public void toString(StringBuilder out) {
        out.append(normState(q));
        out.append(" ");
        out.append(normLabel(a));
        out.append(" -> ");
        out.append(normState(q_));
        out.append(" ");
        if (w.size() == 0) {
            out.append(" _");
        } else {
            for (String b : w) {
                out.append(" ");
                out.append(normLabel(b));
            }
        }
    }

    public void toString(PrintStream out) {
        out.print(normState(q));
        out.print(" ");
        out.print(normLabel(a));
        out.print(" -> ");
        out.print(normState(q_));
        out.print(" ");
        if (w.size() == 0) {
            out.print(" _");
        } else {
            for (String b : w) {
                out.print(" ");
                out.print(normLabel(b));
            }
        }
    }


    public String getq() {
        return q;
    }

    public String geta() {
        return a;
    }


    public String getq_() {
        return q_;
    }

    public List<String> getw() {
        return w;
    }

    private String normLabel(String label) {
        if (label.equals(Namer.getInitStack()))
            return label;
        else
            return "cp" + label;
    }


    private static String normState(String state) {
        return "cs" + state;
    }


}
