import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Arrays;
import java.util.Scanner;

class Property{
    int label,
        numPixels,
        minR,
        minC,
        maxR,
        maxC;
    Property(int label, int numPixels, int minR, int minC, int maxR, int maxC){
        this.label = label;
        this.numPixels = numPixels;
        this.minR = minR;
        this.minC = minC;
        this.maxR = maxR;
        this.maxC = maxC;
    }
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
        for (int i = 0; i < equalArray.length; i++) {
            equalArray[i] = i;
        }
        zeroFramedArray = new int[numRows + 2][numCols + 2];
        newLabel = 0;

        loadImage(inFile);
    }

    void zero2D(int[][] array){
        for (int[] ints : array) {
            Arrays.fill(ints, 0);
        }
    }

    void negative1D(int[] array){
        Arrays.fill(array, -1);
    }

    void loadImage(Scanner inFile){
        for (int i = 1; i < numRows + 1; i++) {
            for (int j = 1; j < numCols + 1; j++) {
                zeroFramedArray[i][j] = inFile.nextInt();
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

    void imageReformat(FileWriter outFile) throws IOException {
        int curWidth, pixelWidth = Integer.toString(findMax(zeroFramedArray)).length();

        for(int r = 1; r < numRows + 1; r++){
            for(int c = 1; c < numCols + 1; c++){
                if (zeroFramedArray[r][c] == 0)
                    outFile.write(".");
                else
                    outFile.write(Integer.toString(zeroFramedArray[r][c]));

                curWidth = Integer.toString(zeroFramedArray[r][c]).length();
                outFile.write(" ");
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
        reformatPrettyPrintFile.write("After connected4 pass1, zeroFramedArray\n");
        imageReformat(reformatPrettyPrintFile);
        reformatPrettyPrintFile.write("\n\nAfter connected4 pass1, equalArray\n");
        printEqualArray(reformatPrettyPrintFile);

        // 2
        connect4Pass2();
        debugFile.write("After connected4 pass2, newLabel = " + newLabel);
        reformatPrettyPrintFile.write("\n\nAfter connected4 pass2\n");
        reformatPrettyPrintFile.write("\n\nAfter connected4 pass2, zeroFramedArray\n");
        imageReformat(reformatPrettyPrintFile);
        reformatPrettyPrintFile.write("\n\nAfter connected4 pass2, equalArray\n");
        printEqualArray(reformatPrettyPrintFile);

        // 3
        trueNumConnectedComponent = manageEqualArray();
        reformatPrettyPrintFile.write("\n\nAfter connected4 manageEqualArray(), equalArray\n");
        printEqualArray(reformatPrettyPrintFile);
        newMin = 0;
        newMax = trueNumConnectedComponent;
        connectedComponentProperty = new Property[newMax + 1];
        debugFile.write("In connected4, after manageEqualArray, trueNumConnectedComponent = "
                            + trueNumConnectedComponent + "\n");

        // 4
        connectPass3(debugFile);

        reformatPrettyPrintFile.write("\n\nAfter connected4 pass3, zeroFramedArray\n");
        imageReformat(reformatPrettyPrintFile);
        reformatPrettyPrintFile.write("\n\nAfter connected4 pass3, equalArray\n");
        printEqualArray(reformatPrettyPrintFile);

        debugFile.write("Leaving connected4 method\n");
    }

    void connected8(FileWriter reformatPrettyPrintFile, FileWriter debugFile) throws IOException {
        debugFile.write("Entering connect8 method\n");

        // 1
        connect8Pass1();
        debugFile.write("After connected8 pass1, newLabel = " + newLabel + "\n");
        reformatPrettyPrintFile.write("After connected8 pass1, zeroFramedArray\n");
        imageReformat(reformatPrettyPrintFile);
        reformatPrettyPrintFile.write("\n\nAfter connected8 pass1, equalArray\n");
        printEqualArray(reformatPrettyPrintFile);

        // 2
        connect8Pass2();
        debugFile.write("After connected8 pass2, newLabel = " + newLabel);
        reformatPrettyPrintFile.write("\n\nAfter connected8 pass2\n");
        reformatPrettyPrintFile.write("\n\nAfter connected8 pass2, zeroFramedArray\n");
        imageReformat(reformatPrettyPrintFile);
        reformatPrettyPrintFile.write("\n\nAfter connected8 pass2, equalArray\n");
        printEqualArray(reformatPrettyPrintFile);

        // 3
        trueNumConnectedComponent = manageEqualArray();
        reformatPrettyPrintFile.write("\n\nAfter connected8 manageEqualArray(), equalArray\n");
        printEqualArray(reformatPrettyPrintFile);
        newMin = 0;
        newMax = trueNumConnectedComponent;
        connectedComponentProperty = new Property[newMax + 1];
        debugFile.write("\nIn connected8, after manageEqualArray, trueNumConnectedComponent = "
                + trueNumConnectedComponent + "\n");

        // 4
        connectPass3(debugFile);

        reformatPrettyPrintFile.write("\n\nAfter connected8 pass3, zeroFramedArray\n");
        imageReformat(reformatPrettyPrintFile);
        reformatPrettyPrintFile.write("\n\nAfter connected8 pass3, equalArray\n");
        printEqualArray(reformatPrettyPrintFile);

        debugFile.write("Leaving connected8 method\n");
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
        // zeroFramedArray, equalArray
        int[] neighbors;
        int minLabel = 0;
        for (int i = numRows; i > 0 ; i--) {
            for (int j = numCols; j > 0 ; j--) {
                if(zeroFramedArray[i][j] == 0)
                    continue;
                neighbors = new int[]{
                        zeroFramedArray[i][j+1],
                        zeroFramedArray[i+1][j-1],
                        zeroFramedArray[i+1][j],
                        zeroFramedArray[i+1][j+1]
                };

                // do nothing if neighbors are 0 or have same label,
                // but we only have to check if they have same label bc the latter includes the former
                if(!(neighbors[0] == neighbors[1] && neighbors[0] == neighbors[2] && neighbors[0] == neighbors[3])){
                    Arrays.sort(neighbors);
                    for(int label: neighbors){
                        if(label != 0){
                            minLabel = label;
                            break;
                        }
                    }

                    zeroFramedArray[i][j] = minLabel;

                    for(int label: neighbors){
                        if(label != 0){
                            equalArray[label] = minLabel;
                        }
                    }
                }
            }
        }
    }

    void connect4Pass1(){
        // zeroFramedArray, newLabel, equalArray
        int[] neighbors;
        int minLabel = 0;
        for (int i = 1; i < numRows + 1; i++) {
            for (int j = 1; j < numCols + 1; j++) {
                if (zeroFramedArray[i][j] == 0)
                    continue;
                neighbors = new int[]{
                        zeroFramedArray[i-1][j],
                        zeroFramedArray[i][j-1]
                };
                if(neighbors[0] == 0 && neighbors[1] == 0){
                    zeroFramedArray[i][j] = ++newLabel;
                }
                else if(neighbors[0] == neighbors[1]){
                    zeroFramedArray[i][j] = neighbors[0];
                }
                else{
                    // find minLabel
                    Arrays.sort(neighbors);
                    minLabel = neighbors[0];
                    if(neighbors[0] == 0){
                        minLabel = neighbors[1];
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

    void connect4Pass2(){
        // zeroFramedArray, equalArray
        int[] neighbors;
        int minLabel = 0;
        for (int i = numRows; i > 0 ; i--) {
            for (int j = numCols; j > 0 ; j--) {
                if(zeroFramedArray[i][j] == 0)
                    continue;
                neighbors = new int[]{
                        zeroFramedArray[i][j+1],
                        zeroFramedArray[i+1][j]
                };

                // do nothing if neighbors are 0 or have same label,
                // but we only have to check if they have same label bc the latter includes the former
                if(!(neighbors[0] == neighbors[1])){
                    Arrays.sort(neighbors);
                    minLabel = neighbors[0];
                    if(neighbors[0] == 0){
                        minLabel = neighbors[1];
                    }

                    zeroFramedArray[i][j] = minLabel;
                    for(int label: neighbors){
                        if(label != 0){
                            equalArray[label] = minLabel;
                        }
                    }
                }
            }
        }
    }

    void connectPass3(FileWriter debugFile) throws IOException {
        debugFile.write("Entering connectPass3()\n");
        for (int i = 1; i < connectedComponentProperty.length; i++) {
            connectedComponentProperty[i] = new Property(
                    i,
                    0,
                    numRows,
                    numCols,
                    0,
                    0
            );
        }
        Property property;
        for (int r = 1; r <= numRows; r++) {
            for (int c = 1; c <= numCols; c++) {
                if (zeroFramedArray[r][c] == 0)
                    continue;
                // relabelling
                zeroFramedArray[r][c] = equalArray[zeroFramedArray[r][c]];
                // property at index i associate with group i
                property = connectedComponentProperty[zeroFramedArray[r][c]];
                property.numPixels++;
                if(r < property.minR)
                    property.minR = r;
                if(r > property.maxR)
                    property.maxR = r;
                if(c < property.minC)
                    property.minC = c;
                if(c > property.maxC)
                    property.maxC = c;
            }
        }
        debugFile.write("Leaving connectedPass3()\n");
    }

    void updateEqualArray(){

    }

    int manageEqualArray(){
        int readLabel = 0;
        for (int i = 1; i <= newLabel; i++) {
            if(i == equalArray[i])
                equalArray[i] = ++readLabel;
            else
                equalArray[i] = equalArray[equalArray[i]];
        }
        return readLabel;
    }

    void printConnectedComponentProperty(FileWriter outFile) throws IOException{
        outFile.write(numRows + " " + numCols + " " + newMin + " " + newMax + "\n");
        outFile.write(trueNumConnectedComponent + "\n");
        for (int i = 1; i <= trueNumConnectedComponent; i++) {
            outFile.write(connectedComponentProperty[i].label + "\n"
                            + connectedComponentProperty[i].numPixels + "\n"
                            + (connectedComponentProperty[i].minR - 1) + " "
                            +( connectedComponentProperty[i].minC - 1) + "\n"
                            + (connectedComponentProperty[i].maxR - 1) + " "
                            + (connectedComponentProperty[i].maxC - 1) + "\n");
        }


    }

    void printEqualArray(FileWriter outFile) throws IOException{
        for (int i = 1; i <= newLabel; i++) {
            outFile.write(equalArray[i] + " ");
        }
        outFile.write("\n");
    }

    void drawBoxes(FileWriter debugFile) throws IOException{
        debugFile.write("Entering drawBoxes()\n");
        int label;
        for (int i = 1; i <= trueNumConnectedComponent; i++) {
            label = connectedComponentProperty[i].label;
            for (int r = connectedComponentProperty[i].minR; r <= connectedComponentProperty[i].maxR; r++) {
                zeroFramedArray[r][connectedComponentProperty[i].minC] = label;
                zeroFramedArray[r][connectedComponentProperty[i].maxC] = label;
            }
            for (int c = connectedComponentProperty[i].minC; c <= connectedComponentProperty[i].maxC; c++) {
                zeroFramedArray[connectedComponentProperty[i].minR][c] = label;
                zeroFramedArray[connectedComponentProperty[i].maxR][c] = label;
            }
        }
        debugFile.write("Leaving drawBoxes()\n");
    }

    void printImage(FileWriter outFile) throws IOException{
        outFile.write(numRows + " " + numCols + " " + newMin + " " + newMax + "\n");
        for (int i = 1; i < numRows + 1; i++) {
            for (int j = 1; j < numCols + 1; j++) {
                outFile.write(zeroFramedArray[i][j] + " ");
            }
            outFile.write("\n");
        }

    }


}

public class LiuJ_Project4_Main {
    public static void main(String[] args) throws IOException {
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

        connectedComponentLabel.printImage(labelFile);
        connectedComponentLabel.printConnectedComponentProperty(propertyFile);
        connectedComponentLabel.drawBoxes(debugFile);

        reformatPrettyPrintFile.write("\n\nAfter connectedComponentLabel.drawBoxes()\n");
        connectedComponentLabel.imageReformat(reformatPrettyPrintFile);
        reformatPrettyPrintFile.write("\nTrue Number of Connected Component: "
                                        + connectedComponentLabel.trueNumConnectedComponent + '\n');

        inFile.close();
        reformatPrettyPrintFile.close();
        labelFile.close();
        propertyFile.close();
        debugFile.close();
    }
}
