package day3;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class Day3 {
    public static void main(String[] args) throws FileNotFoundException {
        File f = new File("day3/input.txt");
        Scanner scanner = new Scanner(f);
        int[] counts = new int[12];

        while (scanner.hasNext()){
            String line = scanner.next();
            char[] arr = line.toCharArray();
            for (int i = 0; i < counts.length; i ++){
                counts[i] += (arr[i] == '1' ? 1 : -1);
            }
            
        }
        scanner.close();
        int gamma = 0;
        for (int i = 0; i < counts.length; i++){
            gamma <<= 1;
            if(counts[i] > 0){
                gamma |= 1;
            }
        }
        int epsilon = ~gamma & 0xFFF;
        System.out.println(epsilon * gamma);
    }
    
}
