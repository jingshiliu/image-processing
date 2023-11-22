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

    void constructBoundary(ifstream& chainCodeFile){
        int label, row, col, chainCode;
        // pass the headers: numRows, numCols, minVal, maxVal
        for(int i = 0; i < 4; i++){
            chainCodeFile >> label;
        }
        chainCodeFile >> label >> row >> col;
        reconstructAry[row][col] = label;
        while(chainCodeFile >> chainCode){
            row += coordOffset[chainCode].row;
            col += coordOffset[chainCode].col;
            reconstructAry[row][col] = label;
        }
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
    outFile << "After loadImage, zeroFramedAry: \n";
    chainCode.reformatPrettyPrint(chainCode.zeroFramedAry, outFile);

    chainCode.getChainCode(chainCodeFile, debugFile);
    debugFile<< "After getChainCode, zeroFramedAry: \n";
    outFile << "\n\nAfter getChainCode, zeroFramedAry: \n";
    chainCode.reformatPrettyPrint(chainCode.zeroFramedAry, outFile);

    chainCodeFile.close();
    ifstream chainCodeFileIn(args[1] + string("_chainCode.txt"));
    chainCode.constructBoundary(chainCodeFileIn);

    outFile << "\n\nAfter constructBoundary, reconstructAry: \n";
    chainCode.reformatPrettyPrint(chainCode.reconstructAry, outFile);
    chainCode.imageReformat(chainCode.reconstructAry, boundaryFile);

    return 0;
}