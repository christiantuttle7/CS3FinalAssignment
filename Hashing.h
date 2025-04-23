#ifndef HASHING_H
#define HASHING_H
#include <iostream>
#include <fstream>
#include "ResizingArray.h"
#include <cstring>
using namespace std;


class Hashing{

    private:
        ResizingArray<string>** chainArray;
        string** linProbingArray;

    
    public:
        Hashing();
        ~Hashing();
        void HashFunctionExperiments();
        void OccupancyRatioExperiments();
        void ReoccuringWords();
        //add more

};

Hashing::Hashing(){
    //just declaring a bunch of variables
    ifstream inputText("A Scandal In Bohemia.txt");
    char filler[100];
    int numWords = 0;
    string fillerString;
    int stringLength = 0;
    int stringValue = 0;
    int hashValue = 0;
    char currentChar = ' ';
    bool stringPlaced = false;

    //when file reads in this string it will then switch from chainging to linear probing
    string stopString = "VII.";

    //allocating memory for the chainArray
    chainArray = new ResizingArray<string> *[100];
    for(int i = 0; i < 100; i++){
        chainArray[i] = new ResizingArray<string>;
    }

    //going through works 1-6, and storing it using chaining
    while(inputText >> filler){
        stringLength = strlen(filler);
        stringValue = 0;
        numWords++;
        fillerString = filler;
        if(fillerString == stopString) break;
        for(int i = 0; i < stringLength; i++){
            currentChar = filler[i];
            stringValue += currentChar;

        }
        hashValue = stringValue % 100;
        chainArray[hashValue]->Push(filler);
    }
    cout << endl;
    



    
    //allocating space for linear probing
    linProbingArray = new string*[80000];
    for(int i = 0; i < 80000; i++){
        linProbingArray[i] = nullptr;
    }

    //goes through works 7-12 and stores using linear probing
    while(inputText >> filler){
        stringLength = strlen(filler);
        stringValue = 0;
        numWords++;
        for(int i = 0; i < stringLength; i++){
            currentChar = filler[i];
            stringValue += currentChar;

        }
        hashValue = stringValue % 80000;
        stringPlaced = false;
        while(!stringPlaced){
            if(linProbingArray[hashValue] == nullptr){
                linProbingArray[hashValue] = new string(filler);
                stringPlaced = true;
            }
            else{
                hashValue++;
            }
        }
        
    }
    cout << "Number of words: " << numWords << endl;

}

Hashing::~Hashing(){
    //de allocating memory
    for(int i = 0; i < 100; i++){
        delete chainArray[i];
    }
    delete [] chainArray;

    for(int i = 0; i < 80000; i++){
        delete linProbingArray[i];
    }
    delete [] linProbingArray;
}



#endif
