//
//  main.cpp
//  CS381_Image_Processing_Project0B
//
//  Created by Jingshi Liu on 9/2/2023.
//


#include <iostream>
#include <fstream>

using namespace std;

void processing(ifstream& inFile, ofstream& outFile1, ofstream& outFile2, int thrVal, int numCols){
    int pixelVal;

    int i = 0;
    while(inFile >> pixelVal){
        if(pixelVal >= thrVal){
            outFile1 << 1 << " ";
            outFile2 << pixelVal << " ";
        }else{
            outFile1 << 0 << " ";
            outFile2 << 0 << " ";
        }
        if(++i == numCols){
            outFile1 << '\n';
            outFile2 << '\n';
            i = 0;
        }
    }
}

int main(int argc, const char * argv[]){
    ifstream inFile;
    ofstream outFile1, outFile2;

    inFile.open(argv[1]);
    outFile1.open(argv[2]);
    outFile2.open(argv[3]);

    int numRows, numCols, minVal, maxVal;
    inFile >> numRows >> numCols >> minVal >> maxVal;

    int thrVal;
    cout<<"Enter a Threshold Value: ";
    cin>>thrVal;
    cout<<endl;

    outFile1<< numRows<< " "<< numCols<< " "<< minVal<< " "<< 1<< '\n';
    outFile2<< numRows<< " "<< numCols<< " "<< minVal<< " "<< maxVal<< '\n';

    processing(inFile, outFile1, outFile2, thrVal, numCols);

    inFile.close();
    outFile1.close();
    outFile2.close();
}