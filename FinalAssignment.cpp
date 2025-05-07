#include <iostream>
#include <fstream>
#include <cstring>
#include "ResizingArray.h"
#include "Hashing.h"
using namespace std;

bool askUserToContinue(){
   string userInput;
   cout << "Would you like to continue[y/n]:";
   cin >> userInput;
   cin.ignore();
   if(userInput == "y" || userInput == "yes" || userInput == "Yes") return true;
   return false;
}

int main()
{

   Hashing Object;

   // main menue //switch case for choice -noah
   bool stopProgram = false;
   int userInput = -1;

   while (stopProgram == false)
   {
      cout << "\n";
      cout << "Menu\n";
      cout << "------------------\n";
      cout << "1. Print 80 Most Frequent Words To File\n";
      cout << "2. Print 80 Least Frequent Words To File\n";
      cout << "3. Count Sentences in the text\n";
      cout << "4. Prompt User for Word Search in Engineer's Thumb\n";
      cout << "5. Run All Performance Reports\n";
      cout << "0. Exit\n";
      cout << "Enter your choice: ";
      cin >> userInput;
      cin.ignore();

      switch (userInput)
      {
      case 0:
         cout << "Goodbye!" << endl;
         exit(0);

      case 1:
         Object.printTopEighty();
         if(askUserToContinue()) break;
         else stopProgram = true;
         break;

      case 2:
         Object.printBottomEighty();
         if(askUserToContinue()) break;
         else stopProgram = true;
         break;

      case 3:
         Object.printNumSentences();
         if(askUserToContinue()) break;
         else stopProgram = true;
         break;

      case 4:
         break;
      case 5:
         Object.printReport();
         if(askUserToContinue()) break;
         else stopProgram = true;
         break;
      default:
         cout << "this is an invalid option" << endl;
      }
   }

   return 0;
}

