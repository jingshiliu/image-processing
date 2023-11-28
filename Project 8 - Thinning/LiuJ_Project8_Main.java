import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

class Thinning{
    int numRows,
        numCols,
        minVal,
        maxVal,
        changeCount,
        cycleCount;
    int[][] array1;
    int[][] array2;
    int neighborAry[] = new int[9];

    Thinning(int numRows, int numCols, int minVal, int maxVal){
        this.numRows = numRows;
        this.numCols = numCols;
        this.minVal = minVal;
        this.maxVal = maxVal;
        changeCount = 0;
        cycleCount = 0;

        array1 = new int[numRows+2][numCols+2];
        array2 = new int[numRows+2][numCols+2];
    }

    int loadNeighbors(int[][] array, int row, int col){
        int index = 0, count = 0;
        for(int i = row-1; i < row+2; i++){
            for(int j = col-1; j < col+2; j++){
                if(i == row && j == col)
                    continue;
                if(array[i][j] > 0)
                    count++;
                neighborAry[index] = array[i][j];
                index++;
            }
        }
        return count;
    }

    void copyArrays(){
        for(int i = 0; i < numRows+2; i++){
            for(int j = 0; j < numCols+2; j++){
                array1[i][j] = array2[i][j];
            }
        }
    }

    void loadImage(Scanner input, int[][] array){
        for(int i = 1; i < numRows+1; i++){
            for(int j = 1; j < numCols+1; j++){
                array1[i][j] = input.nextInt();
            }
        }
    }

    int findMax(int[][] array){
        int max = array[0][0];
        for (int[] ints : array) {
            for (int anInt : ints) {
                if (anInt > max)
                    max = anInt;
            }
        }
        return max;
    }

    void imageReformat(FileWriter outFile, int[][] image) throws IOException {
        int curWidth, pixelWidth = Integer.toString(findMax(image)).length();

        for(int r = 1; r < numRows + 1; r++){
            for(int c = 1; c < numCols + 1; c++){
                curWidth = Integer.toString(image[r][c]).length();
                outFile.write(" ");
                while(curWidth < pixelWidth){
                    outFile.write(" ");
                    curWidth++;
                }
            }
            outFile.write('\n');
        }
    }

    void reformatPrettyPrint(FileWriter outFile, int[][] image) throws IOException {
        int curWidth, pixelWidth = Integer.toString(findMax(image)).length();

        for(int r = 1; r < numRows + 1; r++){
            for(int c = 1; c < numCols + 1; c++){
                if (image[r][c] == 0)
                    outFile.write(".");
                else
                    outFile.write(Integer.toString(image[r][c]));

                curWidth = Integer.toString(image[r][c]).length();
                outFile.write(" ");
                while(curWidth < pixelWidth){
                    outFile.write(" ");
                    curWidth++;
                }
            }
            outFile.write('\n');
        }
    }
}

public class LiuJ_Project8_Main {
    public static void main(String[] args) throws IOException {
        Scanner input = new Scanner(new FileReader(args[0]));
        FileWriter outFile = new FileWriter(args[1]);
        FileWriter debugFile = new FileWriter(args[2]);

        Thinning thinning = new Thinning(input.nextInt(), input.nextInt(), input.nextInt(), input.nextInt());
        outFile.write(thinning.numRows + " " + thinning.numCols + " " + thinning.minVal + " " + thinning.maxVal + "\n");

        thinning.loadImage(input, thinning.array1);

        outFile.write("In main(), before thinning, changeCount = " + thinning.changeCount + " cycleCount = " + thinning.cycleCount + "\n");
        thinning.reformatPrettyPrint(outFile, thinning.array1);


        input.close();
        outFile.close();
        debugFile.close();
    }
}