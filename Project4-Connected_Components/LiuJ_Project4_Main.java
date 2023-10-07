import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Arrays;
import java.util.Scanner;

class Property{
    int label,
        numPixels,
        minR,
        maxR,
        maxC;
}

class ConnectedComponentLabel{
    int numRows,
        numCols,
        minVal,
        maxVal,
        newLabel,
        trueNumConnectedComponent,
        newMin,
        newMax;
    int[][] zeroFramedArray;
    int[] nonZeroNeighborArray;
    int[] equalArray;
    char option;
    Property[] connectedComponentProperty;

    ConnectedComponentLabel(Scanner inFile){
        numRows = inFile.nextInt();
        numCols = inFile.nextInt();
        minVal = inFile.nextInt();
        maxVal = inFile.nextInt();

        zeroFramedArray = new int[numRows + 2][numCols + 2];
        newLabel = 0;

        loadImage(inFile);
    }

    void zero2D(int[][] array){
        for (int[] ints : array) {
            Arrays.fill(ints, 0);
        }
    }

    void negative1D(){

    }

    void loadImage(Scanner inFile){
        for (int i = 1; i < numRows + 1; i++) {
            for (int j = 1; j < numCols + 1; j++) {
                zeroFramedArray[i][j] = inFile.nextInt();
            }
        }
    }

    void imageReformat(){

    }

    void connect8Pass1(){

    }

    void connect8Pass2(){

    }

    void connect4Pass1(){

    }

    void connect4Pass2(){

    }

    void connectPass3(){

    }

    void updateEqualArray(){

    }

    int manageEqualArray(){
        return 0;
    }

    void printConnectedComponentProperty(){

    }

    void printEqualArray(){

    }

    void drawBoxes(){

    }

    void printImage(){

    }


}

public class LiuJ_Project4_Main {
    public static void main(String[] args) {
        Scanner inFile;
        String connectness = args[1];
        FileWriter reformatPrettyPrintFile, labelFile, propertyFile, debugFile;

        try{
            inFile = new Scanner(new FileReader(args[0]));
            reformatPrettyPrintFile = new FileWriter(args[2]);
            labelFile = new FileWriter(args[3]);
            propertyFile = new FileWriter(args[4]);
            debugFile = new FileWriter(args[5]);

        }catch (IOException exception){
            System.out.println(exception);
        }

        ConnectedComponentLabel connectedComponentLabel = new ConnectedComponentLabel(inFile);

    }
}
