#ifndef HASHING_H
#define HASHING_H
#include <iostream>

#include <fstream>
#include "ResizingArray.h"
#include <cstring>
#include <cctype>
#include <chrono>
using namespace std;
using namespace std::chrono;


class Hashing{

    private:
        ResizingArray<string>** chainArray;
        string** linProbingArray;

    
    public:
        Hashing();
        ~Hashing();

        //test multiple hash functions
        void testHashFunctions();


        void doesWordOccur(string);
        void OccupancyRatioExperiments();
        void ReoccuringWords();
        int readInFile(int);

        //This will take in a word and remove the punctuation as well as lowercase everything
        //We can then push it to the linear probing or chaining array
        void simplifyWord(char*);

        //help us calculate hash values
        int calculateSimpleHashValue(char[], int);
        int calculateBetterHashValue(char[], int);
        int calculateComplicatedHashValue(char[], int);
        //add more

};

void Hashing::doesWordOccur(string wordToFind){
    int occurences = 0;

    for(int i = 0; i < 80000; i++){
        if(linProbingArray[i] != nullptr && *linProbingArray[i] == wordToFind){
            occurences++;
        }
    }
    cout << "Word Occurs " << occurences << " times" << endl;
}

int Hashing::calculateSimpleHashValue(char wordToCompute[], int modNum){
    
        int stringLength = strlen(wordToCompute);
        int stringValue = 0;
        char currentChar;
        
        for(int i = 0; i < stringLength; i++){
            currentChar = wordToCompute[i];
            stringValue += currentChar;

        }
        int hashValue = stringValue % modNum;
        return hashValue;
    
   
}

int Hashing::calculateBetterHashValue(char wordToCompute[], int modNum){
    unsigned long long hashValue = 0;
    int prime = 31;
    int length = strlen(wordToCompute);

    for (int i = 0; i < length; i++) {
        int charValue = tolower(wordToCompute[i]);
        hashValue = (hashValue * prime + charValue) % modNum;
    }

    
    return static_cast<int>(hashValue);
}

int Hashing::calculateComplicatedHashValue(char wordToCompute[], int modNum){
    return modNum;
}

void Hashing::simplifyWord(char* wordToSimplify){
    int length = strlen(wordToSimplify);
    for(int i = 0; i < length; i++){
        wordToSimplify[i] = tolower(wordToSimplify[i]);
    }
    if(ispunct(wordToSimplify[length-1])){
        wordToSimplify[length-1] = '\0';
    }
}



Hashing::Hashing(){
    //just declaring a bunch of variables
    ifstream inputText("A Scandal In Bohemia.txt");
    char word[50];
    int numWords = 0;
    
    int hashValue = 0;
    bool stringPlaced = false;

    //when file reads in this string it will then switch from chaining to linear probing
    string stopString = "VII.";

    //allocating memory for the chainArray
    chainArray = new ResizingArray<string> *[100];
    for(int i = 0; i < 100; i++){
        chainArray[i] = new ResizingArray<string>;
    }

    //allocating space for linear probing
    linProbingArray = new string*[80000];
    for(int i = 0; i < 80000; i++){
        linProbingArray[i] = nullptr;
        
    }



    //starting timer
    auto start = high_resolution_clock::now();

    //going through works 1-6, and storing it using chaining
    while(inputText >> word){
        if(word == stopString) break;
        
        numWords++;
        
        
        char *ptrToPass = word;
        simplifyWord(ptrToPass);
        hashValue = calculateSimpleHashValue(word, 100);

        chainArray[hashValue]->Push(word);
    }
    //cout << endl;

    //cout << numWords << endl;


    

    //goes through works 7-12 and stores using linear probing
    while(inputText >> word){
        
        numWords++;
        char *ptrToPass = word;
        simplifyWord(ptrToPass);
        hashValue = calculateSimpleHashValue(word, 80000);
        stringPlaced = false;
        while(!stringPlaced){
            if(linProbingArray[hashValue] == nullptr){
                linProbingArray[hashValue] = new string(word);
                stringPlaced = true;
            }
            else{
                if(hashValue == 79999)
                    hashValue = 0;
                else hashValue++;
            }
        }
        //cout << numWords << " " << hashValue << endl;
    }
    cout << "Number of words: " << numWords << endl;
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Initial Read in took: " << duration.count() << "milliseconds" <<  endl;


    inputText.close();

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










int Hashing::readInFile(int hashFunctionType){
    ifstream inputText("A Scandal In Bohemia.txt");
    char word[50];
    
    int hashValue = 0;
    bool stringPlaced = false;

    //when file reads in this string it will then switch from chaining to linear probing
    string stopString = "VII.";

    //allocating space for temporary chain array
    ResizingArray<string> **tempChainArray = new ResizingArray<string> *[101];
    for(int i = 0; i < 101; i++){
        tempChainArray[i] = new ResizingArray<string>;
    }

    //allocating space for temporary linear probing array
    string ** tempLinProbingArray = new string*[80000];
    for(int i = 0; i < 80000; i++){
        tempLinProbingArray[i] = nullptr;
        
    }


    //starting timer
    auto start = high_resolution_clock::now();

    //going through works 1-6, and storing it using chaining
    while(inputText >> word){
        if(word == stopString) break;
        
        
        
        
        char *ptrToPass = word;
        simplifyWord(ptrToPass);
        if(hashFunctionType == 1) hashValue = calculateSimpleHashValue(word, 101);
        else if(hashFunctionType == 2) hashValue = calculateBetterHashValue(word, 101);
        else hashValue = calculateComplicatedHashValue(word, 101);

        tempChainArray[hashValue]->Push(word);
    }
    //cout << endl;

    


    

    //goes through works 7-12 and stores using linear probing
    while(inputText >> word){
        
        
        char *ptrToPass = word;
        simplifyWord(ptrToPass);
        if(hashFunctionType == 1) hashValue = calculateSimpleHashValue(word, 80000);
        else if(hashFunctionType == 2) hashValue = calculateBetterHashValue(word, 80000);
        else hashValue = calculateComplicatedHashValue(word, 80000);
        stringPlaced = false;
        while(!stringPlaced){
            if(tempLinProbingArray[hashValue] == nullptr){
                tempLinProbingArray[hashValue] = new string(word);
                stringPlaced = true;
            }
            else{
                if(hashValue == 79999)
                    hashValue = 0;
                else hashValue++;
            }
        }
        
    }
    inputText.close();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);


    for(int i = 0; i < 100; i++){
        delete tempChainArray[i];
    }
    delete [] tempChainArray;

    for(int i = 0; i < 80000; i++){
        delete tempLinProbingArray[i];
    }
    delete [] tempLinProbingArray;

    return duration.count();
}

void Hashing::testHashFunctions(){

    cout << "Hash Function 1 took: " << readInFile(1) << " milliseconds" << endl;
    cout << "Hash Function 2 took: " << readInFile(2) << " milliseconds" << endl;
    
}



#endif
