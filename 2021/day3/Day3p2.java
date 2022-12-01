package day3;

import java.io.FileNotFoundException;
import java.util.Iterator;
import java.util.List;

public class Day3p2 {
    public static void main(String[] args) throws FileNotFoundException {
        List<Integer> in = ReadFile.readin("day3/input.txt");
        //First, find the O2 rating
        int o2;
        int i = 11;
        while (in.size() > 1){ //exit when there is only one number left
            //count
            Iterator<Integer> itt = in.listIterator();
            int count = 0;
            while(itt.hasNext()){
                int next = itt.next();
                //basically what I did in part one, but this time vertically
                count += ((next >> i) & 1) == 1 ? 1 : -1; 
            }
            int toEleimante = (count >= 0) ? 1 : 0; //If 0 and 1 are equally common, keep values with a 1 in the position being considered.
            final int finalI = i; //inner classes can't be initialized with variables that can change, so final
            in.removeIf(j -> ((j >> finalI) & 1) == toEleimante);
            i --;
        }
        o2 = in.get(0);
        System.out.printf("%12d\n",Long.parseLong(Integer.toBinaryString(o2)));
        in = ReadFile.readin("day3/input.txt");
        //then, the co2 rating
        int co2;
        i = 11;
        //I know I shouldn't repeat myself, but:
        while (in.size() > 1){ //exit when there is only one number left
            //count
            Iterator<Integer> itt = in.listIterator();
            int count = 0;

            while(itt.hasNext()){
                int next = itt.next();
                //basically what I did in part one, but this time vertically
                count += ((next >> i) & 1) == 1 ? -1 : 1; //flipped
            }
            int toEleimante = (count > 0) ? 1 : 0; //If 0 and 1 are equally common, keep values with a 0 in the position being considered.
            final int finalI = i; //inner classes can't be initialized with variables that can change so final
            in.removeIf(j -> ((j >> finalI) & 1) == toEleimante);
            i --;
        }
        co2 = in.get(0);
        System.out.printf("%12d\n",Long.parseLong(Integer.toBinaryString(co2)));
        System.out.println(co2 * o2);
    }
    
}
