// Randomly generate numbers between a min and max for user to guess.


//include brings in content from the different libraries
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Define is a preprocessor directive used to create the constants max and min
#define MIN 1
#define MAX 1000


//declaring the functions
void guessGame(void); 
bool isCorrect(int guess, int answer); 

int main(void) {
    //seed random generator using time for more randomness
   srand(time(0)); 

   //call guess game
   guessGame();
} // end main



// function that runs the game
void guessGame(void) {
    
    //initialize integers for the users guess and their response to continue playing
   int response =0;
   int guess = 0;

   // Do while loop keeps program going until user wants to stop
   do {

      // generates a random number between 1 and 1000
      int answer = 1 + rand() % 1000;

      //printf tells user it has a number between the max and min
      printf("I have a number between %d and %d.\n", MIN, MAX);

      // puts asks user to guess number
      puts("Can you guess my number?\n" 
           "Please type your first guess.");

      //%s put question mark on next line
      printf("%s", "? ");
      

      //scanf reads input and stores it as guess
      scanf("%d", &guess);

      // If guess is incorrect scanf stores users next guess. If guess is correct user is notified and asked if they want to play again
       
      while (!isCorrect(guess, answer)) {
         scanf("%d", &guess);
      }


      puts("\nExcellent! You guessed the number!\n"
         "Would you like to play again?");
      printf("%s", "Please type (1=yes, 2=no)? ");
      scanf("%d", &response);
      puts("");

   } while (response == 1);
} // end function guessGame

// isCorrect function checks if users guess is correct
bool isCorrect(int guess, int answer) {

    //initialize boolean correct as false
    bool correct = false;

   // if statement sets correct to true if users guess is correct.
   if (guess == answer) {
      correct = true;
   }

   // If/else statement determines if an incorrect guess is too high or too low
   if (guess < answer) {
      printf( "%s", "Too low. Try again.\n? " );
   }
   else {
      printf( "%s", "Too high. Try again.\n? " );
   }

   return correct;
} // end function isCorrect



/**************************************************************************
 * (C) Copyright 1992-2012 by Deitel & Associates, Inc. and               *
 * Pearson Education, Inc. All Rights Reserved.                           *
 *                                                                        *
 * DISCLAIMER: The authors and publisher of this book have used their     *
 * best efforts in preparing the book. These efforts include the          *
 * development, research, and testing of the theories and programs        *
 * to determine their effectiveness. The authors and publisher make       *
 * no warranty of any kind, expressed or implied, with regard to these    *
 * programs or to the documentation contained in these books. The authors *
 * and publisher shall not be liable in any event for incidental or       *
 * consequential damages in connection with, or arising out of, the       *
 * furnishing, performance, or use of these programs.                     *
 **************************************************************************/
