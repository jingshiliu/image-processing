import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.lang.reflect.Array;
import java.util.Arrays;
import java.util.Scanner;

public class ThresholdSelection{
    private int numRows, numCols, minVal, maxVal;
    private int[] histogramArray;
    private int[] gaussArray;
    private int biGaussThrVal;
    private int maxHeight;

    ThresholdSelection(int numRows, int numCols, int minVal, int maxVal){
        this.numRows = numRows;
        this.numCols = numCols;
        this.minVal = minVal;
        this.maxVal = maxVal;
        this.maxHeight = 0;

        this.histogramArray = new int[maxVal + 1];
        this.gaussArray = new int[maxVal + 1];
    }

    public int getBiGaussThrVal() {
        return biGaussThrVal;
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

    public void setZero(int[] array){
        Arrays.fill(array, 0);
    }

    public int biGauss(FileWriter debugFile) throws IOException {
        debugFile.write("Entering biGauss method\n");
        double sum1, sum2, total, minSumDiff = 99999.0;
        int offset = (maxVal - minVal) / 10,
            bestThrVal = offset;

        for (int dividePoint = offset; dividePoint < maxVal - offset; dividePoint++) {
            setZero(this.gaussArray);
            sum1 = fitGauss(0, dividePoint, debugFile);
            sum2 = fitGauss(dividePoint, maxVal, debugFile);
            total = sum1 + sum2;
            if (total < minSumDiff){
                minSumDiff = total;
                bestThrVal = dividePoint;
            }
            debugFile.write(dividePoint + " " + sum1 + " " + sum2 + " " + total + " " + minSumDiff + " "
                                + bestThrVal + "\n");

        }
        this.biGaussThrVal = bestThrVal;
        debugFile.write("Leaving biGauss method\n");
        return bestThrVal;
    }

    public double computeMean(int left, int right, FileWriter debugFile) throws IOException {
        debugFile.write("Entering computeMean method\n");
        int numPixels = 0;
        double sum = 0.0;

        for (int i = left; i <= right; i++) {
            sum += histogramArray[i] * i;
            numPixels += histogramArray[i];
            this.maxHeight = Math.max(this.maxHeight, histogramArray[i]);
        }
        debugFile.write("Leaving computeMean method\n");
        return sum / numPixels;
    }

    public double computeVariance(int left, int right, double mean, FileWriter debugFile) throws IOException {
        debugFile.write("Entering computeVariance method\n");
        double sum = 0.0;
        int numPixels = 0;
        for (int i = left; i <= right; i++) {
            sum += (double)histogramArray[i] * Math.pow(((double)i - mean), 2);
            numPixels += histogramArray[i];
        }
        debugFile.write("Leaving computeVariance method\n");
        return sum / (double) numPixels;
    }

    public double modifiedGauss(int x, double mean, double variance){
        return (double)this.maxHeight * Math.exp(-(Math.pow((double)x - mean , 2) / (2.0 * variance)));
    }

    public double fitGauss(int left, int right, FileWriter debugFile) throws IOException {
        debugFile.write("Entering fitGauss method\n");
        double mean, variance, sum = 0.0, gaussVal, maxGaussVal;
        mean = computeMean(left, right, debugFile);
        variance = computeVariance(left, right, mean, debugFile);

        for (int i = left; i <= right; i++) {
            gaussVal = modifiedGauss(i, mean, variance);
            sum += Math.abs(gaussVal - (double) histogramArray[i]);
            gaussArray[i] = (int)gaussVal;

        }
        debugFile.write("Leaving firGauss method\n");
        return sum;
    }

    public static void main(String[] args) throws IOException {
                Scanner inFile = new Scanner(new FileReader(args[0]));
                FileWriter  outFile = new FileWriter(args[1]),
                            debugFile = new FileWriter(args[2]);
                int numRows = inFile.nextInt(),
                    numCols = inFile.nextInt(),
                    minVal = inFile.nextInt(),
                    maxVal = inFile.nextInt();

                ThresholdSelection thresholdSelection = new ThresholdSelection(numRows, numCols, minVal, maxVal);
                thresholdSelection.loadHist(inFile);
                thresholdSelection.displayHistogram(outFile);
                thresholdSelection.biGauss(debugFile);

                outFile.write("The Bi-Gaussian Value is " + thresholdSelection.getBiGaussThrVal());

                inFile.close();
                outFile.close();
                debugFile.close();
            }
}