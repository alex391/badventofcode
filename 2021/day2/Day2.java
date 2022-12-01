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
        int aim = 0;
        while (scanner.hasNextLine()){
            switch (scanner.next()){
                case "forward":
                    int distance = scanner.nextInt();
                    x += distance;
                    y += aim * distance;

                break;
                case "down":
                    aim += scanner.nextInt();
                break;
                case "up":
                    aim -= scanner.nextInt();
                break;
            }

        }
        scanner.close();
        System.out.println(x * y);

    }
}
