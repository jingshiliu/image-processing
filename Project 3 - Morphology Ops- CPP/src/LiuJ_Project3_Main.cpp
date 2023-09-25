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
    int** zeroFramedArray,
          morphArray,
          tempArray,
          structArray;

    Morphology(){

    }

    void zero2DArray(int** array, int rows, int cols){

    }

    // load image file to zeroFramedArray
    void loadImage(ifstream& imageFile){

    }

    void loadStruct(ifstream& structFile){

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

    void prettyPrint(int** imageArray, ofstream& outFile){

    }
};

int main(int argc, const char* argv[]){
    ifstream imageFile(argv[1]),
             structFile(argv[2]),
             task1ImageFile(argv[3]),
             task2ImageFile(argv[4]);
    ofstream outFile1(argv[5]),
             outFile2(argv[6]);


}