// fig02_01.c
// A first program in C.
#include <stdio.h>

// function main begins program execution 
int main(void) {
   printf("Welcome to C!\n");
    
    int length = 0;
    int width = 0;
    int area=0;
    int scanfReturn=0;
    double average =0;
    
    puts("Enter the length");
    scanfReturn = scanf("%d", &length);
    if (scanfReturn==0){
        puts("Invalid input");
    }
    else{
        
        puts("Enter the width");
        scanfReturn = scanf("%d", &width);
        if (scanfReturn==0){
            puts("Invalid input");
        }
        
        else{
            
            
            area = width * length;
            
            printf("Length: %d, width: %d,  and area is %d\n", length, width, area);
            
            average = (double)length / width;
            
            printf("The average is %.1f\n",average);
            
        }
    }

    
    return 0;
} // end function main 



/**************************************************************************
 * (C) Copyright 1992-2021 by Deitel & Associates, Inc. and               *
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
 *************************************************************************/
