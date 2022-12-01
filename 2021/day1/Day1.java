package day1;

import java.io.FileNotFoundException;
import java.util.List;
public class Day1{
    public static void main(String[] args) throws FileNotFoundException{
        List<Integer> depths = ReadFile.readin("day1/input.txt");
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