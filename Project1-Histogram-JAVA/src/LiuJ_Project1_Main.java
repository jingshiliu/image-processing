import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

class ThresholdSelection{
    int numRows, numCols, minVal, maxVal;
    int[] histogramArray;
    int[] gaussArray;
    int biGaussThrVal;
    int maxHeight;

    ThresholdSelection(int numRows, int numCols, int minVal, int maxVal){
        this.numRows = numRows;
        this.numCols = numCols;
        this.minVal = minVal;
        this.maxVal = maxVal;
    }

    public int loadHist(){
        return 0;
    }

    public void displayHistogram(){

    }

    public void setZero(){

    }

    public int biGauss(){
        return 0;
    }

    public double computeMean(){
        return 0.0;
    }

    public double computeVariance(){
        return 0.0;
    }

    public void modifiedGauss(int x, double mean, double variance){

    }

    public void fitGauss(){

    }
}


class Liu_Project1_Main{
    public static void main(String[] args) throws IOException {
        Scanner inFile = new Scanner(new FileReader(args[0]));
        FileWriter  outFile = new FileWriter(args[1]),
                    debugFile = new FileWriter(args[2]);
        int numRows = inFile.nextInt(),
            numCols = inFile.nextInt(),
            minVal = inFile.nextInt(),
            maxVal = inFile.nextInt();

        int[] histogramArray = new int[maxVal + 1];

    }
}