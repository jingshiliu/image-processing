import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

class ImageEnhancement{
    int numRows,
        numCols,
        minVal,
        maxval,
        thrVal;
    int[][] imgAry;
    int[][] outAry;
    int[][] thrAry;
    int[] avg = new int[8];
    int[][][] masks = {
            {
                    {0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0},
                    {0, 0, 1, 0, 0},
                    {0, 1, 1, 1, 0},
                    {1, 1, 1, 1, 1}
            },
            {
                    {1, 0, 0, 0, 0},
                    {1, 1, 0, 0, 0},
                    {1, 1, 1, 0, 0},
                    {1, 1, 0, 0, 0},
                    {1, 0, 0, 0, 0}
            },
            {
                    {1, 1, 1, 1, 1},
                    {0, 1, 1, 1, 0},
                    {0, 0, 1, 0, 0},
                    {0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0}
            },
            {
                    {0, 0, 0, 0, 1},
                    {0, 0, 0, 1, 1},
                    {0, 0, 1, 1, 1},
                    {0, 0, 0, 1, 1},
                    {0, 0, 0, 0, 1}
            },
            {
                    {1, 1, 1, 0, 0},
                    {1, 1, 1, 0, 0},
                    {1, 1, 1, 0, 0},
                    {0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0}
            },
            {
                    {0, 0, 1, 1, 1},
                    {0, 0, 1, 1, 1},
                    {0, 0, 1, 1, 1},
                    {0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0}
            },
            {
                    {0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0},
                    {0, 0, 1, 1, 1},
                    {0, 0, 1, 1, 1},
                    {0, 0, 1, 1, 1}
            },
            {
                    {0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0},
                    {1, 1, 1, 0, 0},
                    {1, 1, 1, 0, 0},
                    {1, 1, 1, 0, 0}
            }
    };

    ImageEnhancement(Scanner inFile, int thrVal){
        numRows = inFile.nextInt();
        numCols = inFile.nextInt();
        minVal = inFile.nextInt();
        maxval = inFile.nextInt();
        this.thrVal = thrVal;

        imgAry = new int[numRows][numCols];
        outAry = new int[numRows][numCols];
        thrAry = new int[numRows][numCols];
        loadImage(inFile);
    }

    void loadImage(Scanner inFile){
        for (int i = 0; i < imgAry.length; i++) {
            for (int j = 0; j < imgAry[0].length; j++) {
                imgAry[i][j] = inFile.nextInt();
            }
        }
        outAry = imgAry.clone();
    }

    int convolution(int i, int j, int[][] mask){
        int sum = 0, weight = 0;
        int offsetR = i - 2, offsetC = j - 2;
        for(int r = 0; r < 5; r++){
            for (int c = 0; c < 5; c++) {
                sum += imgAry[r + offsetR][c + offsetC] * mask[r][c];
                weight += mask[r][c];
            }
        }

        return sum / weight;
    }

    void cornerPreserveAvg(){
        int minDiffAvg, minDiff, diff, convolution;
        for (int i = 2; i < numRows - 2; i++) {
            for (int j = 2; j < numCols - 2; j++) {
                minDiffAvg = imgAry[i][j];
                minDiff = 9999;
                for (var mask: masks) {
                    convolution = convolution(i, j, mask);
                    diff = Math.abs(imgAry[i][j] - convolution);
                    if(diff < minDiff){
                        minDiff = diff;
                        minDiffAvg = convolution;
                    }
                }
                outAry[i][j] = minDiffAvg;
            }
        }
    }

    void threshold(){
        for (int i = 0; i < outAry.length; i++) {
            for (int j = 0; j < outAry[0].length; j++) {
                if(outAry[i][j] >= thrVal)
                    thrAry[i][j] = 1;
                else
                    thrAry[i][j] = 0;
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

    int findMin(int[][] array){
        int min = array[0][0];
        for (int[] ints : array) {
            for (int anInt : ints) {
                if (anInt < min)
                    min = anInt;
            }
        }
        return min;
    }

    void reformatPrettyPrint(FileWriter outFile, int[][] image) throws IOException {
        int     rows = image.length,
                cols = image[0].length,
                min = findMin(image),
                max = findMax(image);

        outFile.write(rows + " " + cols + " " + min + " " + max + "\n");

        int curWidth, pixelWidth = Integer.toString(max).length();

        for(int r = 0; r < numRows; r++){
            for(int c = 0; c < numCols; c++){
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

public class LiuJ_Q2_Java{
    public static void main(String[] args) throws IOException {
        Scanner inFile = new Scanner(new FileReader(args[0]));
        int thrVal = Integer.parseInt(args[1]);
        FileWriter outFile = new FileWriter(args[2]);

        ImageEnhancement imageEnhancement = new ImageEnhancement(inFile, thrVal);
        imageEnhancement.reformatPrettyPrint(outFile, imageEnhancement.imgAry);

        imageEnhancement.cornerPreserveAvg();
        outFile.write('\n');
        imageEnhancement.reformatPrettyPrint(outFile, imageEnhancement.outAry);

        imageEnhancement.threshold();
        outFile.write('\n');
        imageEnhancement.reformatPrettyPrint(outFile, imageEnhancement.thrAry);

        inFile.close();
        outFile.close();
    }
}