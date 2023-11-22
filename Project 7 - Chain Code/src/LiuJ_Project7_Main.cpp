#include <iostream>
#include <fstream>
#include <set>

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
        for(int i = 0; i < length; i++){
            if(array[i] < min){
                min = array[i];
            }
        }
        return min;
    }

    static int findMin(int** array, int rows, int cols){
        int min = array[0][0];
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                if(array[i][j] < min){
                    min = array[i][j];
                }
            }
        }
        return min;
    }

    static int findMinSkipZero(int* array, int length){
        int min = array[0];
        for(int i = 0; i < length; i++){
            if(array[i] < min && array[i] != 0){
                min = array[i];
            }
        }
        return min;
    }

    static int findMax(int* array, int length){
        int max = array[0];
        for(int i = 0; i < length; i++){
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

class Point{
public:
    int row;
    int col;

    Point(int row, int col){
        this->row = row;
        this->col = col;
    }

    Point(){
        this->row = 0;
        this->col = 0;
    }

    void setIndex(int row, int col){
        this->row = row;
        this->col = col;
    }

    void moveDirection(Point& other){
        setIndex(this->row + other.row, this->col + other.col);
    }

    bool operator!=(const Point& other){
        return !(this->row == other.row && this->col == other.col);
    }
};

class ChainCode{
public:
    int numRows,
        numCols,
        minVal,
        maxVal,
        label;
    int** zeroFramedAry;
    int** reconstructAry;
    Point coordOffset[8] = {Point(0, 1), Point(-1, 1), Point(-1, 0), Point(-1, -1), Point(0, -1), Point(1, -1), Point(1, 0), Point(1, 1)};
    int zeroTable[8] = {6, 0, 0, 2, 2, 4, 4, 6};
    Point startPoint;
    Point currentPoint;
    Point neighborCoord;
    int lastZeroDirection;
    int chainDirection;

    ChainCode(ifstream& inFile){
        inFile >> numRows >> numCols >> minVal >> maxVal;
        zeroFramedAry = Util::getArray(numRows + 2, numCols + 2);
        reconstructAry = Util::getArray(numRows + 2, numCols + 2);
        label = 1;
        loadImg(inFile);
    }

    void loadImg(ifstream& inFile){
        for(int i = 1; i < numRows + 1; i++){
            for(int j = 1; j < numCols + 1; j++){
                inFile >> zeroFramedAry[i][j];
            }
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

    void reformatPrettyPrint(int** image, ofstream& outFile){
        outFile << numRows << " " << numCols << " " << minVal << " " << maxVal << '\n';
        string str;
        int curWidth,
            pixelWidth = to_string(maxVal).length();

        for(int r = 1; r < numRows + 1; r++){
            for(int c = 1; c < numCols + 1; c++){
                outFile << (image[r][c] == 0 ? "." : to_string(image[r][c]));
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

    void getChainCode(ofstream& chainCodeFile, ofstream& debugFile){
        debugFile << "Entering getChainCode\n";
        chainCodeFile << numRows << " " << numCols << " " << minVal << " " << maxVal << '\n';

        bool foundStartPoint = false;
        for(int i = 0; i < numRows + 2; i++){
            if (foundStartPoint) break;
            for(int j = 0; j < numCols + 2; j++){
                if(zeroFramedAry[i][j] == 0) continue;

                label = zeroFramedAry[i][j];
                startPoint.setIndex(i, j);
                currentPoint.setIndex(i, j);
                lastZeroDirection = 4;
                // break the loop
                foundStartPoint = true;
                break;
            }
        }

        chainCodeFile << label << " " << startPoint.row << " " << startPoint.col << " ";

        do{
            lastZeroDirection = (lastZeroDirection + 1) % 8;
            chainDirection = findNextPoint(debugFile);
            chainCodeFile << chainDirection << " ";

            currentPoint.moveDirection(coordOffset[chainDirection]);
            zeroFramedAry[currentPoint.row][currentPoint.col] = label + 4;
            // lastZeroDirection is the direction of the same zero for the next point
            // for example, if next point direction is 4, that means the last zero direction is 3
            // 3 is the direction of last zero for current point
            // and the last zero is at the direction of 2 for next point
            lastZeroDirection = zeroTable[((chainDirection + 6) % 8)];
            debugFile << "lastZeroDirection: " << lastZeroDirection << " "
                      << "currentPoint: " << currentPoint.row << " " << currentPoint.col << " "
                      << "nextPoint: " << currentPoint.row + coordOffset[chainDirection].row << " "
                      << currentPoint.col + coordOffset[chainDirection].col << "\n";
        }while(currentPoint != startPoint);
        chainCodeFile << '\n';
        debugFile << "Leaving getChainCode \n";
    }

    int findNextPoint(ofstream& debugFile){
        debugFile << "Entering findNextPoint\n";
        int index = lastZeroDirection;
        bool isFound = false;
        int i, j;
        while(!isFound){
            i = currentPoint.row + coordOffset[index].row;
            j = currentPoint.col + coordOffset[index].col;

            if(zeroFramedAry[i][j] == label || zeroFramedAry[i][j] == label + 4) {
                isFound = true;
                chainDirection = index;
            }else{
                index = (index + 1) % 8;
            }
        }
        debugFile << "Leaving findNextPoint\n";
        return chainDirection;
    }

    ~ChainCode(){
        for(int i = 0; i < numRows + 2; i++){
            delete[] zeroFramedAry[i];
            delete[] reconstructAry[i];
        }
        delete[] zeroFramedAry;
        delete[] reconstructAry;
    }
};

int main(int argc, const char* args[]){
    // Read in the file
    ifstream inFile(args[1]);
    ofstream outFile(args[2]),
             debugFile(args[3]),
             chainCodeFile(args[1] + string("_chainCode.txt")),
             boundaryFile(args[1] + string("_boundary.txt"));

    ChainCode chainCode(inFile);
    debugFile<< "After loadImage, zeroFramedAry: \n";
    chainCode.reformatPrettyPrint(chainCode.zeroFramedAry, debugFile);

    return 0;
}