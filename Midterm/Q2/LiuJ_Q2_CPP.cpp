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
}

class ImageProcessing{
public:
    int numRows,
        numCols,
        minVal,
        maxVal,
        newMin,
        newMax;
    int** ZFAry;

    ImageProcessing(ifstream& inFile){
        inFile >> numRows >> numCols >> minVal >> maxVal;

        // getArray initialize array with each attr as 0
        ZFAry = Util::getArray(numRows + 2, numCols + 2);
        newMin = 0;
        newMax = 0;

        setZero(ZFAry);
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
        int num;
        for(int i = 1; i < numRows + 1; i++){
            for(int j = 1; j < numCols + 1; j++){
                inFile >> num;
                this->ZFAry[i][j] = num;
            }
        }
    }

    void fistPass8Distance(){
        newMin = 999;
        newMax = 0;
        for(int i = 1; i < numRows + 1; i++){
            for(int j = 1; j < numCols + 1; j++){
                if(ZFAry[i][j] > 0){
                    ZFAry[i][j] = 1 + myMin4(ZFAry[i-1][j-1], ZFAry[i-1][j], ZFAry[i-1][j+1], ZFAry[i][j-1]);
                }
                if(newMin  > ZFAry[i][j]){
                    newMin = ZFAry[i][j];
                }
                if(newMax < ZFAry[i][j]){
                    newMax = ZFAry[i][j];
                }
            }
        }
    }

    int myMin4(int a, int b, int c, int d){
        if(a <= b && a <= c && a <= d)
            return a;
        if(b <= a && b <= c && b <= d)
            return b;
        if(c <= a && c <= b && c <= d)
            return c;
        return d;
    }

    void imageReformat(int** imageArray, ofstream& outFile){
        outFile << numRows << " " << numCols << " " << minVal << " " << maxVal << '\n';
        string str;
        int curWidth, pixelWidth = to_string(maxVal).length();

        for(int r = 1; r < numRows + 1; r++){
            for(int c = 1; c < numCols + 1; c++){
                outFile << imageArray[r][c];
                str = to_string(imageArray[r][c]);
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
};

int main(int argc, const char* argv[]) {
    ifstream inFile(argv[1]);
    ofstream outFile(argv[2]);

    ImageProcessing* ip = new ImageProcessing(inFile);
    outFile << "Input Image\n";
    ip->imageReformat(ip->ZFAry, outFile);

    ip->fistPass8Distance();
    outFile << "\n\n1st pass distance transform image\n";
    ip->imageReformat(ip->ZFAry, outFile);

    inFile.close();
    outFile.close();
    return 0;
}
