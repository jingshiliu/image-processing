//
//  main.cpp
//  CS381_Image_Processing_Project2_Averaing_and_Gaussian_Filter
//
//  Created by Jingshi Liu on 9/16/2023.
//


#include <iostream>
#include <fstream>

using namespace std;

// ---------------------------------------Util Functions Declaration -------------------------------------------------//
// Util functions are declared at the top the file to make sure all other functions can access the Util Functions

int** getArray(int rows, int cols);

int* getArray(int length);


// --------------------------------------- Class Enhancement ---------------------------------------------------------//


class Enhancement{
public:
    int numRows, numCols, minVal, maxVal, maskRows, maskCols, maskMin, maskMax, maskWeight;
    int** mirroredFramedArray;
    int** averagingArray;
    int** gaussArray;

    int* neighborArray;
    int* maskArray;
    int* histogramAveragingArray;
    int* histogramGaussianArray;

    Enhancement(ifstream& inFile, ifstream& maskFile){
        inFile >> numRows >> numCols >> minVal >> maxVal;
        maskFile >> maskRows >> maskCols >> maskMin >> maskMax;
        mirroredFramedArray = getArray(numRows + 4, numCols + 4);
        averagingArray = getArray(numRows + 4, numCols + 4);
        gaussArray = getArray(numRows + 4, numCols + 4);

        histogramAveragingArray = getArray(maxVal + 1);
        histogramGaussianArray = getArray(maxVal + 1);

        loadImage(inFile);
        mirroFraming();
    }

    void loadImage(ifstream& inFile){
        int num;
        for(int i = 2; i < numRows + 2; i++){
            for(int j = 2; j < numCols + 2; j++){
                inFile >> num;
                this->mirroredFramedArray[i][j] = num;
            }
        }
    }

    void mirroFraming(){
        for(int i = 0; i < numRows + 4; i++){
            // mirror row 2
            mirroredFramedArray[i][0] = mirroredFramedArray[i][3];
            // mirror row 1
            mirroredFramedArray[i][1] = mirroredFramedArray[i][2];
        }
        for(int i = 0; i < numCols + 4; i++){
            // mirror col 2
            mirroredFramedArray[0][i] = mirroredFramedArray[3][i];
            //mirror col 1
            mirroredFramedArray[1][i] = mirroredFramedArray[2][i];
        }
    }

    int loadMaskArray(ifstream& maskFile){
        // maskFile >> maskRows >> maskCols >> maskMin >> maskMax;
        return 0;
    }

    void loadNeightborArray(int i, int j){
        int index = 0;
        for(int r = i - 2; r <= i + 2; r++){
            for(int c = j - 2; c <= i + 2; c++){
                neighborArray[index++] = mirroredFramedArray[r][c];
            }
        }
    }

    void computeAverage5x5(ofstream& debugFile){
        debugFile << "Entering computeAverage5x5 method\n";
        for(int i = 2; i < numRows + 2; i++){
            for(int j = 2; j < numCols + 2; j++){
                averagingArray[i][j] = average5x5(i, j);
            }
        }
        debugFile << "Leaving computeAverage5x5 method\n";
    }

    int average5x5(int i, int j){
        loadNeightborArray(i, j);
        int sum = 0;

        for(int i = 0; i < 25; i++){
            sum += neighborArray[i];
        }
        return sum / 25;
    }

    void computeGaussian5x5(ofstream& debugFile){

    }

    int convolution(){

    }

    void computeHistogram(int** imageArray, int* histogramArray, ofstream& debugFile){
        debugFile << "Entering computeHistogram method\n";
        for(int i = 2; i < numRows + 2; i++){
            for(int j = 2; j < numCols + 2; j++){
                histogramArray[imageArray[i][j]]++;
            }
        }
        debugFile << "Leaving computeHistogram method\n";
    }

    void imageReformat(int** imageArray, ofstream& outFile){
        outFile << numRows << numCols << minVal << maxVal << '\n';
        string str;
        int curWidth, pixelWidth = to_string(maxVal).length();

        for(int r = 2; r < numRows + 2; r++){
            for(int c = 2; c < numCols + 2; c++){
                outFile << imageArray[r][c];
                str = to_string(imageArray[r][c]);
                curWidth = str.length();
                while(curWidth < pixelWidth){
                    outFile<<' ';
                    curWidth++;
                }
            }
            outFile << '\n';
        }
    }

    void printHistogram(int* histogramArray, ofstream& outFile, ofstream& debugFile){
        debugFile << "Entering printHistogram method\n";
        outFile << numCols << numCols << minVal << maxVal << '\n';
        for(int i = 0; i <= maxVal; i++){
            outFile << i << " " << histogramArray[i] << '\n';
        }
        debugFile << "Leaving printHistogram method\n";
    }

    void outputImage(int** imageArray, ofstream& outFile){
        outFile << numRows << " " << numCols << " " << minVal << " " << maxVal <<'\n';
        for(int i = 2; i < numRows + 2; i++){
            for(int j = 2; j < numCols + 2; j++){
                outFile << imageArray[i][j] << " ";
            }
            outFile << "\n";
        }
    }
};

// ---------------------------------- Until Functions Implementation -----------------------------------------------------//

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

void useAverageFilter(const char* argv[], Enhancement* enhancement, ofstream& outFile, ofstream& debugFile){
    enhancement->computeAverage5x5(debugFile);
    enhancement->computeHistogram(enhancement->averagingArray, enhancement->histogramAveragingArray ,debugFile);

    ofstream averageFile("./" + (string)argv[1] + "_Avg5x5.txt");
    enhancement->imageReformat(enhancement->averagingArray, outFile);
    enhancement->outputImage(enhancement->averagingArray, averageFile);

    ofstream histAvgFile("./" + (string)argv[1] + "_Avg5x5_hist.txt");
    enhancement->printHistogram(enhancement->histogramAveragingArray, histAvgFile, debugFile);

    averageFile.close();
    histAvgFile.close();
}

void useGaussianFilter(const char* argv[], Enhancement* enhancement, ifstream& maskFile ,ofstream& outFile, ofstream& debugFile){
    enhancement->loadMaskArray(maskFile);
    enhancement->computeGaussian5x5(debugFile);
    enhancement->computeHistogram(enhancement->gaussArray, enhancement->histogramGaussianArray, debugFile);

    ofstream gaussFile("./" + (string)argv[1] + "_Gauss5x5.txt");
    enhancement->imageReformat(enhancement->gaussArray, outFile);
    enhancement->outputImage(enhancement->gaussArray, gaussFile);

    ofstream histGaussFile("./" + (string)argv[1] + "_Gauss5x5_hist.txt");
    enhancement->printHistogram(enhancement->histogramGaussianArray, histGaussFile, debugFile);

    gaussFile.close();
    histGaussFile.close();
}


// --------------------------------------- Main Function ---------------------------------------------------------//

int main(int argc, const char* argv[]){
    ifstream inFile, maskFile;
    ofstream outFile("./output.txt"), debugFile("./debugFile.txt");

    inFile.open(argv[1]);
    maskFile.open(argv[2]);
    string choice = argv[3];

    Enhancement* enhancement = new Enhancement(inFile, maskFile);
    enhancement->imageReformat(enhancement->mirroredFramedArray, outFile);
    if(choice == "1"){
        useAverageFilter(argv, enhancement, outFile, debugFile);
    }else if(choice == "2"){
        useGaussianFilter(argv, enhancement, maskFile, outFile, debugFile);
    }else{
        cout<< "Unknown choice argument entered, please enter either '1' or '2'\n";
    }

    inFile.close();
    maskFile.close();
    outFile.close();
    debugFile.close();
}