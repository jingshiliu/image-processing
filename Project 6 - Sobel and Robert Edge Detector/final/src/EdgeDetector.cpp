#include <iostream>
#include <fstream>

using namespace std;

namespace Util{
    static int** getArray(int rows, int cols){
        int** array = new int*[rows];
        for(int i = 0; i < rows; i++){
            array[i] = new int[cols];
            for(int j = 0; j < cols; j++){
                array[i][j] = 0;
            }
        }
        return array;
    }

    static int* getArray(int length){
        int* array = new int[length];
        for(int i = 0; i < length; i++){
            array[i] = 0;
        }
        return array;
    }

    static int min(int a, int b){
        return a < b ? a : b;
    }

    static int max(int a, int b){
        return a > b ? a : b;
    }
}

class EdgeDetector{
public:
    int numRows,
        numCols,
        minVal,
        maxVal;
    int** mirrorFramedAry;
    int robertVerticalMask[2][2] = {{1, -1}, {1, -1}};
    int robertHorizontalMask[2][2] = {{1, 1}, {-1, -1}};
    int robertLeftDiagonalMask[2][2] = {{1, -1}, {-1, 1}};
    int robertRightDiagonalMask[2][2] = {{-1, 1}, {1, -1}};
    int** robertEdgeAry;

    int sobelVerticalMask[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int sobelHorizontalMask[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
    int sobelLeftDiagonalMask[3][3] = {{2, 1, 0}, {1, 0, -1}, {0, -1, -2}};
    int sobelRightDiagonalMask[3][3] = {{0, 1, 2}, {-1, 0, 1}, {-2, -1, 0}};
    int** sobelEdgeAry;

    int* histRobertAry;
    int* histSobelAry;

    EdgeDetector(ifstream& inFile){
        inFile >> numRows >> numCols >> minVal >> maxVal;
        mirrorFramedAry = Util::getArray(numRows + 2, numCols + 2);
        robertEdgeAry = Util::getArray(numRows + 2, numCols + 2);
        sobelEdgeAry = Util::getArray(numRows + 2, numCols + 2);
        histRobertAry = Util::getArray(maxVal + 1);
        histSobelAry = Util::getArray(maxVal + 1);
        loadImage(inFile);
        mirrorFraming();
    }

    void loadImage(ifstream& inFile){
        for(int i = 1; i < numRows + 1; i++){
            for(int j = 1; j < numCols + 1; j++){
                inFile >> mirrorFramedAry[i][j];
            }
        }
    }

    void mirrorFraming(){
        for(int i = 0; i < numRows + 2; i++){
            mirrorFramedAry[i][0] = mirrorFramedAry[i][1];
            mirrorFramedAry[i][numCols + 1] = mirrorFramedAry[i][numCols];
        }
        for(int j = 0; j < numCols + 2; j++){
            mirrorFramedAry[0][j] = mirrorFramedAry[1][j];
            mirrorFramedAry[numRows + 1][j] = mirrorFramedAry[numRows][j];
        }
    }

    void imageReformat(int** image, ofstream& outFile){
        outFile << numRows << " " << numCols << " " << minVal << " " << maxVal << '\n';
        string str;
        int curWidth,
            pixelWidth = to_string(maxVal).length();

        for(int r = 1; r < numRows + 1; r++){
            for(int c = 1; c < numCols + 1; c++){
                outFile << image[r][c];
                str = to_string(image[r][c]);
                curWidth = str.length();
                while(curWidth < pixelWidth){
                    outFile<<' ';
                    curWidth++;
                }
                outFile<<' ';
            }
            outFile << '\n';
        }
    }

    void robertEdgeDetector(ofstream& debugFile){
        debugFile << "Enter robertEdgeDetector\n";
        int newMax = 0, newMin = 99999;
        int tempV, tempH, tempLD, tempRD;
        for(int i = 1; i < numRows + 1; i++){
            for(int j = 1; j < numCols + 1; j++){
                tempH = abs(computeRobertConvolution(i, j, robertHorizontalMask));
                tempV = abs(computeRobertConvolution(i, j, robertVerticalMask));
                tempLD = abs(computeRobertConvolution(i, j, robertLeftDiagonalMask));
                tempRD = abs(computeRobertConvolution(i, j, robertRightDiagonalMask));
                robertEdgeAry[i][j] = tempH + tempV + tempLD + tempRD;
                newMax = Util::max(newMax, robertEdgeAry[i][j]);
                newMin = Util::min(newMin, robertEdgeAry[i][j]);
            }
        }
        maxVal = newMax;
        minVal = newMin;

        debugFile << "Exit robertEdgeDetector\n";
    }

    int computeRobertConvolution(int row, int col, int mask[2][2]){
        int sum = 0;
        sum += mirrorFramedAry[row][col] * mask[0][0];
        sum += mirrorFramedAry[row][col + 1] * mask[0][1];
        sum += mirrorFramedAry[row + 1][col] * mask[1][0];
        sum += mirrorFramedAry[row + 1][col + 1] * mask[1][1];
        return sum;
    }

    void sobelEdgeDetector(ofstream& debugFile){
        debugFile << "Enter sobelEdgeDetector\n";
        int newMax = 0, newMin = 99999;
        int tempV, tempH, tempLD, tempRD;
        for(int i = 1; i < numRows + 1; i++){
            for(int j = 1; j < numCols + 1; j++){
                tempH = abs(computeSobelConvolution(i, j, sobelHorizontalMask));
                tempV = abs(computeSobelConvolution(i, j, sobelVerticalMask));
                tempLD = abs(computeSobelConvolution(i, j, sobelLeftDiagonalMask));
                tempRD = abs(computeSobelConvolution(i, j, sobelRightDiagonalMask));
                sobelEdgeAry[i][j] = tempH + tempV + tempLD + tempRD;
                newMax = Util::max(newMax, sobelEdgeAry[i][j]);
                newMin = Util::min(newMin, sobelEdgeAry[i][j]);
            }
        }
        maxVal = newMax;
        minVal = newMin;
        debugFile << "Exit sobelEdgeDetector\n";
    }

    int computeSobelConvolution(int row, int col, int mask[3][3]){
        int sum = 0;
        int startX = row - 1, startY = col - 1;
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                sum += mirrorFramedAry[startX + i][startY + j] * mask[i][j];
            }
        }
        return sum;
    }

    void computeHistogram(int** image, int* hist, ofstream& debugFile){
        debugFile << "Enter computeHistogram\n";
        for(int i = 1; i < numRows + 1; i++){
            for(int j = 1; j < numCols + 1; j++){
                hist[image[i][j]]++;
            }
        }
        debugFile << "Exit computeHistogram\n";
    }

    void outputHistogram(int* hist, ofstream& outFile, ofstream& debugFile){
        debugFile << "Enter outputHistogram\n";
        outFile << numRows << " " << numCols << " " << minVal << " " << maxVal << '\n';
        for(int i = 0; i <= maxVal; i++){
            outFile << i << " " << hist[i] << '\n';
        }
        debugFile << "Exit outputHistogram\n";
    }

    void outputImage(int** image, ofstream& outFile){
        outFile << numRows << " " << numCols << " " << minVal << " " << maxVal << '\n';
        for(int i = 1; i < numRows + 1; i++){
            for(int j = 1; j < numCols + 1; j++){
                outFile << image[i][j] << " ";
            }
            outFile << '\n';
        }
    }
};


void useRobert(const char* argv[], EdgeDetector* edgeDetector, ofstream& outFile, ofstream& debugFile){
    edgeDetector->robertEdgeDetector(debugFile);
    edgeDetector->computeHistogram(edgeDetector->robertEdgeAry, edgeDetector->histRobertAry, debugFile);
    edgeDetector->imageReformat(edgeDetector->robertEdgeAry, outFile);

    ofstream robertEdgeFile((string)argv[1] + "_Robert_Edge.txt"),
             robertHistFile((string)argv[1] + "_Robert_Histogram.txt");
    edgeDetector->outputImage(edgeDetector->robertEdgeAry, robertEdgeFile);
    edgeDetector->outputHistogram(edgeDetector->histRobertAry, robertHistFile, debugFile);
}

void useSobel(const char* argv[], EdgeDetector* edgeDetector, ofstream& outFile, ofstream& debugFile){
    edgeDetector->sobelEdgeDetector(debugFile);
    edgeDetector->computeHistogram(edgeDetector->sobelEdgeAry, edgeDetector->histSobelAry, debugFile);
    edgeDetector->imageReformat(edgeDetector->sobelEdgeAry, outFile);

    ofstream sobelEdgeFile((string)argv[1] + "_Sobel_Edge.txt"),
             sobelHistFile((string)argv[1] + "_Sobel_Histogram.txt");
    edgeDetector->outputImage(edgeDetector->sobelEdgeAry, sobelEdgeFile);
    edgeDetector->outputHistogram(edgeDetector->histSobelAry, sobelHistFile, debugFile);
}


int main(int argc, const char* argv[]) {
    ifstream inFile(argv[1]);
    int choice = atoi(argv[2]);
    ofstream outFile(argv[3]), debugFile(argv[4]);

    EdgeDetector edgeDetector(inFile);
    edgeDetector.imageReformat(edgeDetector.mirrorFramedAry, outFile);

    if (choice == 1) {
        useRobert(argv, &edgeDetector, outFile, debugFile);
    } else if (choice == 2) {
        useSobel(argv, &edgeDetector, outFile, debugFile);
    }

    return 0;
}