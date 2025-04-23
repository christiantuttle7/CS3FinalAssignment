#ifndef RESIZINGARRAY_H
#define RESIZINGARRAY_H
#include <iostream>
#include <fstream>


//template referenced from geeksforgeeks (https://www.geeksforgeeks.org/templates-cpp/)
template <typename T> class ResizingArray {
public:
    ResizingArray() {
        arr = new T[1000];
        size = 0;
        maxSize = 5;    
    }

    ~ResizingArray() {
        delete [] arr;
    }

    T operator[] (int i) const { // credit to stack overflow for overwriting operator (https://stackoverflow.com/questions/37043078/c-overloading-array-operator)
        return arr[i];
    }

    T& operator[] (int i) {
        return arr[i];
    }


    void Push(T element) {
        if (size >= maxSize) 
            Double();
        arr[size] = element;
        size++;
    }
    
    T Pop() {
        //if the array could hold four times as many elements as it does now
        //if (size*4 <= maxSize)
        //    Quarter();
        //Dont bother cleaning out the data at that array - it will be ineccessable
        size--;
        return arr[size];
    }
    
    void Print() {
        std::cout << '{';
        for (int i = 0; i < size; i++) {
            std::cout << arr[i];
            if (i < size - 1)
                std::cout << ", ";
        }
        std::cout << "} (" << size << '/' << maxSize << ")\n";    
    }
    
    int getSize() {
        return size;
    }
    int getMaxSize() {
        return maxSize;
    }

    T getValue(int indexNum){
        return arr[indexNum];
    }

private:
    //We do not trust the user to double the array
    void Double() {
        T* newArr = new T[maxSize*2];
        for (int i = 0; i < maxSize; i++) { //copy elements from old arr to new arr
            newArr[i] = arr[i];
        }
        delete [] arr; //remove old array
        arr = newArr; //move pointer to new array    
        maxSize *= 2; //double the size    
    }
    /*
    void Quarter() {
         T* newArr = new T[maxSize/4];
        for (int i = 0; i < maxSize; i++) { //copy elements from old arr to new arr
            newArr[i] = arr[i];
        }
        delete [] arr; //remove old array
        arr = newArr; //move pointer to new array    
        maxSize /= 4; //quarter the size    
           
    }
    */
    T* arr;
    int size;
    int maxSize;
};

#endif