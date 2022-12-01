package day4;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.List;
import java.util.Scanner;

public class ReadFile{
    /**
     * Read from the file and return an array of the random numbers on the first line
     * @param filename
     * @return array containing the random numbers
     * @throws FileNotFoundException
     */
    public static int[] readrng(String filename) throws FileNotFoundException{
        File f = new File(filename);
        Scanner scanner = new Scanner(f);
        String line = scanner.nextLine();
        String[] splits = line.split(",");
        int[] rng = new int[splits.length];
        for(int i = 0; i < splits.length; i++)
            rng[i] = Integer.parseInt(splits[i]);
        scanner.close();
        return rng;
    }
    /**
     * Read in the boards to a list of boards
     * @return a list of boards
     */
    public static List<Board> readboards(){
        //TODO
        //make a new array
        //read
        //give to board

        return null;
    }
    /**
     * The goal of this main is to be equivilent to cat day4/input.txt
     * to verify that the readfile works as intended
     * @param args
     * @throws FileNotFoundException
     */
    public static void main(String[] args) throws FileNotFoundException {
        int[] rng = readrng("day4/input.txt");
        int i;
        for(i = 0; i < rng.length - 1; i++)
            System.out.printf("%d,", rng[i]);
        System.out.printf("%d", rng[i]); //no comma
        System.out.println();

        //TODO the rest of the main, after readboards
    }


}