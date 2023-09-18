//
//  main.cpp
//  CS381_Image_Processing_Project2_Averaing_and_Gaussian_Filter
//
//  Created by Jingshi Liu on 9/16/2023.
//


#include <iostream>
#include <fstream>

using namespace std;

int** getArray(int rows, int cols){
    int** array = new int*[rows];
    for(int i = 0; i < rows; i++){
        array[i] = new int[cols];
        for(int j = 0; j < cols; j++){
            array[i][j] = 0;
        }
    }
    return array;
}

int* getArray(int length){
    int* array = new int[length];
    for(int i = 0; i < length; i++){
        array[i] = 0;
    }
    return array;
}


class Enhancement{
public:
    int numRows, numCols, minVal, maxVal, maskRows, maskCols, maskMin, maskMax, maskWeight;
    int** mirroredFramedArray;
    int** averagingArray;
    int** gaussianArray;

    int* neighborArray;
    int* maskArray;
    int* histogramAveragingArray;
    int* histogramGaussianArray;

    Enhancement(ifstream& inFile, ifstream& maskFile){
        inFile >> numRows >> numCols >> minVal >> maxVal;
        maskFile >> maskRows >> maskCols >> maskMin >> maskMax;
        mirroredFramedArray = getArray(numRows + 4, numCols + 4);
        averagingArray = getArray(numRows + 4, numCols + 4);
        gaussianArray = getArray(numRows + 4, numCols + 4);

        histogramAveragingArray = getArray(maxVal + 1);
        histogramGaussianArray = getArray(maxVal + 1);
    }

    void loadImage(){

    }

    void mirroFraming(){

    }

    int loadMaskArray(){
        return 0;
    }

    void loadNeightborArray(){
        
    }

    void computeAverage5x5(){

    }

    int average5x5(){
        return 0;
    }

    void computeGaussian5x5(){

    }

    int convolution(){

    }

    void computeHistogram(){

    }

    void imageReformat(){

    }

    void printHistogram(){

    }
};


int main(int argc, const char* argv[]){
    ifstream inFile, maskFile;
    ofstream outFile, debugFile;

    inFile.open(argv[1]);
    maskFile.open(argv[2]);
    string choice = argv[3];

    Enhancement* enhancement = new Enhancement(inFile, maskFile);

}