package day2;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class Day2 {
    public static void main(String[] args) throws FileNotFoundException {
        File f = new File("day2/input.txt");
        Scanner scanner = new Scanner(f);
        int x = 0;
        int y = 0;
        while (scanner.hasNextLine()){
            switch (scanner.next()){
                case "forward":
                    x += scanner.nextInt();
                break;
                case "down":
                    y += scanner.nextInt();
                break;
                case "up":
                    y -= scanner.nextInt();
                break;
            }

        }
        scanner.close();
        System.out.println(x * y);

    }
}
