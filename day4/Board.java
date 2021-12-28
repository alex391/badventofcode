package day4;

public class Board {
    private int[][] board = new int[5][5];
    private boolean[][] markers = new boolean[5][5];

    public Board(int[][] board){
        this.board = board;
    }

    public int[][] getBoard(){
        return board;
    }
    public boolean[][] getMarkers(){
        return markers;
    }
    /**
     * Get the sum of all the unmarked numbers on the board (for calculating the score)
     * @return sum of all the unmarked numbers on the board
     */
    private int sumOfUnmarked(){
        int sum = 0;
        for(int i = 0; i < board.length; i++){
            for (int j = 0; j < board[i].length; j++){
                if(markers[i][j] == false)
                    sum += board[i][j];
            }
        }
        return sum;
    }
    /**
     * Mark a space on the board.
     * @param row row to mark
     * @param col column to mark
     * @return true if this board got marked, else false
     */
    private boolean mark(int draw){
        for(int i = 0; i < board.length; i++){
            for (int j = 0; j < board[i].length; j++){
                if (board[i][j] == draw){
                    markers[i][j] = true;
                    return true;
                }
            }
        }
        return false;
    }
    /**
     * First, mark the board, then check if we hava a bingo. If we have a bingo, return the score, else -1
     * @param draw the number that is getting drawn
     * @return the score of the board if we got a bingo, else -1
     */
    public int draw(int draw){
        if (mark(draw)){
            //if we marked something, check if it's a bingo
            //first, check the columns:
            for(int i = 0; i < board.length; i++){
                int count = 0;
                for (int j = 0; j < board[i].length; j++){
                    if (markers[i][j])
                        count ++;
                    if (count == board.length)
                        return sumOfUnmarked() * draw;
                }
            }
            //TODO: Then check the rows:
        }
        //if not, then no need to check
        return -1;
    }
    //TODO maybe a print function (bonus points if it colorizes the ones that have marks)
    public static void main(String[] args) {
        //TODO test the functions here
    }
}
