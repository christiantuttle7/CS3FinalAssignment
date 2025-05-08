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

struct wordNode
{
    string word = " ";
    int occurence = 0;
};

class Hashing
{

private:
    // chaining array and linear probing arrays
    ResizingArray<string> **chainArray;
    string **linProbingArray;

    // these two keep track of occurences of every word
    wordNode **sortedOccurences;
    wordNode **occurences;

    // these tell us what hash function, load factor, probing occupany to use (good for experimenting)
    int probingArraySize;
    int chainArraySize;
    int hashFunctionType;

    // Keep track of runtimes,and number of words, sentences, etc
    int numWords;
    int uniqueSize;
    int readInTime;
    int readInTimeChaining;
    int readInTimeProbing;
    int findOccurencesTime;
    int sortOccurencesTime;
    int numSentences;

public:
    Hashing();
    ~Hashing();

    // This will take in a word and remove the punctuation as well as lowercase everything
    // We can then calculate hash function push it to the linear probing or chaining array
    bool simplifyWord(char *);

    // hash functions (only one can be chosen each run)
    int calculateSimpleHashValue(char[], int);
    int calculateBetterHashValue(char[], int);
    int calculateCubedHashValue(char[], int);

    // other functions
    void findOccurences();
    void printTopEighty();
    void printBottomEighty();
    int promptUser();
    void doesWordOccur(string);
    void printReport();
    void printNumSentences();
};

void Hashing::printNumSentences()
{
    cout << "Number of sentences: " << numSentences << endl;
}

// unfinished
void Hashing::doesWordOccur(string wordToFind)
{
    int occurences = 0;

    for (int i = 0; i < 80000; i++)
    {
        if (linProbingArray[i] != nullptr && *linProbingArray[i] == wordToFind)
        {
            occurences++;
        }
    }
    cout << "Word Occurs " << occurences << " times" << endl;
}

int Hashing::calculateSimpleHashValue(char wordToCompute[], int modNum)
{

    int stringLength = strlen(wordToCompute);
    int stringValue = 0;
    char currentChar;

    for (int i = 0; i < stringLength; i++)
    {
        currentChar = wordToCompute[i];
        stringValue += currentChar;
    }
    int hashValue = stringValue % modNum;
    return hashValue;
}

int Hashing::calculateBetterHashValue(char wordToCompute[], int modNum)
{
    unsigned long long hashValue = 0;
    int prime = 2431;
    int length = strlen(wordToCompute);

    for (int i = 0; i < length; i++)
    {
        int charValue = tolower(wordToCompute[i]);
        hashValue = (hashValue * prime + charValue);
    }
    hashValue = hashValue % modNum;

    return hashValue;
}

int Hashing::calculateCubedHashValue(char wordToCompute[], int modNum)
{
    int stringLength = strlen(wordToCompute);
    long long int stringValue = 0;

    for (int i = 0; i < stringLength; i++)
    {
        stringValue += wordToCompute[i];
    }
    int hashValue = (stringValue * stringValue * stringValue) % modNum;
    return hashValue;
}

bool Hashing::simplifyWord(char *wordToSimplify)
{
    int currentValue = 0;
    bool isSentence = false;
    char currentChar;
    for (int i = 0; wordToSimplify[i] != '\0'; ++i)
    {
        currentChar = wordToSimplify[i];
        if (isalpha(currentChar) || (currentChar == '-'))
        {
            wordToSimplify[currentValue++] = tolower(wordToSimplify[i]);
        }
        if (currentChar == '.' || currentChar == '!' || currentChar == '?')
            isSentence = true;
    }
    wordToSimplify[currentValue] = '\0';

    return isSentence;
}

Hashing::Hashing()
{
    // just declaring a bunch of variables
    ifstream inputText("A Scandal In Bohemia.txt");
    char word[50];
    numWords = 0;
    numSentences = 0;

    int hashValue = 0;
    bool stringPlaced = false;

    // when file reads in this string it will then switch from chaining to linear probing
    string stopString = "VII.";
    string stopStringTwo = "IX.";

    hashFunctionType = 2;
    chainArraySize = 10000;
    probingArraySize = 160000;

    // allocating memory for the chainArray
    chainArray = new ResizingArray<string> *[chainArraySize];
    for (int i = 0; i < chainArraySize; i++)
    {
        chainArray[i] = new ResizingArray<string>;
    }

    // allocating space for linear probing
    linProbingArray = new string *[probingArraySize];
    for (int i = 0; i < probingArraySize; i++)
    {
        linProbingArray[i] = nullptr;
    }

    // starting timer for reading in all works
    auto startOne = high_resolution_clock::now();

    // going through works 1-6, and storing it using chaining
    while (inputText >> word)
    {
        if (word == stopString)
            break;

        numWords++;

        if (simplifyWord(word))
            numSentences++;
        if (hashFunctionType == 1)
            hashValue = calculateSimpleHashValue(word, chainArraySize);
        else if (hashFunctionType == 2)
            hashValue = calculateBetterHashValue(word, chainArraySize);
        else
            hashValue = calculateCubedHashValue(word, chainArraySize);

        chainArray[hashValue]->Push(word);
    }

    // stopping timer that keeps track of Works I -VI read in time
    auto stopTwo = high_resolution_clock::now();
    auto durationChaining = duration_cast<milliseconds>(stopTwo - startOne);

    readInTimeChaining = durationChaining.count();

    // cant forget to process "VII." in linear probing
    simplifyWord(word);
    if (hashFunctionType == 1)
        hashValue = calculateSimpleHashValue(word, chainArraySize);
    else if (hashFunctionType == 2)
        hashValue = calculateBetterHashValue(word, chainArraySize);
    else
        hashValue = calculateCubedHashValue(word, chainArraySize);
    stringPlaced = false;
    while (!stringPlaced)
    {
        if (linProbingArray[hashValue] == nullptr)
        {
            linProbingArray[hashValue] = new string(word);
            stringPlaced = true;
        }
        else
        {

            hashValue = (hashValue + 1) % probingArraySize;
        }
    }
    numWords++;

    // goes through works 7-12 and stores using linear probing
    while (inputText >> word)
    {
        if (word == stopStringTwo)
        {
            cout << numWords;
            break;
        }
        numWords++;
        // removing all puncutation from a word and lowercasing it, also finding out if a sentence may have occured
        if (simplifyWord(word))
            numSentences++;

        // calculating hash value
        if (hashFunctionType == 1)
            hashValue = calculateSimpleHashValue(word, probingArraySize);
        else if (hashFunctionType == 2)
            hashValue = calculateBetterHashValue(word, probingArraySize);
        else
            hashValue = calculateCubedHashValue(word, probingArraySize);

        stringPlaced = false;

        // Tries to find a place for the word to be placed
        while (!stringPlaced)
        {
            // if the index is empty, place word
            if (linProbingArray[hashValue] == nullptr)
            {
                linProbingArray[hashValue] = new string(word);
                stringPlaced = true;
            }
            else // if the index is taken, try the next one over (linear probing)
            {
                hashValue = (hashValue + 1) % probingArraySize;
            }
        }
    }

    // Asking the user for up to 8 keys
    char userInput[8][50];
    int numKeys = 0;
    cout << "Enter up to 8 keys (type '@@@' to stop):" << endl;
    while (numKeys < 8)
    {
        cout << "Key " << numKeys + 1 << ": ";
        cin >> userInput[numKeys];
        cin.ignore();
        if (strcmp(userInput[numKeys], "@@@") == 0)
            break;
        numKeys++;
    }

    const int base = 256;   // ASCII base
    const int prime = 1001; // A small prime number for modulus

    int m[numKeys];

    int keyHash[numKeys];
    int windowHash[numKeys];
    int h[numKeys];
    char rollingWindow[numKeys][100];
    int charsInWindow[numKeys];
    int matchCount[numKeys];

    int position = 0;

    for (int i = 0; i < numKeys; i++)
    {
        m[i] = strlen(userInput[i]);
        keyHash[i] = 0;
        windowHash[i] = 0;
        h[i] = 1;
        charsInWindow[i] = 0;
        matchCount[i] = 0;
        for (int j = 0; j < m[i] - 1; j++)
            h[i] = (h[i] * base) % prime;
        for (int j = 0; j < m[i]; j++)
            keyHash[i] = (base * keyHash[i] + userInput[i][j]) % prime;
    }


    word[0] = '\0';
    int currentWordIndex = 0;


    while (inputText >> word)
    {

        if (simplifyWord(word))
            numSentences++;
        numWords++;

        // calculating hash value
        if (hashFunctionType == 1)
            hashValue = calculateSimpleHashValue(word, probingArraySize);
        else if (hashFunctionType == 2)
            hashValue = calculateBetterHashValue(word, probingArraySize);
        else
            hashValue = calculateCubedHashValue(word, probingArraySize);

        stringPlaced = false;

        // Tries to find a place for the word to be placed
        while (!stringPlaced)
        {
            // if the index is empty, place word
            if (linProbingArray[hashValue] == nullptr)
            {
                linProbingArray[hashValue] = new string(word);
                stringPlaced = true;
            }
            else // if the index is taken, try the next one over (linear probing)
            {
                hashValue = (hashValue + 1) % probingArraySize;
            }
        }

        for (int r = 0; r < strlen(word); r++)
        {
            for (int i = 0; i < numKeys; i++)
            {
                if (charsInWindow[i] < m[i])
                {
                    // building the initial window
                    rollingWindow[i][charsInWindow[i]++] = word[r];

                    if (charsInWindow[i] == m[i])
                    {
                        windowHash[i] = 0;
                        for (int j = 0; j < m[i]; j++)
                            windowHash[i] = (base * windowHash[i] + rollingWindow[i][j]) % prime;
                    }
                    continue;
                }

                // Rolling case
                char outgoingChar = rollingWindow[i][0]; // SAVE this BEFORE shift

                for (int j = 0; j < m[i] - 1; j++)
                    rollingWindow[i][j] = rollingWindow[i][j + 1];
                rollingWindow[i][m[i] - 1] = word[r];

                windowHash[i] = (base * (windowHash[i] - outgoingChar * h[i]) + word[r]) % prime;
                if (windowHash[i] < 0)
                    windowHash[i] += prime;

                if (windowHash[i] == keyHash[i] &&
                    strncmp(rollingWindow[i], userInput[i], m[i]) == 0)
                {
                    cout << "Match found for key " << i + 1
                         << " at position " << (position - m[i] + 1) << endl;
                    matchCount[i]++;
                }
            }
            position++;
        }
    }

    for (int i = 0; i < numKeys; i++)
    {
        cout << "Total matches found: " << matchCount[i] << endl;
    }

    auto stopOne = high_resolution_clock::now();
    auto durationOne = duration_cast<milliseconds>(stopOne - startOne);

    readInTime = durationOne.count();
    findOccurences();

    inputText.close();
}

Hashing::~Hashing()
{
    // de allocating memory
    for (int i = 0; i < chainArraySize; i++)
    {
        delete chainArray[i];
    }
    delete[] chainArray;

    for (int i = 0; i < probingArraySize; i++)
    {
        delete linProbingArray[i];
    }

    for (int i = 0; i < numWords; i++)
    {
        delete occurences[i];
    }
    delete[] occurences;
    delete[] linProbingArray;
    delete[] sortedOccurences;
}

void Hashing::findOccurences()
{
    const int TABLE_SIZE = 131071; // large prime number
    string currentWord;
    char currentWordC[50];
    bool wordPlaced = false;
    int hashValue;

    // Allocate new occurrence table
    occurences = new wordNode *[TABLE_SIZE]();
    uniqueSize = 0;

    auto start = high_resolution_clock::now();

    // Process chaining array
    for (int i = 0; i < chainArraySize; i++)
    {
        int currentChainSize = chainArray[i]->getSize();
        for (int j = 0; j < currentChainSize; j++)
        {
            currentWord = chainArray[i]->getValue(j);
            strcpy(currentWordC, currentWord.c_str());

            // Hash using fixed size
            if (hashFunctionType == 1)
                hashValue = calculateSimpleHashValue(currentWordC, TABLE_SIZE);
            else if (hashFunctionType == 2)
                hashValue = calculateBetterHashValue(currentWordC, TABLE_SIZE);
            else
                hashValue = calculateCubedHashValue(currentWordC, TABLE_SIZE);

            // Insert or update
            wordPlaced = false;
            int startIdx = hashValue;
            while (!wordPlaced)
            {
                if (occurences[hashValue] == nullptr)
                {
                    occurences[hashValue] = new wordNode{currentWord, 1};
                    uniqueSize++;
                    wordPlaced = true;
                }
                else if (occurences[hashValue]->word == currentWord)
                {
                    occurences[hashValue]->occurence++;
                    wordPlaced = true;
                }
                else
                {
                    hashValue = (hashValue + 1) % TABLE_SIZE;
                    if (hashValue == startIdx)
                        break; // avoid infinite loop
                }
            }
        }
    }

    // Process linear probing array
    for (int i = 0; i < probingArraySize; i++)
    {
        if (linProbingArray[i] == nullptr)
            continue;

        currentWord = *linProbingArray[i];
        strcpy(currentWordC, currentWord.c_str());

        if (hashFunctionType == 1)
            hashValue = calculateSimpleHashValue(currentWordC, TABLE_SIZE);
        else if (hashFunctionType == 2)
            hashValue = calculateBetterHashValue(currentWordC, TABLE_SIZE);
        else
            hashValue = calculateCubedHashValue(currentWordC, TABLE_SIZE);

        // Insert or update
        wordPlaced = false;
        int startIdx = hashValue;
        while (!wordPlaced)
        {
            if (occurences[hashValue] == nullptr)
            {
                occurences[hashValue] = new wordNode{currentWord, 1};
                uniqueSize++;
                wordPlaced = true;
            }
            else if (occurences[hashValue]->word == currentWord)
            {
                occurences[hashValue]->occurence++;
                wordPlaced = true;
            }
            else
            {
                hashValue = (hashValue + 1) % TABLE_SIZE;
                if (hashValue == startIdx)
                    break;
            }
        }
    }
    // finding time it took to find all occurences
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Occurences took: " << duration.count() << " milliseconds" << endl;
    findOccurencesTime = duration.count();

    start = high_resolution_clock::now();

    // Extract and sort unique entries
    sortedOccurences = new wordNode *[uniqueSize];
    int idx = 0;
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (occurences[i] != nullptr)
        {
            sortedOccurences[idx++] = occurences[i];
        }
    }

    // Selection sort (ascending)
    for (int i = 0; i < uniqueSize - 1; i++)
    {
        int minIdx = i;
        for (int j = i + 1; j < uniqueSize; j++)
        {
            if (sortedOccurences[j]->occurence < sortedOccurences[minIdx]->occurence)
            {
                minIdx = j;
            }
        }
        if (minIdx != i)
        {
            wordNode *temp = sortedOccurences[i];
            sortedOccurences[i] = sortedOccurences[minIdx];
            sortedOccurences[minIdx] = temp;
        }
    }

    // finding time it took to sort the occurences
    stop = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stop - start);
    cout << "Occurences took: " << duration.count() << " milliseconds" << endl;
    sortOccurencesTime = duration.count();

    cout << "Unique words: " << uniqueSize << endl;
}

void Hashing::printTopEighty()
{
    ofstream outputTop80("top80.txt");

    outputTop80 << "80 Top Occuring Words: " << endl;
    for (int i = uniqueSize - 1; i >= uniqueSize - 80; i--)
    {
        outputTop80 << sortedOccurences[i]->word << " - " << sortedOccurences[i]->occurence << endl;
    }
    cout << "Top 80 most occuring words have been printed to \"top80.txt\"" << endl;
}

void Hashing::printBottomEighty()
{
    ofstream outputBottom80("bottom80.txt");

    for (int i = 0; i < uniqueSize; i++)
    {
        outputBottom80 << sortedOccurences[i]->word << " - " << sortedOccurences[i]->occurence << endl;
    }
    cout << "Bottom 80 most occuring words have been printed to \"top80.txt\"" << endl;
}

int Hashing::promptUser()
{
    string userInput;

    cout << "What Word are you trying to find in (The Adventure of the Engineer's Thumb) " << endl;
    cin >> userInput;

    for (int i = 0; i < 80000; i++)
    {
        continue;
    }

    return 0;
}

void Hashing::printReport()
{
    cout << "\nHASHING REPORT - All Times in Milliseconds\n";
    cout << "-------------------------------------------\n";
    cout << "Number of Words:              " << numWords << "\n";
    cout << "Number of Sentences:          " << numSentences << "\n";
    cout << "Overall Read-In Time:         " << readInTime << "\n";
    cout << "Works I-VI Read-In Time:      " << readInTimeChaining << "\n";
    cout << "Works VII-IX Read-In Time:    " << readInTimeProbing << "\n";
    cout << "Time to Find Occurrences:     " << findOccurencesTime << "\n";
    cout << "Time to Sort Occurrences:     " << sortOccurencesTime << "\n\n";
}

#endif