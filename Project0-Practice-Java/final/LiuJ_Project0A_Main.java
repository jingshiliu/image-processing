import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;


public class Main {
    public static void processing(Scanner inFile, FileWriter outFile1, FileWriter outFile2, int thrVal, int numRows, int numCols) throws IOException {
        int pixelVal;

        for(int i = 0; i < numRows; i ++){
            for (int j = 0; j < numCols; j ++) {
                pixelVal = inFile.nextInt();
                if(pixelVal >= thrVal){
                    outFile1.write("1 ");
                    outFile2.write(pixelVal + " ");
                }else{
                    outFile1.write("0 ");
                    outFile2.write("0 ");
                }
            }
            outFile1.write('\n');
            outFile2.write('\n');
        }
    }

    public static void main(String[] args) throws IOException {
        Scanner inFile = new Scanner(new FileReader(args[0]));
        FileWriter outFile1 = new FileWriter(args[1]),
                   outFile2 = new FileWriter(args[2]);

        int numRows = inFile.nextInt();
        int numCols = inFile.nextInt();
        int minVal = inFile.nextInt();
        int maxVal = inFile.nextInt();

        System.out.print("Enter a Threshold Value: ");
        int thrVal = (new Scanner(System.in)).nextInt();

        outFile1.write(numRows + " " + numCols + " " + 0 + " " + 1 + "\n");
        outFile2.write(numRows + " " + numCols + " " + 0 + " " + maxVal + "\n");

        processing(inFile, outFile1, outFile2, thrVal, numRows, numCols);

        outFile1.close();
        outFile2.close();
    }
}