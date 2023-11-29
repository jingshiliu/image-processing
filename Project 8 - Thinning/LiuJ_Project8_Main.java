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
    int[] neighborAry = new int[8];
    int[][] connectors = new int[][] { // neighbor confiig, index: 0-7 skip self
            new int[]{-1, 0, -1, -1, -1, -1, 0, -1 },
            new int[]{-1, -1, -1, 0, 0, -1, -1, -1 },
            new int[]{1, 0, -1, 0, -1, -1, -1, -1 },
            new int[]{-1, 0, 1, -1, 0, -1, -1, -1 },
            new int[]{-1, -1, -1, 0, -1, 1, 0, -1 },
            new int[]{-1, -1, -1, -1, 0, -1, 0, 1 },
    };

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

    void thinning(FileWriter debugFile) throws IOException{
        debugFile.write("Entering thinning(), before thinning 4 sides, arrayOne is below\n");
        reformatPrettyPrint(debugFile, array1);
        changeCount = 0;

        northThinning(debugFile);
        debugFile.write("After northThinning(), arrayTwo is below\n");
        reformatPrettyPrint(debugFile, array2);
        copyArrays();

        southThinning(debugFile);
        debugFile.write("After southThinning(), arrayTwo is below\n");
        reformatPrettyPrint(debugFile, array2);
        copyArrays();

        westThinning(debugFile);
        debugFile.write("After westThinning(), arrayTwo is below\n");
        reformatPrettyPrint(debugFile, array2);
        copyArrays();

        eastThinning(debugFile);
        debugFile.write("After eastThinning(), arrayTwo is below\n");
        reformatPrettyPrint(debugFile, array2);
        copyArrays();

        debugFile.write("Leaving thinning(), cycleCount = " + cycleCount + " changeCount = " + changeCount + "\n");
    }

    boolean checkConnector(){
        boolean flag = false;
        for(int[] connector: connectors){
            flag = true;
            for(int i = 0; i < neighborAry.length; i++){
                if(connector[i] == -1)
                    continue;
                if(connector[i] != neighborAry[i]){
                    flag = false;
                    break;
                }
            }
            if(flag)
                return true;
        }
        return false;
    }

    void northThinning(FileWriter debugFile) throws IOException {
        debugFile.write("Entering northThinning(), cycleCount = " + cycleCount + " changeCount = " + changeCount + "\n");
        for (int i = 1; i < numRows + 1; i++) {
            for (int j = 1; j < numCols + 1; j++) {
                if (!(array1[i][j] > 0 && array1[i - 1][j] == 0)) {
                    array2[i][j] = array1[i][j];
                    continue;
                }

                int nonZeroCount = loadNeighbors(array1, i, j);
                boolean flag = checkConnector();
                debugFile.write("In northThinning, i = " + i + " j = " + j + " nonZeroCount = " + nonZeroCount + " flag = " + flag + "\n");

                if(nonZeroCount >= 4 && !flag){
                    array2[i][j] = 0;
                    changeCount++;
                }else{
                    array2[i][j] = array1[i][j];
                }
            }
        }
        debugFile.write("Leaving northThinning(), cycleCount = " + cycleCount + " changeCount = " + changeCount + "\n");
    }

    void southThinning(FileWriter debugFile) throws IOException{
        debugFile.write("Entering southThinning(), cycleCount = " + cycleCount + " changeCount = " + changeCount + "\n");
        for (int i = 1; i < numRows + 1; i++) {
            for (int j = 1; j < numCols + 1; j++) {
                if (!(array1[i][j] > 0 && array1[i + 1][j] == 0)) continue;

                int nonZeroCount = loadNeighbors(array1, i, j);
                boolean flag = checkConnector();
                debugFile.write("In southThinning, i = " + i + " j = " + j + " nonZeroCount = " + nonZeroCount + " flag = " + flag + "\n");

                if(nonZeroCount >= 4 && !flag){
                    array2[i][j] = 0;
                    changeCount++;
                }else{
                    array2[i][j] = array1[i][j];
                }
            }
        }
        debugFile.write("Leaving southThinning(), cycleCount = " + cycleCount + " changeCount = " + changeCount + "\n");

    }

    void westThinning(FileWriter debugFile) throws IOException{
        debugFile.write("Entering westThinning(), cycleCount = " + cycleCount + " changeCount = " + changeCount + "\n");
        for (int i = 1; i < numRows + 1; i++) {
            for (int j = 1; j < numCols + 1; j++) {
                if (!(array1[i][j] > 0 && array1[i][j - 1] == 0)) continue;

                int nonZeroCount = loadNeighbors(array1, i, j);
                boolean flag = checkConnector();
                debugFile.write("In westThinning, i = " + i + " j = " + j + " nonZeroCount = " + nonZeroCount + " flag = " + flag + "\n");

                if(nonZeroCount >= 4 && !flag){
                    array2[i][j] = 0;
                    changeCount++;
                }else{
                    array2[i][j] = array1[i][j];
                }
            }
        }
        debugFile.write("Leaving westThinning(), cycleCount = " + cycleCount + " changeCount = " + changeCount + "\n");
    }

    void eastThinning(FileWriter debugFile) throws IOException{
        debugFile.write("Entering eastThinning(), cycleCount = " + cycleCount + " changeCount = " + changeCount + "\n");
        for (int i = 1; i < numRows + 1; i++) {
            for (int j = 1; j < numCols + 1; j++) {
                if (!(array1[i][j] > 0 && array1[i][j + 1] == 0)) continue;

                int nonZeroCount = loadNeighbors(array1, i, j);
                boolean flag = checkConnector();
                debugFile.write("In eastThinning, i = " + i + " j = " + j + " nonZeroCount = " + nonZeroCount + " flag = " + flag + "\n");

                if(nonZeroCount >= 4 && !flag){
                    array2[i][j] = 0;
                    changeCount++;
                }else{
                    array2[i][j] = array1[i][j];
                }
            }
        }
        debugFile.write("Leaving eastThinning(), cycleCount = " + cycleCount + " changeCount = " + changeCount + "\n");
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
                outFile.write(image[r][c] + " ");
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

        do {
            thinning.thinning(debugFile);
            thinning.cycleCount++;
            outFile.write("In main(), after thinning, changeCount = " + thinning.changeCount + " cycleCount = " + thinning.cycleCount + "\n");
            thinning.reformatPrettyPrint(outFile, thinning.array1);
        }while(thinning.changeCount > 0);

        outFile.write("In main(), after thinning, changeCount = " + thinning.changeCount + " cycleCount = " + thinning.cycleCount + "\n");
        thinning.imageReformat(outFile, thinning.array1);

        input.close();
        outFile.close();
        debugFile.close();
    }
}