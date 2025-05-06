#include <iostream>
#include <fstream>
#include <cstring>
#include "ResizingArray.h"
#include "Hashing.h"
using namespace std;

int main()
{
    
   
Hashing Object;
Object.testHashFunctions();

   //main menue //switch case for choice -noah
int user_input = -1;

while (user_input != 0)
{

      cout << "Menu\n";
      cout<<"------------------\n";
      cout << "1. Display 80 Most Frequent Words\n";
      cout << "2. Display 80 Least Frequent Words\n";
      cout << "3. Count Sentences in the text\n";
      cout << "4. Prompt User for Word Search in Engineer's Thumb\n";
      cout << "5. Run All Performance Reports\n";
      cout << "0. Exit\n";
      cout << "Enter your choice: ";
      cin >> user_input;

      switch (user_input)
      {
         case 0:
            cout << "Goodbye!" << endl;
            break;
            
         case 1: 
         
            break;

         case 2:

            break;

         case 3:

            break;

         case 4:

            break;

         default:
            cout << "this is an invalid option" << endl;

      }
}


   return 0;
   
}