//
// Created by Jingshi Liu on 9/24/23.
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

class Morphology{
public:
    int numImageRows,
        numImageCols,
        imageMin,
        imageMax,
        numStructRows,
        numStructCols,
        structMin,
        structMax,
        rowOrigin,
        colOrigin,
        rowFrameSize, // numStructRows / 2
        colFrameSize, // numStructCols / 2
        extractRows, // rowFrameSize * 2
        extractCols, // colFrameSize * 2
        rowSize, // numImageRows + extraRows
        colSize; // numImageCols + extraCols
    int** zeroFramedArray;
    int** morphArray;
    int** tempArray;
    int** structArray;

    Morphology(ifstream& imageFile, ifstream& structFile){
        imageFile >> numImageRows >> numImageCols >> imageMin >> imageMax;
        structFile >> numStructRows >> numStructCols >> structMin >> structMax >> rowOrigin >> colOrigin;

        rowFrameSize = numStructRows / 2;
        colFrameSize = numStructCols / 2;
        extractRows = rowFrameSize * 2;
        extractCols = colFrameSize * 2;
        rowSize = numImageRows + extractRows;
        colSize = numImageCols + extractCols;

        zeroFramedArray = Util::getArray(rowSize, colSize);
        structArray = Util::getArray(numStructRows, numStructCols);
        morphArray = Util::getArray(rowSize, colSize);
        tempArray = Util::getArray(rowSize, colSize);

        loadImage(imageFile);
        loadStruct(structFile);
    }

    void zero2DArray(int** array, int rows, int cols){
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                array[i][j] = 0;
            }
        }
    }

    // load image file to zeroFramedArray
    void loadImage(ifstream& imageFile){
        int pixelVal;
        for(int i = rowOrigin; i < rowSize - rowFrameSize; i++){
            for(int j = colOrigin; j < colSize - colFrameSize; j++){
                imageFile >> pixelVal;
                zeroFramedArray[i][j] = pixelVal;
            }
        }
    }

    void loadStruct(ifstream& structFile){
        int pixelVal;
        for(int i = 0; i < numStructRows; i++){
            for(int j = 0; j < numStructCols; j++){
                structFile >> pixelVal;
                structArray[i][j] = pixelVal;
            }
        }
    }

    void computeDilation(int** inputImage, int** outputImage){
        for(int i = rowOrigin; i < rowSize - rowFrameSize; i++){
            for(int j = colOrigin; j < colSize - colFrameSize; j++){
                if(inputImage[i][j] > 0){
                    onePixelDilation(i, j, inputImage, outputImage);
                }
            }
        }
    }

    void computeErosion(int** inputImage, int** outputImage){
        for(int i = rowOrigin; i < rowSize - rowFrameSize; i++){
            for(int j = colOrigin; j < colSize - colFrameSize; j++){
                if(inputImage[i][j] > 0){
                    onePixelErosion(i, j, inputImage, outputImage);
                }
            }
        }
    }

    void computeOpening(int** inputImage, int** outputImage, int** tempImage){
        computeErosion(inputImage, tempImage);
        computeDilation(tempImage, morphArray);
    }

    int** computeClosing(int** inputImage, int** outputImage, int** tempImage){
        computeDilation(inputImage, tempImage);
        computeErosion(tempImage, morphArray);
    }

    void onePixelDilation(int i, int j, int** inputImage, int** outputImage){
        int iOffset = i - rowOrigin,
            jOffset = j - colOrigin;
        for(int rIndex = 0; rIndex < numStructRows; rIndex++){
            for(int cIndex = 0; cIndex < numStructCols; cIndex++){
                if(structArray[rIndex][cIndex] > 0){
                    outputImage[iOffset + rIndex][jOffset + cIndex] = 1;
                }
            }
        }
    }

    void onePixelErosion(int i , int j, int** inputImage, int** outputImage){
        int iOffset = i - rowOrigin,
            jOffset = j - colOrigin;
        bool fitStructElement = true;
        for(int rIndex = 0; rIndex < numStructRows; rIndex++){
            for(int cIndex = 0; cIndex < numStructCols; cIndex++){
                if(structArray[rIndex][cIndex] > 0 && inputImage[iOffset + rIndex][jOffset + cIndex] <= 0){
                        fitStructElement = false;
                }
            }
        }
        if(fitStructElement){
            outputImage[i][j] = 1;
        }else{
            outputImage[i][j] = 0;
        }
    }

    void outputImageToFile(int** imageArray, ofstream& outFile){

    }

    void prettyPrint(int** imageArray, int rows, int cols, ofstream& outFile){
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                if(imageArray[i][j] == 0){
                    outFile << ". ";
                }else{
                    outFile <<"1 ";
                }
            }
            outFile << "\n";
        }
    }

    void objectExtraction(){
        zero2DArray(tempArray, rowSize, colSize);
        computeOpening(zeroFramedArray, morphArray, tempArray);
    }

    void fillHoles(){
        zero2DArray(tempArray, rowSize, colSize);
        computeClosing(Util::copyArray(morphArray, rowSize, colSize), morphArray, tempArray);
    }
};

int main(int argc, const char* argv[]){
    ifstream imageFile(argv[1]),
             structFile(argv[2]),
             taskImageFile(argv[3]),
             structImageFile(argv[4]);
    ofstream outFile1(argv[5]),
             outFile2(argv[6]);

    Morphology* morphology = new Morphology(imageFile, structFile);

    outFile1<< "Data 1 Image \n";
    morphology->prettyPrint(morphology->zeroFramedArray, morphology->rowSize, morphology->colSize, outFile1);
    outFile1<< "\n\nStructure Element\n";
    morphology->prettyPrint(morphology->structArray, morphology->numStructRows, morphology->numStructCols, outFile1);

    // Step 6
    morphology->computeDilation(morphology->zeroFramedArray, morphology->morphArray);
    outFile1<< "\n\nData1 Image after Dilation\n";
    morphology->prettyPrint(morphology->morphArray, morphology->rowSize, morphology->colSize,outFile1);

    // Step 7
    morphology->zero2DArray(morphology->morphArray, morphology->rowSize, morphology->colSize);
    morphology->computeErosion(morphology->zeroFramedArray, morphology->morphArray);
    outFile1<< "\n\nData1 Image Erosion\n";
    morphology->prettyPrint(morphology->morphArray, morphology->rowSize, morphology->colSize,outFile1);

    // Step 8
    morphology->zero2DArray(morphology->morphArray, morphology->rowSize, morphology->colSize);
    morphology->computeOpening(morphology->zeroFramedArray, morphology->morphArray, morphology->tempArray);
    outFile1<< "\n\nData1 Image Opening\n";
    morphology->prettyPrint(morphology->morphArray, morphology->rowSize, morphology->colSize,outFile1);

    // Step 9
    morphology->zero2DArray(morphology->morphArray, morphology->rowSize, morphology->colSize);
    morphology->zero2DArray(morphology->tempArray, morphology->rowSize, morphology->colSize);
    morphology->computeClosing(morphology->zeroFramedArray, morphology->morphArray, morphology->tempArray);
    outFile1<< "\n\nData1 Image Closing\n";
    morphology->prettyPrint(morphology->morphArray, morphology->rowSize, morphology->colSize,outFile1);

    // Step 10 - Task 1: extract large blobs and fill holes in the blobs
    Morphology* taskMorphology = new Morphology(taskImageFile, structImageFile);
    outFile2<< "Image 1\n";
    taskMorphology->prettyPrint(taskMorphology->zeroFramedArray, taskMorphology->rowSize, taskMorphology->colSize, outFile2);

    // Structuring Element
    outFile2<< "\n\n\nStructure Element for objectExtraction()\n";
    taskMorphology->prettyPrint(taskMorphology->structArray, taskMorphology->numStructRows, taskMorphology->numStructCols, outFile2);

    // objectExtraction
    taskMorphology->objectExtraction();
    outFile2<< "\n\nObject Extraction\nOperation: Opening\nImage 1 after objectExtraction().\n";
    taskMorphology->prettyPrint(taskMorphology->morphArray, taskMorphology->rowSize, taskMorphology->colSize ,outFile2);

    // fillHoles
    taskMorphology->fillHoles();
    outFile2<< "\n\nFill Holes\nOperation: Closing\nImage after objectExtraction() and fillHoles()\n";
    taskMorphology->prettyPrint(taskMorphology->morphArray, taskMorphology->rowSize, taskMorphology->colSize ,outFile2);

}