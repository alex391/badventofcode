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
            int binary = 0;
            for (int i = 0; i < arr.length; i ++){
                binary |= (arr[i] == '1') ? 1 : 0;
                binary <<= 1; //This is adding on an extra 0
            }
            list.add(binary);
        }
        scanner.close();
        return list;
    }

    public static void main(String[] args) throws FileNotFoundException {
        List<Integer> list = readin("day3/input.txt");
        for(int i: list){
            System.out.println(Integer.toBinaryString(i));
        }

    }
}