import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Arrays;
import java.util.Objects;
import java.util.Scanner;

class Property{
    int label,
        numPixels,
        minR,
        minC,
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

        equalArray = new int[(numRows * numCols)/4];
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

    void imageReformat(FileWriter outFile) throws IOException {
        outFile.write(numRows + " " + numCols + " " + minVal + " " + maxVal + "\n");

        int curWidth, pixelWidth = Integer.toString(maxVal).length();

        for(int r = 1; r < numRows + 1; r++){
            for(int c = 1; c < numCols + 1; c++){
                outFile.write(zeroFramedArray[r][c]);
                curWidth = Integer.toString(zeroFramedArray[r][c]).length();
                while(curWidth < pixelWidth){
                    outFile.write(" ");
                    curWidth++;
                }
            }
            outFile.write('\n');
        }
    }

    void connected4(FileWriter reformatPrettyPrintFile, FileWriter debugFile) throws IOException {
        debugFile.write("Entering connect4 method\n");

        // 1
        connect4Pass1();
        debugFile.write("After connected4 pass1, newLabel = " + newLabel + "\n");
        reformatPrettyPrintFile.write("After connected4 pass1, zeroFramedArray");
        imageReformat(reformatPrettyPrintFile);
        reformatPrettyPrintFile.write("After connected4 pass1, equalArray");
        printEqualArray(reformatPrettyPrintFile);

        // 2
        connect4Pass2();
        debugFile.write("After connected4 pass2, newLabel = " + newLabel);
        reformatPrettyPrintFile.write("After connected4 pass2");
        reformatPrettyPrintFile.write("After connected4 pass2, zeroFramedArray");
        imageReformat(reformatPrettyPrintFile);
        reformatPrettyPrintFile.write("After connected4 pass2, equalArray");
        printEqualArray(reformatPrettyPrintFile);

        // 3
        trueNumConnectedComponent = manageEqualArray();
        reformatPrettyPrintFile.write("After connected4 manageEqualArray(), equalArray");
        printEqualArray(reformatPrettyPrintFile);
        newMin = 0;
        newMax = trueNumConnectedComponent;
        connectedComponentProperty = new Property[newMax + 1];
        debugFile.write("In connected4, after manageEqualArray, trueNumConnectedComponent = "
                            + trueNumConnectedComponent + "\n");

        // 4
        connectPass3(debugFile);

        reformatPrettyPrintFile.write("After connected4 pass3, zeroFramedArray");
        imageReformat(reformatPrettyPrintFile);
        reformatPrettyPrintFile.write("After connected4 pass3, equalArray");
        printEqualArray(reformatPrettyPrintFile);

        debugFile.write("Leaving connected4 method");
    }

    void connected8(FileWriter reformatPrettyPrintFile, FileWriter debugFile) throws IOException {
        debugFile.write("Entering connect8 method\n");

        // 1
        connect8Pass1();
        debugFile.write("After connected8 pass1, newLabel = " + newLabel + "\n");
        reformatPrettyPrintFile.write("After connected8 pass1, zeroFramedArray");
        imageReformat(reformatPrettyPrintFile);
        reformatPrettyPrintFile.write("After connected8 pass1, equalArray");
        printEqualArray(reformatPrettyPrintFile);

        // 2
        connect8Pass2();
        debugFile.write("After connected8 pass2, newLabel = " + newLabel);
        reformatPrettyPrintFile.write("After connected8 pass2");
        reformatPrettyPrintFile.write("After connected8 pass2, zeroFramedArray");
        imageReformat(reformatPrettyPrintFile);
        reformatPrettyPrintFile.write("After connected8 pass2, equalArray");
        printEqualArray(reformatPrettyPrintFile);

        // 3
        trueNumConnectedComponent = manageEqualArray();
        reformatPrettyPrintFile.write("After connected8 manageEqualArray(), equalArray");
        printEqualArray(reformatPrettyPrintFile);
        newMin = 0;
        newMax = trueNumConnectedComponent;
        connectedComponentProperty = new Property[newMax + 1];
        debugFile.write("In connected8, after manageEqualArray, trueNumConnectedComponent = "
                + trueNumConnectedComponent + "\n");

        // 4
        connectPass3(debugFile);

        reformatPrettyPrintFile.write("After connected8 pass3, zeroFramedArray");
        imageReformat(reformatPrettyPrintFile);
        reformatPrettyPrintFile.write("After connected8 pass3, equalArray");
        printEqualArray(reformatPrettyPrintFile);

        debugFile.write("Leaving connected8 method");
    }

    void connect8Pass1(){
        // zeroFramedArray, newLabel, equalArray
        int[] neighbors;
        int minLabel = 0;
        for (int i = 1; i < numRows + 1; i++) {
            for (int j = 1; j < numCols + 1; j++) {
                if (zeroFramedArray[i][j] == 0)
                    continue;
                neighbors = new int[]{
                        zeroFramedArray[i-1][j-1],
                        zeroFramedArray[i-1][j],
                        zeroFramedArray[i-1][j+1],
                        zeroFramedArray[i][j-1]
                };
                if(neighbors[0] == 0 && neighbors[1] == 0 && neighbors[2] == 0 && neighbors[3] == 0){
                    zeroFramedArray[i][j] = ++newLabel;
                }
                else if(neighbors[0] == neighbors[1] && neighbors[0] == neighbors[2] && neighbors[0] == neighbors[3]){
                    zeroFramedArray[i][j] = neighbors[0];
                }
                else{
                    // find minLabel
                    Arrays.sort(neighbors);
                    for(int label: neighbors){
                        if(label != 0){
                            minLabel = label;
                            break;
                        }
                    }
                    zeroFramedArray[i][j] = minLabel;

                    // update equalArray
                    for(int label: neighbors){
                        if(label != 0){
                            equalArray[label] = minLabel;
                        }
                    }
                }

            }
        }
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
            System.out.println("Invalid input");
            return;
        }

        ConnectedComponentLabel connectedComponentLabel = new ConnectedComponentLabel(inFile);

        switch (connectness){
            case "4":
                connectedComponentLabel.connected4(reformatPrettyPrintFile, debugFile);
                break;

            case "8":
                connectedComponentLabel.connected8(reformatPrettyPrintFile, debugFile);
                break;

            default:
                System.out.println("Invalid connectness input. Should be either '4' or '8'");
                return;
        }



    }
}
