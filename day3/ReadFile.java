package day3;

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
            String line = scanner.nextLine();
            char[] arr = line.toCharArray();
            int 
            for (int i = 0; i < arr.length; i ++){

                counts[i] += (arr[i] == '1' ? 1 : -1);
            }
        }
        scanner.close();
        return list;
    }
}