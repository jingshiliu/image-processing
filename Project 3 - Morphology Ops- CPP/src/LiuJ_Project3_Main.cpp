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

    static int* getArray(int length){
        int* array = new int[length];
        for(int i = 0; i < length; i++){
            array[i] = 0;
        }
        return array;
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
        rowsSize, // numImageRows + extraRows
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
        rowsSize = numImageRows + extractRows;
        colSize = numImageCols + extractCols;

        zeroFramedArray = Util::getArray(rowsSize, colSize);
        structArray = Util::getArray(rowsSize, colSize);
        morphArray = Util::getArray(rowsSize, colSize);
        tempArray = Util::getArray(rowsSize, colSize);

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
        for(int i = rowOrigin; i < rowsSize; i++){
            for(int j = colOrigin; j < colSize; j++){
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

    int** computeDilation(int** inputImage){

    }

    int** computeErosion(int** inputImage){

    }

    int** computeOpening(int** inputImage){

    }

    int** computeClosing(int** inputImage){

    }

    void onePixelDilation(int i, int j, int** inputImage, int** outputImage){

    }

    void onePixelErosion(int i , int j, int** inputImage, int** outputImage){

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
};

int main(int argc, const char* argv[]){
    ifstream imageFile(argv[1]),
             structFile(argv[2]),
             task1ImageFile(argv[3]),
             task2ImageFile(argv[4]);
    ofstream outFile1(argv[5]),
             outFile2(argv[6]);

    Morphology* morphology = new Morphology(imageFile, structFile);

    outFile1<< "ZeroFramedArray\n";
    morphology->prettyPrint(morphology->zeroFramedArray, morphology->numImageRows, morphology->numImageCols, outFile1);
    outFile1<< "\n\nStructure Element\n";
    morphology->prettyPrint(morphology->structArray, morphology->numStructRows, morphology->numStructCols, outFile1);
}