//
//  main.cpp
//  CS381_Image_Processing_Project2_Averaing_and_Gaussian_Filter
//
//  Created by Jingshi Liu on 9/16/2023.
//


#include <iostream>
#include <fstream>

using namespace std;

class Enhancement{
    int numRows, numCols, minVal, maxVal, maskRows, maskCols, maskMin, maskMax, maskWeight;
    int** mirroredFramedArray, averagingArray, gaussianArray;
    int* neighborArray, maskArray, histogramAveragingArray, histogramGaussianArray;

    Enhancement(){

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

}