
package examples;


public class Dataflow {

    public static int i;


    public static void main(String[] args) {
        i = 5 / 7;
        a();
        b();
    }


    public static void a() {
        // do nothing
    }


    public static void b() {
        i = i * i;
    }

}
