//
//  main.cpp
//  CS381_Image_Processing_Project5_Distance_Transform
//
//  Created by Jingshi Liu on Oct/28/2023.
//


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

    static int findMin(int* array, int length){
        int min = array[0];
        for(int i = 1; i < length; i++){
            if(array[i] < min){
                min = array[i];
            }
        }
        return min;
    }

    static int findMinSkipZero(int* array, int length){
        int min = array[0];
        for(int i = 1; i < length; i++){
            if(array[i] < min && array[i] != 0){
                min = array[i];
            }
        }
        return min;
    }

    static int findMax(int* array, int length){
        int max = array[0];
        for(int i = 1; i < length; i++){
            if(array[i] > max){
                max = array[i];
            }
        }
        return max;
    }

    static int findMax(int** array, int rows, int cols){
        int max = array[0][0];
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                if(array[i][j] > max){
                    max = array[i][j];
                }
            }
        }
        return max;
    }

    static void printArray(int** array, int row, int col){
        for(int i = 0; i < row; i++){
            for(int j = 0; j < col; j++){
                cout<<array[i][j]<<" ";
            }
            cout<<endl;
        }
        cout<<endl<<endl;
    }

    static void printArray(int* array, int length){
        for(int i = 0; i < length; i++){
            cout<< array[i] << " ";
        }
        cout<<endl<<endl;
    }

    static void prettyPrint(int** array, int rows, int cols){
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                if(array[i][j] == 0){
                    cout<< ". ";
                }else{
                    cout <<"1 ";
                }
            }
            cout << "\n";
        }
    }

    static void loadFileToArray(ifstream& inFile, int** array, int rows, int cols){
        int pixelVal;
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                inFile >> pixelVal;
                array[i][j] = pixelVal;
            }
        }
    }

    static void loadFileToArray(ifstream& inFile, int* array, int length){
        int pixelVal;
        for(int i = 0; i < length; i++){
            inFile >> pixelVal;
            array[i] = pixelVal;
        }
    }

    static int** copyArray(int** array, int rows, int cols){
        int** output = getArray(rows, cols);
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                output[i][j] = array[i][j];
            }
        }
        return output;
    }
}

class ImageCompression{
public:
    int numRows,
        numCols,
        minVal,
        maxVal,
        newMinVal,
        newMaxVal;
    int** ZFAry;
    int** skeletonAry;

    ImageCompression(ifstream& inFile){
        inFile >> numRows >> numCols >> minVal >> maxVal;
        ZFAry = Util::getArray(numRows + 2 , numCols + 2);
        skeletonAry = Util::getArray(numRows + 2, numCols + 2);

        loadImage(inFile);
    }

    void setZero(int** array){
        for(int i = 0; i < numRows + 2; i++){
            for(int j = 0; j < numCols + 2; j++){
                array[i][j] = 0;
            }
        }
    }

    void loadImage(ifstream& inFile){
        int pixelVal;
        for(int i = 1; i < numRows + 1; i++){
            for(int j = 1; j < numCols + 1; j++){
                inFile >> pixelVal;
                ZFAry[i][j] = pixelVal;
            }
        }
    }

    void imageReformat(int** image, ofstream& outFile){
        outFile << numRows << " " << numCols << " " << newMinVal << " " << newMaxVal << '\n';
        string str;
        int curWidth,
            pixelWidth = to_string(newMaxVal).length();

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

    void loadSkeleton(ifstream& inFile){
        int pixelVal;
        for(int i = 1; i < numRows + 1; i++){
            for(int j = 1; j < numCols + 1; j++){
                inFile >> pixelVal;
                skeletonAry[i][j] = pixelVal;
            }
        }
    }

    void distance8(ofstream& outFile, ofstream& debugFile){
        debugFile << "Entering distance8() method" << endl;
        distance8Pass1(debugFile);

        outFile<<"Distance Transform 8 Pass 1"<<endl;
        imageReformat(ZFAry, outFile);
        distance8Pass2(debugFile);

        outFile<<"\n\nDistance Transform 8 Pass 2"<<endl;
        imageReformat(ZFAry, outFile);
        debugFile << "Exiting distance8() method" << endl;
    }

    void distance8Pass1(ofstream& debugFile){
        debugFile << "Entering distancePass1() method" << endl;
        for(int i = 1; i < numRows + 1; i++){
            for(int j = 1; j < numCols + 1; j++){
                if (ZFAry[i][j] == 0) continue;
                int neighbors[4] = {ZFAry[i - 1][j - 1],
                                  ZFAry[i - 1][j],
                                  ZFAry[i - 1][j + 1],
                                  ZFAry[i][j - 1]};
                ZFAry[i][j] = 1 + Util::findMinSkipZero(neighbors, 4);
                newMaxVal = Util::max(ZFAry[i][j], newMaxVal);
                newMinVal = Util::min(ZFAry[i][j], newMinVal);
            }
        }
        debugFile << "Exiting distancePass1() method" << endl;
    }

    void distance8Pass2(ofstream& debugFile){
        debugFile << "Entering distancePass2() method" << endl;
        newMaxVal = 0;
        for(int i = numRows; i > 0; i--){
            for(int j = numCols; j > 0; j--){
                if (ZFAry[i][j] == 0) continue;
                int neighborsAndSelf[5] = {ZFAry[i][j + 1] + 1,
                                    ZFAry[i + 1][j - 1] + 1,
                                    ZFAry[i + 1][j] + 1,
                                    ZFAry[i + 1][j + 1] + 1,
                                    ZFAry[i][j]};
                ZFAry[i][j] = Util::findMinSkipZero(neighborsAndSelf, 5);
                newMaxVal = Util::max(ZFAry[i][j], newMaxVal);
                newMinVal = Util::min(ZFAry[i][j], newMinVal);
            }
        }
        debugFile << "Exiting distancePass2() method" << endl;
    }

    void imageCompression(ofstream& skeletonFile, ofstream& outFile, ofstream& debugFile){
        debugFile << "Entering imageCompression() method" << endl;
        computeLocalMaxima(debugFile);
        outFile<< "\n\nLocal Maxima Skeleton of the image" << endl;
        imageReformat(skeletonAry, outFile);
        extractSkeleton(skeletonFile, debugFile);
        debugFile << "Exiting imageCompression() method" << endl;
    }

    void computeLocalMaxima(ofstream& debugFile) {
        debugFile << "Entering computeLocalMaxima() method" << endl;
        for (int i = 1; i < numRows + 1; i++) {
            for (int j = 1; j < numCols + 1; j++) {
                if (ZFAry[i][j] == 0) continue;
                int neighbors[8] = {ZFAry[i - 1][j - 1],
                                    ZFAry[i - 1][j],
                                    ZFAry[i - 1][j + 1],
                                    ZFAry[i][j - 1],
                                    ZFAry[i][j + 1],
                                    ZFAry[i + 1][j - 1],
                                    ZFAry[i + 1][j],
                                    ZFAry[i + 1][j + 1]};
                int max = Util::findMax(neighbors, 8);
                if (ZFAry[i][j] >= max) {
                    skeletonAry[i][j] = ZFAry[i][j];
                }
            }
            debugFile << "Exiting computeLocalMaxima() method" << endl;
        }
    }

    void extractSkeleton(ofstream& skeletonFile, ofstream& debugFile){
        debugFile << "Entering extractSkeleton() method" << endl;
        for (int i = 1; i < numRows + 1; i++) {
            for (int j = 1; j < numCols + 1; j++) {
                if(skeletonAry[i][j] == 0) continue;
                skeletonFile<< i << " " << j << " " << skeletonAry[i][j] << endl;
            }
        }
        debugFile << "Exiting extractSkeleton() method" << endl;
    }

    void imageDecompression(ofstream &outFile, ofstream &debugFile){
        debugFile << "Entering imageDecompression() method" << endl;
        expansionPass1(debugFile);
        outFile<<"\n\nExpansion Pass 1"<<endl;
        imageReformat(ZFAry, outFile);

        expansionPass2(debugFile);
        outFile<<"\n\nExpansion Pass 2"<<endl;
        imageReformat(ZFAry, outFile);
        debugFile << "Exiting imageDecompression() method" << endl;
    }

    void expansionPass1(ofstream& debugFile){
        debugFile << "Entering expansionPass1() method" << endl;
        for(int i = 1; i < numRows + 1; i++){
            for(int j = 1; j < numCols + 1; j++){
                if(ZFAry[i][j] > 0) continue;
                int neighbors[8] = {ZFAry[i - 1][j - 1],
                                    ZFAry[i - 1][j],
                                    ZFAry[i - 1][j + 1],
                                    ZFAry[i][j - 1],
                                    ZFAry[i][j + 1],
                                    ZFAry[i + 1][j - 1],
                                    ZFAry[i + 1][j],
                                    ZFAry[i + 1][j + 1]};
                ZFAry[i][j] = Util::max(ZFAry[i][j], Util::findMax(neighbors, 8) - 1);
            }
        }
        debugFile << "Exiting expansionPass1() method" << endl;
    }

    void expansionPass2(ofstream& debugFile){
        debugFile << "Entering expansionPass2() method" << endl;
        for(int i = numRows; i > 0; i--){
            for(int j = numCols; j > 0; j--){
                int neighbors[8] = {ZFAry[i - 1][j - 1],
                                    ZFAry[i - 1][j],
                                    ZFAry[i - 1][j + 1],
                                    ZFAry[i][j - 1],
                                    ZFAry[i][j + 1],
                                    ZFAry[i + 1][j - 1],
                                    ZFAry[i + 1][j],
                                    ZFAry[i + 1][j + 1]};
                ZFAry[i][j] = Util::max(ZFAry[i][j], Util::findMax(neighbors, 8) - 1);
            }
        }
        debugFile << "Exiting expansionPass2() method" << endl;
    }
};






// --------------------------------------- Main Function ---------------------------------------------------------//

int main(int argc, const char* argv[]){
    ifstream inFile(argv[1]);
    ofstream outFile(argv[2]),
             debugFile(argv[3]),
             skeletonFile((string)argv[1] + "_skeleton.txt"),
             decompressedFile((string)argv[1] + "_decompressed.txt");

    // compress image and output to skeletonFile
    ImageCompression* imageCompression = new ImageCompression(inFile);
    imageCompression->distance8(outFile, debugFile);
    imageCompression->imageCompression(skeletonFile, outFile, debugFile);
    skeletonFile.close();


    // load skeleton file and decompress
    ifstream skeletonFile2((string)argv[1] + "_skeleton.txt");
    imageCompression->setZero(imageCompression->ZFAry);
    imageCompression->loadSkeleton(skeletonFile2);

    imageCompression->imageDecompression(outFile, debugFile);


    inFile.close();
    outFile.close();
    debugFile.close();
    skeletonFile2.close();
    decompressedFile.close();
    return 0;
}