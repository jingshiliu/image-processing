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