import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Arrays;
import java.util.Scanner;


class ConcavityThreshold{
    int numRows, numCols, minVal, maxVal;
    int x1, x2, y1, y2;
    double m, b;
    int[] histogramArray;
    int maxHeight;
    char[][] histGraph;
    int bestThrVal;

    ConcavityThreshold(Scanner inFile1, Scanner inFile2){
        numRows = inFile1.nextInt();
        numCols = inFile1.nextInt();
        minVal = inFile1.nextInt();
        maxVal = inFile1.nextInt();
        x1 = inFile2.nextInt();
        y1 = inFile2.nextInt();
        x2 = inFile2.nextInt();
        y2 = inFile2.nextInt();

        histogramArray = new int[maxVal+1];
        maxHeight = loadHist(inFile1);
        histGraph = new char[maxVal+1][maxHeight+10];
        for(char[] hist: histGraph){
            Arrays.fill(hist, ' ');
        }
        plotHistGraph();
    }

    public int loadHist(Scanner inFile){
        int maxHeight = 0, grayScaleVal, grayScaleHeight;
        while(inFile.hasNext()){
            grayScaleVal = inFile.nextInt();
            grayScaleHeight = inFile.nextInt();

            this.histogramArray[grayScaleVal] = grayScaleHeight;
            maxHeight = Math.max(maxHeight, grayScaleHeight);
        }
        this.maxHeight = maxHeight;
        return maxHeight;
    }

    public void displayHistogram(FileWriter outFile) throws IOException {
        outFile.write(numRows + ' ' + numCols + ' ' + minVal + ' ' + maxVal + '\n');
        for (int i = 0; i < this.histogramArray.length; i++) {
            outFile.write(i + " (" + this.histogramArray[i] + "):" );
            for (int j = 0; j < this.histogramArray[i]; j++) {
                outFile.write('+');
            }
            outFile.write('\n');
        }
    }

    void plotHistGraph(){
        for (int i = 0; i < maxVal + 1; i++) {
            histGraph[i][histogramArray[i]] = '*';
        }
    }

    void lineEQ(){
        m = (double)(y2 - y1) / (double)(x2 - x1);
        b = (double)y1 - (m * (double)x1 );
    }

    int deepestConcavity(){
        int max = 0, bestThrVal = x1;
        int y, gap;
        for (int x = x1; x <= x2; x++) {
            y = (int)(m * x + b);
            histGraph[x][y] = '+';
            gap = Math.abs(histogramArray[x] - y);
            if (gap > max){
                max = gap;
                bestThrVal = x;
            }
        }
        return bestThrVal;
    }

    public void printHistGraph(FileWriter outFile) throws IOException {
        outFile.write("Histogram Graph\n");
        for (int i = 0; i < histGraph.length; i++) {
            for (int j = 0; j < histGraph[i].length; j++) {
                outFile.write(histGraph[i][j]);
            }
            outFile.write('\n');
        }
    }
}

public class LiuJ_Q1_Java {
    public static void main(String[] args) throws IOException {
        Scanner inFile1 = new Scanner(new FileReader(args[0]));
        Scanner inFile2 = new Scanner(new FileReader(args[1]));
        FileWriter outFile = new FileWriter(args[2]);

        ConcavityThreshold concavityThreshold = new ConcavityThreshold(inFile1, inFile2);
        outFile.write("Histogram\n");
        concavityThreshold.displayHistogram(outFile);
        outFile.write("\n\n");

        concavityThreshold.lineEQ();
        int bestThrVal = concavityThreshold.deepestConcavity();
        outFile.write("The bestThrVal is: " + bestThrVal + "\n\n");

        outFile.write("Histogram with line between peaks\n");
        concavityThreshold.printHistGraph(outFile);

        inFile2.close();
        inFile1.close();
        outFile.close();

    }
}