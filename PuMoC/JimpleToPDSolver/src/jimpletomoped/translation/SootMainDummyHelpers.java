// SootMainDummyHelpers.java
// Author: Matthew Hague



package jimpletomoped.translation;

import java.awt.List;

import java.util.ArrayList;


public class SootMainDummyHelpers {

    public static final String RAND_ARRAY = "randArray";
    public static final String RAND_INT = "randInt";

    private static ArrayList<int[]> arrays = new ArrayList<int[]>();

    static public int randInt() {
        return (int)(Math.random() * Integer.MAX_VALUE);
    }


    static public int[] randArray() {
        int[] arr = null;
        double choice = Math.random();
        if (choice > .66) {
            arr = new int[randInt()];
            for (int i = 0; i < arr.length; i++) {
                arr[i] = randInt();
            }
            arrays.add(arr);
        } else if (choice > .33 && !arrays.isEmpty()) {
            arr = arrays.get(randInt()%arrays.size());
        } 
        return arr;
    }
}
