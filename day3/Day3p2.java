package day3;

import java.io.FileNotFoundException;
import java.util.Iterator;
import java.util.List;

public class Day3p2 {
    public static void main(String[] args) throws FileNotFoundException {
        List<Integer> in = ReadFile.readin("day3/input.txt");
        //First, find the O2 rating
        int o2;
        int i = 12;
        while (in.size() > 1){ //exit when there is only one number left
            //count
            Iterator<Integer> itt = in.listIterator();
            int count = 0;
            while(itt.hasNext()){
                Integer next = itt.next();
                count += ((next >> i) & 1) == 1 ? 1 : -1;
            }
            System.out.println(count);
            //then eliminate
            i --;
            if (i <= 0) break; //TODO remove this exit condition once we're done
            
        }
    }
    
}
