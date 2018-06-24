import java.io.InputStreamReader;
import java.awt.event.KeyEvent;
import java.io.BufferedReader;
import java.awt.AWTException;
import java.util.LinkedList;
import java.io.IOException;
import java.awt.Robot;

public class App {

    private static App app;
    private static boolean running = true;
    private Robot robot;
    private LinkedList<Integer> pressedKeys;

    public static void main(String[] args) {
        System.out.println("##############################");
        System.out.println("#       VirtualControl       #");
        System.out.println("##############################");

        app = new App();

        Runtime.getRuntime().addShutdownHook(new Thread() {
            public void run() {
                System.out.println("Shutdown hook started...");

                running = false;

                if (app != null) {
                    app.releasePressedKeys();
                }

                System.out.println("Shutdown hook completed...");
            }
        });

        app.run();
    }

    public void press(int keyCode) {
        robot.keyPress(keyCode);
        pressedKeys.add(keyCode);
    }

    public void release(int keyCode) {
        robot.keyRelease(keyCode);
        pressedKeys.removeFirstOccurrence(keyCode);
    }

    public void pressRelease(int keyCode) {
        press(keyCode);
        release(keyCode);
    }

    public void releasePressedKeys() {
        if (pressedKeys == null) {
            return;
        }

        java.util.ListIterator<Integer> itr = pressedKeys.listIterator();
        while (itr.hasNext()) {
            release(itr.next());
        }
    }

    private void run() {
        System.out.println("Virtual control is running!");


        try {
            robot = new Robot();
            pressedKeys =  new LinkedList<Integer>();

            BufferedReader stdin = new BufferedReader(new InputStreamReader(System.in));
            while (running) {
                String str = stdin.readLine();
                switch (str) {
                    case "up": pressRelease(KeyEvent.VK_UP); break;
                    case "down": pressRelease(KeyEvent.VK_DOWN); break;
                    case "left": pressRelease(KeyEvent.VK_LEFT); break;
                    case "right": pressRelease(KeyEvent.VK_RIGHT); break;
                    case "p_up": press(KeyEvent.VK_UP); break;
                    case "p_down": press(KeyEvent.VK_DOWN); break;
                    case "p_left": press(KeyEvent.VK_LEFT); break;
                    case "p_right": press(KeyEvent.VK_RIGHT); break;
                    case "r_up": release(KeyEvent.VK_UP); break;
                    case "r_down": release(KeyEvent.VK_DOWN); break;
                    case "r_left": release(KeyEvent.VK_LEFT); break;
                    case "r_right": release(KeyEvent.VK_RIGHT); break;
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        System.out.println("Virtual control is stopped!");
    }
}