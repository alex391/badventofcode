import java.io.File;
import java.io.FileNotFoundException;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;

public class Day1{
    public static List<Integer> readin(String filename){
        List<Integer> list = new LinkedList<>();
        try {
            File f = new File(filename);
            Scanner scanner = new Scanner(f);
            while (scanner.hasNextLine()){
                list.add(scanner.nextInt());
            }
            scanner.close();
            return list;
        }
        catch(FileNotFoundException fnfe){
            System.out.println("dumbass you have the wrong filename or some shit");
            return null;
        }
    }
    public static void main(String[] args) {
        List<Integer> depths = readin("input.txt");
        int prev = Integer.MAX_VALUE;
        int count = 0;
        for(int i = 0; i < depths.size() - 2; i++){
            int sum = 0;
            for (int j = i; j <= i + 2; j ++){
                sum += depths.get(j);
            }
            if (sum > prev){
                count ++;
            }
            prev = sum;
        }
        System.out.println(count);
    }
}