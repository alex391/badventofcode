package day1;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;

public class ReadFile{
    public static List<Integer> readin(String filename) throws FileNotFoundException{
        List<Integer> list = new LinkedList<>();
        File f = new File(filename);
        Scanner scanner = new Scanner(f);
        while (scanner.hasNextLine()){
            list.add(scanner.nextInt());
        }
        scanner.close();
        return list;
    }
}