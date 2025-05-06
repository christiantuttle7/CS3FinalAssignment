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


struct wordNode{
    string word = " ";
    int occurence = 0;
};


class Hashing{

    private:
        ResizingArray<string>** chainArray;
        string** linProbingArray;
        wordNode** sortedOccurences;
        int probingArraySize;
        int chainArraySize;
        int hashFunctionType;
        int numWords;
        int uniqueSize;


    
    public:
        Hashing();
        ~Hashing();

        //test multiple hash functions
        void testHashFunctions();
        void testChaining();
        void testProbing();

    
        void doesWordOccur(string);
        void OccupancyRatioExperiments();
        void ReoccuringWords();
        int  readInFile(int);
        int readInFileProbing(int);
        int readInFileChaining(int);

        //This will take in a word and remove the punctuation as well as lowercase everything
        //We can then push it to the linear probing or chaining array
        void simplifyWord(char*);

        //help us calculate hash values
        int calculateSimpleHashValue(char[], int);
        int calculateBetterHashValue(char[], int);
        int calculateCubedHashValue(char[], int);
        //add more

        void findOccurences();
        void printTopEighty();
        void printBottomEighty();


        int promptUser();

};


//unfinished
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
    int prime = 2431;
    int length = strlen(wordToCompute);

    for (int i = 0; i < length; i++) {
        int charValue = tolower(wordToCompute[i]);
        hashValue = (hashValue * prime + charValue) % modNum;
    }

    
    return static_cast<int>(hashValue);
}

int Hashing::calculateCubedHashValue(char wordToCompute[], int modNum){
    int stringLength = strlen(wordToCompute);
        long long int stringValue = 0;
        
        
        for(int i = 0; i < stringLength; i++){
            stringValue += wordToCompute[i];
        }
        int hashValue = (stringValue * stringValue * stringValue ) % modNum;
        return hashValue;
}

void Hashing::simplifyWord(char* wordToSimplify){
    int currentValue = 0;
    for(int i = 0; wordToSimplify[i] != '\0'; ++i){
        if(isalpha(wordToSimplify[i])){
            wordToSimplify[currentValue++] = tolower(wordToSimplify[i]);
        }
    }
    wordToSimplify[currentValue] = '\0'; 
}



Hashing::Hashing(){
    //just declaring a bunch of variables
    ifstream inputText("A Scandal In Bohemia.txt");
    char word[50];
    numWords = 0;
    //char inputWordOne[50];
    //char inputWordTwo[50];
    //char inputWordThree[50];
    //int inputHashOne, inputHashTwo, inputHashThree;;
    int hashValue = 0;
    bool stringPlaced = false;

    //when file reads in this string it will then switch from chaining to linear probing
    string stopString = "VII.";
    string stopStringTwo = "IX.";

    hashFunctionType = 3;
    chainArraySize = 1001;
    probingArraySize = 320000;



    //allocating memory for the chainArray
    chainArray = new ResizingArray<string> *[chainArraySize];
    for(int i = 0; i < chainArraySize; i++){
        chainArray[i] = new ResizingArray<string>;
    }

    //allocating space for linear probing
    linProbingArray = new string*[probingArraySize];
    for(int i = 0; i < probingArraySize; i++){
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
        if(hashFunctionType == 1)hashValue = calculateSimpleHashValue(word, chainArraySize);
        else if(hashFunctionType == 2) hashValue = calculateBetterHashValue(word, chainArraySize);
        else hashValue = calculateCubedHashValue(word, chainArraySize);
        

        

        chainArray[hashValue]->Push(word);
    }

    //cant forget to process "VII."
    if(hashFunctionType == 1)hashValue = calculateSimpleHashValue(word, chainArraySize);
    else if(hashFunctionType == 2) hashValue = calculateBetterHashValue(word, chainArraySize);
    else hashValue = calculateCubedHashValue(word, chainArraySize);

    

    

    //goes through works 7-12 and stores using linear probing
    while(inputText >> word){
        //if(word == stopStringTwo) break;
        numWords++;
        char *ptrToPass = word;
        simplifyWord(ptrToPass);
        if(hashFunctionType == 1)hashValue = calculateSimpleHashValue(word, chainArraySize);
        else if(hashFunctionType == 2) hashValue = calculateBetterHashValue(word, chainArraySize);
        else hashValue = calculateCubedHashValue(word, chainArraySize);
        

        stringPlaced = false;
        while(!stringPlaced){
            if(linProbingArray[hashValue] == nullptr){
                linProbingArray[hashValue] = new string(word);
                stringPlaced = true;
            }
            else{
                if(hashValue == probingArraySize - 1)
                    hashValue = 0;
                else hashValue++;
            }
        }
        
    }




    cout << "Number of words: " << numWords << endl;
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Initial Read in took: " << duration.count() << " milliseconds" <<  endl;


    inputText.close();

}

Hashing::~Hashing(){
    //de allocating memory
    for(int i = 0; i < chainArraySize; i++){
        delete chainArray[i];
    }
    delete [] chainArray;

    for(int i = 0; i < probingArraySize; i++){
        delete linProbingArray[i];
    }
    delete [] linProbingArray;
    delete[] sortedOccurences;
}



void Hashing::findOccurences() {
    string currentWord;
    char currentWordC[50];
    bool wordPlaced = false;
    int hashValue;
    uniqueSize = 0;

    // Create array to track word occurrences
    wordNode** occurences = new wordNode*[numWords]();
    
    // Start timer
    auto start = high_resolution_clock::now();

    // Chaining Array 
    for (int i = 0; i < chainArraySize; i++) {
        int currentChainSize = chainArray[i]->getSize();
        for (int j = 0; j < currentChainSize; j++) {
            currentWord = chainArray[i]->getValue(j);
            strcpy(currentWordC, currentWord.c_str());

            // Compute hash
            if (hashFunctionType == 1)
                hashValue = calculateSimpleHashValue(currentWordC, numWords);
            else if (hashFunctionType == 2)
                hashValue = calculateBetterHashValue(currentWordC, numWords);
            else
                hashValue = calculateCubedHashValue(currentWordC, numWords);

            // Insert or update occurrence
            wordPlaced = false;
            while (!wordPlaced) {
                if (occurences[hashValue] == nullptr) {
                    occurences[hashValue] = new wordNode{currentWordC, 1};
                    uniqueSize++;
                    wordPlaced = true;
                } else if (occurences[hashValue]->word == currentWord) {
                    occurences[hashValue]->occurence++;
                    wordPlaced = true;
                } else {
                    hashValue = (hashValue + 1) % numWords;
                }
            }
        }
    }

    // probing array
    for (int i = 0; i < probingArraySize; i++) {
        if (linProbingArray[i] == nullptr)
            continue;

        currentWord = *linProbingArray[i];
        strcpy(currentWordC, currentWord.c_str());

        // Compute hash
        if (hashFunctionType == 1)
            hashValue = calculateSimpleHashValue(currentWordC, numWords);
        else if (hashFunctionType == 2)
            hashValue = calculateBetterHashValue(currentWordC, numWords);
        else
            hashValue = calculateCubedHashValue(currentWordC, numWords);

        // Insert or update occurrence
        wordPlaced = false;
        while (!wordPlaced) {
            if (occurences[hashValue] == nullptr) {
                occurences[hashValue] = new wordNode{currentWordC, 1};
                uniqueSize++;
                wordPlaced = true;
            } else if (occurences[hashValue]->word == currentWord) {
                occurences[hashValue]->occurence++;
                wordPlaced = true;
            } else {
                hashValue = (hashValue + 1) % numWords;
            }
        }
    }

    // Stop timer
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    
    cout << "Occurences took: " << duration.count() << " milliseconds" << endl;


    //cleaning out the null Values into new array
    sortedOccurences = new wordNode*[uniqueSize];
    int indexSorted = 0;
    for (int i = 0; i < numWords; i++) {
        if (occurences[i] != nullptr) {
            sortedOccurences[indexSorted] = occurences[i];
            indexSorted++;
        }
    }

    
    for (int i = 0; i < uniqueSize - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < uniqueSize; j++) {
            if (sortedOccurences[j]->occurence < sortedOccurences[minIdx]->occurence) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            wordNode* temp = sortedOccurences[i];
            sortedOccurences[i] = sortedOccurences[minIdx];
            sortedOccurences[minIdx] = temp;
        }
    }

    
    cout << uniqueSize << endl;

    printTopEighty();
    printBottomEighty();


    


    for (int i = 0; i < numWords; i++) {
        delete occurences[i];
    }
    delete[] occurences;
}

void Hashing::printTopEighty(){
    ofstream outputTop80("top80.txt");
    
    for(int i = uniqueSize - 1; i >= uniqueSize - 80; i--) {
        outputTop80 << sortedOccurences[i]->word << " - " << sortedOccurences[i]->occurence << endl;
    }
}


void Hashing::printBottomEighty(){
    ofstream outputBottom80("bottom80.txt");
    
    for(int i = 0; i < uniqueSize; i++) {
        outputBottom80 << sortedOccurences[i]->word << " - " << sortedOccurences[i]->occurence << endl;
    }
}


int Hashing::promptUser()
{
    string userInput;

    cout << "What Word are you trying to find in (The Adventure of the Engineer's Thumb) " << endl;
    cin >> userInput;

    for (int i = 0; i < 80000; i ++)
    {
        continue;
    }

    return 0;

    
}








































void Hashing::testProbing(){
    //cout << "Linear Probing Test 1(320000): " << readInFileProbing(320000) << " Milliseconds";
    cout << "Linear Probing Test 1(160000): " << readInFileProbing(160000) << " Milliseconds" << endl;
    cout << "Linear Probing Test 3(320000): " << readInFileProbing(320000) << " Milliseconds" << endl;
    cout << "Linear Probing Test 3(240000): " << readInFileProbing(240000) << " Milliseconds" << endl;

    

}

void Hashing::testChaining(){
    cout << "Chaining Test 1(100): " << readInFileChaining(100) << " Milliseconds" << endl;
    cout << "Chaining Test 2(500): " << readInFileChaining(500) << " Milliseconds" << endl;
    cout << "Chaining Test 3(1000): " << readInFileChaining(1000) << " Milliseconds" << endl;
}



int Hashing::readInFileProbing(int probeArraySize){

    //starting timer
    auto start = high_resolution_clock::now();

    ifstream inputText("A Scandal In Bohemia.txt");
    char word[50];
    
    int hashValue = 0;
    bool stringPlaced = false;

    string ** tempLinProbingArray = new string*[probeArraySize];
    for(int i = 0; i < probeArraySize; i++){
        tempLinProbingArray[i] = nullptr;
    }


    while(inputText >> word){
        
        char *ptrToPass = word;
        simplifyWord(ptrToPass);
        hashValue = calculateCubedHashValue(word, probeArraySize);
        stringPlaced = false;
        while(!stringPlaced){
            if(tempLinProbingArray[hashValue] == nullptr){
                tempLinProbingArray[hashValue] = new string(word);
                stringPlaced = true;
            }
            else{
                if(hashValue == probeArraySize-1)
                    hashValue = 0;
                else hashValue++;
            }
        }
        
    }
    

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    inputText.close();

    for(int i = 0; i < probeArraySize; i++){
        delete tempLinProbingArray[i];
    }
    delete [] tempLinProbingArray;

    return duration.count();
}

int Hashing::readInFileChaining(int chainArraySize){
    //starting timer
    auto start = high_resolution_clock::now();

    ifstream inputText("A Scandal In Bohemia.txt");
    char word[50];
    
    int hashValue = 0;
    
    
    //allocating space for temporary chain array
    ResizingArray<string> **tempChainArray = new ResizingArray<string> *[chainArraySize];
    for(int i = 0; i < chainArraySize; i++){
        tempChainArray[i] = new ResizingArray<string>;
    }

    while(inputText >> word){
        
        
        char *ptrToPass = word;
        simplifyWord(ptrToPass);
        
        hashValue = calculateCubedHashValue(word, chainArraySize);

        tempChainArray[hashValue]->Push(word);
    }


    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    inputText.close();

    for(int i = 0; i < 100; i++){
        delete tempChainArray[i];
    }
    delete [] tempChainArray;

    return duration.count();

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
        else hashValue = calculateCubedHashValue(word, 101);

        tempChainArray[hashValue]->Push(word);
    }
    //cout << endl;

    


    

    //goes through works 7-12 and stores using linear probing
    while(inputText >> word){
        
        
        char *ptrToPass = word;
        simplifyWord(ptrToPass);
        if(hashFunctionType == 1) hashValue = calculateSimpleHashValue(word, 80000);
        else if(hashFunctionType == 2) hashValue = calculateBetterHashValue(word, 80000);
        else hashValue = calculateCubedHashValue(word, 80000);
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
    cout << "Hash Function 3 took: " << readInFile(3) << " milliseconds" << endl;
    
}



#endif
